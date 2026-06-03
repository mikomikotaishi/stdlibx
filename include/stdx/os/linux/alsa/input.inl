#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Abstract input source for config parsers — alsa/input.h.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    using Input = ::snd_input_t;
    using InputType = ::snd_input_type_t;

    using ::snd_input_stdio_open;
    using ::snd_input_stdio_attach;
    using ::snd_input_buffer_open;
    using ::snd_input_close;
    using ::snd_input_scanf;
    using ::snd_input_gets;
    using ::snd_input_getc;
    using ::snd_input_ungetc;
    #endif
    #endif
}
