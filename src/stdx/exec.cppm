/**
 * @file exec.cppm
 * @module stdx:exec
 * @brief Import of execution modules.
 *
 * This file imports the modules for the execution handling library.
 * This is built only when NVIDIA stdexec is available, and is not
 * used when the official std::execution senders library (C++26) is available.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY
#include <stdexec/execution.hpp>
#endif

export module stdx:exec;

import :foundation;

#ifdef __GNUC__
using namespace stdx::core;
#endif

export namespace stdx::exec {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY
    // Concepts
    template <typename S>
    concept Sender = stdexec::sender<S>;

    template <typename S, typename... Envs>
    concept SenderIn = stdexec::sender_in<S, Envs...>;

    template <typename S, typename R>
    concept SenderTo = stdexec::sender_to<S, R>;

    template <typename S, typename SetSig, typename... Envs>
    concept SenderOf = stdexec::sender_of<S, SetSig, Envs...>;

    template <typename R>
    concept Receiver = stdexec::receiver<R>;

    template <typename R, typename Compl>
    concept ReceiverOf = stdexec::receiver_of<R, Compl>;

    template <typename Op>
    concept OperationState = stdexec::operation_state<Op>;

    template <typename Sch>
    concept Scheduler = stdexec::scheduler<Sch>;

    template <typename Tok>
    concept ScopeToken = stdexec::scope_token<Tok>;

    template <typename Assoc>
    concept ScopeAssociation = stdexec::scope_association<Assoc>;

    template <typename Tok>
    concept StoppableToken = stdexec::stoppable_token<Tok>;

    template <typename Tok>
    concept UnstoppableToken = stdexec::unstoppable_token<Tok>;

    // Environments
    template <typename... Envs>
    using Env = stdexec::env<Envs...>;

    template <typename Qry, typename V>
    using Prop = stdexec::prop<Qry, V>;

    // Tags
    using SenderTag = stdexec::sender_tag;
    using ReceiverTag = stdexec::receiver_tag;
    using OperationStateTag = stdexec::operation_state_tag;
    using SchedulerTag = stdexec::scheduler_tag;

    // Core classes
    using RunLoop = stdexec::run_loop;
    using DefaultDomain = stdexec::default_domain;
    using TaskScheduler = stdexec::task_scheduler;
    using ForwardProgressGuarantee = stdexec::forward_progress_guarantee;

    template <typename Derived>
    using SenderAdaptorClosure = stdexec::sender_adaptor_closure<Derived>;

    // Stop tokens
    using NeverStopToken = stdexec::never_stop_token;
    using InplaceStopToken = stdexec::inplace_stop_token;
    using InplaceStopSource = stdexec::inplace_stop_source;

    template <typename Fn>
    using InplaceStopCallback = stdexec::inplace_stop_callback<Fn>;

    template <typename Tok, typename Fn>
    using StopCallbackFor = stdexec::stop_callback_for_t<Tok, Fn>;

    // Sender traits
    template <
        typename S,
        typename Envir = Env<>,
        template <typename...> typename Tpl = Tuple,
        template <typename...> typename Var = Variant
    >
        requires SenderIn<S, Envir>
    using ValueTypesOf = stdexec::value_types_of_t<S, Envir, Tpl, Var>;

    template <
        typename S,
        typename Envir = Env<>,
        template <typename...> typename Var = Variant
    >
        requires SenderIn<S, Envir>
    using ErrorTypesOf = stdexec::error_types_of_t<S, Envir, Var>;

    template <typename S, typename... Envs>
        requires SenderIn<S, Envs...>
    using CompletionSignaturesOf = stdexec::completion_signatures_of_t<S, Envs...>;

    template <typename... Fns>
    using CompletionSignatures = stdexec::completion_signatures<Fns...>;

    template <typename EnvProvider>
    using EnvOf = stdexec::env_of_t<EnvProvider>;

    template <typename S>
    using TagOf = stdexec::tag_of_t<S>;

    template <typename S, typename R>
    using ConnectResult = stdexec::connect_result_t<S, R>;

    template <typename Sch>
    using ScheduleResult = stdexec::schedule_result_t<Sch>;

    template <typename E>
    using StopTokenOf = stdexec::stop_token_of_t<E>;

    template <typename S, typename Envir = Env<>>
        requires SenderIn<S, Envir>
    inline constexpr bool SendsStopped = stdexec::sends_stopped<S, Envir>;

    template <typename S>
    inline constexpr bool EnableSender = stdexec::enable_sender<S>;

    template <typename Prom>
    using WithAwaitableSenders = stdexec::with_awaitable_senders<Prom>;

    template <typename T, typename Envir = Env<>>
    using Task = stdexec::task<T, Envir>;

    namespace types {
        // Customization points
        using Connect = stdexec::connect_t;
        using Start = stdexec::start_t;
        using SetValue = stdexec::set_value_t;
        using SetError = stdexec::set_error_t;
        using SetStopped = stdexec::set_stopped_t;
        using GetEnv = stdexec::get_env_t;

        // Queries
        using GetScheduler = stdexec::get_scheduler_t;
        using GetDelegationScheduler = stdexec::get_delegation_scheduler_t;
        using GetAllocator = stdexec::get_allocator_t;
        using GetStopToken = stdexec::get_stop_token_t;
        using GetDomain = stdexec::get_domain_t;
        using GetForwardProgressGuarantee = stdexec::get_forward_progress_guarantee_t;
        using ForwardingQuery = stdexec::forwarding_query_t;

        template <typename Qry>
        using GetCompletionScheduler = stdexec::get_completion_scheduler_t<Qry>;

        // Sender factories
        using Just = stdexec::just_t;
        using JustError = stdexec::just_error_t;
        using JustStopped = stdexec::just_stopped_t;
        using ReadEnv = decltype(stdexec::read_env);
        using Schedule = stdexec::schedule_t;

        // Sender adaptors
        using Then = stdexec::then_t;
        using UponError = stdexec::upon_error_t;
        using UponStopped = stdexec::upon_stopped_t;
        using LetValue = stdexec::let_value_t;
        using LetError = stdexec::let_error_t;
        using LetStopped = stdexec::let_stopped_t;

        // Scheduling adaptors
        using StartsOn = stdexec::starts_on_t;
        using ContinuesOn = stdexec::continues_on_t;
        using On = stdexec::on_t;

        // Composition adaptors
        using WhenAll = stdexec::when_all_t;
        using WhenAllWithVariant = stdexec::when_all_with_variant_t;
        using IntoVariant = stdexec::into_variant_t;

        // Parallel-loop adaptors
        using Bulk = stdexec::bulk_t;
        using BulkChunked = stdexec::bulk_chunked_t;
        using BulkUnchunked = stdexec::bulk_unchunked_t;

        // Stopped-channel translator adaptors
        using StoppedAsError = stdexec::stopped_as_error_t;
        using StoppedAsOptional = stdexec::stopped_as_optional_t;

        // Environment adaptors
        using WriteEnv = decltype(stdexec::write_env);

        // Coroutine utilities
        using AsAwaitable = stdexec::as_awaitable_t;

        // Sender consumers
        using SyncWait = stdexec::sync_wait_t;
        using SyncWaitWithVariant = stdexec::sync_wait_with_variant_t;
        using Spawn = stdexec::spawn_t;
        using SpawnFuture = stdexec::spawn_future_t;
    }

    // Customization points
    inline constexpr types::Connect Connect = stdexec::connect;
    inline constexpr types::Start Start = stdexec::start;
    inline constexpr types::SetValue SetValue = stdexec::set_value;
    inline constexpr types::SetError SetError = stdexec::set_error;
    inline constexpr types::SetStopped SetStopped = stdexec::set_stopped;
    inline constexpr types::GetEnv GetEnv = stdexec::get_env;

    // Queries
    inline constexpr types::GetScheduler GetScheduler = stdexec::get_scheduler;
    inline constexpr types::GetDelegationScheduler GetDelegationScheduler = stdexec::get_delegation_scheduler;
    inline constexpr types::GetAllocator GetAllocator = stdexec::get_allocator;
    inline constexpr types::GetStopToken GetStopToken = stdexec::get_stop_token;
    inline constexpr types::GetDomain GetDomain = stdexec::get_domain;
    inline constexpr types::GetForwardProgressGuarantee GetForwardProgressGuarantee = stdexec::get_forward_progress_guarantee;
    inline constexpr types::ForwardingQuery ForwardingQuery = stdexec::forwarding_query;

    template <typename Qry>
    inline constexpr types::GetCompletionScheduler<Qry> GetCompletionScheduler = stdexec::get_completion_scheduler<Qry>;

    // Sender factories
    inline constexpr types::Just Just = stdexec::just;
    inline constexpr types::JustError JustError = stdexec::just_error;
    inline constexpr types::JustStopped JustStopped = stdexec::just_stopped;
    inline constexpr types::ReadEnv ReadEnv = stdexec::read_env;
    inline constexpr types::Schedule Schedule = stdexec::schedule;

    // Sender adaptors
    inline constexpr types::Then Then = stdexec::then;
    inline constexpr types::UponError UponError = stdexec::upon_error;
    inline constexpr types::UponStopped UponStopped = stdexec::upon_stopped;
    inline constexpr types::LetValue LetValue = stdexec::let_value;
    inline constexpr types::LetError LetError = stdexec::let_error;
    inline constexpr types::LetStopped LetStopped = stdexec::let_stopped;

    // Scheduling adaptors
    inline constexpr types::StartsOn StartsOn = stdexec::starts_on;
    inline constexpr types::ContinuesOn ContinuesOn = stdexec::continues_on;
    inline constexpr types::On On = stdexec::on;

    // Composition adaptors
    inline constexpr types::WhenAll WhenAll = stdexec::when_all;
    inline constexpr types::WhenAllWithVariant WhenAllWithVariant = stdexec::when_all_with_variant;
    inline constexpr types::IntoVariant IntoVariant = stdexec::into_variant;

    // Parallel-loop adaptors
    inline constexpr types::Bulk Bulk = stdexec::bulk;
    inline constexpr types::BulkChunked BulkChunked = stdexec::bulk_chunked;
    inline constexpr types::BulkUnchunked BulkUnchunked = stdexec::bulk_unchunked;

    // Stopped-channel translator adaptors
    inline constexpr types::StoppedAsError StoppedAsError = stdexec::stopped_as_error;
    inline constexpr types::StoppedAsOptional StoppedAsOptional = stdexec::stopped_as_optional;

    // Environment adaptors
    inline constexpr types::WriteEnv WriteEnv = stdexec::write_env;

    // Coroutine utilities
    inline constexpr types::AsAwaitable AsAwaitable = stdexec::as_awaitable;

    // Sender consumers
    inline constexpr types::SyncWait SyncWait = stdexec::sync_wait;
    inline constexpr types::SyncWaitWithVariant SyncWaitWithVariant = stdexec::sync_wait_with_variant;
    inline constexpr types::Spawn Spawn = stdexec::spawn;
    inline constexpr types::SpawnFuture SpawnFuture = stdexec::spawn_future;

    using ParallelScheduler = stdexec::parallel_scheduler;

    using stdexec::get_parallel_scheduler;

    namespace replaceability {
        using ReceiverProxy = stdexec::parallel_scheduler_replacement::receiver_proxy;
        using BulkItemReceiverProxy = stdexec::parallel_scheduler_replacement::bulk_item_receiver_proxy;
        using ParallelSchedulerBackend = stdexec::parallel_scheduler_replacement::parallel_scheduler_backend;

        using stdexec::parallel_scheduler_replacement::query_parallel_scheduler_backend;
    }
    #endif
}
