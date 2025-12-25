/**
 * @file generator.cppm
 * @module std:ranges.generator
 * @brief Module file for standard library generator operations.
 *
 * This file contains the implementation of the generator operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:ranges.generator;
#else
export module stdlib:ranges.generator;
#endif

import core;

/**
 * @namespace std::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::ranges {
#else 
export namespace stdlib::ranges {
#endif
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
