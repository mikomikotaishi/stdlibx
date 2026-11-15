#pragma once

using stdx::core::SameAs;
using stdx::meta::FixedString;
using stdx::meta::RemoveConstVolatileReferenceType;

namespace stdx::data {
    /**
     * @internal
     * @enum DataFormat
     * @brief Discriminator distinguishing one data format's annotations from another's.
     */
    enum class DataFormat {
        JSON, ///< The JSON data format.
        XML, ///< The XML data format.
        TOML, ///< The TOML data format.
        YAML, ///< The YAML data format.
        CSV, ///< The CSV data format.
    };
}

/**
 * @namespace stdx::data
 * @brief Data format parsing and serialization/deserialization.
 */
export namespace stdx::data {
    /**
     * @struct PropertyNameAnnotation
     * @brief Field annotation overriding the data format key used for a member.
     * @tparam Format The data format this annotation applies to.
     * @tparam Name The key to read and write instead of the member's own name.
     *
     * Applied to a struct member, it makes a given format's serializer emit, and
     * its deserializer read, @p Name in place of the C++ identifier. The
     * @p Format tag means one format never honors another's rename.
     */
    template <DataFormat Format, FixedString Name>
    struct PropertyNameAnnotation {
        static constexpr DataFormat FORMAT = Format; ///< The owning format.
        static constexpr FixedString NAME = Name; ///< The overriding key.

        /**
         * @brief The overriding key.
         * @return The key as a view.
         */
        [[nodiscard]]
        static constexpr StringView str() noexcept {
            return NAME.view();
        }
    };

    /**
     * @struct IgnoreAnnotation
     * @brief Field annotation excluding a member from a format's serialization.
     * @tparam Format The data format this annotation applies to.
     *
     * The @p Format tag is the annotation's only state; it exists solely so that
     * one format's Ignore is a different type from another's, which is what lets
     * serde match its own and ignore the rest.
     */
    template <DataFormat Format>
    struct IgnoreAnnotation {
        static constexpr DataFormat FORMAT = Format; ///< The owning format.
    };

    /**
     * @struct TransientAnnotation
     * @brief Field annotation excluding a member from *every* format's serialization.
     *
     * The format-agnostic counterpart to @ref IgnoreAnnotation: where a tagged
     * Ignore excludes a member from one format only, Transient excludes it from
     * all of them.
     */
    struct TransientAnnotation {};

    /**
     * @brief The {@code [[=Transient]]} annotation.
     *
     * Marks a member as never serialized, in any format. For a format-specific
     * exclusion, use that format's Ignore (e.g. {@code [[=JsonIgnore]]}) instead.
     */
    inline constexpr TransientAnnotation Transient = {};
}

namespace stdx::data {
    /**
     * @internal
     * @brief Whether T is a @ref PropertyNameAnnotation for the given format.
     *
     * A dedicated trait rather than stdx::meta::SpecializationOf because the
     * annotation carries non-type (DataFormat, FixedString) arguments, which the
     * type-only SpecializationOf cannot match; and because matching must pin the
     * format, not merely the template.
     */
    template <typename T, DataFormat Format>
    constexpr bool IsPropertyNameAnnotationValue = false;

    template <DataFormat Format, FixedString Name>
    constexpr bool IsPropertyNameAnnotationValue<PropertyNameAnnotation<Format, Name>, Format> = true;

    /**
     * @concept IsPropertyNameAnnotation
     * @brief Satisfied only by a @ref PropertyNameAnnotation whose format is @p Format.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format that must match.
     */
    template <typename T, DataFormat Format>
    concept IsPropertyNameAnnotation = IsPropertyNameAnnotationValue<RemoveConstVolatileReferenceType<T>, Format>;

    /**
     * @internal
     * @brief Whether T is the @ref IgnoreAnnotation for the given format.
     */
    template <typename T, DataFormat Format>
    constexpr bool IsIgnoreAnnotationValue = false;

    template <DataFormat Format>
    constexpr bool IsIgnoreAnnotationValue<IgnoreAnnotation<Format>, Format> = true;

    /**
     * @concept IsIgnoreAnnotation
     * @brief Satisfied only by the @ref IgnoreAnnotation whose format is @p Format.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format that must match.
     */
    template <typename T, DataFormat Format>
    concept IsIgnoreAnnotation = IsIgnoreAnnotationValue<RemoveConstVolatileReferenceType<T>, Format>;

    /**
     * @concept IsTransientAnnotation
     * @brief Satisfied only by the format-agnostic @ref TransientAnnotation.
     * @tparam T The (cvref-stripped) annotation type to test.
     *
     * Unlike @ref IsIgnoreAnnotation this takes no format: a Transient member is
     * excluded everywhere, so every format matches it the same way.
     */
    template <typename T>
    concept IsTransientAnnotation = SameAs<RemoveConstVolatileReferenceType<T>, TransientAnnotation>;
}
