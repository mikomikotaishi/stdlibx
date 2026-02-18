#pragma once

using alloc::String;

using core::Errc;
using core::FloatingPoint;
using core::Integral;
using core::NumericLimits;
using core::Optional;
using core::StringView;
using core::StrongOrdering;
using core::nullopt;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #include "Numbers.inl"
}
