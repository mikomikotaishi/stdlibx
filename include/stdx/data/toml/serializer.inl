#pragma once

/**
 * @namespace stdx::data::toml
 * @brief TOML parsing, construction, and serialization (TOML v1.0.0).
 */
namespace stdx::data::toml {
    // Forward declarations: the inline-value writers are mutually recursive
    // (an array holds values; a value may be an inline table; a table holds
    // values), so each needs to see the others before it is defined.
    inline void write_inline(const Toml& value, String& out);
    inline void write_inline_table(const TomlTable& table, String& out);
    inline void write_inline_value_or_table(const Toml& value, String& out);

    /**
     * @internal
     * @brief Whether @p key may be written bare, or must be quoted.
     * @param key The key to test.
     * @return true if @p key is a non-empty run of A-Z a-z 0-9 '_' '-'.
     *
     * TOML bare keys are limited to that set; anything else (spaces, dots,
     * unicode, the empty string) has to travel as a quoted key.
     */
    [[nodiscard]]
    inline bool is_bare_key(StringView key) noexcept {
        if (key.empty()) {
            return false;
        }
        for (const char c: key) {
            const bool ok = (c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9')
                || c == '_'
                || c == '-';
            if (!ok) {
                return false;
            }
        }
        return true;
    }

    /**
     * @internal
     * @brief Appends a basic-string literal for @p s, with the required escapes.
     * @param s The raw string.
     * @param out The buffer to append to.
     */
    inline void write_basic_string(StringView s, String& out) {
        out += '"';
        for (const char c: s) {
            switch (c) {
                case '"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\b': out += "\\b"; break;
                case '\f': out += "\\f"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
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
     * @brief Appends @p key as a bare key where possible, else a quoted key.
     */
    inline void write_key(StringView key, String& out) {
        if (is_bare_key(key)) {
            out += key;
        } else {
            write_basic_string(key, out);
        }
    }

    /**
     * @internal
     * @brief Formats a number into @p out, keeping a float distinguishable from an integer.
     */
    inline void write_number(const Toml& value, String& out) {
        if (value.is_integer()) {
            out += Ops::fmt("{}", *value.as<i64>());
            return;
        }
        const f64 number = *value.as<f64>();
        if (Math::is_nan(number)) {
            out += "nan";
            return;
        }
        if (Math::is_infinite(number)) {
            out += number < 0 ? "-inf" : "inf";
            return;
        }
        char buffer[32];
        const auto result = std::to_chars(buffer, buffer + sizeof(buffer), number);
        StringView text(buffer, static_cast<usize>(result.ptr - buffer));
        out += text;
        if (text.find('.') == StringView::npos
            && text.find('e') == StringView::npos
            && text.find('E') == StringView::npos) {
            out += ".0";
        }
    }

    /**
     * @internal
     * @brief Appends a two-digit, zero-padded integer (for date-time fields).
     */
    inline void write_pad2(i64 n, String& out) {
        out += Ops::fmt("{:02}", n);
    }

    inline void write_local_date(const TomlLocalDate& d, String& out) {
        out += Ops::fmt("{:04}-", static_cast<int>(d.year()));
        write_pad2(static_cast<i64>(static_cast<unsigned>(d.month())), out);
        out += '-';
        write_pad2(static_cast<i64>(static_cast<unsigned>(d.day())), out);
    }

    inline void write_local_time(const TomlLocalTime& t, String& out) {
        write_pad2(t.hours().count(), out);
        out += ':';
        write_pad2(t.minutes().count(), out);
        out += ':';
        write_pad2(t.seconds().count(), out);
        const i64 sub = t.subseconds().count();
        if (sub != 0) {
            // Nanosecond fraction, trimmed of trailing zeros.
            String frac = Ops::fmt("{:09}", sub);
            while (!frac.empty() && frac.back() == '0') {
                frac.pop_back();
            }
            out += '.';
            out += frac;
        }
    }

    inline void write_offset(const Minutes& offset, String& out) {
        if (offset.count() == 0) {
            out += 'Z';
            return;
        }
        const i64 total = offset.count();
        out += total < 0 ? '-' : '+';
        const i64 magnitude = total < 0 ? -total : total;
        write_pad2(magnitude / 60, out);
        out += ':';
        write_pad2(magnitude % 60, out);
    }

    /**
     * @internal
     * @brief Formats a non-table, non-sub-table value inline (the RHS of `key = ...`).
     * @param value The value (must not be a table).
     * @param out The buffer to append to.
     *
     * Handles scalars, the date-time kinds, and arrays (including arrays of
     * inline tables). A table reached here is written as an inline table, which
     * the top-level emitter avoids for anything it can lift into a [section].
     */
    inline void write_inline(const Toml& value, String& out) {
        switch (value.type()) {
            case Toml::Type::BOOLEAN:
                out += (*value.as<bool>() ? "true" : "false");
                break;
            case Toml::Type::INTEGER:
            case Toml::Type::FLOAT:
                write_number(value, out);
                break;
            case Toml::Type::STRING:
                write_basic_string(*value.as<StringView>(), out);
                break;
            case Toml::Type::LOCAL_DATE:
                write_local_date(*value.as<TomlLocalDate>(), out);
                break;
            case Toml::Type::LOCAL_TIME:
                write_local_time(*value.as<TomlLocalTime>(), out);
                break;
            case Toml::Type::LOCAL_DATE_TIME: {
                const TomlLocalDateTime dt = *value.as<TomlLocalDateTime>();
                write_local_date(dt.date, out);
                out += 'T';
                write_local_time(dt.time, out);
                break;
            }
            case Toml::Type::OFFSET_DATE_TIME: {
                const TomlOffsetDateTime dt = *value.as<TomlOffsetDateTime>();
                write_local_date(dt.datetime.date, out);
                out += 'T';
                write_local_time(dt.datetime.time, out);
                write_offset(dt.offset, out);
                break;
            }
            case Toml::Type::ARRAY: {
                const Toml::Array& arr = value.as_array();
                out += '[';
                for (usize i = 0; i < arr.size(); ++i) {
                    if (i != 0) {
                        out += ", ";
                    }
                    write_inline_value_or_table(arr[i], out);
                }
                out += ']';
                break;
            }
            case Toml::Type::TABLE:
                write_inline_table(value.as_table(), out);
                break;
        }
    }

    /**
     * @internal
     * @brief Writes a table in inline `{ k = v, ... }` form.
     */
    inline void write_inline_table(const TomlTable& table, String& out) {
        out += '{';
        const Vector<String>& keys = table.keys();
        const Vector<Toml>& values = table.values();
        for (usize i = 0; i < keys.size(); ++i) {
            if (i != 0) {
                out += ", ";
            } else {
                out += ' ';
            }
            write_key(keys[i], out);
            out += " = ";
            write_inline_value_or_table(values[i], out);
        }
        out += keys.empty() ? "}" : " }";
    }

    /**
     * @internal
     * @brief Dispatches an array element: inline table for a table, else a plain value.
     */
    inline void write_inline_value_or_table(const Toml& value, String& out) {
        write_inline(value, out);
    }

    /**
     * @internal
     * @brief Emits a table as `key = value` lines plus nested `[header]` sections.
     * @param table The table to emit.
     * @param path The dotted header path of this table ("" for the root).
     * @param out The buffer to append to.
     *
     * Non-table members are written as key/value lines first; then each
     * sub-table is emitted as its own `[path.key]` section (recursively), and an
     * array whose every element is a table becomes a run of `[[path.key]]`
     * blocks. This canonical shape round-trips and keeps nesting readable rather
     * than collapsing everything into inline tables.
     */
    inline void write_section(const TomlTable& table, const String& path, String& out) {
        const Vector<String>& keys = table.keys();
        const Vector<Toml>& values = table.values();

        auto is_array_of_tables = [](const Toml& v) -> bool {
            if (!v.is_array()) {
                return false;
            }
            const Toml::Array& arr = v.as_array();
            if (arr.empty()) {
                return false;
            }
            for (const Toml& e: arr) {
                if (!e.is_table()) {
                    return false;
                }
            }
            return true;
        };

        // Pass 1: scalar / non-section members as key = value lines.
        for (usize i = 0; i < keys.size(); ++i) {
            const Toml& v = values[i];
            if (v.is_table() || is_array_of_tables(v)) {
                continue;
            }
            write_key(keys[i], out);
            out += " = ";
            write_inline(v, out);
            out += '\n';
        }

        // Pass 2: sub-tables and arrays-of-tables as their own sections.
        for (usize i = 0; i < keys.size(); ++i) {
            const Toml& v = values[i];
            String child_path = path.empty() ? ""s : path;
            if (!path.empty()) {
                child_path += '.';
            }
            // Build the child's dotted path with the key properly keyed.
            String key_text;
            write_key(keys[i], key_text);
            child_path += key_text;

            if (v.is_table()) {
                out += Ops::fmt("\n[{}]\n", child_path);
                write_section(v.as_table(), child_path, out);
            } else if (is_array_of_tables(v)) {
                for (const Toml& element: v.as_array()) {
                    out += Ops::fmt("\n[[{}]]\n", child_path);
                    write_section(element.as_table(), child_path, out);
                }
            }
        }
    }

    inline String Toml::to_string() const {
        String out;
        if (is_table()) {
            write_section(as_table(), ""s, out);
        } else {
            // A non-table top-level value has no key; emit it bare. (Parsing
            // always yields a table, so this only arises for hand-built values.)
            write_inline(*this, out);
            out += '\n';
        }
        return out;
    }
}
