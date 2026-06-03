#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA configuration tree — alsa/conf.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using Config = ::snd_config_t;
    using ConfigIterator = ::snd_config_iterator_t;
    using ConfigUpdate = ::snd_config_update_t;
    using ConfigType = ::snd_config_type_t;
    using ConfigExpandFn = ::snd_config_expand_fcn_t;
    using DeviceName = ::snd_devname_t;

    // Top-level
    using ::snd_config_top;
    using ::snd_config_load;
    using ::snd_config_load_string;
    using ::snd_config_load_override;
    using ::snd_config_save;
    using ::snd_config_update;
    using ::snd_config_update_r;
    using ::snd_config_update_free;
    using ::snd_config_update_free_global;
    using ::snd_config_update_ref;
    using ::snd_config_ref;
    using ::snd_config_unref;

    // Node construction
    using ::snd_config_add;
    using ::snd_config_delete;
    using ::snd_config_delete_compound_members;
    using ::snd_config_copy;
    using ::snd_config_make;
    using ::snd_config_make_integer;
    using ::snd_config_make_integer64;
    using ::snd_config_make_real;
    using ::snd_config_make_string;
    using ::snd_config_make_pointer;
    using ::snd_config_make_compound;

    // Search / set / get
    using ::snd_config_search;
    using ::snd_config_searchv;
    using ::snd_config_search_alias;
    using ::snd_config_search_alias_hooks;
    using ::snd_config_get_type;
    using ::snd_config_get_id;
    using ::snd_config_set_id;
    using ::snd_config_get_integer;
    using ::snd_config_get_integer64;
    using ::snd_config_get_real;
    using ::snd_config_get_ireal;
    using ::snd_config_get_string;
    using ::snd_config_get_ascii;
    using ::snd_config_get_pointer;
    using ::snd_config_set_integer;
    using ::snd_config_set_integer64;
    using ::snd_config_set_real;
    using ::snd_config_set_string;
    using ::snd_config_set_ascii;
    using ::snd_config_set_pointer;

    // Iteration
    using ::snd_config_iterator_first;
    using ::snd_config_iterator_next;
    using ::snd_config_iterator_end;
    using ::snd_config_iterator_entry;
    #endif
    #endif
}
