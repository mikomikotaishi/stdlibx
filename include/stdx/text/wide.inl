#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::text
 * @brief UTF-16 to UTF-8 conversion, for the Windows wide API surface.
 *
 * Named for the encodings rather than for wchar_t deliberately. wchar_t is two
 * bytes and UTF-16 on Windows but four and UTF-32 on Linux and Darwin, so a
 * to_utf8(WStringView) would silently mean a different conversion per platform -
 * the same class of trap as a hardcoded OS constant. Nothing here exists off
 * Windows, because no other platform this library targets has a UTF-16 API.
 */
export namespace stdx::text {
    #ifdef _WIN32
    /**
     * @brief Converts a UTF-16 sequence to UTF-8.
     * @param data The code units. Need not be NUL-terminated.
     * @param size The number of code units, excluding any terminator.
     * @return The UTF-8 encoding, with U+FFFD in place of anything ill-formed.
     *
     * Lossy by choice. This sits on startup paths - Environment::args is one - and
     * failing a process over one malformed argument is worse than carrying a
     * replacement character through it. A caller that must detect ill-formed input
     * passes win32::WC_ERR_INVALID_CHARS to WideCharToMultiByte directly, which
     * makes the conversion fail rather than substitute.
     */
    [[nodiscard]]
    inline String utf16_to_utf8(const wchar* data, usize size) {
        if (data == nullptr || size == 0) {
            return "";
        }
        const int units = static_cast<int>(size);
        const int bytes = win32::WideCharToMultiByte(
            win32::CP_UTF8, 0, data, units, nullptr, 0, nullptr, nullptr
        );
        if (bytes <= 0) {
            return "";
        }
        String out(static_cast<usize>(bytes), '\0');
        const int written = win32::WideCharToMultiByte(
            win32::CP_UTF8, 0, data, units, out.data(), bytes, nullptr, nullptr
        );
        out.resize(written > 0 ? static_cast<usize>(written) : 0uz);
        return out;
    }

    /**
     * @brief Converts a NUL-terminated UTF-16 string to UTF-8.
     * @param text The string, or nullptr for an empty result.
     * @return The UTF-8 encoding, without the terminator.
     */
    [[nodiscard]]
    inline String utf16_to_utf8(const wchar* text) {
        if (text == nullptr) {
            return "";
        }
        usize size = 0;
        while (text[size] != L'\0') {
            ++size;
        }
        return utf16_to_utf8(text, size);
    }
    #endif
}
