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

    // Endpoint info: lifecycle, selectors, and queries used for enumeration.
    using ::snd_ump_endpoint_info_malloc;
    using ::snd_ump_endpoint_info_free;
    using ::snd_ump_endpoint_info_set_device;
    using ::snd_ump_endpoint_info_get_device;
    using ::snd_ump_endpoint_info_get_protocol_caps;
    using ::snd_ump_endpoint_info_get_protocol;
    using ::snd_ump_endpoint_info_get_num_blocks;
    using ::snd_ump_endpoint_info_get_name;
    using ::snd_ump_endpoint_info_set_name;
    using ::snd_ump_endpoint_info_set_protocol_caps;
    using ::snd_ump_endpoint_info_set_protocol;
    using ::snd_ump_endpoint_info_set_num_blocks;
    using ::snd_ump_endpoint_info_set_version;
    using ::snd_ump_endpoint_info_set_product_id;

    // Protocol-capability bit flags (MIDI 1.0 / 2.0).
    using ::snd_ump_block_info_malloc;
    using ::snd_ump_block_info_free;
    using ::snd_ump_block_info_set_block_id;
    using ::snd_ump_block_info_get_active;
    using ::snd_ump_block_info_get_direction;
    using ::snd_ump_block_info_get_first_group;
    using ::snd_ump_block_info_get_num_groups;
    using ::snd_ump_block_info_get_ui_hint;
    using ::snd_ump_block_info_get_name;
    using ::snd_ump_block_info_set_active;
    using ::snd_ump_block_info_set_direction;
    using ::snd_ump_block_info_set_first_group;
    using ::snd_ump_block_info_set_num_groups;
    using ::snd_ump_block_info_set_ui_hint;
    using ::snd_ump_block_info_set_name;

    // Macro constants re-exported as typed constants (the preprocessor macros
    // themselves cannot be brought in with a using-declaration).
    inline constexpr unsigned int UMP_DIR_INPUT = SND_UMP_DIR_INPUT;
    inline constexpr unsigned int UMP_DIR_OUTPUT = SND_UMP_DIR_OUTPUT;
    inline constexpr unsigned int UMP_DIR_BIDIRECTION = SND_UMP_DIR_BIDIRECTION;
    inline constexpr unsigned int UMP_EP_INFO_PROTO_MIDI2 = SND_UMP_EP_INFO_PROTO_MIDI2;
    inline constexpr unsigned int UMP_EP_INFO_PROTO_MIDI1 = SND_UMP_EP_INFO_PROTO_MIDI1;
    #endif
}
