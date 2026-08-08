#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::collections::EnumMap;
using stdx::collections::EnumSet;
using stdx::collections::Vector;
using stdx::linq::Query;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Annotation;
using stdx::meta::reflect::Base;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Concept;
using stdx::meta::reflect::Constructor;
using stdx::meta::reflect::CvQualifier;
using stdx::meta::reflect::Destructor;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Enumerator;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::FunctionSpecifier;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::Method;
using stdx::meta::reflect::Mirror;
using stdx::meta::reflect::Namespace;
using stdx::meta::reflect::NamespaceAlias;
using stdx::meta::reflect::Parameter;
using stdx::meta::reflect::ReflectableAsClass;
using stdx::meta::reflect::Template;
using stdx::meta::reflect::ThrownExceptions;
using stdx::meta::reflect::Type;
using stdx::meta::reflect::TypeAlias;
using stdx::meta::reflect::Union;
using stdx::meta::reflect::Variable;
using stdx::util::ArgumentParser;
using stdx::util::CommandLineParserException;
using stdx::util::Description;
using stdx::util::Env;
using stdx::util::ShortName;

namespace reflect = stdx::meta::reflect;

using namespace stdx::test;

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

struct Shape {
    i32 id;
};

struct Named {
    StringView label;
};

struct Tagged {
    u8 tag;
};

/**
 * @struct Circle
 * @brief Hierarchy fixture for the Base -> Class round trip.
 * @extends Shape
 * @extends Named
 * @extends Tagged
 *
 * One base of each interesting kind - plain public, virtual public, private -
 * so bases() has something to report that a Class<Base> alone could not carry.
 */
struct Circle: public Shape, virtual Named, private Tagged {
    f64 radius;
};

/**
 * @struct Widget
 * @brief Fixture for Constructor/Destructor/Callback coverage: a user-declared
 * constructor with a default argument, an explicit converting constructor, and
 * a user-declared (non-deleted, non-defaulted) destructor.
 */
struct Widget {
    i32 value;

    explicit Widget(i32 v = 0) noexcept:
        value{v} {}

    explicit Widget(StringView _) noexcept:
        value{0} {}

    ~Widget() noexcept {}
};

/**
 * @struct Locked
 * @brief Fixture whose destructor is explicitly deleted, so Destructor::is_deleted()
 * has a true case to observe.
 */
struct Locked {
    ~Locked() = delete;
};

/**
 * @union Num32
 * @brief Fixture for Union coverage: two same-size alternatives.
 */
union Num32 {
    i32 as_int;
    f32 as_float;
};

namespace reflection_fixtures {
    struct Nested {
        i32 value;
    };

    using NestedAlias = Nested;

    inline constexpr i32 ANSWER = 42;
}

namespace aliased_fixtures = reflection_fixtures;

/**
 * @struct WithStatic
 * @brief Fixture for Variable coverage: one static data member alongside one
 * instance field, so Class::static_fields and Class::fields can be told apart.
 */
struct WithStatic {
    static constexpr i32 LIMIT = 100;
    const i32 id = 0;
};

/**
 * @concept SmallInt
 * @brief Fixture concept for Concept/Template coverage: satisfied only by
 * integral types no larger than 4 bytes.
 */
template <typename T>
concept SmallInt = requires { requires stdx::meta::reflect::is_integral_type(^^T); } && sizeof(T) <= 4;

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

[[=Throws<ArithmeticException, InvalidOperationException>]]
i32 annotated_thrower(i32 x) {
    return x;
}

i32 unannotated_function(i32 x) {
    return x;
}

struct AnnotatedCallable {
    [[=Throws<ArithmeticException>]]
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

template <ReflectableAsClass T>
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

    {
        const BasicOptions o = parse_tokens<BasicOptions>({"reftest", "-i", "f.txt", "-n", "7"});
        expect(o.input == "f.txt", "short flag: string value");
        expect(o.count == 7, "short flag: integer value");
        expect(!o.verbose, "bool absent is false");
        expect(!o.limit.has_value(), "optional absent is empty");
        expect(o.log_level == "info", "defaulted field keeps default");
        expect(o.maxRetries == 3, "defaulted camelCase keeps default");
    }

    expect_throws<CommandLineParserException>(
        [] -> void { static_cast<void>(parse_tokens<BasicOptions>({"reftest"})); },
        "missing required argument throws"
    );
    expect_throws<CommandLineParserException>(
        [] -> void { static_cast<void>(parse_tokens<BasicOptions>({"reftest", "-i", "a", "--input", "b"})); },
        "one field via both spellings throws"
    );
    expect_throws<CommandLineParserException>(
        [] -> void { static_cast<void>(parse_tokens<BasicOptions>({"reftest", "-i", "x", "--count"})); },
        "value flag missing its value throws"
    );

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

    {
        Environment::unset("REFTEST_TOKEN");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest"});
        expect(o.token == "default-token", "env unset keeps default");
    }

    {
        Environment::set("REFTEST_TOKEN", "from-env");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest"});
        expect(o.token == "from-env", "environment supplies value");
        Environment::unset("REFTEST_TOKEN");
    }

    {
        Environment::set("REFTEST_TOKEN", "from-env");
        const EnvOptions o = parse_tokens<EnvOptions>({"reftest", "--token", "from-cli"});
        expect(o.token == "from-cli", "command line overrides environment");
        Environment::unset("REFTEST_TOKEN");
    }

    {
        Environment::unset("REFTEST_MISSING");
        expect_throws<CommandLineParserException>(
            [] -> void { static_cast<void>(parse_tokens<EnvRequired>({"reftest"})); },
            "required env-backed field unset throws"
        );
    }
}

static_assert(Mirror::NONE.value() == Info{}, "Mirror::NONE is the null reflection");

/**
 * @brief Asserts on the reflection class wrappers (Class, Field, Method, Enum,
 * Enumerator, EnumSet, Type). Each reflected property is evaluated into a
 * constexpr value first, then checked at runtime - the accessors are
 * compile-time only.
 */
void test_reflection_classes() {
    constexpr AccessContext ctx = AccessContext::unchecked();

    constexpr StringView s = "Hello, world!";
    constexpr Info m1 = ^^s;
    constexpr usize spliced_length = length([:m1:]);
    expect(spliced_length == s.length(), "splicing a reflected StringView recovers its length");

    static constexpr Class<Vec2> VEC2_CLASS = Class<Vec2>();
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

    static constexpr Span<const Field> VEC2_FIELDS = Ops::static_array(VEC2_CLASS.fields(ctx));
    constexpr usize field_count = VEC2_FIELDS.size();
    constexpr StringView field0_name = VEC2_FIELDS[0].name().value();
    constexpr StringView field1_name = VEC2_FIELDS[1].name().value();
    constexpr usize field0_offset = VEC2_FIELDS[0].offset().bytes;
    constexpr usize field1_offset = VEC2_FIELDS[1].offset().bytes;
    expect_eq(field_count, 2uz, "Vec2 has two fields");
    expect_eq(field0_name, "x", "first field is x");
    expect_eq(field1_name, "y", "second field is y");
    expect(field0_offset == 0, "field x is at offset 0");
    expect(field1_offset == sizeof(f64), "field y is at offset sizeof(f64)");

    static constexpr Span<const Method> VEC2_METHODS = Query(Ops::static_array(VEC2_CLASS.methods(ctx)))
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
        } else if constexpr (name.empty()) {
            continue;
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

    static constexpr Enum<Suit> SUIT_ENUM = Enum<Suit>();
    constexpr StringView suit_name = SUIT_ENUM.name().value();
    constexpr bool suit_scoped = SUIT_ENUM.is_scoped();
    expect_eq(suit_name, "Suit", "Enum<Suit> reports its name");
    expect(suit_scoped, "Suit is a scoped enum");

    static constexpr Span<const Enumerator> SUITS = Ops::static_array(SUIT_ENUM.enumerators());
    constexpr usize suit_count = SUITS.size();
    constexpr StringView first_suit = SUITS[0].name().value();
    constexpr u8 first_suit_value = Ops::to_underlying(SUITS[0].as<Suit>());
    constexpr StringView last_suit = SUITS[3].name().value();
    constexpr u8 last_suit_value = Ops::to_underlying(SUITS[3].as<Suit>());
    expect_eq(suit_count, 4uz, "Suit has four enumerators");
    expect_eq(first_suit, "CLUBS", "first enumerator is CLUBS");
    expect(first_suit_value == 0, "CLUBS == 0");
    expect_eq(last_suit, "SPADES", "last enumerator is SPADES");
    expect(last_suit_value == 3, "SPADES == 3");

    static constexpr EnumSet<Suit> REDS = EnumSet<Suit>::of(Suit::DIAMONDS, Suit::HEARTS);
    static constexpr EnumSet<Suit> BLACKS = ~REDS;
    expect_eq(REDS.size(), 2uz, "two red suits");
    expect(REDS.contains(Suit::HEARTS), "REDS contains HEARTS");
    expect(!REDS.contains(Suit::CLUBS), "REDS does not contain CLUBS");
    expect_eq(BLACKS.size(), 2uz, "two black suits");
    expect(BLACKS.contains(Suit::CLUBS) && BLACKS.contains(Suit::SPADES), "BLACKS contains clubs and spades");
    expect((REDS | BLACKS).is_full(), "REDS | BLACKS covers every suit");

    static constexpr Type INT_TYPE = Type::of<i32>();
    constexpr bool int_is_integral = INT_TYPE.is_integral();
    constexpr bool int_is_signed = INT_TYPE.is_signed();
    constexpr usize int_size = INT_TYPE.size();
    expect(int_is_integral, "i32 is integral");
    expect(int_is_signed, "i32 is signed");
    expect(int_size == sizeof(i32), "i32 size matches sizeof(i32)");
}

/**
 * @brief Round-trips an erased Base back into a statically-typed Class through
 * the Info-keyed constructor, and confirms the base-specifier data a Class<Base>
 * cannot carry is still reachable from the Base itself.
 */
void test_base_to_class() {
    constexpr AccessContext ctx = AccessContext::unchecked();

    static constexpr Span<const Base> CIRCLE_BASES = Ops::static_array(Class<Circle>().bases(ctx));
    constexpr usize base_count = CIRCLE_BASES.size();
    expect_eq(base_count, 3uz, "Circle declares three direct bases");

    constexpr Class<Shape> clazz(CIRCLE_BASES[0].type());
    constexpr bool shape_identity = Class<Shape>::VALUE == ^^Shape;
    constexpr StringView shape_name = clazz.name().value();
    constexpr usize shape_fields = clazz.fields(ctx).size();
    expect(shape_identity, "the first base round-trips to Class<Shape>");
    expect_eq(shape_name, "Shape", "the round-tripped wrapper reports Shape's name");
    expect_eq(shape_fields, 1uz, "Class<Shape> reaches Shape's own fields");

    constexpr bool named_virtual = CIRCLE_BASES[1].is_virtual();
    constexpr bool tagged_private = CIRCLE_BASES[2].is_private();
    constexpr bool shape_plain = !CIRCLE_BASES[0].is_virtual() && CIRCLE_BASES[0].is_public();
    expect(shape_plain, "Shape is a plain public base");
    expect(named_virtual, "Named is a virtual base");
    expect(tagged_private, "Tagged is a private base");

    Vector<StringView> base_names;
    template for (constexpr Base b: CIRCLE_BASES) {
        constexpr StringView name = b.type()
            .name()
            .value();
        base_names.push_back(name);
    }
    expect_eq(base_names.size(), 3uz, "the expansion visits every base");
    expect_eq(base_names[0], "Shape", "bases are reported in declaration order");
    expect_eq(base_names[1], "Named", "the virtual base round-trips too");
    expect_eq(base_names[2], "Tagged", "the private base round-trips under an unchecked context");
}

/**
 * @brief Exercises Constructor and Destructor, including the Callback members they
 * now inherit (parameters(), is_deleted(), is_explicit(), is_noexcept(), return_type()).
 */
void test_constructor_and_destructor() {
    constexpr AccessContext ctx = AccessContext::unchecked();

    static constexpr Span<const Constructor> WIDGET_CTORS = Ops::static_array(Class<Widget>().constructors(ctx));
    constexpr usize ctor_count = WIDGET_CTORS.size();
    expect_eq(ctor_count, 3uz, "Widget has two user-declared constructors plus one implicit copy constructor");

    bool found_int_ctor = false;
    bool found_string_ctor = false;
    bool found_copy_ctor = false;
    template for (constexpr Constructor c: WIDGET_CTORS) {
        constexpr bool is_copy_ctor = c.is_copy();
        if constexpr (is_copy_ctor) {
            found_copy_ctor = true;
            constexpr bool is_defaulted = c.is_defaulted();
            constexpr bool is_explicit = c.is_explicit();
            expect(is_defaulted, "the implicit copy constructor is defaulted");
            expect(!is_explicit, "the implicit copy constructor is not explicit");
        } else {
            constexpr bool is_explicit = c.is_explicit();
            constexpr usize param_count = c.parameters().size();
            expect(is_explicit, "each user-declared Widget constructor is declared explicit");
            if constexpr (param_count == 1uz) {
                constexpr Type param_type = Type(c.parameters()[0].type());
                if constexpr (param_type.is_integral()) {
                    found_int_ctor = true;
                    constexpr bool has_default = c.parameters()[0].has_default();
                    constexpr bool is_default_ctor = c.is_default();
                    constexpr bool is_move_ctor = c.is_move();
                    expect(has_default, "the i32 constructor's parameter has a default argument");
                    expect(is_default_ctor, "a single defaulted parameter makes it usable as a default constructor");
                    expect(!is_move_ctor, "the i32 constructor is not a move constructor");
                } else {
                    found_string_ctor = true;
                }
            }
        }
    }
    expect(found_int_ctor, "Widget's i32 constructor was found");
    expect(found_string_ctor, "Widget's StringView constructor was found");
    expect(found_copy_ctor, "Widget's implicit copy constructor was found");

    static constexpr Optional<Destructor> WIDGET_DTOR = Class<Widget>().destructor(ctx);
    constexpr bool widget_dtor_present = WIDGET_DTOR.has_value();
    expect(widget_dtor_present, "Widget's destructor is visible under an unchecked context");
    constexpr bool widget_dtor_deleted = WIDGET_DTOR->is_deleted();
    constexpr bool widget_dtor_noexcept = WIDGET_DTOR->is_noexcept();
    expect(!widget_dtor_deleted, "Widget's user-declared destructor is not deleted");
    expect(widget_dtor_noexcept, "Widget's destructor is noexcept");

    static constexpr Optional<Destructor> LOCKED_DTOR = Class<Locked>().destructor(ctx);
    constexpr bool locked_dtor_present = LOCKED_DTOR.has_value();
    expect(locked_dtor_present, "Locked's destructor is visible under an unchecked context");
    constexpr bool locked_dtor_deleted = LOCKED_DTOR->is_deleted();
    expect(locked_dtor_deleted, "Locked's explicitly-deleted destructor reports is_deleted()");

    constexpr bool ctor_declaring_class_ok = WIDGET_CTORS[0].declaring_class().value() == ^^Widget;
    constexpr bool dtor_declaring_class_ok = WIDGET_DTOR->declaring_class().value() == ^^Widget;
    expect(ctor_declaring_class_ok, "Constructor::declaring_class reports Widget");
    expect(dtor_declaring_class_ok, "Destructor::declaring_class reports Widget");
}

/**
 * @brief Exercises Variable (a static data member) and confirms Class::static_fields
 * finds it, alongside Field's own cv_qualifiers().
 */
void test_variable_and_static_fields() {
    constexpr AccessContext ctx = AccessContext::unchecked();
    static constexpr Span<const Variable> STATICS = Ops::static_array(Class<WithStatic>().static_fields(ctx));
    constexpr usize static_count = STATICS.size();
    expect_eq(static_count, 1uz, "WithStatic has one static data member");

    constexpr StringView static_name = STATICS[0].name().value();
    constexpr bool static_has_static_storage = STATICS[0].has_static_storage();
    constexpr bool static_const = STATICS[0].is_const();
    expect_eq(static_name, "LIMIT", "the static field is named LIMIT");
    expect(static_has_static_storage, "a static data member has static storage duration");
    expect(static_const, "LIMIT is const");

    static constexpr Span<const Field> WITH_STATIC_FIELDS = Ops::static_array(Class<WithStatic>().fields(ctx));
    constexpr usize field_count = WITH_STATIC_FIELDS.size();
    expect_eq(field_count, 1uz, "static_fields and fields do not overlap: one instance field");
    constexpr bool id_const = WITH_STATIC_FIELDS[0]
        .cv_qualifiers()
        .contains(CvQualifier::CONST);
    expect(id_const, "Field::cv_qualifiers reports id's const qualifier");
}

static_assert(Namespace::GLOBAL.is_global_namespace(), "Namespace::GLOBAL reports is_global_namespace()");
static_assert(Namespace::GLOBAL.depth() == 0uz, "Namespace::GLOBAL has depth 0");
static_assert(!Namespace::GLOBAL.parent().has_value(), "Namespace::GLOBAL has no parent");

/**
 * @brief Exercises Namespace, NamespaceAlias, TypeAlias, and the Mirror scope
 * helpers (scope_chain, namespace_depth, enclosing_namespace, enclosing_class).
 */
void test_namespace_and_aliases() {
    constexpr Type nested_type(^^reflection_fixtures::Nested);
    constexpr Namespace enclosing_ns = nested_type.enclosing_namespace();
    constexpr StringView ns_name = enclosing_ns.name().value();
    expect_eq(ns_name, "reflection_fixtures", "Nested's enclosing namespace is reflection_fixtures");

    constexpr TypeAlias nested_alias(^^reflection_fixtures::NestedAlias);
    constexpr Type alias_target = nested_alias.target();
    constexpr bool alias_resolves = alias_target.value() == ^^reflection_fixtures::Nested;
    expect(alias_resolves, "TypeAlias::target dealiases NestedAlias back to Nested");

    constexpr NamespaceAlias ns_alias(^^aliased_fixtures);
    constexpr Namespace alias_ns_target = ns_alias.target();
    constexpr bool ns_alias_resolves = alias_ns_target.value() == ^^reflection_fixtures;
    expect(ns_alias_resolves, "NamespaceAlias::target dealiases aliased_fixtures back to reflection_fixtures");

    constexpr Variable answer(^^reflection_fixtures::ANSWER);
    constexpr StringView answer_name = answer.name().value();
    constexpr bool answer_const = answer.is_const();
    constexpr bool answer_static_storage = answer.has_static_storage();
    expect_eq(answer_name, "ANSWER", "Variable::name reports ANSWER");
    expect(answer_const, "ANSWER is declared const");
    expect(answer_static_storage, "a namespace-scope variable has static storage duration");

    constexpr Optional<Type> field_enclosing_class = Class<Vec2>()
        .fields()[0]
        .enclosing_class();
    constexpr bool field_enclosing_class_present = field_enclosing_class.has_value();
    expect(field_enclosing_class_present, "a field's enclosing_class is present");
    constexpr bool field_enclosing_class_ok = field_enclosing_class->value() == ^^Vec2;
    expect(field_enclosing_class_ok, "Vec2::x's enclosing_class is Vec2");

    constexpr usize nested_depth = nested_type.namespace_depth();
    expect(nested_depth >= 1uz, "Nested is at least one namespace deep");

    constexpr Namespace reflection_fixtures_parent(enclosing_ns.parent()->value());
    constexpr bool reflection_fixtures_parent_is_global = reflection_fixtures_parent.value() == Namespace::GLOBAL.value();
    expect(reflection_fixtures_parent_is_global, "reflection_fixtures's parent is Namespace::GLOBAL");
}

/**
 * @brief Exercises Union: fields() and members() on a reflected union type.
 */
void test_union() {
    constexpr AccessContext ctx = AccessContext::unchecked();
    static constexpr Union<Num32> VARIANT = Union<Num32>();
    constexpr StringView variant_name = VARIANT.name().value();
    expect_eq(variant_name, "Num32", "Union<Num32> reports its name");

    static constexpr Span<const Field> VARIANT_FIELDS = Ops::static_array(VARIANT.fields(ctx));
    constexpr usize variant_field_count = VARIANT_FIELDS.size();
    expect_eq(variant_field_count, 2uz, "Num32 has two alternatives");
    expect_eq(VARIANT_FIELDS[0].name().value(), "as_int", "the first alternative is as_int");
    expect_eq(VARIANT_FIELDS[1].name().value(), "as_float", "the second alternative is as_float");
}

/**
 * @brief Exercises Concept (satisfaction checks) and Template/Type::template_of
 * plus template_arguments on a genuine class template specialization.
 */
void test_template_and_concept() {
    constexpr bool i32_satisfies = SmallInt<i32>;
    constexpr bool i64_rejects = !SmallInt<i64>;
    expect(i32_satisfies, "i32 satisfies SmallInt");
    expect(i64_rejects, "i64 does not satisfy SmallInt");

    constexpr Concept small_int(^^SmallInt);
    constexpr bool i32_substitutes = small_int.can_substitute<i32>();
    constexpr bool i64_substitutes = small_int.can_substitute<i64>();
    expect(i32_substitutes, "i32 substitutes into SmallInt<T> without a hard error");
    expect(i64_substitutes, "i64 also substitutes into SmallInt<T> without a hard error (it just yields false)");

    constexpr Type vector_i32_type = Type::of<Vector<i32>>();
    constexpr bool vector_has_template_args = vector_i32_type.has_template_arguments();
    expect(vector_has_template_args, "Vector<i32> reports having template arguments");

    constexpr Template vector_template = vector_i32_type.template_of();
    constexpr bool vector_is_class_template = vector_template.is_class_template();
    expect(vector_is_class_template, "the template underlying Vector<i32> is a class template");

    constexpr Span<const Mirror> vector_args = Ops::static_array(vector_i32_type.template_arguments());
    constexpr usize vector_arg_count = vector_args.size();
    expect(vector_arg_count >= 1uz, "Vector<i32> has at least one template argument");
    constexpr bool first_vector_arg_is_type = vector_args[0].is_type();
    constexpr Type first_vector_arg = Type(vector_args[0].value());
    constexpr bool first_vector_arg_is_i32 = first_vector_arg.dealias().remove_cvref().value() == reflect::dealias(^^i32);
    expect(first_vector_arg_is_i32, "Vector<i32>'s first template argument is i32");
}

/**
 * @brief Exercises Annotation directly: type() and as<T>() on the annotations
 * carried by a reflected field.
 */
void test_annotation() {
    static constexpr Span<const Annotation> INPUT_ANNOTATIONS = Ops::static_array(
        Class<BasicOptions>()
            .fields()[0]
            .annotations()
    );
    constexpr usize annotation_count = INPUT_ANNOTATIONS.size();
    expect(annotation_count >= 2uz, "BasicOptions::input carries at least ShortName and Description annotations");

    bool found_description = false;
    template for (constexpr Annotation a: INPUT_ANNOTATIONS) {
        constexpr Type annotation_type = a.type();
        constexpr StringView type_display_name = annotation_type.display_name();
        if constexpr (type_display_name.find("Description") != StringView::npos) {
            found_description = true;
            constexpr Description<"input file"> described = a.as<Description<"input file">>();
            constexpr StringView described_text = described.str();
            expect_eq(described_text, "input file", "Annotation::as<Description<...>> extracts the annotation's text");
        }
    }
    expect(found_description, "one of input's annotations is a Description");
}

/**
 * @brief Exercises the Throws annotation: gathering declared exception types from a
 * function and from a callable type, and the empty case.
 */
void test_throws_annotation() {
    constexpr ThrownExceptions<^^annotated_thrower> declared = Ops::thrown_exceptions<^^annotated_thrower>();
    using Declared = decltype(declared);
    constexpr bool declared_order = (TupleElementType<0, Declared>::VALUE == ^^ArithmeticException)
        && (TupleElementType<1, Declared>::VALUE == ^^InvalidOperationException);
    expect_eq(TupleSize<Declared>::value, 2uz, "annotated_thrower declares two exceptions");
    expect(declared_order, "annotated_thrower declares Class<ArithmeticException> then Class<InvalidOperationException>");

    using None = decltype(Ops::thrown_exceptions<^^unannotated_function>());
    expect_eq(TupleSize<None>::value, 0uz, "unannotated_function declares no exceptions");

    constexpr ThrownExceptions<^^AnnotatedCallable> from_callable = Ops::thrown_exceptions<^^AnnotatedCallable>();
    using FromCallable = decltype(from_callable);
    constexpr bool callable_ok = (TupleElementType<0, FromCallable>::VALUE == ^^ArithmeticException);
    expect_eq(TupleSize<FromCallable>::value, 1uz, "callable type declares one exception");
    expect(callable_ok, "callable type declares Class<ArithmeticException> via operator()");

    constexpr bool function_ok = FunctionOrCallable<^^annotated_thrower>;
    constexpr bool callable_type_ok = FunctionOrCallable<^^AnnotatedCallable>;
    constexpr bool non_callable_rejected = !FunctionOrCallable<^^i32>;
    expect(function_ok, "a function is function-or-callable");
    expect(callable_type_ok, "a callable type is function-or-callable");
    expect(non_callable_rejected, "a non-callable type is not function-or-callable");
}

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

    EnumMap<Suit, i32> empty;
    expect(empty.empty(), "a fresh map is empty");
    expect_eq(empty.size(), 0uz, "a fresh map has size zero");
    expect(!empty.contains(Suit::CLUBS), "a fresh map contains no keys");
    expect(empty.find(Suit::CLUBS) == nullptr, "find on an absent key is nullptr");
    expect_throws<OutOfRangeException>(
        [&] -> void { static_cast<void>(empty.at(Suit::CLUBS)); },
        "at on an absent key throws OutOfRangeException"
    );

    EnumMap<Suit, i32> m;
    expect(!m.insert_or_assign(Suit::CLUBS, 1).has_value(), "inserting a new key has no previous value");
    Optional<i32> previous = m.insert_or_assign(Suit::CLUBS, 2);
    require(previous.has_value(), "overwriting returns the previous value");
    expect_eq(*previous, 1, "the previous value is the old mapping");
    expect_eq(m.size(), 1uz, "overwriting does not grow the map");

    m.insert_or_assign(Suit::HEARTS, 7);
    expect(m.contains(Suit::HEARTS), "contains finds a present key");
    expect_eq(m.at(Suit::HEARTS), 7, "at returns the mapped value");
    expect_throws<OutOfRangeException>(
        [&] -> void { static_cast<void>(m.at(Suit::SPADES)); },
        "at on an absent key throws OutOfRangeException"
    );
    require(m.find(Suit::HEARTS) != nullptr, "find returns a pointer to a present value");
    expect_eq(*m.find(Suit::HEARTS), 7, "the found pointer dereferences to the value");
    expect(m.contains_value(7), "contains_value finds a stored value");
    expect(!m.contains_value(8), "contains_value rejects an absent value");

    EnumMap<Suit, i32> idx;
    idx[Suit::CLUBS] = 5;
    expect_eq(idx[Suit::CLUBS], 5, "operator[] reads back what was assigned");
    i32& slot = idx[Suit::DIAMONDS];
    expect_eq(slot, 0, "operator[] default-inserts for an absent key");
    expect_eq(idx.size(), 2uz, "operator[] grows the map on insertion");

    EnumMap<Suit, i32> r;
    r.insert_or_assign(Suit::CLUBS, 1);
    Optional<i32> removed = r.erase(Suit::CLUBS);
    require(removed.has_value(), "erase returns the previous value");
    expect_eq(*removed, 1, "the erased value is the old mapping");
    expect(r.empty(), "the map is empty after erasing its only entry");
    expect(!r.erase(Suit::CLUBS).has_value(), "erasing an absent key returns empty");

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

    EnumSet<Suit> present = ordered.key_set();
    expect_eq(present.size(), 3uz, "key_set has one entry per mapping");
    expect(present.contains(Suit::CLUBS) && !present.contains(Suit::DIAMONDS), "key_set reflects the mappings");
    Vector<i32> only_values = ordered.values();
    expect(only_values.size() == 3 && only_values[0] == 1 && only_values[2] == 4, "values are in declaration order");

    EnumMap<Suit, i32> a;
    a.insert_or_assign(Suit::CLUBS, 1);
    EnumMap<Suit, i32> b;
    b.insert_or_assign(Suit::HEARTS, 3);
    b.insert_or_assign(Suit::CLUBS, 99);
    a.put_all(b);
    expect_eq(a.size(), 2uz, "put_all unions the key sets");
    expect_eq(*a.find(Suit::CLUBS), 99, "put_all overwrites on conflict");

    EnumMap<Suit, i32> x = {{Suit::CLUBS, 1}, {Suit::SPADES, 4}};
    expect_eq(x.size(), 2uz, "the initializer-list constructor inserts each entry");
    EnumMap<Suit, i32> y = x;
    expect(x == y, "copy constructor yields an equal map");
    y.insert_or_assign(Suit::HEARTS, 3);
    expect(x != y, "maps compare unequal after a divergent insert");

    x.clear();
    expect(x.empty(), "clear empties the map");

    EnumMap<Suit, i32> f;
    f.insert_or_assign(Suit::CLUBS, 1);
    f.insert_or_assign(Suit::HEARTS, 3);
    expect_eq(Ops::fmt("{}", f), "{CLUBS=1, HEARTS=3}", "formatter lists entries in declaration order");

    const EnumMap<Suit, i32> made = EnumMap<Suit, i32>::of(
        Ops::pair(Suit::CLUBS, 1),
        Ops::pair(Suit::SPADES, 4)
    );
    expect_eq(made.size(), 2uz, "of() inserts every entry");
    expect_eq(*made.find(Suit::CLUBS), 1, "of() keeps the first entry's value");
    expect_eq(*made.find(Suit::SPADES), 4, "of() keeps the last entry's value");

    const EnumMap<Suit, i32> repeated = EnumMap<Suit, i32>::of(
        Ops::pair(Suit::HEARTS, 1),
        Ops::pair(Suit::HEARTS, 2)
    );
    expect_eq(repeated.size(), 1uz, "a repeated key does not add a second entry");
    expect_eq(*repeated.find(Suit::HEARTS), 2, "the later entry overwrites the earlier one");

    EnumMap<Suit, UniquePointer<i32>> owned = EnumMap<Suit, UniquePointer<i32>>::of(
        Ops::pair(Suit::CLUBS, Pointers::unique<i32>(7))
    );
    expect_eq(owned.size(), 1uz, "of() moves a move-only value into the map");
    expect_eq(**owned.find(Suit::CLUBS), 7, "and the moved-from pointer's value survives");

    const Pair<Suit, i32> entry = Ops::pair(Suit::DIAMONDS, 9);
    const EnumMap<Suit, i32> copied = EnumMap<Suit, i32>::of(entry);
    expect_eq(*copied.find(Suit::DIAMONDS), 9, "of() accepts a const lvalue entry");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    return run(argc, argv, {
        {"Meta.reflection_classes", test_reflection_classes},
        {"Meta.base_to_class", test_base_to_class},
        {"Meta.constructor_and_destructor", test_constructor_and_destructor},
        {"Meta.variable_and_static_fields", test_variable_and_static_fields},
        {"Meta.namespace_and_aliases", test_namespace_and_aliases},
        {"Meta.union", test_union},
        {"Meta.template_and_concept", test_template_and_concept},
        {"Meta.annotation", test_annotation},
        {"Meta.argument_parser", test_argument_parser},
        {"Meta.throws_annotation", test_throws_annotation},
        {"Collections.enum_map", test_enum_map},
    });
    #else
    System::out.println("[test] Test disabled (compiler does not support reflection).");
    return System::EXIT_SUCCESS;
    #endif
}
