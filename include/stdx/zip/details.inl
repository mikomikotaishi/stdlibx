#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
namespace stdx::zip {
    [[nodiscard]]
    inline String build_error_message(StringView prefix, const z_stream& stream, i32 code) {
        String text(prefix);
        text += Ops::fmt(" (zlib code {})", code);
        if (stream.msg) {
            text += Ops::fmt(": {}", stream.msg);
        }
        return text;
    }
}
#endif
