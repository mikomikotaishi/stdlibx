#pragma once

/**
 * @namespace core::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace core::text {
    using std::mbrtoc16;
    using std::c16rtomb;
    using std::mbrtoc32;
    using std::c32rtomb;
    using std::mbrtoc8;
    using std::c8rtomb;
}
