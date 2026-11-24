#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::data::csv::CsvDialect;
using stdx::data::csv::CsvDocument;
using stdx::data::csv::CsvParseException;
using stdx::data::csv::CsvRow;
using stdx::data::csv::CsvTypeException;

#ifdef __cpp_lib_reflection
using stdx::data::AllowPrivate;
using stdx::data::Reflectable;
using stdx::data::Rename;
using stdx::data::Transient;
using stdx::data::csv::CsvFlatten;
using stdx::data::csv::CsvIgnore;
using stdx::data::csv::CsvPropertyName;
#endif

using namespace stdx::test;
using namespace stdx::literals;

/**
 * @brief Basic parsing: header split, rows, field access by index and name.
 */
void test_csv_parse_basic() {
    CsvDocument doc = CsvDocument::parse("name,age,city\nReimu,17,Hakurei\nMarisa,18,Forest\n");
    expect(doc.has_header(), "a document parses with a header by default");
    expect_eq(doc.header().size(), 3uz, "header has three columns");
    expect_eq(doc.header()[0], "name"s, "first column is name");
    expect_eq(doc.size(), 2uz, "two data rows (header excluded)");

    const CsvRow& first = doc.row(0);
    expect_eq(first[0], "Reimu"s, "field by index");
    expect_eq(first.at("city"), "Hakurei"s, "field by column name");
    expect_eq(doc.row(1).at("age"), "18"s, "second row by name");
}

/**
 * @brief Quoting: delimiters, quotes, and newlines inside quoted fields.
 */
void test_csv_quoting() {
    CsvDocument doc = CsvDocument::parse(
        "name,note\n"
        "\"Reimu, Hakurei\",\"she said \"\"hi\"\"\"\n"
        "\"multi\nline\",plain\n"
    );
    expect_eq(doc.row(0)[0], "Reimu, Hakurei"s, "a quoted field keeps an embedded delimiter");
    expect_eq(doc.row(0)[1], "she said \"hi\""s, "a doubled quote collapses to one");
    expect_eq(doc.row(1)[0], "multi\nline"s, "a quoted field keeps an embedded newline");
    expect_eq(doc.row(1)[1], "plain"s, "an unquoted field alongside a quoted one");
}

/**
 * @brief Dialects: a custom delimiter and a headerless document.
 */
void test_csv_dialects() {
    CsvDocument tsv = CsvDocument::parse("a\tb\tc\n1\t2\t3\n", CsvDialect::tsv());
    expect_eq(tsv.header()[1], "b"s, "tab delimiter splits the header");
    expect_eq(tsv.row(0)[2], "3"s, "tab delimiter splits the row");

    CsvDialect semicolon{.delimiter = ';', .quote = '"', .has_header = false};
    CsvDocument doc = CsvDocument::parse("1;2;3\n4;5;6\n", semicolon);
    expect(!doc.has_header(), "a headerless dialect keeps every record as data");
    expect_eq(doc.size(), 2uz, "both records are data rows");
    expect_eq(doc.row(0)[0], "1"s, "semicolon delimiter splits fields");
}

/**
 * @brief Serialization: quoting on the way out, and parse/serialize round-trip.
 */
void test_csv_serialize() {
    CsvDocument doc = CsvDocument::parse("name,note\nReimu,\"a, b\"\n");
    const String text = doc.to_string();
    expect(text.find("\"a, b\"") != String::npos, "a field with a delimiter is quoted on output");
    expect(text.find("name,note") != String::npos, "the header is written first");

    // Round-trip: re-parsing the serialized form yields an equal document.
    expect_eq(CsvDocument::parse(text), doc, "parse/serialize round-trips");

    // A quote in a field is doubled and the field quoted.
    CsvRow row(Vector<String>{"he said \"hi\""});
    CsvDocument one(Vector<String>{"q"}, true, Vector<CsvRow>{row});
    expect(one.to_string().find("\"he said \"\"hi\"\"\"") != String::npos, "an embedded quote is doubled and wrapped");
}

/**
 * @brief Error and edge cases.
 */
void test_csv_errors() {
    expect(!CsvDocument::try_parse("\"unterminated\n").has_value(), "an unterminated quoted field is rejected");
    expect(CsvDocument::try_parse("a,b\n1,2\n").has_value(), "well-formed input parses");

    expect_throws<CsvParseException>(
        [] -> void { static_cast<void>(CsvDocument::parse("a,\"bad\"x\n")); },
        "a stray character after a closing quote is rejected"
    );

    // A trailing newline does not produce a spurious empty row.
    expect_eq(CsvDocument::parse("a\n1\n2\n").size(), 2uz, "trailing newline adds no empty row");

    // Checked accessors throw out of range.
    CsvDocument doc = CsvDocument::parse("a,b\n1,2\n");
    expect_throws<CsvTypeException>(
        [&] -> void { static_cast<void>(doc.row(5)); }, "out-of-range row throws"
    );
    expect_throws<CsvTypeException>(
        [&] -> void { static_cast<void>(doc.row(0).at("missing")); }, "absent column throws"
    );

    // The _csv literal.
    CsvDocument lit = "x,y\n1,2\n"_csv;
    expect_eq(lit.row(0).at("y"), "2"s, "the _csv literal parses");
}

#ifdef __cpp_lib_reflection

enum class Rank: u8 {
    SHRINE_MAIDEN,
    MAGICIAN,
    WITCH,
};

struct Person {
    [[=CsvPropertyName<"full_name">]]
    String name;

    i32 age = 0;
    bool active = false;
    Rank rank = Rank::SHRINE_MAIDEN;

    [[=CsvIgnore]]
    String note;

    [[=Transient]]
    String secret;
};

/**
 * @brief Reflection-driven row<->struct serialize/deserialize.
 */
void test_csv_serde() {
    Vector<Person> people = {
        Person{.name = "Reimu", .age = 17, .active = true, .rank = Rank::SHRINE_MAIDEN, .note = "x", .secret = "y"},
        Person{.name = "Marisa", .age = 18, .active = false, .rank = Rank::WITCH, .note = "z", .secret = "w"},
    };

    CsvDocument doc = CsvDocument::serialize(people);
    expect(doc.has_header(), "serialized rows carry a header");
    // Header: full_name (renamed), age, active, rank. note and secret excluded.
    expect_eq(doc.header().size(), 4uz, "ignored and transient fields are excluded from the header");
    expect_eq(doc.header()[0], "full_name"s, "CsvPropertyName renames the column");
    expect(stdx::ranges::find(doc.header(), "note"s) == doc.header().end(), "a CsvIgnore field is absent");
    expect(stdx::ranges::find(doc.header(), "secret"s) == doc.header().end(), "a Transient field is absent");

    expect_eq(doc.row(0).at("full_name"), "Reimu"s, "renamed column holds the value");
    expect_eq(doc.row(0).at("age"), "17"s, "integer field");
    expect_eq(doc.row(0).at("active"), "true"s, "bool field as true/false");
    expect_eq(doc.row(1).at("rank"), "WITCH"s, "enum field as its name");

    // Round-trip back into structs.
    Vector<Person> back = doc.deserialize<Person>();
    expect_eq(back.size(), 2uz, "all rows read back");
    expect_eq(back[0].name, "Reimu"s, "renamed column reads into the field");
    expect_eq(back[0].age, 17, "integer round-trips");
    expect(back[0].active, "bool round-trips");
    expect(back[1].rank == Rank::WITCH, "enum reads back from its name");
    expect(back[0].note.empty(), "an ignored field stays default on the way in");
    expect(back[0].secret.empty(), "a transient field stays default on the way in");
}


namespace {
    struct Location {
        String street;
        [[=Rename("town")]]
        String city;
    };

    struct Customer {
        [[=Rename("customer")]]
        String name;

        [[=CsvFlatten]]
        Location address;

        i32 orders = 0;
    };
}

void test_csv_rename_and_flatten() {
    Vector<Customer> rows = {
        {.name = "Reimu", .address = {.street = "Hakurei", .city = "Gensokyo"}, .orders = 3},
        {.name = "Nitori", .address = {.street = "River", .city = "Youkai"}, .orders = 12},
    };
    CsvDocument doc = CsvDocument::serialize(rows);
    require(doc.header().size() == 4, "a flattened struct contributes its fields as columns");
    expect_eq(doc.header()[0], "customer"s, "Rename renames the column");
    expect_eq(doc.header()[1], "street"s, "flattened columns sit where the member was");
    expect_eq(doc.header()[2], "town"s, "with their own renames");
    expect_eq(doc.header()[3], "orders"s, "followed by the rest of the row");

    Vector<Customer> back = doc.deserialize<Customer>();
    require(back.size() == 2, "both rows read back");
    expect_eq(back[1].address.city, "Youkai"s, "flattened columns read back into the nested struct");
    expect_eq(back[1].orders, 12, "plain columns too");
}

namespace {
    class Coordinates {
    public:
        Coordinates() = default;
        Coordinates(f64 lat, f64 lon): _lat{lat}, _lon{lon} {}

        [[nodiscard]]
        f64 lat() const noexcept {
            return _lat;
        }
    private:
        f64 _lat = 0;
        f64 _lon = 0;
    };

    struct Landmark {
        String name;
        [[=CsvFlatten]]
        Coordinates position; // foreign type, flattened into the row
    };

    class [[=Reflectable]] Tally {
    public:
        String label;
        Tally() = default;
        Tally(String l, i32 n): label{Ops::move(l)}, _count{n} {}

        [[nodiscard]]
        i32 count() const noexcept {
            return _count;
        }
    private:
        i32 _count = 0;
    };
}

void test_csv_private_members() {
    Vector<Tally> tallies = {{"votes", 12}};
    CsvDocument marked = CsvDocument::serialize(tallies);
    require(marked.header().size() == 2, "a Reflectable class's private member is a column");
    expect_eq(marked.header()[1], "_count"s, "under its own name");
    expect_eq(marked.deserialize<Tally>()[0].count(), 12, "and reads back");

    Vector<Landmark> rows = {{.name = "Peak", .position = Coordinates{35.36, 138.73}}};
    CsvDocument forced = CsvDocument::serialize(rows, AllowPrivate);
    require(forced.header().size() == 3, "AllowPrivate reaches into a flattened foreign type");
    expect_eq(forced.header()[1], "_lat"s, "its private members become columns");
    Vector<Landmark> back = forced.deserialize<Landmark>(AllowPrivate);
    expect(back[0].position.lat() > 35.35 && back[0].position.lat() < 35.37, "and read back");
}
#endif // __cpp_lib_reflection

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Csv.parse_basic", test_csv_parse_basic},
        {"Csv.quoting", test_csv_quoting},
        {"Csv.dialects", test_csv_dialects},
        {"Csv.serialize", test_csv_serialize},
        {"Csv.errors", test_csv_errors},
        #ifdef __cpp_lib_reflection
        {"Csv.serde", test_csv_serde},
        {"Csv.rename_and_flatten", test_csv_rename_and_flatten},
        {"Csv.private_members", test_csv_private_members},
        #endif
    });
}
