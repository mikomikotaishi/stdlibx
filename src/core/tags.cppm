/**
 * @file tags.cppm
 * @module core:tags
 * @brief Module file for standard library tags aggregation class.
 *
 * This file contains the implementation of the tags aggregation class in the standard library.
 */

module;

#define STDLIBX_NO_STD_NO_ALLOC_FEATURES

export module core:tags;

import :cstddef;
import :expected;
import :ranges.ranges;
import :util.utility;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #include "Tags.inl"
}
