#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief The Mach primitive types every other mach header is written in terms of
 *        - <mach/mach_types.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_types.h>)
    using KernelReturn = ::kern_return_t;
    using MachPort = ::mach_port_t;
    using MachPortName = ::mach_port_name_t;
    using MachPortRight = ::mach_port_right_t;
    using MachPortType = ::mach_port_type_t;
    using Task = ::task_t;
    using TaskName = ::task_name_t;
    using Thread = ::thread_t;
    using ThreadAct = ::thread_act_t;
    using Host = ::host_t;
    using HostPriv = ::host_priv_t;
    using ProcessorSet = ::processor_set_t;
    using Semaphore = ::semaphore_t;
    using IpcSpace = ::ipc_space_t;
    using VmMap = ::vm_map_t;
    using VmAddress = ::vm_address_t;
    using VmSize = ::vm_size_t;
    using VmOffset = ::vm_offset_t;
    using VmProt = ::vm_prot_t;
    using MachVmAddress = ::mach_vm_address_t;
    using MachVmSize = ::mach_vm_size_t;
    using MachTimespec = ::mach_timespec_t;
    using NaturalT = ::natural_t;
    using IntegerT = ::integer_t;
    #endif
}
