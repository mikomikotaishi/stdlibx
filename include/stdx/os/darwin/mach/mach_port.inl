#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Port right management - <mach/mach_port.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_port.h>)
    using MachPortStatus = ::mach_port_status_t;
    using MachPortLimits = ::mach_port_limits_t;

    using ::mach_port_allocate;
    using ::mach_port_allocate_name;
    using ::mach_port_deallocate;
    using ::mach_port_destroy;
    using ::mach_port_insert_right;
    using ::mach_port_extract_right;
    using ::mach_port_mod_refs;
    using ::mach_port_get_refs;
    using ::mach_port_get_attributes;
    using ::mach_port_set_attributes;
    using ::mach_port_move_member;
    using ::mach_port_names;
    using ::mach_port_type;
    #endif
}
