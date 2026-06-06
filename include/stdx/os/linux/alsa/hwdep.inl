#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Hardware-dependent interface - alsa/hwdep.h.
 *        Direct passthrough to device-specific protocols (firmware loaders,
 *        DSP control, etc.).
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundHwDep = ::snd_hwdep_t;
    using SoundHwDepInfo = ::snd_hwdep_info_t;
    using SoundHwDepDspStatus = ::snd_hwdep_dsp_status_t;
    using SoundHwDepDspImage = ::snd_hwdep_dsp_image_t;
    using SoundHwDepType = ::snd_hwdep_type_t;
    using SoundHwDepInterface = ::snd_hwdep_iface_t;

    using ::snd_hwdep_open;
    using ::snd_hwdep_close;
    using ::snd_hwdep_poll_descriptors;
    using ::snd_hwdep_poll_descriptors_count;
    using ::snd_hwdep_poll_descriptors_revents;
    using ::snd_hwdep_nonblock;
    using ::snd_hwdep_info;
    using ::snd_hwdep_dsp_status;
    using ::snd_hwdep_dsp_load;
    using ::snd_hwdep_ioctl;
    using ::snd_hwdep_write;
    using ::snd_hwdep_read;
    #endif
}
