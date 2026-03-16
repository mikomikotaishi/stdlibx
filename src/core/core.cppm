/**
 * @file core.cppm
 * @module core
 * @brief Module file for standard library core operations.
 *
 * This file contains the imports for the core operations in the standard library.
 */

module;

#include "Macros.hpp"

#define STDLIBX_NO_STD_CORE_FEATURES
#define STDLIBX_NO_STD_NO_ALLOC_FEATURES

export module core;

import std;

export import :main;
export import :constants;
export import :gsl;

// Include math, system in module core so that constants declared in core:constants
// can be used in core::prelude::util::Math, core::prelude::util::System
#include "core/math.inl"
#include "core/system.inl"

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
export using namespace core::prelude;
export using namespace core::prelude::util;
#endif

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_LITERALS)
export using namespace core::literals;
#endif
