#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using ::core::text::mbrtoc16;
    using ::core::text::c16rtomb;
    using ::core::text::mbrtoc32;
    using ::core::text::c32rtomb;
    using ::core::text::mbrtoc8;
    using ::core::text::c8rtomb;
}
