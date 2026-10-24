#pragma once

using stdx::fmt::Formatter;
using stdx::json::Json;
using stdx::json::JsonParseException;
using stdx::json::JsonType;

/**
 * @namespace stdx::literals
 * @brief Standard library user-defined literals.
 */
export namespace stdx::literals::inline json_literals {
    /**
     * @brief Parses a JSON literal at the call site.
     * @param s The literal text.
     * @param len Its length.
     * @return The parsed value.
     * @throws JsonParseException if the literal is malformed.
     *
     * Lets a document be written inline: `auto j = R"({"a":1})"_json;`. Throws
     * on malformed input.
     */
    [[nodiscard]]
    THROWS(JsonParseException)
    inline Json operator""_json(const char* s, usize len) {
        return Json::parse(StringView(s, len));
    }
}

/**
 * @namespace stdx::fmt
 * @brief Standard library formatting operations.
 */
namespace stdx::fmt {
    /**
     * @brief Formats a @ref Json.
     */
    template <>
    struct Formatter<Json> {
        bool pretty = false; ///< Whether the '#' alternate flag was given.

        constexpr auto parse(FormatParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it == '#') {
                pretty = true;
                ++it;
            }
            return it;
        }

        auto format(const Json& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", pretty ? value.dump() : value.to_string());
        }
    };

    template <>
    struct Formatter<Json::Type> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(Json::Type value, FormatContext& ctx) const {
            StringView name;
            switch (value) {
                case Json::Type::NULL_VALUE:
                    name = "null";
                    break;
                case Json::Type::BOOLEAN:
                    name = "boolean";
                    break;
                case Json::Type::INTEGER:
                case Json::Type::FLOAT:
                    name = "number";
                    break;
                case Json::Type::STRING:
                    name = "string";
                    break;
                case Json::Type::ARRAY:
                    name = "array";
                    break;
                case Json::Type::OBJECT:
                    name = "object";
                    break;
            }
            return format_to(ctx.out(), "{}", name);
        }
    };
}

template <>
struct stdx::fmt::formatter<Json>: public Formatter<Json> {};

template <>
struct stdx::fmt::formatter<Json::Type>: public Formatter<Json::Type> {};
