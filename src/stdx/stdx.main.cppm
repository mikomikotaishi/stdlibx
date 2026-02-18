/**
 * @file stdx.main.cppm
 * @module stdx:main
 * @brief Module file for standard library extensions.
 *
 * This file contains everything that is included as an *.inl file.
 */

module;

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

import std;

export import :constants;

import core;
import alloc;

STDLIBX_PREPARE_EXPORT_CORE();

#include "stdx/core/string.inl"
#include "stdx/core/string_view.inl"
#include "stdx/core/any.inl"
#include "stdx/core/array.inl"
#include "stdx/core/compare.inl"
#include "stdx/core/concepts.inl"
#include "stdx/core/contracts.inl"
#include "stdx/core/cstddef.inl"
#include "stdx/core/exception.inl"
#include "stdx/core/expected.inl"
#include "stdx/core/functional.inl"
#include "stdx/core/initializer_list.inl"
#include "stdx/core/limits.inl"
#include "stdx/core/optional.inl"
#include "stdx/core/pair.inl"
#include "stdx/core/sequence.inl"
#include "stdx/core/span.inl"
#include "stdx/core/system_error.inl"
#include "stdx/core/tuple.inl"
#include "stdx/core/variant.inl"

#include "stdx/collections/bitset.inl"
#include "stdx/collections/deque.inl"
#include "stdx/collections/flat_map.inl"
#include "stdx/collections/flat_set.inl"
#include "stdx/collections/hash_map.inl"
#include "stdx/collections/hash_set.inl"
#include "stdx/collections/linked_list.inl"
#include "stdx/collections/queue.inl"
#include "stdx/collections/singly_linked_list.inl"
#include "stdx/collections/stack.inl"
#include "stdx/collections/tree_map.inl"
#include "stdx/collections/tree_set.inl"
#include "stdx/collections/valarray.inl"
#include "stdx/collections/vector.inl"

#include "stdx/execution/execution.inl"

#include "stdx/fmt/charconv.inl"
#include "stdx/fmt/cinttypes.inl"
#include "stdx/fmt/format.inl"

#include "stdx/core/cstddef.Numbers.inl"

#include "stdx/fs/fs.inl"

#include "stdx/future/coroutine.inl"
#include "stdx/future/future.inl"

#include "stdx/io/cwchar.inl"
#include "stdx/io/fstream.inl"
#include "stdx/io/iomanip.inl"
#include "stdx/io/ios.inl"
#include "stdx/io/istream.inl"
#include "stdx/io/ostream.inl"
#include "stdx/io/iostream.inl"
#include "stdx/io/print.inl"
#include "stdx/io/spanstream.inl"
#include "stdx/io/sstream.inl"
#include "stdx/io/streambuf.inl"
#include "stdx/io/string.inl"
#include "stdx/io/syncstream.inl"

#include "stdx/iter/iterator.inl"

#include "stdx/random/random.inl"

#include "stdx/math/algorithm.inl"
#include "stdx/math/bits.inl"
#include "stdx/math/cmath.inl"
#include "stdx/math/complex.inl"
#include "stdx/math/cstddef.inl"
#include "stdx/math/numbers.inl"
#include "stdx/math/numeric.inl"
#include "stdx/math/random.inl"
#include "stdx/math/ratio.inl"

#include "stdx/mem/cstdlib.inl"
#include "stdx/mem/memory.inl"
#include "stdx/mem/memory_resource.inl"
#include "stdx/mem/new.inl"
#include "stdx/mem/scoped_allocator.inl"

#include "stdx/io/cstdio.inl"

#include "stdx/meta/source_location.inl"
#include "stdx/meta/stacktrace.inl"
#include "stdx/meta/type_traits.inl"
#include "stdx/meta/typeindex.inl"
#include "stdx/meta/typeinfo.inl"

#include "stdx/net/buffer.inl"
#include "stdx/net/executor.inl"
#include "stdx/net/internet.inl"
#include "stdx/net/io_context.inl"
#include "stdx/net/socket.inl"
#include "stdx/net/timer.inl"

#include "stdx/ranges/algorithm.inl"
#include "stdx/ranges/functional.inl"
#include "stdx/ranges/generator.inl"
#include "stdx/ranges/iterator.inl"
#include "stdx/ranges/memory.inl"
#include "stdx/ranges/ranges.inl"

#include "stdx/sync/atomic.inl"
#include "stdx/sync/barrier.inl"
#include "stdx/sync/condition_variable.inl"
#include "stdx/sync/latch.inl"
#include "stdx/sync/mutex.inl"
#include "stdx/sync/semaphore.inl"
#include "stdx/sync/shared_mutex.inl"

#include "stdx/sys/cinttypes.inl"
#include "stdx/sys/csetjmp.inl"
#include "stdx/sys/csignal.inl"
#include "stdx/sys/cstdlib.inl"
#include "stdx/sys/utility.inl"

#include "stdx/text/cctype.inl"
#include "stdx/text/charconv.inl"
#include "stdx/text/cstdlib.inl"
#include "stdx/text/regex.inl"
#include "stdx/text/string/cstring.inl"
#include "stdx/text/string/string.inl"
#include "stdx/text/string/string_view.inl"

#include "stdx/thread/stop_token.inl"
#include "stdx/thread/thread.inl"

#include "stdx/time/chrono.inl"
#include "stdx/time/ctime.inl"

#include "stdx/util/algorithm.inl"
#include "stdx/util/cstdlib.inl"
#include "stdx/util/numeric.inl"
#include "stdx/util/utility.inl"

#include "stdx/core/tags.inl"
