#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using ::core::text::iswalnum;
    using ::core::text::iswalpha;
    using ::core::text::iswlower;
    using ::core::text::iswupper;
    using ::core::text::iswdigit;
    using ::core::text::iswxdigit;
    using ::core::text::iswcntrl;
    using ::core::text::iswgraph;
    using ::core::text::iswspace;
    using ::core::text::iswblank;
    using ::core::text::iswprint;
    using ::core::text::iswpunct;
    using ::core::text::iswctype;
    using ::core::text::wctype;
    using ::core::text::towlower;
    using ::core::text::towupper;
    using ::core::text::towctrans;
    using ::core::text::wctrans;
}
