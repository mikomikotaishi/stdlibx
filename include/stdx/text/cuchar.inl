#pragma once

/**
 * @namespace stdx::text
 * @brief Standard library text operations.
 */
export namespace stdx::text {
    using std::mbrtoc16;
    using std::c16rtomb;
    using std::mbrtoc32;
    using std::c32rtomb;
    // mbrtoc8/c8rtomb need C-library support that Darwin's libc lacks and
    // libc++ never exposes; libstdc++ advertises it with this config macro.
    #ifdef _GLIBCXX_USE_UCHAR_C8RTOMB_MBRTOC8_CXX20
    using std::mbrtoc8;
    using std::c8rtomb;
    #endif
}
