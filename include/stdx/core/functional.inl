#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    namespace types {
        using ::core::prelude::types::MemberFunction;
    }

    using ::core::prelude::Function;
    #ifdef __cpp_lib_move_only_function
    using ::core::prelude::MoveOnlyFunction;
    #endif
    #ifdef __cpp_lib_copyable_function
    using ::core::prelude::CopyableFunction;
    #endif
    #ifdef __cpp_lib_function_ref
    using ::core::prelude::FunctionReference;
    #endif
    using ::core::prelude::MemberFunction;
    using ::core::prelude::ReferenceWrapper;
    using ::core::prelude::UnwrapReference;
    using ::core::prelude::UnwrapReferenceDecay;
    using ::core::prelude::UnwrapReferenceType;
    using ::core::prelude::UnwrapReferenceDecayType;
    using ::core::prelude::IsBindExpression;
    using ::core::prelude::IsPlaceholder;
    using ::core::prelude::Plus;
    using ::core::prelude::Minus;
    using ::core::prelude::Multiplies;
    using ::core::prelude::Divides;
    using ::core::prelude::Modulus;
    using ::core::prelude::Negate;
    using ::core::prelude::EqualTo;
    using ::core::prelude::NotEqualTo;
    using ::core::prelude::Greater;
    using ::core::prelude::Less;
    using ::core::prelude::GreaterEqual;
    using ::core::prelude::LessEqual;
    using ::core::prelude::CompareThreeWay;
    using ::core::prelude::LogicalAnd;
    using ::core::prelude::LogicalOr;
    using ::core::prelude::BitAnd;
    using ::core::prelude::BitOr;
    using ::core::prelude::BitNot;
    using ::core::prelude::Identity;
    using ::core::prelude::DefaultSearcher;
    using ::core::prelude::BoyerMooreSearcher;
    using ::core::prelude::BoyerMooreHorspoolSearcher;
    using ::core::prelude::BadFunctionCallException;

    using ::core::prelude::hash;
    using ::core::prelude::Hash;

    using ::core::prelude::Placeholders;

    using ::core::prelude::bind_front;
    using ::core::prelude::bind;
    using ::core::prelude::ref;
    using ::core::prelude::cref;
    using ::core::prelude::invoke;
    using ::core::prelude::invoke_r;
}
