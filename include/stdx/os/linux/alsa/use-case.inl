#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Use Case Manager — alsa/use-case.h.
 *        High-level audio routing scenarios (e.g. "phone call", "voice
 *        recognition") parsed from per-card UCM config files.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/use-case.h>)
    using UseCaseManager = ::snd_use_case_mgr_t;

    using ::snd_use_case_mgr_open;
    using ::snd_use_case_mgr_close;
    using ::snd_use_case_mgr_reload;
    using ::snd_use_case_mgr_reset;

    using ::snd_use_case_get_list;
    using ::snd_use_case_free_list;
    using ::snd_use_case_get;
    using ::snd_use_case_geti;
    using ::snd_use_case_set;
    using ::snd_use_case_card_list;

    using ::snd_use_case_parse_selector_name;
    using ::snd_use_case_parse_ctl_elem_id;
    using ::snd_use_case_parse_ctl_elem_value;
    #endif
    #endif
}
