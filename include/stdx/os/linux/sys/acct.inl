#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using Account = struct acct;
    using AccountV3 = ::acct_v3;

    using ::acct;
    #endif
}
