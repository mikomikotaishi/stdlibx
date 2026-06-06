#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA sequencer event types - alsa/seq_event.h wrappers.
 *        Most contents are #define constants for event types; this file
 *        exposes the structural typedefs.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    // Time scalars
    using SoundSequencerEventTypeId = ::snd_seq_event_type_t;
    using SoundSequencerTickTime = ::snd_seq_tick_time_t;
    using SoundSequencerRealTime = ::snd_seq_real_time_t;
    using SoundSequencerTimestamp = ::snd_seq_timestamp_t;

    // Endpoint addressing
    using SoundSequencerAddress = ::snd_seq_addr_t;
    using SoundSequencerConnect = ::snd_seq_connect_t;

    // Event union and helpers
    using SoundSequencerEvent = ::snd_seq_event_t;
    using SoundSequencerExtendedEvent = ::snd_seq_ev_ext_t;
    using SoundSequencerNoteEvent = ::snd_seq_ev_note_t;
    using SoundSequencerControlEvent = ::snd_seq_ev_ctrl_t;
    using SoundSequencerQueueControlEvent = ::snd_seq_ev_queue_control_t;
    #endif
}
