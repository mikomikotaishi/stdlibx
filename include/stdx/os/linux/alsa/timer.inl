#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA timer API — alsa/timer.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using Timer = ::snd_timer_t;
    using TimerQuery = ::snd_timer_query_t;
    using TimerId = ::snd_timer_id_t;
    using TimerGlobalInfo = ::snd_timer_ginfo_t;
    using TimerGlobalParams = ::snd_timer_gparams_t;
    using TimerGlobalStatus = ::snd_timer_gstatus_t;
    using TimerInfo = ::snd_timer_info_t;
    using TimerParams = ::snd_timer_params_t;
    using TimerStatus = ::snd_timer_status_t;
    using TimerRead = ::snd_timer_read_t;
    using TimerTimestampRead = ::snd_timer_tread_t;
    using TimerClass = ::snd_timer_class_t;
    using TimerSlaveClass = ::snd_timer_slave_class_t;
    using TimerEvent = ::snd_timer_event_t;

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
    using ::snd_timer_async;
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
    #endif
}
