#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA PCM (digital audio) - alsa/pcm.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    // Opaque handles
    using SoundPcm = ::snd_pcm_t;
    using SoundPcmInfo = ::snd_pcm_info_t;
    using SoundPcmHwParams = ::snd_pcm_hw_params_t;
    using SoundPcmSwParams = ::snd_pcm_sw_params_t;
    using SoundPcmStatus = ::snd_pcm_status_t;
    using SoundPcmAccessMask = ::snd_pcm_access_mask_t;
    using SoundPcmFormatMask = ::snd_pcm_format_mask_t;
    using SoundPcmSubformatMask = ::snd_pcm_subformat_mask_t;
    using SoundPcmChannelArea = ::snd_pcm_channel_area_t;
    using SoundPcmSyncId = ::snd_pcm_sync_id_t;
    using SoundPcmChmap = ::snd_pcm_chmap_t;
    using SoundPcmChmapQuery = ::snd_pcm_chmap_query_t;

    // Frame counters
    using SoundPcmUFrames = ::snd_pcm_uframes_t;
    using SoundPcmSFrames = ::snd_pcm_sframes_t;

    // Enums (each wrapped so callers can write SoundPcmStream::PLAYBACK
    // instead of SND_PCM_STREAM_PLAYBACK while still implicit-converting to
    // and from the underlying ::snd_pcm_*_t)

    class [[nodiscard]] SoundPcmClass final {
    public:
        using Self = ::snd_pcm_class_t;

        static constexpr Self GENERIC = ::snd_pcm_class_t::SND_PCM_CLASS_GENERIC;
        static constexpr Self MULTI = ::snd_pcm_class_t::SND_PCM_CLASS_MULTI;
        static constexpr Self MODEM = ::snd_pcm_class_t::SND_PCM_CLASS_MODEM;
        static constexpr Self DIGITIZER = ::snd_pcm_class_t::SND_PCM_CLASS_DIGITIZER;
    private:
        const Self value;
    public:
        constexpr SoundPcmClass(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmSubclass final {
    public:
        using Self = ::snd_pcm_subclass_t;

        static constexpr Self GENERIC_MIX = ::snd_pcm_subclass_t::SND_PCM_SUBCLASS_GENERIC_MIX;
        static constexpr Self MULTI_MIX = ::snd_pcm_subclass_t::SND_PCM_SUBCLASS_MULTI_MIX;
    private:
        const Self value;
    public:
        constexpr SoundPcmSubclass(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmStream final {
    public:
        using Self = ::snd_pcm_stream_t;

        static constexpr Self PLAYBACK = ::snd_pcm_stream_t::SND_PCM_STREAM_PLAYBACK;
        static constexpr Self CAPTURE = ::snd_pcm_stream_t::SND_PCM_STREAM_CAPTURE;
    private:
        const Self value;
    public:
        constexpr SoundPcmStream(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmAccess final {
    public:
        using Self = ::snd_pcm_access_t;

        static constexpr Self MMAP_INTERLEAVED = ::snd_pcm_access_t::SND_PCM_ACCESS_MMAP_INTERLEAVED;
        static constexpr Self MMAP_NONINTERLEAVED = ::snd_pcm_access_t::SND_PCM_ACCESS_MMAP_NONINTERLEAVED;
        static constexpr Self MMAP_COMPLEX = ::snd_pcm_access_t::SND_PCM_ACCESS_MMAP_COMPLEX;
        static constexpr Self RW_INTERLEAVED = ::snd_pcm_access_t::SND_PCM_ACCESS_RW_INTERLEAVED;
        static constexpr Self RW_NONINTERLEAVED = ::snd_pcm_access_t::SND_PCM_ACCESS_RW_NONINTERLEAVED;
    private:
        const Self value;
    public:
        constexpr SoundPcmAccess(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmFormat final {
    public:
        using Self = ::snd_pcm_format_t;

        static constexpr Self UNKNOWN = ::snd_pcm_format_t::SND_PCM_FORMAT_UNKNOWN;
        static constexpr Self S8 = ::snd_pcm_format_t::SND_PCM_FORMAT_S8;
        static constexpr Self U8 = ::snd_pcm_format_t::SND_PCM_FORMAT_U8;
        static constexpr Self S16_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S16_LE;
        static constexpr Self S16_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S16_BE;
        static constexpr Self U16_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U16_LE;
        static constexpr Self U16_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U16_BE;
        static constexpr Self S24_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S24_LE;
        static constexpr Self S24_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S24_BE;
        static constexpr Self U24_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U24_LE;
        static constexpr Self U24_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U24_BE;
        static constexpr Self S32_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S32_LE;
        static constexpr Self S32_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S32_BE;
        static constexpr Self U32_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U32_LE;
        static constexpr Self U32_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U32_BE;
        static constexpr Self FLOAT_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT_LE;
        static constexpr Self FLOAT_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT_BE;
        static constexpr Self FLOAT64_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT64_LE;
        static constexpr Self FLOAT64_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT64_BE;
        static constexpr Self IEC958_SUBFRAME_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_IEC958_SUBFRAME_LE;
        static constexpr Self IEC958_SUBFRAME_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_IEC958_SUBFRAME_BE;
        static constexpr Self MU_LAW = ::snd_pcm_format_t::SND_PCM_FORMAT_MU_LAW;
        static constexpr Self A_LAW = ::snd_pcm_format_t::SND_PCM_FORMAT_A_LAW;
        static constexpr Self IMA_ADPCM = ::snd_pcm_format_t::SND_PCM_FORMAT_IMA_ADPCM;
        static constexpr Self MPEG = ::snd_pcm_format_t::SND_PCM_FORMAT_MPEG;
        static constexpr Self GSM = ::snd_pcm_format_t::SND_PCM_FORMAT_GSM;
        static constexpr Self S20_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S20_LE;
        static constexpr Self S20_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S20_BE;
        static constexpr Self U20_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U20_LE;
        static constexpr Self U20_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U20_BE;
        static constexpr Self SPECIAL = ::snd_pcm_format_t::SND_PCM_FORMAT_SPECIAL;
        static constexpr Self S24_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S24_3LE;
        static constexpr Self S24_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S24_3BE;
        static constexpr Self U24_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U24_3LE;
        static constexpr Self U24_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U24_3BE;
        static constexpr Self S20_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S20_3LE;
        static constexpr Self S20_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S20_3BE;
        static constexpr Self U20_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U20_3LE;
        static constexpr Self U20_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U20_3BE;
        static constexpr Self S18_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_S18_3LE;
        static constexpr Self S18_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_S18_3BE;
        static constexpr Self U18_3LE = ::snd_pcm_format_t::SND_PCM_FORMAT_U18_3LE;
        static constexpr Self U18_3BE = ::snd_pcm_format_t::SND_PCM_FORMAT_U18_3BE;
        static constexpr Self G723_24 = ::snd_pcm_format_t::SND_PCM_FORMAT_G723_24;
        static constexpr Self G723_24_1B = ::snd_pcm_format_t::SND_PCM_FORMAT_G723_24_1B;
        static constexpr Self G723_40 = ::snd_pcm_format_t::SND_PCM_FORMAT_G723_40;
        static constexpr Self G723_40_1B = ::snd_pcm_format_t::SND_PCM_FORMAT_G723_40_1B;
        static constexpr Self DSD_U8 = ::snd_pcm_format_t::SND_PCM_FORMAT_DSD_U8;
        static constexpr Self DSD_U16_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_DSD_U16_LE;
        static constexpr Self DSD_U32_LE = ::snd_pcm_format_t::SND_PCM_FORMAT_DSD_U32_LE;
        static constexpr Self DSD_U16_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_DSD_U16_BE;
        static constexpr Self DSD_U32_BE = ::snd_pcm_format_t::SND_PCM_FORMAT_DSD_U32_BE;

        // CPU-endian aliases - pick the right byte order at compile time.
        static constexpr Self S16 = ::snd_pcm_format_t::SND_PCM_FORMAT_S16;
        static constexpr Self U16 = ::snd_pcm_format_t::SND_PCM_FORMAT_U16;
        static constexpr Self S24 = ::snd_pcm_format_t::SND_PCM_FORMAT_S24;
        static constexpr Self U24 = ::snd_pcm_format_t::SND_PCM_FORMAT_U24;
        static constexpr Self S32 = ::snd_pcm_format_t::SND_PCM_FORMAT_S32;
        static constexpr Self U32 = ::snd_pcm_format_t::SND_PCM_FORMAT_U32;
        static constexpr Self FLOAT = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT;
        static constexpr Self FLOAT64 = ::snd_pcm_format_t::SND_PCM_FORMAT_FLOAT64;
        static constexpr Self IEC958_SUBFRAME = ::snd_pcm_format_t::SND_PCM_FORMAT_IEC958_SUBFRAME;
        static constexpr Self S20 = ::snd_pcm_format_t::SND_PCM_FORMAT_S20;
        static constexpr Self U20 = ::snd_pcm_format_t::SND_PCM_FORMAT_U20;
    private:
        const Self value;
    public:
        constexpr SoundPcmFormat(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmSubformat final {
    public:
        using Self = ::snd_pcm_subformat_t;

        static constexpr Self UNKNOWN = ::snd_pcm_subformat_t::SND_PCM_SUBFORMAT_UNKNOWN;
        static constexpr Self STD = ::snd_pcm_subformat_t::SND_PCM_SUBFORMAT_STD;
        static constexpr Self MSBITS_MAX = ::snd_pcm_subformat_t::SND_PCM_SUBFORMAT_MSBITS_MAX;
        static constexpr Self MSBITS_20 = ::snd_pcm_subformat_t::SND_PCM_SUBFORMAT_MSBITS_20;
        static constexpr Self MSBITS_24 = ::snd_pcm_subformat_t::SND_PCM_SUBFORMAT_MSBITS_24;
    private:
        const Self value;
    public:
        constexpr SoundPcmSubformat(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmState final {
    public:
        using Self = ::snd_pcm_state_t;

        static constexpr Self OPEN = ::snd_pcm_state_t::SND_PCM_STATE_OPEN;
        static constexpr Self SETUP = ::snd_pcm_state_t::SND_PCM_STATE_SETUP;
        static constexpr Self PREPARED = ::snd_pcm_state_t::SND_PCM_STATE_PREPARED;
        static constexpr Self RUNNING = ::snd_pcm_state_t::SND_PCM_STATE_RUNNING;
        static constexpr Self XRUN = ::snd_pcm_state_t::SND_PCM_STATE_XRUN;
        static constexpr Self DRAINING = ::snd_pcm_state_t::SND_PCM_STATE_DRAINING;
        static constexpr Self PAUSED = ::snd_pcm_state_t::SND_PCM_STATE_PAUSED;
        static constexpr Self SUSPENDED = ::snd_pcm_state_t::SND_PCM_STATE_SUSPENDED;
        static constexpr Self DISCONNECTED = ::snd_pcm_state_t::SND_PCM_STATE_DISCONNECTED;
        static constexpr Self PRIVATE1 = ::snd_pcm_state_t::SND_PCM_STATE_PRIVATE1;
    private:
        const Self value;
    public:
        constexpr SoundPcmState(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmStart final {
    public:
        using Self = ::snd_pcm_start_t;

        // NOTE: SND_PCM_START_DATA renames to AUTO here because the bare
        // identifier DATA is a #define in <arpa/tftp.h> (TFTP opcode 3)
        // which is unconditionally pulled in by linux.includes.inl. AUTO
        // also reads cleaner against EXPLICIT.
        static constexpr Self AUTO = ::snd_pcm_start_t::SND_PCM_START_DATA;
        static constexpr Self EXPLICIT = ::snd_pcm_start_t::SND_PCM_START_EXPLICIT;
    private:
        const Self value;
    public:
        constexpr SoundPcmStart(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmXrun final {
    public:
        using Self = ::snd_pcm_xrun_t;

        static constexpr Self NONE = ::snd_pcm_xrun_t::SND_PCM_XRUN_NONE;
        static constexpr Self STOP = ::snd_pcm_xrun_t::SND_PCM_XRUN_STOP;
    private:
        const Self value;
    public:
        constexpr SoundPcmXrun(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmTimestamp final {
    public:
        using Self = ::snd_pcm_tstamp_t;

        static constexpr Self NONE = ::snd_pcm_tstamp_t::SND_PCM_TSTAMP_NONE;
        static constexpr Self ENABLE = ::snd_pcm_tstamp_t::SND_PCM_TSTAMP_ENABLE;
        static constexpr Self MMAP = ::snd_pcm_tstamp_t::SND_PCM_TSTAMP_MMAP; ///< compatibility alias for ENABLE
    private:
        const Self value;
    public:
        constexpr SoundPcmTimestamp(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmTimestampType final {
    public:
        using Self = ::snd_pcm_tstamp_type_t;

        static constexpr Self GETTIMEOFDAY = ::snd_pcm_tstamp_type_t::SND_PCM_TSTAMP_TYPE_GETTIMEOFDAY;
        static constexpr Self MONOTONIC = ::snd_pcm_tstamp_type_t::SND_PCM_TSTAMP_TYPE_MONOTONIC;
        static constexpr Self MONOTONIC_RAW = ::snd_pcm_tstamp_type_t::SND_PCM_TSTAMP_TYPE_MONOTONIC_RAW;
    private:
        const Self value;
    public:
        constexpr SoundPcmTimestampType(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmAudioTimestampType final {
    public:
        using Self = ::snd_pcm_audio_tstamp_type_t;

        static constexpr Self COMPAT = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_COMPAT;
        static constexpr Self DEFAULT = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_DEFAULT;
        static constexpr Self LINK = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_LINK;
        static constexpr Self LINK_ABSOLUTE = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_LINK_ABSOLUTE;
        static constexpr Self LINK_ESTIMATED = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_LINK_ESTIMATED;
        static constexpr Self LINK_SYNCHRONIZED = ::snd_pcm_audio_tstamp_type_t::SND_PCM_AUDIO_TSTAMP_TYPE_LINK_SYNCHRONIZED;
    private:
        const Self value;
    public:
        constexpr SoundPcmAudioTimestampType(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmType final {
    public:
        using Self = ::snd_pcm_type_t;

        static constexpr Self HW = ::snd_pcm_type_t::SND_PCM_TYPE_HW;
        static constexpr Self HOOKS = ::snd_pcm_type_t::SND_PCM_TYPE_HOOKS;
        static constexpr Self MULTI = ::snd_pcm_type_t::SND_PCM_TYPE_MULTI;
        static constexpr Self FILE = ::snd_pcm_type_t::SND_PCM_TYPE_FILE;
        static constexpr Self NULL_TYPE = ::snd_pcm_type_t::SND_PCM_TYPE_NULL;
        static constexpr Self SHM = ::snd_pcm_type_t::SND_PCM_TYPE_SHM;
        static constexpr Self INET = ::snd_pcm_type_t::SND_PCM_TYPE_INET;
        static constexpr Self COPY = ::snd_pcm_type_t::SND_PCM_TYPE_COPY;
        static constexpr Self LINEAR = ::snd_pcm_type_t::SND_PCM_TYPE_LINEAR;
        static constexpr Self ALAW = ::snd_pcm_type_t::SND_PCM_TYPE_ALAW;
        static constexpr Self MULAW = ::snd_pcm_type_t::SND_PCM_TYPE_MULAW;
        static constexpr Self ADPCM = ::snd_pcm_type_t::SND_PCM_TYPE_ADPCM;
        static constexpr Self RATE = ::snd_pcm_type_t::SND_PCM_TYPE_RATE;
        static constexpr Self ROUTE = ::snd_pcm_type_t::SND_PCM_TYPE_ROUTE;
        static constexpr Self PLUG = ::snd_pcm_type_t::SND_PCM_TYPE_PLUG;
        static constexpr Self SHARE = ::snd_pcm_type_t::SND_PCM_TYPE_SHARE;
        static constexpr Self METER = ::snd_pcm_type_t::SND_PCM_TYPE_METER;
        static constexpr Self MIX = ::snd_pcm_type_t::SND_PCM_TYPE_MIX;
        static constexpr Self DROUTE = ::snd_pcm_type_t::SND_PCM_TYPE_DROUTE;
        static constexpr Self LBSERVER = ::snd_pcm_type_t::SND_PCM_TYPE_LBSERVER;
        static constexpr Self LINEAR_FLOAT = ::snd_pcm_type_t::SND_PCM_TYPE_LINEAR_FLOAT;
        static constexpr Self LADSPA = ::snd_pcm_type_t::SND_PCM_TYPE_LADSPA;
        static constexpr Self DMIX = ::snd_pcm_type_t::SND_PCM_TYPE_DMIX;
        static constexpr Self JACK = ::snd_pcm_type_t::SND_PCM_TYPE_JACK;
        static constexpr Self DSNOOP = ::snd_pcm_type_t::SND_PCM_TYPE_DSNOOP;
        static constexpr Self DSHARE = ::snd_pcm_type_t::SND_PCM_TYPE_DSHARE;
        static constexpr Self IEC958 = ::snd_pcm_type_t::SND_PCM_TYPE_IEC958;
        static constexpr Self SOFTVOL = ::snd_pcm_type_t::SND_PCM_TYPE_SOFTVOL;
        static constexpr Self IOPLUG = ::snd_pcm_type_t::SND_PCM_TYPE_IOPLUG;
        static constexpr Self EXTPLUG = ::snd_pcm_type_t::SND_PCM_TYPE_EXTPLUG;
    private:
        const Self value;
    public:
        constexpr SoundPcmType(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SoundPcmHookType final {
    public:
        using Self = ::snd_pcm_hook_type_t;

        static constexpr Self HW_PARAMS = ::snd_pcm_hook_type_t::SND_PCM_HOOK_TYPE_HW_PARAMS;
        static constexpr Self HW_FREE = ::snd_pcm_hook_type_t::SND_PCM_HOOK_TYPE_HW_FREE;
        static constexpr Self CLOSE = ::snd_pcm_hook_type_t::SND_PCM_HOOK_TYPE_CLOSE;
    private:
        const Self value;
    public:
        constexpr SoundPcmHookType(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    // Hooks / scopes
    using SoundPcmHook = ::snd_pcm_hook_t;
    using SoundPcmHookFn = ::snd_pcm_hook_func_t;
    using SoundPcmScope = ::snd_pcm_scope_t;
    using SoundPcmScopeOps = ::snd_pcm_scope_ops_t;

    // Simple PCM helpers
    using SoundSpcmLatency = ::snd_spcm_latency_t;
    using SoundSpcmXrunType = ::snd_spcm_xrun_type_t;
    using SoundSpcmDuplexType = ::snd_spcm_duplex_type_t;

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

    // Read/write - blocking, interleaved
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
    using ::snd_pcm_hw_params_set_channels_near;
    using ::snd_pcm_hw_params_set_buffer_size_near;
    using ::snd_pcm_hw_params_set_period_size_near;
    using ::snd_pcm_hw_params_get_rate;
    using ::snd_pcm_hw_params_get_channels;
    using ::snd_pcm_hw_params_get_buffer_size;
    using ::snd_pcm_hw_params_get_period_size;

    // snd_pcm_info accessors. NOTE: snd_pcm_info_alloca and
    // snd_pcm_hw_params_alloca are #define macros around alloca() - they
    // can't be brought into a namespace via using-declaration. Call them
    // unqualified at the use site, or use the _malloc / _free pair below.
    using ::snd_pcm_info_malloc;
    using ::snd_pcm_info_free;
    using ::snd_pcm_info_sizeof;
    using ::snd_pcm_info_get_device;
    using ::snd_pcm_info_get_subdevice;
    using ::snd_pcm_info_get_stream;
    using ::snd_pcm_info_get_card;
    using ::snd_pcm_info_get_id;
    using ::snd_pcm_info_get_name;
    using ::snd_pcm_info_get_subdevice_name;
    using ::snd_pcm_info_get_class;
    using ::snd_pcm_info_get_subclass;
    using ::snd_pcm_info_get_subdevices_count;
    using ::snd_pcm_info_get_subdevices_avail;
    using ::snd_pcm_info_get_sync;
    using ::snd_pcm_info_set_device;
    using ::snd_pcm_info_set_subdevice;
    using ::snd_pcm_info_set_stream;

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
}
