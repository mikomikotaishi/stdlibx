#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Abstract input source for config parsers - alsa/input.h.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundInput = ::snd_input_t;
    using SoundInputType = ::snd_input_type_t;

    using ::snd_input_stdio_open;
    using ::snd_input_stdio_attach;
    using ::snd_input_buffer_open;
    using ::snd_input_close;
    using ::snd_input_scanf;
    using ::snd_input_gets;
    using ::snd_input_getc;
    using ::snd_input_ungetc;
    #endif
}
