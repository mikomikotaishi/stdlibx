/**
 * @file type_traits.cppm
 * @module core:meta.type_traits
 * @brief Module file for standard library type traits operations.
 *
 * This file contains the implementation of the type traits operations in the standard library.
 */

module;

#include <type_traits>

export module core:meta.type_traits;

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    template <typename T, T v>
    using IntegralConstant = std::integral_constant<T, v>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <bool B>
    using BoolConstant = std::bool_constant<B>;
    #endif

    template <bool B>
    using BooleanConstant = std::bool_constant<B>;

    using TrueType = std::true_type;
    using FalseType = std::false_type;

    template <typename T>
    using IsVoid = std::is_void<T>;

    template <typename T>
    constexpr bool IsVoidValue = std::is_void_v<T>;

    template <typename T>
    using IsNullPointer = std::is_null_pointer<T>;

    template <typename T>
    constexpr bool IsNullPointerValue = std::is_null_pointer_v<T>;

    template <typename T>
    using IsIntegral = std::is_integral<T>;

    template <typename T>
    constexpr bool IsIntegralValue = std::is_integral_v<T>;

    template <typename T>
    using IsFloatingPoint = std::is_floating_point<T>;

    template <typename T>
    constexpr bool IsFloatingPointValue = std::is_floating_point_v<T>;

    template <typename T>
    using IsArray = std::is_array<T>;

    template <typename T>
    constexpr bool IsArrayValue = std::is_array_v<T>;

    template <typename T>
    using IsEnum = std::is_enum<T>;

    template <typename T>
    constexpr bool IsEnumValue = std::is_enum_v<T>;

    template <typename T>
    using IsUnion = std::is_union<T>;

    template <typename T>
    constexpr bool IsUnionValue = std::is_union_v<T>;

    template <typename T>
    using IsClass = std::is_class<T>;

    template <typename T>
    constexpr bool IsClassValue = std::is_class_v<T>;

    template <typename T>
    using IsFunction = std::is_function<T>;

    template <typename T>
    constexpr bool IsFunctionValue = std::is_function_v<T>;

    template <typename T>
    using IsPointer = std::is_pointer<T>;

    template <typename T>
    constexpr bool IsPointerValue = std::is_pointer_v<T>;

    template <typename T>
    using IsLvalueReference = std::is_lvalue_reference<T>;

    template <typename T>
    constexpr bool IsLvalueReferenceValue = std::is_lvalue_reference_v<T>;

    template <typename T>
    using IsRvalueReference = std::is_rvalue_reference<T>;

    template <typename T>
    constexpr bool IsRvalueReferenceValue = std::is_rvalue_reference_v<T>;

    template <typename T>
    using IsMemberObjectPointer = std::is_member_object_pointer<T>;

    template <typename T>
    constexpr bool IsMemberObjectPointerValue = std::is_member_object_pointer_v<T>;

    template <typename T>
    using IsMemberFunctionPointer = std::is_member_function_pointer<T>;

    template <typename T>
    constexpr bool IsMemberFunctionPointerValue = std::is_member_function_pointer_v<T>;

    template <typename T>
    using IsFundamental = std::is_fundamental<T>;

    template <typename T>
    constexpr bool IsFundamentalValue = std::is_fundamental_v<T>;

    template <typename T>
    using IsArithmetic = std::is_arithmetic<T>;

    template <typename T>
    constexpr bool IsArithmeticValue = std::is_arithmetic_v<T>;

    template <typename T>
    using IsScalar = std::is_scalar<T>;

    template <typename T>
    constexpr bool IsScalarValue = std::is_scalar_v<T>;

    template <typename T>
    using IsObject = std::is_object<T>;

    template <typename T>
    constexpr bool IsObjectValue = std::is_object_v<T>;

    template <typename T>
    using IsCompound = std::is_compound<T>;

    template <typename T>
    constexpr bool IsCompoundValue = std::is_compound_v<T>;

    template <typename T>
    using IsReference = std::is_reference<T>;

    template <typename T>
    constexpr bool IsReferenceValue = std::is_reference_v<T>;

    template <typename T>
    using IsMemberPointer = std::is_member_pointer<T>;

    template <typename T>
    constexpr bool IsMemberPointerValue = std::is_member_pointer_v<T>;

    template <typename T>
    using IsConst = std::is_const<T>;

    template <typename T>
    constexpr bool IsConstValue = std::is_const_v<T>;

    template <typename T>
    using IsVolatile = std::is_volatile<T>;

    template <typename T>
    constexpr bool IsVolatileValue = std::is_volatile_v<T>;

    template <typename T>
    using IsTrivial = std::is_trivial<T>;

    template <typename T>
    constexpr bool IsTrivialValue = std::is_trivial_v<T>;

    template <typename T>
    using IsTriviallyCopyable = std::is_trivially_copyable<T>;

    template <typename T>
    constexpr bool IsTriviallyCopyableValue = std::is_trivially_copyable_v<T>;

    template <typename T>
    using IsStandardLayout = std::is_standard_layout<T>;

    template <typename T>
    constexpr bool IsStandardLayoutValue = std::is_standard_layout_v<T>;

    template <typename T>
    using IsPOD = std::is_pod<T>;

    template <typename T>
    constexpr bool IsPODValue = std::is_pod_v<T>;

    template <typename T>
    using HasUniqueObjectRepresentations = std::has_unique_object_representations<T>;

    template <typename T>
    constexpr bool HasUniqueObjectRepresentationsValue = std::has_unique_object_representations_v<T>;

    template <typename T>
    using IsEmpty = std::is_empty<T>;

    template <typename T>
    constexpr bool IsEmptyValue = std::is_empty_v<T>;

    template <typename T>
    using IsPolymorphic = std::is_polymorphic<T>;

    template <typename T>
    constexpr bool IsPolymorphicValue = std::is_polymorphic_v<T>;

    template <typename T>
    using IsAbstract = std::is_abstract<T>;

    template <typename T>
    constexpr bool IsAbstractValue = std::is_abstract_v<T>;

    template <typename T>
    using IsFinal = std::is_final<T>;

    template <typename T>
    constexpr bool IsFinalValue = std::is_final_v<T>;

    template <typename T>
    using IsAggregate = std::is_aggregate<T>;

    template <typename T>
    constexpr bool IsAggregateValue = std::is_aggregate_v<T>;

    template <typename T>
    using IsSigned = std::is_signed<T>;

    template <typename T>
    constexpr bool IsSignedValue = std::is_signed_v<T>;

    template <typename T>
    using IsUnsigned = std::is_unsigned<T>;

    template <typename T>
    constexpr bool IsUnsignedValue = std::is_unsigned_v<T>;

    template <typename T>
    using IsBoundedArray = std::is_bounded_array<T>;

    template <typename T>
    constexpr bool IsBoundedArrayValue = std::is_bounded_array_v<T>;

    template <typename T>
    using IsUnboundedArray = std::is_unbounded_array<T>;

    template <typename T>
    constexpr bool IsUnboundedArrayValue = std::is_unbounded_array_v<T>;

    template <typename T>
    using IsConstructible = std::is_constructible<T>;

    template <typename T>
    constexpr bool IsConstructibleValue = std::is_constructible_v<T>;

    template <typename T>
    using IsTriviallyConstructible = std::is_trivially_constructible<T>;

    template <typename T>
    constexpr bool IsTriviallyConstructibleValue = std::is_trivially_constructible_v<T>;

    template <typename T>
    using IsNothrowConstructible = std::is_nothrow_constructible<T>;

    template <typename T>
    constexpr bool IsNothrowConstructibleValue = std::is_nothrow_constructible_v<T>;

    template <typename T>
    using IsDefaultConstructible = std::is_default_constructible<T>;

    template <typename T>
    constexpr bool IsDefaultConstructibleValue = std::is_default_constructible_v<T>;

    template <typename T>
    using IsTriviallyDefaultConstructible = std::is_trivially_default_constructible<T>;

    template <typename T>
    constexpr bool IsTriviallyDefaultConstructibleValue = std::is_trivially_default_constructible_v<T>;

    template <typename T>
    using IsNothrowDefaultConstructible = std::is_nothrow_default_constructible<T>;

    template <typename T>
    constexpr bool IsNothrowDefaultConstructibleValue = std::is_nothrow_default_constructible_v<T>;

    template <typename T>
    using IsCopyConstructible = std::is_copy_constructible<T>;

    template <typename T>
    constexpr bool IsCopyConstructibleValue = std::is_copy_constructible_v<T>;

    template <typename T>
    using IsTriviallyCopyConstructible = std::is_trivially_copy_constructible<T>;

    template <typename T>
    constexpr bool IsTriviallyCopyConstructibleValue = std::is_trivially_copy_constructible_v<T>;

    template <typename T>
    using IsNothrowCopyConstructible = std::is_nothrow_copy_constructible<T>;

    template <typename T>
    constexpr bool IsNothrowCopyConstructibleValue = std::is_nothrow_copy_constructible_v<T>;

    template <typename T>
    using IsMoveConstructible = std::is_move_constructible<T>;

    template <typename T>
    constexpr bool IsMoveConstructibleValue = std::is_move_constructible_v<T>;
    
    template <typename T>
    using IsTriviallyMoveConstructible = std::is_trivially_move_constructible<T>;

    template <typename T>
    constexpr bool IsTriviallyMoveConstructibleValue = std::is_trivially_move_constructible_v<T>;
    
    template <typename T>
    using IsNothrowMoveConstructible = std::is_nothrow_move_constructible<T>;

    template <typename T>
    constexpr bool IsNothrowMoveConstructibleValue = std::is_nothrow_move_constructible_v<T>;

    template <typename T>
    using IsDestructible = std::is_destructible<T>;

    template <typename T>
    constexpr bool IsDestructibleValue = std::is_destructible_v<T>;

    template <typename T>
    using IsTriviallyDestructible = std::is_trivially_destructible<T>;

    template <typename T>
    constexpr bool IsTriviallyDestructibleValue = std::is_trivially_destructible_v<T>;

    template <typename T>
    using IsNothrowDestructible = std::is_nothrow_destructible<T>;

    template <typename T>
    constexpr bool IsNothrowDestructibleValue = std::is_nothrow_destructible_v<T>;

    template <typename T>
    using HasVirtualDestructor = std::has_virtual_destructor<T>;

    template <typename T>
    constexpr bool HasVirtualDestructorValue = std::has_virtual_destructor_v<T>;

    template <typename T, typename U>
    using IsSwappableWith = std::is_swappable_with<T, U>;

    template <typename T, typename U>
    constexpr bool IsSwappableWithValue = std::is_swappable_with_v<T, U>;

    template <typename T>
    using IsSwappable = std::is_swappable<T>;

    template <typename T>
    constexpr bool IsSwappableValue = std::is_swappable_v<T>;

    template <typename T, typename U>
    using IsNothrowSwappableWith = std::is_nothrow_swappable_with<T, U>;

    template <typename T, typename U>
    constexpr bool IsNothrowSwappableWithValue = std::is_nothrow_swappable_with_v<T, U>;

    template <typename T>
    using IsNothrowSwappable = std::is_nothrow_swappable<T>;

    template <typename T>
    constexpr bool IsNothrowSwappableValue = std::is_nothrow_swappable_v<T>;

    template <typename T>
    using AlignmentOf = std::alignment_of<T>;

    template <typename T>
    constexpr bool AlignmentOfValue = std::alignment_of_v<T>;

    template <typename T>
    using Rank = std::rank<T>;

    template <typename T>
    constexpr bool RankValue = std::rank_v<T>;

    template <typename T, unsigned int Index = 0>
    using Extent = std::extent<T, Index>;

    template <typename T, unsigned int Index = 0>
    constexpr bool ExtentValue = std::extent_v<T, Index>;

    template <typename T, typename U>
    using IsSame = std::is_same<T, U>;

    template <typename T, typename U>
    constexpr bool IsSameValue = std::is_same_v<T, U>;

    template <typename Base, typename Derived>
    using IsBaseOf = std::is_base_of<Base, Derived>;

    template <typename Base, typename Derived>
    constexpr bool IsBaseOfValue = std::is_base_of_v<Base, Derived>;

    template <typename From, typename To>
    using IsConvertible = std::is_convertible<From, To>;

    template <typename From, typename To>
    constexpr bool IsConvertibleValue = std::is_convertible_v<From, To>;

    template <typename From, typename To>
    using IsNothrowConvertible = std::is_nothrow_convertible<From, To>;

    template <typename From, typename To>
    constexpr bool IsNothrowConvertibleValue = std::is_nothrow_convertible_v<From, To>;

    template <typename T, typename U>
    using IsLayoutCompatible = std::is_layout_compatible<T, U>;

    template <typename T, typename U>
    constexpr bool IsLayoutCompatibleValue = std::is_layout_compatible_v<T, U>;

    template <typename Base, typename Derived>
    using IsPointerInterconvertibleBaseOf = std::is_pointer_interconvertible_base_of<Base, Derived>;

    template <typename Base, typename Derived>
    constexpr bool IsPointerInterconvertibleBaseOfValue = std::is_pointer_interconvertible_base_of_v<Base, Derived>;

    template <typename Fn, typename... ArgTypes>
    using IsInvocable = std::is_invocable<Fn, ArgTypes...>;

    template <typename Fn, typename... ArgTypes>
    constexpr bool IsInvocableValue = std::is_invocable_v<Fn, ArgTypes...>;

    template <typename R, typename Fn, typename... ArgTypes>
    using IsInvocableReturn = std::is_invocable_r<R, Fn, ArgTypes...>;

    template <typename R, typename Fn, typename... ArgTypes>
    constexpr bool IsInvocableReturnValue = std::is_invocable_r_v<R, Fn, ArgTypes...>;

    template <typename Fn, typename... ArgTypes>
    using IsNothrowInvocable = std::is_nothrow_invocable<Fn, ArgTypes...>;

    template <typename Fn, typename... ArgTypes>
    constexpr bool IsNothrowInvocableValue = std::is_nothrow_invocable_v<Fn, ArgTypes...>;

    template <typename R, typename Fn, typename... ArgTypes>
    using IsNothrowInvocableReturn = std::is_nothrow_invocable_r<R, Fn, ArgTypes...>;

    template <typename R, typename Fn, typename... ArgTypes>
    constexpr bool IsNothrowInvocableReturnValue = std::is_nothrow_invocable_r_v<R, Fn, ArgTypes...>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using RemoveConst_t = std::remove_const_t<T>;

    template <typename T>
    using RemoveVolatile_t = std::remove_volatile_t<T>;

    template <typename T>
    using AddConst_t = std::add_const_t<T>;

    template <typename T>
    using AddVolatile_t = std::add_volatile_t<T>;

    template <typename T>
    using RemoveReference_t = std::remove_reference_t<T>;

    template <typename T>
    using AddLvalueReference_t = std::add_lvalue_reference_t<T>;

    template <typename T>
    using AddRvalueReference_t = std::add_rvalue_reference_t<T>;

    template <typename T>
    using AddPointer_t = std::add_pointer_t<T>;

    template <typename T>
    using MakeSigned_t = std::make_signed_t<T>;

    template <typename T>
    using MakeUnsigned_t = std::make_unsigned_t<T>;

    template <typename T>
    using RemoveExtent_t = std::remove_extent_t<T>;

    template <typename T>
    using RemoveAllExtents_t = std::remove_all_extents_t<T>;
    #endif

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using RemoveCV = std::remove_cv<T>;

    template <typename T>
    using RemoveCV_t = std::remove_cv_t<T>;

    template <typename T>
    using RemoveConstVolatile_t = std::remove_cv_t<T>;
    #endif

    template <typename T>
    using RemoveConstVolatile = std::remove_cv<T>;

    template <typename T>
    using RemoveConstVolatileType = std::remove_cv_t<T>;

    template <typename T>
    using RemoveConst = std::remove_const<T>;

    template <typename T>
    using RemoveConstType = std::remove_const_t<T>;

    template <typename T>
    using RemoveVolatile = std::remove_volatile<T>;

    template <typename T>
    using RemoveVolatileType = std::remove_volatile_t<T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using AddCV = std::add_cv<T>;

    template <typename T>
    using AddCV_t = std::add_cv_t<T>;

    template <typename T>
    using AddConstVolatile_t = std::add_cv_t<T>;
    #endif

    template <typename T>
    using AddConstVolatile = std::add_cv<T>;

    template <typename T>
    using AddConstVolatileType = std::add_cv_t<T>;

    template <typename T>
    using AddConst = std::add_const<T>;

    template <typename T>
    using AddConstType = std::add_const_t<T>;

    template <typename T>
    using AddVolatile = std::add_volatile<T>;

    template <typename T>
    using AddVolatileType = std::add_volatile_t<T>;

    template <typename T>
    using RemoveReference = std::remove_reference<T>;

    template <typename T>
    using RemoveReferenceType = std::remove_reference_t<T>;

    template <typename T>
    using AddLvalueReference = std::add_lvalue_reference<T>;

    template <typename T>
    using AddLvalueReferenceType = std::add_lvalue_reference_t<T>;

    template <typename T>
    using AddRvalueReference = std::add_rvalue_reference<T>;

    template <typename T>
    using AddRvalueReferenceType = std::add_lvalue_reference_t<T>;

    template <typename T>
    using RemovePointer = std::remove_pointer<T>;

    template <typename T>
    using RemovePointerType = std::remove_pointer_t<T>;

    template <typename T>
    using AddPointer = std::add_pointer<T>;

    template <typename T>
    using AddPointerType = std::add_pointer_t<T>;

    template <typename T>
    using MakeSigned = std::make_signed<T>;

    template <typename T>
    using MakeSignedType = std::make_signed_t<T>;

    template <typename T>
    using MakeUnsigned = std::make_unsigned<T>;

    template <typename T>
    using MakeUnsignedType = std::make_unsigned_t<T>;

    template <typename T>
    using RemoveExtent = std::remove_extent<T>;

    template <typename T>
    using RemoveExtentType = std::remove_extent_t<T>;

    template <typename T>
    using RemoveAllExtents = std::remove_all_extents<T>;

    template <typename T>
    using RemoveAllExtentsType = std::remove_all_extents_t<T>;

    template <typename T>
    using Decay = std::decay<T>;

    template <typename T>
    using DecayType = std::decay_t<T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using RemoveCVRef = std::remove_cvref<T>;

    template <typename T>
    using RemoveCVRef_t = std::remove_cvref_t<T>;
    #endif

    template <typename T>
    using RemoveConstVolatileReference = std::remove_cvref<T>;

    template <typename T>
    using RemoveConstVolatileReferenceType = std::remove_cvref_t<T>;

    template <bool B, typename T>
    using EnableIf = std::enable_if<B, T>;

    template <bool B, typename T = void>
    using EnableIfType = std::enable_if_t<B, T>;

    template <bool Cond, typename IfTrue, typename IfFalse>
    using Conditional = std::conditional<Cond, IfTrue, IfFalse>;

    template <bool Cond, typename IfTrue, typename IfFalse>
    using ConditionalType = std::conditional_t<Cond, IfTrue, IfFalse>;

    template <typename T>
    using UnderlyingType = std::underlying_type<T>;

    template <typename T>
    using UnderlyingTypeType = std::underlying_type_t<T>;

    template <typename T, typename U>
    using CommonType = std::common_type<T, U>;

    template <typename... T>
    using CommonTypeType = std::common_type_t<T...>;

    template <typename... T>
    using CommonReference = std::common_reference<T...>;

    template <typename... T>
    using CommonReferenceType = std::common_reference_t<T...>;

    template <typename T, typename U, template <typename> class TQual, template <typename> class UQual>
    using BasicCommonReference = std::basic_common_reference<T, U, TQual, UQual>;

    template <typename Functor, typename... ArgTypes>
    using InvokeResult = std::invoke_result<Functor, ArgTypes...>;

    template <typename Functor, typename... ArgTypes>
    using InvokeResultType = std::invoke_result_t<Functor, ArgTypes...>;

    using VoidType = std::void_t<>;

    template <typename T>
    using TypeIdentity = std::type_identity<T>;

    template <typename T>
    using TypeIdentityType = std::type_identity_t<T>;

    template <typename T>
    using UnwrapReference = std::unwrap_reference<T>;

    template <typename T>
    using UnwrapReferenceType = std::unwrap_reference_t<T>;

    template <typename Bn>
    using Conjunction = std::conjunction<Bn>;

    template <typename Bn>
    constexpr bool ConjunctionValue = std::conjunction_v<Bn>;

    template <typename Bn>
    using Disjunction = std::disjunction<Bn>;

    template <typename Bn>
    constexpr bool DisjunctionValue = std::disjunction_v<Bn>;

    template <typename Bn>
    using Negation = std::negation<Bn>;

    template <typename Bn>
    constexpr bool NegationValue = std::negation_v<Bn>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Bn>
    constexpr bool Conjunction_v = std::conjunction_v<Bn>;

    template <typename Bn>
    constexpr bool Disjunction_v = std::disjunction_v<Bn>;

    template <typename Bn>
    constexpr bool Negation_v = std::negation_v<Bn>;
    #endif

    using std::is_constant_evaluated;
}
