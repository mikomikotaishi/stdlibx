/**
 * @file type_traits.cppm
 * @module std:meta.type_traits
 * @brief Module file for standard library type traits operations.
 *
 * This file contains the implementation of the type traits operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:meta.type_traits;
#else
export module stdlib:meta.type_traits;
#endif

import core;

/**
 * @namespace std::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::meta {
#else 
export namespace stdlib::meta {
#endif
    using core::meta::IntegralConstant;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::BoolConstant;
    #endif
    using core::meta::BooleanConstant;
    using core::meta::TrueType;
    using core::meta::FalseType;
    using core::meta::IsVoid;
    using core::meta::IsVoidValue;
    using core::meta::IsNullPointer;
    using core::meta::IsNullPointerValue;
    using core::meta::IsIntegral;
    using core::meta::IsIntegralValue;
    using core::meta::IsFloatingPoint;
    using core::meta::IsFloatingPointValue;
    using core::meta::IsArray;
    using core::meta::IsArrayValue;
    using core::meta::IsEnum;
    using core::meta::IsEnumValue;
    using core::meta::IsUnion;
    using core::meta::IsUnionValue;
    using core::meta::IsClass;
    using core::meta::IsClassValue;
    using core::meta::IsFunction;
    using core::meta::IsFunctionValue;
    using core::meta::IsPointer;
    using core::meta::IsPointerValue;
    using core::meta::IsLvalueReference;
    using core::meta::IsLvalueReferenceValue;
    using core::meta::IsRvalueReference;
    using core::meta::IsRvalueReferenceValue;
    using core::meta::IsMemberObjectPointer;
    using core::meta::IsMemberObjectPointerValue;
    using core::meta::IsMemberFunctionPointer;
    using core::meta::IsMemberFunctionPointerValue;
    using core::meta::IsFundamental;
    using core::meta::IsFundamentalValue;
    using core::meta::IsArithmetic;
    using core::meta::IsArithmeticValue;
    using core::meta::IsScalar;
    using core::meta::IsScalarValue;
    using core::meta::IsObject;
    using core::meta::IsObjectValue;
    using core::meta::IsCompound;
    using core::meta::IsCompoundValue;
    using core::meta::IsReference;
    using core::meta::IsReferenceValue;
    using core::meta::IsMemberPointer;
    using core::meta::IsMemberPointerValue;
    using core::meta::IsConst;
    using core::meta::IsConstValue;
    using core::meta::IsVolatile;
    using core::meta::IsVolatileValue;
    using core::meta::IsTrivial;
    using core::meta::IsTrivialValue;
    using core::meta::IsTriviallyCopyable;
    using core::meta::IsTriviallyCopyableValue;
    using core::meta::IsStandardLayout;
    using core::meta::IsStandardLayoutValue;
    using core::meta::IsPOD;
    using core::meta::IsPODValue;
    using core::meta::HasUniqueObjectRepresentations;
    using core::meta::HasUniqueObjectRepresentationsValue;
    using core::meta::IsEmpty;
    using core::meta::IsEmptyValue;
    using core::meta::IsPolymorphic;
    using core::meta::IsPolymorphicValue;
    using core::meta::IsAbstract;
    using core::meta::IsAbstractValue;
    using core::meta::IsFinal;
    using core::meta::IsFinalValue;
    using core::meta::IsAggregate;
    using core::meta::IsAggregateValue;
    using core::meta::IsSigned;
    using core::meta::IsSignedValue;
    using core::meta::IsUnsigned;
    using core::meta::IsUnsignedValue;
    using core::meta::IsBoundedArray;
    using core::meta::IsBoundedArrayValue;
    using core::meta::IsUnboundedArray;
    using core::meta::IsUnboundedArrayValue;
    using core::meta::IsConstructible;
    using core::meta::IsConstructibleValue;
    using core::meta::IsTriviallyConstructible;
    using core::meta::IsTriviallyConstructibleValue;
    using core::meta::IsNothrowConstructible;
    using core::meta::IsNothrowConstructibleValue;
    using core::meta::IsDefaultConstructible;
    using core::meta::IsDefaultConstructibleValue;
    using core::meta::IsTriviallyDefaultConstructible;
    using core::meta::IsTriviallyDefaultConstructibleValue;
    using core::meta::IsNothrowDefaultConstructible;
    using core::meta::IsNothrowDefaultConstructibleValue;
    using core::meta::IsCopyConstructible;
    using core::meta::IsCopyConstructibleValue;
    using core::meta::IsTriviallyCopyConstructible;
    using core::meta::IsTriviallyCopyConstructibleValue;
    using core::meta::IsNothrowCopyConstructible;
    using core::meta::IsNothrowCopyConstructibleValue;
    using core::meta::IsMoveConstructible;
    using core::meta::IsMoveConstructibleValue;
    using core::meta::IsTriviallyMoveConstructible;
    using core::meta::IsTriviallyMoveConstructibleValue;
    using core::meta::IsNothrowMoveConstructible;
    using core::meta::IsNothrowMoveConstructibleValue;
    using core::meta::IsDestructible;
    using core::meta::IsDestructibleValue;
    using core::meta::IsTriviallyDestructible;
    using core::meta::IsTriviallyDestructibleValue;
    using core::meta::IsNothrowDestructible;
    using core::meta::IsNothrowDestructibleValue;
    using core::meta::HasVirtualDestructor;
    using core::meta::HasVirtualDestructorValue;
    using core::meta::IsSwappableWith;
    using core::meta::IsSwappableWithValue;
    using core::meta::IsSwappable;
    using core::meta::IsSwappableValue;
    using core::meta::IsNothrowSwappableWith;
    using core::meta::IsNothrowSwappableWithValue;
    using core::meta::IsNothrowSwappable;
    using core::meta::IsNothrowSwappableValue;
    using core::meta::AlignmentOf;
    using core::meta::AlignmentOfValue;
    using core::meta::Rank;
    using core::meta::RankValue;
    using core::meta::Extent;
    using core::meta::ExtentValue;
    using core::meta::IsSame;
    using core::meta::IsSameValue;
    using core::meta::IsBaseOf;
    using core::meta::IsBaseOfValue;
    using core::meta::IsConvertible;
    using core::meta::IsConvertibleValue;
    using core::meta::IsNothrowConvertible;
    using core::meta::IsNothrowConvertibleValue;
    using core::meta::IsLayoutCompatible;
    using core::meta::IsLayoutCompatibleValue;
    using core::meta::IsPointerInterconvertibleBaseOf;
    using core::meta::IsPointerInterconvertibleBaseOfValue;
    using core::meta::IsInvocable;
    using core::meta::IsInvocableValue;
    using core::meta::IsInvocableReturn;
    using core::meta::IsInvocableReturnValue;
    using core::meta::IsNothrowInvocableReturn;
    using core::meta::IsNothrowInvocableReturnValue;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::RemoveConst_t;
    using core::meta::RemoveVolatile_t;
    using core::meta::AddConst_t;
    using core::meta::AddVolatile_t;
    using core::meta::RemoveReference_t;
    using core::meta::AddLvalueReference_t;
    using core::meta::AddRvalueReference_t;
    using core::meta::AddPointer_t;
    using core::meta::MakeSigned_t;
    using core::meta::MakeUnsigned_t;
    using core::meta::RemoveExtent_t;
    using core::meta::RemoveAllExtents_t;
    #endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::RemoveCV;
    using core::meta::RemoveCV_t;
    using core::meta::RemoveConstVolatile_t;
    #endif
    using core::meta::RemoveConstVolatile;
    using core::meta::RemoveConstVolatileType;
    using core::meta::RemoveConst;
    using core::meta::RemoveConstType;
    using core::meta::RemoveVolatile;
    using core::meta::RemoveVolatileType;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::AddCV;
    using core::meta::AddCV_t;
    using core::meta::AddConstVolatile_t;
    #endif
    using core::meta::AddConstVolatile;
    using core::meta::AddConstVolatileType;
    using core::meta::AddConst;
    using core::meta::AddConstType;
    using core::meta::AddVolatile;
    using core::meta::AddVolatileType;
    using core::meta::RemoveReference;
    using core::meta::RemoveReferenceType;
    using core::meta::AddLvalueReference;
    using core::meta::AddLvalueReferenceType;
    using core::meta::AddRvalueReference;
    using core::meta::AddRvalueReferenceType;
    using core::meta::RemovePointer;
    using core::meta::RemovePointerType;
    using core::meta::AddPointer;
    using core::meta::AddPointerType;
    using core::meta::MakeSigned;
    using core::meta::MakeSignedType;
    using core::meta::MakeUnsigned;
    using core::meta::MakeUnsignedType;
    using core::meta::RemoveExtent;
    using core::meta::RemoveExtentType;
    using core::meta::RemoveAllExtents;
    using core::meta::RemoveAllExtentsType;
    using core::meta::Decay;
    using core::meta::DecayType;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::RemoveCVRef;
    using core::meta::RemoveCVRef_t;
    #endif
    using core::meta::RemoveConstVolatileReference;
    using core::meta::RemoveConstVolatileReferenceType;
    using core::meta::EnableIf;
    using core::meta::EnableIfType;
    using core::meta::Conditional;
    using core::meta::ConditionalType;
    using core::meta::UnderlyingType;
    using core::meta::UnderlyingTypeType;
    using core::meta::CommonType;
    using core::meta::CommonTypeType;
    using core::meta::CommonReference;
    using core::meta::CommonReferenceType;
    using core::meta::BasicCommonReference;
    using core::meta::InvokeResult;
    using core::meta::InvokeResultType;
    using core::meta::VoidType;
    using core::meta::TypeIdentity;
    using core::meta::TypeIdentityType;
    using core::meta::UnwrapReference;
    using core::meta::UnwrapReferenceType;
    using core::meta::Conjunction;
    using core::meta::ConjunctionValue;
    using core::meta::Disjunction;
    using core::meta::DisjunctionValue;
    using core::meta::Negation;
    using core::meta::NegationValue;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::meta::Conjunction_v;
    using core::meta::Disjunction_v;
    using core::meta::Negation_v;
    #endif

    using core::meta::is_constant_evaluated;
}
