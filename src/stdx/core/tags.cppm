/**
 * @file tags.cppm
 * @module stdx:core.tags
 * @brief Module file for standard library tags aggregation class.
 *
 * This file contains the implementation of the tags aggregation class in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.tags;

import :sync.mutex;
import :thread.stop_token;
import :time.chrono;

import core;
import alloc;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #include "Tags.inl"
}
