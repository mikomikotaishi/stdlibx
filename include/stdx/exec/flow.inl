#pragma once

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::ContinuesOn;
using stdx::exec::Env;
using stdx::exec::Just;
using stdx::exec::LetValue;
using stdx::exec::Scheduler;
using stdx::exec::Schedule;
using stdx::exec::Sender;
using stdx::exec::SyncWait;
using stdx::exec::Task;
using stdx::exec::Then;
using stdx::exec::UponError;
using stdx::exec::ValueTypesOf;
using stdx::exec::WhenAll;
using stdx::meta::RemoveConstVolatileReferenceType;
#endif

/**
 * @namespace stdx::exec
 * @brief Standard library execution operations (internal helpers).
 *
 * This block is intentionally a plain (non-exported) namespace: the value-type
 * machinery below is an implementation detail of Flow, not part of the public
 * surface.
 */
namespace stdx::exec {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    /**
     * @struct NoSingleValue
     * @brief Sentinel meaning a Sender does not complete with exactly one value.
     */
    struct NoSingleValue {};

    /**
     * @struct SoleTypeImpl
     * @brief Collapses a parameter pack of exactly one type down to that type,
     * yielding NoSingleValue for any other arity. Kept total (rather than
     * undefined) on purpose: SingleValueType is eagerly instantiated when
     * Flow<S> is - even for calls like wait() that never need it - so an
     * undefined primary would hard-error over a multi-value sender (e.g.
     * when_all). value()/as_task() gate on the sentinel instead.
     * @tparam Ts The types to collapse.
     */
    template <typename... Ts>
    struct SoleTypeImpl {
        using Type = NoSingleValue;
    };

    template <typename T>
    struct SoleTypeImpl<T> {
        using Type = T;
    };

    template <typename... Ts>
    using SoleType = typename SoleTypeImpl<Ts...>::Type;

    /**
     * @brief The single value type a Sender completes with, i.e. the sole T of
     * its sole `set_value(T)` completion; NoSingleValue if it sends zero
     * or many values, or has multiple value completions.
     * @tparam S The Sender type.
     */
    template <Sender S>
    using SingleValueType = ValueTypesOf<S, Env<>, SoleType, SoleType>;

    /**
     * @concept SingleValued
     * @brief Whether S completes with exactly one value, so it can be unwrapped
     * by Flow::value() / Flow::as_task(). Multi-value or void senders use
     * Flow::wait() and destructure the Tuple.
     * @tparam S The Sender type.
     */
    template <typename S>
    concept SingleValued = Sender<S> && !SameAs<SingleValueType<S>, NoSingleValue>;

    /**
     * @concept Thenable
     * @brief Whether Then (execution::then) accepts sender S and function Func.
     * @tparam Func The value-mapping function type.
     * @tparam S The sender type.
     */
    template <typename Func, typename S>
    concept Thenable = requires (Func&& f) {
        Then(Ops::declval<S>(), Ops::forward<Func>(f));
    };

    /**
     * @concept ErrorHandleable
     * @brief Whether UponError (execution::upon_error) accepts S and Func.
     * @tparam Func The error-handling function type.
     * @tparam S The sender type.
     */
    template <typename Func, typename S>
    concept ErrorHandleable = requires (Func&& f) {
        UponError(Ops::declval<S>(), Ops::forward<Func>(f));
    };

    /**
     * @concept Bindable
     * @brief Whether LetValue (execution::let_value) accepts S and Func.
     * @tparam Func A function returning a sender.
     * @tparam S The sender type.
     */
    template <typename Func, typename S>
    concept Bindable = requires (Func&& f) {
        LetValue(Ops::declval<S>(), Ops::forward<Func>(f));
    };

    /**
     * @concept ContinuableOn
     * @brief Whether ContinuesOn (execution::continues_on) can transfer S onto Sch.
     * @tparam Sch The scheduler type.
     * @tparam S The sender type.
     */
    template <typename Sch, typename S>
    concept ContinuableOn = Scheduler<Sch> && requires (Sch sch) {
        ContinuesOn(Ops::declval<S>(), Ops::move(sch));
    };

    /**
     * @concept ThenContinuableOn
     * @brief Whether S can be transferred onto Sch and then mapped by Func (the
     * fused then_on operation).
     * @tparam S The sender type.
     * @tparam Sch The scheduler type.
     * @tparam Func The value-mapping function type.
     */
    template <typename S, typename Sch, typename Func>
    concept ThenContinuableOn = Scheduler<Sch> && requires (Sch sch, Func&& f) {
        Then(ContinuesOn(Ops::declval<S>(), Ops::move(sch)), Ops::forward<Func>(f));
    };

    /**
     * @concept WhenAllable
     * @brief Whether WhenAll (execution::when_all) accepts S together with Others.
     * @tparam S The sender type.
     * @tparam Others The other sender types.
     */
    template <typename S, typename... Others>
    concept WhenAllable = requires (Others&&... others) {
        WhenAll(Ops::declval<S>(), Ops::forward<Others>(others)...);
    };
    #endif
}

/**
 * @namespace stdx::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    /**
     * @class Flow
     * @brief A fluent, method-chaining wrapper over a Sender.
     * @tparam S The wrapped Sender type.
     *
     * Senders are lazy, move-only and single-shot, so every method is
     * rvalue-ref qualified: it consumes `*this` and yields the next stage.
     */
    template <Sender S = decltype(Just())>
    class [[nodiscard]] Flow {
    private:
        S sender; ///< The underlying sender being built up.

        /**
         * @brief Materializes the pipeline as a coroutine Task.
         * @tparam V The single value type (deduced from the sender).
         * @return A Task<V> completing with the sent value.
         *
         * This is the WhenAll-safe endpoint (mirrors Async::offload): routing
         * through a Task frame lets scheduler-backed work compose under WhenAll.
         *
         * @warning Do not `co_await` the resulting Task inside another Task when
         * it is parallel-scheduler-backed; use unwrap() for the co_await-in-Task
         * path instead.
         */
        template <typename V, Sender Snd>
        static Task<V> into_task(Snd snd) {
            co_return co_await Ops::move(snd);
        }
    public:
        /**
         * @brief Wraps a sender (rvalue or matching lvalue) in a Flow.
         * @param s The sender to wrap; its decayed type must be S.
         */
        explicit Flow(SameAs<RemoveConstVolatileReferenceType<S>> auto&& s):
            sender{Ops::forward<decltype(s)>(s)} {}

        /**
         * @brief Starts a Flow on a scheduler.
         * @param scheduler The scheduler whose schedule-sender becomes the first stage.
         *
         * Reachable only through class template argument deduction: `Flow(sched)`
         * deduces `Flow<ScheduleSender>`. Naming `Flow<>` disables CTAD, and CTAD
         * cannot deduce from an absent argument, so use `on_schedule()` when you
         * want the default parallel scheduler.
         */
        explicit Flow(Scheduler auto scheduler)
            requires SameAs<S, RemoveConstVolatileReferenceType<decltype(Schedule(Ops::move(scheduler)))>>:
            sender{Schedule(Ops::move(scheduler))} {}

        /**
         * @brief Maps the value channel through a function.
         * @tparam Func The mapping function type.
         * @param func The function applied to the sent value(s).
         * @return A Flow over the transformed sender.
         */
        template <Thenable<S> Func>
        [[nodiscard]]
        auto then(Func&& func) && {
            return Flow<decltype(Then(Ops::move(sender), Ops::forward<Func>(func)))>(
                Then(Ops::move(sender), Ops::forward<Func>(func))
            );
        }

        /**
         * @brief Handles the error channel with a function.
         * @tparam Func The handler type.
         * @param func The function applied to the error.
         * @return A Flow over the recovering sender.
         */
        template <ErrorHandleable<S> Func>
        [[nodiscard]]
        auto catch_error(Func&& func) && {
            return Flow<decltype(UponError(Ops::move(sender), Ops::forward<Func>(func)))>(
                UponError(Ops::move(sender), Ops::forward<Func>(func))
            );
        }

        /**
         * @brief Continues into a child sender per value.
         * @param func The function producing the next sender.
         * @return A Flow over the composed sender.
         */
        template <Bindable<S> Func>
        [[nodiscard]]
        auto let(Func&& func) && {
            return Flow<decltype(LetValue(Ops::move(sender), Ops::forward<Func>(func)))>(
                LetValue(Ops::move(sender), Ops::forward<Func>(func))
            );
        }

        /**
         * @brief Transfers continuation onto a scheduler.
         * @param scheduler The scheduler to continue on.
         * @return A Flow whose downstream runs on @p scheduler.
         */
        [[nodiscard]]
        auto continue_on(ContinuableOn<S> auto scheduler) && {
            return Flow<decltype(ContinuesOn(Ops::move(sender), Ops::move(scheduler)))>(
                ContinuesOn(Ops::move(sender), Ops::move(scheduler))
            );
        }

        /**
         * @brief Fused hop-then-map: continue on @p scheduler, then apply @p func.
         * @tparam Sch The scheduler type.
         * @tparam Func The mapping function type.
         * @param scheduler The scheduler to run @p func on.
         * @param func The function applied on that scheduler.
         * @return A Flow over the transformed sender.
         */
        template <typename Sch, typename Func>
            requires ThenContinuableOn<S, Sch, Func>
        [[nodiscard]]
        auto then_on(Sch scheduler, Func&& func) && {
            auto scheduled = ContinuesOn(Ops::move(sender), Ops::move(scheduler));
            return Flow<decltype(Then(Ops::move(scheduled), Ops::forward<Func>(func)))>(
                Then(Ops::move(scheduled), Ops::forward<Func>(func))
            );
        }

        /**
         * @brief Runs this pipeline concurrently with others.
         * @tparam Others The other Sender types.
         * @param others The other senders to overlap with.
         * @return A Flow completing when all have completed.
         */
        template <Sender... Others>
            requires WhenAllable<S, Others...>
        [[nodiscard]]
        auto when_all(Others&&... others) && {
            return Flow<decltype(WhenAll(Ops::move(sender), Ops::forward<Others>(others)...))>(
                WhenAll(Ops::move(sender), Ops::forward<Others>(others)...)
            );
        }

        /**
         * @brief Releases the built-up sender for direct use.
         * @return The wrapped sender.
         *
         * This is the co_await-in-Task-safe endpoint: `co_await flow.unwrap()`
         * inside a coroutine Task awaits the raw sender, avoiding the nested
         * parallel-scheduler-Task pitfall that as_task() can hit.
         */
        [[nodiscard]]
        S unwrap() && {
            return Ops::move(sender);
        }

        /**
         * @brief Blocks the calling thread until the pipeline completes.
         * @return An Optional<Tuple<...>> of the sent values (empty if stopped).
         */
        [[nodiscard]]
        auto wait() && {
            return SyncWait(Ops::move(sender));
        }

        /**
         * @brief Blocks until completion, unwrapping a single sent value.
         * @tparam V The single value type (deduced from the sender).
         * @return The value, or an empty Optional if the pipeline stopped.
         *
         * Convenience over wait() for the common single-value case: yields the
         * lone value directly instead of a Tuple.
         */
        template <typename V = SingleValueType<S>>
            requires SingleValued<S>
        [[nodiscard]]
        Optional<V> value() && {
            auto result = SyncWait(Ops::move(sender));
            if (!result) {
                return Optional<V>();
            }
            return Optional<V>(Ops::get<0>(Ops::move(*result)));
        }

        template <typename V = SingleValueType<S>>
            requires SingleValued<S>
        [[nodiscard]]
        Task<V> as_task() && {
            return into_task<V>(Ops::move(sender));
        }

        /**
         * @brief Wraps an existing sender in a Flow.
         * @tparam Snd The sender type (named distinctly from the class's S,
         * which it would otherwise shadow).
         * @param sender The sender to wrap.
         * @return A Flow over @p sender.
         */
        template <Sender Snd>
        [[nodiscard]]
        static Flow<RemoveConstVolatileReferenceType<Snd>> from(Snd&& sender) {
            return Flow<RemoveConstVolatileReferenceType<Snd>>(Ops::forward<Snd>(sender));
        }

        /**
         * @brief Starts a Flow on a scheduler.
         * @tparam Sch The scheduler type.
         * @param scheduler The scheduler to start on.
         * @return A Flow whose first stage runs on @p scheduler.
         */
        template <Scheduler Sch = decltype(Async::parallel_scheduler())>
        [[nodiscard]]
        static auto on_schedule(Sch scheduler = Async::parallel_scheduler()) {
            return from(Schedule(Ops::move(scheduler)));
        }
    };

    /**
     * @brief Deduction guide: wrap a sender by value.
     */
    template <Sender S>
    Flow(S) -> Flow<S>;

    /**
     * @brief Deduction guide: build the first stage from a scheduler.
     */
    template <Scheduler Sch>
    Flow(Sch) -> Flow<RemoveConstVolatileReferenceType<decltype(Schedule(Ops::declval<Sch>()))>>;
    #endif
}
