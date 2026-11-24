#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::data::toml::Toml;
using stdx::data::toml::TomlParseException;
using stdx::data::toml::TomlTypeException;

#ifdef __cpp_lib_reflection
using stdx::data::AllowPrivate;
using stdx::data::Flatten;
using stdx::data::Reflectable;
using stdx::data::Rename;
using stdx::data::Transient;
using stdx::data::toml::TomlFlatten;
using stdx::data::toml::TomlIgnore;
using stdx::data::toml::TomlPropertyName;
#endif

using namespace stdx::test;

/**
 * @brief Scalars: booleans, integers (incl. bases), floats, strings.
 */
void test_toml_scalars() {
    Toml t = Toml::parse(
        "b = true\n"
        "i = 42\n"
        "neg = -17\n"
        "hex = 0xff\n"
        "oct = 0o17\n"
        "bin = 0b1010\n"
        "sep = 1_000_000\n"
        "f = 3.5\n"
        "e = 1e3\n"
        "s = \"hello\"\n"
    );
    expect(t.is_table(), "a document is a table");
    expect(t.at("b").get<bool>(), "boolean");
    expect_eq(t.at("i").get<i64>(), 42ll, "integer");
    expect_eq(t.at("neg").get<i64>(), -17ll, "negative integer");
    expect_eq(t.at("hex").get<i64>(), 255ll, "hex integer");
    expect_eq(t.at("oct").get<i64>(), 15ll, "octal integer");
    expect_eq(t.at("bin").get<i64>(), 10ll, "binary integer");
    expect_eq(t.at("sep").get<i64>(), 1000000ll, "underscore digit separators");
    expect(t.at("f").is_float(), "fractional number is a float");
    expect_near(t.at("f").get<f64>(), 3.5, 1e-12, "float value");
    expect_near(t.at("e").get<f64>(), 1000.0, 1e-9, "exponent float");
    expect_eq(t.at("s").get<String>(), "hello"s, "basic string");
}

/**
 * @brief String flavors: basic escapes, literal, multiline.
 */
void test_toml_strings() {
    Toml t = Toml::parse(
        "basic = \"a\\nb\\t\\\"c\\\"\"\n"
        "literal = 'C:\\path\\no\\escape'\n"
        "multi = \"\"\"\nline1\nline2\"\"\"\n"
        "unicode = \"\\u00e9\"\n"
    );
    expect_eq(t.at("basic").get<String>(), "a\nb\t\"c\""s, "basic string escapes");
    expect_eq(t.at("literal").get<String>(), "C:\\path\\no\\escape"s, "literal string has no escapes");
    expect_eq(t.at("multi").get<String>(), "line1\nline2"s, "multiline trims the leading newline");
    expect_eq(t.at("unicode").get<String>(), "\xc3\xa9"s, "unicode escape to UTF-8");
}

/**
 * @brief Tables: headers, dotted keys, nesting.
 */
void test_toml_tables() {
    Toml t = Toml::parse(
        "title = \"root\"\n"
        "a.b.c = 1\n"
        "\n"
        "[server]\n"
        "host = \"localhost\"\n"
        "port = 8080\n"
        "\n"
        "[server.auth]\n"
        "enabled = true\n"
    );
    expect_eq(t.at("title").get<String>(), "root"s, "root-level key");
    expect(t.at("server").is_table(), "a header creates a table");
    expect_eq(t.at("server").at("host").get<String>(), "localhost"s, "nested key");
    expect_eq(t.at("server").at("port").get<i64>(), 8080ll, "nested integer");
    expect(t.at("server").at("auth").at("enabled").get<bool>(), "dotted header nests");
    expect_eq(t.at("a").at("b").at("c").get<i64>(), 1ll, "dotted key builds nested tables");
}

/**
 * @brief Arrays, inline tables, and arrays of tables.
 */
void test_toml_arrays() {
    Toml t = Toml::parse(
        "nums = [1, 2, 3]\n"
        "nested = [[1, 2], [3]]\n"
        "point = { x = 1, y = 2 }\n"
        "\n"
        "[[products]]\n"
        "name = \"hammer\"\n"
        "\n"
        "[[products]]\n"
        "name = \"nail\"\n"
    );
    expect(t.at("nums").is_array(), "array value");
    expect_eq(t.at("nums").size(), 3uz, "array length");
    expect_eq(t.at("nums")[2uz].get<i64>(), 3ll, "array element");
    expect_eq(t.at("nested")[0uz][1uz].get<i64>(), 2ll, "nested array element");
    expect(t.at("point").is_table(), "inline table is a table");
    expect_eq(t.at("point").at("y").get<i64>(), 2ll, "inline table member");
    expect(t.at("products").is_array(), "array of tables is an array");
    expect_eq(t.at("products").size(), 2uz, "two array-of-tables entries");
    expect_eq(t.at("products")[0uz].at("name").get<String>(), "hammer"s, "first product");
    expect_eq(t.at("products")[1uz].at("name").get<String>(), "nail"s, "second product");
}

/**
 * @brief The four date-time kinds.
 */
void test_toml_datetimes() {
    Toml t = Toml::parse(
        "date = 1979-05-27\n"
        "time = 07:32:00\n"
        "ldt = 1979-05-27T07:32:00\n"
        "odt = 1979-05-27T07:32:00-08:00\n"
        "utc = 1979-05-27T07:32:00Z\n"
    );
    expect(t.at("date").is_local_date(), "local date");
    expect(t.at("time").is_local_time(), "local time");
    expect(t.at("ldt").is_local_date_time(), "local date-time");
    expect(t.at("odt").is_offset_date_time(), "offset date-time");
    expect(t.at("utc").is_offset_date_time(), "Z offset is an offset date-time");

    const Toml::OffsetDateTime odt = t.at("odt").get<Toml::OffsetDateTime>();
    expect_eq(odt.offset.count(), -480ll, "offset of -08:00 is -480 minutes");
    expect(t.at("utc").get<Toml::OffsetDateTime>().offset.count() == 0, "Z offset is zero");
}

/**
 * @brief Serialization and parse/serialize round-trip.
 */
void test_toml_serialize() {
    Toml t = Toml::parse(
        "name = \"app\"\n"
        "port = 8080\n"
        "[db]\n"
        "host = \"localhost\"\n"
    );
    const String text = t.to_string();
    expect(text.find("name = \"app\"") != String::npos, "root key serialized");
    expect(text.find("[db]") != String::npos, "sub-table emitted as a section");

    // Round-trip: re-parsing the serialized form yields an equal value.
    expect_eq(Toml::parse(text), t, "parse/serialize round-trips");

    // A float keeps a decimal point so it reparses as a float.
    Toml f;
    f.set("x", Toml(1.0));
    expect(f.to_string().find("1.0") != String::npos, "a whole float keeps a .0");
}

/**
 * @brief Errors and edge cases.
 */
void test_toml_errors() {
    expect(!Toml::try_parse("x = ").has_value(), "a missing value is rejected");
    expect(!Toml::try_parse("= 1").has_value(), "a missing key is rejected");
    expect(!Toml::try_parse("x = \"unterminated\n").has_value(), "an unterminated string is rejected");
    expect(Toml::try_parse("x = 1\n").has_value(), "well-formed input parses");

    expect_throws<TomlParseException>(
        [] -> void { static_cast<void>(Toml::parse("x = 1\nx = 2\n")); },
        "a duplicate key is rejected"
    );
    expect_throws<TomlParseException>(
        [] -> void { static_cast<void>(Toml::parse("[a]\nx = 1\n[a]\ny = 2\n")); },
        "redefining a table is rejected"
    );

    // Comments and blank lines are ignored.
    Toml t = Toml::parse("# a comment\n\nx = 1 # trailing\n");
    expect_eq(t.at("x").get<i64>(), 1ll, "comments and blank lines are skipped");

    // The _toml literal.
    Toml lit = "k = 42"_toml;
    expect_eq(lit.at("k").get<i64>(), 42ll, "the _toml literal parses");
}

#ifdef __cpp_lib_reflection

enum class Mode: u8 {
    DEV,
    PROD,
};

struct Database {
    String host = "localhost";
    i32 port = 5432;
};

struct Config {
    [[=TomlPropertyName<"app_name">]]
    String name;

    Mode mode = Mode::DEV;
    Optional<i32> workers;
    Vector<String> tags;
    Database db;

    [[=TomlIgnore]]
    String computed;

    [[=Transient]]
    String secret;
};

/**
 * @brief Reflection-driven serialize/deserialize, incl. nested struct and date-less kinds.
 */
void test_toml_serde() {
    Config cfg = {
        .name = "myapp",
        .mode = Mode::PROD,
        .workers = 4,
        .tags = {"web", "api"},
        .db = Database{.host = "db.example", .port = 5433},
        .computed = "x",
        .secret = "y",
    };

    Toml t = Toml::serialize(cfg);
    expect(t.is_table(), "a struct serializes to a table");
    expect(t.contains("app_name"), "TomlPropertyName renames the key");
    expect(!t.contains("name"), "the original field name is not used");
    expect(!t.contains("computed"), "a TomlIgnore field is omitted");
    expect(!t.contains("secret"), "a Transient field is omitted");
    expect_eq(t.at("app_name").get<String>(), "myapp"s, "renamed string member");
    expect_eq(t.at("mode").get<String>(), "PROD"s, "enum as its name");
    expect_eq(t.at("workers").get<i64>(), 4ll, "populated Optional serializes as its value");
    expect_eq(t.at("tags").size(), 2uz, "a Vector becomes an array");
    expect(t.at("db").is_table(), "a nested struct becomes a sub-table");
    expect_eq(t.at("db").at("port").get<i64>(), 5433ll, "nested struct member");

    // Round-trip.
    Config back = t.deserialize<Config>();
    expect_eq(back.name, "myapp"s, "renamed key reads back");
    expect(back.mode == Mode::PROD, "enum reads back from its name");
    expect(back.workers.has_value() && *back.workers == 4, "Optional round-trips");
    expect_eq(back.tags.size(), 2uz, "Vector round-trips");
    expect_eq(back.db.host, "db.example"s, "nested struct round-trips");
    expect(back.computed.empty(), "an ignored field stays default");
    expect(back.secret.empty(), "a transient field stays default");

    // An empty Optional is omitted entirely (TOML has no null).
    Config no_workers = cfg;
    no_workers.workers = nullopt;
    expect(!Toml::serialize(no_workers).contains("workers"), "an empty Optional is omitted");
}


namespace {
    struct Limits {
        i64 max_connections = 0;
        Optional<i64> timeout;
    };

    struct Endpoint {
        String host;
        i64 port = 0;
    };

    struct Service {
        [[=Rename("service_name")]]
        String name;

        [[=Flatten]]
        Limits limits;

        [[=TomlFlatten]]
        Endpoint endpoint;
    };
}

void test_toml_rename_and_flatten() {
    Service svc = {
        .name = "api",
        .limits = {.max_connections = 64, .timeout = {}},
        .endpoint = {.host = "localhost", .port = 8080},
    };
    Toml t = Toml::serialize(svc);
    expect(t.contains("service_name") && !t.contains("name"), "Rename renames the key");
    expect(!t.contains("limits") && !t.contains("endpoint"), "flattened members have no table of their own");
    expect_eq(t.at("max_connections").get<i64>(), 64ll, "Flatten lifts fields into the parent table");
    expect(!t.contains("timeout"), "an empty Optional inside a flattened member is still omitted");
    expect_eq(t.at("host").get<String>(), "localhost"s, "TomlFlatten lifts fields too");

    Service back = t.deserialize<Service>();
    expect_eq(back.name, "api"s, "renamed member reads back");
    expect_eq(back.limits.max_connections, 64ll, "flattened member reads back");
    expect(!back.limits.timeout.has_value(), "absent Optional stays empty");
    expect_eq(back.endpoint.port, 8080ll, "TomlFlatten member reads back");
}

namespace {
    class [[=Reflectable]] Secretive {
    public:
        Secretive() = default;
        explicit Secretive(i64 level): _level{level} {}

        [[nodiscard]]
        i64 level() const noexcept {
            return _level;
        }
    private:
        i64 _level = 0;
    };

    class Opaque {
    public:
        Opaque() = default;
        explicit Opaque(String id): _id{Ops::move(id)} {}

        [[nodiscard]]
        const String& id() const noexcept {
            return _id;
        }
    private:
        String _id;
    };
}

void test_toml_private_members() {
    Toml marked = Toml::serialize(Secretive{7});
    expect_eq(marked.at("_level").get<i64>(), 7ll, "a Reflectable class's private member serializes");
    expect_eq(marked.deserialize<Secretive>().level(), i64{7}, "and reads back");

    Toml forced = Toml::serialize(Opaque{"x-1"}, AllowPrivate);
    expect_eq(forced.at("_id").get<String>(), "x-1"s, "AllowPrivate reads a foreign type's private member");
    expect_eq(forced.deserialize<Opaque>(AllowPrivate).id(), "x-1"s, "and writes it back");
}
#endif // __cpp_lib_reflection

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Toml.scalars", test_toml_scalars},
        {"Toml.strings", test_toml_strings},
        {"Toml.tables", test_toml_tables},
        {"Toml.arrays", test_toml_arrays},
        {"Toml.datetimes", test_toml_datetimes},
        {"Toml.serialize", test_toml_serialize},
        {"Toml.errors", test_toml_errors},
        #ifdef __cpp_lib_reflection
        {"Toml.serde", test_toml_serde},
        {"Toml.rename_and_flatten", test_toml_rename_and_flatten},
        {"Toml.private_members", test_toml_private_members},
        #endif
    });
}
