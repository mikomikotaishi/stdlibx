#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using std::mbrtoc16;
    using std::c16rtomb;
    using std::mbrtoc32;
    using std::c32rtomb;
    using std::mbrtoc8;
    using std::c8rtomb;
}
