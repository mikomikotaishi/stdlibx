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

    class Placeholders {
    public:
        Placeholders() = delete;

        static constexpr auto _1 = std::placeholders::_1;
        static constexpr auto _2 = std::placeholders::_2;
        static constexpr auto _3 = std::placeholders::_3;
        static constexpr auto _4 = std::placeholders::_4;
        static constexpr auto _5 = std::placeholders::_5;
        static constexpr auto _6 = std::placeholders::_6;
        static constexpr auto _7 = std::placeholders::_7;
        static constexpr auto _8 = std::placeholders::_8;
        static constexpr auto _9 = std::placeholders::_9;
        static constexpr auto _10 = std::placeholders::_10;
        static constexpr auto _11 = std::placeholders::_11;
        static constexpr auto _12 = std::placeholders::_12;
        static constexpr auto _13 = std::placeholders::_13;
        static constexpr auto _14 = std::placeholders::_14;
        static constexpr auto _15 = std::placeholders::_15;
        static constexpr auto _16 = std::placeholders::_16;
        static constexpr auto _17 = std::placeholders::_17;
        static constexpr auto _18 = std::placeholders::_18;
        static constexpr auto _19 = std::placeholders::_19;
        static constexpr auto _20 = std::placeholders::_20;
        static constexpr auto _21 = std::placeholders::_21;
        static constexpr auto _22 = std::placeholders::_22;
        static constexpr auto _23 = std::placeholders::_23;
        static constexpr auto _24 = std::placeholders::_24;
        static constexpr auto _25 = std::placeholders::_25;
        static constexpr auto _26 = std::placeholders::_26;
        static constexpr auto _27 = std::placeholders::_27;
        static constexpr auto _28 = std::placeholders::_28;
        static constexpr auto _29 = std::placeholders::_29;
    };

    using std::bind_front;
    using std::bind;
    using std::ref;
    using std::cref;
    using std::invoke;
}
