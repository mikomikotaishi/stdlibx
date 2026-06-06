#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/acct.h>)
    using Account = struct acct;
    using AccountV3 = ::acct_v3;

    using ::acct;
    #endif
}
