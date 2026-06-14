/**
 * @file Macros.hpp
 * @brief Macros used in the library
 *
 * This file contains all macros that are used in this library.
 */

#pragma once

#define throws(...) noexcept(__VA_OPT__(!) true)

/**
 * @brief Utility macro to import the stdx::core namespace within the module.
 */
#define STDLIBX_PREPARE_IMPORT_CORE() \
    namespace stdx::core {} \
    using namespace stdx::core;

/**
 * @brief Utility macro to import the stdx::literals namespace within the module.
 */
#define STDLIBX_PREPARE_IMPORT_LITERALS() \
    namespace stdx::literals {} \
    using namespace stdx::literals;

/**
 * @brief Automatically generate an implicit constructor for a class Derived
 * which extends Base.
 *
 * @param Derived The derived class
 * @param Base The base class 
 */
#define STDLIBX_GENERATE_DERIVED_CONSTRUCTOR(Derived, Base) \
    Derived(const Base& x): \
        Base(x) {}

/**
 * @brief A utility to specialise a type in std::formatter. 
 * Requires that Formatter is imported into the current scope.
 * Used as a hack to allow stdx::fmt::Formatter to be used by stdx::fmt::format().
 * 
 * @param Typename The type to specialise into the formatter.
 */
#define SPECIALIZE_FORMATTER(Typename) \
    template <> \
    struct stdx::fmt::formatter<Typename> : public Formatter<Typename> {};

/**
 * @brief A utility to specialise a type in std::hash. 
 * Requires that Hash is imported into the current scope.
 * Used as a hack to allow stdx::core::Hash to be used by hash().
 * 
 * @param Typename The type to specialise into the hash.
 */
#define SPECIALIZE_HASH(Typename) \
    template <> \
    struct stdx::core::hash<Typename> : public Hash<Typename> {};
