/**
 * @file initializer_list.cppm
 * @module stdx:core.initializer_list
 * @brief Module file for importing the InitializerList class from the standard library.
 * 
 * This file exports the InitializerList class operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.initializer_list;

import core;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::InitializerList;

    using ::core::begin;
    using ::core::end;
}
