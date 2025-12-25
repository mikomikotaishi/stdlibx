/**
 * @file pair.cppm
 * @module core:pair
 * @brief Module file for standard library pair operations.
 *
 * This file contains the implementation of the pair operations in the standard library.
 */

module;

#include <utility>

#include "Macros.hpp"

export module core:pair;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T1, typename T2>
    using Pair = std::pair<T1, T2>;

    using std::swap;
    using std::get;

    using std::operator==;
    using std::operator<=>;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
