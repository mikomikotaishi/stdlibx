#pragma once

using stdx::future::LaunchPolicy;
using stdx::meta::AddRvalueReferenceType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsNothrowAssignableValue;
using stdx::meta::IsNothrowInvocableReturnValue;
using stdx::meta::IsNothrowInvocableValue;
using stdx::meta::IsNothrowMoveConstructibleValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TypeIdentityType;
using stdx::ranges::InputRange;

#ifdef __cpp_lib_reflection
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::ReflectableClass;
using stdx::meta::reflect::ReflectableEnum;
using stdx::meta::reflect::ReflectableUnion;
using stdx::meta::reflect::Type;
using stdx::meta::reflect::Union;
#endif

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    class Ops final {
    public:
        Ops() = delete;

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
        static constexpr auto to_array(T (&a)[N]) {
            return std::to_array(a);
        }

        template <typename T, usize N>
        static constexpr auto to_array(T (&&a)[N]) {
            return std::to_array(a);
        }

        template <typename T>
        static constexpr auto size(const T& x) {
            return std::size(x);
        }

        template <typename T>
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
        static constexpr decltype(auto) forward(RemoveReferenceType<T>& t) noexcept {
            return std::forward<T>(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) forward(RemoveReferenceType<T>&& t) noexcept {
            return std::forward<T>(t);
        }

        template <typename T, typename U = T>
        static constexpr decltype(auto) exchange(T& obj, U&& new_value)
            noexcept(IsNothrowMoveConstructibleValue<T> && IsNothrowAssignableValue<T&, U>){
            return std::exchange(obj, forward<U>(new_value));
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) in_range(T t) noexcept {
            return std::in_range(t);
        }

        #ifdef __cpp_lib_forward_like
        template <typename T, typename U>
        [[nodiscard]]
        static constexpr decltype(auto) forward_like(U&& u) noexcept {
            return std::forward_like<T, U>(u);
        }
        #endif

        template <typename E>
        [[nodiscard]]
        static constexpr decltype(auto) to_underlying(E e) noexcept {
            return std::to_underlying(e);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) move(T&& t) noexcept {
            return std::move(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) move_if_noexcept(T&& t) noexcept {
            return std::move_if_noexcept(t);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr decltype(auto) as_const(T& t) noexcept {
            return std::as_const(t);
        }

        template <typename T>
        static void as_const(const T&&) = delete;

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
        static decltype(auto) ref(T& t) noexcept {
            return std::ref(t);
        }

        template <typename T>
        static decltype(auto) ref(ReferenceWrapper<T> t) noexcept {
            return std::ref(t);
        }

        template <typename T>
        static void ref(const T&&) = delete;

        template <typename T>
        static decltype(auto) cref(const T& t) noexcept {
            return std::cref(t);
        }

        template <typename T>
        static decltype(auto) cref(ReferenceWrapper<const T> t) noexcept {
            return std::cref(t);
        }

        template <typename T>
        static void cref(const T&&) = delete;

        template <typename F, typename... Args>
        static constexpr decltype(auto) invoke(F&& f, Args&&... args)
            noexcept(IsNothrowInvocableValue<F, Args...>) {
            return std::invoke(forward<F>(f), forward<Args>(args)...);
        }

        template <typename R, typename F, typename... Args>
        static constexpr decltype(auto) invoke_r(F&& f, Args&&... args)
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
        static constexpr decltype(auto) optional(T&& value) noexcept {
            return std::make_optional(forward<T>(value));
        }

        template <typename T, typename... Args>
        [[nodiscard]]
        static constexpr decltype(auto) optional(Args&&... args) noexcept {
            return std::make_optional<T>(forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args>
        [[nodiscard]]
        static constexpr decltype(auto) optional(InitializerList<U> il, Args&&... args) noexcept {
            return std::make_optional<T>(il, forward<Args>(args)...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) tie(Ts&... args) noexcept {
            return std::tie(args...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) tuple(Ts&&... args) {
            return std::make_tuple(forward<Ts>(args)...);
        }

        template <typename... Ts>
        [[nodiscard]]
        static constexpr decltype(auto) forward_as_tuple(Ts&&... args) noexcept {
            return std::forward_as_tuple(forward<Ts>(args)...);
        }

        template <typename... Tpls>
        [[nodiscard]]
        static constexpr decltype(auto) tuple_cat(Tpls&&... tpls) {
            return std::tuple_cat(forward<Tpls>(tpls)...);
        }

        template <typename T, typename Tpl>
        [[nodiscard]]
        static constexpr decltype(auto) from_tuple(Tpl&& tpl) {
            return std::make_from_tuple<T>(forward<Tpl>(tpl));
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr decltype(auto) pair(T&& x, U&& y) noexcept {
            return std::make_pair(forward<T>(x), forward<U>(y));
        }

        template <typename F, typename... Args>
        static auto async(F&& f, Args&&... args) {
            return std::async(forward<F>(f), forward<Args>(args)...);
        }

        template <typename F, typename... Args>
        static auto async(LaunchPolicy policy, F&& f, Args&&... args) {
            return std::async(policy, forward<F>(f), forward<Args>(args)...);
        }

        #ifdef __cpp_lib_reflection
        template <typename T>
        static consteval Info reflect_type() {
            return ^^T;
        }

        template <CharacterLike T>
        static consteval bool reflect_string_literal(const T* p) {
            return std::is_string_literal(p);
        }

        template <InputRange R>
        static consteval decltype(auto) define_static_string(R&& r) {
            return std::define_static_string(forward<R>(r));
        }

        template <InputRange R>
        static consteval decltype(auto) define_static_array(R&& r) {
            return std::define_static_array(forward<R>(r));
        }

        template <typename T>
        static consteval decltype(auto) define_static_object(T&& t) {
            return std::define_static_object(forward<T>(t));
        }

        template <InputRange R>
        static consteval decltype(auto) reflect_constant_string(R&& r) {
            return std::meta::reflect_constant_string(forward<R>(r));
        }

        template <InputRange R>
        static consteval decltype(auto) reflect_constant_array(R&& r) {
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
        #endif
    };
}
