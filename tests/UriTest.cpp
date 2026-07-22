import stdx;

using stdx::net::Uri;
using stdx::net::UriSyntaxException;

using namespace stdx::test;

void test_uri_full() {
    Uri u{"http://user@example.com:8080/path?q=1#frag"};
    expect(u.is_absolute(), "has a scheme");
    expect_eq(u.scheme(), "http", "scheme is http");
    expect(u.has_authority(), "has an authority");
    expect_eq(u.userinfo(), "user", "userinfo is user");
    expect_eq(u.host(), "example.com", "host is example.com");
    expect(u.port() == 8080, "port is 8080");
    expect_eq(u.path(), "/path", "path is /path");

    Optional<StringView> q = u.query();
    expect(q.has_value() && *q == "q=1", "query is q=1");
    Optional<StringView> f = u.fragment();
    expect(f.has_value() && *f == "frag", "fragment is frag");
    expect(!u.is_urn(), "http is not a urn");
}

void test_uri_urn() {
    Uri u{"urn:isbn:0451450523"};
    expect(u.is_urn(), "urn:isbn:... is a urn");
    expect(u.is_absolute(), "urn has a scheme");
    expect_eq(u.scheme(), "urn", "scheme is urn");
    expect(!u.has_authority(), "urn has no authority");
    expect_eq(u.path(), "isbn:0451450523", "urn path is the NSS");
    expect(!u.query().has_value(), "urn has no query");
}

void test_uri_relative() {
    Uri u{"/path/to?x=y"};
    expect(!u.is_absolute(), "relative reference has no scheme");
    expect(!u.has_authority(), "relative reference has no authority");
    expect_eq(u.path(), "/path/to", "relative path preserved");
    Optional<StringView> q = u.query();
    expect(q.has_value() && *q == "x=y", "relative query preserved");
}

void test_uri_ipv6() {
    Uri u{"http://[::1]:80/index"};
    expect(u.has_authority(), "ipv6 uri has authority");
    expect_eq(u.host(), "::1", "ipv6 host is unbracketed");
    expect(u.port() == 80, "ipv6 port is 80");
    expect_eq(u.path(), "/index", "ipv6 path preserved");
}

void test_uri_special_forms() {
    // mailto: has no authority - the whole remainder is the path.
    Uri mail{"mailto:user@example.com"};
    expect_eq(mail.scheme(), "mailto", "mailto scheme");
    expect(!mail.has_authority(), "mailto has no authority");
    expect_eq(mail.path(), "user@example.com", "mailto path is the address");

    // file:// with an empty authority.
    Uri file{"file:///etc/hosts"};
    expect(file.has_authority(), "file uri has an (empty) authority");
    expect(file.host().empty(), "file authority host is empty");
    expect_eq(file.path(), "/etc/hosts", "file path preserved");

    // An empty port ("host:") is allowed and means "no explicit port".
    Uri empty_port{"http://host:/"};
    expect(!empty_port.port().has_value(), "empty port parses as no port");
    expect_eq(empty_port.host(), "host", "host parsed despite empty port");
}

void test_uri_roundtrip() {
    static constexpr Array<StringView, 5> CASES = {{
        "http://user@example.com:8080/path?q=1#frag",
        "urn:isbn:0451450523",
        "/path/to?x=y",
        "http://[::1]:80/index",
        "file:///etc/hosts",
    }};
    for (const StringView c: CASES) {
        expect_eq(StringView(Uri(c).to_string()), c, c);
    }
}

void test_uri_malformed() {
    expect_throws<UriSyntaxException>(
        [] -> void { (void)Uri("http://[::1/"); }, "unterminated IPv6 literal is rejected"
    );
    expect_throws<UriSyntaxException>(
        [] -> void { (void)Uri("http://host:99999/"); }, "out-of-range port is rejected"
    );
    expect_throws<UriSyntaxException>(
        [] -> void { (void)Uri("http://host:80x/"); }, "non-numeric port is rejected"
    );
    // parse reports the same failures without throwing.
    expect(!Uri::parse("http://[::1/").has_value(), "parse returns empty on malformed input");
    expect(Uri::parse("http://ok.example/").has_value(), "parse returns a value on valid input");
}

void test_uri_equality() {
    expect(Uri("http://a/b") == Uri("http://a/b"), "identical URIs compare equal");
    expect(!(Uri("http://a/b") == Uri("http://a/c")), "differing paths compare unequal");
    expect(!(Uri("http://a/b") == Uri("https://a/b")), "differing schemes compare unequal");
}

/**
 * @brief Tests percent-decoding, and that components are stored as written.
 *
 * The split has to happen before any decoding: were it the other way round, a
 * "%2F" in a path or a "%3F" before a query would become a real delimiter and
 * move where the components divide. Keeping the stored form raw is also what
 * makes to_string round-trip, since nothing records which characters were
 * escaped originally.
 */
void test_uri_percent_decoding() {
    Uri u{"http://us%3Aer:p%40ss%3B1@example.com/a%20b/c%2Fd?q=%26x#f%23g"};

    expect_eq(u.userinfo(), "us%3Aer:p%40ss%3B1", "userinfo is kept as written");
    expect_eq(u.decoded_userinfo(), "us:er:p@ss;1", "userinfo decodes to the real credential");
    expect_eq(u.host(), "example.com", "an escaped ':' in userinfo does not disturb the host");
    expect_eq(u.path(), "/a%20b/c%2Fd", "path is kept as written");
    expect_eq(u.decoded_path(), "/a b/c/d", "path decodes, escaped slash included");
    expect_eq(*u.query(), "q=%26x", "query is returned raw, with no decoding counterpart");

    expect_eq(Uri::percent_decode("%41%42%43"), "ABC", "uppercase hex digits decode");
    expect_eq(Uri::percent_decode("%61%62%63"), "abc", "lowercase hex digits decode");
    expect_eq(Uri::percent_decode("no escapes"), "no escapes", "text without '%' is unchanged");
    expect_eq(Uri::percent_decode("100%"), "100%", "a trailing '%' is left alone rather than raising");
    expect_eq(Uri::percent_decode("%C3%A9"), "\xc3\xa9", "decoding yields bytes, not transcoded characters");

    // Every '%' must introduce two hex digits (RFC 3986 2.1); a Uri that parsed
    // would otherwise hold a component its own decoder could not read.
    expect(!Uri::parse("http://a/100%").has_value(), "a truncated escape is rejected");
    expect(!Uri::parse("http://a/%zz").has_value(), "a non-hex escape is rejected");
    expect(!Uri::parse("http://a/?q=%1").has_value(), "a short escape in the query is rejected");
    expect(!Uri::parse("http://us%er@a/").has_value(), "a bad escape in userinfo is rejected");
}

/**
 * @brief Tests that the authority splits on the last '@', not the first.
 *
 * RFC 3986 keeps '@' out of userinfo, so a conforming authority holds one - but
 * an unescaped '@' in a password is a common mistake, and splitting on the first
 * would hand back part of the password as the host. Browsers and Java's URI both
 * take the last; reading a host differently from every other client is how a
 * link that looks like one server reaches another.
 */
void test_uri_multiple_at_signs() {
    Uri u{"http://user:pa@ss@example.com/path"};
    expect_eq(u.host(), "example.com", "the host is what follows the last '@'");
    expect_eq(u.userinfo(), "user:pa@ss", "everything before the last '@' is userinfo");

    Uri spoof{"http://good.com@evil.com/"};
    expect_eq(spoof.host(), "evil.com", "a single '@' still leaves the host after it");

    Uri chained{"http://good.com@evil.com@real.com/"};
    expect_eq(chained.host(), "real.com", "with several, the host is the final segment");

    expect_eq(Uri("http://user@host/").userinfo(), "user", "the ordinary single-'@' case is unchanged");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Uri.full", test_uri_full},
        {"Uri.percent_decoding", test_uri_percent_decoding},
        {"Uri.multiple_at_signs", test_uri_multiple_at_signs},
        {"Uri.urn", test_uri_urn},
        {"Uri.relative", test_uri_relative},
        {"Uri.ipv6", test_uri_ipv6},
        {"Uri.special_forms", test_uri_special_forms},
        {"Uri.roundtrip", test_uri_roundtrip},
        {"Uri.malformed", test_uri_malformed},
        {"Uri.equality", test_uri_equality},
    });
}
