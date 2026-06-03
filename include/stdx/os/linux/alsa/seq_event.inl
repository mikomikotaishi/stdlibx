#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA sequencer event types — alsa/seq_event.h wrappers.
 *        Most contents are #define constants for event types; this file
 *        exposes the structural typedefs.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    // Time scalars
    using SequencerEventTypeId = ::snd_seq_event_type_t;
    using SequencerTickTime = ::snd_seq_tick_time_t;
    using SequencerRealTime = ::snd_seq_real_time_t;
    using SequencerTimestamp = ::snd_seq_timestamp_t;

    // Endpoint addressing
    using SequencerAddress = ::snd_seq_addr_t;
    using SequencerConnect = ::snd_seq_connect_t;

    // Event union and helpers
    using SequencerEvent = ::snd_seq_event_t;
    using SequencerExtendedEvent = ::snd_seq_ev_ext_t;
    using SequencerNoteEvent = ::snd_seq_ev_note_t;
    using SequencerControlEvent = ::snd_seq_ev_ctrl_t;
    using SequencerQueueControlEvent = ::snd_seq_ev_queue_control_t;
    #endif
    #endif
}
