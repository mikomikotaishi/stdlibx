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
 * @namespace stdx::data::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 *
 * This file adds reflection-driven conversion between a @ref Json value and an
 * arbitrary C++ aggregate, in the spirit of C#'s System.Text.Json
 * `Serialize`/`Deserialize`. It is compiled only where the toolchain provides
 * C++26 reflection (`__cpp_lib_reflection`); the DOM, parser, and serializer
 * never depend on it, so the rest of the module is usable without reflection
 * support.
 */
export namespace stdx::data::json {
    /**
     * @brief The {@code [[=JsonPropertyName<...>]]} annotation.
     * @tparam Name The key to read and write instead of the member's own name.
     */
    template <FixedString Name>
    inline constexpr PropertyNameAnnotation<DataFormat::JSON, Name> JsonPropertyName = {};

    /**
     * @brief The {@code [[=JsonIgnore]]} annotation.
     */
    inline constexpr IgnoreAnnotation<DataFormat::JSON> JsonIgnore = {};
}

namespace stdx::data::json {
    /**
     * @internal
     * @concept SerdeOptional
     * @brief Satisfied by Optional specializations; such a field is omitted when
     * empty, and an absent key leaves it empty. Reuses the shared Optional
     * detection from stdx::meta.
     */
    template <typename T>
    concept SerdeOptional = IsOptional<T>;

    /**
     * @internal
     * @concept SerdeSequence
     * @brief Satisfied by Vector specializations, mapped to a JSON array. Uses
     * the shared SpecializationOf from stdx::meta.
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
     * @brief Whether T is a scalar directly representable as a JSON leaf.
     */
    template <typename T>
    concept SerdeScalar = IsAnyOf<T, bool, String>
        || Integral<T>
        || FloatingPoint<T>;

    /**
     * @internal
     * @brief Converts an enum to its enumerator name, or deserializes from it.
     *
     * Enums travel as strings (their enumerator identifiers), matching C#'s
     * `JsonStringEnumConverter`. An unknown name on the way in throws a
     * @ref JsonTypeException.
     */
    template <typename E>
        requires IsEnumValue<E>
    [[nodiscard]]
    Json enum_to_json(E value) {
        Json result;
        template for (constexpr Enumerator e: Ops::static_array(Enum<E>().enumerators())) {
            constexpr StringView name = e.name().value();
            if (value == [:e.value():]) {
                result = Json(String(name));
            }
        }
        return result;
    }

    template <typename E>
        requires IsEnumValue<E>
    [[nodiscard]]
    [[=Throws<JsonTypeException>]]
    E enum_from_json(const Json& value) {
        const Optional<String> name = value.as<String>();
        if (!name.has_value()) {
            throw JsonTypeException("expected a string for an enum value");
        }
        template for (constexpr Enumerator e: Ops::static_array(Enum<E>().enumerators())) {
            constexpr StringView enumerator_name = e.name().value();
            if (*name == enumerator_name) {
                return [:e.value():];
            }
        }
        throw JsonTypeException(Ops::fmt("unknown enumerator '{}'", *name));
    }

    /**
     * @internal
     * @brief The JSON key for a field: its PropertyName override, else its name.
     *
     * The last PropertyName wins; absent one, the member's own identifier is the key.
     */
    template <Field field>
    [[nodiscard]]
    consteval StringView json_key_of() {
        StringView key = field.name().value();
        template for (constexpr Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsPropertyNameAnnotation<Ann, DataFormat::JSON>) {
                constexpr Ann instance = ann.template as<Ann>();
                key = instance.str();
            }
        }
        return key;
    }

    /**
     * @internal
     * @brief Whether a field is excluded from JSON serialization.
     *
     * True if the member carries either the universal @ref stdx::data::Transient
     * annotation (excluded from every format) or JSON's own @ref JsonIgnore
     * (excluded from JSON specifically). Another format's Ignore does not count.
     */
    template <Field field>
    [[nodiscard]]
    consteval bool is_ignored() {
        bool ignored = false;
        template for (constexpr Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsTransientAnnotation<Ann> || IsIgnoreAnnotation<Ann, DataFormat::JSON>) {
                ignored = true;
            }
        }
        return ignored;
    }

    template <typename T>
    inline Json Json::serialize(const T& value) {
        if constexpr (SameAs<T, Json>) {
            return value;
        } else if constexpr (SerdeScalar<T>) {
            return Json(value);
        } else if constexpr (SameAs<T, StringView>) {
            return Json(String(value));
        } else if constexpr (IsEnumValue<T>) {
            return enum_to_json(value);
        } else if constexpr (SerdeOptional<T>) {
            if (!value.has_value()) {
                return Json();
            }
            return Json::serialize(*value);
        } else if constexpr (SerdeSequence<T>) {
            Json::Array array;
            array.reserve(value.size());
            for (const auto& element: value) {
                array.push_back(Json::serialize(element));
            }
            return Json(Ops::move(array));
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-serializable");
            constexpr AccessContext ctx = AccessContext::unchecked();
            JsonObject object;
            template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
                if constexpr (!is_ignored<field>()) {
                    object.set(json_key_of<field>(), Json::serialize(value.[:field.value():]));
                }
            }
            return Json(Ops::move(object));
        }
    }

    template <typename T>
    [[=Throws<JsonTypeException>]]
    inline T Json::deserialize() const {
        if constexpr (SameAs<T, Json>) {
            return *this;
        } else if constexpr (SerdeScalar<T> || SameAs<T, StringView>) {
            Optional<T> v = as<T>();
            if (!v.has_value()) {
                throw JsonTypeException(Ops::fmt("JSON value is not the requested scalar (is {})", type_name()));
            }
            return *v;
        } else if constexpr (IsEnumValue<T>) {
            return enum_from_json<T>(*this);
        } else if constexpr (SerdeOptional<T>) {
            using Inner = OptionalValue<T>;
            if (is_null()) {
                return T{};
            }
            return T(deserialize<Inner>());
        } else if constexpr (SerdeSequence<T>) {
            using Element = SerdeSequenceValue<T>;
            if (!is_array()) {
                throw JsonTypeException(Ops::fmt("expected an array (got {})", type_name()));
            }
            T out;
            const Json::Array& array = as_array();
            out.reserve(array.size());
            for (const Json& element: array) {
                out.push_back(element.deserialize<Element>());
            }
            return out;
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-deserializable");
            static_assert(
                Class<T>().is_default_constructible(),
                "JSON deserialization requires the target struct to be default-constructible"
            );
            if (!is_object()) {
                throw JsonTypeException(Ops::fmt("expected an object (got {})", type_name()));
            }
            constexpr AccessContext ctx = AccessContext::unchecked();
            T result;
            template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
                if constexpr (!is_ignored<field>()) {
                    using Mem = typename [:field.type().value():];
                    constexpr StringView key = json_key_of<field>();
                    if (const Json* member = as_object().find(key)) {
                        result.[:field.value():] = member->deserialize<Mem>();
                    }
                    // Absent key: keep the default-constructed member value,
                    // so missing fields and explicit Optional both work.
                }
            }
            return result;
        }
    }
}
#endif // __cpp_lib_reflection
