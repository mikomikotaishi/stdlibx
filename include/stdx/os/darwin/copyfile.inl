#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Darwin file and metadata copying - <copyfile.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<copyfile.h>)
    using CopyFileState = ::copyfile_state_t;
    using CopyFileFlags = ::copyfile_flags_t;
    using CopyFileCallback = ::copyfile_callback_t;

    using ::copyfile;
    using ::fcopyfile;
    using ::copyfile_state_alloc;
    using ::copyfile_state_free;
    using ::copyfile_state_get;
    using ::copyfile_state_set;
    #endif
}
