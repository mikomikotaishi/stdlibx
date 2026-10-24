#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::data::json::Json;
using stdx::data::json::JsonObject;
using stdx::data::json::JsonParseException;
using stdx::data::json::JsonPointer;
using stdx::data::json::JsonPointerException;
using stdx::data::json::JsonTypeException;

#ifdef __cpp_lib_reflection
using stdx::data::Transient;
using stdx::data::json::JsonIgnore;
using stdx::data::json::JsonPropertyName;
#endif

using namespace stdx::test;

/**
 * @brief Parsing each scalar kind, and the type queries over the result.
 */
void test_json_parse_scalars() {
    expect(Json::parse("null").is_null(), "null parses to null");
    expect(Json::parse("true").get<bool>(), "true parses to boolean true");
    expect(!Json::parse("false").get<bool>(), "false parses to boolean false");

    Json n = Json::parse("42");
    expect(n.is_integer(), "42 parses as an integer");
    expect_eq(n.get<i64>(), 42ll, "42 reads back as 42");

    Json f = Json::parse("3.5");
    expect(f.is_float(), "3.5 parses as a float");
    expect_near(f.get<f64>(), 3.5, 1e-12, "3.5 reads back as 3.5");

    Json neg = Json::parse("-17");
    expect_eq(neg.get<i64>(), -17ll, "negative integer");

    Json exp = Json::parse("1e3");
    expect(exp.is_float(), "1e3 is a float");
    expect_near(exp.get<f64>(), 1000.0, 1e-9, "1e3 is 1000");

    Json s = Json::parse("\"hello\"");
    expect(s.is_string(), "quoted text parses as a string");
    expect_eq(s.get<String>(), "hello"s, "string content preserved");
}

/**
 * @brief The nullptr <-> JSON null correspondence, across every path.
 */
void test_json_null() {
    // Construction and the default both yield null.
    expect(Json(nullptr).is_null(), "Json(nullptr) is null");
    expect(Json().is_null(), "a default-constructed Json is null");
    expect(!Json(nullptr).is_boolean(), "nullptr does not select the bool constructor");
    expect_eq(Json(nullptr).type(), Json::Type::NULL_VALUE, "nullptr's type is NULL_VALUE");

    // Equality: nullptr-built and default-built nulls are equal; null != a value.
    expect_eq(Json(nullptr), Json(), "nullptr-built and default-built nulls compare equal");
    expect_ne(Json(nullptr), Json(0), "null is not equal to the integer 0");
    expect_ne(Json(nullptr), Json(false), "null is not equal to false");

    // Assignment through nullptr (the `j[key] = nullptr` idiom).
    Json obj = Json::object();
    obj["a"] = nullptr;
    expect(obj["a"].is_null(), "assigning nullptr to a member stores null");

    // nullptr directly inside a braced initializer, as nlohmann::json allows.
    Json doc = {
        {"name", "John Doe"},
        {"nothing", nullptr},
    };
    expect(doc.is_object(), "an init-list with a nullptr value is still an object");
    expect(doc["nothing"].is_null(), "a nullptr entry in the init-list becomes null");
    expect_eq(doc["name"].get<String>(), "John Doe"s, "the sibling entry is unaffected");
    expect_eq(doc.to_string(), R"({"name":"John Doe","nothing":null})"s, "init-list with null serializes as expected");

    // Serialize and round-trip.
    expect_eq(Json(nullptr).to_string(), "null"s, "nullptr serializes to the literal null");
    expect(Json::parse("null").is_null(), "the literal null parses to null");
    expect_eq(Json::parse(Json(nullptr).to_string()), Json(nullptr), "null round-trips");
    expect_eq(obj.to_string(), R"({"a":null})"s, "a null member serializes in context");

    // Safe accessors on a null report "no value" rather than throwing.
    expect(!Json(nullptr).as<i64>().has_value(), "as<i64> on null is empty");
    expect(!Json(nullptr).as<String>().has_value(), "as<String> on null is empty");
    expect(Json(nullptr).empty(), "null counts as empty");
    expect_eq(Json(nullptr).size(), 0uz, "null has size 0");
}

/**
 * @brief Arrays and objects, including nesting and type queries.
 */
void test_json_parse_containers() {
    Json arr = Json::parse("[1, 2, 3]");
    expect(arr.is_array(), "array parses as array");
    expect_eq(arr.size(), 3uz, "array has three elements");
    expect_eq(arr[0uz].get<i64>(), 1ll, "first element");
    expect_eq(arr[2uz].get<i64>(), 3ll, "last element");

    Json obj = Json::parse(R"({"name": "Reimu", "age": 17, "shrine": true})");
    expect(obj.is_object(), "object parses as object");
    expect_eq(obj.size(), 3uz, "object has three members");
    expect_eq(obj["name"].get<String>(), "Reimu"s, "string member");
    expect_eq(obj["age"].get<i64>(), 17ll, "integer member");
    expect(obj["shrine"].get<bool>(), "boolean member");

    Json nested = Json::parse(R"({"a": {"b": [1, {"c": 2}]}})");
    expect_eq(nested["a"]["b"][1uz]["c"].get<i64>(), 2ll, "deep path reads through nesting");

    expect(Json::parse("[]").is_array() && Json::parse("[]").empty(), "empty array");
    expect(Json::parse("{}").is_object() && Json::parse("{}").empty(), "empty object");
}

/**
 * @brief String escapes, including \\uXXXX and surrogate pairs.
 */
void test_json_string_escapes() {
    expect_eq(Json::parse(R"("a\nb")").get<String>(), "a\nb"s, "newline escape");
    expect_eq(Json::parse(R"("tab\there")").get<String>(), "tab\there"s, "tab escape");
    expect_eq(Json::parse(R"("quote\"end")").get<String>(), "quote\"end"s, "escaped quote");
    expect_eq(Json::parse(R"("slash\/")").get<String>(), "slash/"s, "escaped solidus");

    // U+00E9 (e-acute) -> UTF-8 C3 A9.
    expect_eq(Json::parse(R"("\u00e9")").get<String>(), "\xc3\xa9"s, "BMP unicode escape to UTF-8");
    // U+1F600 via surrogate pair -> UTF-8 F0 9F 98 80.
    expect_eq(
        Json::parse(R"("\ud83d\ude00")").get<String>(),
        "\xf0\x9f\x98\x80"s,
        "surrogate pair decodes to astral code point"
    );
}

/**
 * @brief The safe (as) and throwing (get) typed accessors.
 */
void test_json_typed_access() {
    Json n = Json(42);
    expect(n.as<i64>().has_value(), "as<i64> succeeds on an integer");
    expect(!n.as<String>().has_value(), "as<String> fails on an integer");
    expect_eq(n.as_or<i64>(0), 42ll, "as_or returns the value when present");
    expect_eq(Json("x").as_or<i64>(99), 99ll, "as_or returns the fallback on mismatch");

    // Numbers cross-convert between integral and floating requests.
    expect_near(Json(7).as<f64>().value_or(0.0), 7.0, 1e-12, "an integer reads as a float");
    expect_eq(Json(2.9).as<i64>().value_or(0), 2ll, "a float truncates to an integer");

    expect_throws<JsonTypeException>(
        [] -> void { static_cast<void>(Json("not a number").get<i64>()); },
        "get<i64> on a string throws"
    );

    // Implicit conversion.
    Json s = Json("convert me");
    String out = s;
    expect_eq(out, "convert me"s, "implicit conversion to String");
}

/**
 * @brief operator[] auto-vivification versus at()'s checked access.
 */
void test_json_access_semantics() {
    Json obj = Json::object();
    obj["a"]["b"] = 1; // builds the path
    expect_eq(obj["a"]["b"].get<i64>(), 1ll, "subscript auto-vivifies nested objects");

    expect(obj.contains("a"), "contains finds a present key");
    expect(!obj.contains("missing"), "contains rejects an absent key");

    expect_throws<JsonTypeException>(
        [&] -> void { static_cast<void>(obj.at("missing")); },
        "at() throws on a missing key"
    );

    expect_eq(obj.value("missing", 7ll), 7ll, "value() returns the fallback for a missing key");
    expect_eq(obj.value<i64>("a", 7ll), 7ll, "value() returns the fallback on a type mismatch");

    Json arr = Json::array();
    arr.push_back(1);
    arr += 2;
    expect_eq(arr.size(), 2uz, "push_back and += both append");
    expect_throws<JsonTypeException>(
        [&] -> void { static_cast<void>(arr[5uz]); },
        "out-of-range array index throws"
    );
}

/**
 * @brief Braced-init construction and the object/array disambiguation.
 */
void test_json_construction() {
    Json obj = {{"name", "Marisa"}, {"power", 9000}};
    expect(obj.is_object(), "a list of string-keyed pairs is an object");
    expect_eq(obj["name"].get<String>(), "Marisa"s, "object key from init list");
    expect_eq(obj["power"].get<i64>(), 9000ll, "object value from init list");

    Json arr = {1, 2, 3};
    expect(arr.is_array(), "a list of scalars is an array");

    Json forced = Json::array({"a", "b"});
    expect(forced.is_array() && forced.size() == 2, "array() forces an array");

    // The fluent builder.
    Json built = JsonObject::Builder()
        .add("host", "localhost")
        .add("port", 8080)
        .add("tags", Json::Array({"x", "y"}))
        .build();
    expect_eq(built["host"].get<String>(), "localhost"s, "builder sets string member");
    expect_eq(built["port"].get<i64>(), 8080ll, "builder sets integer member");
    expect_eq(built["tags"].size(), 2uz, "builder nests an array");
}

/**
 * @brief Serialization: compact, pretty, round-trip, and key-order preservation.
 */
void test_json_serialize() {
    Json obj = Json::parse(R"({"b":1,"a":2,"c":3})");
    // Insertion/document order is preserved, not sorted.
    expect_eq(obj.to_string(), R"({"b":1,"a":2,"c":3})"s, "object key order is preserved on round-trip");

    expect_eq(Json(true).to_string(), "true"s, "bool serializes");
    expect_eq(Json().to_string(), "null"s, "null serializes");
    expect_eq(Json(42).to_string(), "42"s, "integer serializes without a decimal point");
    expect_eq(Json(1.0).to_string(), "1.0"s, "a whole float keeps a .0 so it reparses as a float");

    // String escaping round-trips.
    Json s("line1\nline2\t\"q\"");
    expect_eq(Json::parse(s.to_string()).get<String>(), s.get<String>(), "escaped string round-trips");

    // Pretty printing contains newlines and indentation.
    Json nested = Json::parse(R"({"a":[1,2]})");
    String pretty = nested.dump(2);
    expect(pretty.find('\n') != String::npos, "pretty output has newlines");
    expect_eq(Json::parse(pretty), nested, "pretty output reparses to the same value");

    // Formatter integration.
    expect_eq(Ops::fmt("{}", Json(42)), "42"s, "Json formats through {}");
}

/**
 * @brief JSON Pointer (RFC 6901): parsing, escaping, resolution, and the edges.
 */
void test_json_pointer() {
    Json doc = Json::parse(R"({
        "users": [{"name": "Reimu"}, {"name": "Marisa"}],
        "a/b": 1,
        "m~n": 2,
        "": 3,
        "nested": {"": {"x": 42}}
    })");

    // Basic resolution through objects and arrays.
    JsonPointer ptr = JsonPointer::parse("/users/1/name");
    expect_eq(doc.at(ptr).get<String>(), "Marisa"s, "pointer resolves object/array path");
    expect_eq(doc.at("/users/0/name"_jptr).get<String>(), "Reimu"s, "_jptr literal resolves");

    // The empty pointer addresses the whole document.
    JsonPointer whole = JsonPointer::parse("");
    expect(whole.empty(), "empty text parses to the whole-document pointer");
    expect(&doc.at(whole) == &doc, "the empty pointer resolves to the document itself");

    // Escaping: ~1 is '/', ~0 is '~'.
    expect_eq(doc.at("/a~1b"_jptr).get<i64>(), 1ll, "~1 decodes to a slash in the key");
    expect_eq(doc.at("/m~0n"_jptr).get<i64>(), 2ll, "~0 decodes to a tilde in the key");

    // A literal empty-string key, and nesting through one.
    expect_eq(doc.at("/"_jptr).get<i64>(), 3ll, "a lone '/' addresses the empty-string key");
    expect_eq(doc.at("/nested//x"_jptr).get<i64>(), 42ll, "an empty token nests through the empty-string key");

    // Non-throwing find: present vs absent.
    expect(doc.find("/users/0"_jptr) != nullptr, "find returns a pointer for a resolvable path");
    expect(doc.find("/users/9"_jptr) == nullptr, "find returns nullptr for an out-of-range index");
    expect(doc.find("/missing"_jptr) == nullptr, "find returns nullptr for an absent key");
    expect(doc.contains("/users/1/name"_jptr), "contains is true for a resolvable pointer");
    expect(!doc.contains("/users/1/age"_jptr), "contains is false for an unresolvable pointer");

    // The StringView convenience overload parses inline.
    expect(doc.find("/users/0/name") != nullptr, "find(StringView) parses and resolves");
    expect(doc.find("not-a-pointer") == nullptr, "find(StringView) returns nullptr on malformed syntax");

    // Array index rules: '-' and leading zeros never resolve to an element.
    expect(doc.find("/users/-"_jptr) == nullptr, "'-' does not resolve to an existing element");
    expect(doc.find("/users/01"_jptr) == nullptr, "a leading-zero index is rejected");
    expect(doc.find("/users/x"_jptr) == nullptr, "a non-numeric array token does not resolve");

    // Descending past a scalar fails rather than throwing.
    expect(doc.find("/a~1b/deeper"_jptr) == nullptr, "a token beyond a scalar does not resolve");

    // at() throws on an unresolvable (but well-formed) pointer.
    expect_throws<JsonPointerException>(
        [&] -> void { static_cast<void>(doc.at("/users/9"_jptr)); },
        "at() throws when the pointer does not resolve"
    );

    // Malformed pointer syntax.
    expect_throws<JsonPointerException>(
        [] -> void { static_cast<void>(JsonPointer::parse("users/0")); },
        "a non-empty pointer not starting with '/' is rejected"
    );
    expect_throws<JsonPointerException>(
        [] -> void { static_cast<void>(JsonPointer::parse("/bad~escape")); },
        "a malformed '~' escape is rejected"
    );
    expect(!JsonPointer::try_parse("nope").has_value(), "try_parse returns empty on malformed syntax");
    expect(JsonPointer::try_parse("/users/0").has_value(), "try_parse returns a value on valid syntax");

    // Round-trip: parse -> to_string re-escapes faithfully.
    expect_eq(JsonPointer::parse("/a~1b/m~0n").to_string(), "/a~1b/m~0n"s, "to_string re-escapes tokens");
    expect_eq(JsonPointer::parse("").to_string(), ""s, "the whole-document pointer serializes to empty");

    // Mutable resolution: find returns a mutable reference that writes through.
    if (Json* name = doc.find("/users/0/name"_jptr)) {
        *name = "Sanae";
    }
    expect_eq(doc.at("/users/0/name"_jptr).get<String>(), "Sanae"s, "find yields a mutable reference");

    // Programmatic building via push.
    JsonPointer built = Json::Pointer()
        .push("users")
        .push("1")
        .push("name");
    expect_eq(built.to_string(), "/users/1/name"s, "push builds a pointer token by token");
    expect_eq(doc.at(built).get<String>(), "Marisa"s, "a built pointer resolves");
}

/**
 * @brief try_parse and the parse-error path.
 */
void test_json_errors() {
    expect(!Json::try_parse("{bad}").has_value(), "try_parse returns empty on malformed input");
    expect(Json::try_parse("[1,2,3]").has_value(), "try_parse returns a value on valid input");

    expect_throws<JsonParseException>(
        [] -> void { static_cast<void>(Json::parse("{\"a\":}")); },
        "missing value is rejected"
    );
    expect_throws<JsonParseException>(
        [] -> void { static_cast<void>(Json::parse("[1,2,")); },
        "unterminated array is rejected"
    );
    expect_throws<JsonParseException>(
        [] -> void { static_cast<void>(Json::parse("01")); },
        "leading zero is rejected"
    );
    expect_throws<JsonParseException>(
        [] -> void { static_cast<void>(Json::parse("\"unterminated")); },
        "unterminated string is rejected"
    );
    expect_throws<JsonParseException>(
        [] -> void { static_cast<void>(Json::parse("{} trailing")); },
        "trailing content after a value is rejected"
    );

    // Duplicate keys: last value wins.
    expect_eq(Json::parse(R"({"k":1,"k":2})")["k"].get<i64>(), 2ll, "duplicate key keeps the last value");

    // The _json literal.
    Json lit = R"({"ok":true})"_json;
    expect(lit["ok"].get<bool>(), "the _json literal parses");
}

#ifdef __cpp_lib_reflection
enum class Element: u8 {
    FIRE,
    WATER,
    EARTH,
    WIND,
};

struct Spell {
    String name;
    i32 cost = 0;
    Element element = Element::FIRE;
};

struct Player {
    [[=JsonPropertyName<"full_name">]]
    String name;

    i32 hp = 100;
    Optional<i32> mana;
    Vector<Spell> spells;

    [[=JsonIgnore]]
    String scratch;

    [[=Transient]]
    String password;
};

/**
 * @brief Reflection-driven serialize/deserialize.
 */
void test_json_serde() {
    Player reimu = {
        .name = "Reimu",
        .hp = 120,
        .mana = 30,
        .spells = {
            Spell {.name = "Fantasy Seal", .cost = 50, .element = Element::WIND},
            Spell {.name = "Evil-Sealing Circle", .cost = 20, .element = Element::FIRE},
        },
        .scratch = "do not serialize me",
        .password = "never persist me",
    };

    Json j = Json::serialize(reimu);
    expect(j.is_object(), "a struct serializes to an object");
    expect(j.contains("full_name"), "JsonPropertyName renames the key");
    expect(!j.contains("name"), "the original field name is not used");
    expect(!j.contains("scratch"), "a JsonIgnore field is omitted");
    expect(!j.contains("password"), "a Transient field is omitted");
    expect_eq(j["full_name"].get<String>(), "Reimu"s, "renamed string member");
    expect_eq(j["hp"].get<i64>(), 120ll, "integer member");
    expect_eq(j["mana"].get<i64>(), 30ll, "populated Optional serializes as its value");
    expect_eq(j["spells"].size(), 2uz, "a Vector member becomes an array");
    expect_eq(j["spells"][0uz]["element"].get<String>(), "WIND"s, "an enum serializes as its name");

    // Round-trip back into a struct.
    Player back = j.deserialize<Player>();
    expect_eq(back.name, "Reimu"s, "renamed key reads back into the field");
    expect_eq(back.hp, 120, "integer round-trips");
    expect(back.mana.has_value() && *back.mana == 30, "Optional round-trips");
    expect_eq(back.spells.size(), 2uz, "the array reads back into the Vector");
    expect(back.spells[1].element == Element::FIRE, "enum reads back from its name");
    expect(back.scratch.empty(), "a JsonIgnore field stays default on the way in");
    expect(back.password.empty(), "a Transient field stays default on the way in");

    // An empty Optional serializes to JSON null.
    Player empty_mana = reimu;
    empty_mana.mana = nullopt;
    expect(Json::serialize(empty_mana).at("mana").is_null(), "an empty Optional serializes to null");

    // A missing Optional becomes an empty Optional, not an error.
    Player none = Json::parse(R"({"full_name":"Marisa","hp":90})").deserialize<Player>();
    expect_eq(none.name, "Marisa"s, "parse + deserialize parses and maps");
    expect(!none.mana.has_value(), "a missing Optional field stays empty");
    expect_eq(none.hp, 90, "present field is read");
    expect_eq(none.spells.size(), 0uz, "a missing Vector field stays empty");

    // An explicit null (not just an absent key) also becomes an empty Optional.
    Player null_mana = Json::parse(R"({"full_name":"Alice","hp":80,"mana":null})").deserialize<Player>();
    expect(!null_mana.mana.has_value(), "an explicit null Optional field reads back empty");

    // Serialize straight to text.
    String text = Json::serialize(reimu).to_string();
    expect(text.find("full_name") != String::npos, "serialize + to_string emits the renamed key");
    expect(text.find("scratch") == String::npos, "the ignored field is omitted");
}
#endif // __cpp_lib_reflection

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Json.parse_scalars", test_json_parse_scalars},
        {"Json.null", test_json_null},
        {"Json.parse_containers", test_json_parse_containers},
        {"Json.string_escapes", test_json_string_escapes},
        {"Json.typed_access", test_json_typed_access},
        {"Json.access_semantics", test_json_access_semantics},
        {"Json.construction", test_json_construction},
        {"Json.serialize", test_json_serialize},
        {"Json.pointer", test_json_pointer},
        {"Json.errors", test_json_errors},
        #ifdef __cpp_lib_reflection
        {"Json.serde", test_json_serde},
        #endif
    });
}
