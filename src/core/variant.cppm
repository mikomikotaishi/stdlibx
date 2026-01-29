/**
 * @file variant.cppm
 * @module core:variant
 * @brief Module file for importing the Variant class from the standard library.
 * 
 * This file exports the Variant class operations in the standard library.
 */

module;

#include <variant>

export module core:variant;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename... Types>
    using Variant = std::variant<Types...>;

    using Monostate = std::monostate;

    template <typename V>
    using VariantSize = std::variant_size<V>;

    template <std::size_t I, typename V>
    using VariantAlternative = std::variant_alternative<I, V>;


    template <std::size_t I, typename T>
    using VariantAlternativeType = std::variant_alternative_t<I, T>;

    using BadVariantAccessException = std::bad_variant_access;

    using std::hash;


    inline constexpr std::size_t VariantNoPosition = std::variant_npos;

    using std::visit;
    using std::holds_alternative;
    using std::get;
    using std::get_if;
    using std::swap;
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
