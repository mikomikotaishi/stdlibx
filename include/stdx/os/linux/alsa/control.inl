#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA control (CTL) API — alsa/control.h wrappers.
 *        Card-level control: card enumeration, mixer elements, events.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    // Card / control handles
    using Control = ::snd_ctl_t;
    using StaticControl = ::snd_sctl_t;
    using HardwareControl = ::snd_hctl_t;
    using HardwareControlElement = ::snd_hctl_elem_t;
    using HardwareControlCallback = ::snd_hctl_callback_t;
    using HardwareControlElementCallback = ::snd_hctl_elem_callback_t;

    // Info / element / value / event
    using ControlCardInfo = ::snd_ctl_card_info_t;
    using ControlElementId = ::snd_ctl_elem_id_t;
    using ControlElementList = ::snd_ctl_elem_list_t;
    using ControlElementInfo = ::snd_ctl_elem_info_t;
    using ControlElementValue = ::snd_ctl_elem_value_t;
    using ControlEvent = ::snd_ctl_event_t;

    // Enums
    using ControlElementType = ::snd_ctl_elem_type_t;
    using ControlElementInterface = ::snd_ctl_elem_iface_t;
    using ControlEventType = ::snd_ctl_event_type_t;

    // Card enumeration
    using ::snd_card_load;
    using ::snd_card_next;
    using ::snd_card_get_index;
    using ::snd_card_get_name;
    using ::snd_card_get_longname;

    // Control device lifecycle
    using ::snd_ctl_open;
    using ::snd_ctl_open_lconf;
    using ::snd_ctl_open_fallback;
    using ::snd_ctl_close;
    using ::snd_ctl_nonblock;
    using ::snd_ctl_async;
    using ::snd_ctl_poll_descriptors;
    using ::snd_ctl_poll_descriptors_count;
    using ::snd_ctl_poll_descriptors_revents;

    // Card / device info
    using ::snd_ctl_card_info;
    using ::snd_ctl_card_info_malloc;
    using ::snd_ctl_card_info_free;
    using ::snd_ctl_card_info_get_id;
    using ::snd_ctl_card_info_get_driver;
    using ::snd_ctl_card_info_get_name;
    using ::snd_ctl_card_info_get_longname;
    using ::snd_ctl_card_info_get_mixername;
    using ::snd_ctl_card_info_get_components;

    // Device enumeration
    using ::snd_ctl_pcm_next_device;
    using ::snd_ctl_pcm_info;
    using ::snd_ctl_rawmidi_next_device;
    using ::snd_ctl_rawmidi_info;
    using ::snd_ctl_hwdep_next_device;
    using ::snd_ctl_hwdep_info;

    // Element-level queries
    using ::snd_ctl_elem_list;
    using ::snd_ctl_elem_info;
    using ::snd_ctl_elem_read;
    using ::snd_ctl_elem_write;
    using ::snd_ctl_elem_add_integer;
    using ::snd_ctl_elem_add_boolean;
    using ::snd_ctl_elem_add_enumerated;
    using ::snd_ctl_elem_remove;
    using ::snd_ctl_elem_lock;
    using ::snd_ctl_elem_unlock;

    // Subscribe / events
    using ::snd_ctl_subscribe_events;
    using ::snd_ctl_read;
    using ::snd_ctl_wait;
    #endif
    #endif
}
