/**
 * @file functional.cppm
 * @module std:core.functional
 * @brief Module file for importing function objects from the standard library.
 *
 * This file contains the implementation of the function object operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.functional;
#else
export module stdlib:core.functional;
#endif

import core;

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    namespace types {
        using ::core::types::MemberFunction;
    }

    using ::core::Function;
    using ::core::MoveOnlyFunction;
    using ::core::MemberFunction;
    using ::core::ReferenceWrapper;
    using ::core::UnwrapReference;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::UnwrapRefDecay;
    #endif
    using ::core::UnwrapReferenceDecay;
    using ::core::IsBindExpression;
    using ::core::IsPlaceholder;
    using ::core::Plus;
    using ::core::Minus;
    using ::core::Multiplies;
    using ::core::Divides;
    using ::core::Modulus;
    using ::core::Negate;
    using ::core::EqualTo;
    using ::core::NotEqualTo;
    using ::core::Greater;
    using ::core::Less;
    using ::core::GreaterEqual;
    using ::core::LessEqual;
    using ::core::CompareThreeWay;
    using ::core::LogicalAnd;
    using ::core::LogicalOr;
    using ::core::BitAnd;
    using ::core::BitOr;
    using ::core::BitNot;
    using ::core::Identity;
    using ::core::DefaultSearcher;
    using ::core::BoyerMooreSearcher;
    using ::core::BoyerMooreHorspoolSearcher;
    using ::core::BadFunctionCallException;

    using ::core::hash;
    using ::core::Hash;

    /**
     * @brief Wrapper namespace for placeholder objects of the Functional namespace (up to 29).
     */
    namespace Placeholders {
        using ::core::Placeholders::_1;
        using ::core::Placeholders::_2;
        using ::core::Placeholders::_3;
        using ::core::Placeholders::_4;
        using ::core::Placeholders::_5;
        using ::core::Placeholders::_6;
        using ::core::Placeholders::_7;
        using ::core::Placeholders::_8;
        using ::core::Placeholders::_9;
        using ::core::Placeholders::_10;
        using ::core::Placeholders::_11;
        using ::core::Placeholders::_12;
        using ::core::Placeholders::_13;
        using ::core::Placeholders::_14;
        using ::core::Placeholders::_15;
        using ::core::Placeholders::_16;
        using ::core::Placeholders::_17;
        using ::core::Placeholders::_18;
        using ::core::Placeholders::_19;
        using ::core::Placeholders::_20;
        using ::core::Placeholders::_21;
        using ::core::Placeholders::_22;
        using ::core::Placeholders::_23;
        using ::core::Placeholders::_24;
        using ::core::Placeholders::_25;
        using ::core::Placeholders::_26;
        using ::core::Placeholders::_27;
        using ::core::Placeholders::_28;
        using ::core::Placeholders::_29;
    }

    using ::core::bind_front;
    using ::core::bind;
    using ::core::ref;
    using ::core::cref;
    using ::core::invoke;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
