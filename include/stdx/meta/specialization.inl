#pragma once

/**
 * @namespace stdx::meta
 * @brief Standard library metaprogramming operations.
 */
namespace stdx::meta {
    /**
     * @internal
     * @brief Whether T is a specialization of the class template Templ.
     * @tparam T The type to test.
     * @tparam Templ The class template
     */
    template <typename T, template <typename...> typename Templ>
    constexpr bool IsSpecializationOfValue = false;

    template <template <typename...> typename Templ, typename... Args>
    constexpr bool IsSpecializationOfValue<Templ<Args...>, Templ> = true;

    /**
     * @internal
     * @concept SpecializationOf
     * @brief Satisfied when T is a specialization of the class template Template.
     * @tparam T The type to test.
     * @tparam Templ The class template, e.g. Optional or Vector.
     */
    template <typename T, template <typename...> typename Templ>
    concept SpecializationOf = IsSpecializationOfValue<RemoveConstVolatileReferenceType<T>, Templ>;

    /**
     * @internal
     * @concept IsOptional
     * @brief Satisfied by Optional specializations.
     * @tparam T The type to test.
     */
    template <typename T>
    concept IsOptional = SpecializationOf<T, Optional>;

    /**
     * @struct OptionalValueType
     * @brief Extracts the value type U from an Optional<U>.
     * @tparam T An Optional specialization.
     */
    template <IsOptional T>
    struct OptionalValueType {
        using Value = typename RemoveConstVolatileReferenceType<T>::value_type;
    };

    /**
     * @brief The value type of an Optional<U> (i.e. U).
     * @tparam T An Optional specialization.
     */
    template <IsOptional T>
    using OptionalValue = typename OptionalValueType<T>::Value;
}
