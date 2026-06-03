#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA Topology compiler — alsa/topology.h.
 *        Builds binary topology blobs from text configuration; used by
 *        DSP setup tooling, not typically by audio app code.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/topology.h>)
    using Topology = ::snd_tplg_t;
    using TopologyObjectTemplate = ::snd_tplg_obj_template_t;

    using ::snd_tplg_new;
    using ::snd_tplg_free;
    using ::snd_tplg_load;
    using ::snd_tplg_build_file;
    using ::snd_tplg_verbose;
    using ::snd_tplg_add_object;
    using ::snd_tplg_build;
    using ::snd_tplg_build_bin;
    using ::snd_tplg_set_manifest_data;
    #endif
    #endif
}
