#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Darwin system operations.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/event.h>)
    /**
     * @brief One change to, or one report from, a kernel event queue.
     *
     * Spelled with an elaborated type specifier because `kevent` names both this
     * struct and the function below, and in C++ the function hides the struct in
     * ordinary lookup.
     */
    using KEvent = struct ::kevent;
    using KEvent64 = struct ::kevent64_s;

    using ::kqueue;
    using ::kevent;
    using ::kevent64;
    #endif
}

/**
 * @internal
 * @namespace stdx::os::darwin::captured
 * @brief The <sys/event.h> macro values, read where they exist.
 *
 * darwin.constants.inl declares these under their real names and asserts each
 * against the value here, so a mistyped literal fails the build on Darwin rather
 * than registering the wrong kqueue filter at run time. See
 * stdx::os::unix::sys::captured for why the values have to be laundered through
 * a variable at all.
 */
namespace stdx::os::darwin::captured {
    #if defined(__APPLE__) && __has_include(<sys/event.h>)
    constexpr auto EVFILT_READ_VALUE = EVFILT_READ;
    constexpr auto EVFILT_WRITE_VALUE = EVFILT_WRITE;
    constexpr auto EVFILT_USER_VALUE = EVFILT_USER;
    constexpr auto EVFILT_TIMER_VALUE = EVFILT_TIMER;
    constexpr auto EVFILT_EXCEPT_VALUE = EVFILT_EXCEPT;

    constexpr auto EV_ADD_VALUE = EV_ADD;
    constexpr auto EV_DELETE_VALUE = EV_DELETE;
    constexpr auto EV_ENABLE_VALUE = EV_ENABLE;
    constexpr auto EV_DISABLE_VALUE = EV_DISABLE;
    constexpr auto EV_ONESHOT_VALUE = EV_ONESHOT;
    constexpr auto EV_CLEAR_VALUE = EV_CLEAR;
    constexpr auto EV_RECEIPT_VALUE = EV_RECEIPT;
    constexpr auto EV_DISPATCH_VALUE = EV_DISPATCH;
    constexpr auto EV_ERROR_VALUE = EV_ERROR;
    constexpr auto EV_EOF_VALUE = EV_EOF;

    constexpr auto NOTE_TRIGGER_VALUE = NOTE_TRIGGER;
    #endif
}
