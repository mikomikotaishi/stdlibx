#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Raw-MIDI <-> sequencer-event conversion — alsa/seq_midi_event.h.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using MidiEventParser = ::snd_midi_event_t;

    using ::snd_midi_event_new;
    using ::snd_midi_event_resize_buffer;
    using ::snd_midi_event_free;
    using ::snd_midi_event_init;
    using ::snd_midi_event_reset_encode;
    using ::snd_midi_event_reset_decode;
    using ::snd_midi_event_no_status;
    using ::snd_midi_event_encode;
    using ::snd_midi_event_encode_byte;
    using ::snd_midi_event_decode;
    #endif
    #endif
}
