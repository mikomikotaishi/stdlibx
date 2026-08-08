#pragma once

/**
 * @namespace stdx::os::darwin::os
 * @brief Signposts for Instruments - <os/signpost.h>.
 */
export namespace stdx::os::darwin::os {
    #if defined(__APPLE__) && defined(__clang__) && __has_include(<os/signpost.h>)
    using SignpostId = ::os_signpost_id_t;
    using SignpostType = ::os_signpost_type_t;

    using ::os_signpost_id_generate;
    using ::os_signpost_id_make_with_pointer;
    using ::os_signpost_enabled;
    #endif
}
