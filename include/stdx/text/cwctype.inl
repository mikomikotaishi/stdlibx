#pragma once

/**
 * @namespace stdx::text
 * @brief Standard library text operations.
 */
export namespace stdx::text {
    using std::iswalnum;
    using std::iswalpha;
    using std::iswlower;
    using std::iswupper;
    using std::iswdigit;
    using std::iswxdigit;
    using std::iswcntrl;
    using std::iswgraph;
    using std::iswspace;
    using std::iswblank;
    using std::iswprint;
    using std::iswpunct;
    using std::iswctype;
    using std::wctype;
    using std::towlower;
    using std::towupper;
    using std::towctrans;
    using std::wctrans;
}
