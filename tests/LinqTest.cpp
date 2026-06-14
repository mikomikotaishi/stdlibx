import stdx;

using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::linq::Query;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_basic_operations() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // where (filter) - using template template parameter syntax
    Vector<i32> evens = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .to<Vector>();
    expect_eq(evens, Vector<i32>{2, 4, 6, 8, 10}, "where: even numbers");

    // select (map/transform) - using explicit type syntax
    Vector<i32> squares = Query(numbers)
        .select([](i32 x) -> i32 { return x * x; })
        .to<Vector<i32>>();
    expect_eq(squares, Vector<i32>{1, 4, 9, 16, 25, 36, 49, 64, 81, 100}, "select: squares");

    // select via from() method
    Vector<i32> incremented = Query<Vector<i32>>::from(numbers)
        .select([](i32 x) -> i32 { return x + 1; })
        .to<Vector>();
    expect_eq(incremented, Vector<i32>{2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, "select via from(): +1");

    Vector<String> to_strings = Query<>::from(numbers)
        .select([](i32 x) -> String { return Integer::to_string(x); })
        .to<Vector>();
    expect_eq(
        to_strings,
        Vector<String>{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"},
        "select: to strings"
    );

    // skip
    Vector<i32> skipped = Query(numbers)
        .skip(5)
        .to<Vector>();
    expect_eq(skipped, Vector<i32>{6, 7, 8, 9, 10}, "skip 5");

    // take
    Vector<i32> taken = Query(numbers)
        .take(3)
        .to<Vector>();
    expect_eq(taken, Vector<i32>{1, 2, 3}, "take 3");
}

void test_chaining() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Complex chain: filter evens, square them, take first 3
    Vector<i32> result = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .select([](i32 x) -> i32 { return x * x; })
        .take(3)
        .to<Vector>();
    expect_eq(result, Vector<i32>{4, 16, 36}, "evens squared, first 3");
}

void test_aggregation() {
    Vector<i32> numbers = {1, 2, 3, 4, 5};

    expect(Query(numbers).any(), "any() on non-empty");
    expect(Query(numbers).any([](i32 x) -> bool { return x % 2 == 0; }), "any even");
    expect(Query(numbers).all([](i32 x) -> bool { return x > 0; }), "all positive");
    expect(!Query(numbers).all([](i32 x) -> bool { return x % 2 == 0; }), "not all even");
    expect(Query(numbers).none([](i32 x) -> bool { return x < 0; }), "none negative");
    expect(!Query(numbers).none([](i32 x) -> bool { return x % 2 == 0; }), "some even");
    expect_eq(Query(numbers).count(), 5uz, "count");
    expect_eq(Query(numbers).count([](i32 x) -> bool { return x % 2 == 0; }), 2uz, "count even");
    expect(Query(numbers).contains(3), "contains 3");
    expect(!Query(numbers).contains(99), "does not contain 99");
}

void test_terminal_operations() {
    Vector<i32> numbers = {5, 2, 8, 1, 9};
    Vector<i32> empty;

    expect_eq(Query(numbers).first(), 5, "first");
    expect_eq(Query(empty).first_or(42), 42, "first_or on empty");
    expect_eq(Query(numbers).last(), 9, "last");
    expect_eq(Query(empty).last_or(-1), -1, "last_or on empty");
    expect_eq(Query(numbers).element_at(2), 8, "element_at(2)");
    expect_eq(Query(numbers).element_at_or(100, -1), -1, "element_at_or out of range");

    Vector<i32> single_vec = {99};
    expect_eq(Query(single_vec).single(), 99, "single");
    expect_eq(Query(numbers).single([](i32 x) -> bool { return x == 8; }), 8, "single matching predicate");
    expect_eq(
        Query(numbers).single([](i32 x) -> bool { return x > 100; }, -1),
        -1,
        "single with no match returns default"
    );

    expect_eq(Query(numbers).to<Vector>().size(), 5uz, "to<Vector> size");
}

void test_sorting() {
    Vector<i32> numbers = {5, 2, 8, 1, 9, 2, 5, 3};

    Vector<i32> sorted = Query(numbers)
        .order_by([](i32 x) -> i32 { return x; })
        .to<Vector>();
    expect_eq(sorted, Vector<i32>{1, 2, 2, 3, 5, 5, 8, 9}, "order_by ascending");

    Vector<i32> sorted_desc = Query(numbers)
        .order_by_descending([](i32 x) -> i32 { return x; })
        .to<Vector>();
    expect_eq(sorted_desc, Vector<i32>{9, 8, 5, 5, 3, 2, 2, 1}, "order_by_descending");

    // distinct returns the unique elements in sorted order
    Vector<i32> unique = Query(numbers)
        .distinct()
        .to<Vector>();
    expect_eq(unique, Vector<i32>{1, 2, 3, 5, 8, 9}, "distinct (sorted unique)");

    Vector<i32> reversed = Query(numbers)
        .reverse()
        .to<Vector>();
    expect_eq(reversed, Vector<i32>{3, 5, 2, 9, 1, 8, 2, 5}, "reverse");
}

void test_error_cases() {
    Vector<i32> empty;
    Vector<i32> multiple = {1, 2};

    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).first(); }, "first() on empty throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).last(); }, "last() on empty throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).element_at(0); }, "element_at() on empty throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).single(); }, "single() on empty throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(multiple).single(); }, "single() on multiple elements throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).min(); }, "min() on empty throws"
    );
    expect_throws<InvalidOperationException>(
        [&] -> void { (void)Query(empty).max(); }, "max() on empty throws"
    );
}

void test_with_strings() {
    Vector<String> words = {"apple", "banana", "apricot", "blueberry", "avocado"};

    Vector<String> words_of_a = Query(words)
        .where([](const String& s) -> bool { return s[0] == 'a'; })
        .to<Vector<String>>();
    expect_eq(words_of_a, Vector<String>{"apple", "apricot", "avocado"}, "words starting with 'a'");

    Vector<usize> lengths = Query(words)
        .select([](const String& s) -> usize { return s.size(); })
        .to<Vector<usize>>();
    expect_eq(lengths, Vector<usize>{5, 6, 7, 9, 7}, "word lengths");

    expect(Query(words).contains(String("banana")), "contains 'banana'");
}

void test_skip_take_while() {
    Vector<i32> numbers = {1, 2, 3, 10, 20, 30, 4, 5};

    Vector<i32> skipped = Query(numbers)
        .skip_while([](i32 x) -> bool { return x < 10; })
        .to<Vector>();
    expect_eq(skipped, Vector<i32>{10, 20, 30, 4, 5}, "skip_while < 10");

    Vector<i32> taken = Query(numbers)
        .take_while([](i32 x) -> bool { return x < 10; })
        .to<Vector>();
    expect_eq(taken, Vector<i32>{1, 2, 3}, "take_while < 10");
}

void test_min_max_sum() {
    Vector<i32> numbers = {5, 2, 8, 1, 9};

    expect_eq(Query(numbers).min(), 1, "min");
    expect_eq(Query(numbers).max(), 9, "max");

    #ifdef __cpp_lib_ranges_fold
    expect_eq(Query(numbers).sum(), 25, "sum");
    expect_eq(
        Query(numbers).aggregate(1, [](i32 acc, i32 x) -> i32 { return acc * x; }),
        720,
        "aggregate: product"
    );
    expect_eq(
        Query(numbers).where([](i32 x) -> bool { return x % 2 == 0; }).sum(),
        10,
        "sum of evens"
    );
    #endif
}

void test_concat() {
    Vector<i32> a = {1, 2, 3};
    Vector<i32> b = {4, 5, 6};

    Vector<i32> combined = Query(a)
        .concat(b)
        .to<Vector>();
    expect_eq(combined, Vector<i32>{1, 2, 3, 4, 5, 6}, "concat");
}

void test_select_many() {
    Vector<Vector<i32>> nested = {{1, 2}, {3, 4}, {5, 6}};

    Vector<i32> flat = Query(nested)
        .select_many([](const Vector<i32>& v) -> const Vector<i32>& { return v; })
        .to<Vector>();
    expect_eq(flat, Vector<i32>{1, 2, 3, 4, 5, 6}, "select_many (flatten)");

    Vector<i32> flat_2 = Query(nested)
        .flatten()
        .to<Vector>();
    expect_eq(flat_2, Vector<i32>{1, 2, 3, 4, 5, 6}, "flatten");
}

void test_keys_values() {
    Vector<Pair<String, i32>> entries = {{"Alice", 90}, {"Bob", 85}, {"Carol", 92}};

    Vector<String> names = Query(entries)
        .keys()
        .to<Vector<String>>();
    expect_eq(names, Vector<String>{"Alice", "Bob", "Carol"}, "keys");

    Vector<i32> scores = Query(entries)
        .values()
        .to<Vector<i32>>();
    expect_eq(scores, Vector<i32>{90, 85, 92}, "values");

    Vector<String> top_students = Query(entries)
        .where([](const Pair<String, i32>& p) -> bool { return p.second >= 90; })
        .keys()
        .to<Vector<String>>();
    expect_eq(top_students, Vector<String>{"Alice", "Carol"}, "keys where score >= 90");

    // TreeMap iterates in sorted key order.
    TreeMap<String, i32> tree_scores = {{"alice", 90}, {"bob", 85}, {"carol", 92}};

    Vector<String> tree_keys = Query(tree_scores)
        .keys()
        .to<Vector<String>>();
    expect_eq(tree_keys, Vector<String>{"alice", "bob", "carol"}, "TreeMap keys (sorted)");

    Vector<i32> tree_values = Query(tree_scores)
        .values()
        .to<Vector<i32>>();
    expect_eq(tree_values, Vector<i32>{90, 85, 92}, "TreeMap values (key order)");

    Vector<String> tree_top = Query(tree_scores)
        .where([](const Pair<const String, i32>& p) -> bool { return p.second >= 90; })
        .keys()
        .to<Vector<String>>();
    expect_eq(tree_top, Vector<String>{"alice", "carol"}, "TreeMap keys where score >= 90");

    // HashMap iteration order is unspecified, so sort before comparing.
    HashMap<String, i32> hash_scores = {{"alice", 90}, {"bob", 85}, {"carol", 92}};

    Vector<String> hash_keys = Query(hash_scores)
        .keys()
        .order_by([](const String& s) -> String { return s; })
        .to<Vector<String>>();
    expect_eq(hash_keys, Vector<String>{"alice", "bob", "carol"}, "HashMap keys (sorted)");

    Vector<i32> hash_values = Query(hash_scores)
        .values()
        .order_by([](i32 v) -> i32 { return v; })
        .to<Vector<i32>>();
    expect_eq(hash_values, Vector<i32>{85, 90, 92}, "HashMap values (sorted)");

    #ifdef __cpp_lib_ranges_fold
    expect_eq(Query(hash_scores).values().sum(), 267, "HashMap value sum");
    #endif
}

#ifdef __cpp_lib_ranges_enumerate
void test_enumerate() {
    Vector<String> words = {"foo", "bar", "baz"};

    Vector<usize> indices;
    Vector<String> values;
    Query(words)
        .enumerate()
        .for_each([&](auto pair) -> void {
            auto [i, w] = pair;
            indices.push_back(i);
            values.push_back(String(w));
        });
    expect_eq(indices, Vector<usize>{0, 1, 2}, "enumerate indices");
    expect_eq(values, Vector<String>{"foo", "bar", "baz"}, "enumerate values");
}
#endif

void test_zip() {
    Vector<String> names = {"Alice", "Bob", "Carol"};
    Vector<i32> scores = {90, 85, 92};

    Vector<String> formatted = Query(names)
        .zip(scores)
        .select([](auto pair) -> String {
            auto [name, score] = pair;
            return stdx::fmt::format("{}: {}", name, score);
        })
        .to<Vector<String>>();
    expect_eq(
        formatted,
        Vector<String>{"Alice: 90", "Bob: 85", "Carol: 92"},
        "zip names and scores"
    );
}

void test_for_each() {
    Vector<i32> numbers = {1, 2, 3, 4, 5};

    Vector<i32> odds;
    Query(numbers)
        .where([](i32 x) -> bool { return x % 2 != 0; })
        .for_each([&](i32 x) -> void { odds.push_back(x); });
    expect_eq(odds, Vector<i32>{1, 3, 5}, "for_each over odds");
}

void test_complex_chain() {
    Vector<i32> data = {9, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    // Deduplicate, sort ascending, skip smallest 2, take next 4, reverse
    Vector<i32> result = Query(data)
        .distinct()
        .order_by([](i32 x) -> i32 { return x; })
        .skip(2)
        .take(4)
        .reverse()
        .to<Vector>();
    expect_eq(result, Vector<i32>{6, 5, 4, 3}, "complex chain");

    // Count distinct evens
    usize n = Query(data)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .distinct()
        .count();
    expect_eq(n, 3uz, "distinct even count");
}

void test_split() {
    // split<Into>(delim) materialises each piece as Into; the bare split(delim)
    // mirrors std::views::split and yields the lazy subranges.

    // StringView -> Vector<String>
    StringView csv = "alpha,beta,gamma";
    Vector<String> parts = Query(csv)
        .split<String>(',')
        .to<Vector>();
    expect_eq(parts, Vector<String>{"alpha", "beta", "gamma"}, "split StringView");

    // String -> Vector<String>
    String sentence = "The quick brown fox jumps over the lazy dog";
    Vector<String> tokens = Query(sentence)
        .split<String>(' ')
        .to<Vector>();
    expect_eq(
        tokens,
        Vector<String>{"The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"},
        "split String"
    );

    // const char* -> wrap in StringView first
    const char* raw = "a/b/c/d";
    Vector<String> segments = Query(StringView(raw))
        .split<String>('/')
        .to<Vector>();
    expect_eq(segments, Vector<String>{"a", "b", "c", "d"}, "split const char*");

    // Materialise into non-owning StringViews over the original buffer instead.
    Vector<StringView> views = Query(csv)
        .split<StringView>(',')
        .to<Vector>();
    expect_eq(views, Vector<StringView>{"alpha", "beta", "gamma"}, "split into StringViews");

    // Multi-character C-string delimiter still has its trailing '\0' dropped.
    StringView log = "INFO :: load :: INFO :: ready";
    Vector<String> infos = Query(log)
        .split<String>(" :: ")
        .where([](const String& s) -> bool { return s != "INFO"; })
        .to<Vector>();
    expect_eq(infos, Vector<String>{"load", "ready"}, "split + filter");

    // Empty fields are preserved.
    Vector<String> empties = Query(StringView("x,,y"))
        .split<String>(',')
        .to<Vector>();
    expect_eq(empties, Vector<String>{"x", "", "y"}, "split keeps empty fields");

    // Generality: any collection, not just strings. Split a Vector<i32> on an
    // element value, materialising each piece into an owning Vector<i32>.
    Vector<i32> numbers = {1, 2, 0, 3, 4, 0, 0, 5};
    Vector<Vector<i32>> groups = Query(numbers)
        .split<Vector<i32>>(0)
        .to<Vector>();
    expect_eq(
        groups,
        Vector<Vector<i32>>{{1, 2}, {3, 4}, {}, {5}},
        "split Vector<i32> on 0"
    );

    // The bare default yields the lazy subranges - consume them without
    // materialising by summing each piece in place.
    Vector<i32> sums = Query(numbers)
        .split(0)
        .select([](auto&& piece) -> i32 {
            i32 total = 0;
            for (i32 x: piece) {
                total += x;
            }
            return total;
        })
        .to<Vector>();
    expect_eq(sums, Vector<i32>{3, 7, 0, 5}, "sum of each split piece");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Linq.basic_operations", test_basic_operations},
        {"Linq.chaining", test_chaining},
        {"Linq.aggregation", test_aggregation},
        {"Linq.terminal_operations", test_terminal_operations},
        {"Linq.sorting", test_sorting},
        {"Linq.error_cases", test_error_cases},
        {"Linq.with_strings", test_with_strings},
        {"Linq.skip_take_while", test_skip_take_while},
        {"Linq.min_max_sum", test_min_max_sum},
        {"Linq.concat", test_concat},
        {"Linq.select_many", test_select_many},
        {"Linq.keys_values", test_keys_values},
        #ifdef __cpp_lib_ranges_enumerate
        {"Linq.enumerate", test_enumerate},
        #endif
        {"Linq.zip", test_zip},
        {"Linq.for_each", test_for_each},
        {"Linq.complex_chain", test_complex_chain},
        {"Linq.split", test_split},
    });
}
