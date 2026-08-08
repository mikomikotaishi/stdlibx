#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief The Mach IPC primitive - <mach/message.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/message.h>)
    using MachMessageHeader = ::mach_msg_header_t;
    using MachMessageBody = ::mach_msg_body_t;
    using MachMessageTrailer = ::mach_msg_trailer_t;
    using MachMessageAuditTrailer = ::mach_msg_audit_trailer_t;
    using MachMessagePortDescriptor = ::mach_msg_port_descriptor_t;
    using MachMessageOolDescriptor = ::mach_msg_ool_descriptor_t;
    using MachMessageReturn = ::mach_msg_return_t;
    using MachMessageOption = ::mach_msg_option_t;
    using MachMessageSize = ::mach_msg_size_t;
    using MachMessageTimeout = ::mach_msg_timeout_t;

    using ::mach_msg;
    using ::mach_msg_overwrite;
    using ::mach_msg_destroy;
    using ::mach_msg_receive;
    using ::mach_msg_send;
    #endif
}
