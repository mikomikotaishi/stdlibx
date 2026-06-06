#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief Abstract output sink - alsa/output.h.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundOutput = ::snd_output_t;
    using SoundOutputType = ::snd_output_type_t;

    using ::snd_output_stdio_open;
    using ::snd_output_stdio_attach;
    using ::snd_output_buffer_open;
    using ::snd_output_buffer_string;
    using ::snd_output_buffer_steal;
    using ::snd_output_close;
    using ::snd_output_printf;
    using ::snd_output_vprintf;
    using ::snd_output_puts;
    using ::snd_output_putc;
    using ::snd_output_flush;
    #endif
}
