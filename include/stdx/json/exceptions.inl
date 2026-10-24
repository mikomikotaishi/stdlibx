#pragma once

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
export namespace stdx::json {
    /**
     * @class JsonException
     * @brief Base class for every error raised by the JSON module.
     * @extends Exception
     *
     * Carries a human-readable message. Thrown directly only in situations
     * that fit no more specific subclass; most failures raise a
     * @ref JsonParseException or @ref JsonTypeException instead, both of which
     * are catchable as a JsonException.
     */
    class JsonException: public Exception {
    private:
        String _msg; ///< The human-readable description of the error.
    public:
        explicit JsonException(const String& msg = ""):
            Exception(), _msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    /**
     * @class JsonParseException
     * @brief Thrown when a document is not well-formed JSON.
     * @extends JsonException
     *
     * Records the 1-based line and column at which parsing gave up, so a
     * caller can point at the offending character rather than just report
     * that the whole document failed. The @ref what message already embeds
     * the position; @ref line and @ref column expose it numerically for
     * callers that want to format their own.
     */
    class JsonParseException: public JsonException {
    private:
        usize _line; ///< The 1-based line of the failure.
        usize _column; ///< The 1-based column of the failure.
    public:
        JsonParseException(const String& reason, usize line, usize column):
            JsonException(Ops::fmt("{} (at line {}, column {})", reason, line, column)),
            _line{line}, _column{column} {}

        /**
         * @brief The 1-based line where parsing failed.
         * @return The line number.
         */
        [[nodiscard]]
        usize line() const noexcept {
            return _line;
        }

        /**
         * @brief The 1-based column where parsing failed.
         * @return The column number.
         */
        [[nodiscard]]
        usize column() const noexcept {
            return _column;
        }
    };

    /**
     * @class JsonTypeException
     * @brief Thrown when a value is accessed as a type it does not hold.
     * @extends JsonException
     *
     * Raised by the throwing accessors - @ref Json::get, the conversion
     * operators, @ref Json::at, and the typed element views - when the stored
     * alternative does not match what was asked for. The non-throwing
     * counterparts (@ref Json::as, @ref Json::value, @ref Json::try_parse)
     * never raise this; they report the mismatch in their return type instead.
     */
    class JsonTypeException: public JsonException {
    public:
        using JsonException::JsonException;
    };

    /**
     * @class JsonPointerException
     * @brief Thrown for a malformed JSON Pointer, or a lookup that fails to resolve.
     * @extends JsonException
     *
     * Covers both halves of RFC 6901: a syntactically invalid pointer (one that
     * is non-empty yet does not begin with '/', or carries a stray '~' escape)
     * raised by @ref JsonPointer::parse, and a pointer that is well-formed but
     * does not address an existing value raised by @ref Json::at(const JsonPointer&).
     * The non-throwing @ref JsonPointer::try_parse and @ref Json::find report
     * those same conditions through their return type instead.
     */
    class JsonPointerException: public JsonException {
    public:
        using JsonException::JsonException;
    };
}
