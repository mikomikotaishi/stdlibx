#include <version>

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
    expect_eq(Ops::fmt("{:#}", Integer(5)), "5", "Wrapper formats with raw value with :# specifier");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Wrappers.basic_operations", test_basic_operations},
        {"Wrappers.formatters", test_formatters},
    });
}
