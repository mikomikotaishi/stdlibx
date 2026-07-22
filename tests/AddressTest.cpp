import stdx;

using stdx::collections::HashMap;
using stdx::collections::HashSet;
using stdx::net::AddressSyntaxException;
using stdx::net::Endpoint;
using stdx::net::IPAddress;
using stdx::net::IPv4Address;
using stdx::net::IPv6Address;

using namespace stdx::test;

// Parsing is usable at compile time, which is the point of keeping the address
// types free of any OS dependency.
static_assert(*IPv4Address::parse("192.168.0.1") == IPv4Address(192, 168, 0, 1));
static_assert(!IPv4Address::parse("192.168.0.256").has_value());
static_assert(*IPv6Address::parse("::1") == IPv6Address::LOOPBACK);
static_assert(IPv6Address::parse("2001:db8::1")->groups()[0] == 0x2001);
static_assert(Endpoint::parse("[::1]:443")->port() == 443);

void test_ipv4_construction() {
    const IPv4Address address{10, 0, 0, 42};
    expect_eq(address.host_order(), 0x0A00002Au, "host order packs the leading octet high");
    expect_eq(address[0], 10, "index 0 is the leading octet");
    expect_eq(address[3], 42, "index 3 is the trailing octet");
    expect(address.octets() == Array<u8, 4>{10, 0, 0, 42}, "octets are in network order");
    expect_eq(IPv4Address(0x0A00002Au), address, "the u32 ctor agrees with the octet ctor");
    expect_eq(IPv4Address(), IPv4Address::ANY, "a default address is 0.0.0.0");
}

void test_ipv4_predicates() {
    expect(IPv4Address::ANY.is_unspecified(), "0.0.0.0 is unspecified");
    expect(IPv4Address::LOOPBACK.is_loopback(), "127.0.0.1 is loopback");
    expect(IPv4Address(127, 1, 2, 3).is_loopback(), "all of 127/8 is loopback");
    expect(IPv4Address::BROADCAST.is_broadcast(), "255.255.255.255 is broadcast");

    expect(IPv4Address(10, 1, 1, 1).is_private(), "10/8 is private");
    expect(IPv4Address(172, 16, 0, 1).is_private(), "172.16/12 starts at 172.16");
    expect(IPv4Address(172, 31, 255, 255).is_private(), "172.16/12 ends at 172.31");
    expect(!IPv4Address(172, 32, 0, 1).is_private(), "172.32 is outside 172.16/12");
    expect(IPv4Address(192, 168, 1, 1).is_private(), "192.168/16 is private");
    expect(!IPv4Address(8, 8, 8, 8).is_private(), "8.8.8.8 is public");

    expect(IPv4Address(169, 254, 0, 1).is_link_local(), "169.254/16 is link-local");
    expect(IPv4Address(224, 0, 0, 1).is_multicast(), "224.0.0.1 is multicast");
    expect(IPv4Address(239, 255, 255, 255).is_multicast(), "239.255.255.255 is multicast");
    expect(!IPv4Address(240, 0, 0, 1).is_multicast(), "240/4 is past the multicast range");
}

void test_ipv4_parse() {
    expect_eq(*IPv4Address::parse("0.0.0.0"), IPv4Address::ANY, "all-zero literal");
    expect_eq(*IPv4Address::parse("255.255.255.255"), IPv4Address::BROADCAST, "all-ones literal");
    expect_eq(*IPv4Address::parse("127.0.0.1"), IPv4Address::LOOPBACK, "loopback literal");

    // Non-canonical and short forms are rejected: a resolver that reads "010" as
    // octal and one that reads it as decimal disagree about the destination.
    expect(!IPv4Address::parse("010.1.1.1").has_value(), "leading zeros are rejected");
    expect(!IPv4Address::parse("1.1.1.01").has_value(), "trailing leading-zero octet is rejected");
    expect(!IPv4Address::parse("10.1").has_value(), "the two-part short form is rejected");
    expect(!IPv4Address::parse("1.2.3.4.5").has_value(), "a fifth octet is rejected");
    expect(!IPv4Address::parse("1.2.3").has_value(), "a missing octet is rejected");
    expect(!IPv4Address::parse("1.2.3.").has_value(), "a trailing dot is rejected");
    expect(!IPv4Address::parse(".1.2.3").has_value(), "a leading dot is rejected");
    expect(!IPv4Address::parse("1.2.3.256").has_value(), "an out-of-range octet is rejected");
    expect(!IPv4Address::parse("0x7f.0.0.1").has_value(), "hexadecimal octets are rejected");
    expect(!IPv4Address::parse("1.2.3.4 ").has_value(), "trailing whitespace is rejected");
    expect(!IPv4Address::parse("").has_value(), "the empty string is rejected");

    expect_throws<AddressSyntaxException>(
        [] -> void { (void)IPv4Address("1.2.3.4.5"); }, "the ctor throws on a malformed literal"
    );
    expect_no_throw([] -> void { (void)IPv4Address("1.2.3.4"); }, "the ctor accepts a valid literal");
}

void test_ipv4_to_string() {
    expect_eq(IPv4Address(192, 168, 0, 1).to_string(), "192.168.0.1", "dotted-quad form");
    expect_eq(IPv4Address::ANY.to_string(), "0.0.0.0", "unspecified form");
    expect_eq(IPv4Address::BROADCAST.to_string(), "255.255.255.255", "broadcast form");
    expect_eq(Ops::fmt("{}", IPv4Address(8, 8, 4, 4)), "8.8.4.4", "the formatter prints the address");
}

void test_ipv6_construction() {
    const IPv6Address address{0x2001, 0xdb8, 0, 0, 0, 0, 0, 1};
    expect(address.groups() == Array<u16, 8>{0x2001, 0xdb8, 0, 0, 0, 0, 0, 1}, "groups are host order");
    expect_eq(address[0], 0x20, "index 0 is the leading byte");
    expect_eq(address[1], 0x01, "index 1 is the second byte");
    expect_eq(address[15], 1, "index 15 is the trailing byte");
    expect_eq(address.scope_id(), 0u, "an unscoped address has zone 0");
    expect_eq(IPv6Address(), IPv6Address::ANY, "a default address is ::");
    expect_eq(address.with_scope_id(7).scope_id(), 7u, "with_scope_id sets the zone");
    expect_ne(address, address.with_scope_id(7), "the zone participates in equality");
}

void test_ipv6_predicates() {
    expect(IPv6Address::ANY.is_unspecified(), ":: is unspecified");
    expect(!IPv6Address::LOOPBACK.is_unspecified(), "::1 is not unspecified");
    expect(IPv6Address::LOOPBACK.is_loopback(), "::1 is loopback");
    expect(!IPv6Address(0, 0, 0, 0, 0, 0, 0, 2).is_loopback(), "::2 is not loopback");

    expect(IPv6Address::parse("ff02::1")->is_multicast(), "ff00::/8 is multicast");
    expect(IPv6Address::parse("fe80::1")->is_link_local(), "fe80::/10 is link-local");
    expect(IPv6Address::parse("febf::1")->is_link_local(), "fe80::/10 ends at febf");
    expect(!IPv6Address::parse("fec0::1")->is_link_local(), "fec0:: is outside fe80::/10");
    expect(IPv6Address::parse("fd00::1")->is_unique_local(), "fd00:: is unique-local");
    expect(IPv6Address::parse("fc00::1")->is_unique_local(), "fc00::/7 starts at fc00");
    expect(!IPv6Address::parse("2001:db8::1")->is_unique_local(), "2001:db8:: is global");
}

void test_ipv6_parse() {
    expect_eq(*IPv6Address::parse("::"), IPv6Address::ANY, ":: is the unspecified address");
    expect_eq(*IPv6Address::parse("::1"), IPv6Address::LOOPBACK, "::1 is loopback");
    expect_eq(
        *IPv6Address::parse("2001:0db8:0000:0000:0000:0000:0000:0001"),
        IPv6Address(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1),
        "the fully expanded form"
    );
    expect_eq(
        *IPv6Address::parse("2001:db8::1"),
        IPv6Address(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1),
        "the compressed form means the same address"
    );
    expect_eq(
        *IPv6Address::parse("2001:db8::"),
        IPv6Address(0x2001, 0xdb8, 0, 0, 0, 0, 0, 0),
        "a trailing :: elides the tail"
    );
    expect_eq(
        *IPv6Address::parse("1:2:3:4:5:6:7:8"),
        IPv6Address(1, 2, 3, 4, 5, 6, 7, 8),
        "eight explicit groups"
    );
    expect_eq(
        *IPv6Address::parse("ABCD::EF"),
        IPv6Address(0xabcd, 0, 0, 0, 0, 0, 0, 0xef),
        "hex digits are case-insensitive"
    );

    // An embedded IPv4 literal occupies the final two groups.
    expect_eq(
        *IPv6Address::parse("::ffff:192.168.0.1"),
        IPv4Address(192, 168, 0, 1).to_ipv6_mapped(),
        "the IPv4-mapped form"
    );
    expect_eq(
        *IPv6Address::parse("64:ff9b::1.2.3.4"),
        IPv6Address(0x64, 0xff9b, 0, 0, 0, 0, 0x0102, 0x0304),
        "an embedded IPv4 literal after a compressed run"
    );

    expect_eq(IPv6Address::parse("fe80::1%3")->scope_id(), 3u, "a numeric zone index is parsed");
    expect_eq(
        *IPv6Address::parse("fe80::1%3"),
        IPv6Address(0xfe80, 0, 0, 0, 0, 0, 0, 1).with_scope_id(3),
        "the zone is kept alongside the bytes"
    );

    expect(!IPv6Address::parse("1:2:3:4:5:6:7:8:9").has_value(), "nine groups are rejected");
    expect(!IPv6Address::parse("1:2:3:4:5:6:7").has_value(), "seven groups without :: are rejected");
    expect(!IPv6Address::parse("1::2::3").has_value(), "a second :: is rejected");
    expect(!IPv6Address::parse(":1:2:3:4:5:6:7:8").has_value(), "a single leading colon is rejected");
    expect(!IPv6Address::parse("1:2:3:4:5:6:7:8:").has_value(), "a single trailing colon is rejected");
    expect(!IPv6Address::parse("12345::1").has_value(), "a five-digit group is rejected");
    expect(!IPv6Address::parse("1:2:3:4:5:6:7:8::").has_value(), ":: eliding nothing is rejected");
    expect(!IPv6Address::parse("::ffff:192.168.0.256").has_value(), "a bad embedded IPv4 is rejected");
    expect(!IPv6Address::parse("1:2:3:4:5:6:7:1.2.3.4").has_value(), "an overlong embedded IPv4 is rejected");
    expect(!IPv6Address::parse("fe80::1%").has_value(), "an empty zone is rejected");
    expect(!IPv6Address::parse("fe80::1%eth0").has_value(), "a named zone needs the OS, so it is rejected");
    expect(!IPv6Address::parse("::g").has_value(), "a non-hex digit is rejected");
    expect(!IPv6Address::parse("").has_value(), "the empty string is rejected");

    expect_throws<AddressSyntaxException>(
        [] -> void { (void)IPv6Address("1::2::3"); }, "the ctor throws on a malformed literal"
    );
    expect_no_throw([] -> void { (void)IPv6Address("2001:db8::1"); }, "the ctor accepts a valid literal");
}

void test_ipv6_to_string() {
    expect_eq(IPv6Address::ANY.to_string(), "::", "the unspecified address");
    expect_eq(IPv6Address::LOOPBACK.to_string(), "::1", "the loopback address");
    expect_eq(
        IPv6Address(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1).to_string(),
        "2001:db8::1",
        "leading zeros are stripped and the zero run is elided"
    );
    expect_eq(
        IPv6Address(1, 2, 3, 4, 5, 6, 7, 8).to_string(),
        "1:2:3:4:5:6:7:8",
        "an address with no zero run keeps every group"
    );
    expect_eq(
        IPv6Address(0x2001, 0xdb8, 0, 0, 0, 0, 0, 0).to_string(),
        "2001:db8::",
        "a trailing zero run is elided"
    );
    // RFC 5952: elide the longest run, and the leftmost of two equal runs.
    expect_eq(
        IPv6Address(1, 0, 0, 0, 2, 0, 0, 3).to_string(),
        "1::2:0:0:3",
        "the longest zero run is the one elided"
    );
    expect_eq(
        IPv6Address(1, 0, 0, 2, 3, 0, 0, 4).to_string(),
        "1::2:3:0:0:4",
        "the leftmost of two equal runs is elided"
    );
    expect_eq(
        IPv6Address(1, 0, 2, 0, 3, 0, 4, 0).to_string(),
        "1:0:2:0:3:0:4:0",
        "a single zero group is never elided"
    );
    expect_eq(
        IPv6Address(0xabcd, 0, 0, 0, 0, 0, 0, 0xef).to_string(),
        "abcd::ef",
        "hex digits print lowercase"
    );
    expect_eq(
        IPv4Address(192, 168, 0, 1).to_ipv6_mapped().to_string(),
        "::ffff:192.168.0.1",
        "an IPv4-mapped address prints in dotted-quad form"
    );
    expect_eq(
        IPv6Address(0xfe80, 0, 0, 0, 0, 0, 0, 1).with_scope_id(3).to_string(),
        "fe80::1%3",
        "a non-zero zone index is appended"
    );
    expect_eq(
        Ops::fmt("{}", IPv6Address::LOOPBACK),
        "::1",
        "the formatter prints the address"
    );
}

void test_ipv6_roundtrip() {
    constexpr StringView literals[] = {
        "::",
        "::1",
        "2001:db8::1",
        "2001:db8::",
        "1:2:3:4:5:6:7:8",
        "1::2:0:0:3",
        "abcd::ef",
        "::ffff:192.168.0.1",
        "fe80::1%3",
    };
    for (const StringView literal: literals) {
        const Optional<IPv6Address> address = IPv6Address::parse(literal);
        require(address.has_value(), Ops::fmt("{} parses", literal));
        expect_eq(address->to_string(), String(literal), "canonical text survives a round trip");
    }
}

void test_ipv4_ipv6_conversion() {
    const IPv4Address v4{192, 168, 0, 1};
    const IPv6Address mapped = v4.to_ipv6_mapped();
    expect(mapped.is_v4_mapped(), "the mapped address is in ::ffff:0:0/96");
    expect(!mapped.is_v4_compatible(), "a mapped address is not the deprecated compatible form");
    expect(mapped.to_ipv4() == v4, "the embedded address is recovered");

    const IPv6Address compatible = *IPv6Address::parse("::192.168.0.1");
    expect(compatible.is_v4_compatible(), "::a.b.c.d is the compatible form");
    expect(!compatible.is_v4_mapped(), "the compatible form is not mapped");
    expect(compatible.to_ipv4() == v4, "the compatible form also yields its IPv4 address");

    expect(!IPv6Address::LOOPBACK.to_ipv4().has_value(), "::1 embeds no IPv4 address");
    expect(!IPv6Address::ANY.to_ipv4().has_value(), ":: embeds no IPv4 address");
    expect(
        !IPv6Address::parse("2001:db8::1")->to_ipv4().has_value(),
        "a global address embeds no IPv4 address"
    );
}

void test_ip_address() {
    const IPAddress v4{IPv4Address(127, 0, 0, 1)};
    const IPAddress v6{IPv6Address::LOOPBACK};

    expect(v4.is_v4() && !v4.is_v6(), "an IPv4 address reports its family");
    expect(v6.is_v6() && !v6.is_v4(), "an IPv6 address reports its family");
    expect(v4.family() == IPAddress::Family::IPV4, "family() is IPV4");
    expect(v6.family() == IPAddress::Family::IPV6, "family() is IPV6");

    expect(v4.to_v4() == IPv4Address::LOOPBACK, "to_v4 yields the held address");
    expect(!v4.to_v6().has_value(), "to_v6 is empty for an IPv4 address");
    expect(v6.to_v6() == IPv6Address::LOOPBACK, "to_v6 yields the held address");
    expect(!v6.to_v4().has_value(), "to_v4 is empty for an IPv6 address");

    expect(v4.is_loopback() && v6.is_loopback(), "loopback is reported for both families");
    expect(IPAddress::any(IPAddress::Family::IPV4).is_unspecified(), "0.0.0.0 is unspecified");
    expect(IPAddress::any(IPAddress::Family::IPV6).is_unspecified(), ":: is unspecified");
    expect(IPAddress(IPv4Address(224, 0, 0, 1)).is_multicast(), "IPv4 multicast delegates");
    expect(IPAddress(*IPv6Address::parse("ff02::1")).is_multicast(), "IPv6 multicast delegates");
    expect(IPAddress(*IPv6Address::parse("fe80::1")).is_link_local(), "IPv6 link-local delegates");

    expect_eq(*IPAddress::parse("10.0.0.1"), IPAddress(IPv4Address(10, 0, 0, 1)), "parses IPv4");
    expect_eq(*IPAddress::parse("::1"), IPAddress(IPv6Address::LOOPBACK), "parses IPv6");
    expect(!IPAddress::parse("example.com").has_value(), "a host name is not a literal");
    expect(!IPAddress::parse("1.2.3.4:80").has_value(), "an endpoint is not an address");

    expect_eq(v4.to_string(), "127.0.0.1", "IPv4 text form");
    expect_eq(v6.to_string(), "::1", "IPv6 text form");
    expect_eq(Ops::fmt("{}", v6), "::1", "the formatter prints the held address");

    expect_eq(
        v4.to_v6_mapped(),
        IPv4Address::LOOPBACK.to_ipv6_mapped(),
        "an IPv4 address maps into IPv6 for a dual-stack socket"
    );
    expect_eq(v6.to_v6_mapped(), IPv6Address::LOOPBACK, "an IPv6 address maps to itself");

    // Ordering puts every IPv4 address before every IPv6 address.
    expect(IPAddress(IPv4Address::BROADCAST) < IPAddress(IPv6Address::ANY), "IPv4 sorts first");
    expect(IPAddress(IPv4Address(1, 2, 3, 4)) < IPAddress(IPv4Address(1, 2, 3, 5)), "IPv4 sorts by value");

    expect_throws<AddressSyntaxException>(
        [] -> void { (void)IPAddress("nonsense"); }, "the ctor throws on a malformed literal"
    );
}

void test_endpoint() {
    const Endpoint http{IPv4Address(93, 184, 216, 34), 80};
    expect_eq(http.port(), 80, "the port is kept in host order");
    expect_eq(http.address(), IPAddress(IPv4Address(93, 184, 216, 34)), "the address is kept");
    expect(http.family() == IPAddress::Family::IPV4, "family comes from the address");
    expect_eq(http.to_string(), "93.184.216.34:80", "IPv4 endpoints need no brackets");
    expect_eq(http.with_port(8080).port(), 8080, "with_port replaces the port");
    expect_eq(http.with_port(8080).address(), http.address(), "with_port keeps the address");

    const Endpoint tls{IPv6Address::LOOPBACK, 443};
    expect_eq(tls.to_string(), "[::1]:443", "IPv6 endpoints are bracketed");
    expect_eq(Ops::fmt("{}", tls), "[::1]:443", "the formatter prints the endpoint");
    expect_eq(Endpoint().port(), 0, "a default endpoint has port 0");

    expect_eq(*Endpoint::parse("127.0.0.1:8080"), Endpoint(IPv4Address::LOOPBACK, 8080), "parses IPv4");
    expect_eq(*Endpoint::parse("[::1]:443"), tls, "parses a bracketed IPv6 address");
    expect_eq(
        *Endpoint::parse("[fe80::1%3]:53"),
        Endpoint(IPv6Address(0xfe80, 0, 0, 0, 0, 0, 0, 1).with_scope_id(3), 53),
        "parses a scoped IPv6 address"
    );
    expect_eq(*Endpoint::parse("0.0.0.0:0"), Endpoint(), "parses the wildcard endpoint");

    expect(!Endpoint::parse("::1:443").has_value(), "an unbracketed IPv6 address is ambiguous");
    expect(!Endpoint::parse("127.0.0.1").has_value(), "the port is required");
    expect(!Endpoint::parse("127.0.0.1:").has_value(), "an empty port is rejected");
    expect(!Endpoint::parse("127.0.0.1:65536").has_value(), "an out-of-range port is rejected");
    expect(!Endpoint::parse("127.0.0.1:80x").has_value(), "a non-numeric port is rejected");
    expect(!Endpoint::parse("[::1]443").has_value(), "the colon after ] is required");
    expect(!Endpoint::parse("[::1]").has_value(), "a bracketed address still needs a port");
    expect(!Endpoint::parse("[::1:443").has_value(), "an unterminated bracket is rejected");
    expect(!Endpoint::parse("example.com:80").has_value(), "a host name is not an endpoint");
    expect(!Endpoint::parse("").has_value(), "the empty string is rejected");

    expect_throws<AddressSyntaxException>(
        [] -> void { (void)Endpoint("127.0.0.1"); }, "the ctor throws on a missing port"
    );

    // Endpoints sort by address, then by port.
    expect(Endpoint(IPv4Address::LOOPBACK, 80) < Endpoint(IPv4Address::LOOPBACK, 443), "port breaks ties");
    expect(Endpoint(IPv4Address(1, 1, 1, 1), 443) < Endpoint(IPv4Address(1, 1, 1, 2), 80), "address wins");
}

void test_hashing() {
    HashSet<IPAddress> addresses;
    addresses.insert(IPAddress(IPv4Address::LOOPBACK));
    addresses.insert(IPAddress(IPv4Address::LOOPBACK));
    addresses.insert(IPAddress(IPv6Address::LOOPBACK));
    expect_eq(addresses.size(), 2u, "equal addresses hash to one slot");

    HashMap<Endpoint, String> names;
    names[Endpoint(IPv4Address::LOOPBACK, 80)] = "http";
    names[Endpoint(IPv6Address::LOOPBACK, 443)] = "https";
    expect_eq(names.size(), 2u, "endpoints are usable as keys");
    expect_eq(names[Endpoint(IPv4Address::LOOPBACK, 80)], "http", "lookup finds the value");

    expect_eq(
        Hash<IPv6Address>()(IPv6Address::LOOPBACK),
        Hash<IPv6Address>()(IPv6Address::LOOPBACK),
        "hashing is stable"
    );
    expect_ne(
        Hash<IPv6Address>()(IPv6Address::LOOPBACK),
        Hash<IPv6Address>()(IPv6Address::LOOPBACK.with_scope_id(1)),
        "the zone index feeds the hash"
    );
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Address.ipv4_construction", test_ipv4_construction},
        {"Address.ipv4_predicates", test_ipv4_predicates},
        {"Address.ipv4_parse", test_ipv4_parse},
        {"Address.ipv4_to_string", test_ipv4_to_string},
        {"Address.ipv6_construction", test_ipv6_construction},
        {"Address.ipv6_predicates", test_ipv6_predicates},
        {"Address.ipv6_parse", test_ipv6_parse},
        {"Address.ipv6_to_string", test_ipv6_to_string},
        {"Address.ipv6_roundtrip", test_ipv6_roundtrip},
        {"Address.ipv4_ipv6_conversion", test_ipv4_ipv6_conversion},
        {"Address.ip_address", test_ip_address},
        {"Address.endpoint", test_endpoint},
        {"Address.hashing", test_hashing},
    });
}
