#pragma once

/**
 * @namespace stdx::os::darwin::macho
 * @brief Locating a section within the running image - <mach-o/getsect.h>.
 */
export namespace stdx::os::darwin::macho {
    #if defined(__APPLE__) && __has_include(<mach-o/getsect.h>)
    using ::getsectbyname;
    using ::getsegbyname;
    using ::getsectiondata;
    using ::getsegmentdata;
    using ::getsectdatafromFramework;
    #endif
}
