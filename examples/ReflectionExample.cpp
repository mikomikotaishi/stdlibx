#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::collections::EnumSet;
using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::linq::Query;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::AccessFlag;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::CvQualifier;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Enumerator;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::FunctionSpecifier;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::Method;
using stdx::meta::reflect::ReflectableClass;
using stdx::meta::reflect::ReflectionOf;
using stdx::meta::reflect::Type;
using stdx::util::ArgumentParser;
using stdx::util::CommandLineParserException;
using stdx::util::Description;
using stdx::util::Env;
using stdx::util::ShortName;

namespace reflect = stdx::meta::reflect;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef __cpp_lib_reflection
/**
 * @brief Example function to demonstrate reflection capabilities.
 * @param s A string view to calculate the length of.
 * @return The length of the input string view.
 */
[[nodiscard]]
consteval usize length(StringView s) {
    return s.length();
}

/**
 * @struct Vec2
 * @brief Example class for reflection testing.
 */
struct Vec2 {
    f64 x; ///< The x-coordinate.
    f64 y; ///< The y-coordinate.

    constexpr Vec2() noexcept = default;

    constexpr Vec2(f64 x, f64 y) noexcept:
        x{x}, y{y} {}

    [[nodiscard]]
    constexpr f64 norm_squared() const noexcept {
        return x * x + y * y;
    }

    [[nodiscard]]
    constexpr Vec2 operator+(Vec2 other) const noexcept {
        return Vec2(x + other.x, y + other.y);
    }
};

/**
 * @enum Suit
 * @brief Example enum for reflection testing.
 */
enum class Suit: u8 {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
};

/**
 * @struct BasicOptions
 * @brief Basic options for the reflection test.
 * 
 * Argument structs exercised by ArgumentParser::parse / help below.
 * They are plain aggregates: field names map to long flags (snake_case and
 * camelCase both kebab-cased), annotations add short flags, help text,
 * and env fallbacks.
 */
struct BasicOptions {
    [[=ShortName<'i'>()]]
    [[=Description<"input file">()]]
    String input;

    [[=ShortName<'n'>()]]
    [[=Description<"how many">()]]
    i32 count;

    [[=ShortName<'v'>()]]
    bool verbose = false;

    Optional<i32> limit;

    String log_level = "info";

    i32 maxRetries = 3;
};

/**
 * @struct EnvOptions
 * @brief Options with environment variable fallbacks.
 */
struct EnvOptions {
    [[=Env<"REFTEST_TOKEN">()]]
    String token = "default-token";
};

/**
 * @struct EnvRequired
 * @brief Options with a required environment variable (no default value).
 */
struct EnvRequired {
    [[=Env<"REFTEST_MISSING">()]]
    String value;
};
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    // Example:
    // ReflectionExample --input data.csv --count 42 --verbose --limit 100
    System::out.println("ArgumentParser::help<BasicOptions>:");
    System::out.println("{}", ArgumentParser::help<BasicOptions>(argc, argv));

    try {
        const BasicOptions opts = ArgumentParser::parse<BasicOptions>(argc, argv);
        System::out.println("Parsed BasicOptions:");
        System::out.println("  input      = {}", opts.input);
        System::out.println("  count      = {}", opts.count);
        System::out.println("  verbose    = {}", opts.verbose);
        if (opts.limit.has_value()) {
            System::out.println("  limit      = {}", *opts.limit);
        } else {
            System::out.println("  limit      = (unset)");
        }
        System::out.println("  log_level  = {}", opts.log_level);
        System::out.println("  maxRetries = {}", opts.maxRetries);
    } catch (const CommandLineParserException& e) {
        System::out.println("No valid arguments supplied ({}).", e.what());
        System::out.println("Pass --input <file> --count <n> to see them parsed.");
    }

    constexpr AccessContext ctx = AccessContext::unchecked();
    constexpr StringView s = "Hello, world!";
    constexpr Info m1 = ^^s;
    constexpr Info m2 = Ops::info<String>(); // Necessary, as reflecting a using-declaration is illegal
    constexpr Info m3 = ctx.designating_class();
    usize len = length([:m1:]);
    HashMap<typename [:m2:], u64> names = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    System::out.println("Length of '{}': {}", s, len);
    System::out.println(names);

    constexpr Info MATH_CLASS = Ops::info<Math>();
    constexpr Info SYSTEM_CLASS = Ops::info<System>();
    static constexpr Span<const Info> MATH_CONSTANTS = Query(Ops::define_static_array(reflect::members_of(MATH_CLASS, ctx)))
        .where([](Info m) -> bool { return reflect::is_public(m); })
        .where([](Info m) -> bool { return reflect::is_variable(m); })
        .where([](Info m) -> bool { return reflect::is_const(m); })
        .where([](Info m) -> bool { return reflect::has_static_storage_duration(m); })
        .to_array();
    static constexpr Span<const Info> SYSTEM_MEMBERS = Query(Ops::define_static_array(reflect::members_of(SYSTEM_CLASS, ctx)))
        .where([](Info m) -> bool { return reflect::has_identifier(m); })
        .to_array();

    static constinit Array<Pair<StringView, f64>, MATH_CONSTANTS.size()> math_constants;
    static constinit usize index = 0;
    template for (constexpr Info mem: MATH_CONSTANTS) {
        math_constants[index++] = Ops::pair(reflect::identifier_of(mem), [:mem:]);
    }

    static constinit Vector<StringView> system_members;
    template for (constexpr Info mem: SYSTEM_MEMBERS) {
        system_members.push_back(reflect::identifier_of(mem));
    }

    System::out.println("Constants of Math class: {}", math_constants);
    System::out.println("Members of System class: {}", system_members);

    System::out.println();
    System::out.println("Class<Vec2>:");
    constexpr Class<Vec2> VEC2_CLASS = Ops::class_of<Vec2>();
    constexpr EnumSet<ReflectionOf> VEC2_KINDS = VEC2_CLASS.kinds();
    System::out.println("name: {}", VEC2_CLASS.name().value());
    System::out.println("size/align: {} / {} bytes", VEC2_CLASS.size(), VEC2_CLASS.alignment());
    System::out.println("aggregate: {}", VEC2_CLASS.is_aggregate());
    System::out.println("trivially copyable: {}", VEC2_CLASS.is_trivially_copyable());
    System::out.println("kinds: {}", VEC2_KINDS);

    static constexpr Span<const Field> VEC2_FIELDS = Ops::define_static_array(VEC2_CLASS.fields(ctx));
    System::out.println("Fields:");
    template for (constexpr Field f: VEC2_FIELDS) {
        constexpr StringView name = f.name().value_or("unnamed field");
        constexpr StringView type_name = f.type().display_name();
        constexpr usize offset = f.offset().bytes;
        constexpr usize access = Ops::to_underlying(f.access());
        System::out.println("  {} : {} at +{} (access={})", name, type_name, offset, access);
    }

    static constexpr Span<const Method> VEC2_METHODS = Query(Ops::define_static_array(VEC2_CLASS.methods(ctx)))
        .where([](Method m) -> bool { return m.name().has_value(); })
        .to_array();
    System::out.println("Methods:");
    template for (constexpr Method m: VEC2_METHODS) {
        constexpr StringView name = m.name().value_or("unnamed method");
        if constexpr (m.is_operator()) {
            constexpr StringView sym = m.operator_symbol();
            System::out.println("  {} -> operator '{}'", name, sym);
        } else {
            constexpr bool const_qualified = m.cv_qualifiers().contains(CvQualifier::CONST);
            constexpr bool noexcept_marked = m.specifiers().contains(FunctionSpecifier::NOEXCEPT);
            System::out.println("  {} (const: {}, noexcept: {})", name, const_qualified, noexcept_marked);
        }
    }

    System::out.println();
    System::out.println("Enum<Suit>:");
    constexpr Enum<Suit> SUIT_ENUM = Ops::enum_of<Suit>();
    System::out.println("name: {}", SUIT_ENUM.name().value_or("unnamed enum"));
    System::out.println("scoped: {}", SUIT_ENUM.is_scoped());

    static constexpr Span<const Enumerator> SUITS = Ops::define_static_array(SUIT_ENUM.enumerators());
    System::out.println("Enumerators:");
    template for (constexpr Enumerator e: SUITS) {
        constexpr StringView name = e.name().value_or("unnamed enumerator");
        constexpr usize value = Ops::to_underlying(e.as<Suit>());
        System::out.println("  {} = {}", name, value);
    }

    System::out.println();
    System::out.println("EnumSet<Suit>:");
    constexpr EnumSet<Suit> REDS = EnumSet<Suit>::of(Suit::DIAMONDS, Suit::HEARTS);
    constexpr EnumSet<Suit> BLACKS = ~REDS;
    System::out.println("|reds| = {}, contains HEARTS: {}", REDS.size(), REDS.contains(Suit::HEARTS));
    System::out.println("|blacks| = {}, contains SPADES: {}", BLACKS.size(), BLACKS.contains(Suit::SPADES));
    System::out.println("reds | blacks fully covers Suit? {}", (REDS | BLACKS).is_full());

    System::out.println();
    constexpr Type INT_TYPE = Ops::type_of<i32>();
    System::out.println(
        "{} (integral: {}, signed: {}, size: {} bytes)",
        INT_TYPE.display_name(),
        INT_TYPE.is_integral(),
        INT_TYPE.is_signed(),
        INT_TYPE.size()
    );
    #else
    System::out.println("Example disabled (compiler does not support reflection).");
    #endif
}
