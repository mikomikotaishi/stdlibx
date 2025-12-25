/**
 * @file Macros.hpp
 * @brief Macros used in the library
 *
 * This file contains all macros that are used in this library.
 */

#pragma once

#define throws(...) noexcept(false)

/**
 * @brief Automatically exports the symbols in the std::core namespace.
 */
#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
#define STDLIBX_STD_MODULE_EXPORT_CORE() export using namespace std::core;
#else
#define STDLIBX_STD_MODULE_EXPORT_CORE() export using namespace stdlib::core;
#endif

/**
 * @brief Automatically exports the symbols in the stdx::core namespace.
 */
#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
#define STDLIBX_STDX_MODULE_EXPORT_CORE() export using namespace stdx::core;
#else
#define STDLIBX_STDX_MODULE_EXPORT_CORE() export using namespace stdlibx::core;
#endif

/**
 * @brief Automatically exports the symbols in the alloc namespace.
 */
#define STDLIBX_ALLOC_MODULE_EXPORT_CORE() export using namespace alloc;

/**
 * @brief Automatically exports the symbols in the core namespace.
 */
#define STDLIBX_CORE_MODULE_EXPORT_CORE() export using namespace core;

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
 * Used as a hack to allow std::fmt::Formatter to be used by std::fmt::format().
 * 
 * @param Namespace The namespace (either std or stdlib).
 * @param Typename The type to specialise into the formatter.
 */
#define SPECIALIZE_FORMATTER(Namespace, Typename) \
    template <> \
    struct Namespace::fmt::formatter<Typename> : public Formatter<Typename> {};

/**
 * @brief A utility to specialise a type in std::hash. 
 * Requires that Hash is imported into the current scope.
 * Used as a hack to allow std::core::Hash to be used by hash().
 * 
 * @param Namespace The namespace (either std or stdlib).
 * @param Typename The type to specialise into the hash.
 */
#define SPECIALIZE_HASH(Namespace, Typename) \
    template <> \
    struct Namespace::core::hash<Typename> : public Hash<Typename> {};
