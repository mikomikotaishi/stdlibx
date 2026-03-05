#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using RegexPatternBuffer = ::re_pattern_buffer;
    using RegexRegisters = ::re_registers;    

    using ::re_set_syntax;
    using ::re_compile_pattern;
    using ::re_compile_fastmap;
    using ::re_search;
    using ::re_search_2;
    using ::re_match;
    using ::re_match_2;
    using ::re_set_registers;
    using ::regcomp;
    using ::regexec;
    using ::regerror;
    using ::regfree;
    #endif
}
