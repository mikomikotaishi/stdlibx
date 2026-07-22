#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class UriSyntaxException
     * @brief Exception class for URI syntax errors.
     * @extends Exception
     *
     * The UriSyntaxException class is used to represent an exception that occurs when a URI has invalid syntax.
     */
    class UriSyntaxException: public Exception {
    public:
        explicit UriSyntaxException(const String& msg = ""):
            Exception(), msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return msg.c_str();
        }
    private:
        String msg; ///< The human-readable description of the syntax error.
    };

    /**
     * @class Uri
     * @brief A parsed Uniform Resource Identifier (RFC 3986).
     *
     * Represents the generic syntax
     * `scheme ":" ["//" authority] path ["?" query] ["#" fragment]`.
     * A single value type covers every URI - hierarchical URLs, URNs, and
     * relative references alike; "URL"/"URN" are roles queried via
     * @ref has_authority / @ref is_urn, not distinct types. Construction is
     * purely syntactic (no network access); use a Uri-consuming client to
     * actually resolve or fetch a resource.
     */
    class [[nodiscard]] Uri {
    private:
        Optional<String> _scheme; ///< The scheme, without the trailing ':' (empty if this is a relative reference).
        Optional<String> _userinfo; ///< The userinfo, without the trailing '@'.
        String _host; ///< The host, with any IPv6 brackets stripped.
        String _path; ///< The path (always present, possibly empty).
        Optional<String> _query; ///< The query, without the leading '?'.
        Optional<String> _fragment; ///< The fragment, without the leading '#'.
        Optional<u16> _port; ///< The port, if an explicit non-empty one was given.
        bool _has_authority = false; ///< Whether an "//authority" component is present (it may still be empty).

        /**
         * @brief Case-insensitive ASCII string comparison.
         * @param a The first string to compare.
         * @param b The second string to compare.
         * @return true if @p a and @p b are equal ignoring ASCII case, false otherwise.
         */
        [[nodiscard]]
        static bool ascii_ci_equal(StringView a, StringView b) noexcept {
            if (a.size() != b.size()) {
                return false;
            }
            for (usize i = 0; i < a.size(); ++i) {
                char ca = a[i];
                char cb = b[i];
                if (ca >= 'A' && ca <= 'Z') {
                    ca = static_cast<char>(ca + 32);
                }
                if (cb >= 'A' && cb <= 'Z') {
                    cb = static_cast<char>(cb + 32);
                }
                if (ca != cb) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Whether @p s is a syntactically valid scheme: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ).
         * @param s The scheme to validate.
         * @return true if @p s is a valid scheme, false otherwise.
         */
        [[nodiscard]]
        static bool valid_scheme(StringView s) noexcept {
            if (s.empty()) {
                return false;
            }
            const char first = s.front();
            if (!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z'))) {
                return false;
            }
            for (const char c: s) {
                const bool ok = (c >= 'a' && c <= 'z')
                    || (c >= 'A' && c <= 'Z')
                    || (c >= '0' && c <= '9')
                    || c == '+'
                    || c == '-'
                    || c == '.';
                if (!ok) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief The numeric value of a hexadecimal digit.
         * @param c The character to read.
         * @return The value 0-15, or an empty Optional if @p c is not a hex digit.
         */
        [[nodiscard]]
        static Optional<u8> hex_value(char c) noexcept {
            if (c >= '0' && c <= '9') {
                return static_cast<u8>(c - '0');
            }
            if (c >= 'a' && c <= 'f') {
                return static_cast<u8>(c - 'a' + 10);
            }
            if (c >= 'A' && c <= 'F') {
                return static_cast<u8>(c - 'A' + 10);
            }
            return nullopt;
        }

        /**
         * @brief Whether every '%' in @p text introduces a "%HH" triplet.
         * @param text The component to check.
         * @return true if the percent-encoding is well formed, false otherwise.
         */
        [[nodiscard]]
        static bool valid_percent_encoding(StringView text) noexcept {
            for (usize index = 0; index < text.size(); ++index) {
                if (text[index] != '%') {
                    continue;
                }
                if (index + 2 >= text.size()) {
                    return false;
                }
                if (!hex_value(text[index + 1]) || !hex_value(text[index + 2])) {
                    return false;
                }
                index += 2;
            }
            return true;
        }

        /**
         * @brief Rejects a component whose percent-encoding is malformed.
         * @param component The component text.
         * @param what The component's name, for the message.
         * @throws UriSyntaxException if @p component contains a '%' not followed by two hex digits.
         */
        THROWS(UriSyntaxException)
        static void require_valid_encoding(StringView component, StringView what) {
            if (!valid_percent_encoding(component)) {
                throw UriSyntaxException(Ops::fmt("malformed percent-encoding in {}", what));
            }
        }
    public:
        Uri() = default;

        THROWS(UriSyntaxException)
        explicit Uri(StringView uri) {
            StringView s = uri;

            // RFC 3986 Appendix B decomposition, peeled right-to-left so each
            // delimiter is unambiguous by the time we look for it.
            if (const auto hash = s.find('#'); hash != StringView::npos) {
                this->_fragment = String(s.substr(hash + 1));
                s = s.substr(0, hash);
            }
            if (const auto question = s.find('?'); question != StringView::npos) {
                this->_query = String(s.substr(question + 1));
                s = s.substr(0, question);
            }

            // scheme: a valid scheme followed by ':' occurring before any '/'.
            if (const auto colon = s.find(':'); colon != StringView::npos) {
                const auto slash = s.find('/');
                const StringView candidate = s.substr(0, colon);
                if ((slash == StringView::npos || colon < slash) && valid_scheme(candidate)) {
                    _scheme = String(candidate);
                    s = s.substr(colon + 1);
                }
            }

            // authority: introduced by "//", runs to the next '/' (or the end).
            if (s.size() >= 2 && s[0] == '/' && s[1] == '/') {
                this->_has_authority = true;
                s = s.substr(2);
                const auto slash = s.find('/');
                StringView authority = (slash == StringView::npos) ? s : s.substr(0, slash);
                s = (slash == StringView::npos) ? ""sv : s.substr(slash);

                // The *last* '@', not the first. RFC 3986 keeps '@' out of
                // userinfo, so a conforming authority holds only one - but a
                // password with an unescaped '@' is a routine mistake, and
                // splitting on the first would silently take part of it as the
                // host. Browsers and Java's URI both split on the last, and
                // reading a host differently from every other client is how a
                // link that looks like one server reaches another.
                if (const auto at = authority.rfind('@'); at != StringView::npos) {
                    this->_userinfo = String(authority.substr(0, at));
                    authority = authority.substr(at + 1);
                }

                StringView port_text;
                bool has_port = false;
                if (!authority.empty() && authority.front() == '[') {
                    // IP-literal (e.g. IPv6): host is bracketed and may itself contain ':'.
                    const auto close = authority.find(']');
                    if (close == StringView::npos) {
                        throw UriSyntaxException("unterminated IPv6 literal");
                    }
                    this->_host = String(authority.substr(1, close - 1));
                    const StringView after = authority.substr(close + 1);
                    if (!after.empty()) {
                        if (after.front() != ':') {
                            throw UriSyntaxException("unexpected characters after IPv6 literal");
                        }
                        port_text = after.substr(1);
                        has_port = true;
                    }
                } else if (const auto colon = authority.find(':'); colon != StringView::npos) {
                    this->_host = String(authority.substr(0, colon));
                    port_text = authority.substr(colon + 1);
                    has_port = true;
                } else {
                    this->_host = String(authority);
                }

                // An empty port ("host:") is permitted and means "no explicit port".
                if (has_port && !port_text.empty()) {
                    u32 value = 0;
                    for (const char c: port_text) {
                        if (c < '0' || c > '9') {
                            throw UriSyntaxException("port number contains non-digit");
                        }
                        value = value * 10 + static_cast<u32>(c - '0');
                        if (value > 65535u) {
                            throw UriSyntaxException("port number out of range");
                        }
                    }
                    this->_port = static_cast<u16>(value);
                }
            }

            this->_path = String(s);

            // Checked after the split and never before it: decoding first would
            // manufacture delimiters out of "%2F" and "%3F" and change where the
            // components divide, which is how two parsers come to disagree about
            // the same URI. Validating here means every component of a
            // constructed Uri is known decodable, so the decoding accessors
            // cannot fail.
            if (this->_userinfo) {
                require_valid_encoding(*this->_userinfo, "userinfo");
            }
            require_valid_encoding(this->_host, "host");
            require_valid_encoding(this->_path, "path");
            if (this->_query) {
                require_valid_encoding(*this->_query, "query");
            }
            if (this->_fragment) {
                require_valid_encoding(*this->_fragment, "fragment");
            }
        }

        /**
         * @brief Resolves the percent-escapes in @p text.
         * @param text The text to decode.
         * @return The decoded bytes as a String.
         *
         * A '%' that does not introduce two hex digits is left as it stands
         * rather than raising, so this is safe to call on arbitrary text. It
         * cannot arise from a component of a constructed Uri, which validates
         * its encoding up front.
         *
         * The result is bytes, not characters: RFC 3986 percent-encodes octets,
         * so decoding UTF-8 escapes yields UTF-8, and decoding escapes from some
         * other encoding yields that. Nothing here transcodes.
         */
        [[nodiscard]]
        static String percent_decode(StringView text) {
            String out;
            out.reserve(text.size());
            for (usize index = 0; index < text.size(); ++index) {
                const char c = text[index];
                if (c != '%' || index + 2 >= text.size()) {
                    out += c;
                    continue;
                }
                const Optional<u8> high = hex_value(text[index + 1]);
                const Optional<u8> low = hex_value(text[index + 2]);
                if (!high || !low) {
                    out += c;
                    continue;
                }
                out += static_cast<char>((*high << 4) | *low);
                index += 2;
            }
            return out;
        }

        /**
         * @brief Parse a string into a Uri.
         * @param uri The text to parse.
         * @return The parsed Uri, or an empty Optional if @p uri is malformed.
         */
        [[nodiscard]]
        static Optional<Uri> parse(StringView uri) noexcept {
            try {
                return Uri(uri);
            } catch (const UriSyntaxException& _) {
                return nullopt;
            }
        }

        /**
         * @brief The scheme, without the trailing ':'.
         * @return The scheme, or an empty view for a relative reference.
         */
        [[nodiscard]]
        StringView scheme() const noexcept {
            if (!_scheme) {
                return ""sv;
            }
            return StringView(*_scheme);
        }

        /**
         * @brief Whether an "//authority" component is present.
         * @return true if an authority is present (it may still be empty), false otherwise.
         */
        [[nodiscard]]
        bool has_authority() const noexcept {
            return _has_authority;
        }

        /**
         * @brief The userinfo, without the trailing '@'.
         * @return The userinfo, or an empty view if absent.
         */
        [[nodiscard]]
        StringView userinfo() const noexcept {
            if (!_userinfo) {
                return ""sv;
            }
            return StringView(*_userinfo);
        }

        /**
         * @brief The userinfo with its percent-escapes resolved.
         * @return The decoded userinfo, or an empty String if absent.
         *
         * This is the form to authenticate with: a password written into a URI
         * has to escape ':', '@', '/' and '?', so the raw @ref userinfo is
         * rarely the credential itself. Split on the first ':' *after* decoding
         * only if the password cannot contain one - otherwise split the raw
         * userinfo first and decode the halves separately.
         */
        [[nodiscard]]
        String decoded_userinfo() const {
            if (!_userinfo) {
                return ""s;
            }
            return percent_decode(*_userinfo);
        }

        /**
         * @brief The host, with any IPv6 brackets stripped.
         * @return The host, or an empty view if there is no authority.
         */
        [[nodiscard]]
        StringView host() const noexcept {
            return StringView(_host);
        }

        /**
         * @brief The explicit port, if one was given.
         * @return An Optional containing the port number, or an empty Optional if no explicit port was given.
         */
        [[nodiscard]]
        Optional<u16> port() const noexcept {
            return _port;
        }

        /**
         * @brief The path (always present, possibly empty).
         * @return The path.
         */
        [[nodiscard]]
        StringView path() const noexcept {
            return StringView(_path);
        }

        /**
         * @brief The path with its percent-escapes resolved.
         * @return The decoded path.
         *
         * Decodes the path as one piece, so an escaped '/' ("%2F") becomes an
         * ordinary '/' and stops being distinguishable from a segment boundary.
         * Split the raw @ref path on '/' first and decode each segment where
         * that distinction matters - a file name holding a slash, for instance.
         */
        [[nodiscard]]
        String decoded_path() const {
            return percent_decode(_path);
        }

        /**
         * @brief The query, without the leading '?'.
         * @return The query, or an empty Optional if no '?' was present.
         *
         * Returned raw, and deliberately without a decoding counterpart. A query
         * is a structure, not a string: decoding it whole would turn an escaped
         * '&' or '=' inside a value into a separator and lose the boundary it was
         * escaped to protect. Split on '&' and '=' first, then run
         * @ref percent_decode over each name and value. Note too that a '+'
         * means a space only under form encoding, which RFC 3986 does not define
         * and @ref percent_decode does not assume.
         */
        [[nodiscard]]
        Optional<StringView> query() const noexcept {
            if (!_query) {
                return nullopt;
            }
            return StringView(*_query);
        }

        /**
         * @brief The fragment, without the leading '#'.
         * @return The fragment, or an empty Optional if no '#' was present.
         */
        [[nodiscard]]
        Optional<StringView> fragment() const noexcept {
            if (!_fragment) {
                return nullopt;
            }
            return StringView(*_fragment);
        }

        /**
         * @brief Whether this URI has a scheme.
         * @return true if a scheme is present (RFC 3986 calls this a URI, as
         * opposed to a relative reference). Note this is "has a scheme", not
         * RFC 3986's stricter "absolute-URI" (which also forbids a fragment).
         */
        [[nodiscard]]
        bool is_absolute() const noexcept {
            return _scheme.has_value();
        }

        /**
         * @brief Whether this URI uses the "urn" scheme.
         * @return true if the scheme is "urn" (case-insensitive), false otherwise.
         */
        [[nodiscard]]
        bool is_urn() const noexcept {
            return _scheme && ascii_ci_equal(*_scheme, "urn");
        }

        /**
         * @brief Serialize this URI back to its string form.
         * @return The reassembled URI text.
         */
        [[nodiscard]]
        String to_string() const {
            String out;
            if (_scheme) {
                out += Ops::fmt("{}{}", *_scheme, ':');
            }
            if (_has_authority) {
                out += "//";
                if (_userinfo) {
                    out += Ops::fmt("{}{}", *_userinfo, '@');
                }
                if (_host.find(':') != String::npos) {
                    out += Ops::fmt("[{}]", _host);
                } else {
                    out += _host;
                }
                if (_port) {
                    out += Ops::fmt("{}{}", ':', *_port);
                }
            }
            out += _path;
            if (_query) {
                out += Ops::fmt("{}{}", '?', *_query);
            }
            if (_fragment) {
                out += Ops::fmt("{}{}", '#', *_fragment);
            }
            return out;
        }

        /**
         * @brief Component-wise equality.
         *
         * Compares the parsed components exactly; this is syntactic equality,
         * not RFC 3986 normalized equivalence (e.g. scheme/host case and
         * percent-encoding are not normalized before comparison).
         */
        [[nodiscard]]
        bool operator==(const Uri& other) const = default;
    };
}

using stdx::net::Uri;

export namespace stdx::literals::inline net_literals {
    [[nodiscard]]
    inline Uri operator""_uri(const char* s, usize len) {
        return Uri(StringView(s, len));
    }
}

namespace stdx::fmt {
    template <>
    struct Formatter<Uri> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const Uri& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<Uri> : public stdx::fmt::Formatter<Uri> {};
