#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Universal MIDI Packet (UMP) I/O - alsa/ump.h.
 *        MIDI 2.0 endpoint access.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundUmp = ::snd_ump_t;
    using SoundUmpEndpointInfo = ::snd_ump_endpoint_info_t;
    using SoundUmpBlockInfo = ::snd_ump_block_info_t;

    using ::snd_ump_open;
    using ::snd_ump_close;
    using ::snd_ump_poll_descriptors_count;
    using ::snd_ump_poll_descriptors;
    using ::snd_ump_poll_descriptors_revents;
    using ::snd_ump_nonblock;
    using ::snd_ump_rawmidi_info;
    using ::snd_ump_rawmidi_params;
    using ::snd_ump_rawmidi_status;
    using ::snd_ump_endpoint_info;
    using ::snd_ump_block_info;
    using ::snd_ump_drain;
    using ::snd_ump_drop;
    using ::snd_ump_write;
    using ::snd_ump_read;
    #endif
}
