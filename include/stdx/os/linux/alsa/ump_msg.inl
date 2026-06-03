#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Universal MIDI Packet message parsing — alsa/ump_msg.h.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    // UMP message-type structures (union of MIDI 1.0 / 2.0 message layouts)
    using UmpMsgHeader = ::snd_ump_msg_hdr_t;
    using UmpMsgNoteOff = ::snd_ump_msg_note_off_t;
    using UmpMsgNoteOn = ::snd_ump_msg_note_on_t;
    using UmpMsgPolyPressure = ::snd_ump_msg_poly_pressure_t;
    using UmpMsgControlChange = ::snd_ump_msg_control_change_t;
    using UmpMsgProgramChange = ::snd_ump_msg_program_change_t;
    using UmpMsgChannelPressure = ::snd_ump_msg_channel_pressure_t;
    using UmpMsgPitchBend = ::snd_ump_msg_pitchbend_t;

    using ::snd_ump_msg_sysex_expand;
    using ::snd_ump_packet_length;
    #endif
    #endif
}
