#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::OutputFileStream;

using namespace stdx::test;

// A scratch directory tree, rebuilt fresh for the suite:
//
//   <tmp>/stdx_fs_walk_test/
//     alpha/
//       deep/
//       leaf.txt
//     beta/
//     top.txt
//
// walk() and glob() are both exercised against it. walk needs generator
// support; glob does not, so only the walk half is compiled conditionally.
//
// walk(root)            should yield all 5 descendants (pre-order).
// walk(root, dironly)   should yield only the 3 directories.
static Path g_root;

void touch(const Path& p) {
    OutputFileStream out(p);
    out << "x";
}

bool contains(const Vector<Path>& haystack, const Path& needle) {
    for (const Path& p: haystack) {
        if (p == needle) {
            return true;
        }
    }
    return false;
}

#ifdef __cpp_lib_generator
Vector<Path> collect(const Path& dir, bool dironly = false) {
    Vector<Path> out;
    for (const Path& p: stdx::fs::walk(dir, dironly)) {
        out.push_back(p);
    }
    return out;
}

void test_walk_yields_full_tree() {
    Vector<Path> entries = collect(g_root);

    expect_eq(entries.size(), 5uz, "walk yields every descendant once");
    expect(contains(entries, g_root / "alpha"), "yields the alpha directory");
    expect(contains(entries, g_root / "alpha" / "deep"), "descends into nested directories");
    expect(contains(entries, g_root / "alpha" / "leaf.txt"), "yields a nested file");
    expect(contains(entries, g_root / "beta"), "yields the beta directory");
    expect(contains(entries, g_root / "top.txt"), "yields a top-level file");
}

void test_walk_dironly_skips_files() {
    Vector<Path> dirs = collect(g_root, true);

    expect_eq(dirs.size(), 3uz, "dironly yields only directories");
    expect(contains(dirs, g_root / "alpha"), "dironly keeps alpha");
    expect(contains(dirs, g_root / "alpha" / "deep"), "dironly still recurses");
    expect(contains(dirs, g_root / "beta"), "dironly keeps beta");
    expect(!contains(dirs, g_root / "top.txt"), "dironly drops the top-level file");
    expect(!contains(dirs, g_root / "alpha" / "leaf.txt"), "dironly drops the nested file");
}

void test_walk_is_lazily_consumable() {
    usize seen = 0;
    for (const Path& _: stdx::fs::walk(g_root)) {
        ++seen;
        break;
    }
    expect_eq(seen, 1uz, "the generator can be consumed one element at a time");
}

void test_walk_missing_directory_is_empty() {
    usize seen = 0;
    for (const Path& _: stdx::fs::walk(g_root / "does_not_exist")) {
        ++seen;
    }
    expect_eq(seen, 0uz, "walking a nonexistent directory yields nothing");
}
#endif

void test_glob_literal_path_matches_existing_entry() {
    Vector<Path> hits = stdx::fs::glob(g_root / "top.txt");

    expect_eq(hits.size(), 1uz, "a magic-free path yields the entry it names");
    expect(contains(hits, g_root / "top.txt"), "the literal match is the file itself");
}

void test_glob_literal_path_missing_is_empty() {
    Vector<Path> hits = stdx::fs::glob(g_root / "nope.txt");

    expect_eq(hits.size(), 0uz, "a magic-free path that names nothing yields nothing");
}

void test_glob_star_matches_every_top_level_entry() {
    Vector<Path> hits = stdx::fs::glob(g_root / "*");

    expect_eq(hits.size(), 3uz, "* matches each top-level entry once");
    expect(contains(hits, g_root / "alpha"), "* matches the alpha directory");
    expect(contains(hits, g_root / "beta"), "* matches the beta directory");
    expect(contains(hits, g_root / "top.txt"), "* matches the top-level file");
    expect(!contains(hits, g_root / "alpha" / "leaf.txt"), "* does not descend into subdirectories");
}

void test_glob_star_with_suffix_filters_by_extension() {
    Vector<Path> hits = stdx::fs::glob(g_root / "*.txt");

    expect_eq(hits.size(), 1uz, "*.txt matches only the matching top-level file");
    expect(contains(hits, g_root / "top.txt"), "*.txt keeps top.txt");
    expect(!contains(hits, g_root / "alpha"), "*.txt drops directories without the suffix");
}

void test_glob_question_mark_matches_single_character() {
    Vector<Path> hits = stdx::fs::glob(g_root / "????");

    expect(contains(hits, g_root / "beta"), "? matches exactly one character each");
    expect(!contains(hits, g_root / "alpha"), "? does not match a longer name");
}

void test_glob_character_class_matches_listed_names() {
    Vector<Path> hits = stdx::fs::glob(g_root / "[ab]*");

    expect_eq(hits.size(), 2uz, "the class matches alpha and beta only");
    expect(contains(hits, g_root / "alpha"), "the class keeps alpha");
    expect(contains(hits, g_root / "beta"), "the class keeps beta");
    expect(!contains(hits, g_root / "top.txt"), "the class drops top.txt");
}

void test_glob_matches_within_a_subdirectory() {
    Vector<Path> hits = stdx::fs::glob(g_root / "alpha" / "*");

    expect_eq(hits.size(), 2uz, "a nested pattern lists the subdirectory's entries");
    expect(contains(hits, g_root / "alpha" / "deep"), "yields the nested directory");
    expect(contains(hits, g_root / "alpha" / "leaf.txt"), "yields the nested file");
}

void test_glob_magic_in_intermediate_component() {
    Vector<Path> hits = stdx::fs::glob(g_root / "*" / "leaf.txt");

    expect_eq(hits.size(), 1uz, "an intermediate wildcard expands before the final component");
    expect(contains(hits, g_root / "alpha" / "leaf.txt"), "resolves through the alpha directory");
}

void test_glob_dironly_keeps_only_directories() {
    Vector<Path> hits = stdx::fs::glob(g_root / "*", false, true);

    expect_eq(hits.size(), 2uz, "dironly keeps only the directories");
    expect(contains(hits, g_root / "alpha"), "dironly keeps alpha");
    expect(contains(hits, g_root / "beta"), "dironly keeps beta");
    expect(!contains(hits, g_root / "top.txt"), "dironly drops the top-level file");
}

void test_glob_no_match_is_empty() {
    Vector<Path> hits = stdx::fs::glob(g_root / "*.missing");

    expect_eq(hits.size(), 0uz, "a pattern that matches nothing yields nothing");
}

void test_glob_accepts_multiple_patterns() {
    Vector<Path> hits = stdx::fs::glob({g_root / "top.txt", g_root / "alpha"});

    expect_eq(hits.size(), 2uz, "the initializer-list overload globs each pattern");
    expect(contains(hits, g_root / "top.txt"), "keeps the first pattern's match");
    expect(contains(hits, g_root / "alpha"), "keeps the second pattern's match");
}

/**
 * @brief Tests that a Path formats the same way whichever library supplies it.
 *
 * libstdc++ has shipped the C++26 formatter since GCC 15; libc++ has not, so
 * fs.inl supplies one behind __cpp_lib_format_path. The two must be
 * indistinguishable, which is what this pins.
 *
 * The range case is the one that regressed: a formatter whose format() names a
 * single concrete context does not satisfy `formattable`, and a Vector<Path>
 * then finds no formatter at all rather than a plain-looking one. Formatting a
 * Path on its own keeps working, so nothing else notices.
 */
void test_path_formats_like_a_string() {
    const Path path = "/tmp/a b/c.txt";
    expect_eq(Ops::fmt("{}", path), "/tmp/a b/c.txt", "a path formats as its string");
    expect_eq(Ops::fmt("{:>20}", path), "      /tmp/a b/c.txt", "width and alignment apply");
    expect_eq(Ops::fmt("{:*^24}", path), "*****/tmp/a b/c.txt*****", "an explicit fill applies");
    expect_eq(Ops::fmt("{:>{}}", path, 20), "      /tmp/a b/c.txt", "a dynamic width applies");
    expect_eq(Ops::fmt("{:?}", path), "\"/tmp/a b/c.txt\"", "the debug format quotes");

    const Path awkward = "/tmp/he said \"hi\"/back\\slash";
    expect_eq(
        Ops::fmt("{:?}", awkward),
        "\"/tmp/he said \\\"hi\\\"/back\\\\slash\"",
        "the debug format escapes quotes and backslashes"
    );

    const Path wide = "/tmp/日本語/file.txt";
    expect_eq(
        Ops::fmt("{:*>30}", wide),
        "**********/tmp/日本語/file.txt",
        "an east-asian character counts as two columns"
    );

    const Vector<Path> paths{path, "x"};
    expect_eq(Ops::fmt("{}", paths), "[\"/tmp/a b/c.txt\", \"x\"]", "a Vector of paths formats");
}

int main(int argc, char* argv[]) {
    // The scratch tree feeds both the walk and glob tests, so build it whether
    // or not the standard library supports generators.
    g_root = stdx::fs::temp_directory_path() / "stdx_fs_walk_test";
    stdx::fs::remove_all(g_root);
    stdx::fs::create_directories(g_root / "alpha" / "deep");
    stdx::fs::create_directories(g_root / "beta");
    touch(g_root / "top.txt");
    touch(g_root / "alpha" / "leaf.txt");

    #ifndef __cpp_lib_generator
    System::out.println("[test] Directory walking disabled (standard library does not support generators).");
    #endif

    i32 result = run(argc, argv, {
        #ifdef __cpp_lib_generator
        {"FileSystem.yields_full_tree", test_walk_yields_full_tree},
        {"FileSystem.dironly_skips_files", test_walk_dironly_skips_files},
        {"FileSystem.lazily_consumable", test_walk_is_lazily_consumable},
        {"FileSystem.missing_directory_is_empty", test_walk_missing_directory_is_empty},
        #endif
        {"FileSystem.path_formats_like_a_string", test_path_formats_like_a_string},
        {"FileSystem.Glob.literal_path_matches", test_glob_literal_path_matches_existing_entry},
        {"FileSystem.Glob.literal_path_missing", test_glob_literal_path_missing_is_empty},
        {"FileSystem.Glob.star_matches_top_level", test_glob_star_matches_every_top_level_entry},
        {"FileSystem.Glob.star_suffix", test_glob_star_with_suffix_filters_by_extension},
        {"FileSystem.Glob.question_mark", test_glob_question_mark_matches_single_character},
        {"FileSystem.Glob.character_class", test_glob_character_class_matches_listed_names},
        {"FileSystem.Glob.within_subdirectory", test_glob_matches_within_a_subdirectory},
        {"FileSystem.Glob.intermediate_magic", test_glob_magic_in_intermediate_component},
        {"FileSystem.Glob.dironly", test_glob_dironly_keeps_only_directories},
        {"FileSystem.Glob.no_match", test_glob_no_match_is_empty},
        {"FileSystem.Glob.multiple_patterns", test_glob_accepts_multiple_patterns},
    });

    stdx::fs::remove_all(g_root);
    return result;
}
