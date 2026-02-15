/**
 * @file executor.cppm
 * @module stdx:net.executor
 * @brief Module file for (experimental) standard library networking executor operations.
 *
 * This file contains the implementation of the (experimental) networking executor operations in the standard library.
 */

module;

#if __has_include(<experimental/executor>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/executor>
#endif

export module stdx:net.executor;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/executor>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
    template <typename CompletionToken, typename Signature>
    using AsyncResult = std::experimental::net::async_result<CompletionToken, Signature>;

    template <typename CompletionToken, typename Signature>
    using AsyncCompletion = std::experimental::net::async_completion<CompletionToken, Signature>;

    using Executor = std::experimental::net::executor;

    template <typename T, typename Executor>
    using ExecutorBinder = std::experimental::net::executor_binder<T, Executor>;

    using ExecutionContext = std::experimental::net::execution_context;
    using ExecutionContextService = std::experimental::net::execution_context::service;
    using SystemExecutor = std::experimental::net::system_executor;

    template <typename Executor>
    using ExecutorWorkGuard = std::experimental::net::executor_work_guard<Executor>;

    class [[nodiscard]] ForkEvent final {
    public:
        using Self = std::experimental::net::fork_event;

        static constexpr Self PREPARE = std::experimental::net::fork_event::prepare;
        static constexpr Self PARENT = std::experimental::net::fork_event::parent;
        static constexpr Self CHILD = std::experimental::net::fork_event::child;
    private:
        Self value;
    public:
        constexpr ForkEvent(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    template <typename Executor>
    using Strand = std::experimental::net::strand<Executor>;

    using SystemContext = std::experimental::net::system_context;
    using SystemExecutor = std::experimental::net::system_executor;

    using BadExecutorException = std::experimental::net::bad_executor;
    using ServiceAlreadyExistsException = std::experimental::net::service_already_exists;

    template <typename T, typename Executor = SystemExecutor>
    using AssociatedExecutor = std::experimental::net::associated_executor<T, Executor>;

    template <typename T>
    using IsExecutor = std::experimental::net::is_executor<T>;

    template <typename T>
    constexpr bool IsExecutorValue = std::experimental::net::is_executor_v<T>;
    
    template <typename T, typename Executor>
    using UsesExecutor = std::experimental::net::uses_executor<T, Executor>;

    template <typename T, typename Executor>
    constexpr bool UsesExecutorValue = std::experimental::net::uses_executor_v<T, Executor>;

    using ExecutorArgumentTag = std::experimental::net::executor_arg_t;
    inline constexpr ExecutorArgumentTag ExecutorArg = std::experimental::net::executor_arg;
    
    using std::experimental::net::swap;
    using std::experimental::net::make_work_guard;
    using std::experimental::net::bind_executor;
    using std::experimental::net::get_associated_executor;
    using std::experimental::net::get_associated_allocator;
    using std::experimental::net::post;
    using std::experimental::net::defer;
    using std::experimental::net::use_service;
    using std::experimental::net::make_service;
    using std::experimental::net::has_service;

    using std::experimental::net::operator==;
    using std::experimental::net::operator!=;
    #endif
}
