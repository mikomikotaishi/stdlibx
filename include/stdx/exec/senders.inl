#pragma once

/**
 * @namespace std::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    #if defined(__cpp_lib_senders) && !defined(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    // Concepts
    template <typename S>
    concept Sender = std::execution::sender<S>;

    template <typename S, typename... Envs>
    concept SenderIn = std::execution::sender_in<S, Envs...>;

    template <typename S, typename R>
    concept SenderTo = std::execution::sender_to<S, R>;

    template <typename S, typename SetSig, typename... Envs>
    concept SenderOf = std::execution::sender_of<S, SetSig, Envs...>;

    template <typename R>
    concept Receiver = std::execution::receiver<R>;

    template <typename R, typename Compl>
    concept ReceiverOf = std::execution::receiver_of<R, Compl>;

    template <typename Op>
    concept OperationState = std::execution::operation_state<Op>;

    template <typename Sch>
    concept Scheduler = std::execution::scheduler<Sch>;

    template <typename Tok>
    concept ScopeToken = std::execution::scope_token<Tok>;

    template <typename Assoc>
    concept ScopeAssociation = std::execution::scope_association<Assoc>;

    template <typename Tok>
    concept StoppableToken = std::execution::stoppable_token<Tok>;

    template <typename Tok>
    concept UnstoppableToken = std::execution::unstoppable_token<Tok>;

    // Environments
    template <typename... Envs>
    using Env = std::execution::env<Envs...>;

    template <typename Qur, typename V>
    using Prop = std::execution::prop<Qur, V>;

    // Tags
    using SenderTag = std::execution::sender_tag;
    using ReceiverTag = std::execution::receiver_tag;
    using OperationStateTag = std::execution::operation_state_tag;
    using SchedulerTag = std::execution::scheduler_tag;

    // Core classes
    using RunLoop = std::execution::run_loop;
    using DefaultDomain = std::execution::default_domain;
    using ForwardProgressGuarantee = std::execution::forward_progress_guarantee;

    template <typename Derived>
    using SenderAdaptorClosure = std::execution::sender_adaptor_closure<Derived>;

    // Stop tokens
    using NeverStopToken = std::execution::never_stop_token;
    using InplaceStopToken = std::execution::inplace_stop_token;
    using InplaceStopSource = std::execution::inplace_stop_source;

    template <typename Fn>
    using InplaceStopCallback = std::execution::inplace_stop_callback<Fn>;

    template <typename Tok, typename Fn>
    using StopCallbackFor = std::execution::stop_callback_for_t<Tok, Fn>;

    // Sender traits
    template <
        typename S,
        typename Envir = Env<>,
        template <typename...> typename Tpl = Tuple,
        template <typename...> typename Var = Variant
    >
        requires SenderIn<S, Envir>
    using ValueTypesOf = std::execution::value_types_of_t<S, Envir, Tpl, Var>;

    template <
        typename S,
        typename Envir = Env<>,
        template <typename...> typename Var = Variant
    >
        requires SenderIn<S, Envir>
    using ErrorTypesOf = std::execution::error_types_of_t<S, Envir, Var>;

    template <typename S, typename... Envs>
        requires SenderIn<S, Envs...>
    using CompletionSignaturesOf = std::execution::completion_signatures_of_t<S, Envs...>;

    template <typename... Fns>
    using CompletionSignatures = std::execution::completion_signatures<Fns...>;

    template <typename EnvProvider>
    using EnvOf = std::execution::env_of_t<EnvProvider>;

    template <typename S>
    using TagOf = std::execution::tag_of_t<S>;

    template <typename S, typename R>
    using ConnectResult = std::execution::connect_result_t<S, R>;

    template <typename Sch>
    using ScheduleResult = std::execution::schedule_result_t<Sch>;

    template <typename E>
    using StopTokenOf = std::execution::stop_token_of_t<E>;

    template <typename S, typename Envir = Env<>>
        requires SenderIn<S, Envir>
    inline constexpr bool SendsStopped = std::execution::sends_stopped<S, Envir>;

    template <typename S>
    inline constexpr bool EnableSender = std::execution::enable_sender<S>;

    template <typename Prom>
    using WithAwaitableSenders = std::execution::with_awaitable_senders<Prom>;

    namespace types {
        // Customization points
        using Connect = std::execution::connect_t;
        using Start = std::execution::start_t;
        using SetValue = std::execution::set_value_t;
        using SetError = std::execution::set_error_t;
        using SetStopped = std::execution::set_stopped_t;
        using GetEnv = std::execution::get_env_t;

        // Queries
        using GetScheduler = std::execution::get_scheduler_t;
        using GetDelegationScheduler = std::execution::get_delegation_scheduler_t;
        using GetAllocator = std::execution::get_allocator_t;
        using GetStopToken = std::execution::get_stop_token_t;
        using GetDomain = std::execution::get_domain_t;
        using GetForwardProgressGuarantee = std::execution::get_forward_progress_guarantee_t;
        using ForwardingQuery = std::execution::forwarding_query_t;

        template <typename Qur>
        using GetCompletionScheduler = std::execution::get_completion_scheduler_t<Qur>;

        // Sender factories
        using Just = std::execution::just_t;
        using JustError = std::execution::just_error_t;
        using JustStopped = std::execution::just_stopped_t;
        using ReadEnv = decltype(std::execution::read_env);
        using Schedule = std::execution::schedule_t;

        // Sender adaptors
        using Then = std::execution::then_t;
        using UponError = std::execution::upon_error_t;
        using UponStopped = std::execution::upon_stopped_t;
        using LetValue = std::execution::let_value_t;
        using LetError = std::execution::let_error_t;
        using LetStopped = std::execution::let_stopped_t;

        // Scheduling adaptors
        using StartsOn = std::execution::starts_on_t;
        using ContinuesOn = std::execution::continues_on_t;
        using On = std::execution::on_t;

        // Composition adaptors
        using WhenAll = std::execution::when_all_t;
        using WhenAllWithVariant = std::execution::when_all_with_variant_t;
        using IntoVariant = std::execution::into_variant_t;

        // Parallel-loop adaptors
        using Bulk = std::execution::bulk_t;
        using BulkChunked = std::execution::bulk_chunked_t;
        using BulkUnchunked = std::execution::bulk_unchunked_t;

        // Stopped-channel translator adaptors
        using StoppedAsError = std::execution::stopped_as_error_t;
        using StoppedAsOptional = std::execution::stopped_as_optional_t;

        // Environment adaptors
        using WriteEnv = decltype(std::execution::write_env);

        // Coroutine utilities
        using AsAwaitable = std::execution::as_awaitable_t;

        // Sender consumers
        using SyncWait = std::execution::sync_wait_t;
        using SyncWaitWithVariant = std::execution::sync_wait_with_variant_t;
        using Spawn = std::execution::spawn_t;
        using SpawnFuture = std::execution::spawn_future_t;
    }

    // Customization points
    inline constexpr types::Connect Connect = std::execution::connect;
    inline constexpr types::Start Start = std::execution::start;
    inline constexpr types::SetValue SetValue = std::execution::set_value;
    inline constexpr types::SetError SetError = std::execution::set_error;
    inline constexpr types::SetStopped SetStopped = std::execution::set_stopped;
    inline constexpr types::GetEnv GetEnv = std::execution::get_env;

    // Queries
    inline constexpr types::GetScheduler GetScheduler = std::execution::get_scheduler;
    inline constexpr types::GetDelegationScheduler GetDelegationScheduler = std::execution::get_delegation_scheduler;
    inline constexpr types::GetAllocator GetAllocator = std::execution::get_allocator;
    inline constexpr types::GetStopToken GetStopToken = std::execution::get_stop_token;
    inline constexpr types::GetDomain GetDomain = std::execution::get_domain;
    inline constexpr types::GetForwardProgressGuarantee GetForwardProgressGuarantee = std::execution::get_forward_progress_guarantee;
    inline constexpr types::ForwardingQuery ForwardingQuery = std::execution::forwarding_query;

    template <typename Qur>
    inline constexpr types::GetCompletionScheduler<Query> GetCompletionScheduler = std::execution::get_completion_scheduler<Qur>;

    // Sender factories
    inline constexpr types::Just Just = std::execution::just;
    inline constexpr types::JustError JustError = std::execution::just_error;
    inline constexpr types::JustStopped JustStopped = std::execution::just_stopped;
    inline constexpr types::ReadEnv ReadEnv = std::execution::read_env;
    inline constexpr types::Schedule Schedule = std::execution::schedule;

    // Sender adaptors
    inline constexpr types::Then Then = std::execution::then;
    inline constexpr types::UponError UponError = std::execution::upon_error;
    inline constexpr types::UponStopped UponStopped = std::execution::upon_stopped;
    inline constexpr types::LetValue LetValue = std::execution::let_value;
    inline constexpr types::LetError LetError = std::execution::let_error;
    inline constexpr types::LetStopped LetStopped = std::execution::let_stopped;

    // Scheduling adaptors
    inline constexpr types::StartsOn StartsOn = std::execution::starts_on;
    inline constexpr types::ContinuesOn ContinuesOn = std::execution::continues_on;
    inline constexpr types::On On = std::execution::on;

    // Composition adaptors
    inline constexpr types::WhenAll WhenAll = std::execution::when_all;
    inline constexpr types::WhenAllWithVariant WhenAllWithVariant = std::execution::when_all_with_variant;
    inline constexpr types::IntoVariant IntoVariant = std::execution::into_variant;

    // Parallel-loop adaptors
    inline constexpr types::Bulk Bulk = std::execution::bulk;
    inline constexpr types::BulkChunked BulkChunked = std::execution::bulk_chunked;
    inline constexpr types::BulkUnchunked BulkUnchunked = std::execution::bulk_unchunked;

    // Stopped-channel translator adaptors
    inline constexpr types::StoppedAsError StoppedAsError = std::execution::stopped_as_error;
    inline constexpr types::StoppedAsOptional StoppedAsOptional = std::execution::stopped_as_optional;

    // Environment adaptors
    inline constexpr types::WriteEnv WriteEnv = std::execution::write_env;

    // Coroutine utilities
    inline constexpr types::AsAwaitable AsAwaitable = std::execution::as_awaitable;

    // Sender consumers
    inline constexpr types::SyncWait SyncWait = std::execution::sync_wait;
    inline constexpr types::SyncWaitWithVariant SyncWaitWithVariant = std::execution::sync_wait_with_variant;
    inline constexpr types::Spawn Spawn = std::execution::spawn;
    inline constexpr types::SpawnFuture SpawnFuture = std::execution::spawn_future;
    #endif
}
