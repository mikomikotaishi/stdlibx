#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief The monotonic Mach clock - <mach/mach_time.h>.
 *
 * mach_absolute_time is the clock CoreAudio timestamps are expressed in;
 * mach_timebase_info gives the rational factor that converts it to nanoseconds.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_time.h>)
    using MachTimebaseInfo = ::mach_timebase_info_data_t;

    using ::mach_timebase_info;
    using ::mach_absolute_time;
    using ::mach_approximate_time;
    using ::mach_continuous_time;
    using ::mach_continuous_approximate_time;
    using ::mach_wait_until;
    #endif
}
