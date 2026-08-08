#pragma once

/**
 * @namespace stdx::os::darwin::macho
 * @brief The Mach-O file format - <mach-o/loader.h>.
 */
export namespace stdx::os::darwin::macho {
    #if defined(__APPLE__) && __has_include(<mach-o/loader.h>)
    using MachHeader = ::mach_header;
    using MachHeader64 = ::mach_header_64;
    using LoadCommand = ::load_command;
    using SegmentCommand = ::segment_command;
    using SegmentCommand64 = ::segment_command_64;
    using Section = ::section;
    using Section64 = ::section_64;
    using SymtabCommand = ::symtab_command;
    using DysymtabCommand = ::dysymtab_command;
    using DylibCommand = ::dylib_command;
    using UuidCommand = ::uuid_command;
    using LinkeditDataCommand = ::linkedit_data_command;
    #endif
}
