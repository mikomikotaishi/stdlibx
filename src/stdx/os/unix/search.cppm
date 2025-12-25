/**
 * @file search.cppm
 * @module stdx:os.unix.search
 * @brief Module file for Unix search table operations.
 *
 * This file contains the implementation of the Unix search table operations from the POSIX libraries,
 * located in <search.h>.
 */

module;

#ifdef __unix__
#include <search.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.search;
#else
export module stdlibx:os.unix.search;
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
    using QElem = ::qelem;
    #endif

    using QueueElement = ::qelem;

    using ::insque;
    using ::remque;

    using Entry = ::entry;

    using ::hsearch;
    using ::hcreate;
    using ::hdestroy;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using HSearchData = ::hsearch_data;
    #endif

    using HashSearchData = ::hsearch_data;

    using ::hsearch_r;
    using ::hcreate_r;
    using ::hdestroy_r;

    using ::tsearch;
    using ::tfind;
    using ::tdelete;

    using ::twalk;
    using ::twalk_r;
    using ::tdestroy;
    using ::lfind;
    using ::lsearch;
    #endif
}
