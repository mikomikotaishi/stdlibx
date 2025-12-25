/**
 * @file regex.cppm
 * @module stdx:os.unix.regex
 * @brief Module file for Unix regular expression operations.
 *
 * This file contains the implementation of the Unix regular expression operations from the POSIX libraries,
 * located in <regex.h>.
 */

module;

#ifdef __unix__
#include <regex.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.regex;
#else
export module stdlibx:os.unix.regex;
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
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using REPatternBuffer = ::re_pattern_buffer;
    using RERegisters = ::re_registers;
    #endif

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
