#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA error handling — alsa/error.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using LibLogHandler = ::snd_lib_log_handler_t;
    using LibErrorHandler = ::snd_lib_error_handler_t;
    using LocalErrorHandler = ::snd_local_error_handler_t;

    using ::snd_strerror;
    using ::snd_lib_log;
    using ::snd_lib_log_filter;
    using ::snd_lib_check;
    using ::snd_lib_log_set_handler;
    using ::snd_lib_log_set_local;
    using ::snd_lib_error_set_handler;
    using ::snd_lib_error_set_local;
    #endif
    #endif
}
