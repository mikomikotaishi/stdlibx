#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Universal MIDI Packet message parsing - alsa/ump_msg.h.
 *
 * MIDI 1.0 (Channel Voice Message) and MIDI 2.0 message layouts are
 * exposed as separate types; pick the variant that matches the protocol
 * the endpoint negotiated. The trailing union types wrap every variant
 * for ergonomic access.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    // Header common to all UMP messages
    using SoundUmpMsgHeader = ::snd_ump_msg_hdr_t;

    // MIDI 1.0 (over UMP) message layouts. Note-on and note-off share the
    // same `_note_t` structure - the status byte tells them apart.
    using SoundUmpMsgMidi1Note = ::snd_ump_msg_midi1_note_t;
    using SoundUmpMsgMidi1PolyAfterTouch = ::snd_ump_msg_midi1_paf_t;
    using SoundUmpMsgMidi1ControlChange = ::snd_ump_msg_midi1_cc_t;
    using SoundUmpMsgMidi1ProgramChange = ::snd_ump_msg_midi1_program_t;
    using SoundUmpMsgMidi1ChannelAfterTouch = ::snd_ump_msg_midi1_caf_t;
    using SoundUmpMsgMidi1PitchBend = ::snd_ump_msg_midi1_pitchbend_t;
    using SoundUmpMsgMidi1 = ::snd_ump_msg_midi1_t;

    // MIDI 2.0 message layouts
    using SoundUmpMsgMidi2Note = ::snd_ump_msg_midi2_note_t;
    using SoundUmpMsgMidi2PolyAfterTouch = ::snd_ump_msg_midi2_paf_t;
    using SoundUmpMsgMidi2PerNoteControlChange = ::snd_ump_msg_midi2_per_note_cc_t;
    using SoundUmpMsgMidi2PerNoteManagement = ::snd_ump_msg_midi2_per_note_mgmt_t;
    using SoundUmpMsgMidi2ControlChange = ::snd_ump_msg_midi2_cc_t;
    using SoundUmpMsgMidi2RegisteredParameter = ::snd_ump_msg_midi2_rpn_t;
    using SoundUmpMsgMidi2ProgramChange = ::snd_ump_msg_midi2_program_t;
    using SoundUmpMsgMidi2ChannelAfterTouch = ::snd_ump_msg_midi2_caf_t;
    using SoundUmpMsgMidi2PitchBend = ::snd_ump_msg_midi2_pitchbend_t;
    using SoundUmpMsgMidi2PerNotePitchBend = ::snd_ump_msg_midi2_per_note_pitchbend_t;
    using SoundUmpMsgMidi2 = ::snd_ump_msg_midi2_t;

    // System / utility / stream / flex-data
    using SoundUmpMsgSystem = ::snd_ump_msg_system_t;
    using SoundUmpMsgUtility = ::snd_ump_msg_utility_t;

    using ::snd_ump_msg_sysex_expand;
    using ::snd_ump_packet_length;
    #endif
}
