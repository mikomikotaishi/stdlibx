#pragma once

#ifdef __cpp_lib_reflection
using stdx::collections::Vector;
using stdx::meta::FixedString;
using stdx::meta::IsEnumValue;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Annotation;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Enum;
using stdx::meta::reflect::Enumerator;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::ReflectableAsClass;

/**
 * @namespace stdx::data::csv
 * @brief CSV parsing, construction, and serialization (RFC 4180).
 *
 * This file adds reflection-driven conversion between a sequence of C++ structs
 * and a @ref CsvDocument - one struct per row, one field per column, columns
 * named by the struct's fields. It is compiled only where the toolchain
 * provides C++26 reflection; the document, parser, and serializer never depend
 * on it. CSV has no nested structure, so unlike JSON the serde handles only
 * scalar/enum fields - a container member, or a nested struct that is not
 * flattened, is a compile error. Flattening a nested struct ({@code
 * [[=CsvFlatten]]}, or {@code [[=Flatten]]} for every format) turns its fields
 * into columns of the parent row, which is how a record that nests in JSON is
 * laid out as one CSV row.
 */
export namespace stdx::data::csv {
    /**
     * @brief The {@code [[=CsvPropertyName<...>]]} annotation.
     * @tparam Name The column name to read and write instead of the field's own.
     */
    template <FixedString Name>
    inline constexpr PropertyNameAnnotation<DataFormat::CSV, Name> CsvPropertyName = {};

    /**
     * @brief The {@code [[=CsvIgnore]]} annotation.
     */
    inline constexpr IgnoreAnnotation<DataFormat::CSV> CsvIgnore = {};

    /**
     * @brief The {@code [[=CsvFlatten]]} annotation: a nested struct's members
     * become columns of the parent row, in CSV only - so the same struct can
     * stay nested in JSON or TOML.
     * @see stdx::data::Flatten for every format at once.
     */
    inline constexpr FormatFlattenAnnotation<DataFormat::CSV> CsvFlatten = {};
}

namespace stdx::data::csv {
    /**
     * @internal
     * @brief Whether T is a scalar a CSV field can hold directly.
     */
    template <typename T>
    concept CsvScalar = IsAnyOf<T, bool, String> || Integral<T> || FloatingPoint<T>;

    /**
     * @internal
     * @brief Renders a field value to its CSV text form.
     * @tparam T The field type (scalar or enum).
     * @param value The value to render.
     * @return The text to store in the cell.
     *
     * Booleans travel as "true"/"false"; enums as their enumerator name;
     * numbers and strings as themselves. Quoting is the serializer's concern,
     * applied later when the cell is written.
     */
    template <typename T>
    [[nodiscard]]
    String field_to_string(const T& value) {
        if constexpr (SameAs<T, String>) {
            return value;
        } else if constexpr (SameAs<T, bool>) {
            return value ? "true" : "false";
        } else if constexpr (Integral<T> || FloatingPoint<T>) {
            return Ops::fmt("{}", value);
        } else {
            static_assert(IsEnumValue<T>, "a CSV field must be a scalar or enum");
            String out;
            template for (constexpr Enumerator e: Ops::static_array(Enum<T>().enumerators())) {
                constexpr StringView name = e.name().value();
                if (value == [:e.value():]) {
                    out = String(name);
                }
            }
            return out;
        }
    }

    /**
     * @internal
     * @brief Parses a CSV field's text into a value of type T.
     * @tparam T The field type (scalar or enum).
     * @param text The cell text.
     * @return The parsed value.
     * @throws CsvTypeException if the text does not denote a valid T.
     */
    template <typename T>
    [[nodiscard]]
    [[=Throws<CsvTypeException>]]
    T field_from_string(StringView text) {
        if constexpr (SameAs<T, String>) {
            return String(text);
        } else if constexpr (SameAs<T, bool>) {
            if (text == "true") {
                return true;
            }
            if (text == "false") {
                return false;
            }
            throw CsvTypeException(Ops::fmt("expected a boolean, got '{}'", text));
        } else if constexpr (Integral<T> || FloatingPoint<T>) {
            T value{};
            const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
            if (result.ec != std::errc() || result.ptr != text.data() + text.size()) {
                throw CsvTypeException(Ops::fmt("expected a number, got '{}'", text));
            }
            return value;
        } else {
            static_assert(IsEnumValue<T>, "a CSV field must be a scalar or enum");
            template for (constexpr Enumerator e: Ops::static_array(Enum<T>().enumerators())) {
                constexpr StringView name = e.name().value();
                if (text == name) {
                    return [:e.value():];
                }
            }
            throw CsvTypeException(Ops::fmt("unknown enumerator '{}'", text));
        }
    }

    /**
     * @internal
     * @brief Append T's column names to @p header, a flattened member's
     * columns in its place.
     * @tparam Forced Whether private members are read regardless (AllowPrivate).
     */
    template <bool Forced, typename T>
    void append_columns(Vector<String>& header) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::CSV, field>()) {
                if constexpr (is_flattened<DataFormat::CSV, field>()) {
                    append_columns<Forced, RemoveConstVolatileReferenceType<typename [:field.type().value():]>>(header);
                } else {
                    header.emplace_back(key_of<DataFormat::CSV, field>());
                }
            }
        }
    }

    /**
     * @internal
     * @brief Append one row's cells for @p value, in the same order as
     * append_columns.
     */
    template <bool Forced, typename T>
    void append_cells(const T& value, Vector<String>& cells) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::CSV, field>()) {
                if constexpr (is_flattened<DataFormat::CSV, field>()) {
                    append_cells<Forced>(value.[:field.value():], cells);
                } else {
                    cells.push_back(field_to_string(value.[:field.value():]));
                }
            }
        }
    }

    /**
     * @internal
     * @brief Fill @p value from @p row by column name, the reverse of
     * append_cells. A column the document lacks leaves its field at its
     * default, so a struct may carry fields the file omits.
     */
    template <bool Forced, typename T>
    [[=Throws<CsvTypeException>]]
    void fill_from_row(const CsvRow& row, T& value) {
        constexpr AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr Field field: Ops::static_array(Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<DataFormat::CSV, field>()) {
                if constexpr (is_flattened<DataFormat::CSV, field>()) {
                    fill_from_row<Forced>(row, value.[:field.value():]);
                } else {
                    using Mem = typename [:field.type().value():];
                    if (const String* cell = row.find(key_of<DataFormat::CSV, field>())) {
                        value.[:field.value():] = field_from_string<Mem>(*cell);
                    }
                }
            }
        }
    }

    /**
     * @internal
     * @brief CsvDocument::serialize, under the given access mode.
     */
    template <bool Forced, typename T>
    [[nodiscard]]
    CsvDocument serialize_rows(const Vector<T>& rows) {
        static_assert(ReflectableAsClass<T>, "CSV rows must be a reflectable struct type");
        constexpr StringView key_problem = key_error<DataFormat::CSV, T, Forced>();
        static_assert(key_problem.empty(), key_problem);

        Vector<String> header;
        append_columns<Forced, T>(header);

        Vector<CsvRow> out_rows;
        out_rows.reserve(rows.size());
        for (const T& row: rows) {
            Vector<String> fields;
            fields.reserve(header.size());
            append_cells<Forced>(row, fields);
            out_rows.emplace_back(Ops::move(fields));
        }
        return CsvDocument(Ops::move(header), true, Ops::move(out_rows));
    }

    /**
     * @internal
     * @brief CsvDocument::deserialize, under the given access mode.
     */
    template <typename T, bool Forced>
    [[nodiscard]]
    [[=Throws<CsvTypeException>]]
    Vector<T> deserialize_rows(const CsvDocument& document) {
        static_assert(ReflectableAsClass<T>, "CSV rows must be a reflectable struct type");
        static_assert(
            Class<T>().is_default_constructible(),
            "CSV deserialization requires the row struct to be default-constructible"
        );
        constexpr StringView key_problem = key_error<DataFormat::CSV, T, Forced>();
        static_assert(key_problem.empty(), key_problem);
        if (!document.has_header()) {
            throw CsvTypeException("CSV deserialization into structs requires a header row");
        }

        Vector<T> result;
        result.reserve(document.rows().size());
        for (const CsvRow& row: document.rows()) {
            T value;
            fill_from_row<Forced>(row, value);
            result.push_back(Ops::move(value));
        }
        return result;
    }

    template <typename T>
    inline CsvDocument CsvDocument::serialize(const Vector<T>& rows) {
        return serialize_rows<false>(rows);
    }

    template <typename T>
    inline CsvDocument CsvDocument::serialize(const Vector<T>& rows, AllowPrivateTag) {
        return serialize_rows<true>(rows);
    }

    template <typename T>
    [[=Throws<CsvTypeException>]]
    inline Vector<T> CsvDocument::deserialize() const {
        return deserialize_rows<T, false>(*this);
    }

    template <typename T>
    [[=Throws<CsvTypeException>]]
    inline Vector<T> CsvDocument::deserialize(AllowPrivateTag) const {
        return deserialize_rows<T, true>(*this);
    }
}
#endif // __cpp_lib_reflection
