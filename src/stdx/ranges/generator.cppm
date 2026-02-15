/**
 * @file generator.cppm
 * @module stdx:ranges.generator
 * @brief Module file for standard library generator operations.
 *
 * This file contains the implementation of the generator operations in the standard library.
 */

module;

export module stdx:ranges.generator;

import core;

/**
 * @namespace stdx::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace stdx::ranges {
    #if __has_include(<generator>)
    using core::ranges::Generator;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using core::ranges::pmr::Generator;
    }
    #endif
}
