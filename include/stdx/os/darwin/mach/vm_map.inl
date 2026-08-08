#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Virtual memory of a task - <mach/vm_map.h> and <mach/mach_vm.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_vm.h>)
    using VmRegionBasicInfo64 = ::vm_region_basic_info_data_64_t;
    using VmRegionExtendedInfo = ::vm_region_extended_info_data_t;
    using VmRegionSubmapInfo64 = ::vm_region_submap_info_data_64_t;

    using ::mach_vm_allocate;
    using ::mach_vm_deallocate;
    using ::mach_vm_protect;
    using ::mach_vm_read;
    using ::mach_vm_read_overwrite;
    using ::mach_vm_write;
    using ::mach_vm_copy;
    using ::mach_vm_map;
    using ::mach_vm_remap;
    using ::mach_vm_region;
    using ::mach_vm_region_recurse;
    using ::mach_vm_behavior_set;
    using ::mach_vm_msync;
    using ::mach_vm_wire;
    #endif
}
