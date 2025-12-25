/**
 * @file glob.constants.cppm
 * @module stdx:os.unix.glob.constants
 * @brief Module file for Unix globbing operations.
 *
 * This file contains the implementation of the constants used 
 * in <glob.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.glob.constants;
#else
export module stdlibx:os.unix.glob.constants;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    inline constexpr int GLOB_ERR = (1 << 0); ///< Return on read errors.
    inline constexpr int GLOB_MARK = (1 << 1); ///< Append a slash to each name.
    inline constexpr int GLOB_NOSORT = (1 << 2); ///< Don't sort the names.
    inline constexpr int GLOB_DOOFFS = (1 << 3); ///< Insert PGLOB->gl_offs NULLs.
    inline constexpr int GLOB_NOCHECK = (1 << 4); ///< If nothing matches, return the pattern.
    inline constexpr int GLOB_APPEND = (1 << 5); ///< Append to results of a previous call.
    inline constexpr int GLOB_NOESCAPE = (1 << 6); ///< Backslashes don't quote metacharacters.
    inline constexpr int GLOB_PERIOD = (1 << 7); ///< Leading `.` can be matched by metachars.

    inline constexpr int GLOB_MAGCHAR = (1 << 8); ///< Set in gl_flags if any metachars seen.
    inline constexpr int GLOB_ALTDIRFUNC = (1 << 9); ///< Use gl_opendir et al functions.
    inline constexpr int GLOB_BRACE = (1 << 10); ///< Expand "{a,b}" to "a" "b".
    inline constexpr int GLOB_NOMAGIC = (1 << 11); ///< If no magic chars, return the pattern.
    inline constexpr int GLOB_TILDE = (1 << 12); ///< Expand ~user and ~ to home directories.
    inline constexpr int GLOB_ONLYDIR = (1 << 13); ///< Match only directories.
    inline constexpr int GLOB_TILDE_CHECK = (1 << 14); ///< Like GLOB_TILDE but return an error if user name is unavailable.

    inline constexpr int __GLOB_FLAGS =
        GLOB_ERR | GLOB_MARK | GLOB_NOSORT | GLOB_DOOFFS |
        GLOB_NOESCAPE | GLOB_NOCHECK | GLOB_APPEND | GLOB_PERIOD |
        GLOB_ALTDIRFUNC | GLOB_BRACE | GLOB_NOMAGIC |
        GLOB_TILDE | GLOB_ONLYDIR | GLOB_TILDE_CHECK;

    /// Error returns from glob
    inline constexpr int GLOB_NOSPACE = 1; ///< Ran out of memory.
    inline constexpr int GLOB_ABORTED = 2; ///< Read error.
    inline constexpr int GLOB_NOMATCH = 3; ///< No matches found.
    inline constexpr int GLOB_NOSYS = 4; ///< Not implemented.
}
