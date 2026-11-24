#pragma once

using stdx::fmt::Formatter;
using stdx::data::csv::CsvDocument;
using stdx::data::csv::CsvParseException;

/**
 * @namespace stdx::literals
 * @brief Standard library user-defined literals.
 */
export namespace stdx::literals::inline csv_literals {
    /**
     * @brief Parses a CSV literal at the call site under the standard dialect.
     * @param s The literal text.
     * @param len Its length.
     * @return The parsed document.
     * @throws CsvParseException if the literal is malformed.
     *
     * Lets a table be written inline: `auto doc = "a,b\n1,2\n"_csv;`. Throws on
     * malformed input, since a literal is written by the programmer.
     */
    [[nodiscard]]
    THROWS(CsvParseException)
    inline CsvDocument operator""_csv(const char* s, usize len) {
        return CsvDocument::parse(StringView(s, len));
    }
}

/**
 * @namespace stdx::fmt
 * @brief Standard library formatting operations.
 */
namespace stdx::fmt {
    /**
     * @brief Formats a @ref CsvDocument as its standard-dialect text.
     */
    template <>
    struct Formatter<CsvDocument> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const CsvDocument& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<CsvDocument>: public Formatter<CsvDocument> {};
