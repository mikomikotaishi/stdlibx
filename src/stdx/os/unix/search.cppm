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

export module stdx:os.unix.search;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using QueueElement = ::qelem;

    using ::insque;
    using ::remque;

    using Entry = ::entry;

    using ::hsearch;
    using ::hcreate;
    using ::hdestroy;

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
