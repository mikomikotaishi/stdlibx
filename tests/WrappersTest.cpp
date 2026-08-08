import stdx;

using stdx::meta::IsConstructibleValue;
using stdx::meta::IsPolymorphicValue;
using stdx::meta::IsSameValue;
using stdx::meta::IsStandardLayoutValue;
using stdx::meta::IsTriviallyCopyableValue;
using stdx::meta::IsTriviallyDestructibleValue;

using namespace stdx::test;

#define STDLIBX_ASSERT_NUMBER_IS_FREE(Wrapper, Underlying) \
    static_assert(sizeof(Wrapper) == sizeof(Underlying), \
        #Wrapper " must cost exactly what it wraps"); \
    static_assert(alignof(Wrapper) == alignof(Underlying), \
        #Wrapper " must align like what it wraps"); \
    static_assert(IsTriviallyCopyableValue<Wrapper>, \
        #Wrapper " must stay trivially copyable"); \
    static_assert(IsTriviallyDestructibleValue<Wrapper>, \
        #Wrapper " must stay trivially destructible"); \
    static_assert(IsStandardLayoutValue<Wrapper>, \
        #Wrapper " must stay standard layout"); \
    static_assert(!IsPolymorphicValue<Wrapper>, \
        #Wrapper " must not acquire a vtable"); \
    static_assert(IsConstructibleValue<Wrapper, Underlying>, \
        #Wrapper " must be constructible from the value it wraps")

STDLIBX_ASSERT_NUMBER_IS_FREE(SignedByte, i8);
STDLIBX_ASSERT_NUMBER_IS_FREE(Byte, u8);
STDLIBX_ASSERT_NUMBER_IS_FREE(Short, i16);
STDLIBX_ASSERT_NUMBER_IS_FREE(Integer, i32);
STDLIBX_ASSERT_NUMBER_IS_FREE(Long, i64);
STDLIBX_ASSERT_NUMBER_IS_FREE(UnsignedShort, u16);
STDLIBX_ASSERT_NUMBER_IS_FREE(UnsignedInteger, u32);
STDLIBX_ASSERT_NUMBER_IS_FREE(UnsignedLong, u64);
STDLIBX_ASSERT_NUMBER_IS_FREE(Character, char);
STDLIBX_ASSERT_NUMBER_IS_FREE(UnsignedCharacter, unsigned char);
STDLIBX_ASSERT_NUMBER_IS_FREE(Utf8Character, char8);
STDLIBX_ASSERT_NUMBER_IS_FREE(Utf16Character, char16);
STDLIBX_ASSERT_NUMBER_IS_FREE(Utf32Character, char32);
STDLIBX_ASSERT_NUMBER_IS_FREE(WideCharacter, wchar);
STDLIBX_ASSERT_NUMBER_IS_FREE(SignedSize, isize);
STDLIBX_ASSERT_NUMBER_IS_FREE(UnsignedSize, usize);
#ifdef __STDCPP_FLOAT16_T__
STDLIBX_ASSERT_NUMBER_IS_FREE(Half, f16);
#endif
#ifdef __STDCPP_BFLOAT16_T__
STDLIBX_ASSERT_NUMBER_IS_FREE(BrainHalf, bf16);
#endif
STDLIBX_ASSERT_NUMBER_IS_FREE(Float, f32);
STDLIBX_ASSERT_NUMBER_IS_FREE(Double, f64);
STDLIBX_ASSERT_NUMBER_IS_FREE(Quad, f128);
STDLIBX_ASSERT_NUMBER_IS_FREE(Boolean, bool);

#undef STDLIBX_ASSERT_NUMBER_IS_FREE

static_assert(IsSameValue<decltype(Integer(1) + 1), Integer>);
static_assert(IsSameValue<decltype(Long(1) * 2), Long>);
static_assert(IsSameValue<decltype(-Float(1.0f)), Float>);
static_assert(IsSameValue<decltype(~Byte(0)), Byte>);
static_assert(IsSameValue<decltype(Character('a') + 1), Character>);
static_assert(IsSameValue<decltype(++Ops::declval<Integer&>()), Integer&>);

void test_basic_operations() {
    expect_eq((Integer(40) + 2).get(), 42, "Arithmetic on object with primitive wraps correctly");
    expect_eq((Integer(7) % 4).get(), 3, "Modulo operator on object with primitive wraps correctly");
    expect_eq((Byte(0b1010) & 0b0110).get(), 0b0010, "Bitwise operations on object by primitive wraps correctly");
    expect_eq((Short(1) << 4).get(), 16, "Bit shifting on object by primitive wraps correctly");
    expect_eq(Character('a').get(), 'a', "Character retrieval is correct");
    expect_lt(Integer(1), Integer(2), "Comparison on wrappers is correct");
    expect_eq(Double(1.5), Double(1.5), "Identical objects are equal");
}

void test_formatters() {
    expect_eq(Ops::fmt("{}", Integer(5)), "Integer(5)", "Wrapper formats with class name");
    expect_eq(Ops::fmt("{:?}", Integer(5)), "5", "Wrapper formats with raw value with :? specifier");
}

/**
 * @brief Exercises the compound assignment and increment/decrement operators
 * on Number<T>, which test_basic_operations does not touch.
 */
void test_compound_assignment() {
    Integer i(10);
    i += 5;
    expect_eq(i.get(), 15, "operator+= mutates in place");
    i -= 3;
    expect_eq(i.get(), 12, "operator-= mutates in place");
    i *= 2;
    expect_eq(i.get(), 24, "operator*= mutates in place");
    i /= 4;
    expect_eq(i.get(), 6, "operator/= mutates in place");
    i %= 4;
    expect_eq(i.get(), 2, "operator%= mutates in place");

    Byte b(0b1010);
    b &= 0b0110;
    expect_eq(b.get(), 0b0010, "operator&= mutates in place");
    b |= 0b1000;
    expect_eq(b.get(), 0b1010, "operator|= mutates in place");
    b ^= 0b1111;
    expect_eq(b.get(), 0b0101, "operator^= mutates in place");

    Short s(1);
    s <<= 3;
    expect_eq(s.get(), 8, "operator<<= mutates in place");
    s >>= 2;
    expect_eq(s.get(), 2, "operator>>= mutates in place");

    Integer inc(5);
    Integer& incremented = ++inc;
    expect_eq(inc.get(), 6, "operator++ increments in place");
    expect_eq(incremented.get(), 6, "operator++ returns a reference to the incremented object");

    Integer dec(5);
    Integer& decremented = --dec;
    expect_eq(dec.get(), 4, "operator-- decrements in place");
    expect_eq(decremented.get(), 4, "operator-- returns a reference to the decremented object");

    expect_eq((-Integer(7)).get(), -7, "unary operator- negates");
    expect_eq((Byte(0b0110) | 0b1001).get(), 0b1111, "operator| ors with a primitive");
    expect_eq((Byte(0b0110) ^ 0b0011).get(), 0b0101, "operator^ xors with a primitive");
    expect_eq((Short(16) >> 2).get(), 4, "operator>> shifts right");
}

/**
 * @brief Exercises the *_value() conversions, MAX_VALUE/MIN_VALUE/LOWEST
 * constants, and the parse/parse_or/to_string helpers on Number<T>.
 */
void test_number_conversions_and_parsing() {
    const Integer big(1000);
    expect_eq(big.unsigned_int_value(), 1000u, "unsigned_int_value reads back the value");
    expect_eq(big.long_value(), 1000, "long_value widens without loss");
    expect_eq(big.float_value(), 1000.0f, "float_value converts to a float");
    expect_eq(big.double_value(), 1000.0, "double_value converts to a double");

    const Integer truncatable(0x1FF);
    expect_eq(truncatable.signed_byte_value(), static_cast<i8>(0x1FF), "signed_byte_value truncates like static_cast");
    expect_eq(truncatable.unsigned_byte_value(), static_cast<u8>(0x1FF), "unsigned_byte_value truncates like static_cast");
    expect_eq(Integer::unsigned_byte_value(0x1FF), static_cast<u8>(0x1FF), "the static overload matches the member overload");

    expect_eq(Integer::MAX_VALUE, NumericLimits<i32>::max(), "MAX_VALUE matches the underlying type's max");
    expect_eq(Integer::MIN_VALUE, NumericLimits<i32>::min(), "MIN_VALUE matches the underlying type's min");
    expect_eq(Integer::LOWEST, NumericLimits<i32>::lowest(), "LOWEST matches the underlying type's lowest");
    expect_eq(UnsignedInteger::MIN_VALUE, UnsignedInteger::LOWEST, "for an unsigned type, MIN_VALUE and LOWEST agree");

    require(Integer::parse("42").has_value(), "parse succeeds on well-formed input");
    expect_eq(*Integer::parse("42"), 42, "parse recovers the parsed value");
    expect(!Integer::parse("not-a-number").has_value(), "parse fails on malformed input");
    expect(!Integer::parse("").has_value(), "parse fails on empty input");
    expect_eq(Integer::parse_or("garbage", -1), -1, "parse_or falls back to the default on failure");
    expect_eq(Integer::parse_or("7"), 7, "parse_or returns the parsed value on success");

    expect_eq(Integer(42).to_string(), "42", "to_string renders the raw value, not the wrapper name");
    expect_eq(Integer::to_string(42), "42", "the static overload matches the member overload");
}

static_assert(Boolean::TRUE.get(), "TRUE wraps true");
static_assert(!Boolean::FALSE.get(), "FALSE wraps false");

/**
 * @brief Exercises Boolean: construction, operator!, parse/parse_or, to_string,
 * and the TRUE/FALSE constants.
 */
void test_boolean() {
    expect(Boolean(true).get(), "constructing from true stores true");
    expect(!(Boolean(true).operator!().get()), "operator! negates");
    expect(Boolean(false).operator!().get(), "operator! negates false to true");

    require(Boolean::parse("true").has_value(), "parse accepts \"true\"");
    expect(*Boolean::parse("true"), "parse(\"true\") yields true");
    require(Boolean::parse("1").has_value(), "parse accepts \"1\"");
    expect(*Boolean::parse("1"), "parse(\"1\") yields true");
    require(Boolean::parse("false").has_value(), "parse accepts \"false\"");
    expect(!*Boolean::parse("false"), "parse(\"false\") yields false");
    require(Boolean::parse("0").has_value(), "parse accepts \"0\"");
    expect(!*Boolean::parse("0"), "parse(\"0\") yields false");
    expect(!Boolean::parse("TRUE").has_value(), "parse is case-sensitive, rejecting \"TRUE\"");
    expect(!Boolean::parse("yes").has_value(), "parse rejects \"yes\"");
    expect(!Boolean::parse("").has_value(), "parse rejects an empty string");

    expect_eq(Boolean::parse_or("garbage", true), true, "parse_or falls back to the default on failure");
    expect_eq(Boolean::parse_or("false", true), false, "parse_or returns the parsed value on success");

    expect_eq(Boolean(true).to_string(), "true", "to_string renders \"true\"");
    expect_eq(Boolean(false).to_string(), "false", "to_string renders \"false\"");
    expect_eq(Boolean::to_string(true), "true", "the static overload matches the member overload");
}

static_assert(
    Character::BLOCK_STARTS[0] == 0u,
    "the first Unicode block starts at code point 0"
);

static_assert(
    WideCharacter::BLOCK_STARTS[0] == Character::BLOCK_STARTS[0],
    "every character type shares the same BLOCK_STARTS table"
);

/**
 * @brief Exercises CharacterBase's classification predicates, in both the
 * member and static forms, and the is_valid_code_point / BLOCK_STARTS /
 * SCRIPT_STARTS surface.
 */
void test_character_classification() {
    expect(Character('a').is_alphabetic(), "'a' is alphabetic");
    expect(Character('a').is_lower_case(), "'a' is lower case");
    expect(!Character('a').is_upper_case(), "'a' is not upper case");
    expect(Character('A').is_upper_case(), "'A' is upper case");
    expect(Character('5').is_digit(), "'5' is a digit");
    expect(!Character('a').is_digit(), "'a' is not a digit");
    expect(Character('a').is_alphanumeric(), "'a' is alphanumeric");
    expect(Character('5').is_alphanumeric(), "'5' is alphanumeric");
    expect(!Character('!').is_alphanumeric(), "'!' is not alphanumeric");
    expect(Character(' ').is_whitespace(), "' ' is whitespace");
    expect(!Character('a').is_whitespace(), "'a' is not whitespace");
    expect(Character('\n').is_control(), "'\\n' is a control character");
    expect(!Character('a').is_control(), "'a' is not a control character");
    expect(Character('a').is_printable(), "'a' is printable");
    expect(!Character('\n').is_printable(), "'\\n' is not printable");

    expect(Character::is_alphabetic('z'), "the static overload matches the member overload for is_alphabetic");
    expect(Character::is_digit('9'), "the static overload matches the member overload for is_digit");
    expect(Character::is_upper_case('Z'), "the static overload matches the member overload for is_upper_case");
    expect(Character::is_lower_case('z'), "the static overload matches the member overload for is_lower_case");
    expect(Character::is_whitespace('\t'), "the static overload matches the member overload for is_whitespace");
    expect(Character::is_control('\0'), "the static overload matches the member overload for is_control");
    expect(Character::is_printable('~'), "the static overload matches the member overload for is_printable");
    expect(Character::is_alphanumeric('0'), "the static overload matches the member overload for is_alphanumeric");

    expect(Character('a').is_valid_code_point(), "an 8-bit character is trivially a valid code point");
    expect(Character::is_valid_code_point(static_cast<char>(0)), "the static overload agrees for the null character");
    expect(Utf32Character(0x10FFFF).is_valid_code_point(), "the last valid Unicode code point is valid");
    expect(!Utf32Character(0x110000).is_valid_code_point(), "one past the last valid Unicode code point is invalid");
}


static_assert(Float::NaN != Float::NaN, "NaN does not compare equal to itself");

/**
 * @brief Exercises FloatingPointBase: is_nan/is_infinite/is_finite, and the
 * special-value constants, including that NaN compares unordered rather
 * than equal to itself (Number<T>::operator<=> is deduced per type).
 */
void test_floating_point_special_values() {
    expect(Float(1.0f).is_finite(), "1.0 is finite");
    expect(!Float(1.0f).is_nan(), "1.0 is not NaN");
    expect(!Float(1.0f).is_infinite(), "1.0 is not infinite");

    expect(Float(Float::NaN).is_nan(), "Float::NaN is reported as NaN");
    expect(!Float(Float::NaN).is_finite(), "NaN is not finite");

    expect(Float(Float::POSITIVE_INFINITY).is_infinite(), "positive infinity is reported as infinite");
    expect(!Float(Float::POSITIVE_INFINITY).is_finite(), "infinity is not finite");
    expect(Float(Float::NEGATIVE_INFINITY).is_infinite(), "negative infinity is reported as infinite");
    expect(Float::NEGATIVE_INFINITY < 0.0f, "NEGATIVE_INFINITY is negative");

    expect(Double(Double::NaN).is_nan(), "Double::NaN is reported as NaN");
    expect(Double(Double::POSITIVE_INFINITY).is_infinite(), "Double positive infinity is reported as infinite");
    expect(Double(1.0).is_finite(), "a finite Double is reported as finite");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Wrappers.basic_operations", test_basic_operations},
        {"Wrappers.compound_assignment", test_compound_assignment},
        {"Wrappers.number_conversions_and_parsing", test_number_conversions_and_parsing},
        {"Wrappers.boolean", test_boolean},
        {"Wrappers.character_classification", test_character_classification},
        {"Wrappers.floating_point_special_values", test_floating_point_special_values},
        {"Wrappers.formatters", test_formatters},
    });
}
