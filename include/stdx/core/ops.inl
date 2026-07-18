#pragma once

using stdx::future::Future;
using stdx::future::LaunchPolicy;
using stdx::meta::AddConstType;
using stdx::meta::AddRvalueReferenceType;
using stdx::meta::DecayType;
using stdx::meta::InvokeResultType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsNothrowAssignableValue;
using stdx::meta::IsNothrowInvocableReturnValue;
using stdx::meta::IsNothrowInvocableValue;
using stdx::meta::IsNothrowMoveConstructibleValue;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::RemoveConstVolatileType;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TypeIdentityType;
using stdx::meta::UnderlyingTypeType;
using stdx::ranges::InputRange;
using stdx::ranges::RangeValue;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::ParallelScheduler;
using stdx::exec::Schedule;
using stdx::exec::Sender;
using stdx::exec::Task;
using stdx::exec::Then;
#endif

#ifdef __cpp_lib_reflection
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::ReflectableClass;
using stdx::meta::reflect::ReflectableEnum;
using stdx::meta::reflect::ReflectableUnion;
using stdx::meta::reflect::ThrownExceptions;
using stdx::meta::reflect::Type;
using stdx::meta::reflect::Union;
#endif

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @class Ops
     * @brief Utility class providing static wrappers for standard library operations.
     */
    class Ops final {
    public:
        Ops() = delete("Ops is a static utility class and cannot be instantiated.");

        /**
        * @brief Extracts the type of the parameter of a function type.
        * Left undefined so non-template types cause a compilation failure.
        * 
        * @tparam T 
        */
        template <typename T>
        class ExtractParameter;

        template <template <typename...> class Tmpl, typename... Args>
        class ExtractParameter<Tmpl<Args...>> {
        public:
            static constexpr usize ARITY = sizeof...(Args);

            #ifdef __cpp_pack_indexing
            template <usize I>
            using Arg = Args...[I];
            #else
            template <usize I>
            using Arg = TupleElementType<I, Tuple<Args...>>;
            #endif

            template <typename... NewArgs>
            using Rebind = Tmpl<NewArgs...>;
        };

        template <typename T>
        static constexpr void swap(T& a, T& b) {
            std::swap(a, b);
        }

        template <typename T, usize N>
        static constexpr void swap(T (&a)[N], T (&b)[N]) {
            std::swap(a, b);
        }

        template <typename T, usize N>
        [[nodiscard]]
        static constexpr Array<RemoveConstVolatileType<T>, N> to_array(T (&a)[N]) {
            return std::to_array(a);
        }

        template <typename T, usize N>
        [[nodiscard]]
        static constexpr Array<RemoveConstVolatileType<T>, N> to_array(T (&&a)[N]) {
            return std::to_array(a);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr auto size(const T& x) {
            return std::size(x);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr auto ssize(const T& x) {
            return std::ssize(x);
        }

        template <typename T, usize N>
        [[nodiscard]]
        static constexpr usize size(const T (&a)[N]) noexcept {
            return N;
        }

        template <typename T, isize N>
        [[nodiscard]]
        static constexpr isize ssize(const T (&a)[N]) noexcept {
            return N;
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T&& forward(RemoveReferenceType<T>& t) noexcept {
            return std::forward<T>(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T&& forward(RemoveReferenceType<T>&& t) noexcept {
            return std::forward<T>(t);
        }

        #ifdef __cpp_lib_forward_like
        template <typename T, typename U>
        [[nodiscard]]
        static constexpr auto&& forward_like(U&& u) noexcept {
            return std::forward_like<T, U>(u);
        }
        #endif

        template <typename T, typename U = T>
        static constexpr T exchange(T& obj, U&& new_value)
            noexcept(IsNothrowMoveConstructibleValue<T> && IsNothrowAssignableValue<T&, U>){
            return std::exchange(obj, forward<U>(new_value));
        }

        template <typename T>
        [[nodiscard]]
        static constexpr bool in_range(T t) noexcept {
            return std::in_range(t);
        }

        template <typename E>
        [[nodiscard]]
        static constexpr UnderlyingTypeType<E> to_underlying(E e) noexcept {
            return std::to_underlying(e);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr RemoveReferenceType<T>&& move(T&& t) noexcept {
            return std::move(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) move_if_noexcept(T&& t) noexcept {
            return std::move_if_noexcept(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr AddConstType<T>& as_const(T& t) noexcept {
            return std::as_const(t);
        }

        template <typename T>
        static void as_const(const T&&) = delete("as_const is not callable with an rvalue reference.");

        template <typename T>
        [[nodiscard]]
        static AddRvalueReferenceType<T> declval() noexcept {
            return std::declval<T>();
        }

        [[noreturn]]
        static void unreachable() noexcept {
            std::unreachable();
        }

        template <typename F, typename... Args>
        static constexpr decltype(auto) bind(F&& f, Args&&... args) {
            return std::bind(forward<F>(f), forward<Args>(args)...);
        }

        template <typename T>
        static ReferenceWrapper<T> ref(T& t) noexcept {
            return std::ref(t);
        }

        template <typename T>
        static ReferenceWrapper<T> ref(ReferenceWrapper<T> t) noexcept {
            return std::ref(t);
        }

        template <typename T>
        static void ref(const T&&) = delete("ref is not callable with an rvalue reference.");

        template <typename T>
        static ReferenceWrapper<const T> cref(const T& t) noexcept {
            return std::cref(t);
        }

        template <typename T>
        static ReferenceWrapper<const T> cref(ReferenceWrapper<const T> t) noexcept {
            return std::cref(t);
        }

        template <typename T>
        static void cref(const T&&) = delete("cref is not callable with an rvalue reference.");

        template <typename F, typename... Args>
        static constexpr InvokeResultType<F, Args...> invoke(F&& f, Args&&... args)
            noexcept(IsNothrowInvocableValue<F, Args...>) {
            return std::invoke(forward<F>(f), forward<Args>(args)...);
        }

        template <typename R, typename F, typename... Args>
        static constexpr R invoke_r(F&& f, Args&&... args)
            noexcept(IsNothrowInvocableReturnValue<R, F, Args...>) {
            return std::invoke_r<R>(forward<F>(f), forward<Args>(args)...);
        }

        template <usize I, typename T>
        [[nodiscard]]
        static constexpr decltype(auto) get(T&& t) noexcept {
            return std::get<I>(forward<T>(t));
        }

        template <typename T, typename V>
        [[nodiscard]]
        static constexpr decltype(auto) get(V&& v) {
            return std::get<T>(forward<V>(v));
        }

        template <usize I, typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) get_if(Variant<Ts...>* v) noexcept {
            return std::get_if<I>(v);
        }

        template <usize I, typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) get_if(const Variant<Ts...>* v) noexcept {
            return std::get_if<I>(v);
        }

        template <typename T, typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) get_if(Variant<Ts...>* v) noexcept {
            return std::get_if<T>(v);
        }

        template <typename T, typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) get_if(const Variant<Ts...>* v) noexcept {
            return std::get_if<T>(v);
        }

        template <typename Vst, typename... Vars>
        static constexpr decltype(auto) visit(Vst&& v, Vars&&... vars) {
            return std::visit(forward<Vst>(v), forward<Vars>(vars)...);
        }

        template <typename R, typename Vst, typename... Vars>
        static constexpr R visit(Vst&& v, Vars&&... vars) {
            return std::visit(forward<Vst>(v), forward<Vars>(vars)...);
        }

        template <typename F, typename Tpl>
        static constexpr decltype(auto) apply(F&& f, Tpl&& tpl)
            noexcept(noexcept(std::apply(forward<F>(f), forward<Tpl>(tpl)))) {
            return std::apply(forward<F>(f), forward<Tpl>(tpl));
        }

        [[nodiscard]]
        static const ErrorCategory& generic_category() noexcept {
            return std::generic_category();
        }

        [[nodiscard]]
        static const ErrorCategory& system_category() noexcept {
            return std::system_category();
        }

        [[nodiscard]]
        static const ErrorCategory& future_category() noexcept {
            return ::stdx::future::future_category();
        }

        [[nodiscard]]
        static ErrorCode error_code(Errc e) noexcept {
            return std::make_error_code(e);
        }

        [[nodiscard]]
        static ErrorCode error_code(IOErrc e) noexcept {
            return std::make_error_code(e);
        }

        [[nodiscard]]
        static ErrorCode error_code(FutureErrc e) noexcept {
            return ::stdx::future::make_error_code(e);
        }

        [[nodiscard]]
        static ErrorCondition error_condition(Errc e) noexcept {
            return std::make_error_condition(e);
        }

        [[nodiscard]]
        static ErrorCondition error_condition(IOErrc e) noexcept {
            return std::make_error_condition(e);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T any_cast(const Any& operand) {
            return std::any_cast<T>(operand);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T any_cast(Any& operand) {
            return std::any_cast<T>(operand);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T any_cast(Any&& operand) {
            return std::any_cast<T>(move(operand));
        }

        template <typename T>
        [[nodiscard]]
        static constexpr const T* any_cast(const Any* operand) noexcept {
            return std::any_cast<T>(operand);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T* any_cast(Any* operand) noexcept {
            return std::any_cast<T>(operand);
        }

        template <typename T, typename... Args>
        [[nodiscard]]
        static constexpr Any any(Args&&... args) {
            return std::make_any<T>(forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]]
        static constexpr Any any(InitializerList<U> il, Args&&... args) {
            return std::make_any<T>(il, forward<Args>(args)...);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr Optional<DecayType<T>> optional(T&& value) noexcept {
            return std::make_optional(forward<T>(value));
        }

        template <typename T, typename... Args>
        [[nodiscard]]
        static constexpr Optional<T> optional(Args&&... args) noexcept {
            return std::make_optional<T>(forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]]
        static constexpr Optional<T> optional(InitializerList<U> il, Args&&... args) noexcept {
            return std::make_optional<T>(il, forward<Args>(args)...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr Tuple<Ts&...> tie(Ts&... args) noexcept {
            return std::tie(args...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) tuple(Ts&&... args) {
            return std::make_tuple(forward<Ts>(args)...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr Tuple<Ts&&...> forward_as_tuple(Ts&&... args) noexcept {
            return std::forward_as_tuple(forward<Ts>(args)...);
        }

        template <typename... Tpls>
        [[nodiscard]]
        static constexpr decltype(auto) tuple_cat(Tpls&&... tpls) {
            return std::tuple_cat(forward<Tpls>(tpls)...);
        }

        template <typename T, typename Tpl>
        [[nodiscard]]
        static constexpr T from_tuple(Tpl&& tpl) {
            return std::make_from_tuple<T>(forward<Tpl>(tpl));
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr Pair<UnwrapReferenceDecayType<T>, UnwrapReferenceDecayType<U>> pair(T&& x, U&& y) noexcept {
            return std::make_pair(forward<T>(x), forward<U>(y));
        }

        #ifdef __cpp_lib_reflection
        template <typename T>
        [[nodiscard]]
        static consteval Info info() {
            return ^^T;
        }

        template <typename T>
        [[nodiscard]]
        static consteval T extract(Info r) {
            return std::meta::extract<T>(r);
        }

        template <CharacterLike T>
        [[nodiscard]]
        static consteval bool is_string_literal(const T* p) {
            return std::is_string_literal(p);
        }

        template <InputRange R>
        [[nodiscard]]
        static consteval const RangeValue<R>* define_static_string(R&& r) {
            return std::define_static_string(forward<R>(r));
        }

        template <InputRange R>
        [[nodiscard]]
        static consteval Span<const RangeValue<R>> define_static_array(R&& r) {
            return std::define_static_array(forward<R>(r));
        }

        template <typename T>
        [[nodiscard]]
        static consteval RemoveConstVolatileReferenceType<T>* define_static_object(T&& t) {
            return std::define_static_object(forward<T>(t));
        }

        template <InputRange R>
        [[nodiscard]]
        static consteval Info reflect_constant_string(R&& r) {
            return std::meta::reflect_constant_string(forward<R>(r));
        }

        template <InputRange R>
        [[nodiscard]]
        static consteval Info reflect_constant_array(R&& r) {
            return std::meta::reflect_constant_array(forward<R>(r));
        }

        template <ReflectableClass T>
        [[nodiscard]]
        static consteval Class<T> class_of() noexcept {
            return Class<T>();
        }

        template <ReflectableEnum E>
        [[nodiscard]]
        static consteval Enum<E> enum_of() noexcept {
            return Enum<E>();
        }

        template <ReflectableUnion U>
        [[nodiscard]]
        static consteval Union<U> union_of() noexcept {
            return Union<U>();
        }

        template <typename T>
        [[nodiscard]]
        static consteval Type type_of() noexcept {
            return Type(^^T);
        }

        /**
         * @brief All exception types a function (or callable) @p Fn declares via
         * Throws annotations, each as a Class<E>, deduplicated and in declaration
         * order. The Throws annotation machinery is supplied by core/throws.inl
         * (included before this file); the body is defined out-of-line below.
         * @tparam Fn The reflection of a function or callable type.
         * @return A Tuple<Class<E>...> of the declared exception types.
         */
        template <Info Fn>
            requires FunctionOrCallable<Fn>
        [[nodiscard]]
        static consteval ThrownExceptions<Fn> thrown_exceptions() {
            return build_thrown_exceptions<Fn>(IndexSequenceOf<ThrownInfos<Fn>.size()>{});
        }
        #endif
    };

    /**
     * @class Async
     * @brief Utility class providing static wrappers for standard library operations.
     */
    class Async final {
    public:
        Async() = delete("Async is a static utility class and cannot be instantiated.");

        /**
         * @brief Defers or spawns a callable on a new thread by calling std::async.
         * @tparam F A callable type.
         * @tparam Args The argument types for the callable.
         * @param f The callable to invoke.
         * @param args The arguments to pass to the callable.
         * @return A Future representing the result of the callable.
         */
        template <typename F, typename... Args>
        static Future<InvokeResultType<DecayType<F>, DecayType<Args>...>> defer_or_spawn(F&& f, Args&&... args) {
            return std::async(forward<F>(f), forward<Args>(args)...);
        }

        /**
         * @brief Defers or spawns a callable on a new thread by calling std::async with a launch policy.
         * @tparam F A callable type.
         * @tparam Args The argument types for the callable.
         * @param policy The launch policy (e.g., std::launch::async or std::launch::deferred).
         * @param f The callable to invoke.
         * @param args The arguments to pass to the callable.
         * @return A Future representing the result of the callable.
         */
        template <typename F, typename... Args>
        static Future<InvokeResultType<DecayType<F>, DecayType<Args>...>> defer_or_spawn(LaunchPolicy policy, F&& f, Args&&... args) {
            return std::async(policy, forward<F>(f), forward<Args>(args)...);
        }

        #ifdef STDLIBX_EXECUTION_AVAILABLE
        [[nodiscard]]
        static ParallelScheduler parallel_scheduler() {
            return stdx::exec::get_parallel_scheduler();
        }

        /**
         * @brief Offloads a blocking callable onto the system thread pool.
         * @tparam F A nullary callable.
         * @param f The blocking work to run off the calling thread.
         * @return A sender completing with the result of f() (or set_error if it
         * throws; the exception is re-raised at the await / sync_wait site).
         *
         * @warning Do not compose two of these senders under WhenAll.
         * To overlap two offloads, use the Task-based `Ops::offload` and
         * `WhenAll(offload(f), offload(g))` instead. Likewise a `void` callable
         *  under `SyncWait` trips the same move miscompile — use `offload` there.
         */
        template <Invocable F>
        [[nodiscard]]
        static Sender auto offload_sender(F f) {
            return Then(Schedule(parallel_scheduler()), Ops::move(f));
        }

        /**
         * @brief Coroutine bridge: run a blocking callable on the parallel scheduler.
         * @tparam F A nullary callable.
         * @param f The blocking work to run off the calling thread.
         * @return A Task completing with the result of f() (or set_error if it throws;
         * the exception is re-raised at the await / sync_wait site).
         *
         * @warning Do not `co_await offload(...)` inside another Task.
         * To co_await an offload from inside a Task, use the
         * sender-based `Ops::offload_sender`.
         */
        template <Invocable F>
        static Task<InvokeResultType<F&>> offload(F f) {
            co_await Schedule(parallel_scheduler());
            co_return f();
        }
        #endif
    };
}
