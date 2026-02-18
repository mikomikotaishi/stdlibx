#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename Signature>
    using Function = std::function<Signature>;
    
    template <typename... Signature>
    using MoveOnlyFunction = std::move_only_function<Signature...>;

    using std::hash;
    
    template <typename T>
    struct Hash: std::hash<T> {};

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
    using EqualTo = std::equal_to<T>;

    template <typename T = void>
    using NotEqualTo = std::not_equal_to<T>;

    template <typename T = void>
    using Greater = std::greater<T>;

    template <typename T = void>
    using Less = std::less<T>;

    template <typename T = void>
    using GreaterEqual = std::greater_equal<T>;

    template <typename T = void>
    using LessEqual = std::less_equal<T>;

    using CompareThreeWay = std::compare_three_way;

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

    template <typename ForwardIterator, typename BinaryPredicate = EqualTo<>>
    using DefaultSearcher = std::default_searcher<ForwardIterator>;

    template <typename RandomIterator, typename H = Hash<typename std::iterator_traits<RandomIterator>::value_type>, typename BinaryPredicate = EqualTo<>>
    using BoyerMooreSearcher = std::boyer_moore_searcher<RandomIterator>;

    template <typename RandomIterator, typename H = Hash<typename std::iterator_traits<RandomIterator>::value_type>, typename BinaryPredicate = EqualTo<>>
    using BoyerMooreHorspoolSearcher = std::boyer_moore_horspool_searcher<RandomIterator>;

    using BadFunctionCallException = std::bad_function_call;

    using std::bind_front;
    using std::bind;
    using std::ref;
    using std::cref;
    using std::invoke;
}
