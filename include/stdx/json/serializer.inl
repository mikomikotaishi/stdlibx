#pragma once

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
namespace stdx::json {
    /**
     * @internal
     * @brief Appends @p s to @p out as a quoted, escaped JSON string.
     * @param s The raw string contents.
     * @param out The buffer to append to.
     *
     * Escapes the characters JSON requires - the quote, the backslash, and the
     * C0 controls - using the short forms (`\\n`, `\\t`, ...) where they exist
     * and `\\u00XX` for the rest. Bytes at or above 0x20 other than `"` and
     * `\\` are emitted unchanged, so already-valid UTF-8 passes through without
     * being expanded into escapes.
     */
    inline void write_escaped_string(StringView s, String& out) {
        out += '"';
        for (const char c: s) {
            switch (c) {
                case '"':
                    out += "\\\"";
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case '\b':
                    out += "\\b";
                    break;
                case '\f':
                    out += "\\f";
                    break;
                case '\n':
                    out += "\\n";
                    break;
                case '\r':
                    out += "\\r";
                    break;
                case '\t':
                    out += "\\t";
                    break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        out += Ops::fmt("\\u{:04x}", static_cast<unsigned>(static_cast<unsigned char>(c)));
                    } else {
                        out += c;
                    }
            }
        }
        out += '"';
    }

    /**
     * @internal
     * @brief Formats a JSON number into @p out.
     * @param value The Json (must hold a number).
     * @param out The buffer to append to.
     *
     * An integer is written with no decimal point. A float goes through the
     * shortest round-trippable form `std::to_chars` produces, then is nudged to
     * carry a ".0" if it came out looking like an integer, so a float value
     * reparses as a float rather than silently becoming an integer.
     */
    inline void write_number(const Json& value, String& out) {
        if (value.is_integer()) {
            out += Ops::fmt("{}", *value.as<i64>());
            return;
        }
        const f64 number = *value.as<f64>();
        char buffer[32];
        const auto result = std::to_chars(buffer, buffer + sizeof(buffer), number);
        StringView text(buffer, static_cast<usize>(result.ptr - buffer));
        out += text;
        // Keep floats distinguishable from integers on re-parse.
        if (text.find('.') == StringView::npos
            && text.find('e') == StringView::npos
            && text.find('E') == StringView::npos
            && text.find("inf") == StringView::npos
            && text.find("nan") == StringView::npos) {
            out += ".0";
        }
    }

    /**
     * @internal
     * @brief Recursively serializes @p value into @p out.
     * @param value The value to serialize.
     * @param out The buffer to append to.
     * @param indent Spaces per level, or 0 for the compact form.
     * @param depth The current nesting depth, for indentation.
     *
     * With @p indent of 0 this emits the compact form - no spaces, no newlines.
     * Otherwise each array element and object member goes on its own line,
     * indented @p indent spaces per level, with a space after each object key's
     * colon, which is the human-readable form @ref Json::dump produces.
     */
    inline void write_value(const Json& value, String& out, usize indent, usize depth) {
        const bool pretty = indent > 0;

        auto write_newline_indent = [&](usize level) -> void {
            if (pretty) {
                out += '\n';
                out.append(indent * level, ' ');
            }
        };

        switch (value.type()) {
            case JsonType::NULL_VALUE:
                out += "null";
                break;
            case JsonType::BOOLEAN:
                out += (*value.as<bool>() ? "true" : "false");
                break;
            case JsonType::INTEGER:
            case JsonType::FLOAT:
                write_number(value, out);
                break;
            case JsonType::STRING:
                write_escaped_string(*value.as<StringView>(), out);
                break;
            case JsonType::ARRAY: {
                const Json::Array& arr = value.as_array();
                if (arr.empty()) {
                    out += "[]";
                    break;
                }
                out += '[';
                bool first = true;
                for (const Json& element: arr) {
                    if (!first) {
                        out += ',';
                    }
                    first = false;
                    write_newline_indent(depth + 1);
                    write_value(element, out, indent, depth + 1);
                }
                write_newline_indent(depth);
                out += ']';
                break;
            }
            case JsonType::OBJECT: {
                const JsonObject& obj = value.as_object();
                if (obj.empty()) {
                    out += "{}";
                    break;
                }
                out += '{';
                const Vector<String>& keys = obj.keys();
                const Vector<Json>& values = obj.values();
                for (usize i = 0; i < keys.size(); ++i) {
                    if (i != 0) {
                        out += ',';
                    }
                    write_newline_indent(depth + 1);
                    write_escaped_string(keys[i], out);
                    out += (pretty ? ": " : ":");
                    write_value(values[i], out, indent, depth + 1);
                }
                write_newline_indent(depth);
                out += '}';
                break;
            }
        }
    }

    // Definitions of the serialization entry points declared in value.inl.

    inline String Json::to_string() const {
        String out;
        write_value(*this, out, 0, 0);
        return out;
    }

    inline String Json::dump(usize indent) const {
        String out;
        write_value(*this, out, indent, 0);
        return out;
    }
}
