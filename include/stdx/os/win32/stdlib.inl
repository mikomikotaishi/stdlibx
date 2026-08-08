#pragma once

/**
 * @namespace stdx::os::win32
 * @brief C runtime process-environment access (<stdlib.h>).
 *
 * The MSVC CRT keeps its own environment block (what getenv reads); these are
 * the entry points that stay in sync with it, unlike the Win32 Environment
 * Strings APIs. Mirrors the unix stdlib.inl (setenv/unsetenv).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32)
    // Environment mutation (keep the CRT environment in sync)
    using ::_putenv_s;
    using ::_wputenv_s;

    /**
     * @brief The CRT environment block, as unix::environ exposes it.
     * @return The null-terminated "name=value" array, or nullptr if the CRT has no
     * environment.
     */
    [[nodiscard]]
    inline char** EnvironmentBlock() noexcept {
        return _environ;
    }

    /**
     * @brief The wide CRT environment block. See EnvironmentBlock.
     * @return The null-terminated "name=value" array, or nullptr if the CRT has no
     * environment.
     */
    [[nodiscard]]
    inline wchar_t** WideEnvironmentBlock() noexcept {
        return _wenviron;
    }

    /**
     * @brief The argument count the CRT computed for main.
     * @return The count, or 0 if the CRT startup never populated it.
     *
     * Read in a function body for the same reason as _environ above: __argc and
     * __wargv are macros over accessor calls, not entities. This is also the only
     * side of `import :os` where <stdlib.h> is visible at all.
     */
    [[nodiscard]]
    inline int ArgCount() noexcept {
        return __argc;
    }

    /**
     * @brief The wide argument vector the CRT computed for main.
     *
     * Preferred over GetCommandLineW plus CommandLineToArgvW because it is what
     * main's argv was itself built from, so it splits arguments identically; the
     * shell API applies its own quoting rules and disagrees at the edges.
     * @return The vector, or nullptr if the CRT startup never populated it.
     */
    [[nodiscard]]
    inline wchar_t** WideArgv() noexcept {
        return __wargv;
    }
    #endif
}
