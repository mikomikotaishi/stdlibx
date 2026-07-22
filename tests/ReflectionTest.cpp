#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::collections::EnumMap;
using stdx::collections::EnumSet;
using stdx::collections::Vector;
using stdx::linq::Query;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::CvQualifier;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Enumerator;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::FunctionSpecifier;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::Method;
using stdx::meta::reflect::ReflectableClass;
using stdx::meta::reflect::ThrownExceptions;
using stdx::meta::reflect::Type;
using stdx::util::ArgumentParser;
using stdx::util::CommandLineParserException;
using stdx::util::Description;
using stdx::util::Env;
using stdx::util::ShortName;

namespace reflect = stdx::meta::reflect;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef __cpp_impl_reflection
[[nodiscard]]
consteval usize length(StringView s) {
    return s.length();
}

struct Vec2 {
    f64 x;
    f64 y;

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

enum class Suit: u8 {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
};

/**
 * @struct BasicOptions
 * @brief Argument struct exercised by ArgumentParser::parse / help.
 *
 * A plain aggregate: field names map to long flags (snake_case and camelCase
 * both kebab-cased), annotations add short flags, help text, and env fallbacks.
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

struct EnvOptions {
    [[=Env<"REFTEST_TOKEN">()]]
    String token = "default-token";
};

struct EnvRequired {
    [[=Env<"REFTEST_MISSING">()]]
    String value;
};

// Fixtures for the Throws annotation: a free function carrying two exception
// types, an unannotated function, and a callable type annotating its operator().
// Real exception classes (not the std:: alias typedefs) are used so reflection
// identity is unambiguous.
[[=Throws<ArithmeticException, InvalidOperationException>()]]
i32 annotated_thrower(i32 x) {
    return x;
}

i32 unannotated_function(i32 x) {
    return x;
}

struct AnnotatedCallable {
    [[=Throws<ArithmeticException>()]]
    void operator()() const {}
};

[[nodiscard]]
Vector<char*> make_argv(InitializerList<const char*> tokens) {
    Vector<char*> argv;
    for (const char* token: tokens) {
        argv.push_back(const_cast<char*>(token));
    }
    return argv;
}

template <ReflectableClass T>
[[nodiscard]]
T parse_tokens(InitializerList<const char*> tokens) {
    Vector<char*> argv = make_argv(tokens);
    return ArgumentParser::parse<T>(static_cast<i32>(argv.size()), argv.data());
}

/**
 * @brief Exercises the reflection-driven ArgumentParser: every kind of field, short
 * and long spellings, error paths, help text, and environment fallbacks.
 */
void test_argument_parser() {
    // Long flags, every kind of field populated.
    {
        const BasicOptions o = parse_tokens<BasicOptions>({
            "reftest", "--input", "data.txt", "--count", "3", "--verbose",
            "--limit", "5", "--log-level", "debug", "--max-retries", "8"
        });
        expect(o.input == "data.txt", "long flag: string value");
        expect(o.count == 3, "long flag: integer value");
        expect(o.verbose, "long flag: bool present is true");
        expect(o.limit.has_value() && *o.limit == 5, "long flag: optional populated");
        expect(o.log_level == "debug", "snake_case maps to --kebab-case");
        expect(o.maxRetries == 8, "camelCase maps to --kebab-case");
    }

    // Short flags, with optionals/defaults left untouched.
    {
        const BasicOptions o = parse_tokens<BasicOptions>({"reftest", "-i", "f.txt", "-n", "7"});
        expect(o.input == "f.txt", "short flag: string value");
        expect(o.count == 7, "short flag: integer value");
        expect(!o.verbose, "bool absent is false");
        expect(!o.limit.has_value(), "optional absent is empty");
        expect(o.log_level == "info", "defaulted field keeps default");
        expect(o.maxRetries == 3, "defaulted camelCase keeps default");
    }

    // A required field with no value supplied is an error.
    expect_throws<CommandLineParserException>(
        [] -> void { (void)parse_tokens<BasicOptions>({"reftest"}); },
        "missing required argument throws"
    );

    // The same option in both spellings is ambiguous.
    expect_throws<CommandLineParserException>(
        [] -> void { (void)parse_tokens<BasicOptions>({"reftest", "-i", "a", "--input", "b"}); },
        "one field via both spellings throws"
    );

    // A value-taking flag given without its value is an error.
    expect_throws<CommandLineParserException>(
        [] -> void { (void)parse_tokens<BasicOptions>({"reftest", "-i", "x", "--count"}); },
        "value flag missing its value throws"
    );

    // help() lists every field with its flags, type, and markers.
    {
        Vector<char*> argv = make_argv({"reftest"});
        const String usage = ArgumentParser::help<BasicOptions>(
            static_cast<i32>(argv.size()), argv.data()
        );
        expect(usage.find("--input") != String::npos, "help lists long flag");
        expect(usage.find("-i, --input") != String::npos, "help pairs short and long");
        expect(usage.find("input file") != String::npos, "help shows description");
        expect(usage.find("[type: string]") != String::npos, "help shows string type");
        expect(usage.find("[type: number]") != String::npos, "help shows number type");
        expect(usage.find("[optional]") != String::npos, "help marks optional field");
        expect(usage.find("[default:") != String::npos, "help shows default");
    }

    // Env fallback: default kept when neither CLI nor environment provides it.
    {
        Environment::unset("REFTEST_TOKEN");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest"});
        expect(o.token == "default-token", "env unset keeps default");
    }
    // Env fallback: the environment supplies the value when the flag is absent.
    {
        Environment::set("REFTEST_TOKEN", "from-env");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest"});
        expect(o.token == "from-env", "environment supplies value");
        Environment::unset("REFTEST_TOKEN");
    }
    // Env fallback: the command line still wins over the environment.
    {
        Environment::set("REFTEST_TOKEN", "from-env");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest", "--token", "from-cli"});
        expect(o.token == "from-cli", "command line overrides environment");
        Environment::unset("REFTEST_TOKEN");
    }
    // A required env-backed field with the variable unset is an error.
    {
        Environment::unset("REFTEST_MISSING");
        expect_throws<CommandLineParserException>(
            [] -> void { (void)parse_tokens<EnvRequired>({"reftest"}); },
            "required env-backed field unset throws"
        );
    }
}

/**
 * @brief Asserts on the reflection class wrappers (Class, Field, Method, Enum,
 * Enumerator, EnumSet, Type). Each reflected property is evaluated into a
 * constexpr value first, then checked at runtime - the accessors are
 * compile-time only.
 */
void test_reflection_classes() {
    constexpr AccessContext ctx = AccessContext::unchecked();

    // Reflect an expression with ^^ and splice it back with [: :].
    constexpr StringView s = "Hello, world!";
    constexpr Info m1 = ^^s;
    constexpr usize spliced_length = length([:m1:]);
    expect(spliced_length == s.length(), "splicing a reflected StringView recovers its length");

    // Class<Vec2>: structural facts cross-checked against the language itself.
    constexpr Class<Vec2> VEC2_CLASS = Ops::class_of<Vec2>();
    constexpr StringView vec2_name = VEC2_CLASS.name().value();
    constexpr usize vec2_size = VEC2_CLASS.size();
    constexpr usize vec2_align = VEC2_CLASS.alignment();
    constexpr bool vec2_trivially_copyable = VEC2_CLASS.is_trivially_copyable();
    constexpr bool vec2_aggregate = VEC2_CLASS.is_aggregate();
    expect_eq(vec2_name, "Vec2", "Class<Vec2> reports its name");
    expect(vec2_size == sizeof(Vec2), "Class<Vec2> size matches sizeof(Vec2)");
    expect(vec2_align == alignof(Vec2), "Class<Vec2> alignment matches alignof(Vec2)");
    expect(vec2_trivially_copyable, "Vec2 is trivially copyable");
    expect(!vec2_aggregate, "Vec2 is not an aggregate (it has user-declared constructors)");

    static constexpr Span<const Field> VEC2_FIELDS = Ops::define_static_array(VEC2_CLASS.fields(ctx));
    constexpr usize field_count = VEC2_FIELDS.size();
    constexpr StringView field0_name = VEC2_FIELDS[0].name().value_or("");
    constexpr StringView field1_name = VEC2_FIELDS[1].name().value_or("");
    constexpr usize field0_offset = VEC2_FIELDS[0].offset().bytes;
    constexpr usize field1_offset = VEC2_FIELDS[1].offset().bytes;
    expect_eq(field_count, 2uz, "Vec2 has two fields");
    expect_eq(field0_name, "x", "first field is x");
    expect_eq(field1_name, "y", "second field is y");
    expect(field0_offset == 0, "field x is at offset 0");
    expect(field1_offset == sizeof(f64), "field y is at offset sizeof(f64)");

    // Keep named methods and operators (an operator reports no identifier, so a
    // bare name().has_value() filter would drop operator+).
    static constexpr Span<const Method> VEC2_METHODS = Query(Ops::define_static_array(VEC2_CLASS.methods(ctx)))
        .where([](Method m) -> bool { return m.name().has_value() || m.is_operator(); })
        .to_array();
    bool found_norm_squared = false;
    bool found_operator_plus = false;
    template for (constexpr Method m: VEC2_METHODS) {
        constexpr StringView name = m.name().value_or("");
        if constexpr (m.is_operator()) {
            constexpr StringView symbol = m.operator_symbol();
            if (symbol == "+") {
                found_operator_plus = true;
            }
        } else if constexpr (name == "norm_squared") {
            found_norm_squared = true;
            constexpr bool is_const = m.cv_qualifiers().contains(CvQualifier::CONST);
            constexpr bool is_noexcept = m.specifiers().contains(FunctionSpecifier::NOEXCEPT);
            expect(is_const, "norm_squared is const-qualified");
            expect(is_noexcept, "norm_squared is noexcept");
        }
    }
    expect(found_norm_squared, "Vec2 exposes a norm_squared method");
    expect(found_operator_plus, "Vec2 exposes operator+");

    // Enum<Suit> and its enumerators.
    constexpr Enum<Suit> SUIT_ENUM = Ops::enum_of<Suit>();
    constexpr StringView suit_name = SUIT_ENUM.name().value_or("");
    constexpr bool suit_scoped = SUIT_ENUM.is_scoped();
    expect_eq(suit_name, "Suit", "Enum<Suit> reports its name");
    expect(suit_scoped, "Suit is a scoped enum");

    static constexpr Span<const Enumerator> SUITS = Ops::define_static_array(SUIT_ENUM.enumerators());
    constexpr usize suit_count = SUITS.size();
    constexpr StringView first_suit = SUITS[0].name().value_or("");
    constexpr u8 first_suit_value = Ops::to_underlying(SUITS[0].as<Suit>());
    constexpr StringView last_suit = SUITS[3].name().value_or("");
    constexpr u8 last_suit_value = Ops::to_underlying(SUITS[3].as<Suit>());
    expect_eq(suit_count, 4uz, "Suit has four enumerators");
    expect_eq(first_suit, "CLUBS", "first enumerator is CLUBS");
    expect(first_suit_value == 0, "CLUBS == 0");
    expect_eq(last_suit, "SPADES", "last enumerator is SPADES");
    expect(last_suit_value == 3, "SPADES == 3");

    // EnumSet<Suit> algebra.
    constexpr EnumSet<Suit> REDS = EnumSet<Suit>::of(Suit::DIAMONDS, Suit::HEARTS);
    constexpr EnumSet<Suit> BLACKS = ~REDS;
    expect_eq(REDS.size(), 2uz, "two red suits");
    expect(REDS.contains(Suit::HEARTS), "reds contains HEARTS");
    expect(!REDS.contains(Suit::CLUBS), "reds does not contain CLUBS");
    expect_eq(BLACKS.size(), 2uz, "two black suits");
    expect(BLACKS.contains(Suit::CLUBS) && BLACKS.contains(Suit::SPADES), "blacks are clubs and spades");
    expect((REDS | BLACKS).is_full(), "reds | blacks covers every suit");

    // Type queries.
    constexpr Type INT_TYPE = Ops::type_of<i32>();
    constexpr bool int_integral = INT_TYPE.is_integral();
    constexpr bool int_signed = INT_TYPE.is_signed();
    constexpr usize int_size = INT_TYPE.size();
    expect(int_integral, "i32 is integral");
    expect(int_signed, "i32 is signed");
    expect(int_size == sizeof(i32), "i32 size matches sizeof(i32)");
}

/**
 * @brief Exercises the Throws annotation: gathering declared exception types from a
 * function and from a callable type, and the empty case.
 */
void test_throws_annotation() {
    // thrown_exceptions returns Tuple<Class<E>...>: each element keeps its concrete
    // exception type. Identity is checked via Class<E>::VALUE (== ^^E), parenthesised
    // so the ^^ operator does not greedily read a following && as a reference type-id.

    // A function's Throws annotation yields its exception types, in order.
    constexpr ThrownExceptions<^^annotated_thrower> declared = Ops::thrown_exceptions<^^annotated_thrower>();
    using Declared = decltype(declared);
    constexpr bool declared_order =
        (TupleElementType<0, Declared>::VALUE == ^^ArithmeticException) &&
        (TupleElementType<1, Declared>::VALUE == ^^InvalidOperationException);
    expect_eq(TupleSize<Declared>::value, 2uz, "annotated_thrower declares two exceptions");
    expect(declared_order, "annotated_thrower declares Class<ArithmeticException> then Class<InvalidOperationException>");

    // No Throws annotation means an empty tuple.
    using None = decltype(Ops::thrown_exceptions<^^unannotated_function>());
    expect_eq(TupleSize<None>::value, 0uz, "unannotated_function declares no exceptions");

    // A callable type is queried through its operator().
    constexpr ThrownExceptions<^^AnnotatedCallable> from_callable = Ops::thrown_exceptions<^^AnnotatedCallable>();
    using FromCallable = decltype(from_callable);
    constexpr bool callable_ok = (TupleElementType<0, FromCallable>::VALUE == ^^ArithmeticException);
    expect_eq(TupleSize<FromCallable>::value, 1uz, "callable type declares one exception");
    expect(callable_ok, "callable type declares Class<ArithmeticException> via operator()");

    // The annotation is only applicable to functions and callables.
    constexpr bool function_ok = FunctionOrCallable<^^annotated_thrower>;
    constexpr bool callable_type_ok = FunctionOrCallable<^^AnnotatedCallable>;
    constexpr bool non_callable_rejected = !FunctionOrCallable<^^i32>;
    expect(function_ok, "a function is function-or-callable");
    expect(callable_type_ok, "a callable type is function-or-callable");
    expect(non_callable_rejected, "a non-callable type is not function-or-callable");
}

// EnumMap is usable in a constant expression: build, index, mutate, query.
static_assert(
    [] -> bool {
        EnumMap<Suit, i32> m;
        m[Suit::CLUBS] = 1;
        m[Suit::HEARTS] = 3;
        m.erase(Suit::CLUBS);
        return m.size() == 1
            && m.contains(Suit::HEARTS)
            && !m.contains(Suit::CLUBS);
    }(),
    "EnumMap is usable in constant expressions"
);

/**
 * @brief Exercises EnumMap<Suit, V>: the STL-style API (insert_or_assign/get/erase,
 * keyed by enumerator), iteration in declaration order, and the formatter.
 */
void test_enum_map() {
    expect_eq(EnumMap<Suit, i32>::capacity(), 4uz, "capacity is the enumerator count");

    // An empty map.
    EnumMap<Suit, i32> empty;
    expect(empty.empty(), "a fresh map is empty");
    expect_eq(empty.size(), 0uz, "a fresh map has size zero");
    expect(!empty.contains(Suit::CLUBS), "a fresh map contains no keys");
    expect(empty.find(Suit::CLUBS) == nullptr, "find on an absent key is nullptr");
    expect_throws<OutOfRangeException>(
        [&] -> void { (void)empty.at(Suit::CLUBS); },
        "at on an absent key throws OutOfRangeException"
    );

    // insert_or_assign returns the previous mapping (empty when newly inserted).
    EnumMap<Suit, i32> m;
    expect(!m.insert_or_assign(Suit::CLUBS, 1).has_value(), "inserting a new key has no previous value");
    Optional<i32> previous = m.insert_or_assign(Suit::CLUBS, 2);
    require(previous.has_value(), "overwriting returns the previous value");
    expect_eq(*previous, 1, "the previous value is the old mapping");
    expect_eq(m.size(), 1uz, "overwriting does not grow the map");

    // at / find / contains_value.
    m.insert_or_assign(Suit::HEARTS, 7);
    expect(m.contains(Suit::HEARTS), "contains finds a present key");
    expect_eq(m.at(Suit::HEARTS), 7, "at returns the mapped value");
    expect_throws<OutOfRangeException>(
        [&] -> void { (void)m.at(Suit::SPADES); },
        "at on an absent key throws OutOfRangeException"
    );
    require(m.find(Suit::HEARTS) != nullptr, "find returns a pointer to a present value");
    expect_eq(*m.find(Suit::HEARTS), 7, "the found pointer dereferences to the value");
    expect(m.contains_value(7), "contains_value finds a stored value");
    expect(!m.contains_value(8), "contains_value rejects an absent value");

    // operator[] inserts a default value for an absent key.
    EnumMap<Suit, i32> idx;
    idx[Suit::CLUBS] = 5;
    expect_eq(idx[Suit::CLUBS], 5, "operator[] reads back what was assigned");
    i32& slot = idx[Suit::DIAMONDS];
    expect_eq(slot, 0, "operator[] default-inserts for an absent key");
    expect_eq(idx.size(), 2uz, "operator[] grows the map on insertion");

    // erase returns the previous mapping and shrinks the map.
    EnumMap<Suit, i32> r;
    r.insert_or_assign(Suit::CLUBS, 1);
    Optional<i32> removed = r.erase(Suit::CLUBS);
    require(removed.has_value(), "erase returns the previous value");
    expect_eq(*removed, 1, "the erased value is the old mapping");
    expect(r.empty(), "the map is empty after erasing its only entry");
    expect(!r.erase(Suit::CLUBS).has_value(), "erasing an absent key returns empty");

    // Iteration visits present entries in enumerator declaration order.
    EnumMap<Suit, i32> ordered;
    ordered.insert_or_assign(Suit::SPADES, 4);
    ordered.insert_or_assign(Suit::CLUBS, 1);
    ordered.insert_or_assign(Suit::HEARTS, 3);
    Vector<Suit> keys;
    Vector<i32> vals;
    for (auto [key, value]: ordered) {
        keys.push_back(key);
        vals.push_back(value);
    }
    expect_eq(keys.size(), 3uz, "iteration visits every present entry");
    expect(
        keys[0] == Suit::CLUBS && keys[1] == Suit::HEARTS && keys[2] == Suit::SPADES,
        "iteration is in enumerator declaration order"
    );
    expect(vals[0] == 1 && vals[1] == 3 && vals[2] == 4, "values follow their keys in order");

    // key_set yields an EnumSet of the present keys; values yields them in order.
    EnumSet<Suit> present = ordered.key_set();
    expect_eq(present.size(), 3uz, "key_set has one entry per mapping");
    expect(present.contains(Suit::CLUBS) && !present.contains(Suit::DIAMONDS), "key_set reflects the mappings");
    Vector<i32> only_values = ordered.values();
    expect(only_values.size() == 3 && only_values[0] == 1 && only_values[2] == 4, "values are in declaration order");

    // put_all merges, overwriting on conflict.
    EnumMap<Suit, i32> a;
    a.insert_or_assign(Suit::CLUBS, 1);
    EnumMap<Suit, i32> b;
    b.insert_or_assign(Suit::HEARTS, 3);
    b.insert_or_assign(Suit::CLUBS, 99);
    a.put_all(b);
    expect_eq(a.size(), 2uz, "put_all unions the key sets");
    expect_eq(*a.find(Suit::CLUBS), 99, "put_all overwrites on conflict");

    // Equality
    EnumMap<Suit, i32> x = {{Suit::CLUBS, 1}, {Suit::SPADES, 4}};
    expect_eq(x.size(), 2uz, "the initializer-list constructor inserts each entry");
    EnumMap<Suit, i32> y = x;
    expect(x == y, "copy constructor yields an equal map");
    y.insert_or_assign(Suit::HEARTS, 3);
    expect(x != y, "maps compare unequal after a divergent insert");

    // clear.
    x.clear();
    expect(x.empty(), "clear empties the map");

    // Formatter: {KEY=value, ...} in declaration order.
    EnumMap<Suit, i32> f;
    f.insert_or_assign(Suit::CLUBS, 1);
    f.insert_or_assign(Suit::HEARTS, 3);
    expect_eq(Ops::format("{}", f), "{CLUBS=1, HEARTS=3}", "formatter lists entries in declaration order");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    return run(argc, argv, {
        {"meta.reflection_classes", test_reflection_classes},
        {"meta.argument_parser", test_argument_parser},
        {"meta.throws_annotation", test_throws_annotation},
        {"collections.enum_map", test_enum_map},
    });
    #else
    System::out.println("[test] Test disabled (compiler does not support reflection).");
    return System::EXIT_SUCCESS;
    #endif
}
