/**
 * @file constants.cppm
 * @module core:constants
 * @brief Module file for constants of the standard library.
 *
 * This file contains the implementation of constants in the standard library.
 */

module;

#include <cstdio>

export module stdx:constants;

/**
 * @internal
 * @namespace _detail
 * @brief Detail namespace - not to be exported for external use
 */
namespace _detail {
    constexpr auto Eof = EOF;
    constexpr auto FopenMax = FOPEN_MAX;
    constexpr auto FilenameMax = FILENAME_MAX;
    constexpr auto BufSiz = BUFSIZ;
    constexpr auto Iofbf = _IOFBF;
    constexpr auto Iolbf = _IOLBF;
    constexpr auto Ionbf = _IONBF;
    constexpr auto SeekSet = SEEK_SET;
    constexpr auto SeekCur = SEEK_CUR;
    constexpr auto SeekEnd = SEEK_END;
    constexpr auto TmpMax = TMP_MAX;
    constexpr auto Ltmpnam = L_tmpnam; 
}

#undef EOF
#undef FOPEN_MAX
#undef FILENAME_MAX
#undef BUFSIZ
#undef _IOFBF
#undef _IOLBF
#undef _IONBF
#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#undef TMP_MAX
#undef L_tmpnam

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    constexpr auto EOF = _detail::Eof;
    constexpr auto FOPEN_MAX = _detail::FopenMax;
    constexpr auto FILENAME_MAX = _detail::FilenameMax;
    constexpr auto BUFSIZ = _detail::BufSiz;
    constexpr auto _IOFBF = _detail::Iofbf;
    constexpr auto _IOLBF = _detail::Iolbf;
    constexpr auto _IONBF = _detail::Ionbf;
    constexpr auto SEEK_SET = _detail::SeekSet;
    constexpr auto SEEK_CUR = _detail::SeekCur;
    constexpr auto SEEK_END = _detail::SeekEnd;
    constexpr auto TMP_MAX = _detail::TmpMax;
    constexpr auto L_tmpnam = _detail::Ltmpnam;

    using CFilePosition = ::fpos_t;
    using CFilePosition64 = ::fpos64_t;

    FILE* popen(const char* command, const char* mode) noexcept {
        #ifdef _WIN32
        return ::_popen(command, mode);
        #else
        return ::popen(command, mode);
        #endif
    }

    int pclose(FILE* stream) noexcept {
        #ifdef _WIN32
        return ::_pclose(stream);
        #else
        return ::pclose(stream);
        #endif
    }
}