#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA mixer API — alsa/mixer.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using Mixer = ::snd_mixer_t;
    using MixerClass = ::snd_mixer_class_t;
    using MixerElement = ::snd_mixer_elem_t;
    using MixerSimpleElementId = ::snd_mixer_selem_id_t;
    using MixerSimpleChannelId = ::snd_mixer_selem_channel_id_t;
    using MixerElementType = ::snd_mixer_elem_type_t;
    using MixerElementCallback = ::snd_mixer_elem_callback_t;
    using MixerCallback = ::snd_mixer_callback_t;

    // Mixer lifecycle
    using ::snd_mixer_open;
    using ::snd_mixer_close;
    using ::snd_mixer_attach;
    using ::snd_mixer_attach_hctl;
    using ::snd_mixer_detach;
    using ::snd_mixer_detach_hctl;
    using ::snd_mixer_get_hctl;
    using ::snd_mixer_load;
    using ::snd_mixer_free;
    using ::snd_mixer_handle_events;
    using ::snd_mixer_poll_descriptors;
    using ::snd_mixer_poll_descriptors_count;
    using ::snd_mixer_poll_descriptors_revents;
    using ::snd_mixer_wait;

    // Callback registration
    using ::snd_mixer_set_callback;
    using ::snd_mixer_set_callback_private;
    using ::snd_mixer_get_callback_private;
    using ::snd_mixer_get_count;

    // Element traversal
    using ::snd_mixer_first_elem;
    using ::snd_mixer_last_elem;
    using ::snd_mixer_elem_next;
    using ::snd_mixer_elem_prev;
    using ::snd_mixer_elem_get_name;
    using ::snd_mixer_elem_get_index;
    using ::snd_mixer_elem_set_callback;
    using ::snd_mixer_elem_set_callback_private;
    using ::snd_mixer_elem_get_callback_private;

    // Simple-mixer (selem) — most commonly used
    using ::snd_mixer_selem_register;
    using ::snd_mixer_selem_id_malloc;
    using ::snd_mixer_selem_id_free;
    using ::snd_mixer_selem_id_set_name;
    using ::snd_mixer_selem_id_set_index;
    using ::snd_mixer_selem_id_get_name;
    using ::snd_mixer_selem_id_get_index;
    using ::snd_mixer_find_selem;
    using ::snd_mixer_selem_get_id;
    using ::snd_mixer_selem_get_name;
    using ::snd_mixer_selem_get_index;

    // Simple-mixer queries
    using ::snd_mixer_selem_has_playback_volume;
    using ::snd_mixer_selem_has_playback_switch;
    using ::snd_mixer_selem_has_capture_volume;
    using ::snd_mixer_selem_has_capture_switch;
    using ::snd_mixer_selem_get_playback_volume_range;
    using ::snd_mixer_selem_get_capture_volume_range;
    using ::snd_mixer_selem_get_playback_dB_range;
    using ::snd_mixer_selem_get_capture_dB_range;
    using ::snd_mixer_selem_set_playback_volume_range;
    using ::snd_mixer_selem_set_capture_volume_range;

    // Simple-mixer volume / switch get/set
    using ::snd_mixer_selem_get_playback_volume;
    using ::snd_mixer_selem_set_playback_volume;
    using ::snd_mixer_selem_set_playback_volume_all;
    using ::snd_mixer_selem_get_capture_volume;
    using ::snd_mixer_selem_set_capture_volume;
    using ::snd_mixer_selem_set_capture_volume_all;
    using ::snd_mixer_selem_get_playback_dB;
    using ::snd_mixer_selem_set_playback_dB;
    using ::snd_mixer_selem_set_playback_dB_all;
    using ::snd_mixer_selem_get_capture_dB;
    using ::snd_mixer_selem_set_capture_dB;
    using ::snd_mixer_selem_set_capture_dB_all;
    using ::snd_mixer_selem_get_playback_switch;
    using ::snd_mixer_selem_set_playback_switch;
    using ::snd_mixer_selem_set_playback_switch_all;
    using ::snd_mixer_selem_get_capture_switch;
    using ::snd_mixer_selem_set_capture_switch;
    using ::snd_mixer_selem_set_capture_switch_all;
    #endif
    #endif
}
