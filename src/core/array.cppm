/**
 * @file array.cppm
 * @module core:array
 * @brief Module file for standard library array operations.
 *
 * This file contains the implementation of the array operations in the standard library.
 */

module;

#include <array>

#include "Macros.hpp"

export module core:array;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T, std::size_t N>
    using Array = std::array<T, N>;

    using std::operator==;
    using std::operator<=>;
    using std::to_array;
    using std::get;
    
    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::empty;
    using std::data;

    using std::swap;
}

#if defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
