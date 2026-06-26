import stdx;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

constexpr u64 MSB = 0x0123456789ABCDEFull;
constexpr u64 LSB = 0xFEDCBA9876543210ull;

void test_nil_and_max() {
    expect(Uuid::NIL.is_nil(), "NIL is nil");
    expect(Uuid().is_nil(), "default-constructed UUID is nil");
    expect(Uuid::NIL == Uuid(), "default-constructed UUID equals NIL");
    expect(!Uuid::MAX.is_nil(), "MAX is not nil");

    expect_eq(Uuid::NIL.to_string(), "00000000-0000-0000-0000-000000000000", "NIL string form");
    expect_eq(Uuid::MAX.to_string(), "ffffffff-ffff-ffff-ffff-ffffffffffff", "MAX string form");

    expect_eq(Uuid::NIL.most_significant_bits(), 0, "NIL has zero msb");
    expect_eq(Uuid::NIL.least_significant_bits(), 0, "NIL has zero lsb");
    expect_eq(Uuid::MAX.most_significant_bits(), ~0ull, "MAX has all-ones msb");
    expect_eq(Uuid::MAX.least_significant_bits(), ~0ull, "MAX has all-ones lsb");
}

void test_from_bits() {
    Uuid u(MSB, LSB);

    expect_eq(u.most_significant_bits(), MSB, "msb round-trips");
    expect_eq(u.least_significant_bits(), LSB, "lsb round-trips");
    expect_eq(u.to_string(), "01234567-89ab-cdef-fedc-ba9876543210", "canonical string form");
    expect(!u.is_nil(), "a non-zero UUID is not nil");
}

void test_string_roundtrip() {
    static constexpr StringView CANONICAL = "01234567-89ab-cdef-fedc-ba9876543210";
    Optional<Uuid> parsed = Uuid::from_string(CANONICAL);
    require(parsed.has_value(), "the canonical string parses");
    expect_eq(parsed->to_string(), CANONICAL, "parse then format is the identity");
    expect_eq(parsed->most_significant_bits(), MSB, "parsed msb");
    expect_eq(parsed->least_significant_bits(), LSB, "parsed lsb");

    // Uppercase hex is accepted and normalized to lowercase on output.
    Optional<Uuid> upper = Uuid::from_string("01234567-89AB-CDEF-FEDC-BA9876543210");
    require(upper.has_value(), "an uppercase string parses");
    expect_eq(upper->to_string(), CANONICAL, "uppercase normalizes to lowercase");
    expect(*upper == *parsed, "uppercase and lowercase parse to the same UUID");
}

void test_from_string_invalid() {
    expect(!Uuid::from_string("").has_value(), "empty string is rejected");
    expect(!Uuid::from_string("not-a-uuid").has_value(), "a too-short string is rejected");
    expect(
        !Uuid::from_string("01234567-89ab-cdef-fedc-ba9876543210-extra").has_value(),
        "an over-long string is rejected"
    );
    // Right length (36), but the separator at index 8 is missing.
    expect(
        !Uuid::from_string("01234567x89ab-cdef-fedc-ba9876543210").has_value(),
        "a misplaced separator is rejected"
    );
    // Right shape, but a non-hex digit ('g') where a nibble is expected.
    expect(
        !Uuid::from_string("0123456g-89ab-cdef-fedc-ba9876543210").has_value(),
        "a non-hex digit is rejected"
    );
}

void test_random_uuid() {
    Uuid u = Uuid::random_uuid();
    expect(u.version() == 4, "random_uuid is version 4");
    expect(u.variant() == 2, "random_uuid uses the RFC 9562 variant");
    expect(!u.is_nil(), "random_uuid is not nil");

    Optional<Uuid> reparsed = Uuid::from_string(u.to_string());
    require(reparsed.has_value(), "a random UUID's string form parses");
    expect(*reparsed == u, "a random UUID round-trips through its string form");

    expect(Uuid::random_uuid() != Uuid::random_uuid(), "two random draws differ");
}

void test_timestamp_uuid() {
    const u64 before = System::current_time_millis();
    Uuid u = Uuid::timestamp_uuid();
    const u64 after = System::current_time_millis();

    expect(u.version() == 7, "timestamp_uuid is version 7");
    expect(u.variant() == 2, "timestamp_uuid uses the RFC 9562 variant");
    expect(u.timestamp() >= before && u.timestamp() <= after, "the embedded timestamp is the current wall-clock time");
    expect(!u.is_nil(), "timestamp_uuid is not nil");
}

void test_comparison_and_ordering() {
    Uuid a(1, 2);
    Uuid b(1, 2);
    Uuid c(1, 3);

    expect(a == b, "identical bit patterns compare equal");
    expect(a != c, "differing bit patterns compare unequal");
    expect(Uuid(0, 1) < Uuid(1, 0), "ordering compares the most-significant bits first");
    expect(Uuid::NIL < Uuid::MAX, "NIL orders before MAX");
}

void test_hash() {
    Uuid a(0xABCD, 0x1234);
    Uuid b(0xABCD, 0x1234);
    Uuid c(0xABCD, 0x1235);
    Hash<Uuid> hasher;

    expect_eq(hasher(a), hasher(b), "equal UUIDs hash equally");
    expect(hasher(a) != hasher(c), "UUIDs differing by one bit hash differently");
}

void test_bytes() {
    Uuid u(MSB, LSB);
    Span<const ByteUnit, 16> raw = u.bytes();

    expect_eq(raw.size(), 16uz, "bytes() exposes all 16 bytes");
    expect(Byte::to_integer<u8>(raw[0]) == 0x01, "first byte is the msb high byte");
    expect(Byte::to_integer<u8>(raw[15]) == 0x10, "last byte is the lsb low byte");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Uuid.nil_and_max", test_nil_and_max},
        {"Uuid.from_bits", test_from_bits},
        {"Uuid.string_roundtrip", test_string_roundtrip},
        {"Uuid.from_string_invalid", test_from_string_invalid},
        {"Uuid.random_uuid", test_random_uuid},
        {"Uuid.timestamp_uuid", test_timestamp_uuid},
        {"Uuid.comparison_and_ordering", test_comparison_and_ordering},
        {"Uuid.hash", test_hash},
        {"Uuid.bytes", test_bytes},
    });
}
