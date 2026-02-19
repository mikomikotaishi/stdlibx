#pragma once

/**
 * @namespace core::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace core::text {
    using MultiByteState = std::mbstate_t;
    using WideCharacterInt = std::wint_t;

    using std::wcscpy;
    using std::wcsncpy;
    using std::wcscat;
    using std::wcsncat;
    using std::wcsxfrm;
    using std::wcslen;
    using std::wcscmp;
    using std::wcsncmp;
    using std::wcscoll;
    using std::wcschr;
    using std::wcsrchr;
    using std::wcsspn;
    using std::wcscspn;
    using std::wcspbrk;
    using std::wcsstr;
    using std::wcstok;
    using std::wmemcpy;
    using std::wmemmove;
    using std::wmemcmp;
    using std::wmemchr;
    using std::wmemset;
    using std::mbsinit;
    using std::btowc;
    using std::wctob;
    using std::mbrlen;
    using std::mbrtowc;
    using std::wcrtomb;
    using std::mbsrtowcs;
    using std::wcsrtombs;
}
