#pragma once

/**
 * @namespace stdx::data::toml
 * @brief TOML parsing, construction, and serialization (TOML v1.0.0).
 */
namespace stdx::data::toml {
    /**
     * @class TomlParser
     * @internal
     * @brief A recursive-descent parser for one TOML v1.0.0 document.
     *
     * Parses the document into a root @ref Toml table. The grammar is
     * substantially larger than JSON's: bare, quoted and dotted keys; `[table]`
     * and `[[array-of-tables]]` headers; inline tables; arrays; four string
     * flavors (basic, multiline basic, literal, multiline literal); integers in
     * decimal, hex, octal and binary with `_` separators; floats with `inf` and
     * `nan`; and the four date-time literal forms. Beyond lexing, TOML has real
     * semantic rules - a table may not be defined twice, a value may not be
     * overwritten by a table, a key may not be redefined - which this enforces
     * as it goes. One instance parses one document.
     */
    class TomlParser {
    private:
        StringView _text; ///< The source text.
        usize _pos = 0; ///< The current byte offset.
        usize _line = 1; ///< The 1-based line at @ref _pos.
        usize _column = 1; ///< The 1-based column at @ref _pos.

        [[nodiscard]]
        bool at_end() const noexcept {
            return _pos >= _text.size();
        }

        [[nodiscard]]
        char peek(usize ahead = 0) const noexcept {
            return _pos + ahead < _text.size() ? _text[_pos + ahead] : '\0';
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
        THROWS(TomlParseException)
        void fail(const String& reason) const {
            throw TomlParseException(reason, _line, _column);
        }

        [[nodiscard]]
        static bool is_bare_char(char c) noexcept {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9') || c == '_' || c == '-';
        }

        [[nodiscard]]
        static bool is_digit(char c) noexcept {
            return c >= '0' && c <= '9';
        }

        [[nodiscard]]
        static Optional<u8> hex_value(char c) noexcept {
            if (c >= '0' && c <= '9') return static_cast<u8>(c - '0');
            if (c >= 'a' && c <= 'f') return static_cast<u8>(c - 'a' + 10);
            if (c >= 'A' && c <= 'F') return static_cast<u8>(c - 'A' + 10);
            return nullopt;
        }

        /**
         * @brief Skips spaces and tabs (not newlines).
         */
        void skip_inline_whitespace() noexcept {
            while (!at_end() && (peek() == ' ' || peek() == '\t')) {
                advance();
            }
        }

        /**
         * @brief Skips a comment (from '#' to end of line), if one is present.
         */
        void skip_comment() noexcept {
            if (!at_end() && peek() == '#') {
                while (!at_end() && peek() != '\n') {
                    advance();
                }
            }
        }

        /**
         * @brief Consumes a single newline (LF or CRLF), returning whether it did.
         */
        bool consume_newline() noexcept {
            if (peek() == '\r' && peek(1) == '\n') {
                advance();
                advance();
                return true;
            }
            if (peek() == '\n') {
                advance();
                return true;
            }
            return false;
        }

        /**
         * @brief Skips whitespace, comments, and blank lines between statements.
         */
        void skip_to_next_statement() noexcept {
            while (!at_end()) {
                skip_inline_whitespace();
                skip_comment();
                if (!consume_newline()) {
                    break;
                }
            }
        }

        // --- Strings ---

        /**
         * @brief Appends a Unicode code point to @p out as UTF-8.
         */
        static void encode_utf8(u32 code, String& out) {
            if (code <= 0x7F) {
                out += static_cast<char>(code);
            } else if (code <= 0x7FF) {
                out += static_cast<char>(0xC0 | (code >> 6));
                out += static_cast<char>(0x80 | (code & 0x3F));
            } else if (code <= 0xFFFF) {
                out += static_cast<char>(0xE0 | (code >> 12));
                out += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                out += static_cast<char>(0x80 | (code & 0x3F));
            } else {
                out += static_cast<char>(0xF0 | (code >> 18));
                out += static_cast<char>(0x80 | ((code >> 12) & 0x3F));
                out += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                out += static_cast<char>(0x80 | (code & 0x3F));
            }
        }

        /**
         * @brief Reads @p count hex digits as a code point and appends it as UTF-8.
         */
        THROWS(TomlParseException)
        void read_unicode_escape(usize count, String& out) {
            u32 code = 0;
            for (usize i = 0; i < count; ++i) {
                const Optional<u8> digit = at_end() ? nullopt : hex_value(peek());
                if (!digit.has_value()) {
                    fail("invalid \\u/\\U escape");
                }
                code = (code << 4) | *digit;
                advance();
            }
            encode_utf8(code, out);
        }

        /**
         * @brief Decodes one backslash escape in a basic string into @p out.
         */
        THROWS(TomlParseException)
        void parse_escape(String& out, bool multiline) {
            advance(); // the backslash
            if (at_end()) {
                fail("unterminated escape");
            }
            const char e = peek();
            switch (e) {
                case '"':
                    advance();
                    out += '"';
                    break;
                case '\\':
                    advance();
                    out += '\\';
                    break;
                case 'b':
                    advance();
                    out += '\b';
                    break;
                case 'f':
                    advance();
                    out += '\f';
                    break;
                case 'n':
                    advance();
                    out += '\n';
                    break;
                case 'r':
                    advance();
                    out += '\r';
                    break;
                case 't':
                    advance();
                    out += '\t';
                    break;
                case 'u':
                    advance();
                    read_unicode_escape(4, out);
                    break;
                case 'U':
                    advance();
                    read_unicode_escape(8, out);
                    break;
                default:
                    if (multiline && (e == '\n' || e == '\r' || e == ' ' || e == '\t')) {
                        // Line-ending backslash: trim the newline and all the
                        // whitespace that follows it.
                        skip_inline_whitespace();
                        if (!consume_newline()) {
                            fail("invalid escape (a backslash may trim only a trailing newline)");
                        }
                        while (!at_end() && (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r')) {
                            advance();
                        }
                    } else {
                        fail("invalid escape sequence");
                    }
            }
        }

        /**
         * @brief Parses a basic string (the opening quote is at the cursor).
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        String parse_basic_string() {
            advance(); // opening quote
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated basic string");
                }
                if (peek() == '"') {
                    advance();
                    return out;
                }
                if (peek() == '\\') {
                    parse_escape(out, false);
                    continue;
                }
                if (peek() == '\n') {
                    fail("newline in basic string");
                }
                out += advance();
            }
        }

        /**
         * @brief Parses a multiline basic string (`"""` is at the cursor).
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        String parse_multiline_basic_string() {
            advance(); advance(); advance(); // """
            // A newline immediately after the opening delimiter is trimmed.
            consume_newline();
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated multiline basic string");
                }
                if (peek() == '"' && peek(1) == '"' && peek(2) == '"') {
                    advance(); advance(); advance();
                    return out;
                }
                if (peek() == '\\') {
                    parse_escape(out, true);
                    continue;
                }
                out += advance();
            }
        }

        /**
         * @brief Parses a literal string (`'` is at the cursor; no escapes).
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        String parse_literal_string() {
            advance(); // opening quote
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated literal string");
                }
                if (peek() == '\'') {
                    advance();
                    return out;
                }
                if (peek() == '\n') {
                    fail("newline in literal string");
                }
                out += advance();
            }
        }

        /**
         * @brief Parses a multiline literal string (`'''` is at the cursor).
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        String parse_multiline_literal_string() {
            advance();
            advance();
            advance(); // '''
            consume_newline();
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated multiline literal string");
                }
                if (peek() == '\'' && peek(1) == '\'' && peek(2) == '\'') {
                    advance(); advance(); advance();
                    return out;
                }
                out += advance();
            }
        }

        // --- Keys ---

        /**
         * @brief Parses one key segment (bare, basic-quoted, or literal-quoted).
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        String parse_key_segment() {
            if (peek() == '"') {
                return parse_basic_string();
            }
            if (peek() == '\'') {
                return parse_literal_string();
            }
            String out;
            while (!at_end() && is_bare_char(peek())) {
                out += advance();
            }
            if (out.empty()) {
                fail("expected a key");
            }
            return out;
        }

        /**
         * @brief Parses a dotted key path (`a.b.c`) into its segments.
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        Vector<String> parse_key_path() {
            Vector<String> path;
            while (true) {
                skip_inline_whitespace();
                path.push_back(parse_key_segment());
                skip_inline_whitespace();
                if (peek() == '.') {
                    advance();
                    continue;
                }
                break;
            }
            return path;
        }

        // --- Values ---

        /**
         * @brief Parses the value on the right of a `key = value` or array element.
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_value() {
            if (at_end()) {
                fail("expected a value");
            }
            const char c = peek();
            if (c == '"') {
                return Toml((peek(1) == '"' && peek(2) == '"')
                    ? parse_multiline_basic_string()
                    : parse_basic_string());
            }
            if (c == '\'') {
                return Toml((peek(1) == '\'' && peek(2) == '\'')
                    ? parse_multiline_literal_string()
                    : parse_literal_string());
            }
            if (c == '[') {
                return parse_array();
            }
            if (c == '{') {
                return parse_inline_table();
            }
            if (c == 't' || c == 'f') {
                return parse_boolean();
            }
            // A bare word that is not true/false: inf/nan handled in the number path.
            return parse_number_or_datetime();
        }

        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_boolean() {
            if (peek() == 't') {
                expect_word("true");
                return Toml(true);
            }
            expect_word("false");
            return Toml(false);
        }

        THROWS(TomlParseException)
        void expect_word(StringView word) {
            for (const char expected: word) {
                if (at_end() || peek() != expected) {
                    fail(Ops::fmt("expected '{}'", word));
                }
                advance();
            }
        }

        /**
         * @brief Reads the run of value characters (up to a delimiter/newline/comment).
         *
         * Date-times and numbers share a lexical prefix, so the whole token is
         * lifted first and then classified.
         */
        [[nodiscard]]
        StringView read_value_token() noexcept {
            const usize start = _pos;
            while (!at_end()) {
                const char c = peek();
                if (
                    c == ',' || c == ']' || c == '}' || c == '\n' ||
                    c == '\r' ||c == '#' || c == ' ' || c == '\t'
                ) {
                    break;
                }
                advance();
            }
            return _text.substr(start, _pos - start);
        }

        /**
         * @brief Classifies a bare value token as a date-time, integer, or float.
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_number_or_datetime() {
            const StringView token = read_value_token();
            if (token.empty()) {
                fail("expected a value");
            }

            // Date-time: a token with a '-' at index 4 (date) or a ':' (time).
            if (looks_like_datetime(token)) {
                return parse_datetime(token);
            }
            if (token == "inf" || token == "+inf") {
                return Toml(Double::POSITIVE_INFINITY);
            }
            if (token == "-inf") {
                return Toml(Double::NEGATIVE_INFINITY);
            }
            if (token == "nan" || token == "+nan" || token == "-nan") {
                return Toml(Double::NaN);
            }
            return parse_number(token);
        }

        [[nodiscard]]
        static bool looks_like_datetime(StringView token) noexcept {
            // YYYY-MM-DD... has '-' at position 4; HH:MM... has ':' at position 2.
            if (token.size() >= 5 && token[4] == '-' && is_digit(token[0])) {
                return true;
            }
            if (token.size() >= 3 && token[2] == ':' && is_digit(token[0]) && is_digit(token[1])) {
                return true;
            }
            return false;
        }

        // --- Numbers ---

        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_number(StringView token) {
            // Strip underscores (only legal between digits; we are lenient and
            // just remove them, relying on from_chars to reject malformed runs).
            String digits;
            digits.reserve(token.size());
            for (const char c: token) {
                if (c != '_') {
                    digits += c;
                }
            }
            StringView s(digits);

            // Radix prefixes: 0x / 0o / 0b (integers only).
            if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'o' || s[1] == 'b')) {
                const int base = s[1] == 'x' ? 16 : (s[1] == 'o' ? 8 : 2);
                const StringView body = s.substr(2);
                i64 value = 0;
                const auto result = std::from_chars(body.data(), body.data() + body.size(), value, base);
                if (result.ec != std::errc() || result.ptr != body.data() + body.size()) {
                    fail(Ops::fmt("invalid based integer '{}'", token));
                }
                return Toml(value);
            }

            const bool is_float = s.find('.') != StringView::npos
                || s.find('e') != StringView::npos
                || s.find('E') != StringView::npos;

            if (!is_float) {
                i64 value = 0;
                const auto result = std::from_chars(s.data(), s.data() + s.size(), value);
                if (result.ec == std::errc() && result.ptr == s.data() + s.size()) {
                    return Toml(value);
                }
                // Fall through to float on overflow.
            }

            f64 number = 0;
            const auto result = std::from_chars(s.data(), s.data() + s.size(), number);
            if (result.ec != std::errc() || result.ptr != s.data() + s.size()) {
                fail(Ops::fmt("invalid number '{}'", token));
            }
            return Toml(number);
        }

        // --- Date-times ---

        THROWS(TomlParseException)
        [[nodiscard]]
        i64 read_int_field(StringView s, usize& i, usize count) {
            i64 value = 0;
            for (usize k = 0; k < count; ++k) {
                if (i >= s.size() || !is_digit(s[i])) {
                    fail("malformed date-time");
                }
                value = value * 10 + (s[i] - '0');
                ++i;
            }
            return value;
        }

        THROWS(TomlParseException)
        [[nodiscard]]
        TomlLocalTime parse_time_fields(StringView s, usize& i) {
            const i64 hour = read_int_field(s, i, 2);
            if (i >= s.size() || s[i] != ':') {
                fail("malformed time");
            }
            ++i;
            const i64 minute = read_int_field(s, i, 2);
            if (i >= s.size() || s[i] != ':') {
                fail("malformed time");
            }
            ++i;
            const i64 second = read_int_field(s, i, 2);
            i64 nanos = 0;
            if (i < s.size() && s[i] == '.') {
                ++i;
                i64 scale = 100000000; // 1e8, for the first fractional digit
                while (i < s.size() && is_digit(s[i])) {
                    if (scale >= 1) {
                        nanos += (s[i] - '0') * scale;
                        scale /= 10;
                    }
                    ++i;
                }
            }
            using stdx::time::Hours;
            using stdx::time::Nanoseconds;
            const Nanoseconds total = Hours(hour) + Minutes(minute)
                + stdx::time::Seconds(second) + Nanoseconds(nanos);
            return TomlLocalTime(total);
        }

        THROWS(TomlParseException)
        [[nodiscard]]
        TomlLocalDate parse_date_fields(StringView s, usize& i) {
            const i64 year = read_int_field(s, i, 4);
            if (i >= s.size() || s[i] != '-') {
                fail("malformed date");
            }
            ++i;
            const i64 month = read_int_field(s, i, 2);
            if (i >= s.size() || s[i] != '-') {
                fail("malformed date");
            }
            ++i;
            const i64 day = read_int_field(s, i, 2);
            using stdx::time::Day;
            using stdx::time::Month;
            using stdx::time::Year;
            return TomlLocalDate{
                Year(static_cast<int>(year)) / Month(static_cast<unsigned>(month)) / Day(static_cast<unsigned>(day))
            };
        }

        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_datetime(StringView token) {
            usize i = 0;
            // Time-only: starts HH:MM:SS.
            if (token.size() >= 3 && token[2] == ':') {
                return Toml(parse_time_fields(token, i));
            }
            const TomlLocalDate date = parse_date_fields(token, i);
            if (i >= token.size()) {
                return Toml(date);
            }
            const char sep = token[i];
            if (sep != 'T' && sep != 't' && sep != ' ') {
                fail("expected a date/time separator");
            }
            ++i;
            const TomlLocalTime time = parse_time_fields(token, i);
            if (i >= token.size()) {
                return Toml(TomlLocalDateTime{date, time});
            }
            // Offset: 'Z' or (+|-)HH:MM.
            Minutes offset{0};
            if (token[i] == 'Z' || token[i] == 'z') {
                ++i;
            } else if (token[i] == '+' || token[i] == '-') {
                const bool negative = token[i] == '-';
                ++i;
                const i64 oh = read_int_field(token, i, 2);
                if (i >= token.size() || token[i] != ':') {
                    fail("malformed offset");
                }
                ++i;
                const i64 om = read_int_field(token, i, 2);
                const i64 total = oh * 60 + om;
                offset = Minutes(negative ? -total : total);
            } else {
                fail("malformed date-time offset");
            }
            return Toml(TomlOffsetDateTime{TomlLocalDateTime{date, time}, offset});
        }

        // --- Arrays and inline tables ---

        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_array() {
            advance(); // '['
            Toml::Array elements;
            while (true) {
                skip_array_whitespace();
                if (at_end()) {
                    fail("unterminated array");
                }
                if (peek() == ']') {
                    advance();
                    break;
                }
                elements.push_back(parse_value());
                skip_array_whitespace();
                if (peek() == ',') {
                    advance();
                    continue;
                }
                skip_array_whitespace();
                if (!at_end() && peek() == ']') {
                    advance();
                    break;
                }
                fail("expected ',' or ']' in array");
            }
            return Toml(Ops::move(elements));
        }

        /**
         * @brief Skips whitespace, newlines, and comments permitted inside an array.
         */
        void skip_array_whitespace() noexcept {
            while (!at_end()) {
                const char c = peek();
                if (c == ' ' || c == '\t') {
                    advance();
                } else if (c == '\n' || c == '\r') {
                    consume_newline();
                } else if (c == '#') {
                    skip_comment();
                } else {
                    break;
                }
            }
        }

        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_inline_table() {
            advance(); // '{'
            TomlTable table;
            skip_inline_whitespace();
            if (!at_end() && peek() == '}') {
                advance();
                return Toml(Ops::move(table));
            }
            while (true) {
                skip_inline_whitespace();
                const Vector<String> path = parse_key_path();
                skip_inline_whitespace();
                if (at_end() || peek() != '=') {
                    fail("expected '=' in inline table");
                }
                advance();
                skip_inline_whitespace();
                Toml value = parse_value();
                insert_dotted(table, path, Ops::move(value));
                skip_inline_whitespace();
                if (peek() == ',') {
                    advance();
                    continue;
                }
                if (peek() == '}') {
                    advance();
                    break;
                }
                fail("expected ',' or '}' in inline table");
            }
            return Toml(Ops::move(table));
        }

        // --- Dotted-key insertion and table navigation ---

        /**
         * @brief Inserts @p value into @p table at the dotted @p path, creating
         * intermediate tables, and rejecting a redefinition or a value/table clash.
         */
        THROWS(TomlParseException)
        void insert_dotted(TomlTable& table, const Vector<String>& path, Toml value) {
            TomlTable* current = &table;
            for (usize k = 0; k + 1 < path.size(); ++k) {
                Toml* next = current->find(path[k]);
                if (next == nullptr) {
                    current->set(path[k], Toml(TomlTable{}));
                    next = current->find(path[k]);
                } else if (!next->is_table()) {
                    fail(Ops::fmt("key '{}' is not a table", path[k]));
                }
                current = &next->as_table();
            }
            const String& leaf = path.back();
            if (current->contains(leaf)) {
                fail(Ops::fmt("duplicate key '{}'", leaf));
            }
            current->set(leaf, Ops::move(value));
        }

        /**
         * @brief Resolves a `[header]` path to the table it names, creating it.
         * @param root The document root.
         * @param path The header's dotted segments.
         * @param array_of_tables Whether this is a `[[...]]` header.
         * @return The table to populate with the section's key/value pairs.
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        TomlTable* resolve_header(TomlTable& root, const Vector<String>& path, bool array_of_tables) {
            TomlTable* current = &root;
            for (usize k = 0; k + 1 < path.size(); ++k) {
                Toml* next = current->find(path[k]);
                if (next == nullptr) {
                    current->set(path[k], Toml(TomlTable{}));
                    current = &current->find(path[k])->as_table();
                } else if (next->is_table()) {
                    current = &next->as_table();
                } else if (next->is_array() && !next->as_array().empty()
                    && next->as_array().back().is_table()) {
                    // Descend into the most recent element of an array-of-tables.
                    current = &next->as_array().back().as_table();
                } else {
                    fail(Ops::fmt("key '{}' is not a table", path[k]));
                }
            }

            const String& leaf = path.back();
            Toml* existing = current->find(leaf);
            if (array_of_tables) {
                if (existing == nullptr) {
                    current->set(leaf, Toml::array());
                    existing = current->find(leaf);
                } else if (!existing->is_array()) {
                    fail(Ops::fmt("key '{}' is not an array of tables", leaf));
                }
                existing->push_back(Toml(TomlTable{}));
                return &existing->as_array().back().as_table();
            }

            if (existing == nullptr) {
                current->set(leaf, Toml(TomlTable{}));
                return &current->find(leaf)->as_table();
            }
            if (existing->is_table()) {
                // Allowed only if it was created implicitly (as a parent); TOML
                // forbids defining the same table header twice, but detecting
                // implicit-vs-explicit precisely needs bookkeeping beyond this
                // pass, so a non-empty redefinition is the signal we reject.
                fail(Ops::fmt("table '{}' is defined more than once", leaf));
            }
            fail(Ops::fmt("key '{}' is not a table", leaf));
        }

        // --- Statements ---

        /**
         * @brief Parses one `key = value` statement into the current table.
         */
        THROWS(TomlParseException)
        void parse_keyval(TomlTable& current) {
            const Vector<String> path = parse_key_path();
            skip_inline_whitespace();
            if (at_end() || peek() != '=') {
                fail("expected '=' after key");
            }
            advance();
            skip_inline_whitespace();
            Toml value = parse_value();
            insert_dotted(current, path, Ops::move(value));
        }
    public:
        explicit TomlParser(StringView text) noexcept:
            _text{text} {}

        /**
         * @brief Parses the whole document into a root table.
         * @return The root @ref Toml table.
         * @throws TomlParseException on any lexical or semantic error.
         */
        THROWS(TomlParseException)
        [[nodiscard]]
        Toml parse_document() {
            Toml root(TomlTable{});
            TomlTable* current = &root.as_table();

            while (true) {
                skip_to_next_statement();
                if (at_end()) {
                    break;
                }
                if (peek() == '[') {
                    // A table header or an array-of-tables header.
                    const bool array_of_tables = peek(1) == '[';
                    advance();
                    if (array_of_tables) {
                        advance();
                    }
                    skip_inline_whitespace();
                    const Vector<String> path = parse_key_path();
                    skip_inline_whitespace();
                    if (at_end() || peek() != ']') {
                        fail("expected ']' to close a table header");
                    }
                    advance();
                    if (array_of_tables) {
                        if (at_end() || peek() != ']') {
                            fail("expected ']]' to close an array-of-tables header");
                        }
                        advance();
                    }
                    current = resolve_header(root.as_table(), path, array_of_tables);
                } else {
                    parse_keyval(*current);
                }
                // After a statement, only whitespace/comment then a newline or EOF.
                skip_inline_whitespace();
                skip_comment();
                if (!at_end() && !consume_newline()) {
                    fail("expected a newline after a statement");
                }
            }
            return root;
        }
    };

    inline Toml Toml::parse(StringView text) {
        return TomlParser(text).parse_document();
    }

    inline Optional<Toml> Toml::try_parse(StringView text) noexcept {
        try {
            return TomlParser(text).parse_document();
        } catch (const TomlParseException&) {
            return nullopt;
        }
    }
}
