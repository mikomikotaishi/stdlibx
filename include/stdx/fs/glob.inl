#pragma once

using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::linq::Query;
using stdx::ranges::views::Filter;
using stdx::text::regex::Regex;
using stdx::text::regex::SyntaxOption;

#ifdef __cpp_lib_generator
using stdx::ranges::Generator;
#endif

/**
 * @namespace stdx::fs
 * @brief Standard library file system operations.
 */
namespace stdx::fs {
    bool string_replace(String& s, const String& from, const String& to) noexcept {
        usize start = s.find(from);
        if (start == String::npos) {
            return false;
        }
        s.replace(start, from.length(), to);
        return true;
    }

    [[nodiscard]]
    String translate(const String& pattern) noexcept {
        usize i = 0;
        usize j = 0;
        usize n = pattern.length();
        String result;
        while (i < n) {
            char ch = pattern[i++];
            switch (ch) {
                case '*':
                    result += ".*";
                    break;
                case '?':
                    result += ".";
                    break;
                case '[':
                    j = i;
                    if (j < n && pattern[j] == '!') {
                        ++j;
                    }
                    if (j < n && pattern[j] == ']') {
                        ++j;
                    }
                    while (j < n && pattern[j] != ']') {
                        ++j;
                    }
                    if (j >= n) {
                        result += "\\[";
                    } else {
                        String contents = String(pattern.begin() + i, pattern.end() + j);
                        if (contents.find("--") == String::npos) {
                            string_replace(contents, "\\", R"("")");
                        } else {
                            Vector<String> chunks;
                            usize k = i + (pattern[i] == '!' ? 2 : 1);
                            while (true) {
                                k = pattern.find("-", k, j);
                                if (k == String::npos) {
                                    break;
                                }
                                chunks.emplace_back(pattern.begin() + i, pattern.begin() + k);
                                i = k + 1;
                                k += 3;
                            }
                            chunks.emplace_back(pattern.begin() + i, pattern.begin() + j);
                            bool first = false;
                            for (String& s: chunks) {
                                string_replace(s, "\\", R"(\\)");
                                string_replace(s, "-", R"(\-)");
                                if (first) {
                                    contents += s;
                                    first = false;
                                } else {
                                    contents += stdx::fmt::format("-{}", s);
                                }
                            }
                        }
                        String rep;
                        stdx::text::regex::regex_replace(
                            Iterators::back_inserter(rep),
                            contents.begin(),
                            contents.end(),
                            Regex(R"([&~|])"),
                            R"(\\\1)"
                        );
                        contents = rep;
                        i = j + 1;
                        switch (contents[0]) {
                            case '!':
                                contents = stdx::fmt::format("^{}", String(contents.begin() + 1, contents.end()));
                                break;
                            case '^':
                            case '[':
                                contents = stdx::fmt::format("\\\\{}", contents);
                                break;
                            default:
                                // empty
                                break;
                        }
                        result = stdx::fmt::format("{}[{}]", result, contents);
                    }
                    break;
                default:
                    static constexpr StringView SPECIAL_CHARACTERS = "()[]{}?*+-|^$\\.&~# \t\n\r\v\f";
                    static TreeMap<i32, String> special_characters_map;
                    if (special_characters_map.empty()) {
                        for (const char& sc: SPECIAL_CHARACTERS) {
                            special_characters_map.insert(
                                {static_cast<i32>(sc), stdx::fmt::format("\\{}", String(1, sc))}
                            );
                        }
                    }

                    if (SPECIAL_CHARACTERS.find(ch) != StringView::npos) {
                        result += special_characters_map[static_cast<i32>(ch)];
                    } else {
                        result += ch;
                    }
            }
        }
        return stdx::fmt::format("(({}{}", result, R"()|[\r\n])$)");
    }

    [[nodiscard]]
    Regex compile_pattern(const String& pattern) {
        return Regex(translate(pattern), SyntaxOption::ECMASCRIPT);
    }

    bool filename_match(const Path& name, const String& pattern) {
        return stdx::text::regex::regex_match(name.string(), compile_pattern(pattern));
    }

    [[nodiscard]]
    Vector<Path> filter(const Vector<Path>& names, const String& pattern) noexcept {
        return Query(names)
            .where([pattern](const Path& name) {
                return filename_match(name, pattern);
            })
            .to<Vector<Path>>();
    }

    [[nodiscard]]
    Optional<Path> expand_tilde(Path p) noexcept {
        if (p.empty()) {
            return p;
        }
        #ifdef _WIN32
        static constexpr StringView HOME_VARIABLE = "USERNAME";
        #else
        static constexpr StringView HOME_VARIABLE = "USER";
        #endif
        Optional<StringView> home = Environment::get(HOME_VARIABLE);
        if (!home.has_value()) {
            return nullopt; // `~` could not be expanded due to HOME variable not being set
        }
        String s = p.string();
        if (s[0] == '~') {
            s = stdx::fmt::format("{}{}", home.value(), s.substr(1, s.length() - 1));
            return Path(s);
        }
        return p;
    }

    [[nodiscard]]
    bool has_magic(const String& p) {
        return stdx::text::regex::regex_search(p, Regex("([*?[])"));
    }

    [[nodiscard]]
    bool is_hidden(const String& p) {
        return stdx::text::regex::regex_match(p, Regex("^(.*\\/)*\\.[^\\.\\/]+\\/*$"));
    }

    [[nodiscard]]
    bool is_recursive(const String& pattern) noexcept {
        return pattern == "**";
    }

    [[nodiscard]]
    Vector<Path> iterate_over_directory(const Path& dir, bool dironly) noexcept {
        Vector<Path> result;
        Path current = dir;
        if (current.empty()) {
            current = current_path();
        }
        if (exists(current)) {
            try {
                for (const DirectoryEntry& entry: DirectoryIterator(
                    current,
                    DirectoryOptions::FOLLOW_DIRECTORY_SYMLINK | DirectoryOptions::SKIP_PERMISSION_DENIED
                )) {
                    if (!dironly || entry.is_directory()) {
                        result.emplace_back(dir.is_absolute() ? entry.path() : relative(entry.path()));
                    }
                }
            } catch (const Exception& _) {
                // do nothing
            }
        }
        return result;
    }

    [[nodiscard]]
    Vector<Path> listdir_recursive(const Path& dir, bool dironly) noexcept {
        Vector<Path> result;
        Vector<Path> names = iterate_over_directory(dir, dironly);
        for (Path& p: names) {
            if (!is_hidden(p.string())) {
                result.emplace_back(p);
                for (Path& q: listdir_recursive(p, dironly)) {
                    result.emplace_back(q);
                }
            }
        }
        return result;
    }

    [[nodiscard]]
    Vector<Path> glob_relative_pathnames(const Path& dir, [[maybe_unused]] const String& pattern, bool dironly) noexcept {
        Vector<Path> result;
        if (exists(dir)) {
            result.emplace_back(".");
        }
        for (Path& d: listdir_recursive(dir, dironly)) {
            result.emplace_back(d);
        }
        return result;
    }

    [[nodiscard]]
    Vector<Path> glob_over_pattern(const Path& dir, const String& pattern, bool dironly) noexcept {
        Vector<Path> names = iterate_over_directory(dir, dironly);
        Vector<Path> filtered;
        for (Path& name: names) {
            if (!is_hidden(name.string())) {
                filtered.emplace_back(name.filename());
            }
        }
        return filter(filtered, pattern);
    }

    [[nodiscard]]
    Vector<Path> glob_over_base(const Path& dir, const Path& base, [[maybe_unused]] bool dironly) noexcept {
        Vector<Path> result;
        if (base.empty()) {
            if (is_directory(dir)) {
                result = {base};
            }
        } else {
            if (exists(dir / base)) {
                result = {base};
            }
        }
        return result;
    }

    [[nodiscard]]
    Vector<Path> glob_impl(const String& pathname, bool recursive = false, bool dironly = false) noexcept {
        Vector<Path> result;
        Path path(pathname);
        if (pathname[0] == '~') {
            path = *expand_tilde(path);
        }
        Path dir = path.parent_path();
        Path base = path.filename();
        if (!has_magic(pathname)) {
            if (!base.empty()) {
                if (exists(path)) {
                    result.emplace_back(path);
                }
            } else {
                if (is_directory(dir)) {
                    result.emplace_back(path);
                }
            }
            return result;
        }
        if (dir.empty()) {
            if (recursive && is_recursive(base.string())) {
                return glob_relative_pathnames(dir, base.string(), dironly);
            } else {
                return glob_over_pattern(dir, base.string(), dironly);
            }
        }

        Vector<Path> dirs;
        if (dir != Path(pathname) && has_magic(dir.string())) {
            dirs = glob_impl(dir.string(), recursive, true);
        } else {
            dirs = {dir};
        }

        Function<Vector<Path>(const Path&, const String&, bool)> glob_in_dir;
        if (has_magic(base.string())) {
            if (recursive && is_recursive(base.string())) {
                glob_in_dir = glob_relative_pathnames;
            } else {
                glob_in_dir = glob_over_pattern;
            }
        } else {
            glob_in_dir = glob_over_base;
        }

        for (Path& d: dirs) {
            for (Path& name: glob_in_dir(d, base.string(), dironly)) {
                Path subresult = name;
                if (name.parent_path().empty()) {
                    subresult = d / name;
                }
                result.emplace_back(subresult.lexically_normal());
            }
        }
        return result;
    }

}

/**
 * @namespace stdx::fs
 * @brief Standard library file system operations.
 */
export namespace stdx::fs {
    [[nodiscard]]
    Vector<Path> glob(const String& path) noexcept {
        return glob_impl(path, false);
    }

    [[nodiscard]]
    Vector<Path> glob_recursive(const String& path) noexcept {
        return glob_impl(path, true);
    }

    [[nodiscard]]
    Vector<Path> glob(const Vector<String>& paths) noexcept {
        return Query(paths)
            .select_many([](const String& path) -> Vector<Path> {
                return glob_impl(path, false);
            })
            .to<Vector<Path>>();
    }

    [[nodiscard]]
    Vector<Path> glob_recursive(const Vector<String>& paths) noexcept {
        return Query(paths)
            .select_many([](const String& path) -> Vector<Path> {
                return glob_impl(path, true);
            })
            .to<Vector<Path>>();
    }

    [[nodiscard]]
    Vector<Path> glob(const InitializerList<String>& paths) noexcept {
        return glob(Vector<String>(paths));
    }

    [[nodiscard]]
    Vector<Path> glob_recursive(const InitializerList<String>& paths) noexcept {
        return glob_recursive(Vector<String>(paths));
    }

    #ifdef __cpp_lib_generator
    /**
     * @brief Lazily walks a directory tree, yielding each entry in turn.
     *
     * The recursive, lazy twin of listdir_recursive: where the eager listing
     * materialises the entire subtree up front, this yields one entry at a
     * time and descends only as the caller consumes, so a partial walk (or an
     * early break) never enumerates the rest of the tree. Hidden entries are
     * skipped, mirroring the glob helpers.
     *
     * @param dir The root directory to walk.
     * @param dironly When true, yield only directories.
     * @return A Generator yielding each descendant Path in pre-order.
     *
     * @note Synchronous: each step performs blocking filesystem reads. This is
     *       lazy iteration, not asynchronous I/O. Only the current directory
     *       level is held in memory at a time, never the whole subtree.
     */
    [[nodiscard]]
    Generator<Path> walk(const Path& dir, bool dironly = false) {
        for (const Path& entry: iterate_over_directory(dir, dironly)) {
            if (!is_hidden(entry.string())) {
                co_yield entry;
                for (const Path& child: walk(entry, dironly)) {
                    co_yield child;
                }
            }
        }
    }
    #endif
}
