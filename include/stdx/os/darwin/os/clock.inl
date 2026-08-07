#pragma once

/**
 * @namespace stdx::os::darwin::os
 * @brief The nanosecond clock reads - <time.h> clock_gettime_nsec_np.
 */
export namespace stdx::os::darwin::os {
    #if defined(__APPLE__) && __has_include(<time.h>)
    using ::clock_gettime_nsec_np;
    #endif
}
