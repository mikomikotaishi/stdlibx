#pragma once

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

    template <usize I, typename V>
    using VariantAlternative = std::variant_alternative<I, V>;

    template <usize I, typename T>
    using VariantAlternativeType = std::variant_alternative_t<I, T>;

    using BadVariantAccessException = std::bad_variant_access;

    using std::hash;

    inline constexpr usize VariantNoPosition = std::variant_npos;

    using std::visit;
    using std::holds_alternative;
    using std::get;
    using std::get_if;
    using std::swap;
}
