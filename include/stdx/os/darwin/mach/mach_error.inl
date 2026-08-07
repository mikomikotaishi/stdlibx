#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Rendering a kern_return_t as text - <mach/mach_error.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_error.h>)
    using ::mach_error;
    using ::mach_error_string;
    using ::mach_error_type;
    #endif
}
