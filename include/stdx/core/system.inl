#pragma once

using stdx::collections::HashMap;
using stdx::future::LaunchPolicy;
using stdx::io::Cerr;
using stdx::io::Cin;
using stdx::io::Clog;
using stdx::io::Cout;
using stdx::io::IOErrc;
using stdx::io::InputStream;
using stdx::io::File;
using stdx::io::OutputStream;
using stdx::meta::AddRvalueReferenceType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsNothrowAssignableValue;
using stdx::meta::IsNothrowInvocableReturnValue;
using stdx::meta::IsNothrowInvocableValue;
using stdx::meta::IsNothrowMoveConstructibleValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TypeIdentityType;
using stdx::time::Milliseconds;
using stdx::time::Nanoseconds;
using stdx::time::SystemClock;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #include "System.inl"
}
