#pragma once

#ifdef __cpp_lib_reflection
using stdx::collections::Vector;
using stdx::meta::FixedString;
using stdx::meta::IsEnumValue;
using stdx::meta::IsOptional;
using stdx::meta::OptionalValue;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::SpecializationOf;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Annotation;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Enumerator;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::ReflectableAsClass;

/**
 * @namespace stdx::data::toml
 * @brief TOML parsing, construction, and serialization (TOML v1.0.0).
 *
 * Reflection-driven conversion between a @ref Toml value and an arbitrary C++
 * aggregate, mirroring the JSON module's serde with the TOML format tag and the
 * three date-time kinds added as pass-through scalars. Compiled only where the
 * toolchain provides C++26 reflection; the DOM, parser, and serializer never
 * depend on it. TOML has no null, so an empty Optional member is omitted from
 * the table entirely rather than written as a null.
 */
export namespace stdx::data::toml {
    /**
     * @brief The {@code [[=TomlPropertyName<...>]]} annotation.
     * @tparam Name The key to read and write instead of the member's own name.
     */
    template <FixedString Name>
    inline constexpr PropertyNameAnnotation<DataFormat::TOML, Name> TomlPropertyName = {};

    /**
     * @brief The {@code [[=TomlIgnore]]} annotation.
     */
    inline constexpr IgnoreAnnotation<DataFormat::TOML> TomlIgnore = {};

    /**
     * @brief The {@code [[=TomlFlatten]]} annotation: inline a nested struct's
     * members into the parent table, in TOML only.
     * @see stdx::data::Flatten for every format at once.
     */
    inline constexpr FormatFlattenAnnotation<DataFormat::TOML> TomlFlatten = {};
}

namespace stdx::data::toml {
    /**
     * @internal
     * @concept SerdeOptional
     * @brief Satisfied by Optional specializations; an empty one is omitted.
     */
    template <typename T>
    concept SerdeOptional = IsOptional<T>;

    /**
     * @internal
     * @concept SerdeSequence
     * @brief Satisfied by Vector specializations, mapped to a TOML array.
     */
    template <typename T>
    concept SerdeSequence = SpecializationOf<T, std::vector>;

    /**
     * @internal
     * @brief The element type of a Vector serde-sequence.
     */
    template <SerdeSequence T>
    using SerdeSequenceValue = typename RemoveConstVolatileReferenceType<T>::value_type;

    /**
     * @internal
     * @brief A type stored directly as a TOML scalar kind (no recursion needed).
     *
     * The four date-time types are first-class TOML values, so a field of one of
     * them round-trips through the variant without any string conversion, just
     * like a bool or number.
     */
    template <typename T>
    concept SerdeNative = IsAnyOf<T, bool, String,
        Toml::LocalDate, Toml::LocalTime, Toml::LocalDateTime, Toml::OffsetDateTime>
        || Integral<T>
        || FloatingPoint<T>;

    /**
     * @internal
     * @brief Converts an enum to its enumerator name, or deserializes from it.
     */
    template <typename E>
        requires IsEnumValue<E>
    [[nodiscard]]
    Toml enum_to_toml(E value) {
        Toml result;
        template for (constexpr Enumerator e: Ops::static_array(Enum<E>().enumerators())) {
            constexpr StringView name = e.name().value();
            if (value == [:e.value():]) {
                result = Toml(String(name));
            }
        }
        return result;
    }

    template <typename E>
        requires IsEnumValue<E>
    [[nodiscard]]
    [[=Throws<TomlTypeException>]]
    E enum_from_toml(const Toml& value) {
        const Optional<String> name = value.as<String>();
        if (!name.has_value()) {
            throw TomlTypeException("expected a string for an enum value");
        }
        template for (constexpr Enumerator e: Ops::static_array(Enum<E>().enumerators())) {
            constexpr StringView enumerator_name = e.name().value();
            if (*name == enumerator_name) {
                return [:e.value():];
            }
        }
        throw TomlTypeException(Ops::fmt("unknown enumerator '{}'", *name));
    }

    template <bool Forced, typename T>
    [[nodiscard]]
    Toml serialize_value(const T& value);

    template <typename T, bool Forced>
    [[nodiscard]]
    [[=Throws<TomlTypeException>]]
    T deserialize_value(const Toml& toml);

    /**
     * @internal
     * @brief Write T's members into @p table: each non-excluded member under
     * its key (an empty Optional omitted, as TOML cannot store null), a
     * flattened member's own members in its place.
     * @tparam Forced Whether private members are read regardless (AllowPrivate).
     */
    template <bool Forced, typename T>
    void write_fields(const T& value, TomlTable& table) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::TOML, field>()) {
                using Mem = typename [:field.type().value():];
                if constexpr (is_flattened<DataFormat::TOML, field>()) {
                    write_fields<Forced>(value.[:field.value():], table);
                } else if constexpr (SerdeOptional<Mem>) {
                    if (value.[:field.value():].has_value()) {
                        table.set(key_of<DataFormat::TOML, field>(), serialize_value<Forced>(value.[:field.value():]));
                    }
                } else {
                    table.set(key_of<DataFormat::TOML, field>(), serialize_value<Forced>(value.[:field.value():]));
                }
            }
        }
    }

    /**
     * @internal
     * @brief Read T's members out of @p table, the reverse of write_fields:
     * a flattened member is read from the same table as its parent. An
     * absent key leaves the member default-constructed.
     */
    template <bool Forced, typename T>
    [[=Throws<TomlTypeException>]]
    void read_fields(const TomlTable& table, T& result) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::TOML, field>()) {
                if constexpr (is_flattened<DataFormat::TOML, field>()) {
                    read_fields<Forced>(table, result.[:field.value():]);
                } else {
                    using Mem = typename [:field.type().value():];
                    if (const Toml* member = table.find(key_of<DataFormat::TOML, field>())) {
                        result.[:field.value():] = deserialize_value<Mem, Forced>(*member);
                    }
                }
            }
        }
    }

    /**
     * @internal
     * @brief Toml::serialize, with the access mode carried down to every
     * nested value.
     */
    template <bool Forced, typename T>
    Toml serialize_value(const T& value) {
        if constexpr (SameAs<T, Toml>) {
            return value;
        } else if constexpr (SerdeNative<T>) {
            return Toml(value);
        } else if constexpr (SameAs<T, StringView>) {
            return Toml(String(value));
        } else if constexpr (IsEnumValue<T>) {
            return enum_to_toml(value);
        } else if constexpr (SerdeOptional<T>) {
            // TOML has no null: an empty Optional produces no value, and the
            // caller (the struct walk) omits the key entirely.
            if (!value.has_value()) {
                return Toml();
            }
            return serialize_value<Forced>(*value);
        } else if constexpr (SerdeSequence<T>) {
            Toml::Array array;
            array.reserve(value.size());
            for (const auto& element: value) {
                array.push_back(serialize_value<Forced>(element));
            }
            return Toml(Ops::move(array));
        } else {
            static_assert(ReflectableAsClass<T>, "type is not TOML-serializable");
            constexpr StringView key_problem = key_error<DataFormat::TOML, T, Forced>();
            static_assert(key_problem.empty(), key_problem);
            TomlTable table;
            write_fields<Forced>(value, table);
            return Toml(Ops::move(table));
        }
    }

    /**
     * @internal
     * @brief Toml::deserialize, with the access mode carried down to every
     * nested value.
     */
    template <typename T, bool Forced>
    T deserialize_value(const Toml& toml) {
        if constexpr (SameAs<T, Toml>) {
            return toml;
        } else if constexpr (SerdeNative<T> || SameAs<T, StringView>) {
            Optional<T> v = toml.as<T>();
            if (!v.has_value()) {
                throw TomlTypeException(Ops::fmt("TOML value is not the requested scalar (is {})", toml.type_name()));
            }
            return *v;
        } else if constexpr (IsEnumValue<T>) {
            return enum_from_toml<T>(toml);
        } else if constexpr (SerdeOptional<T>) {
            using Inner = OptionalValue<T>;
            return T(deserialize_value<Inner, Forced>(toml));
        } else if constexpr (SerdeSequence<T>) {
            using Element = SerdeSequenceValue<T>;
            if (!toml.is_array()) {
                throw TomlTypeException(Ops::fmt("expected an array (got {})", toml.type_name()));
            }
            T out;
            const Toml::Array& array = toml.as_array();
            out.reserve(array.size());
            for (const Toml& element: array) {
                out.push_back(deserialize_value<Element, Forced>(element));
            }
            return out;
        } else {
            static_assert(ReflectableAsClass<T>, "type is not TOML-deserializable");
            static_assert(
                Class<T>().is_default_constructible(),
                "TOML deserialization requires the target struct to be default-constructible"
            );
            constexpr StringView key_problem = key_error<DataFormat::TOML, T, Forced>();
            static_assert(key_problem.empty(), key_problem);
            if (!toml.is_table()) {
                throw TomlTypeException(Ops::fmt("expected a table (got {})", toml.type_name()));
            }
            // Absent keys keep the default-constructed member value, so a
            // missing key and an Optional field both work.
            T result;
            read_fields<Forced>(toml.as_table(), result);
            return result;
        }
    }

    template <typename T>
    inline Toml Toml::serialize(const T& value) {
        return serialize_value<false>(value);
    }

    template <typename T>
    inline Toml Toml::serialize(const T& value, AllowPrivateTag) {
        return serialize_value<true>(value);
    }

    template <typename T>
    [[=Throws<TomlTypeException>]]
    inline T Toml::deserialize() const {
        return deserialize_value<T, false>(*this);
    }

    template <typename T>
    [[=Throws<TomlTypeException>]]
    inline T Toml::deserialize(AllowPrivateTag) const {
        return deserialize_value<T, true>(*this);
    }
}
#endif // __cpp_lib_reflection
