/**
 * @file core.main.cppm
 * @module core:main
 * @brief Module file for standard library core operations.
 *
 * This file contains everything that is included as an *.inl file.
 */

module;

#include <csignal>

#ifdef __clang__
#include <type_traits>
#endif

#if __has_include(<experimental/contract>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/contract>
#endif

#include "Macros.hpp"

#define STDLIBX_NO_STD_CORE_FEATURES
#define STDLIBX_NO_STD_NO_ALLOC_FEATURES

export module core:main;

import std;

#include "core/array.inl"
#include "core/compare.inl"
#include "core/concepts.inl"
#include "core/contracts.inl"
#include "core/cstddef.inl"
#include "core/exception.inl"
#include "core/functional.inl"
#include "core/initializer_list.inl"
#include "core/limits.inl"
#include "core/optional.inl"
#include "core/pair.inl"
#include "core/sequence.inl"
#include "core/span.inl"
#include "core/system_error.inl"
#include "core/tuple.inl"
#include "core/variant.inl"

#include "core/collections/bitset.inl"

#include "core/fmt/cinttypes.inl"

#include "core/io/ios.inl"
#include "core/io/streambuf.inl"

#include "core/math/algorithm.inl"
#include "core/math/bit.inl"
#include "core/math/cmath.inl"
#include "core/math/complex.inl"
#include "core/math/cstddef.inl"
#include "core/math/numbers.inl"
#include "core/math/numeric.inl"
#include "core/math/ratio.inl"

#include "core/meta/source_location.inl"
#include "core/meta/stacktrace.inl"
#include "core/meta/type_traits.inl"
#include "core/meta/typeindex.inl"
#include "core/meta/typeinfo.inl"

#include "core/iter/iterator.inl"

#include "core/ranges/algorithm.inl"
#include "core/ranges/functional.inl"
#include "core/ranges/generator.inl"
#include "core/ranges/iterator.inl"
#include "core/ranges/ranges.inl"

#include "core/sys/cinttypes.inl"
#include "core/sys/csetjmp.inl"
#include "core/sys/csignal.inl"
#include "core/sys/cstdlib.inl"
#include "core/sys/utility.inl"

#include "core/text/cctype.inl"
#include "core/text/charconv.inl"
#include "core/text/cstdlib.inl"
#include "core/text/string/cstring.inl"
#include "core/text/string/string_view.inl"

#include "core/util/algorithm.inl"
#include "core/util/cstdlib.inl"
#include "core/util/numeric.inl"
#include "core/util/utility.inl"

// appears at end
#include "core/string_view.inl"
#include "core/cstddef.Numbers.inl"
#include "core/expected.inl"
#include "core/tags.inl"
