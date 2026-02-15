/**
 * @file tags.cppm
 * @module core:tags
 * @brief Module file for standard library tags aggregation class.
 *
 * This file contains the implementation of the tags aggregation class in the standard library.
 */

module;

#define STDLIBX_NO_STD_FEATURES

export module alloc:tags;

import :collections;
import :mem;

import core;

/**
 * @namespace alloc
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace alloc {
    #include "Tags.inl"
}
