#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::setutxent;
    using ::endutxent;
    using ::getutxent;
    using ::getutxid;
    using ::getutxline;
    using ::pututxline;
    using ::utmpxname;
    using ::updwtmpx;
    using ::getutmp;
    using ::getutmpx;
    #endif
}
