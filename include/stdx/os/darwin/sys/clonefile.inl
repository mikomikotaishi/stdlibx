#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Copy-on-write file cloning on APFS - <sys/clonefile.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/clonefile.h>)
    using ::clonefile;
    using ::clonefileat;
    using ::fclonefileat;
    #endif
}
