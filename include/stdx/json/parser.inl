#pragma once

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
namespace stdx::json {
    /**
     * @class JsonParser
     * @internal
     * @brief A recursive-descent parser for a single JSON document.
     *
     * Holds a cursor into the source text plus the line/column it maps to, so a
     * failure can be reported against the offending character. One instance
     * parses one document: construct it over the text, call @ref parse_document,
     * and discard it. Not part of the public surface - callers go through
     * @ref Json::parse or @ref Json::try_parse.
     *
     * The grammar is RFC 8259 in full: the four literal forms, numbers with
     * optional sign/fraction/exponent, strings with the standard escapes and
     * `\uXXXX` (including surrogate pairs decoded to UTF-8), arrays, and
     * objects. Nesting is bounded by @ref MAX_DEPTH so a pathologically deep
     * document fails cleanly rather than exhausting the stack.
     */
    class JsonParser {
    public:
        static constexpr usize MAX_DEPTH = 512; ///< Maximum array/object nesting before parsing is abandoned.
    private:
        StringView _text; ///< The source text.
        usize _pos = 0; ///< The current byte offset into @ref _text.
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

        /**
         * @brief Consumes and returns the current character, tracking position.
         * @return The consumed character.
         *
         * Advances line/column so a later @ref fail points at the right spot; a
         * newline bumps the line and resets the column.
         */
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
        THROWS(JsonParseException)
        void fail(const String& reason) const {
            throw JsonParseException(reason, _line, _column);
        }

        void skip_whitespace() noexcept {
            while (!at_end()) {
                const char c = peek();
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                    advance();
                } else {
                    break;
                }
            }
        }

        /**
         * @brief Parses any value at the current position.
         * @param depth The current nesting depth (0 at the top level).
         * @return The parsed value.
         * @throws JsonParseException on malformed input or excessive nesting.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_value(usize depth) {
            if (at_end()) {
                fail("unexpected end of input");
            }
            switch (peek()) {
                case '{':
                    return parse_object(depth);
                case '[':
                    return parse_array(depth);
                case '"':
                    return Json(parse_string());
                case 't':
                case 'f':
                    return parse_bool();
                case 'n':
                    return parse_null();
                default:
                    return parse_number();
            }
            Ops::unreachable();
        }

        /**
         * @brief Consumes a bare keyword (`true`, `false`, or `null`).
         * @param word The exact keyword expected.
         * @throws JsonParseException if the text at the cursor is not @p word.
         */
        THROWS(JsonParseException)
        void expect_literal(StringView word) {
            for (const char expected: word) {
                if (at_end() || peek() != expected) {
                    fail(Ops::fmt("invalid literal, expected '{}'", word));
                }
                advance();
            }
        }

        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_bool() {
            if (peek() == 't') {
                expect_literal("true");
                return Json(true);
            }
            expect_literal("false");
            return Json(false);
        }

        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_null() {
            expect_literal("null");
            return Json();
        }

        /**
         * @brief Parses a number, choosing integer or float representation.
         * @return A Json holding an i64 or an f64.
         * @throws JsonParseException on a malformed number.
         *
         * A number with no fraction and no exponent that fits an i64 is stored
         * as an integer, so it round-trips without gaining a ".0"; everything
         * else, and anything that overflows i64, is stored as a float. The
         * digit grammar is validated here rather than deferred to a conversion
         * routine, so a leading zero or a lone '-' is rejected with a position.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_number() {
            const usize start = _pos;
            bool is_float = false;

            if (!at_end() && peek() == '-') {
                advance();
            }

            if (at_end() || !is_digit(peek())) {
                fail("invalid number");
            }

            // Integer part: a single '0', or a nonzero digit then more digits.
            if (peek() == '0') {
                advance();
            } else {
                while (!at_end() && is_digit(peek())) {
                    advance();
                }
            }

            // Fraction.
            if (!at_end() && peek() == '.') {
                is_float = true;
                advance();
                if (at_end() || !is_digit(peek())) {
                    fail("digit expected after decimal point");
                }
                while (!at_end() && is_digit(peek())) {
                    advance();
                }
            }

            // Exponent.
            if (!at_end() && (peek() == 'e' || peek() == 'E')) {
                is_float = true;
                advance();
                if (!at_end() && (peek() == '+' || peek() == '-')) {
                    advance();
                }
                if (at_end() || !is_digit(peek())) {
                    fail("digit expected in exponent");
                }
                while (!at_end() && is_digit(peek())) {
                    advance();
                }
            }

            const StringView token = _text.substr(start, _pos - start);

            if (!is_float) {
                if (Optional<i64> integer = Long::parse(token)) {
                    return Json(*integer);
                }
                // Too big for i64: fall through and keep it as a float.
            }

            if (Optional<f64> number = Double::parse(token)) {
                return Json(*number);
            }
            fail("invalid number");
            Ops::unreachable();
        }

        /**
         * @brief Parses a quoted string (the opening quote is at the cursor).
         * @return The decoded string contents.
         * @throws JsonParseException on an unterminated string, a control
         * character, or a malformed escape.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        String parse_string() {
            advance(); // opening quote
            String out;
            while (true) {
                if (at_end()) {
                    fail("unterminated string");
                }
                const char c = advance();
                if (c == '"') {
                    return out;
                }
                if (c == '\\') {
                    parse_escape(out);
                    continue;
                }
                // Raw control characters are not permitted in a JSON string.
                if (static_cast<unsigned char>(c) < 0x20) {
                    fail("control character in string");
                }
                out += c;
            }
        }

        /**
         * @brief Decodes one backslash escape into @p out (the '\\' is consumed).
         * @param out The string being built.
         * @throws JsonParseException on an unknown or truncated escape.
         */
        THROWS(JsonParseException)
        void parse_escape(String& out) {
            if (at_end()) {
                fail("unterminated escape");
            }
            const char e = advance();
            switch (e) {
                case '"':
                    out += '"';
                    break;
                case '\\':
                    out += '\\';
                    break;
                case '/':
                    out += '/';
                    break;
                case 'b':
                    out += '\b';
                    break;
                case 'f':
                    out += '\f';
                    break;
                case 'n':
                    out += '\n';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case 't':
                    out += '\t';
                    break;
                case 'u':
                    parse_unicode_escape(out);
                    break;
                default:
                    fail("invalid escape sequence");
            }
        }

        /**
         * @brief Decodes a `\\uXXXX` escape (the 'u' is already consumed).
         * @param out The string being built; the decoded code point is appended
         * as UTF-8.
         * @throws JsonParseException on a short escape, a bad hex digit, or a
         * malformed surrogate pair.
         *
         * A high surrogate must be followed by a `\\uXXXX` low surrogate; the
         * pair is combined into the astral code point before encoding. A lone
         * surrogate is rejected rather than emitted, so the output is always
         * well-formed UTF-8.
         */
        THROWS(JsonParseException)
        void parse_unicode_escape(String& out) {
            u32 code = read_hex4();
            if (code >= 0xD800 && code <= 0xDBFF) {
                // High surrogate: a low surrogate must follow.
                if (at_end() || advance() != '\\' || at_end() || advance() != 'u') {
                    fail("expected low surrogate after high surrogate");
                }
                const u32 low = read_hex4();
                if (low < 0xDC00 || low > 0xDFFF) {
                    fail("invalid low surrogate");
                }
                code = 0x10000 + ((code - 0xD800) << 10) + (low - 0xDC00);
            } else if (code >= 0xDC00 && code <= 0xDFFF) {
                fail("unexpected low surrogate");
            }
            encode_utf8(code, out);
        }

        /**
         * @brief Reads exactly four hex digits as a 16-bit value.
         * @return The decoded value.
         * @throws JsonParseException if fewer than four hex digits are present.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        u32 read_hex4() {
            u32 value = 0;
            for (usize i = 0; i < 4; ++i) {
                if (at_end()) {
                    fail("truncated unicode escape");
                }
                const char c = advance();
                u32 digit;
                if (c >= '0' && c <= '9') {
                    digit = static_cast<u32>(c - '0');
                } else if (c >= 'a' && c <= 'f') {
                    digit = static_cast<u32>(c - 'a' + 10);
                } else if (c >= 'A' && c <= 'F') {
                    digit = static_cast<u32>(c - 'A' + 10);
                } else {
                    fail("invalid hex digit in unicode escape");
                }
                value = (value << 4) | digit;
            }
            return value;
        }

        /**
         * @brief Appends a code point to @p out as UTF-8.
         * @param code The Unicode code point.
         * @param out The string being built.
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

        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_array(usize depth) {
            if (depth >= MAX_DEPTH) {
                fail("maximum nesting depth exceeded");
            }
            advance(); // '['
            Json::Array elements;
            skip_whitespace();
            if (!at_end() && peek() == ']') {
                advance();
                return Json(Ops::move(elements));
            }
            while (true) {
                skip_whitespace();
                elements.push_back(parse_value(depth + 1));
                skip_whitespace();
                if (at_end()) {
                    fail("unterminated array");
                }
                const char c = advance();
                if (c == ']') {
                    break;
                }
                if (c != ',') {
                    fail("expected ',' or ']' in array");
                }
            }
            return Json(Ops::move(elements));
        }

        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_object(usize depth) {
            if (depth >= MAX_DEPTH) {
                fail("maximum nesting depth exceeded");
            }
            advance(); // '{'
            JsonObject object;
            skip_whitespace();
            if (!at_end() && peek() == '}') {
                advance();
                return Json(Ops::move(object));
            }
            while (true) {
                skip_whitespace();
                if (at_end() || peek() != '"') {
                    fail("expected string key in object");
                }
                String key = parse_string();
                skip_whitespace();
                if (at_end() || advance() != ':') {
                    fail("expected ':' after object key");
                }
                skip_whitespace();
                // Duplicate keys: last value wins, per JsonObject::set.
                object.set(key, parse_value(depth + 1));
                skip_whitespace();
                if (at_end()) {
                    fail("unterminated object");
                }
                const char c = advance();
                if (c == '}') {
                    break;
                }
                if (c != ',') {
                    fail("expected ',' or '}' in object");
                }
            }
            return Json(Ops::move(object));
        }

        [[nodiscard]]
        static bool is_digit(char c) noexcept {
            return c >= '0' && c <= '9';
        }
    public:
        explicit JsonParser(StringView text) noexcept:
            _text{text} {}

        /**
         * @brief Parses the whole document.
         * @return The parsed value.
         * @throws JsonParseException on any malformed input, or on trailing
         * non-whitespace after the top-level value.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        Json parse_document() {
            skip_whitespace();
            Json value = parse_value(0);
            skip_whitespace();
            if (!at_end()) {
                fail("trailing characters after JSON value");
            }
            return value;
        }
    };

    THROWS(JsonParseException)
    inline Json Json::parse(StringView text) {
        return JsonParser(text).parse_document();
    }

    inline Optional<Json> Json::try_parse(StringView text) noexcept {
        try {
            return JsonParser(text).parse_document();
        } catch (const JsonParseException&) {
            return nullopt;
        }
    }
}
