import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::OutputFileStream;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

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

int main(int argc, char* argv[]) {
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
    });

    stdx::fs::remove_all(g_root);
    return result;
}
