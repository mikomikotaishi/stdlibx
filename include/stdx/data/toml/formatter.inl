#pragma once

using stdx::fmt::Formatter;
using stdx::data::toml::Toml;
using stdx::data::toml::TomlParseException;

/**
 * @namespace stdx::literals
 * @brief Standard library user-defined literals.
 */
export namespace stdx::literals::inline toml_literals {
    /**
     * @brief Parses a TOML literal at the call site.
     * @param s The literal text.
     * @param len Its length.
     * @return The parsed value (a table).
     * @throws TomlParseException if the literal is malformed.
     *
     * Lets a document be written inline: `auto t = "a = 1"_toml;`. Throws on
     * malformed input, since a literal is written by the programmer.
     */
    [[nodiscard]]
    THROWS(TomlParseException)
    inline Toml operator""_toml(const char* s, usize len) {
        return Toml::parse(StringView(s, len));
    }
}

/**
 * @namespace stdx::fmt
 * @brief Standard library formatting operations.
 */
namespace stdx::fmt {
    /**
     * @brief Formats a @ref Toml as its serialized text.
     */
    template <>
    struct Formatter<Toml> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const Toml& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<Toml>: public Formatter<Toml> {};
