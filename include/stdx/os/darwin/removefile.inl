#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Darwin recursive removal - <removefile.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<removefile.h>)
    using RemoveFileState = ::removefile_state_t;
    using RemoveFileFlags = ::removefile_flags_t;
    using RemoveFileCallback = ::removefile_callback_t;

    using ::removefile;
    using ::removefileat;
    using ::removefile_cancel;
    using ::removefile_state_alloc;
    using ::removefile_state_free;
    using ::removefile_state_get;
    using ::removefile_state_set;
    #endif
}
