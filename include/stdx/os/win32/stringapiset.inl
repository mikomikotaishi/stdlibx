#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Code page conversion between UTF-16 and the narrow encodings (stringapiset.h).
 *
 * Windows is the only platform this library targets whose native API is UTF-16, so
 * every wide Win32 result has to pass through one of these to become a stdx String.
 * stdx::text::utf16_to_utf8 is the wrapper that does it; these are the raw entry
 * points, exported here because a String is not available on the stdx:os side.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<stringapiset.h>)
    using ::WideCharToMultiByte;
    using ::MultiByteToWideChar;
    #endif
}
