#pragma once

using stdx::text::CharTraits;

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicOutputStream = std::basic_ostream<Char, Traits>;

    using OutputStream = std::ostream;
    using WideOutputStream = std::wostream;

    using std::operator<<;

    using std::endl;
    using std::ends;
    using std::flush;
    using std::emit_on_flush;
    using std::noemit_on_flush;
    using std::flush_emit;
}
