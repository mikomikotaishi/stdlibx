#pragma once

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
