/**
 * @file initializer_list.cppm
 * @module core:initializer_list
 * @brief Module file for importing the InitializerList class from the standard library.
 * 
 * This file exports the InitializerList class operations in the standard library.
 */

module;

#include <initializer_list>

#include "Macros.hpp"

export module core:initializer_list;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    using InitializerList = std::initializer_list<T>;

    using std::begin;
    using std::end;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
