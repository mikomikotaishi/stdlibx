#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    namespace types {
        using ::core::types::MemberFunction;
    }

    using ::core::Function;
    using ::core::MoveOnlyFunction;
    using ::core::MemberFunction;
    using ::core::ReferenceWrapper;
    using ::core::UnwrapReference;
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

    using ::core::Placeholders;

    using ::core::bind_front;
    using ::core::bind;
    using ::core::ref;
    using ::core::cref;
    using ::core::invoke;
}
