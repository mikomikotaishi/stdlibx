#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA global helpers - alsa/global.h.
 *        Async handlers, dl-loading, timestamps, shared-memory areas.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundAsyncHandler = ::snd_async_handler_t;
    using SoundAsyncCallback = ::snd_async_callback_t;
    using SoundTimestamp = ::snd_timestamp_t;
    using SoundHighResolutionTimestamp = ::snd_htimestamp_t;

    // Dynamic loader paths
    using ::snd_dlopen;
    using ::snd_dlpath;
    using ::snd_dlclose;
    using ::snd_dlsym;

    // Async event registration
    using ::snd_async_add_handler;
    using ::snd_async_del_handler;
    using ::snd_async_handler_get_fd;
    using ::snd_async_handler_get_signo;
    using ::snd_async_handler_get_callback_private;

    // User config / paths
    using ::snd_user_file;
    #endif
}
