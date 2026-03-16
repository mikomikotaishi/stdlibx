import stdx;

using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::linq::Query;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_basic_operations() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Test where (filter) - using template template parameter syntax
    Vector<i32> evens = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .to<Vector>();
    System::out.println("Even numbers: {}", evens);

    // Test select (map/transform) - using explicit type syntax
    Vector<i32> squares = Query(numbers)
        .select([](i32 x) -> i32 { return x * x; })
        .to<Vector<i32>>();
    System::out.println("Squares: {}", squares);

    // Test select (map/transform) - using explicit type syntax and from() method
    Vector<i32> incremented = Query<Vector<i32>>::from(numbers)
        .select([](i32 x) -> i32 { return x + 1; })
        .to<Vector>();
    System::out.println("Incremented: {}", incremented);

    Vector<String> to_strings = Query<>::from(numbers)
        .select([](i32 x) -> String { return Integer::to_string(x); })
        .to<Vector>();
    System::out.println("To strings: {}", to_strings);

    // Test skip
    Vector<i32> skipped = Query(numbers)
        .skip(5)
        .to<Vector>();
    System::out.println("Skip 5: {}", skipped);

    // Test take
    Vector<i32> taken = Query(numbers)
        .take(3)
        .to<Vector>();
    System::out.println("Take 3: {}", taken);
}

void test_chaining() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Complex chain: filter evens, square them, take first 3
    Vector<i32> result = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .select([](i32 x) -> i32 { return x * x; })
        .take(3)
        .to<Vector>();

    System::out.println("Even numbers squared, first 3: {}", result);
}

void test_aggregation() {
    Vector<i32> numbers = {1, 2, 3, 4, 5};
    Query query(numbers);

    // Test any (no predicate)
    bool has_elements = query.any();
    System::out.println("Has elements: {}", has_elements);

    // Test any (with predicate)
    bool has_even = Query(numbers).any([](i32 x) -> bool { return x % 2 == 0; });
    System::out.println("Has even number: {}", has_even);

    // Test all
    bool all_positive = Query(numbers).all([](i32 x) -> bool { return x > 0; });
    System::out.println("All positive: {}", all_positive);

    bool all_even = Query(numbers).all([](i32 x) -> bool { return x % 2 == 0; });
    System::out.println("All even: {}", all_even);

    // Test none
    bool none_negative = Query(numbers).none([](i32 x) -> bool { return x < 0; });
    System::out.println("None negative: {}", none_negative);

    bool none_even = Query(numbers).none([](i32 x) -> bool { return x % 2 == 0; });
    System::out.println("None even: {}", none_even);

    // Test count (no predicate)
    usize total = Query(numbers).count();
    System::out.println("Total count: {}", total);

    // Test count (with predicate)
    usize even_count = Query(numbers).count([](i32 x) -> bool { return x % 2 == 0; });
    System::out.println("Even count: {}", even_count);

    // Test contains
    bool has_3 = Query(numbers).contains(3);
    System::out.println("Contains 3: {}", has_3);

    bool has_99 = Query(numbers).contains(99);
    System::out.println("Contains 99: {}", has_99);
}

void test_terminal_operations() {
    Vector<i32> numbers = {5, 2, 8, 1, 9};

    // Test first
    i32 first = Query(numbers).first();
    System::out.println("First: {}", first);

    // Test first_or
    Vector<i32> empty;
    i32 first_or_default = Query(empty).first_or(42);
    System::out.println("First from empty (default 42): {}", first_or_default);

    // Test last
    i32 last = Query(numbers).last();
    System::out.println("Last: {}", last);

    // Test last_or
    i32 last_or_default = Query(empty).last_or(-1);
    System::out.println("Last from empty (default -1): {}", last_or_default);

    // Test element_at
    i32 third = Query(numbers).element_at(2);
    System::out.println("Element at index 2: {}", third);

    // Test element_at_or
    i32 out_of_range = Query(numbers).element_at_or(100, -1);
    System::out.println("Element at index 100 (default -1): {}", out_of_range);

    // Test single (should work with one-element range)
    Vector<i32> single_vec = {99};
    i32 single = Query(single_vec).single();
    System::out.println("Single element: {}", single);

    // Test single with predicate (throwing overload)
    i32 single_match = Query(numbers).single([](i32 x) -> bool { return x == 8; });
    System::out.println("Single element == 8: {}", single_match);

    // Test single with predicate and default
    i32 single_or_default = Query(numbers).single([](i32 x) -> bool { return x > 100; }, -1);
    System::out.println("Single > 100 (default -1): {}", single_or_default);

    // Test to() conversion
    Vector<i32> vec = Query(numbers).to<Vector>();
    System::out.println("Converted to vector, size: {}", vec.size());
}

void test_sorting() {
    Vector<i32> numbers = {5, 2, 8, 1, 9, 2, 5, 3};

    // Test order_by
    Vector<i32> sorted = Query(numbers)
        .order_by([](i32 x) -> i32 { return x; })
        .to<Vector>();
    System::out.println("Sorted: {}", sorted);

    // Test order_by_descending
    Vector<i32> sorted_desc = Query(numbers)
        .order_by_descending([](i32 x) -> i32 { return x; })
        .to<Vector>();
    System::out.println("Sorted descending: {}", sorted_desc);

    // Test distinct
    Vector<i32> unique = Query(numbers)
        .distinct()
        .to<Vector>();
    System::out.println("Distinct: {}", unique);

    // Test reverse
    Vector<i32> reversed = Query(numbers)
        .reverse()
        .to<Vector>();
    System::out.println("Reversed: {}", reversed);
}

void test_error_cases() {
    Vector<i32> empty;

    // Test first on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).first();
        System::err.println("ERROR: first() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("first() on empty threw: {}", e.what());
    }

    // Test last on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).last();
        System::err.println("ERROR: last() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("last() on empty threw: {}", e.what());
    }

    // Test element_at on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).element_at(0);
        System::err.println("ERROR: element_at() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("element_at() on empty threw: {}", e.what());
    }

    // Test single on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).single();
        System::err.println("ERROR: single() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("single() on empty threw: {}", e.what());
    }

    // Test single on multiple elements (should throw)
    Vector<i32> multiple = {1, 2};
    try {
        [[maybe_unused]] i32 _ = Query(multiple).single();
        System::err.println("ERROR: single() on multiple elements should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("single() on multiple elements threw: {}", e.what());
    }

    // Test min on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).min();
        System::err.println("ERROR: min() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("min() on empty threw: {}", e.what());
    }

    // Test max on empty (should throw)
    try {
        [[maybe_unused]] i32 _ = Query(empty).max();
        System::err.println("ERROR: max() should have thrown!");
    } catch (const InvalidOperationException& e) {
        System::out.println("max() on empty threw: {}", e.what());
    }
}

void test_with_strings() {
    Vector<String> words = {"apple", "banana", "apricot", "blueberry", "avocado"};

    // Find all words starting with 'a'
    Vector<String> words_of_a = Query(words)
        .where([](const String& s) -> bool { return s[0] == 'a'; })
        .to<Vector<String>>();

    System::out.println("Words starting with 'a': {}", words_of_a);

    // Get lengths of words
    Vector<usize> lengths = Query(words)
        .select([](const String& s) -> usize { return s.size(); })
        .to<Vector<usize>>();

    System::out.println("Word lengths: {}", lengths);

    // Test contains with strings
    bool has_banana = Query(words).contains(String("banana"));
    System::out.println("Contains 'banana': {}", has_banana);
}

void test_skip_take_while() {
    Vector<i32> numbers = {1, 2, 3, 10, 20, 30, 4, 5};

    // Test skip_while
    Vector<i32> skipped = Query(numbers)
        .skip_while([](i32 x) -> bool { return x < 10; })
        .to<Vector>();
    System::out.println("Skip while < 10: {}", skipped);

    // Test take_while
    Vector<i32> taken = Query(numbers)
        .take_while([](i32 x) -> bool { return x < 10; })
        .to<Vector>();
    System::out.println("Take while < 10: {}", taken);
}

void test_min_max_sum() {
    Vector<i32> numbers = {5, 2, 8, 1, 9};

    // Test min
    i32 min_val = Query(numbers).min();
    System::out.println("Min: {}", min_val);

    // Test max
    i32 max_val = Query(numbers).max();
    System::out.println("Max: {}", max_val);

    #ifdef __cpp_lib_ranges_fold
    // Test sum
    i32 total = Query(numbers).sum();
    System::out.println("Sum: {}", total);

    // Test aggregate (manual product)
    i32 product = Query(numbers).aggregate(1, [](i32 acc, i32 x) -> i32 { return acc * x; });
    System::out.println("Product (aggregate): {}", product);

    // Test sum of filtered
    i32 even_sum = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .sum();
    System::out.println("Sum of evens: {}", even_sum);
    #endif
}

void test_concat() {
    Vector<i32> a = {1, 2, 3};
    Vector<i32> b = {4, 5, 6};

    Vector<i32> combined = Query(a)
        .concat(b)
        .to<Vector>();
    System::out.println("Concat: {}", combined);
}

void test_select_many() {
    Vector<Vector<i32>> nested = {{1, 2}, {3, 4}, {5, 6}};

    // Flatten nested vectors via select_many (identity)
    Vector<i32> flat = Query(nested)
        .select_many([](const Vector<i32>& v) -> const Vector<i32>& { return v; })
        .to<Vector>();
    System::out.println("SelectMany (flatten): {}", flat);

    // Flatten via flatten()
    Vector<i32> flat_2 = Query(nested)
        .flatten()
        .to<Vector>();
    System::out.println("Flatten: {}", flat_2);
}

void test_keys_values() {
    Vector<Pair<String, i32>> entries = {{"Alice", 90}, {"Bob", 85}, {"Carol", 92}};

    // Test keys
    Vector<String> names = Query(entries)
        .keys()
        .to<Vector<String>>();
    System::out.println("Keys: {}", names);

    // Test values
    Vector<i32> scores = Query(entries)
        .values()
        .to<Vector<i32>>();
    System::out.println("Values: {}", scores);

    // Filter by value, then take keys
    Vector<String> top_students = Query(entries)
        .where([](const Pair<String, i32>& p) -> bool { return p.second >= 90; })
        .keys()
        .to<Vector<String>>();
    System::out.println("Students with score >= 90: {}", top_students);

    // Test keys/values on TreeMap
    TreeMap<String, i32> tree_scores = {{"alice", 90}, {"bob", 85}, {"carol", 92}};

    Vector<String> tree_keys = Query(tree_scores)
        .keys()
        .to<Vector<String>>();
    System::out.println("TreeMap keys: {}", tree_keys);

    Vector<i32> tree_values = Query(tree_scores)
        .values()
        .to<Vector<i32>>();
    System::out.println("TreeMap values: {}", tree_values);

    // Filter TreeMap by value, then take keys
    Vector<String> tree_top = Query(tree_scores)
        .where([](const Pair<const String, i32>& p) -> bool { return p.second >= 90; })
        .keys()
        .to<Vector<String>>();
    System::out.println("TreeMap students with score >= 90: {}", tree_top);

    // Test keys/values on HashMap
    HashMap<String, i32> hash_scores = {{"alice", 90}, {"bob", 85}, {"carol", 92}};

    Vector<String> hash_keys = Query(hash_scores)
        .keys()
        .to<Vector<String>>();
    System::out.println("HashMap keys: {}", hash_keys);

    Vector<i32> hash_values = Query(hash_scores)
        .values()
        .to<Vector<i32>>();
    System::out.println("HashMap values: {}", hash_values);

    #ifdef __cpp_lib_ranges_fold
    // Sum HashMap values
    i32 hash_total = Query(hash_scores)
        .values()
        .sum();
    System::out.println("HashMap value sum: {}", hash_total);
    #endif
}

#ifdef __cpp_lib_ranges_enumerate
void test_enumerate() {
    Vector<String> words = {"foo", "bar", "baz"};

    // Enumerate pairs each element with its index
    System::out.println("Enumerated:");
    Query(words)
        .enumerate()
        .for_each([](auto pair) {
            auto [i, w] = pair;
            System::out.println("  [{}] = {}", i, w);
        });
}
#endif

void test_zip() {
    Vector<String> names = {"Alice", "Bob", "Carol"};
    Vector<i32> scores = {90, 85, 92};

    // Zip names and scores, then format
    Vector<String> formatted = Query(names)
        .zip(scores)
        .select([](auto pair) -> String {
            auto [name, score] = pair;
            return stdx::fmt::format("{}: {}", name, score);
        })
        .to<Vector<String>>();
    System::out.println("Zipped: {}", formatted);
}

void test_for_each() {
    Vector<i32> numbers = {1, 2, 3, 4, 5};

    System::out.print("ForEach: ");
    Query(numbers)
        .where([](i32 x) -> bool { return x % 2 != 0; })
        .for_each([](i32 x) { System::out.print("{} ", x); });
    System::out.println("");
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
    System::out.println("Complex chain: {}", result);

    // Count distinct evens
    usize n = Query(data)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .distinct()
        .count();
    System::out.println("Distinct even count: {}", n);
}

int main() {
    test_basic_operations();
    test_chaining();
    test_aggregation();
    test_terminal_operations();
    test_sorting();
    test_error_cases();
    test_with_strings();
    test_skip_take_while();
    test_min_max_sum();
    test_concat();
    test_select_many();
    test_keys_values();
    #ifdef __cpp_lib_ranges_enumerate
    test_enumerate();
    #endif
    test_zip();
    test_for_each();
    test_complex_chain();

    return 0;
}