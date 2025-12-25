/**
 * @file glob.cppm
 * @module std:fs.glob
 * @brief Module file for standard library file system globbing operations.
 *
 * This file contains the implementation of the file system globbing operations in the standard library.
 * Adapted from p-ranav/glob (https://github.com/p-ranav/glob)
 */

module;

#include <filesystem>
#include <string>
#include <vector>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fs.glob;
#else
export module stdlib:fs.glob;
#endif

import :fmt.format;
import :fs.filesystem;

import core;
import alloc;

using alloc::collections::TreeMap;
using alloc::collections::Vector;
using alloc::String;
using alloc::text::Regex;

using core::ranges::views::Filter;
using core::ranges::views::Join;
using core::ranges::views::Transform;

using namespace core;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
namespace fmt = std::fmt;
#else
namespace fmt = stdlib::fmt;
#endif

/**
 * @namespace std::fs
 * @brief Wrapper namespace for standard library file system operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
namespace std::fs {
#else 
namespace stdlib::fs {
#endif

namespace {
    static bool string_replace(String& s, const String& from, const String& to) noexcept {
        usize start = s.find(from);
        if (start == String::npos) {
            return false;
        }
        s.replace(start, from.length(), to);
        return true;
    }

    [[nodiscard]]
    static String translate(const String& pattern) noexcept {
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
                                    contents += fmt::format("-{}", s);
                                }
                            }
                        }
                        String rep;
                        alloc::text::regex_replace(
                            core::iter::back_inserter(rep),
                            contents.begin(),
                            contents.end(),
                            Regex(R"([&~|])"),
                            R"(\\\1)"
                        );
                        contents = rep;
                        i = j + 1;
                        switch (contents[0]) {
                            case '!':
                                contents = fmt::format("^{}", String(contents.begin() + 1, contents.end()));
                                break;
                            case '^':
                            case '[':
                                contents = fmt::format("\\\\{}", contents);
                                break;
                            default:
                                // empty
                                break;
                        }
                        result = fmt::format("{}[{}]", result, contents);
                    }
                    break;
                default:
                    static constexpr StringView SPECIAL_CHARACTERS = "()[]{}?*+-|^$\\.&~# \t\n\r\v\f";
                    static TreeMap<int, String> special_characters_map;
                    if (special_characters_map.empty()) {
                        for (const char& sc: SPECIAL_CHARACTERS) {
                            special_characters_map.insert(
                                {static_cast<int>(sc), fmt::format("\\{}", String(1, sc))}
                            );
                        }
                    }

                    if (SPECIAL_CHARACTERS.find(ch) != StringView::npos) {
                        result += special_characters_map[static_cast<int>(ch)];
                    } else {
                        result += ch;
                    }
            }
        }
        return fmt::format("(({}{}", result, R"()|[\r\n])$)");
    }

    [[nodiscard]]
    static Regex compile_pattern(const String& pattern) {
        return Regex(translate(pattern), Regex::ECMAScript);
    }

    static bool filename_match(const Path& name, const String& pattern) {
        return alloc::text::regex_match(name.string(), compile_pattern(pattern));
    }

    [[nodiscard]]
    static Vector<Path> filter(const Vector<Path>& names, const String& pattern) noexcept {
        return names |
            Filter([&pattern](const Path& name) -> bool {
                return filename_match(name, pattern);
            }) |
            core::ranges::to<Vector<Path>>();
    }

    [[nodiscard]]
    static String get_env_var(StringView var) noexcept {
        const char* v = core::sys::getenv(var.data());
        return String(v ? v : "");
    }

    [[nodiscard]]
    static Optional<Path> expand_tilde(Path p) noexcept {
        if (p.empty()) {
            return p;
        }
        #ifdef _WIN32
        static constexpr StringView HOME_VARIABLE = "USERNAME";
        #else
        static constexpr StringView HOME_VARIABLE = "USER";
        #endif
        String home = get_env_var(HOME_VARIABLE);
        if (home.empty()) {
            return nullopt; // `~` could not be expanded due to HOME variable not being set
        }
        String s = p.string();
        if (s[0] == '~') {
            s = fmt::format("{}{}", home, s.substr(1, s.length() - 1));
            return Path(s);
        }
        return p;
    }

    [[nodiscard]]
    static bool has_magic(const String& p) {
        return alloc::text::regex_search(p, Regex("([*?[])"));
    }

    [[nodiscard]]
    static bool is_hidden(const String& p) {
        return alloc::text::regex_match(p, Regex("^(.*\\/)*\\.[^\\.\\/]+\\/*$"));
    }

    [[nodiscard]]
    static bool is_recursive(const String& pattern) noexcept {
        return pattern == "**";
    }

    [[nodiscard]]
    static Vector<Path> iterate_over_directory(const Path& dir, bool dironly) noexcept {
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
            } catch ([[maybe_unused]] const Exception& e) {
                // do nothing
            }
        }
        return result;
    }

    [[nodiscard]]
    static Vector<Path> listdir_recursive(const Path& dir, bool dironly) noexcept {
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
    static Vector<Path> glob_relative_pathnames(const Path& dir, [[maybe_unused]] const String& pattern, bool dironly) noexcept {
        (void)pattern;
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
    static Vector<Path> glob_over_pattern(const Path& dir, const String& pattern, bool dironly) noexcept {
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
    static Vector<Path> glob_over_base(const Path& dir, const Path& base, [[maybe_unused]] bool dironly) noexcept {
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

}

/**
 * @namespace std::fs
 * @brief Wrapper namespace for standard library file system operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::fs {
#else 
export namespace stdlib::fs {
#endif
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
        return paths |
            Transform([](const String& path) -> Vector<Path> {
                return glob_impl(path, false);
            }) | 
            Join |
            core::ranges::to<Vector<Path>>();
    }

    [[nodiscard]]
    Vector<Path> glob_recursive(const Vector<String>& paths) noexcept {
        return paths |
            Transform([](const String& path) -> Vector<Path> {
                return glob_impl(path, true);
            }) | 
            Join |
            core::ranges::to<Vector<Path>>();
    }

    [[nodiscard]]
    Vector<Path> glob(const InitializerList<String>& paths) noexcept {
        return glob(Vector<String>(paths));
    }

    [[nodiscard]]
    Vector<Path> glob_recursive(const InitializerList<String>& paths) noexcept {
        return glob_recursive(Vector<String>(paths));
    }
}