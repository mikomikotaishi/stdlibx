#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_move_only_function
    template <typename... Sig>
    using MoveOnlyFunction = std::move_only_function<Sig...>;
    #endif

    #ifdef __cpp_lib_copyable_function
    template <typename... Sig>
    using CopyableFunction = std::copyable_function<Sig...>;
    #endif

    #ifdef __cpp_lib_function_ref
    template <typename... Sig>
    using FunctionReference = std::function_ref<Sig...>;
    #endif

    namespace types {
        template <typename M, typename T>
        using MemberFunction = decltype(std::mem_fn<M, T>);
    }

    template <typename M, typename T>
    constexpr types::MemberFunction<M, T> MemberFunction(M T::* pm) {
        return std::mem_fn<M, T>(pm);
    }

    template <typename T>
    using ReferenceWrapper = std::reference_wrapper<T>;

    template <typename T>
    using UnwrapReference = std::unwrap_reference<T>;

    template <typename T>
    using UnwrapReferenceDecay = std::unwrap_ref_decay<T>;

    template <typename T>
    using UnwrapReferenceType = std::unwrap_reference_t<T>;

    template <typename T>
    using UnwrapReferenceDecayType = std::unwrap_ref_decay_t<T>;

    template <typename T>
    using IsBindExpression = std::is_bind_expression<T>;

    template <typename T>
    using IsPlaceholder = std::is_placeholder<T>;

    template <typename T = void>
    using Plus = std::plus<T>;

    template <typename T = void>
    using Minus = std::minus<T>;

    template <typename T = void>
    using Multiplies = std::multiplies<T>;

    template <typename T = void>
    using Divides = std::divides<T>;

    template <typename T = void>
    using Modulus = std::modulus<T>;

    template <typename T = void>
    using Negate = std::negate<T>;

    template <typename T = void>
    using LogicalAnd = std::logical_and<T>;

    template <typename T = void>
    using LogicalOr = std::logical_or<T>;

    template <typename T = void>
    using LogicalNot = std::logical_not<T>;

    template <typename T = void>
    using BitAnd = std::bit_and<T>;

    template <typename T = void>
    using BitOr = std::bit_or<T>;

    template <typename T = void>
    using BitXor = std::bit_xor<T>;

    template <typename T = void>
    using BitNot = std::bit_not<T>;

    using Identity = std::identity;

    template <typename ForwardIterator, typename BinPred = EqualTo<>>
    using DefaultSearcher = std::default_searcher<ForwardIterator, BinPred>;

    template <typename RandomIterator, typename H = Hash<typename std::iterator_traits<RandomIterator>::value_type>, typename BinPred = EqualTo<>>
    using BoyerMooreSearcher = std::boyer_moore_searcher<RandomIterator, BinPred>;

    template <typename RandomIterator, typename H = Hash<typename std::iterator_traits<RandomIterator>::value_type>, typename BinPred = EqualTo<>>
    using BoyerMooreHorspoolSearcher = std::boyer_moore_horspool_searcher<RandomIterator, BinPred>;

    using BadFunctionCallException = std::bad_function_call;

    using std::bind_front;
    using std::bind;
    using std::ref;
    using std::cref;
    using std::invoke;
    using std::invoke_r;
}
