#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA PCM (digital audio) — alsa/pcm.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    // Opaque handles
    using Pcm = ::snd_pcm_t;
    using PcmInfo = ::snd_pcm_info_t;
    using PcmHwParams = ::snd_pcm_hw_params_t;
    using PcmSwParams = ::snd_pcm_sw_params_t;
    using PcmStatus = ::snd_pcm_status_t;
    using PcmAccessMask = ::snd_pcm_access_mask_t;
    using PcmFormatMask = ::snd_pcm_format_mask_t;
    using PcmSubformatMask = ::snd_pcm_subformat_mask_t;
    using PcmChannelArea = ::snd_pcm_channel_area_t;
    using PcmSyncId = ::snd_pcm_sync_id_t;
    using PcmChmap = ::snd_pcm_chmap_t;
    using PcmChmapQuery = ::snd_pcm_chmap_query_t;

    // Frame counters
    using PcmUFrames = ::snd_pcm_uframes_t;
    using PcmSFrames = ::snd_pcm_sframes_t;

    // Enums
    using PcmClass = ::snd_pcm_class_t;
    using PcmSubclass = ::snd_pcm_subclass_t;
    using PcmStream = ::snd_pcm_stream_t;
    using PcmAccess = ::snd_pcm_access_t;
    using PcmFormat = ::snd_pcm_format_t;
    using PcmSubformat = ::snd_pcm_subformat_t;
    using PcmState = ::snd_pcm_state_t;
    using PcmStart = ::snd_pcm_start_t;
    using PcmXrun = ::snd_pcm_xrun_t;
    using PcmTimestamp = ::snd_pcm_tstamp_t;
    using PcmTimestampType = ::snd_pcm_tstamp_type_t;
    using PcmAudioTimestampType = ::snd_pcm_audio_tstamp_type_t;
    using PcmType = ::snd_pcm_type_t;
    using PcmHookType = ::snd_pcm_hook_type_t;

    // Hooks / scopes
    using PcmHook = ::snd_pcm_hook_t;
    using PcmHookFn = ::snd_pcm_hook_func_t;
    using PcmScope = ::snd_pcm_scope_t;
    using PcmScopeOps = ::snd_pcm_scope_ops_t;

    // Simple PCM helpers
    using SpcmLatency = ::snd_spcm_latency_t;
    using SpcmXrunType = ::snd_spcm_xrun_type_t;
    using SpcmDuplexType = ::snd_spcm_duplex_type_t;

    // Stream / device lifecycle
    using ::snd_pcm_open;
    using ::snd_pcm_open_lconf;
    using ::snd_pcm_open_fallback;
    using ::snd_pcm_close;
    using ::snd_pcm_type;
    using ::snd_pcm_stream;
    using ::snd_pcm_nonblock;
    using ::snd_pcm_state;
    using ::snd_pcm_prepare;
    using ::snd_pcm_start;
    using ::snd_pcm_drop;
    using ::snd_pcm_drain;
    using ::snd_pcm_pause;
    using ::snd_pcm_reset;
    using ::snd_pcm_resume;
    using ::snd_pcm_recover;
    using ::snd_pcm_wait;
    using ::snd_pcm_avail;
    using ::snd_pcm_avail_update;
    using ::snd_pcm_delay;
    using ::snd_pcm_status;
    using ::snd_pcm_info;
    using ::snd_pcm_hwsync;
    using ::snd_pcm_link;
    using ::snd_pcm_unlink;

    // Poll
    using ::snd_pcm_poll_descriptors;
    using ::snd_pcm_poll_descriptors_count;
    using ::snd_pcm_poll_descriptors_revents;

    // Read/write — blocking, interleaved
    using ::snd_pcm_writei;
    using ::snd_pcm_readi;
    using ::snd_pcm_writen;
    using ::snd_pcm_readn;
    using ::snd_pcm_mmap_begin;
    using ::snd_pcm_mmap_commit;
    using ::snd_pcm_mmap_writei;
    using ::snd_pcm_mmap_readi;
    using ::snd_pcm_mmap_writen;
    using ::snd_pcm_mmap_readn;

    // HW/SW parameter helpers
    using ::snd_pcm_hw_params_malloc;
    using ::snd_pcm_hw_params_free;
    using ::snd_pcm_hw_params_any;
    using ::snd_pcm_hw_params;
    using ::snd_pcm_hw_params_current;
    using ::snd_pcm_hw_params_set_access;
    using ::snd_pcm_hw_params_set_format;
    using ::snd_pcm_hw_params_set_rate_near;
    using ::snd_pcm_hw_params_set_rate;
    using ::snd_pcm_hw_params_set_channels;
    using ::snd_pcm_hw_params_set_buffer_size_near;
    using ::snd_pcm_hw_params_set_period_size_near;
    using ::snd_pcm_hw_params_get_rate;
    using ::snd_pcm_hw_params_get_channels;
    using ::snd_pcm_hw_params_get_buffer_size;
    using ::snd_pcm_hw_params_get_period_size;

    using ::snd_pcm_sw_params_malloc;
    using ::snd_pcm_sw_params_free;
    using ::snd_pcm_sw_params_current;
    using ::snd_pcm_sw_params;
    using ::snd_pcm_sw_params_set_avail_min;
    using ::snd_pcm_sw_params_set_start_threshold;
    using ::snd_pcm_sw_params_set_stop_threshold;

    // Format helpers
    using ::snd_pcm_format_name;
    using ::snd_pcm_format_description;
    using ::snd_pcm_format_value;
    using ::snd_pcm_format_signed;
    using ::snd_pcm_format_unsigned;
    using ::snd_pcm_format_linear;
    using ::snd_pcm_format_float;
    using ::snd_pcm_format_little_endian;
    using ::snd_pcm_format_big_endian;
    using ::snd_pcm_format_cpu_endian;
    using ::snd_pcm_format_width;
    using ::snd_pcm_format_physical_width;
    using ::snd_pcm_format_size;
    using ::snd_pcm_format_silence;

    // Channel maps
    using ::snd_pcm_get_chmap;
    using ::snd_pcm_set_chmap;
    using ::snd_pcm_query_chmaps;
    using ::snd_pcm_free_chmaps;
    #endif
    #endif
}
