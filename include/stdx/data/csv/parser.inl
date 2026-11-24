#pragma once

/**
 * @namespace stdx::data::csv
 * @brief CSV parsing, construction, and serialization (RFC 4180).
 */
namespace stdx::data::csv {
    /**
     * @class CsvParser
     * @internal
     * @brief A single-pass RFC 4180 parser over one CSV document.
     *
     * Splits the text into records and fields under a given @ref CsvDialect.
     * The grammar is small: fields are separated by the delimiter and records by
     * a newline (LF or CRLF); a field may be wrapped in the quote character, in
     * which case the delimiter, newlines, and a doubled quote are all literal
     * data. One instance parses one document. Line/column are tracked so a
     * malformed quote can be reported against the offending character.
     */
    class CsvParser {
    private:
        StringView _text; ///< The source text.
        CsvDialect _dialect; ///< The lexical conventions in force.
        usize _pos = 0; ///< The current byte offset.
        usize _line = 1; ///< The 1-based line at @ref _pos.
        usize _column = 1; ///< The 1-based column at @ref _pos.

        [[nodiscard]]
        bool at_end() const noexcept {
            return _pos >= _text.size();
        }

        [[nodiscard]]
        char peek() const noexcept {
            return _text[_pos];
        }

        char advance() noexcept {
            const char c = _text[_pos++];
            if (c == '\n') {
                ++_line;
                _column = 1;
            } else {
                ++_column;
            }
            return c;
        }

        [[noreturn]]
        THROWS(CsvParseException)
        void fail(const String& reason) const {
            throw CsvParseException(reason, _line, _column);
        }

        /**
         * @brief Consumes a single record terminator (CRLF or LF) if present.
         * @return true if a terminator was consumed, false at end of input.
         */
        bool consume_newline() noexcept {
            if (at_end()) {
                return false;
            }
            if (peek() == '\r') {
                advance();
                if (!at_end() && peek() == '\n') {
                    advance();
                }
                return true;
            }
            if (peek() == '\n') {
                advance();
                return true;
            }
            return false;
        }

        /**
         * @brief Parses one field, quoted or not, up to the next delimiter or newline.
         * @return The field's decoded text.
         * @throws CsvParseException on an unterminated or malformed quoted field.
         */
        THROWS(CsvParseException)
        [[nodiscard]]
        String parse_field() {
            if (!at_end() && peek() == _dialect.quote) {
                return parse_quoted_field();
            }
            return parse_unquoted_field();
        }

        /**
         * @brief Parses an unquoted field, ending at the delimiter, a newline, or EOF.
         * @return The field text.
         * @throws CsvParseException if a bare quote appears inside it.
         */
        THROWS(CsvParseException)
        [[nodiscard]]
        String parse_unquoted_field() {
            String out;
            while (!at_end()) {
                const char c = peek();
                if (c == _dialect.delimiter || c == '\n' || c == '\r') {
                    break;
                }
                if (c == _dialect.quote) {
                    fail("unexpected quote in unquoted field");
                }
                out += advance();
            }
            return out;
        }

        /**
         * @brief Parses a quoted field (the opening quote is at the cursor).
         * @return The field text, with the surrounding quotes removed and any
         * doubled quote collapsed to one.
         * @throws CsvParseException if the field is never closed, or a character
         * other than a delimiter/newline follows the closing quote.
         */
        THROWS(CsvParseException)
        [[nodiscard]]
        String parse_quoted_field() {
            advance(); // opening quote
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated quoted field");
                }
                const char c = advance();
                if (c != _dialect.quote) {
                    out += c;
                    continue;
                }
                // A quote: either a doubled quote (an escaped literal) or the
                // close of the field.
                if (!at_end() && peek() == _dialect.quote) {
                    advance();
                    out += _dialect.quote;
                    continue;
                }
                // Closing quote: only a delimiter, a newline, or EOF may follow.
                if (!at_end() && peek() != _dialect.delimiter && peek() != '\n' && peek() != '\r') {
                    fail("unexpected character after closing quote");
                }
                return out;
            }
        }

        /**
         * @brief Parses one record: fields separated by the delimiter.
         * @return The record's fields.
         * @throws CsvParseException on a malformed field.
         *
         * A record always has at least one field; a blank line yields a single
         * empty field, which the caller skips so trailing newlines do not add a
         * spurious empty row.
         */
        THROWS(CsvParseException)
        [[nodiscard]]
        Vector<String> parse_record() {
            Vector<String> fields;
            while (true) {
                fields.push_back(parse_field());
                if (at_end() || peek() == '\n' || peek() == '\r') {
                    break;
                }
                advance(); // the delimiter
            }
            return fields;
        }
    public:
        CsvParser(StringView text, const CsvDialect& dialect) noexcept:
            _text{text}, _dialect{dialect} {}

        /**
         * @brief Parses the whole document into header (if any) and data rows.
         * @return The parsed document.
         * @throws CsvParseException on malformed input.
         */
        THROWS(CsvParseException)
        [[nodiscard]]
        CsvDocument parse_document() {
            Vector<Vector<String>> records;
            while (!at_end()) {
                Vector<String> record = parse_record();
                const bool consumed = consume_newline();
                // A single empty field that is the entire final record is the
                // artifact of a trailing newline, not a real row; drop it.
                const bool blank = record.size() == 1 && record.front().empty();
                if (!(blank && at_end())) {
                    records.push_back(Ops::move(record));
                }
                if (!consumed && at_end()) {
                    break;
                }
            }

            Vector<String> header;
            bool has_header = false;
            Vector<CsvRow> rows;

            auto first = records.begin();
            if (_dialect.has_header && first != records.end()) {
                header = Ops::move(*first);
                has_header = true;
                ++first;
            }
            for (auto it = first; it != records.end(); ++it) {
                rows.emplace_back(Ops::move(*it));
            }
            return CsvDocument(Ops::move(header), has_header, Ops::move(rows));
        }
    };

    inline CsvDocument CsvDocument::parse(StringView text, const CsvDialect& dialect) {
        return CsvParser(text, dialect).parse_document();
    }

    inline Optional<CsvDocument> CsvDocument::try_parse(StringView text, const CsvDialect& dialect) noexcept {
        try {
            return CsvParser(text, dialect).parse_document();
        } catch (const CsvParseException&) {
            return nullopt;
        }
    }
}
