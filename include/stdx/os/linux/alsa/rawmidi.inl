#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA raw MIDI I/O — alsa/rawmidi.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using RawMidi = ::snd_rawmidi_t;
    using RawMidiInfo = ::snd_rawmidi_info_t;
    using RawMidiParams = ::snd_rawmidi_params_t;
    using RawMidiStatus = ::snd_rawmidi_status_t;
    using RawMidiStream = ::snd_rawmidi_stream_t;

    // Lifecycle
    using ::snd_rawmidi_open;
    using ::snd_rawmidi_open_lconf;
    using ::snd_rawmidi_close;
    using ::snd_rawmidi_nonblock;
    using ::snd_rawmidi_name;
    using ::snd_rawmidi_type;
    using ::snd_rawmidi_stream;

    // Poll
    using ::snd_rawmidi_poll_descriptors;
    using ::snd_rawmidi_poll_descriptors_count;
    using ::snd_rawmidi_poll_descriptors_revents;

    // Params / status
    using ::snd_rawmidi_info;
    using ::snd_rawmidi_info_sizeof;
    using ::snd_rawmidi_info_malloc;
    using ::snd_rawmidi_info_free;
    using ::snd_rawmidi_params_sizeof;
    using ::snd_rawmidi_params_malloc;
    using ::snd_rawmidi_params_free;
    using ::snd_rawmidi_params_current;
    using ::snd_rawmidi_params;
    using ::snd_rawmidi_status_sizeof;
    using ::snd_rawmidi_status_malloc;
    using ::snd_rawmidi_status_free;
    using ::snd_rawmidi_status;

    // I/O
    using ::snd_rawmidi_drain;
    using ::snd_rawmidi_drop;
    using ::snd_rawmidi_write;
    using ::snd_rawmidi_read;
    #endif
    #endif
}
