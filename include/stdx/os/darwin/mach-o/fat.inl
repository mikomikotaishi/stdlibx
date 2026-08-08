#pragma once

/**
 * @namespace stdx::os::darwin::macho
 * @brief Universal binary headers - <mach-o/fat.h>.
 */
export namespace stdx::os::darwin::macho {
    #if defined(__APPLE__) && __has_include(<mach-o/fat.h>)
    using FatHeader = ::fat_header;
    using FatArch = ::fat_arch;
    using FatArch64 = ::fat_arch_64;
    #endif
}
