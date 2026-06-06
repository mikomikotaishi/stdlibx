#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA timer API - alsa/timer.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundTimer = ::snd_timer_t;
    using SoundTimerQuery = ::snd_timer_query_t;
    using SoundTimerId = ::snd_timer_id_t;
    using SoundTimerGlobalInfo = ::snd_timer_ginfo_t;
    using SoundTimerGlobalParams = ::snd_timer_gparams_t;
    using SoundTimerGlobalStatus = ::snd_timer_gstatus_t;
    using SoundTimerInfo = ::snd_timer_info_t;
    using SoundTimerParams = ::snd_timer_params_t;
    using SoundTimerStatus = ::snd_timer_status_t;
    using SoundTimerRead = ::snd_timer_read_t;
    using SoundTimerTimestampRead = ::snd_timer_tread_t;
    using SoundTimerClass = ::snd_timer_class_t;
    using SoundTimerSlaveClass = ::snd_timer_slave_class_t;
    using SoundTimerEvent = ::snd_timer_event_t;

    // Query
    using ::snd_timer_query_open;
    using ::snd_timer_query_open_lconf;
    using ::snd_timer_query_close;
    using ::snd_timer_query_next_device;
    using ::snd_timer_query_info;
    using ::snd_timer_query_params;
    using ::snd_timer_query_status;

    // Lifecycle
    using ::snd_timer_open;
    using ::snd_timer_open_lconf;
    using ::snd_timer_close;
    using ::snd_timer_poll_descriptors_count;
    using ::snd_timer_poll_descriptors;
    using ::snd_timer_poll_descriptors_revents;
    using ::snd_timer_info;
    using ::snd_timer_params;
    using ::snd_timer_status;
    using ::snd_timer_start;
    using ::snd_timer_stop;
    using ::snd_timer_continue;
    using ::snd_timer_read;
    #endif
}
