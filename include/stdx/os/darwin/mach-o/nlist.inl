#pragma once

/**
 * @namespace stdx::os::darwin::macho
 * @brief Mach-O symbol table entries - <mach-o/nlist.h>.
 */
export namespace stdx::os::darwin::macho {
    #if defined(__APPLE__) && __has_include(<mach-o/nlist.h>)
    using SymbolListEntry = struct nlist;
    using SymbolListEntry64 = ::nlist_64;
    #endif
}
