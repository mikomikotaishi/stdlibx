#pragma once

/**
 * @namespace stdx::data::toml
 * @brief TOML parsing, construction, and serialization (TOML v1.0.0).
 */
export namespace stdx::data::toml {
    /**
     * @class TomlException
     * @brief Base class for every error raised by the TOML module.
     * @extends Exception
     *
     * Carries a human-readable message. Thrown directly only in situations that
     * fit no more specific subclass; most failures raise a @ref TomlParseException
     * or @ref TomlTypeException instead, both catchable as a TomlException.
     */
    class TomlException: public Exception {
    private:
        String _msg; ///< The human-readable description of the error.
    public:
        explicit TomlException(const String& msg = ""):
            Exception(), _msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    /**
     * @class TomlParseException
     * @brief Thrown when a document is not well-formed TOML.
     * @extends TomlException
     *
     * Records the 1-based line and column at which parsing gave up, so a caller
     * can point at the offending character. @ref what already embeds the
     * position; @ref line and @ref column expose it numerically. Covers both
     * lexical errors (a bad escape, an unterminated string) and the semantic
     * ones TOML uniquely has (redefining a table, overwriting a value with a
     * table, a dotted key that collides with an existing one).
     */
    class TomlParseException: public TomlException {
    private:
        usize _line; ///< The 1-based line of the failure.
        usize _column; ///< The 1-based column of the failure.
    public:
        TomlParseException(const String& reason, usize line, usize column):
            TomlException(Ops::fmt("{} (at line {}, column {})", reason, line, column)),
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
     * @class TomlTypeException
     * @brief Thrown when a value is accessed as a type it does not hold.
     * @extends TomlException
     *
     * Raised by the throwing accessors - @ref Toml::get, @ref Toml::at, the
     * typed element views - when the stored kind does not match what was asked
     * for. The non-throwing counterparts (@ref Toml::as, @ref Toml::try_parse)
     * report the mismatch through their return type instead.
     */
    class TomlTypeException: public TomlException {
    public:
        using TomlException::TomlException;
    };
}
