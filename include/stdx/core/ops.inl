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
    #include "Ops.inl"
}
