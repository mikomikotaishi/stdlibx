#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::OutputFileStream;

using namespace stdx::test;

#ifdef __cpp_lib_generator
// A scratch directory tree, rebuilt fresh for the suite:
//
//   <tmp>/stdx_fs_walk_test/
//     alpha/
//       deep/
//       leaf.txt
//     beta/
//     top.txt
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
    // Taking a single element must not require enumerating the whole tree:
    // partial consumption and early break leave the coroutine in a clean state.
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
    #ifdef __cpp_lib_generator
    g_root = stdx::fs::temp_directory_path() / "stdx_fs_walk_test";
    stdx::fs::remove_all(g_root);
    stdx::fs::create_directories(g_root / "alpha" / "deep");
    stdx::fs::create_directories(g_root / "beta");
    touch(g_root / "top.txt");
    touch(g_root / "alpha" / "leaf.txt");

    i32 result = run(argc, argv, {
        {"FsWalk.yields_full_tree", test_walk_yields_full_tree},
        {"FsWalk.dironly_skips_files", test_walk_dironly_skips_files},
        {"FsWalk.lazily_consumable", test_walk_is_lazily_consumable},
        {"FsWalk.missing_directory_is_empty", test_walk_missing_directory_is_empty},
        {"FsWalk.path_formats_like_a_string", test_path_formats_like_a_string},
    });

    stdx::fs::remove_all(g_root);
    return result;
    #else
    // The walk itself needs std::generator, which libc++ does not have; the
    // path formatter does not, and is exactly what needs covering there.
    System::out.println("[test] Directory walking disabled (standard library does not support generators).");
    return run(argc, argv, {
        {"Fs.path_formats_like_a_string", test_path_formats_like_a_string},
    });
    #endif
}
