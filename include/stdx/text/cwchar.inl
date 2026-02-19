#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using ::core::text::MultiByteState;
    using ::core::text::WideCharacterInt;

    using ::core::text::wcscpy;
    using ::core::text::wcsncpy;
    using ::core::text::wcscat;
    using ::core::text::wcsncat;
    using ::core::text::wcsxfrm;
    using ::core::text::wcslen;
    using ::core::text::wcscmp;
    using ::core::text::wcsncmp;
    using ::core::text::wcscoll;
    using ::core::text::wcschr;
    using ::core::text::wcsrchr;
    using ::core::text::wcsspn;
    using ::core::text::wcscspn;
    using ::core::text::wcspbrk;
    using ::core::text::wcsstr;
    using ::core::text::wcstok;
    using ::core::text::wmemcpy;
    using ::core::text::wmemmove;
    using ::core::text::wmemcmp;
    using ::core::text::wmemchr;
    using ::core::text::wmemset;
    using ::core::text::mbsinit;
    using ::core::text::btowc;
    using ::core::text::wctob;
    using ::core::text::mbrlen;
    using ::core::text::mbrtowc;
    using ::core::text::wcrtomb;
    using ::core::text::mbsrtowcs;
    using ::core::text::wcsrtombs;
}
