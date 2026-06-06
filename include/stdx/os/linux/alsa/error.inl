#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA error handling - alsa/error.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundLibLogHandler = ::snd_lib_log_handler_t;
    using SoundLibErrorHandler = ::snd_lib_error_handler_t;
    using SoundLocalErrorHandler = ::snd_local_error_handler_t;

    using ::snd_strerror;
    using ::snd_lib_log;
    using ::snd_lib_log_filter;
    using ::snd_lib_check;
    using ::snd_lib_log_set_handler;
    using ::snd_lib_log_set_local;
    using ::snd_lib_error_set_handler;
    using ::snd_lib_error_set_local;
    #endif
}
