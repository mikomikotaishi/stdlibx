#pragma once

/**
 * @namespace stdx::os::darwin::os
 * @brief The unified logging system - <os/log.h>.
 *
 * The os_log family proper are macros that capture a format string at compile
 * time; what a using-declaration can reach is the surrounding machinery.
 */
export namespace stdx::os::darwin::os {
    #if defined(__APPLE__) && defined(__clang__) && __has_include(<os/log.h>)
    using Log = ::os_log_t;
    using LogType = ::os_log_type_t;

    using ::os_log_create;
    using ::os_log_type_enabled;
    #endif
}
