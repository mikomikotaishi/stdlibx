#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using ::core::text::atof;
    using ::core::text::atoi;
    using ::core::text::atol;
    using ::core::text::atoll;
    using ::core::text::strtol;
    using ::core::text::strtoll;
    using ::core::text::strtoul;
    using ::core::text::strtoull;
    using ::core::text::strtof;
    using ::core::text::strtod;
    using ::core::text::strtold;
    using ::core::text::mblen;
    using ::core::text::mbtowc;
    using ::core::text::wctomb;
    using ::core::text::mbstowcs;
    using ::core::text::wcstombs;
}
