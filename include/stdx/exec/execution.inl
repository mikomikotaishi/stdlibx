#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T>
    using IsExecutionPolicy = std::is_execution_policy<T>;
}

/**
 * @namespace std::thread::current
 * @brief Namespace for operations on the current thread.
 */
export namespace stdx::thread::current {
    #ifdef __cpp_lib_senders
    using std::this_thread::sync_wait;
    using std::this_thread::sync_wait_with_variant;
    #endif
}

/**
 * @namespace std::exec
 * @brief Wrapper namespace for standard library execution operations.
 */
export namespace stdx::exec {
    using SequencedPolicy = std::execution::sequenced_policy;
    using ParallelPolicy = std::execution::parallel_policy;
    using ParallelUnsequencedPolicy = std::execution::parallel_unsequenced_policy;
    using UnsequencedPolicy = std::execution::unsequenced_policy;

    inline constexpr SequencedPolicy SEQUENCED = std::execution::seq;
    inline constexpr ParallelPolicy PARALLEL = std::execution::par;
    inline constexpr ParallelUnsequencedPolicy PARALLEL_UNSEQUENCED = std::execution::par_unseq;
    inline constexpr UnsequencedPolicy UNSEQUENCED = std::execution::unseq;

    class Policy final {
    public:
        Policy() = delete;

        static constexpr SequencedPolicy Sequenced = SEQUENCED;
        static constexpr ParallelPolicy Parallel = PARALLEL;
        static constexpr ParallelUnsequencedPolicy ParallelUnsequenced = PARALLEL_UNSEQUENCED;
        static constexpr UnsequencedPolicy Unsequenced = UNSEQUENCED;
    };

    #ifdef __cpp_lib_senders
    template <typename QueryTag, typename ValueType>
    using Prop = std::execution::query_result<QueryTag, ValueType>;
    
    template <typename... Envs>
    using Env = std::execution::env<Envs...>;

    class [[nodiscard]] ForwardProgressGuarantee final {
    public:
        using Self = std::execution::forward_progress_guarantee;

        static constexpr Self CONCURRENT = std::execution::forward_progress_guarantee::concurrent;
        static constexpr Self PARALLEL = std::execution::forward_progress_guarantee::parallel;
        static constexpr Self WEAKLY_PARALLEL = std::execution::forward_progress_guarantee::weakly_parallel;
    private:
        const Self value;
    public:
        constexpr ForwardProgressGuarantee(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    template <typename Sch>
    concept Scheduler = std::execution::scheduler<Sch>;

    template <typename S>
    concept Sender = std::execution::sender<S>;

    template <typename S, typename Env = Environment<>>
    concept SenderIn = std::execution::sender_to<S, Env>;

    template <typename S, typename R>
    concept SenderTo = std::execution::sender_to<S, R>;

    template <typename R>
    concept Receiver = std::execution::receiver<R>;

    template <typename R, typename Compl>
    concept ReceiverOf = std::execution::receiver_of<R, Compl>;

    template <typename O>
    concept OperationState = std::execution::operation_state<O>;

    using RunLoop = std::execution::run_loop;
    using DefaultDomain = std::execution::default_domain;

    template <Sender S>
    using TagOf = std::execution::tag_of_t<S>;

    template <
        typename S,
        typename Env = Env<>,
        template <typename...> typename Tpl = Tuple<>,
        template <typename...> typename Var = Variant<>
    >
        requires SenderIn<S, Env>
    using ValueTypesOf = std::execution::value_types_of_t<S, Env, Tpl, Var>;

    template <
        typename S,
        typename Env = Env<>,
        template <typename...> typename Var = Variant<>
    >
        requires SenderIn<S, Env>
    using ErrorTypesOf = std::execution::error_types_of_t<S, Env, Var>;

    template <typename S, typename Env = Env<>>
        requires SenderIn<S, Env>
    inline constexpr bools SendsStopped = std::execution::sends_stopped<S, Env>;

    template <typename... Fns>
    using CompletionSignatures = std::execution::completion_signatures<Fns...>;

    template <typename Prom>
    using WithAwaitableSenders = std::execution::with_awaitable_senders<Prom>;

    using std::execution::transform_sender;
    using std::execution::apply_sender;

    using std::execution::get_env;

    using std::execution::get_domain;
    using std::execution::get_scheduler;
    using std::execution::get_start_scheduler;
    using std::execution::get_delegation_scheduler;
    using std::execution::get_completion_scheduler;
    using std::execution::get_forward_progress_guarantee;

    using std::execution::get_completion_signatures;

    using std::execution::as_awaitable;

    using std::execution::connect;
    using std::execution::start;

    using std::execution::set_value;
    using std::execution::set_error;
    using std::execution::set_stopped;

    using std::execution::just;
    using std::execution::just_error;
    using std::execution::just_stopped;
    using std::execution::read_env;
    using std::execution::schedule;

    using std::execution::starts_on;
    using std::execution::continues_on;
    using std::execution::on;
    using std::execution::schedule_from;
    using std::execution::then;
    using std::execution::upon_error;
    using std::execution::upon_stopped;
    using std::execution::let_value;
    using std::execution::let_error;
    using std::execution::let_stopped;
    using std::execution::bulk;
    using std::execution::bulk_chunked;
    using std::execution::bulk_unchunked;
    using std::execution::when_all;
    using std::execution::when_all_with_variant;
    using std::execution::into_variant;
    using std::execution::stopped_as_optional;
    using std::execution::stopped_as_error;

    // Should be in stdx::core::Ops (originally in std); moved here for now
    using std::execution::forwarding_query;
    using std::execution::get_allocator;
    using std::execution::get_stop_token;
    #endif

    #ifdef __cpp_lib_parallel_scheduler
    using ParallelScheduler = std::execution::parallel_scheduler;

    using std::execution::get_parallel_scheduler;

    namespace replaceability {
        using ParallelScheduler = std::execution::system_context_replaceability::parallel_scheduler;
        using ReceiverProxy = std::execution::system_context_replaceability::receiver_proxy;
        using BulkItemReceiverProxy = std::execution::system_context_replaceability::bulk_item_receiver_proxy;
        using ParallelSchedulerBackend = std::execution::system_context_replaceability::parallel_scheduler_backend;

        using std::execution::system_context_replaceability::query_parallel_scheduler_backend;
    }
    #endif

    #ifdef __cpp_lib_task
    template <typename T, typename Env = Env<>>
    using Task = std::execution::task<T, Env>;

    using TaskScheduler = std::execution::task_scheduler;
    #endif
}
