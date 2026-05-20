/**
 * @file stdx.main.cppm
 * @module stdx:main
 * @brief Module file for standard library extensions.
 *
 * This file contains everything that is included as an *.inl file.
 */

module;

// Needed for feature testing macros
#include <version>

#if __has_include(<experimental/buffer>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/buffer>
#endif

#if __has_include(<experimental/executor>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/executor>
#endif

#if __has_include(<experimental/internet>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/internet>
#endif

#if __has_include(<experimental/io_context>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/io_context>
#endif

#if __has_include(<experimental/socket>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/socket>
#endif

#if __has_include(<experimental/timer>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/timer>
#endif

#include "Macros.hpp"

export module stdx:main;

#ifdef STDLIBX_EXPORT_IMPORT_STD
export import std;
#else
import std;
#endif

export import :constants;

import :os;

import core;
import alloc;

STDLIBX_CORE_PREPARE_IMPORT_PRELUDE();
STDLIBX_CORE_PREPARE_IMPORT_LITERALS();
STDLIBX_STDX_PREPARE_IMPORT_CORE();
STDLIBX_STDX_PREPARE_IMPORT_LITERALS();

extern "C" {
    #ifdef __unix__
    extern char** environ;
    #elifdef _WIN32
    extern char** _environ;
    #endif
}

#include "stdx/core/string.inl"
#include "stdx/core/string_view.inl"
#include "stdx/core/any.inl"
#include "stdx/core/array.inl"
#include "stdx/core/clocale.inl"
#include "stdx/core/compare.inl"
#include "stdx/core/concepts.inl"
#include "stdx/core/contracts.inl"
#include "stdx/core/cstddef.inl"
#include "stdx/core/exception.inl"
#include "stdx/core/expected.inl"
#include "stdx/core/functional.inl"
#include "stdx/core/gsl.inl"
#include "stdx/core/initializer_list.inl"
#include "stdx/core/limits.inl"
#include "stdx/core/locale.inl"
#include "stdx/core/mdspan.inl"
#include "stdx/core/optional.inl"
#include "stdx/core/pair.inl"
#include "stdx/core/sequence.inl"
#include "stdx/core/span.inl"
#include "stdx/core/system_error.inl"
#include "stdx/core/tags.inl"
#include "stdx/core/tuple.inl"
#include "stdx/core/variant.inl"

#include "stdx/util/algorithm.inl"
#include "stdx/util/cstdlib.inl"
#include "stdx/util/numeric.inl"
#include "stdx/util/utility.inl"

#include "stdx/alloc/cstdlib.inl"
#include "stdx/alloc/memory.inl"
#include "stdx/alloc/new.inl"
#include "stdx/alloc/scoped_allocator.inl"

#include "stdx/collections/bitset.inl"
#include "stdx/collections/colony.inl"
#include "stdx/collections/deque.inl"
#include "stdx/collections/flat_map.inl"
#include "stdx/collections/flat_set.inl"
#include "stdx/collections/hash_map.inl"
#include "stdx/collections/hash_set.inl"
#include "stdx/collections/linked_list.inl"
#include "stdx/collections/queue.inl"
#include "stdx/collections/singly_linked_list.inl"
#include "stdx/collections/stack.inl"
#include "stdx/collections/static_vector.inl"
#include "stdx/collections/tree_map.inl"
#include "stdx/collections/tree_set.inl"
#include "stdx/collections/valarray.inl"
#include "stdx/collections/vector.inl"

#include "stdx/debug/debugging.inl"
#include "stdx/debug/source_location.inl"
#include "stdx/debug/stacktrace.inl"

#include "stdx/exec/execution.inl"

#include "stdx/fmt/charconv.inl"
#include "stdx/fmt/cinttypes.inl"
#include "stdx/fmt/format.inl"

#include "stdx/core/numbers.inl"

#include "stdx/fmt/printf_format.inl"

#include "stdx/fs/fs.inl"

#include "stdx/future/coroutine.inl"
#include "stdx/future/future.inl"

#include "stdx/text/cctype.inl"
#include "stdx/text/charconv.inl"
#include "stdx/text/cstdlib.inl"
#include "stdx/text/cuchar.inl"
#include "stdx/text/cwchar.inl"
#include "stdx/text/cwctype.inl"
#include "stdx/text/regex.inl"
#include "stdx/text/text_encoding.inl"
#include "stdx/text/string/cstring.inl"
#include "stdx/text/string/string.inl"
#include "stdx/text/string/string_view.inl"

#include "stdx/io/cwchar.inl"
#include "stdx/io/fstream.inl"
#include "stdx/io/iomanip.inl"
#include "stdx/io/ios.inl"
#include "stdx/io/iosfwd.inl"
#include "stdx/io/istream.inl"
#include "stdx/io/ostream.inl"
#include "stdx/io/iostream.inl"
#include "stdx/io/spanstream.inl"
#include "stdx/io/sstream.inl"
#include "stdx/io/streambuf.inl"
#include "stdx/io/string.inl"
#include "stdx/io/syncstream.inl"

#include "stdx/iter/iterator.inl"

#include "stdx/random/random.inl"

#include "stdx/math/algorithm.inl"
#include "stdx/math/bits.inl"
#include "stdx/math/cfenv.inl"
#include "stdx/math/cmath.inl"
#include "stdx/math/complex.inl"
#include "stdx/math/cstddef.inl"
#include "stdx/math/linalg.inl"
#include "stdx/math/numbers.inl"
#include "stdx/math/numeric.inl"
#include "stdx/math/quaternion.inl"
#include "stdx/math/random.inl"
#include "stdx/math/ratio.inl"
#include "stdx/math/simd.inl"

#include "stdx/core/math.inl"

#include "stdx/mem/cstring.inl"
#include "stdx/mem/cwchar.inl"
#include "stdx/mem/memory.inl"
#include "stdx/mem/new.inl"

#include "stdx/io/cstdio.inl"
#include "stdx/io/print.inl"

#include "stdx/meta/type_traits.inl"
#include "stdx/meta/typeindex.inl"
#include "stdx/meta/typeinfo.inl"

#include "stdx/net/buffer.inl"
#include "stdx/net/executor.inl"
#include "stdx/net/internet.inl"
#include "stdx/net/io_context.inl"
#include "stdx/net/socket.inl"
#include "stdx/net/timer.inl"

#include "stdx/pmr/collections.inl"
#include "stdx/pmr/generator.inl"
#include "stdx/pmr/memory.inl"
#include "stdx/pmr/memory_resource.inl"
#include "stdx/pmr/stacktrace.inl"
#include "stdx/pmr/regex.inl"
#include "stdx/pmr/string.inl"

#include "stdx/ranges/algorithm.inl"
#include "stdx/ranges/functional.inl"
#include "stdx/ranges/generator.inl"
#include "stdx/ranges/iterator.inl"
#include "stdx/ranges/memory.inl"
#include "stdx/ranges/ranges.inl"
#include "stdx/ranges/random.inl"

#include "stdx/meta/meta.inl"
#include "stdx/collections/enum_set.inl"
#include "stdx/meta/reflect.inl"

#include "stdx/compiler/annotations.inl"
#include "stdx/compiler/attributes.inl"

#include "stdx/core/random.inl"

#include "stdx/sync/atomic.inl"
#include "stdx/sync/barrier.inl"
#include "stdx/sync/condition_variable.inl"
#include "stdx/sync/latch.inl"
#include "stdx/sync/mutex.inl"
#include "stdx/sync/rcu.inl"
#include "stdx/sync/semaphore.inl"
#include "stdx/sync/shared_mutex.inl"

#include "stdx/sys/cinttypes.inl"
#include "stdx/sys/csetjmp.inl"
#include "stdx/sys/csignal.inl"
#include "stdx/sys/cstdlib.inl"
#include "stdx/sys/utility.inl"

#include "stdx/fs/glob.inl"

#include "stdx/thread/hazard_pointer.inl"
#include "stdx/thread/stop_token.inl"
#include "stdx/thread/thread.inl"

#include "stdx/time/ctime.inl"
#include "stdx/time/time.inl"

#include "stdx/core/ops.inl"
#include "stdx/core/system.inl"
#include "stdx/core/uuid.inl"

#include "stdx/io/scanner.inl"

#include "stdx/linq/linq.inl"

#include "stdx/process/process.inl"

#include "stdx/time/chrono.inl"

#include "stdx/util/argparse.inl"
#include "stdx/util/logging.inl"

#ifndef STDLIBX_EXPORT_IMPORT_STD
#include "operators.inl"
#endif
