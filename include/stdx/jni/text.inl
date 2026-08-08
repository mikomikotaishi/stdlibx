#pragma once

/**
 * @namespace stdx::jni::details
 * @brief UTF-8 to UTF-16 conversion for the Java string boundary.
 */
namespace stdx::jni::details {
    constexpr ::jchar REPLACEMENT = 0xFFFD;

    /**
     * @brief Converts UTF-8 to UTF-16 code units.
     * @param text The UTF-8 bytes. Need not be NUL-terminated.
     * @return The UTF-16 code units, with U+FFFD in place of anything ill-formed.
     *
     * This exists because JNI's NewStringUTF does not take UTF-8. It takes
     * modified UTF-8, in which U+0000 is two bytes and anything outside the BMP
     * is a CESU-8 surrogate pair rather than a four-byte sequence. Handing it
     * ordinary UTF-8 does not fail; it silently builds the wrong string, so
     * every conversion here goes through NewString with real UTF-16 instead.
     *
     * Lossy by choice, matching stdx::text::utf16_to_utf8: ill-formed input
     * yields U+FFFD rather than an exception. Overlong forms, unpaired
     * surrogates and values above U+10FFFF are all treated as ill-formed.
     */
    [[nodiscard]]
    inline Vector<::jchar> utf8_to_utf16(StringView text) {
        Vector<::jchar> units;
        units.reserve(text.size());

        const u8* data = reinterpret_cast<const u8*>(text.data());
        const usize size = text.size();
        usize index = 0;

        while (index < size) {
            const u8 lead = data[index];
            u32 code = 0;
            usize length = 0;

            if (lead < 0x80) {
                code = lead;
                length = 1;
            } else if ((lead & 0xE0) == 0xC0) {
                code = lead & 0x1Fu;
                length = 2;
            } else if ((lead & 0xF0) == 0xE0) {
                code = lead & 0x0Fu;
                length = 3;
            } else if ((lead & 0xF8) == 0xF0) {
                code = lead & 0x07u;
                length = 4;
            } else {
                units.push_back(REPLACEMENT);
                ++index;
                continue;
            }

            if (index + length > size) {
                units.push_back(REPLACEMENT);
                ++index;
                continue;
            }

            bool well_formed = true;
            for (usize offset = 1; offset < length; ++offset) {
                const u8 continuation = data[index + offset];
                if ((continuation & 0xC0) != 0x80) {
                    well_formed = false;
                    break;
                }
                code = (code << 6) | (continuation & 0x3Fu);
            }

            // Shortest-form minimum for each length, so overlong encodings of
            // NUL and of ASCII cannot smuggle a value past a byte-level check.
            // Qualified: inside stdx::jni, an unqualified JavaArray is the JNI
            // array marker (one parameter), not stdx::core::JavaArray (two).
            constexpr ::stdx::core::Array<u32, 5> MINIMUM = {0, 0, 0x80, 0x800, 0x10000};

            if (!well_formed
                || code < MINIMUM[length]
                || code > 0x10FFFF
                || (code >= 0xD800 && code <= 0xDFFF)) {
                units.push_back(REPLACEMENT);
                ++index;
                continue;
            }

            if (code <= 0xFFFF) {
                units.push_back(static_cast<::jchar>(code));
            } else {
                const u32 scalar = code - 0x10000;
                units.push_back(static_cast<::jchar>(0xD800 + (scalar >> 10)));
                units.push_back(static_cast<::jchar>(0xDC00 + (scalar & 0x3FF)));
            }

            index += length;
        }

        return units;
    }

    /**
     * @brief Converts UTF-16 code units to UTF-8.
     * @param data The code units. Need not be NUL-terminated.
     * @param size The number of code units.
     * @return The UTF-8 encoding, with U+FFFD in place of any unpaired surrogate.
     *
     * Java strings may legally contain unpaired surrogates, which have no UTF-8
     * encoding at all, so this direction is lossy for the same reason as the
     * other one.
     */
    [[nodiscard]]
    inline String utf16_to_utf8(const ::jchar* data, usize size) {
        String out;
        out.reserve(size);

        for (usize index = 0; index < size; ++index) {
            u32 code = data[index];

            if (code >= 0xD800 && code <= 0xDBFF) {
                const bool paired = index + 1 < size
                    && data[index + 1] >= 0xDC00
                    && data[index + 1] <= 0xDFFF;
                if (paired) {
                    code = 0x10000
                        + ((code - 0xD800) << 10)
                        + (data[index + 1] - 0xDC00);
                    ++index;
                } else {
                    code = REPLACEMENT;
                }
            } else if (code >= 0xDC00 && code <= 0xDFFF) {
                code = REPLACEMENT;
            }

            if (code < 0x80) {
                out.push_back(static_cast<char>(code));
            } else if (code < 0x800) {
                out.push_back(static_cast<char>(0xC0 | (code >> 6)));
                out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
            } else if (code < 0x10000) {
                out.push_back(static_cast<char>(0xE0 | (code >> 12)));
                out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
            } else {
                out.push_back(static_cast<char>(0xF0 | (code >> 18)));
                out.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
            }
        }

        return out;
    }
}
