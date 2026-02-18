#pragma once

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

    class [[nodiscard]] Launch final {
    public:
        using Self = std::launch;

        static constexpr Self ASYNC = std::launch::async;
        static constexpr Self DEFERRED = std::launch::deferred;
    private:
        Self value;
    public:
        constexpr Launch(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] FutureStatus final {
    public:
        using Self = std::future_status;

        static constexpr Self READY = std::future_status::ready;
        static constexpr Self TIMEOUT = std::future_status::timeout;
        static constexpr Self DEFERRED = std::future_status::deferred;
    private:
        Self value;
    public:
        constexpr FutureStatus(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] FutureErrc final {
    public:
        using Self = std::future_errc;

        static constexpr Self SUCCESS = std::future_errc();
        static constexpr Self FUTURE_ALREADY_RETRIEVED = std::future_errc::future_already_retrieved;
        static constexpr Self PROMISE_ALREADY_SATISFIED = std::future_errc::promise_already_satisfied;
        static constexpr Self NO_STATE = std::future_errc::no_state;
        static constexpr Self BROKEN_PROMISE = std::future_errc::broken_promise;
    private:
        Self value;
    public:
        constexpr FutureErrc(Self value = SUCCESS) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
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
