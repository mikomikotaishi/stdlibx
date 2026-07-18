import stdx;

using stdx::collections::Vector;
using stdx::math::BigDecimal;
using stdx::math::BigInteger;
using stdx::math::MathContext;
using stdx::math::RoundingMode;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_biginteger_basic() {
    expect(BigInteger() == BigInteger::ZERO, "default constructor is zero");
    expect_eq(BigInteger(0).to_string(), "0", "zero to_string");
    expect_eq(BigInteger(-42).to_string(), "-42", "negative to_string");
    expect_eq(BigInteger("-0").to_string(), "0", "negative zero parses to zero");
    expect_eq(BigInteger("000123").to_string(), "123", "leading zeros");
    expect_eq(BigInteger("+987654321987654321").to_string(), "987654321987654321", "plus sign");
    expect_eq(
        BigInteger(Long::MIN_VALUE).to_string(),
        "-9223372036854775808",
        "i64 min round trip"
    );
    expect_eq(BigInteger::TEN.signum(), 1, "TEN signum");
    expect_eq(BigInteger("-5").signum(), -1, "negative signum");
    expect_eq(BigInteger::ZERO.signum(), 0, "zero signum");
    expect(BigInteger::value_of(123456789) == BigInteger("123456789"), "value_of");
    expect(BigInteger::try_parse("123").has_value(), "try_parse success");
    expect(!BigInteger::try_parse("12x3").has_value(), "try_parse failure");
    expect(BigInteger("ff", 16) == BigInteger(255), "hex parse");
    expect(BigInteger("-FF", 16) == BigInteger(-255), "uppercase hex parse");
    expect(BigInteger("101", 2) == BigInteger(5), "binary parse");
    expect(BigInteger("zz", 36) == BigInteger(35 * 36 + 35), "base 36 parse");
}

void test_biginteger_cross_check() {
    static constexpr Array<i64, 24> VALUES{
        -2147483648LL, -100000007, -129, -128, -100, -9, -8, -7, -3, -2, -1,
        0, 1, 2, 3, 7, 8, 9, 100, 127, 128, 129, 100000007, 2147483647
    };
    static constexpr Array<i32, 4> SHIFTS{0, 1, 7, 13};
    for (i64 a: VALUES) {
        const BigInteger big_a(a);
        expect(big_a.long_value() == a, "long_value round trip");
        for (i64 b: VALUES) {
            const BigInteger big_b(b);
            expect((big_a + big_b).long_value() == a + b, "addition cross-check");
            expect((big_a - big_b).long_value() == a - b, "subtraction cross-check");
            expect((big_a * big_b).long_value() == a * b, "multiplication cross-check");
            if (b != 0) {
                expect((big_a / big_b).long_value() == a / b, "division cross-check");
                expect((big_a % big_b).long_value() == a % b, "remainder cross-check");
                if (b > 0) {
                    expect(big_a.mod(big_b).long_value() == ((a % b) + b) % b, "mod cross-check");
                }
            }
            expect((big_a & big_b).long_value() == (a & b), "and cross-check");
            expect((big_a | big_b).long_value() == (a | b), "or cross-check");
            expect((big_a ^ big_b).long_value() == (a ^ b), "xor cross-check");
            expect((big_a < big_b) == (a < b), "comparison cross-check");
            expect(Math::min(big_a, big_b).long_value() == Math::min(a, b), "min cross-check");
            expect(Math::max(big_a, big_b).long_value() == Math::max(a, b), "max cross-check");
            expect(Math::gcd(big_a, big_b).long_value() == Math::gcd(a, b), "gcd cross-check");
        }
        expect((~big_a).long_value() == ~a, "not cross-check");
        expect((-big_a).long_value() == -a, "negate cross-check");
        for (i32 k: SHIFTS) {
            if (a > -(1LL << 40) && a < (1LL << 40)) {
                expect((big_a << k).long_value() == (a << k), "shift left cross-check");
            }
            expect((big_a >> k).long_value() == (a >> k), "shift right cross-check");
        }
        for (i32 k = 0; k < 70; ++k) {
            const bool expected = ((a >> Math::min(k, 63)) & 1) != 0;
            expect(big_a.test_bit(k) == expected, "test_bit cross-check");
        }
    }
}

void test_biginteger_arithmetic() {
    BigInteger factorial = BigInteger::ONE;
    for (i32 i = 2; i <= 50; ++i) {
        factorial *= BigInteger(i);
    }
    expect_eq(
        factorial.to_string(),
        "30414093201713378043612608166064768844377641568960512000000000000",
        "50 factorial"
    );
    expect_eq(
        BigInteger::TWO.pow(256).to_string(),
        "115792089237316195423570985008687907853269984665640564039457584007913129639936",
        "2^256"
    );
    expect_eq(
        (BigInteger::ONE << 100).to_string(),
        "1267650600228229401496703205376",
        "1 << 100"
    );

    const BigInteger a = "123456789012345678901234567890123456789"_bi;
    const BigInteger b = "98765432109876543210987654321"_bi;
    const BigInteger product = a * b;
    expect(product / b == a, "product divided by factor");
    expect(product % b == BigInteger::ZERO, "product remainder is zero");
    const auto [quotient, remainder] = a.divide_and_remainder(b);
    expect(quotient * b + remainder == a, "divide_and_remainder identity");
    expect(remainder.abs() < b.abs(), "remainder smaller than divisor");

    expect(BigInteger("123456789").gcd(BigInteger("987654321")) == BigInteger(9), "gcd known value");
    expect(BigInteger::TEN.pow(50).sqrt() == BigInteger::TEN.pow(25), "sqrt of 10^50");
    expect((BigInteger::ONE << 100).sqrt() == (BigInteger::ONE << 50), "sqrt of 2^100");
    expect((BigInteger::ONE << 101).sqrt() == BigInteger("1592262918131443"), "sqrt of 2^101");
    const BigInteger n("99999999999999999999999999");
    const auto [root, rest] = n.sqrt_and_remainder();
    expect(root * root + rest == n, "sqrt_and_remainder identity");
    expect(rest.signum() >= 0, "sqrt remainder non-negative");
    expect((root + 1) * (root + 1) > n, "sqrt is floor");

    // Large operands to exercise the Karatsuba and Knuth division paths:
    // (x - 1) * (y - 7) must equal x*y - 7x - y + 7 computed via shift/add only.
    const BigInteger x = BigInteger::ONE << 2500;
    const BigInteger y = BigInteger::ONE << 2300;
    const BigInteger left = (x - 1) * (y - 7);
    const BigInteger right = (BigInteger::ONE << 4800) - (x << 3) + x - y + 7;
    expect(left == right, "Karatsuba identity");
    expect(left / (y - 7) == x - 1, "Knuth division identity");
    expect(left % (y - 7) == BigInteger::ZERO, "Knuth division exactness");
    expect(BigInteger(left.to_string()) == left, "huge decimal round trip");
    expect(BigInteger(left.to_string(16), 16) == left, "huge hex round trip");
}

void test_biginteger_modular() {
    expect(BigInteger(2).mod_pow(BigInteger(10), BigInteger(1000)) == BigInteger(24), "2^10 mod 1000");
    static constexpr i64 MODULUS = 1000003;
    i64 expected = 1;
    for (i32 i = 0; i < 100; ++i) {
        expected = expected * 3 % MODULUS;
    }
    expect(
        BigInteger(3).mod_pow(BigInteger(100), BigInteger(MODULUS)).long_value() == expected,
        "3^100 mod 1000003"
    );
    expect(
        BigInteger(7).mod_pow(BigInteger::ZERO, BigInteger(13)) == BigInteger::ONE,
        "x^0 mod m"
    );
    const BigInteger value(1234567);
    const BigInteger prime(1000003);
    const BigInteger inverse = value.mod_inverse(prime);
    expect(value * inverse % prime == BigInteger::ONE, "mod_inverse identity");
    expect(
        BigInteger(3).mod_pow(BigInteger(-2), BigInteger(7)) == BigInteger(4),
        "negative exponent mod_pow"
    );
    expect(BigInteger(-7).mod(BigInteger(3)) == BigInteger(2), "mod always non-negative");
}

void test_biginteger_bits() {
    expect_eq(BigInteger::ZERO.bit_length(), 0, "bit_length of 0");
    expect_eq(BigInteger::ONE.bit_length(), 1, "bit_length of 1");
    expect_eq(BigInteger(255).bit_length(), 8, "bit_length of 255");
    expect_eq(BigInteger(256).bit_length(), 9, "bit_length of 256");
    expect_eq(BigInteger(-1).bit_length(), 0, "bit_length of -1");
    expect_eq(BigInteger(-256).bit_length(), 8, "bit_length of -256");
    expect_eq(BigInteger(-255).bit_length(), 8, "bit_length of -255");
    expect_eq((BigInteger::ONE << 1000).bit_length(), 1001, "bit_length of 2^1000");

    expect_eq(BigInteger::ZERO.bit_count(), 0, "bit_count of 0");
    expect_eq(BigInteger(7).bit_count(), 3, "bit_count of 7");
    expect_eq(BigInteger(-1).bit_count(), 0, "bit_count of -1");
    expect_eq(BigInteger(-2).bit_count(), 1, "bit_count of -2");
    expect_eq(BigInteger(-8).bit_count(), 3, "bit_count of -8");

    expect_eq(BigInteger::ZERO.get_lowest_set_bit(), -1, "lowest set bit of 0");
    expect_eq(BigInteger(12).get_lowest_set_bit(), 2, "lowest set bit of 12");
    expect_eq((BigInteger::ONE << 70).get_lowest_set_bit(), 70, "lowest set bit of 2^70");

    expect(BigInteger::ZERO.set_bit(40) == BigInteger::ONE << 40, "set_bit");
    expect((BigInteger::ONE << 40).clear_bit(40) == BigInteger::ZERO, "clear_bit");
    expect(BigInteger::ZERO.flip_bit(3) == BigInteger(8), "flip_bit on");
    expect(BigInteger(8).flip_bit(3) == BigInteger::ZERO, "flip_bit off");
    expect(BigInteger(0b1100).and_not(BigInteger(0b1010)) == BigInteger(0b0100), "and_not");
    expect((BigInteger(-100) >> 3) == BigInteger(-13), "arithmetic shift floors");
    expect((BigInteger(100) << -2) == BigInteger(25), "negative left shift");
}

void test_biginteger_primes() {
    expect(BigInteger(2).is_probable_prime(), "2 is prime");
    expect(BigInteger(97).is_probable_prime(), "97 is prime");
    expect(BigInteger(-7).is_probable_prime(), "abs is used for primality");
    expect(!BigInteger::ONE.is_probable_prime(), "1 is not prime");
    expect(!BigInteger::ZERO.is_probable_prime(), "0 is not prime");
    expect(!BigInteger(561).is_probable_prime(), "Carmichael 561 is composite");
    expect(!BigInteger(1105).is_probable_prime(), "Carmichael 1105 is composite");
    expect(BigInteger("2305843009213693951").is_probable_prime(), "2^61 - 1 is prime");
    expect(!((BigInteger::ONE << 67) - 1).is_probable_prime(), "2^67 - 1 is composite");
    expect(BigInteger(42).is_probable_prime(0), "certainty <= 0 is always true");
    expect(BigInteger(90).next_probable_prime() == BigInteger(97), "next prime after 90");
    expect(BigInteger::ZERO.next_probable_prime() == BigInteger::TWO, "next prime after 0");
    expect(BigInteger(2).next_probable_prime() == BigInteger(3), "next prime after 2");
}

void test_biginteger_conversions() {
    expect_eq(BigInteger("4294967296").int_value(), 0, "int_value truncates 2^32");
    expect_eq(BigInteger("4294967301").int_value(), 5, "int_value truncates 2^32 + 5");
    expect_eq(BigInteger(-1).int_value(), -1, "int_value of -1");
    expect(
        ((BigInteger::ONE << 64) + 7).long_value() == 7,
        "long_value truncates 2^64 + 7"
    );
    expect(BigInteger(127).byte_value_exact() == 127, "byte_value_exact in range");
    expect(BigInteger(-128).byte_value_exact() == -128, "byte_value_exact at minimum");
    bool threw = false;
    try {
        (void)BigInteger(128).byte_value_exact();
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "byte_value_exact out of range throws");

    expect(BigInteger::TEN.pow(20).double_value() == 1e20, "double_value exact power of ten");
    expect((BigInteger::ONE << 100).double_value() == Math::ldexp(1.0, 100), "double_value 2^100");
    expect(
        ((BigInteger::ONE << 53) + 1).double_value() == Math::ldexp(1.0, 53),
        "double_value rounds half to even"
    );
    expect(BigInteger(-3).double_value() == -3.0, "double_value small negative");
    expect((BigInteger::ONE << 1100).double_value() == Double::POSITIVE_INFINITY, "double_value overflow");
    expect(BigInteger(16777217).float_value() == 16777216.0f, "float_value rounds half to even");

    static constexpr Array<i64, 10> VALUES{0, 1, -1, 127, 128, -128, -129, 255, 65535, -1000000};
    for (i64 value: VALUES) {
        const BigInteger original(value);
        expect(BigInteger(original.to_byte_array()) == original, "byte array round trip");
    }
    const Vector<u8> bytes_255 = BigInteger(255).to_byte_array();
    expect(bytes_255.size() == 2 && bytes_255[0] == 0x00 && bytes_255[1] == 0xFF, "to_byte_array of 255");
    const Vector<u8> bytes_minus_one = BigInteger(-1).to_byte_array();
    expect(bytes_minus_one.size() == 1 && bytes_minus_one[0] == 0xFF, "to_byte_array of -1");
    expect(BigInteger(1, Vector<u8>{0x01, 0x00}) == BigInteger(256), "signum-magnitude constructor");
    expect(BigInteger(-1, Vector<u8>{0x05}) == BigInteger(-5), "negative signum-magnitude");

    expect_eq(BigInteger(255).to_string(16), "ff", "to_string radix 16");
    expect_eq(BigInteger(-255).to_string(16), "-ff", "negative to_string radix 16");
    expect_eq(BigInteger(5).to_string(2), "101", "to_string radix 2");
    expect_eq(BigInteger(255).to_string(99), "255", "invalid radix falls back to 10");
    expect_eq(stdx::fmt::format("{}", BigInteger("-12345678901234567890")), "-12345678901234567890", "formatter");

    expect(BigInteger(42) == BigInteger("42"), "operator== across constructors");
    expect(
        Hash<BigInteger>()("12345678901234567890"_bi ) == Hash<BigInteger>()("12345678901234567890"_bi),
        "hash consistency"
    );
    BigInteger counter(41);
    expect(++counter == BigInteger(42), "pre-increment");
    expect(counter++ == BigInteger(42) && counter == BigInteger(43), "post-increment");
}

void test_bigdecimal_parse_format() {
    expect_eq(BigDecimal("123.45").to_string(), "123.45", "plain round trip");
    expect_eq(BigDecimal("-0.001").to_string(), "-0.001", "small negative round trip");
    expect_eq(BigDecimal("0.00").to_string(), "0.00", "zero with scale");
    expect_eq(BigDecimal(".5").to_string(), "0.5", "leading dot");
    expect_eq(BigDecimal("5.").to_string(), "5", "trailing dot");
    expect_eq(BigDecimal("1.234E-7").to_string(), "1.234E-7", "scientific round trip");
    expect_eq(BigDecimal("0.000001234").to_string(), "0.000001234", "adjusted -6 stays plain");
    expect_eq(BigDecimal("0.0000001234").to_string(), "1.234E-7", "adjusted -7 goes scientific");
    expect_eq(BigDecimal("123E+3").to_string(), "1.23E+5", "positive exponent");
    expect_eq(BigDecimal("0E+3").to_string(), "0E+3", "zero with negative scale");
    expect_eq(BigDecimal("12.345e2").to_string(), "1234.5", "exponent folds into scale");

    expect_eq(BigDecimal("1E+3").to_plain_string(), "1000", "to_plain_string pads zeros");
    expect_eq(BigDecimal("-1.5E-4").to_plain_string(), "-0.00015", "to_plain_string small");
    expect_eq(BigDecimal("123E+3").to_engineering_string(), "123E+3", "engineering keeps multiple of 3");
    expect_eq(BigDecimal("12E+2").to_engineering_string(), "1.2E+3", "engineering shifts digits");
    expect_eq(BigDecimal("0E+9").to_engineering_string(), "0E+9", "engineering zero");

    expect(BigDecimal("123.45").scale() == 2, "scale of plain");
    expect(BigDecimal("1E+3").scale() == -3, "scale of exponent form");
    expect(BigDecimal("123.45").precision() == 5, "precision");
    expect(BigDecimal("0.00120").precision() == 3, "precision ignores leading zeros");
    expect(BigDecimal("1000").precision() == 4, "precision of power of ten");
    expect(BigDecimal().precision() == 1, "precision of zero");
    expect(BigDecimal("123.45").unscaled_value() == BigInteger(12345), "unscaled_value");

    expect(!BigDecimal::try_parse("").has_value(), "empty string rejected");
    expect(!BigDecimal::try_parse(".").has_value(), "lone dot rejected");
    expect(!BigDecimal::try_parse("1e5x").has_value(), "trailing junk rejected");
    expect(!BigDecimal::try_parse("1e+-5").has_value(), "double exponent sign rejected");
    expect(BigDecimal::try_parse("-12.5e-3").has_value(), "valid form accepted");
}

void test_bigdecimal_arithmetic() {
    expect_eq((BigDecimal("0.1") + BigDecimal("0.2")).to_string(), "0.3", "exact decimal addition");
    expect_eq((BigDecimal("1.0") - BigDecimal("0.75")).to_string(), "0.25", "subtraction aligns scales");
    expect_eq((BigDecimal("1.5") * BigDecimal("2.5")).to_string(), "3.75", "multiplication adds scales");
    expect_eq((BigDecimal("2.0") * BigDecimal("3.00")).to_string(), "6.000", "scale preserved");
    expect_eq(BigDecimal("1.1").pow(2).to_string(), "1.21", "pow");
    expect_eq(BigDecimal("-1.5").abs().to_string(), "1.5", "abs");
    expect_eq((-BigDecimal("1.5")).to_string(), "-1.5", "unary minus");
    expect(+BigDecimal("3.14") == BigDecimal("3.14"), "unary plus");
    expect(BigDecimal("-0.5").signum() == -1, "signum");

    BigDecimal accumulator("0.0");
    for (i32 i = 0; i < 10; ++i) {
        accumulator += BigDecimal("0.1");
    }
    expect_eq(accumulator.to_string(), "1.0", "repeated addition is exact");
}

void test_bigdecimal_divide() {
    expect_eq((BigDecimal(1) / BigDecimal(4)).to_string(), "0.25", "exact divide extends scale");
    expect_eq((BigDecimal("1.0") / BigDecimal("0.25")).to_string(), "4", "exact divide reduces");
    expect_eq((BigDecimal(100) / BigDecimal(5)).to_string(), "20", "integral quotient");
    expect_eq((BigDecimal("2.00") / BigDecimal("0.1")).to_string(), "20.0", "preferred scale");
    bool threw = false;
    try {
        (void)(BigDecimal(1) / BigDecimal(3));
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "non-terminating expansion throws");

    expect_eq(
        BigDecimal(1).divide(BigDecimal(3), 10, RoundingMode::HALF_UP).to_string(),
        "0.3333333333",
        "divide with scale and rounding"
    );
    expect_eq(
        BigDecimal(2).divide(BigDecimal(3), 10, RoundingMode::HALF_UP).to_string(),
        "0.6666666667",
        "divide rounds final digit"
    );
    expect_eq(
        BigDecimal("-1").divide(BigDecimal(3), 4, RoundingMode::FLOOR).to_string(),
        "-0.3334",
        "FLOOR division of negative"
    );
    expect_eq(
        BigDecimal("1.23").divide(BigDecimal("0.3"), RoundingMode::HALF_UP).to_string(),
        "4.10",
        "divide keeps dividend scale"
    );

    expect(
        BigDecimal("5.5").divide_to_integral_value(BigDecimal(2)) == BigDecimal(2),
        "divide_to_integral_value"
    );
    expect(
        BigDecimal(100).divide_to_integral_value(BigDecimal("0.25")) == BigDecimal(400),
        "integral value with negative preferred scale"
    );
    expect_eq((BigDecimal("5.5") % BigDecimal(2)).to_string(), "1.5", "remainder");
    expect_eq((BigDecimal("-5.5") % BigDecimal(2)).to_string(), "-1.5", "remainder keeps dividend sign");
    const auto [quotient, remainder] = BigDecimal("8.7").divide_and_remainder(BigDecimal("2.5"));
    expect(quotient == BigDecimal(3), "divide_and_remainder quotient");
    expect_eq(remainder.to_string(), "1.2", "divide_and_remainder remainder");
}

void test_bigdecimal_rounding() {
    const BigDecimal positive_half("2.5");
    const BigDecimal negative_half("-2.5");
    expect_eq(positive_half.set_scale(0, RoundingMode::UP).to_string(), "3", "UP");
    expect_eq(positive_half.set_scale(0, RoundingMode::DOWN).to_string(), "2", "DOWN");
    expect_eq(positive_half.set_scale(0, RoundingMode::CEILING).to_string(), "3", "CEILING");
    expect_eq(positive_half.set_scale(0, RoundingMode::FLOOR).to_string(), "2", "FLOOR");
    expect_eq(positive_half.set_scale(0, RoundingMode::HALF_UP).to_string(), "3", "HALF_UP");
    expect_eq(positive_half.set_scale(0, RoundingMode::HALF_DOWN).to_string(), "2", "HALF_DOWN");
    expect_eq(positive_half.set_scale(0, RoundingMode::HALF_EVEN).to_string(), "2", "HALF_EVEN ties to even");
    expect_eq(BigDecimal("3.5").set_scale(0, RoundingMode::HALF_EVEN).to_string(), "4", "HALF_EVEN odd neighbour");
    expect_eq(negative_half.set_scale(0, RoundingMode::HALF_UP).to_string(), "-3", "HALF_UP negative");
    expect_eq(negative_half.set_scale(0, RoundingMode::CEILING).to_string(), "-2", "CEILING negative");
    expect_eq(negative_half.set_scale(0, RoundingMode::FLOOR).to_string(), "-3", "FLOOR negative");
    expect_eq(BigDecimal("1.449").set_scale(1, RoundingMode::HALF_UP).to_string(), "1.4", "no double rounding");
    expect_eq(BigDecimal("2.5").set_scale(3).to_string(), "2.500", "set_scale extends exactly");
    bool threw = false;
    try {
        (void)BigDecimal("1.1").set_scale(0);
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "UNNECESSARY rounding throws");

    expect_eq(BigDecimal("123456").round(MathContext(3, RoundingMode::HALF_UP)).to_string(), "1.23E+5", "round to precision");
    expect_eq(BigDecimal("999.9").round(MathContext(3, RoundingMode::HALF_UP)).to_string(), "1.00E+3", "round carries across precision");
    expect(BigDecimal("123.45").round(MathContext::UNLIMITED) == BigDecimal("123.45"), "UNLIMITED round is identity");
    expect(BigDecimal("123.45").round(MathContext::DECIMAL32) == BigDecimal("123.45"), "round within precision is identity");
}

void test_bigdecimal_scaling() {
    expect_eq(BigDecimal("123.45").move_point_left(2).to_string(), "1.2345", "move_point_left");
    expect_eq(BigDecimal("123.45").move_point_right(2).to_string(), "12345", "move_point_right");
    expect_eq(BigDecimal("12").move_point_right(3).to_string(), "12000", "move_point_right pads");
    expect_eq(BigDecimal("12").move_point_left(-3).to_string(), "12000", "negative move_point_left");
    expect_eq(BigDecimal("12").scale_by_power_of_ten(3).to_string(), "1.2E+4", "scale_by_power_of_ten");
    expect_eq(BigDecimal("600.0").strip_trailing_zeros().to_string(), "6E+2", "strip to negative scale");
    expect_eq(BigDecimal("0.500").strip_trailing_zeros().to_string(), "0.5", "strip fraction zeros");
    expect_eq(BigDecimal("0.000").strip_trailing_zeros().to_string(), "0", "strip zero");
    expect_eq(BigDecimal("123.45").ulp().to_string(), "0.01", "ulp");
}

void test_bigdecimal_compare() {
    expect(BigDecimal("2.0") == BigDecimal("2.00"), "operator== is numeric");
    expect(!BigDecimal("2.0").equals(BigDecimal("2.00")), "equals is scale-sensitive");
    expect(BigDecimal("2.0").equals(BigDecimal("2.0")), "equals same representation");
    expect(BigDecimal("1.5") < BigDecimal("1.50001"), "operator<");
    expect(BigDecimal("-3") < BigDecimal("2"), "negative less than positive");
    expect(BigDecimal("1E+100") > BigDecimal("9.9E+99"), "large magnitude comparison");
    expect(BigDecimal("1E-100") > BigDecimal::ZERO, "tiny positive above zero");
    expect((BigDecimal("3.5") <=> BigDecimal("3.5")) == 0, "three-way comparison equivalent");
    expect(Math::min(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.1"), "min");
    expect(Math::max(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.2"), "max");
    expect(
        Hash<BigDecimal>()(BigDecimal("2.0")) == Hash<BigDecimal>()(BigDecimal("2.00")),
        "hash consistent with operator=="
    );
}

void test_bigdecimal_conversions() {
    expect_eq(BigDecimal(0.5).to_string(), "0.5", "exact double constructor for 0.5");
    expect_eq(
        BigDecimal(0.1).to_string(),
        "0.1000000000000000055511151231257827021181583404541015625",
        "exact double constructor for 0.1"
    );
    expect_eq(BigDecimal::value_of(0.1).to_string(), "0.1", "value_of uses shortest representation");
    expect_eq(BigDecimal::value_of(-2.5).to_string(), "-2.5", "value_of negative");
    expect_eq(BigDecimal::value_of(123456789, 4).to_string(), "12345.6789", "value_of with scale");
    bool threw = false;
    try {
        (void)BigDecimal(Double::POSITIVE_INFINITY);
    } catch (const NumberFormatException& _) {
        threw = true;
    }
    expect(threw, "infinite double throws");

    expect(BigDecimal("-5.9").to_big_integer() == BigInteger(-5), "to_big_integer truncates");
    expect(BigDecimal("1E+3").to_big_integer() == BigInteger(1000), "to_big_integer pads");
    expect(BigDecimal("5.00").to_big_integer_exact() == BigInteger(5), "to_big_integer_exact");
    threw = false;
    try {
        (void)BigDecimal("5.5").to_big_integer_exact();
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "to_big_integer_exact throws on fraction");

    expect_eq(BigDecimal("-5.9").int_value(), -5, "int_value");
    expect_eq(BigDecimal("42.00").long_value_exact(), 42, "long_value_exact");
    expect(BigDecimal("0.1").double_value() == 0.1, "double_value round trip");
    expect(BigDecimal("2.5").float_value() == 2.5f, "float_value");
    expect(BigDecimal("1E+400").double_value() == Double::POSITIVE_INFINITY, "double_value overflow");
    expect(BigDecimal("1E-400").double_value() == 0.0, "double_value underflow");
    expect_eq(stdx::fmt::format("{}", BigDecimal("3.14")), "3.14", "formatter");

    expect(BigDecimal(BigInteger("123")) + BigDecimal(5) == BigDecimal(128), "BigInteger interop");
    expect(BigDecimal("2.5") * BigDecimal(2) == BigDecimal(5), "integral operand promotion");
}

void test_exceptions() {
    bool threw = false;
    try {
        (void)(BigInteger(1) / BigInteger::ZERO);
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "BigInteger division by zero throws");

    threw = false;
    try {
        (void)(BigDecimal(1) / BigDecimal());
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "BigDecimal division by zero throws");

    threw = false;
    try {
        (void)BigInteger("123", 37);
    } catch (const NumberFormatException& _) {
        threw = true;
    }
    expect(threw, "radix out of range throws");

    threw = false;
    try {
        (void)BigInteger("");
    } catch (const NumberFormatException& _) {
        threw = true;
    }
    expect(threw, "empty string throws");

    threw = false;
    try {
        (void)BigInteger(-4).sqrt();
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "sqrt of negative throws");

    threw = false;
    try {
        (void)BigInteger(3).pow(-1);
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "negative pow throws");

    threw = false;
    try {
        (void)BigInteger(3).mod(BigInteger(-5));
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "non-positive modulus throws");

    threw = false;
    try {
        (void)BigInteger(4).mod_inverse(BigInteger(8));
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "non-invertible mod_inverse throws");

    threw = false;
    try {
        (void)BigInteger(5).test_bit(-1);
    } catch (const ArithmeticException& _) {
        threw = true;
    }
    expect(threw, "negative bit address throws");
}

void test_free_functions() {
    // Member forms remain available.
    expect(BigInteger(-7).abs() == BigInteger(7), "BigInteger member abs");
    expect(BigDecimal("-2.5").abs() == BigDecimal("2.5"), "BigDecimal member abs");

    // Free functions found by argument-dependent lookup.
    expect(abs(BigInteger(-7)) == BigInteger(7), "free abs(BigInteger) via ADL");
    expect(abs(BigDecimal("-2.5")) == BigDecimal("2.5"), "free abs(BigDecimal) via ADL");
    expect(min(BigInteger(3), BigInteger(8)) == BigInteger(3), "free min(BigInteger) via ADL");
    expect(max(BigInteger(3), BigInteger(8)) == BigInteger(8), "free max(BigInteger) via ADL");
    expect(min(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.1"), "free min(BigDecimal) via ADL");
    expect(max(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.2"), "free max(BigDecimal) via ADL");

    // The standard customisation idiom (using std::abs; abs(x)) resolves to the overloads.
    {
        expect(abs(BigInteger(-9)) == BigInteger(9), "using std::abs; abs(BigInteger)");
        expect(abs(BigDecimal("-9.5")) == BigDecimal("9.5"), "using std::abs; abs(BigDecimal)");
    }

    // Remaining free functions via ADL.
    expect(Math::clamp(BigInteger(10), BigInteger(0), BigInteger(5)) == BigInteger(5), "free clamp(BigInteger) via ADL");
    expect(Math::clamp(BigDecimal("2.5"), BigDecimal("0"), BigDecimal("1")) == BigDecimal("1"), "free clamp(BigDecimal) via ADL");
    expect(Math::signum(BigInteger(-9)) == -1, "free signum(BigInteger) via ADL");
    expect(Math::signum(BigDecimal("0.00")) == 0, "free signum(BigDecimal) via ADL");
    expect(Math::gcd(BigInteger(54), BigInteger(24)) == BigInteger(6), "free gcd(BigInteger) via ADL");
    expect(Math::lcm(BigInteger(4), BigInteger(6)) == BigInteger(12), "free lcm(BigInteger) via ADL");
    expect(Math::lcm(BigInteger(0), BigInteger(6)) == BigInteger::ZERO, "free lcm with zero");
    expect(Math::pow(BigInteger(2), 10) == BigInteger(1024), "free pow(BigInteger) via ADL");
    expect(Math::pow(BigDecimal("1.1"), 2) == BigDecimal("1.21"), "free pow(BigDecimal) via ADL");
    expect(Math::sqrt(BigInteger(144)) == BigInteger(12), "free sqrt(BigInteger) via ADL");

    // Math facade: unconstrained methods already resolve for the big types.
    expect(Math::min(BigInteger(3), BigInteger(8)) == BigInteger(3), "Math::min(BigInteger)");
    expect(Math::max(BigInteger(3), BigInteger(8)) == BigInteger(8), "Math::max(BigInteger)");
    expect(Math::min(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.1"), "Math::min(BigDecimal)");
    expect(Math::max(BigDecimal("1.1"), BigDecimal("1.2")) == BigDecimal("1.2"), "Math::max(BigDecimal)");
    expect(Math::clamp(BigInteger(10), BigInteger(0), BigInteger(5)) == BigInteger(5), "Math::clamp(BigInteger)");

    // Math facade: the new non-Numeric overloads delegate to the member functions.
    expect(Math::abs(BigInteger(-7)) == BigInteger(7), "Math::abs(BigInteger)");
    expect(Math::abs(BigDecimal("-2.5")) == BigDecimal("2.5"), "Math::abs(BigDecimal)");
    expect(Math::signum(BigInteger(-9)) == -1, "Math::signum(BigInteger)");
    expect(Math::signum(BigDecimal("3.5")) == 1, "Math::signum(BigDecimal)");
    expect(Math::pow(BigInteger(2), 10) == BigInteger(1024), "Math::pow(BigInteger)");
    expect(Math::pow(BigDecimal("1.1"), 2) == BigDecimal("1.21"), "Math::pow(BigDecimal)");
    expect(Math::sqrt(BigInteger(144)) == BigInteger(12), "Math::sqrt(BigInteger)");
    expect(Math::gcd(BigInteger(54), BigInteger(24)) == BigInteger(6), "Math::gcd(BigInteger)");
    expect(Math::lcm(BigInteger(4), BigInteger(6)) == BigInteger(12), "Math::lcm(BigInteger)");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"BigInteger.basic", test_biginteger_basic},
        {"BigInteger.cross_check", test_biginteger_cross_check},
        {"BigInteger.arithmetic", test_biginteger_arithmetic},
        {"BigInteger.modular", test_biginteger_modular},
        {"BigInteger.bits", test_biginteger_bits},
        {"BigInteger.primes", test_biginteger_primes},
        {"BigInteger.conversions", test_biginteger_conversions},
        {"BigDecimal.parse_format", test_bigdecimal_parse_format},
        {"BigDecimal.arithmetic", test_bigdecimal_arithmetic},
        {"BigDecimal.divide", test_bigdecimal_divide},
        {"BigDecimal.rounding", test_bigdecimal_rounding},
        {"BigDecimal.scaling", test_bigdecimal_scaling},
        {"BigDecimal.compare", test_bigdecimal_compare},
        {"BigDecimal.conversions", test_bigdecimal_conversions},
        {"BigNumber.exceptions", test_exceptions},
        {"BigNumber.free_functions", test_free_functions},
    });
}
