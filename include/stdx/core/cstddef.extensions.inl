#pragma once

using stdx::core::Array;
using stdx::core::ConvertibleTo;
using stdx::core::Errc;
using stdx::core::FloatingPoint;
using stdx::core::Integral;
using stdx::core::NumericLimits;
using stdx::core::Optional;
using stdx::core::String;
using stdx::core::StringView;
using stdx::core::StrongOrdering;
using stdx::core::nullopt;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
namespace stdx::core {
    #include "Numbers.inl"
}
