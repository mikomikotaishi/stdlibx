/**
 * @file functional.cppm
 * @module core:functional
 * @brief Module file for importing function objects from the standard library.
 *
 * This file contains the implementation of the function object operations in the standard library.
 */

module;

#include <functional>

#include "Macros.hpp"

export module core:functional;

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

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using UnwrapRefDecay = std::unwrap_ref_decay<T>;
    #endif

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

    /**
     * @brief Wrapper namespace for placeholder objects of the Functional namespace (up to 29).
     */
    namespace Placeholders {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        using std::placeholders::_4;
        using std::placeholders::_5;
        using std::placeholders::_6;
        using std::placeholders::_7;
        using std::placeholders::_8;
        using std::placeholders::_9;
        using std::placeholders::_10;
        using std::placeholders::_11;
        using std::placeholders::_12;
        using std::placeholders::_13;
        using std::placeholders::_14;
        using std::placeholders::_15;
        using std::placeholders::_16;
        using std::placeholders::_17;
        using std::placeholders::_18;
        using std::placeholders::_19;
        using std::placeholders::_20;
        using std::placeholders::_21;
        using std::placeholders::_22;
        using std::placeholders::_23;
        using std::placeholders::_24;
        using std::placeholders::_25;
        using std::placeholders::_26;
        using std::placeholders::_27;
        using std::placeholders::_28;
        using std::placeholders::_29;
    }

    using std::bind_front;
    using std::bind;
    using std::ref;
    using std::cref;
    using std::invoke;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
