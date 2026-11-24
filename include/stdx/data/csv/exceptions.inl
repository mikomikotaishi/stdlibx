#pragma once

/**
 * @namespace stdx::data::csv
 * @brief CSV parsing, construction, and serialization (RFC 4180).
 */
export namespace stdx::data::csv {
    /**
     * @class CsvException
     * @brief Base class for every error raised by the CSV module.
     * @extends Exception
     *
     * Carries a human-readable message. Thrown directly only in situations that
     * fit no more specific subclass; most failures raise a @ref CsvParseException
     * or @ref CsvTypeException instead, both catchable as a CsvException.
     */
    class CsvException: public Exception {
    private:
        String _msg; ///< The human-readable description of the error.
    public:
        explicit CsvException(const String& msg = ""):
            Exception(), _msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    /**
     * @class CsvParseException
     * @brief Thrown when a document is not well-formed CSV.
     * @extends CsvException
     *
     * Records the 1-based line and column at which parsing gave up - typically
     * an unterminated quoted field or a stray quote inside an unquoted one - so
     * a caller can point at the offending character. @ref what already embeds
     * the position; @ref line and @ref column expose it numerically.
     */
    class CsvParseException: public CsvException {
    private:
        usize _line; ///< The 1-based line of the failure.
        usize _column; ///< The 1-based column of the failure.
    public:
        CsvParseException(const String& reason, usize line, usize column):
            CsvException(Ops::fmt("{} (at line {}, column {})", reason, line, column)),
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
     * @class CsvTypeException
     * @brief Thrown when a field or column is accessed in a way it cannot satisfy.
     * @extends CsvException
     *
     * Raised by the throwing accessors - an out-of-range field index, a column
     * name absent from the header, or a field that cannot convert to a requested
     * type. The non-throwing counterparts report those conditions through their
     * return type instead.
     */
    class CsvTypeException: public CsvException {
    public:
        using CsvException::CsvException;
    };
}
