#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief The Mach clock services - <mach/clock.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/clock.h>)
    using ClockService = ::clock_serv_t;
    using ClockId = ::clock_id_t;
    using ClockFlavor = ::clock_flavor_t;

    using ::clock_get_time;
    using ::clock_get_attributes;
    using ::clock_alarm;
    using ::clock_sleep;
    #endif
}
