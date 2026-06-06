#pragma once

/**
 * @namespace stdx::future
 * @brief Standard library concurrency operations.
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

    class [[nodiscard]] LaunchPolicy final {
    public:
        using Self = std::launch;

        static constexpr Self ASYNC = std::launch::async;
        static constexpr Self DEFERRED = std::launch::deferred;
    private:
        const Self value;
    public:
        constexpr LaunchPolicy() noexcept = delete;

        constexpr LaunchPolicy(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
        const Self value;
    public:
        constexpr FutureStatus() noexcept = delete;

        constexpr FutureStatus(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
 * @brief Standard library core operations.
 */
export namespace stdx::core {
    using stdx::future::future_category;
    using stdx::future::make_error_code;
    using stdx::future::make_error_condition;
}
