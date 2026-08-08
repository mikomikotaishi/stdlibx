#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Access to the process's argv and environ as dyld holds them - <crt_externs.h>.
 *
 * A dylib on Darwin cannot reference `environ` directly the way it can on Linux;
 * `*_NSGetEnviron()` is the supported way to reach it.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<crt_externs.h>)
    using ::_NSGetArgc;
    using ::_NSGetArgv;
    using ::_NSGetEnviron;
    using ::_NSGetProgname;
    using ::_NSGetMachExecuteHeader;
    #endif
}
