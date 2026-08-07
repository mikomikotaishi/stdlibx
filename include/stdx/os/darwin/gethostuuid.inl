#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief The host's persistent UUID - <gethostuuid.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<gethostuuid.h>)
    using ::gethostuuid;
    #endif
}
