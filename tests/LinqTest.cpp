import stdx;

using stdx::collections::Vector;
using stdx::fs::DirectoryEntry;
using stdx::fs::DirectoryIterator;
using stdx::fs::Path;
using stdx::linq::Query;
using stdx::mem::SharedPointer;
using stdx::mem::UniquePointer;

using namespace stdx::core;

void test_basic_operations() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Test where (filter) - using template template parameter syntax
    Vector<i32> evens = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .to<Vector>();
    stdx::io::print("Even numbers: ");
    for (i32 x : evens) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
    
    // Test select (map/transform) - using explicit type syntax
    Vector<i32> squares = Query(numbers)
        .select([](i32 x) -> i32 { return x * x; })
        .to<Vector<i32>>();
    stdx::io::print("Squares: ");
    for (i32 x : squares) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
    
    // Test skip
    Vector<i32> skipped = Query(numbers)
        .skip(5)
        .to<Vector>();
    stdx::io::print("Skip 5: ");
    for (i32 x : skipped) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
    
    // Test take
    Vector<i32> taken = Query(numbers)
        .take(3)
        .to<Vector>();
    stdx::io::print("Take 3: ");
    for (i32 x : taken) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
}

void test_chaining() {
    Vector<i32> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Complex chain: filter evens, square them, take first 3
    Vector<i32> result = Query(numbers)
        .where([](i32 x) -> bool { return x % 2 == 0; })
        .select([](i32 x) -> i32 { return x * x; })
        .take(3)
        .to<Vector>();
    
    stdx::io::print("Even numbers squared, first 3: ");
    for (i32 x : result) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
}

void test_aggregation() {
    Vector<i32> numbers = {1, 2, 3, 4, 5};
    auto query = Query(numbers);
    
    // Test any (no predicate)
    bool hasElements = query.any();
    stdx::io::println("Has elements: {}", hasElements);
    
    // Test any (with predicate)
    bool hasEven = Query(numbers).any([](i32 x) -> bool { return x % 2 == 0; });
    stdx::io::println("Has even number: {}", hasEven);
    
    // Test all
    bool allPositive = Query(numbers).all([](i32 x) -> bool { return x > 0; });
    stdx::io::println("All positive: {}", allPositive);
    
    bool allEven = Query(numbers).all([](i32 x) -> bool { return x % 2 == 0; });
    stdx::io::println("All even: {}", allEven);
    
    // Test count (no predicate)
    usize total = Query(numbers).count();
    stdx::io::println("Total count: {}", total);
    
    // Test count (with predicate)
    usize evenCount = Query(numbers).count([](i32 x) -> bool { return x % 2 == 0; });
    stdx::io::println("Even count: {}", evenCount);
}

void test_terminal_operations() {
    Vector<i32> numbers = {5, 2, 8, 1, 9};
    
    // Test first
    i32 first = Query(numbers).first();
    stdx::io::println("First: {}", first);
    
    // Test first_or
    Vector<i32> empty;
    i32 firstOrDefault = Query(empty).first_or(42);
    stdx::io::println("First from empty (default 42): {}", firstOrDefault);
    
    // Test single (should work with one-element range)
    Vector<i32> single_vec = {99};
    i32 single = Query(single_vec).single();
    stdx::io::println("Single element: {}", single);
    
    // Test to() conversion
    Vector<i32> vec = Query(numbers).to<Vector>();
    stdx::io::println("Converted to vector, size: {}", vec.size());
}

void test_sorting() {
    Vector<i32> numbers = {5, 2, 8, 1, 9, 2, 5, 3};
    
    // Test order_by
    Vector<i32> sorted = Query(numbers)
        .order_by([](i32 x) -> i32 { return x; })
        .to<Vector>();
    stdx::io::print("Sorted: ");
    for (i32 x : sorted) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
    
    // Test distinct
    Vector<i32> unique = Query(numbers)
        .distinct()
        .to<Vector>();
    stdx::io::print("Distinct: ");
    for (i32 x : unique) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
    
    // Test reverse
    Vector<i32> reversed = Query(numbers)
        .reverse()
        .to<Vector>();
    stdx::io::print("Reversed: ");
    for (i32 x : reversed) {
        stdx::io::print("{} ", x);
    }
    stdx::io::println("");
}

void test_error_cases() {
    Vector<i32> empty;
    
    // Test first on empty (should throw)
    try {
        auto _ = Query(empty).first();
        stdx::io::println("ERROR: first() should have thrown!");
    } catch (const InvalidOperationException& e) {
        stdx::io::println("✓ first() on empty threw: {}", e.what());
    }
    
    // Test single on empty (should throw)
    try {
        auto _ = Query(empty).single();
        stdx::io::println("ERROR: single() should have thrown!");
    } catch (const InvalidOperationException& e) {
        stdx::io::println("✓ single() on empty threw: {}", e.what());
    }
    
    // Test single on multiple elements (should throw)
    Vector<i32> multiple = {1, 2};
    try {
        auto _ = Query(multiple).single();
        stdx::io::println("ERROR: single() on multiple elements should have thrown!");
    } catch (const InvalidOperationException& e) {
        stdx::io::println("✓ single() on multiple elements threw: {}", e.what());
    }
}

void test_with_strings() {
    Vector<String> words = {"apple", "banana", "apricot", "blueberry", "avocado"};
    
    // Find all words starting with 'a'
    Vector<String> aWords = Query(words)
        .where([](const String& s) -> bool { return s[0] == 'a'; })
        .to<Vector<String>>();
    
    stdx::io::print("Words starting with 'a': ");
    for (const String& word : aWords) {
        stdx::io::print("{} ", word);
    }
    stdx::io::println("");
    
    // Get lengths of words
    Vector<usize> lengths = Query(words)
        .select([](const String& s) -> usize { return s.size(); })
        .to<Vector<usize>>();
    
    stdx::io::print("Word lengths: ");
    for (usize len : lengths) {
        stdx::io::print("{} ", len);
    }
    stdx::io::println("");
}

void test_file_system() {
    Path dir = "./userdata";
    Vector<DirectoryEntry> files;
    for (const DirectoryEntry& entry: DirectoryIterator(dir)) {
        if (stdx::fs::is_regular_file(entry)) {
            files.push_back(entry);
        }
    }

    // Find the largest .txt file
    DirectoryEntry result = Query(files)
        .where([](const DirectoryEntry& entry) { return entry.path().extension() == ".txt"; })
        .order_by([](const DirectoryEntry& entry) -> i64 { return -static_cast<i64>(stdx::fs::file_size(entry)); })
        .first_or(DirectoryEntry());

    if (result.path().empty()) {
        stdx::io::println("No .txt files found in directory: {}", dir.string());
    } else {
        stdx::io::println("Largest .txt file: {} ({} bytes)", result.path().string(), stdx::fs::file_size(result.path()));
    }
    
    // Count all files
    usize fileCount = Query(files).count();
    stdx::io::println("Total files: {}", fileCount);
}

int main() {
    
    test_basic_operations();
    test_chaining();
    test_aggregation();
    test_terminal_operations();
    test_sorting();
    test_error_cases();
    test_with_strings();
    test_file_system();
    
    return 0;
}