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
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 *
 * This file adds reflection-driven conversion between a @ref Json value and an
 * arbitrary C++ aggregate, in the spirit of C#'s System.Text.Json
 * `Serialize`/`Deserialize`. It is compiled only where the toolchain provides
 * C++26 reflection (`__cpp_lib_reflection`); the DOM, parser, and serializer
 * never depend on it, so the rest of the module is usable without reflection
 * support.
 */
export namespace stdx::json {
    /**
     * @struct PropertyNameAnnotation
     * @brief Field annotation overriding the JSON key used for a member.
     * @tparam Name The key to read and write instead of the member's own name.
     *
     * The analogue of C#'s `[JsonPropertyName("...")]`. Applied to a struct
     * member, it makes @ref serialize emit, and @ref deserialize read, @p Name
     * in place of the C++ identifier.
     */
    template <FixedString Name>
    struct PropertyNameAnnotation {
        static constexpr FixedString NAME = Name;

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
     * @brief The {@code [[=PropertyName<...>]]} annotation.
     * @tparam Name The key to read and write instead of the member's own name.
     */
    template <FixedString Name>
    inline constexpr PropertyNameAnnotation<Name> PropertyName = {};

    /**
     * @struct JsonIgnoreAnnotation
     * @brief Field annotation excluding a member from serialization entirely.
     */
    struct JsonIgnoreAnnotation {};

    /**
     * @brief The {@code [[=JsonIgnore]]} annotation.
     */
    inline constexpr JsonIgnoreAnnotation JsonIgnore = {};
}

namespace stdx::json {
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

    // Forward declarations: the container and struct paths recurse through these.

    template <typename T>
    Json to_json(const T& value);

    template <typename T>
    T from_json(const Json& value);

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
     * @concept IsPropertyName
     * @brief Satisfied by the @ref PropertyNameAnnotation annotation type.
     *
     * SFINAE-safe: a bare member access on a type lacking it would be a hard
     * error, so the check is wrapped in a requires-expression.
     */
    template <typename T>
    concept IsPropertyName = requires {
        { RemoveConstVolatileReferenceType<T>::str() } -> SameAs<StringView>;
    };

    /**
     * @internal
     * @concept IsJsonIgnore
     * @brief Satisfied by the @ref JsonIgnoreAnnotation annotation type.
     */
    template <typename T>
    concept IsJsonIgnore = SameAs<RemoveConstVolatileReferenceType<T>, JsonIgnoreAnnotation>;

    /**
     * @internal
     * @brief The JSON key for a field: its PropertyName override, else its name.
     *
     * Walks the field's annotations by type. The last PropertyName wins; absent
     * one, the member's own identifier is the key.
     */
    template <Field field>
    [[nodiscard]]
    consteval StringView json_key_of() {
        StringView key = field.name().value();
        template for (constexpr Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsPropertyName<Ann>) {
                key = Ann::str();
            }
        }
        return key;
    }

    /**
     * @internal
     * @brief Whether a field carries the @ref JsonIgnore annotation.
     */
    template <Field field>
    [[nodiscard]]
    consteval bool is_ignored() {
        bool ignored = false;
        template for (constexpr Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsJsonIgnore<Ann>) {
                ignored = true;
            }
        }
        return ignored;
    }

    // Internal recursion: the public Json::serialize / Json::deserialize members
    // delegate here so the dispatch lives in exactly one place.

    template <typename T>
    Json to_json(const T& value) {
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
            return to_json(*value);
        } else if constexpr (SerdeSequence<T>) {
            Json::Array array;
            array.reserve(value.size());
            for (const auto& element: value) {
                array.push_back(to_json(element));
            }
            return Json(Ops::move(array));
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-serializable");
            constexpr AccessContext ctx = AccessContext::unchecked();
            JsonObject object;
            template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
                if constexpr (!is_ignored<field>()) {
                    object.set(json_key_of<field>(), to_json(value.[:field.value():]));
                }
            }
            return Json(Ops::move(object));
        }
    }

    template <typename T>
    [[=Throws<JsonTypeException>]]
    T from_json(const Json& value) {
        if constexpr (SameAs<T, Json>) {
            return value;
        } else if constexpr (SerdeScalar<T> || SameAs<T, StringView>) {
            Optional<T> v = value.as<T>();
            if (!v.has_value()) {
                throw JsonTypeException(Ops::fmt("JSON value is not the requested scalar (is {})", value.type_name()));
            }
            return *v;
        } else if constexpr (IsEnumValue<T>) {
            return enum_from_json<T>(value);
        } else if constexpr (SerdeOptional<T>) {
            using Inner = OptionalValue<T>;
            if (value.is_null()) {
                return T{};
            }
            return T(from_json<Inner>(value));
        } else if constexpr (SerdeSequence<T>) {
            using Element = SerdeSequenceValue<T>;
            if (!value.is_array()) {
                throw JsonTypeException(Ops::fmt("expected an array (got {})", value.type_name()));
            }
            T out;
            const Json::Array& array = value.as_array();
            out.reserve(array.size());
            for (const Json& element: array) {
                out.push_back(from_json<Element>(element));
            }
            return out;
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-deserializable");
            static_assert(
                Class<T>().is_default_constructible(),
                "JSON deserialization requires the target struct to be default-constructible"
            );
            if (!value.is_object()) {
                throw JsonTypeException(Ops::fmt("expected an object (got {})", value.type_name()));
            }
            constexpr AccessContext ctx = AccessContext::unchecked();
            T result;
            template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
                if constexpr (!is_ignored<field>()) {
                    using Mem = typename [:field.type().value():];
                    constexpr StringView key = json_key_of<field>();
                    if (const Json* member = value.as_object().find(key)) {
                        result.[:field.value():] = from_json<Mem>(*member);
                    }
                    // Absent key: keep the default-constructed member value,
                    // so missing fields and explicit Optional both work.
                }
            }
            return result;
        }
    }

    template <typename T>
    inline Json Json::serialize(const T& value) {
        return to_json(value);
    }

    template <typename T>
    [[=Throws<JsonTypeException>]]
    inline T Json::deserialize() const {
        return from_json<T>(*this);
    }
}
#endif // __cpp_lib_reflection
