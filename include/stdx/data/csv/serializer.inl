#pragma once

/**
 * @namespace stdx::data::csv
 * @brief CSV parsing, construction, and serialization (RFC 4180).
 */
namespace stdx::data::csv {
    /**
     * @internal
     * @brief Appends @p field to @p out, quoting it only when RFC 4180 requires.
     * @param field The raw field text.
     * @param out The buffer to append to.
     * @param dialect The lexical conventions in force.
     *
     * A field is wrapped in quotes when it contains the delimiter, a quote, or a
     * newline; inside the wrapping, each quote is doubled. Fields without those
     * characters are written bare, which keeps the common case readable and
     * round-trips through the parser unchanged.
     */
    inline void write_field(StringView field, String& out, const CsvDialect& dialect) {
        const bool needs_quote =
            field.find(dialect.delimiter) != StringView::npos
            || field.find(dialect.quote) != StringView::npos
            || field.find('\n') != StringView::npos
            || field.find('\r') != StringView::npos;

        if (!needs_quote) {
            out += field;
            return;
        }

        out += dialect.quote;
        for (const char c: field) {
            if (c == dialect.quote) {
                out += dialect.quote; // double it
            }
            out += c;
        }
        out += dialect.quote;
    }

    /**
     * @internal
     * @brief Appends one record (its fields joined by the delimiter) plus CRLF.
     * @param fields The record's fields.
     * @param out The buffer to append to.
     * @param dialect The lexical conventions in force.
     */
    inline void write_record(const Vector<String>& fields, String& out, const CsvDialect& dialect) {
        for (usize i = 0; i < fields.size(); ++i) {
            if (i != 0) {
                out += dialect.delimiter;
            }
            write_field(fields[i], out, dialect);
        }
        out += "\r\n"; // RFC 4180 record terminator
    }

    inline String CsvDocument::to_string(const CsvDialect& dialect) const {
        String out;
        if (_has_header) {
            write_record(_header, out, dialect);
        }
        for (const CsvRow& row: _rows) {
            write_record(row.fields(), out, dialect);
        }
        return out;
    }
}
