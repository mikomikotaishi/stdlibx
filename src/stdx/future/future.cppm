/**
 * @file future.cppm
 * @module stdx:future.future
 * @brief Module file for standard library future operations.
 *
 * This file contains the implementation of the future operations in the standard library.
 */

module;

#include <future>

#include "Macros.hpp"

export module stdx:future.future;

/**
 * @namespace stdx::future
 * @brief Wrapper namespace for standard library concurrency operations.
 */
export namespace stdx::future {
    template <typename T>
    using Promise = std::promise<T>;

    template <typename T>
    using PackagedTask = std::packaged_task<T>;

    template <typename T>
    using Future = std::future<T>;

    template <typename T>
    using SharedFuture = std::shared_future<T>;

    class Launch {
    public:
        using Self = std::launch;

        Launch() = delete;

        static constexpr Self ASYNC = std::launch::async;
        static constexpr Self DEFERRED = std::launch::deferred;
    };

    class FutureStatus {
    public:
        using Self = std::future_status;

        FutureStatus() = delete;

        static constexpr Self READY = std::future_status::ready;
        static constexpr Self TIMEOUT = std::future_status::timeout;
        static constexpr Self DEFERRED = std::future_status::deferred;
    };

    class FutureErrc {
    public:
        using Self = std::future_errc;

        FutureErrc() = delete;

        static constexpr Self FUTURE_ALREADY_RETRIEVED = std::future_errc::future_already_retrieved;
        static constexpr Self PROMISE_ALREADY_SATISFIED = std::future_errc::promise_already_satisfied;
        static constexpr Self NO_STATE = std::future_errc::no_state;
        static constexpr Self BROKEN_PROMISE = std::future_errc::broken_promise;
    };

    using FutureException = std::future_error;

    template <typename T>
    using IsErrorCodeEnum = std::is_error_code_enum<T>;

    using std::future_category;
    using std::make_error_code;
    using std::make_error_condition;

    using std::async;
    using std::swap;
}

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for standard library core operations.
 */
export namespace stdx::core {
    using stdx::future::FutureErrc;

    using stdx::future::future_category;
    using stdx::future::make_error_code;
    using stdx::future::make_error_condition;
}

#ifndef STDLIBX_NO_STD
STDLIBX_STDX_MODULE_EXPORT_CORE();
#endif
