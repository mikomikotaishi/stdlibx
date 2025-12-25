/**
 * @file future.cppm
 * @module std:concurrent.future
 * @brief Module file for standard library future operations.
 *
 * This file contains the implementation of the future operations in the standard library.
 */

module;

#include <future>

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:concurrent.future;
#else
export module stdlib:concurrent.future;
#endif

import core;

using core::ErrorCode;
using core::ErrorCondition;

/**
 * @namespace std::concurrent
 * @brief Wrapper namespace for standard library concurrency operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::concurrent {
#else 
export namespace stdlib::concurrent {
#endif
    template <typename T>
    using Promise = ::std::promise<T>;

    template <typename T>
    using PackagedTask = ::std::packaged_task<T>;

    template <typename T>
    using Future = ::std::future<T>;

    template <typename T>
    using SharedFuture = ::std::shared_future<T>;

    class Launch {
    public:
        using Self = ::std::launch;

        Launch() = delete;

        static constexpr Self ASYNC = ::std::launch::async;
        static constexpr Self DEFERRED = ::std::launch::deferred;
    };

    class FutureStatus {
    public:
        using Self = ::std::future_status;

        FutureStatus() = delete;

        static constexpr Self READY = ::std::future_status::ready;
        static constexpr Self TIMEOUT = ::std::future_status::timeout;
        static constexpr Self DEFERRED = ::std::future_status::deferred;
    };

    class FutureErrc {
    public:
        using Self = ::std::future_errc;

        FutureErrc() = delete;

        static constexpr Self FUTURE_ALREADY_RETRIEVED = ::std::future_errc::future_already_retrieved;
        static constexpr Self PROMISE_ALREADY_SATISFIED = ::std::future_errc::promise_already_satisfied;
        static constexpr Self NO_STATE = ::std::future_errc::no_state;
        static constexpr Self BROKEN_PROMISE = ::std::future_errc::broken_promise;
    };

    using FutureException = ::std::future_error;

    template <typename T>
    using IsErrorCodeEnum = ::std::is_error_code_enum<T>;

    using ::std::future_category;
    using ::std::make_error_code;
    using ::std::make_error_condition;

    using ::std::async;
    using ::std::swap;
}

/**
 * @namespace std::core
 * @brief Wrapper namespace for standard library core operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
    using std::concurrent::FutureErrc;

    using std::concurrent::future_category;
    using std::concurrent::make_error_code;
    using std::concurrent::make_error_condition;
}
#else 
export namespace stdlib::core {
    using stdlib::concurrent::FutureErrc;

    using stdlib::concurrent::future_category;
    using stdlib::concurrent::make_error_code;
    using stdlib::concurrent::make_error_condition;
}
#endif

#ifndef STDLIBX_NO_STD
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
