#pragma once

using core::io::IOErrc;
using core::meta::AddRvalueReferenceType;
using core::meta::IsNothrowAssignableValue;
using core::meta::IsNothrowInvocableReturnValue;
using core::meta::IsNothrowInvocableValue;
using core::meta::IsNothrowMoveConstructibleValue;
using core::meta::RemoveReferenceType;

/**
 * @namespace core::prelude::util
 * @brief Wrapper namespace for the core objects of the standard library.
 */
namespace core::prelude::util {
    #include "Ops.inl"
}
