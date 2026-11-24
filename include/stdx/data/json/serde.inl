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

    /**
     * @brief The {@code [[=JsonFlatten]]} annotation: inline a nested struct's
     * members into the parent object, in JSON only.
     * @see stdx::data::Flatten for every format at once.
     */
    inline constexpr FormatFlattenAnnotation<DataFormat::JSON> JsonFlatten = {};
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

    template <bool Forced, typename T>
    [[nodiscard]]
    Json serialize_value(const T& value);

    template <typename T, bool Forced>
    [[nodiscard]]
    [[=Throws<JsonTypeException>]]
    T deserialize_value(const Json& json);

    /**
     * @internal
     * @brief Write T's members into @p object: each non-excluded member under
     * its key, a flattened member's own members in its place.
     * @tparam Forced Whether private members are read regardless (AllowPrivate).
     */
    template <bool Forced, typename T>
    void write_fields(const T& value, JsonObject& object) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::JSON, field>()) {
                if constexpr (is_flattened<DataFormat::JSON, field>()) {
                    write_fields<Forced>(value.[:field.value():], object);
                } else {
                    object.set(key_of<DataFormat::JSON, field>(), serialize_value<Forced>(value.[:field.value():]));
                }
            }
        }
    }

    /**
     * @internal
     * @brief Read T's members out of @p object, the reverse of write_fields:
     * a flattened member is read from the same object as its parent. An
     * absent key leaves the member default-constructed.
     */
    template <bool Forced, typename T>
    [[=Throws<JsonTypeException>]]
    void read_fields(const JsonObject& object, T& result) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::JSON, field>()) {
                if constexpr (is_flattened<DataFormat::JSON, field>()) {
                    read_fields<Forced>(object, result.[:field.value():]);
                } else {
                    using Mem = typename [:field.type().value():];
                    if (const Json* member = object.find(key_of<DataFormat::JSON, field>())) {
                        result.[:field.value():] = deserialize_value<Mem, Forced>(*member);
                    }
                }
            }
        }
    }

    /**
     * @internal
     * @brief Json::serialize, with the access mode carried down to every
     * nested value.
     */
    template <bool Forced, typename T>
    Json serialize_value(const T& value) {
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
            return serialize_value<Forced>(*value);
        } else if constexpr (SerdeSequence<T>) {
            Json::Array array;
            array.reserve(value.size());
            for (const auto& element: value) {
                array.push_back(serialize_value<Forced>(element));
            }
            return Json(Ops::move(array));
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-serializable");
            constexpr StringView key_problem = key_error<DataFormat::JSON, T, Forced>();
            static_assert(key_problem.empty(), key_problem);
            JsonObject object;
            write_fields<Forced>(value, object);
            return Json(Ops::move(object));
        }
    }

    /**
     * @internal
     * @brief Json::deserialize, with the access mode carried down to every
     * nested value.
     */
    template <typename T, bool Forced>
    T deserialize_value(const Json& json) {
        if constexpr (SameAs<T, Json>) {
            return json;
        } else if constexpr (SerdeScalar<T> || SameAs<T, StringView>) {
            Optional<T> v = json.as<T>();
            if (!v.has_value()) {
                throw JsonTypeException(Ops::fmt("JSON value is not the requested scalar (is {})", json.type_name()));
            }
            return *v;
        } else if constexpr (IsEnumValue<T>) {
            return enum_from_json<T>(json);
        } else if constexpr (SerdeOptional<T>) {
            using Inner = OptionalValue<T>;
            if (json.is_null()) {
                return T{};
            }
            return T(deserialize_value<Inner, Forced>(json));
        } else if constexpr (SerdeSequence<T>) {
            using Element = SerdeSequenceValue<T>;
            if (!json.is_array()) {
                throw JsonTypeException(Ops::fmt("expected an array (got {})", json.type_name()));
            }
            T out;
            const Json::Array& array = json.as_array();
            out.reserve(array.size());
            for (const Json& element: array) {
                out.push_back(deserialize_value<Element, Forced>(element));
            }
            return out;
        } else {
            static_assert(ReflectableAsClass<T>, "type is not JSON-deserializable");
            static_assert(
                Class<T>().is_default_constructible(),
                "JSON deserialization requires the target struct to be default-constructible"
            );
            constexpr StringView key_problem = key_error<DataFormat::JSON, T, Forced>();
            static_assert(key_problem.empty(), key_problem);
            if (!json.is_object()) {
                throw JsonTypeException(Ops::fmt("expected an object (got {})", json.type_name()));
            }
            // Absent keys keep the default-constructed member value, so
            // missing fields and explicit Optional both work.
            T result;
            read_fields<Forced>(json.as_object(), result);
            return result;
        }
    }

    template <typename T>
    inline Json Json::serialize(const T& value) {
        return serialize_value<false>(value);
    }

    template <typename T>
    inline Json Json::serialize(const T& value, AllowPrivateTag) {
        return serialize_value<true>(value);
    }

    template <typename T>
    [[=Throws<JsonTypeException>]]
    inline T Json::deserialize() const {
        return deserialize_value<T, false>(*this);
    }

    template <typename T>
    [[=Throws<JsonTypeException>]]
    inline T Json::deserialize(AllowPrivateTag) const {
        return deserialize_value<T, true>(*this);
    }
}
#endif // __cpp_lib_reflection
