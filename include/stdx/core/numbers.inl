#pragma once

using stdx::meta::IsConstructibleValue;
using stdx::meta::IsPolymorphicValue;
using stdx::meta::IsStandardLayoutValue;
using stdx::meta::IsTriviallyCopyableValue;
using stdx::meta::IsTriviallyDestructibleValue;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
namespace stdx::core {
    template <typename T, typename Self>
    concept HasGet = requires (const Self& s) {
        { s.get() } -> ConvertibleTo<T>;
    };

    /**
     * @class Number
     * @brief Base class providing the numeric operations of a wrapper type.
     * @tparam T The underlying numeric type (e.g., int, float, double).
     */
    template <Numeric T>
    class [[nodiscard]] Number {
    protected:
        T _value{0}; ///< The underlying numeric value.
    public:
        static constexpr T MAX_VALUE = NumericLimits<T>::max(); ///< A constant representing the maximum value of the number type.
        static constexpr T MIN_VALUE = NumericLimits<T>::min(); ///< A constant representing the minimum value of the number type.
        static constexpr T LOWEST = NumericLimits<T>::lowest(); ///< A constant representing the lowest value of the number type (same as MIN_value for unsigned types, but may differ for signed types).

        constexpr Number() = default;

        constexpr Number(T value) noexcept:
            _value{value} {}

        constexpr ~Number() = default;

        constexpr Number& operator=(T value) noexcept {
            _value = value;
            return *this;
        }

        constexpr Number(const Number&) = default;
        constexpr Number(Number&&) noexcept = default;
        constexpr Number& operator=(const Number&) = default;
        constexpr Number& operator=(Number&&) noexcept = default;

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator+(this const Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() + value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator-(this const Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() - value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator*(this const Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() * value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator/(this const Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() / value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator%(this const Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() % value)};
        }

        template <typename Self, typename U>
        constexpr Self& operator+=(this Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value + value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator-=(this Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value - value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator*=(this Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value * value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator/=(this Self& self, U value) noexcept requires ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value / value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator%=(this Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value % value);
            return self;
        }

        template <typename Self>
        constexpr Self& operator++(this Self& self) noexcept {
            ++self._value;
            return self;
        }

        template <typename Self>
        constexpr Self& operator--(this Self& self) noexcept {
            --self._value;
            return self;
        }

        template <typename Self>
        [[nodiscard]]
        constexpr Self operator~(this const Self& self) noexcept requires Integral<T> {
            return Self{static_cast<T>(~self.get())};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator&(this const Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() & value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator|(this const Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() | value)};
        }

        template <typename Self, typename U>
        [[nodiscard]]
        constexpr Self operator^(this const Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            return Self{static_cast<T>(self.get() ^ value)};
        }

        template <typename Self, typename U>
        constexpr Self& operator&=(this Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value & value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator|=(this Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value | value);
            return self;
        }

        template <typename Self, typename U>
        constexpr Self& operator^=(this Self& self, U value) noexcept requires Integral<T> && ConvertibleTo<U, T> {
            self._value = static_cast<T>(self._value ^ value);
            return self;
        }

        template <typename Self>
        [[nodiscard]]
        constexpr Self operator-(this const Self& self) noexcept {
            return Self{static_cast<T>(-self.get())};
        }

        template <typename Self>
        [[nodiscard]]
        constexpr Self operator<<(this const Self& self, int shift) noexcept requires Integral<T> {
            return Self{static_cast<T>(self.get() << shift)};
        }

        template <typename Self>
        [[nodiscard]]
        constexpr Self operator>>(this const Self& self, int shift) noexcept requires Integral<T> {
            return Self{static_cast<T>(self.get() >> shift)};
        }

        template <typename Self>
        constexpr Self& operator<<=(this Self& self, int shift) noexcept requires Integral<T> {
            self._value = static_cast<T>(self._value << shift);
            return self;
        }

        template <typename Self>
        constexpr Self& operator>>=(this Self& self, int shift) noexcept requires Integral<T> {
            self._value = static_cast<T>(self._value >> shift);
            return self;
        }

        // Deduced rather than fixed to StrongOrdering: a floating-point member
        // yields PartialOrdering, and a defaulted comparison whose declared
        // return type cannot represent that is defined as deleted - which is
        // what every comparison on Float, Double and Quad used to be.
        [[nodiscard]]
        constexpr auto operator<=>(const Number& other) const noexcept = default;

        [[nodiscard]]
        constexpr operator T() const noexcept {
            return _value;
        }

        [[nodiscard]]
        constexpr T get() const noexcept {
            return _value;
        }

        [[nodiscard]]
        byte byte_value() const noexcept {
            return static_cast<byte>(static_cast<unsigned char>(_value));
        }

        [[nodiscard]]
        static byte byte_value(T x) noexcept {
            return static_cast<byte>(static_cast<unsigned char>(x));
        }

        [[nodiscard]]
        i8 signed_byte_value() const noexcept {
            return static_cast<i8>(_value);
        }

        [[nodiscard]]
        static i8 signed_byte_value(T x) noexcept {
            return static_cast<i8>(x);
        }

        [[nodiscard]]
        u8 unsigned_byte_value() const noexcept {
            return static_cast<u8>(_value);
        }

        [[nodiscard]]
        static u8 unsigned_byte_value(T x) noexcept {
            return static_cast<u8>(x);
        }

        [[nodiscard]]
        i16 short_value() const noexcept {
            return static_cast<i16>(_value);
        }

        [[nodiscard]]
        u16 unsigned_short_value() const noexcept {
            return static_cast<u16>(_value);
        }

        [[nodiscard]]
        i32 int_value() const noexcept {
            return static_cast<i32>(_value);
        }

        [[nodiscard]]
        u32 unsigned_int_value() const noexcept {
            return static_cast<u32>(_value);
        }

        [[nodiscard]]
        i64 long_value() const noexcept {
            return static_cast<i64>(_value);
        }

        [[nodiscard]]
        u64 unsigned_long_value() const noexcept {
            return static_cast<u64>(_value);
        }

        [[nodiscard]]
        f32 float_value() const noexcept {
            return static_cast<f32>(_value);
        }

        [[nodiscard]]
        f64 double_value() const noexcept {
            return static_cast<f64>(_value);
        }

        [[nodiscard]]
        f128 long_double_value() const noexcept {
            return static_cast<f128>(_value);
        }

        [[nodiscard]]
        static Optional<T> parse(StringView s) noexcept {
            T value{};
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
            if (ec != Errc::SUCCESS) {
                return nullopt;
            }
            return value;
        }

        [[nodiscard]]
        static T parse_or(StringView s, T default_value = T{0}) noexcept {
            return parse(s).value_or(default_value);
        }

        [[nodiscard]]
        String to_string() const noexcept {
            return std::format("{}", _value);
        }

        [[nodiscard]]
        static String to_string(T x) noexcept {
            return std::format("{}", x);
        }
    };

    /**
     * @class CharacterBase
     * @brief A base class for character types, providing common character-related methods.
     *
     * The three Unicode enumerations are namespace-scope types aliased in here
     * rather than nested. They describe code points, not the storage type of a
     * character, so there is one of each - nesting them in the template would
     * make CharacterBase<char>::UnicodeNames a different type from
     * CharacterBase<char16>::UnicodeNames, and would instantiate a
     * forty-thousand-enumerator enum once per character type.
     *
     * @tparam Ch The character type (e.g., char, wchar, char16, char32).
     */
    template <CharacterLike Ch>
    class [[nodiscard]] CharacterBase {
    public:
        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_alphanumeric(this const Self& self) noexcept {
            return std::isalnum(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_alphanumeric(Ch c) noexcept {
            return std::isalnum(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_alphabetic(this const Self& self) noexcept {
            return std::isalpha(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_alphabetic(Ch c) noexcept {
            return std::isalpha(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_lower_case(this const Self& self) noexcept {
            return std::islower(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_lower_case(Ch c) noexcept {
            return std::islower(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_upper_case(this const Self& self) noexcept {
            return std::isupper(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_upper_case(Ch c) noexcept {
            return std::isupper(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_digit(this const Self& self) noexcept {
            return std::isdigit(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_digit(Ch c) noexcept {
            return std::isdigit(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_whitespace(this const Self& self) noexcept {
            return std::isspace(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_whitespace(Ch c) noexcept {
            return std::isspace(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_control(this const Self& self) noexcept {
            return std::iscntrl(
                static_cast<unsigned char>(
                    self.get()
                )
            );
        }

        [[nodiscard]]
        static bool is_control(Ch c) noexcept {
            return std::iscntrl(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_printable(this const Self& self) noexcept {
            return std::isprint(static_cast<unsigned char>(
                self.get()
            ));
        }

        [[nodiscard]]
        static bool is_printable(Ch c) noexcept {
            return std::isprint(static_cast<unsigned char>(c));
        }

        template <HasGet<Ch> Self>
        [[nodiscard]]
        bool is_valid_code_point(this const Self& self) noexcept {
            auto c = self.get() >> 16;
            return c < 17;
        }

        [[nodiscard]]
        static bool is_valid_code_point(Ch c) noexcept {
            return (c >> 16) < 17;
        }

        static constexpr Array<char32, 378> BLOCK_STARTS{
            0, 128, 256, 384, 592, 688, 768, 880, 1024, 1280,
            1328, 1424, 1536, 1792, 1872, 1920, 1984, 2048, 2112, 2144,
            2160, 2208, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200,
            3328, 3456, 3584, 3712, 3840, 4096, 4256, 4352, 4608, 4992,
            5024, 5120, 5760, 5792, 5888, 5920, 5952, 5984, 6016, 6144,
            6320, 6400, 6480, 6528, 6624, 6656, 6688, 6832, 6912, 7040,
            7104, 7168, 7248, 7296, 7312, 7360, 7376, 7424, 7552, 7616,
            7680, 7936, 8192, 8304, 8352, 8400, 8448, 8528, 8592, 8704,
            8960, 9216, 9280, 9312, 9472, 9600, 9632, 9728, 9984, 10176,
            10224, 10240, 10496, 10624, 10752, 11008, 11264, 11360, 11392, 11520,
            11568, 11648, 11744, 11776, 11904, 12032, 12256, 12272, 12288, 12352,
            12448, 12544, 12592, 12688, 12704, 12736, 12784, 12800, 13056, 13312,
            19904, 19968, 40960, 42128, 42192, 42240, 42560, 42656, 42752, 42784,
            43008, 43056, 43072, 43136, 43232, 43264, 43312, 43360, 43392, 43488,
            43520, 43616, 43648, 43744, 43776, 43824, 43888, 43968, 44032, 55216,
            55296, 56192, 56320, 57344, 63744, 64256, 64336, 65024, 65040, 65056,
            65072, 65104, 65136, 65280, 65520, 65536, 65664, 65792, 65856, 65936,
            66000, 66048, 66176, 66208, 66272, 66304, 66352, 66384, 66432, 66464,
            66528, 66560, 66640, 66688, 66736, 66816, 66864, 66928, 67008, 67072,
            67456, 67520, 67584, 67648, 67680, 67712, 67760, 67808, 67840, 67872,
            67904, 67968, 68000, 68096, 68192, 68224, 68256, 68288, 68352, 68416,
            68448, 68480, 68528, 68608, 68688, 68736, 68864, 68928, 69216, 69248,
            69312, 69376, 69424, 69488, 69552, 69600, 69632, 69760, 69840, 69888,
            69968, 70016, 70112, 70144, 70224, 70272, 70320, 70400, 70528, 70656,
            70784, 70880, 71040, 71168, 71264, 71296, 71376, 71424, 71504, 71680,
            71760, 71840, 71936, 72032, 72096, 72192, 72272, 72368, 72384, 72448,
            72544, 72704, 72816, 72896, 72960, 73056, 73136, 73440, 73472, 73568,
            73648, 73664, 73728, 74752, 74880, 75088, 77712, 77824, 78896, 78944,
            82944, 83584, 92160, 92736, 92784, 92880, 92928, 93072, 93760, 93856,
            93952, 94112, 94176, 94208, 100352, 101120, 101632, 101760, 110576, 110592,
            110848, 110896, 110960, 111360, 113664, 113824, 113840, 118528, 118736, 118784,
            119040, 119296, 119376, 119488, 119520, 119552, 119648, 119680, 119808, 120832,
            121520, 122624, 122880, 122928, 123024, 123136, 123216, 123536, 123584, 123648,
            124112, 124160, 124896, 124928, 125152, 125184, 125280, 126064, 126144, 126208,
            126288, 126464, 126720, 126976, 127024, 127136, 127232, 127488, 127744, 128512,
            128592, 128640, 128768, 128896, 129024, 129280, 129536, 129648, 129792, 130048,
            131072, 173792, 173824, 177984, 178208, 183984, 191472, 194560, 195104, 196608,
            201552, 205744, 917504, 917632, 917760, 918000, 983040, 1048576,
        }; ///< The starting code points of each Unicode block, used for determining the block of a character.

        static constexpr Array<char32, 1657> SCRIPT_STARTS{
            0, 65, 91, 97, 123, 170, 171, 186, 187, 192,
            215, 216, 247, 248, 697, 736, 741, 746, 748, 768,
            880, 884, 885, 888, 890, 894, 895, 896, 900, 901,
            902, 903, 904, 907, 908, 909, 910, 930, 931, 994,
            1008, 1024, 1157, 1159, 1328, 1329, 1367, 1369, 1419, 1421,
            1424, 1425, 1480, 1488, 1515, 1519, 1525, 1536, 1541, 1542,
            1548, 1549, 1563, 1564, 1567, 1568, 1600, 1601, 1611, 1622,
            1648, 1649, 1757, 1758, 1792, 1806, 1807, 1867, 1869, 1872,
            1920, 1970, 1984, 2043, 2045, 2048, 2094, 2096, 2111, 2112,
            2140, 2142, 2143, 2144, 2155, 2160, 2191, 2192, 2194, 2200,
            2274, 2275, 2304, 2385, 2389, 2404, 2406, 2432, 2436, 2437,
            2445, 2447, 2449, 2451, 2473, 2474, 2481, 2482, 2483, 2486,
            2490, 2492, 2501, 2503, 2505, 2507, 2511, 2519, 2520, 2524,
            2526, 2527, 2532, 2534, 2559, 2561, 2564, 2565, 2571, 2575,
            2577, 2579, 2601, 2602, 2609, 2610, 2612, 2613, 2615, 2616,
            2618, 2620, 2621, 2622, 2627, 2631, 2633, 2635, 2638, 2641,
            2642, 2649, 2653, 2654, 2655, 2662, 2679, 2689, 2692, 2693,
            2702, 2703, 2706, 2707, 2729, 2730, 2737, 2738, 2740, 2741,
            2746, 2748, 2758, 2759, 2762, 2763, 2766, 2768, 2769, 2784,
            2788, 2790, 2802, 2809, 2816, 2817, 2820, 2821, 2829, 2831,
            2833, 2835, 2857, 2858, 2865, 2866, 2868, 2869, 2874, 2876,
            2885, 2887, 2889, 2891, 2894, 2901, 2904, 2908, 2910, 2911,
            2916, 2918, 2936, 2946, 2948, 2949, 2955, 2958, 2961, 2962,
            2966, 2969, 2971, 2972, 2973, 2974, 2976, 2979, 2981, 2984,
            2987, 2990, 3002, 3006, 3011, 3014, 3017, 3018, 3022, 3024,
            3025, 3031, 3032, 3046, 3067, 3072, 3085, 3086, 3089, 3090,
            3113, 3114, 3130, 3132, 3141, 3142, 3145, 3146, 3150, 3157,
            3159, 3160, 3163, 3165, 3166, 3168, 3172, 3174, 3184, 3191,
            3200, 3213, 3214, 3217, 3218, 3241, 3242, 3252, 3253, 3258,
            3260, 3269, 3270, 3273, 3274, 3278, 3285, 3287, 3293, 3295,
            3296, 3300, 3302, 3312, 3313, 3316, 3328, 3341, 3342, 3345,
            3346, 3397, 3398, 3401, 3402, 3408, 3412, 3428, 3430, 3456,
            3457, 3460, 3461, 3479, 3482, 3506, 3507, 3516, 3517, 3518,
            3520, 3527, 3530, 3531, 3535, 3541, 3542, 3543, 3544, 3552,
            3558, 3568, 3570, 3573, 3585, 3643, 3647, 3648, 3676, 3713,
            3715, 3716, 3717, 3718, 3723, 3724, 3748, 3749, 3750, 3751,
            3774, 3776, 3781, 3782, 3783, 3784, 3791, 3792, 3802, 3804,
            3808, 3840, 3912, 3913, 3949, 3953, 3992, 3993, 4029, 4030,
            4045, 4046, 4053, 4057, 4059, 4096, 4256, 4294, 4295, 4296,
            4301, 4302, 4304, 4347, 4348, 4352, 4608, 4681, 4682, 4686,
            4688, 4695, 4696, 4697, 4698, 4702, 4704, 4745, 4746, 4750,
            4752, 4785, 4786, 4790, 4792, 4799, 4800, 4801, 4802, 4806,
            4808, 4823, 4824, 4881, 4882, 4886, 4888, 4955, 4957, 4989,
            4992, 5018, 5024, 5110, 5112, 5118, 5120, 5760, 5789, 5792,
            5867, 5870, 5881, 5888, 5910, 5919, 5920, 5941, 5943, 5952,
            5972, 5984, 5997, 5998, 6001, 6002, 6004, 6016, 6110, 6112,
            6122, 6128, 6138, 6144, 6146, 6148, 6149, 6150, 6170, 6176,
            6265, 6272, 6315, 6320, 6390, 6400, 6431, 6432, 6444, 6448,
            6460, 6464, 6465, 6468, 6480, 6510, 6512, 6517, 6528, 6572,
            6576, 6602, 6608, 6619, 6622, 6624, 6656, 6684, 6686, 6688,
            6751, 6752, 6781, 6783, 6794, 6800, 6810, 6816, 6830, 6832,
            6863, 6912, 6989, 6992, 7039, 7040, 7104, 7156, 7164, 7168,
            7224, 7227, 7242, 7245, 7248, 7296, 7305, 7312, 7355, 7357,
            7360, 7368, 7376, 7379, 7380, 7393, 7394, 7401, 7405, 7406,
            7412, 7413, 7416, 7418, 7419, 7424, 7462, 7467, 7468, 7517,
            7522, 7526, 7531, 7544, 7545, 7615, 7616, 7680, 7936, 7958,
            7960, 7966, 7968, 8006, 8008, 8014, 8016, 8024, 8025, 8026,
            8027, 8028, 8029, 8030, 8031, 8062, 8064, 8117, 8118, 8133,
            8134, 8148, 8150, 8156, 8157, 8176, 8178, 8181, 8182, 8191,
            8192, 8204, 8206, 8293, 8294, 8305, 8306, 8308, 8319, 8320,
            8335, 8336, 8349, 8352, 8385, 8400, 8433, 8448, 8486, 8487,
            8490, 8492, 8498, 8499, 8526, 8527, 8544, 8585, 8588, 8592,
            9255, 9280, 9291, 9312, 10240, 10496, 11124, 11126, 11158, 11159,
            11264, 11360, 11392, 11508, 11513, 11520, 11558, 11559, 11560, 11565,
            11566, 11568, 11624, 11631, 11633, 11647, 11648, 11671, 11680, 11687,
            11688, 11695, 11696, 11703, 11704, 11711, 11712, 11719, 11720, 11727,
            11728, 11735, 11736, 11743, 11744, 11776, 11870, 11904, 11930, 11931,
            12020, 12032, 12246, 12272, 12284, 12288, 12293, 12294, 12295, 12296,
            12321, 12330, 12334, 12336, 12344, 12348, 12352, 12353, 12439, 12441,
            12443, 12445, 12448, 12449, 12539, 12541, 12544, 12549, 12592, 12593,
            12687, 12688, 12704, 12736, 12772, 12784, 12800, 12831, 12832, 12896,
            12927, 13008, 13055, 13056, 13144, 13312, 19904, 19968, 40960, 42125,
            42128, 42183, 42192, 42240, 42540, 42560, 42656, 42744, 42752, 42786,
            42888, 42891, 42955, 42960, 42962, 42963, 42964, 42965, 42970, 42994,
            43008, 43053, 43056, 43066, 43072, 43128, 43136, 43206, 43214, 43226,
            43232, 43264, 43310, 43311, 43312, 43348, 43359, 43360, 43389, 43392,
            43470, 43471, 43472, 43482, 43486, 43488, 43519, 43520, 43575, 43584,
            43598, 43600, 43610, 43612, 43616, 43648, 43715, 43739, 43744, 43767,
            43777, 43783, 43785, 43791, 43793, 43799, 43808, 43815, 43816, 43823,
            43824, 43867, 43868, 43877, 43878, 43882, 43884, 43888, 43968, 44014,
            44016, 44026, 44032, 55204, 55216, 55239, 55243, 55292, 63744, 64110,
            64112, 64218, 64256, 64263, 64275, 64280, 64285, 64311, 64312, 64317,
            64318, 64319, 64320, 64322, 64323, 64325, 64326, 64336, 64451, 64467,
            64830, 64832, 64912, 64914, 64968, 64975, 64976, 65008, 65024, 65040,
            65050, 65056, 65070, 65072, 65107, 65108, 65127, 65128, 65132, 65136,
            65141, 65142, 65277, 65279, 65280, 65281, 65313, 65339, 65345, 65371,
            65382, 65392, 65393, 65438, 65440, 65471, 65474, 65480, 65482, 65488,
            65490, 65496, 65498, 65501, 65504, 65511, 65512, 65519, 65529, 65534,
            65536, 65548, 65549, 65575, 65576, 65595, 65596, 65598, 65599, 65614,
            65616, 65630, 65664, 65787, 65792, 65795, 65799, 65844, 65847, 65856,
            65935, 65936, 65949, 65952, 65953, 66000, 66045, 66046, 66176, 66205,
            66208, 66257, 66272, 66273, 66300, 66304, 66340, 66349, 66352, 66379,
            66384, 66427, 66432, 66462, 66463, 66464, 66500, 66504, 66518, 66560,
            66640, 66688, 66718, 66720, 66730, 66736, 66772, 66776, 66812, 66816,
            66856, 66864, 66916, 66927, 66928, 66939, 66940, 66955, 66956, 66963,
            66964, 66966, 66967, 66978, 66979, 66994, 66995, 67002, 67003, 67005,
            67072, 67383, 67392, 67414, 67424, 67432, 67456, 67462, 67463, 67505,
            67506, 67515, 67584, 67590, 67592, 67593, 67594, 67638, 67639, 67641,
            67644, 67645, 67647, 67648, 67670, 67671, 67680, 67712, 67743, 67751,
            67760, 67808, 67827, 67828, 67830, 67835, 67840, 67868, 67871, 67872,
            67898, 67903, 67904, 67968, 68000, 68024, 68028, 68048, 68050, 68096,
            68100, 68101, 68103, 68108, 68116, 68117, 68120, 68121, 68150, 68152,
            68155, 68159, 68169, 68176, 68185, 68192, 68224, 68256, 68288, 68327,
            68331, 68343, 68352, 68406, 68409, 68416, 68438, 68440, 68448, 68467,
            68472, 68480, 68498, 68505, 68509, 68521, 68528, 68608, 68681, 68736,
            68787, 68800, 68851, 68858, 68864, 68904, 68912, 68922, 69216, 69247,
            69248, 69290, 69291, 69294, 69296, 69298, 69373, 69376, 69416, 69424,
            69466, 69488, 69514, 69552, 69580, 69600, 69623, 69632, 69710, 69714,
            69750, 69759, 69760, 69827, 69837, 69838, 69840, 69865, 69872, 69882,
            69888, 69941, 69942, 69960, 69968, 70007, 70016, 70112, 70113, 70133,
            70144, 70162, 70163, 70210, 70272, 70279, 70280, 70281, 70282, 70286,
            70287, 70302, 70303, 70314, 70320, 70379, 70384, 70394, 70400, 70404,
            70405, 70413, 70415, 70417, 70419, 70441, 70442, 70449, 70450, 70452,
            70453, 70458, 70459, 70460, 70469, 70471, 70473, 70475, 70478, 70480,
            70481, 70487, 70488, 70493, 70500, 70502, 70509, 70512, 70517, 70656,
            70748, 70749, 70754, 70784, 70856, 70864, 70874, 71040, 71094, 71096,
            71134, 71168, 71237, 71248, 71258, 71264, 71277, 71296, 71354, 71360,
            71370, 71424, 71451, 71453, 71468, 71472, 71495, 71680, 71740, 71840,
            71923, 71935, 71936, 71943, 71945, 71946, 71948, 71956, 71957, 71959,
            71960, 71990, 71991, 71993, 71995, 72007, 72016, 72026, 72096, 72104,
            72106, 72152, 72154, 72165, 72192, 72264, 72272, 72355, 72368, 72384,
            72441, 72448, 72458, 72704, 72713, 72714, 72759, 72760, 72774, 72784,
            72813, 72816, 72848, 72850, 72872, 72873, 72887, 72960, 72967, 72968,
            72970, 72971, 73015, 73018, 73019, 73020, 73022, 73023, 73032, 73040,
            73050, 73056, 73062, 73063, 73065, 73066, 73103, 73104, 73106, 73107,
            73113, 73120, 73130, 73440, 73465, 73472, 73489, 73490, 73531, 73534,
            73562, 73648, 73649, 73664, 73714, 73727, 73728, 74650, 74752, 74863,
            74864, 74869, 74880, 75076, 77712, 77811, 77824, 78934, 82944, 83527,
            92160, 92729, 92736, 92767, 92768, 92778, 92782, 92784, 92863, 92864,
            92874, 92880, 92910, 92912, 92918, 92928, 92998, 93008, 93018, 93019,
            93026, 93027, 93048, 93053, 93072, 93760, 93851, 93952, 94027, 94031,
            94088, 94095, 94112, 94176, 94177, 94178, 94180, 94181, 94192, 94194,
            94208, 100344, 100352, 101120, 101590, 101632, 101641, 110576, 110580, 110581,
            110588, 110589, 110591, 110592, 110593, 110880, 110883, 110898, 110899, 110928,
            110931, 110933, 110934, 110948, 110952, 110960, 111356, 113664, 113771, 113776,
            113789, 113792, 113801, 113808, 113818, 113820, 113824, 113828, 118528, 118574,
            118576, 118599, 118608, 118724, 118784, 119030, 119040, 119079, 119081, 119143,
            119146, 119163, 119171, 119173, 119180, 119210, 119214, 119275, 119296, 119366,
            119488, 119508, 119520, 119540, 119552, 119639, 119648, 119673, 119808, 119893,
            119894, 119965, 119966, 119968, 119970, 119971, 119973, 119975, 119977, 119981,
            119982, 119994, 119995, 119996, 119997, 120004, 120005, 120070, 120071, 120075,
            120077, 120085, 120086, 120093, 120094, 120122, 120123, 120127, 120128, 120133,
            120134, 120135, 120138, 120145, 120146, 120486, 120488, 120780, 120782, 120832,
            121484, 121499, 121504, 121505, 121520, 122624, 122655, 122661, 122667, 122880,
            122887, 122888, 122905, 122907, 122914, 122915, 122917, 122918, 122923, 122928,
            122990, 123023, 123024, 123136, 123181, 123184, 123198, 123200, 123210, 123214,
            123216, 123536, 123567, 123584, 123642, 123647, 123648, 124112, 124154, 124896,
            124903, 124904, 124908, 124909, 124911, 124912, 124927, 124928, 125125, 125127,
            125143, 125184, 125260, 125264, 125274, 125278, 125280, 126065, 126133, 126209,
            126270, 126464, 126468, 126469, 126496, 126497, 126499, 126500, 126501, 126503,
            126504, 126505, 126515, 126516, 126520, 126521, 126522, 126523, 126524, 126530,
            126531, 126535, 126536, 126537, 126538, 126539, 126540, 126541, 126544, 126545,
            126547, 126548, 126549, 126551, 126552, 126553, 126554, 126555, 126556, 126557,
            126558, 126559, 126560, 126561, 126563, 126564, 126565, 126567, 126571, 126572,
            126579, 126580, 126584, 126585, 126589, 126590, 126591, 126592, 126602, 126603,
            126620, 126625, 126628, 126629, 126634, 126635, 126652, 126704, 126706, 126976,
            127020, 127024, 127124, 127136, 127151, 127153, 127168, 127169, 127184, 127185,
            127222, 127232, 127406, 127462, 127488, 127489, 127491, 127504, 127548, 127552,
            127561, 127568, 127570, 127584, 127590, 127744, 128728, 128732, 128749, 128752,
            128765, 128768, 128887, 128891, 128986, 128992, 129004, 129008, 129009, 129024,
            129036, 129040, 129096, 129104, 129114, 129120, 129160, 129168, 129198, 129200,
            129202, 129280, 129620, 129632, 129646, 129648, 129661, 129664, 129673, 129680,
            129726, 129727, 129734, 129742, 129756, 129760, 129769, 129776, 129785, 129792,
            129939, 129940, 129995, 130032, 130042, 131072, 173792, 173824, 177978, 177984,
            178206, 178208, 183970, 183984, 191457, 194560, 195102, 196608, 201547, 201552,
            205744, 917505, 917506, 917536, 917632, 917760, 918000,
        }; ///< The starting code points of each Unicode script.
    };

    /**
     * @class FloatingPointBase
     * @brief A base class for floating-point types, providing common methods for handling special floating-point values like NaN and infinity.
     * 
     * @tparam F The floating-point type (e.g., float, double, long double).
     */
    template <FloatingPoint F>
    class [[nodiscard]] FloatingPointBase {
    public:
        static constexpr F EPSILON = NumericLimits<F>::epsilon(); ///< A constant representing the smallest difference between two representable floating-point numbers.
        static constexpr F ROUND_ERROR = NumericLimits<F>::round_error(); ///< A constant representing the maximum rounding error for floating-point types.
        static constexpr F POSITIVE_INFINITY = NumericLimits<F>::infinity(); ///< A constant representing positive infinity for the floating-point type.
        static constexpr F NEGATIVE_INFINITY = -NumericLimits<F>::infinity(); ///< A constant representing negative infinity for the floating-point type.
        static constexpr F NaN = NumericLimits<F>::quiet_NaN(); ///< A constant representing Not-a-Number (NaN) for the floating-point type.
        static constexpr F SIGNALLING_NaN = NumericLimits<F>::signaling_NaN(); ///< A constant representing a signaling NaN for the floating-point type.
        static constexpr F MIN_SUBNORMAL = NumericLimits<F>::denorm_min(); ///< A constant representing the minimum positive subnormal _value for the floating-point type.

        template <HasGet<F> Self>
        [[nodiscard]]
        bool is_nan(this const Self& self) noexcept {
            return std::isnan(self.get());
        }

        template <HasGet<F> Self>
        [[nodiscard]]
        bool is_infinite(this const Self& self) noexcept {
            return std::isinf(self.get());
        }

        template <HasGet<F> Self>
        [[nodiscard]]
        bool is_finite(this const Self& self) noexcept {
            return std::isfinite(self.get());
        }
    };
}

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    class [[nodiscard]] Boolean final {
    private:
        bool _value = false;
    public:
        static const Boolean TRUE; ///< A constant representing the boolean value true.
        static const Boolean FALSE; ///< A constant representing the boolean value false.

        constexpr Boolean() = default;

        constexpr Boolean(bool value) noexcept:
            _value{value} {}

        constexpr ~Boolean() = default;

        constexpr Boolean& operator=(bool value) noexcept {
            _value = value;
            return *this;
        }

        constexpr Boolean(const Boolean&) = default;
        constexpr Boolean(Boolean&&) noexcept = default;
        constexpr Boolean& operator=(const Boolean&) = default;
        constexpr Boolean& operator=(Boolean&&) noexcept = default;

        [[nodiscard]]
        constexpr operator bool() const noexcept {
            return _value;
        }

        [[nodiscard]]
        constexpr bool get() const noexcept {
            return _value;
        }

        [[nodiscard]]
        constexpr Boolean operator!() const noexcept {
            return Boolean{!_value};
        }

        [[nodiscard]]
        static Optional<bool> parse(StringView s) noexcept {
            if (s == "true" || s == "1") {
                return true;
            } else if (s == "false" || s == "0") {
                return false;
            }
            return nullopt;
        }

        [[nodiscard]]
        static bool parse_or(StringView s, bool default_value = false) noexcept {
            return parse(s).value_or(default_value);
        }

        [[nodiscard]]
        String to_string() const noexcept {
            return _value ? "true" : "false";
        }

        [[nodiscard]]
        static String to_string(bool x) noexcept {
            return x ? "true" : "false";
        }
    };

    inline constexpr Boolean Boolean::TRUE = Boolean(true);
    inline constexpr Boolean Boolean::FALSE = Boolean(false);

    class [[nodiscard]] SignedByte final: public Number<i8> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator i8() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Byte final: public Number<u8> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator u8() const noexcept {
            return _value;
        }

        template <typename Int>
        [[nodiscard]]
        static constexpr Int to_integer(byte b) noexcept {
            return std::to_integer<Int>(b);
        }
    };

    class [[nodiscard]] Short final: public Number<i16> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator i16() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Integer final: public Number<i32> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator i32() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Long final: public Number<i64> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator i64() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] UnsignedShort final: public Number<u16> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator u16() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] UnsignedInteger final: public Number<u32> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator u32() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] UnsignedLong final: public Number<u64> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator u64() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Character final: public Number<char>, public CharacterBase<char> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator char() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] UnsignedCharacter final: public Number<unsigned char>, public CharacterBase<unsigned char> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator unsigned char() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] Character8 final: public Number<char8>, public CharacterBase<char8> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator char8() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] Character16 final: public Number<char16>, public CharacterBase<char16> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator char16() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] Character32 final: public Number<char32>, public CharacterBase<char32> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator char32() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] WideCharacter final: public Number<wchar>, public CharacterBase<wchar> {
    public:
        using Number::Number;

        using UnicodeBlock = stdx::core::UnicodeBlock;
        using UnicodeNames = stdx::core::UnicodeNames;
        using UnicodeScript = stdx::core::UnicodeScript;

        [[nodiscard]]
        constexpr operator wchar() const noexcept {
            return _value;
        }

        static constexpr Array BLOCK_STARTS = CharacterBase::BLOCK_STARTS;
        static constexpr Array SCRIPT_STARTS = CharacterBase::SCRIPT_STARTS;
    };

    class [[nodiscard]] SignedSize final: public Number<isize> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator isize() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] UnsignedSize final: public Number<usize> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator usize() const noexcept {
            return _value;
        }
    };

    #ifdef __STDCPP_FLOAT16_T__
    class [[nodiscard]] Half final: public Number<f16>, public FloatingPointBase<f16> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator f16() const noexcept {
            return _value;
        }
    };
    #endif

    #ifdef __STDCPP_BFLOAT16_T__
    class [[nodiscard]] BrainHalf final: public Number<bf16>, public FloatingPointBase<bf16> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator bf16() const noexcept {
            return _value;
        }
    };
    #endif

    class [[nodiscard]] Float final: public Number<f32>, public FloatingPointBase<f32> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator f32() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Double final: public Number<f64>, public FloatingPointBase<f64> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator f64() const noexcept {
            return _value;
        }
    };

    class [[nodiscard]] Quad final: public Number<f128>, public FloatingPointBase<f128> {
    public:
        using Number::Number;

        [[nodiscard]]
        constexpr operator f128() const noexcept {
            return _value;
        }
    };

    using Integer8 = SignedByte;
    using Integer16 = Short;
    using Integer32 = Integer;
    using Integer64 = Long;
    using UnsignedInteger8 = Byte;
    using UnsignedInteger16 = UnsignedShort;
    using UnsignedInteger32 = UnsignedInteger;
    using UnsignedInteger64 = UnsignedLong;
    #ifdef __STDCPP_FLOAT16_T__
    using Float16 = Half;
    #endif
    #ifdef __STDCPP_BFLOAT16_T__
    using BrainFloat16 = BrainHalf;
    #endif
    using Float32 = Float;
    using Float64 = Double;
    using Float128 = Quad;

    /**
     * @internal
     * A wrapper must cost exactly what it wraps. These held until the base
     * acquired a virtual destructor and nine virtual conversions, at which
     * point every wrapper grew a vtable pointer - Integer became 16 bytes for
     * four bytes of payload, and Character 16 for one. Nothing noticed, because
     * nothing in the tree ever constructs one. Asserted here so a vtable cannot
     * come back unremarked.
     */
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
    STDLIBX_ASSERT_NUMBER_IS_FREE(Character8, char8);
    STDLIBX_ASSERT_NUMBER_IS_FREE(Character16, char16);
    STDLIBX_ASSERT_NUMBER_IS_FREE(Character32, char32);
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

    /**
     * @internal
     * The explicit object parameter has to deduce the leaf, not the base. If it
     * ever deduced Number<i32> instead, arithmetic would silently start
     * returning the base class, and the wrapper would evaporate one operation
     * into an expression.
     */
    static_assert(IsSameValue<decltype(Integer{1} + 1), Integer>);
    static_assert(IsSameValue<decltype(Long{1} * 2), Long>);
    static_assert(IsSameValue<decltype(-Float{1.0f}), Float>);
    static_assert(IsSameValue<decltype(~Byte{0}), Byte>);
    static_assert(IsSameValue<decltype(Character{'a'} + 1), Character>);
    static_assert(IsSameValue<decltype(++Ops::declval<Integer&>()), Integer&>);

    /// The operations must survive constant evaluation, which is most of the point of a wrapper.
    static_assert((Integer{40} + 2).get() == 42);
    static_assert((Integer{7} % 4).get() == 3);
    static_assert((Byte{0b1010} & 0b0110).get() == 0b0010);
    static_assert((Short{1} << 4).get() == 16);
    static_assert(Character{'a'}.get() == 'a');
    static_assert(Integer{1} < Integer{2});
    static_assert(Double{1.5} == Double{1.5});
}
