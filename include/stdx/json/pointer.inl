#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
export namespace stdx::json {
    /**
     * @class JsonPointer
     * @brief A parsed JSON Pointer (RFC 6901) addressing a value within a document.
     *
     * A pointer is a sequence of reference tokens, written as a string with each
     * token introduced by '/': `/users/0/name` addresses the `name` member of
     * the first element of the `users` array. The empty string is the pointer to
     * the whole document. Within a token, '/' and '~' are escaped as `~1` and
     * `~0` respectively, so a key that literally contains a slash is still
     * addressable.
     */
    class JsonPointer {
    private:
        Vector<String> _tokens; ///< The decoded reference tokens, in order (empty for the whole-document pointer).

        /**
         * @brief Decodes the RFC 6901 escapes in a single reference token.
         * @param token The raw token text (between two '/').
         * @return The decoded token.
         *
         * `~1` becomes '/' and `~0` becomes '~'. The order matters: `~1` is
         * resolved before `~0` so that `~01` decodes to `~1` and not to '/'.
         * Done in one left-to-right pass, which has that property automatically.
         */
        [[nodiscard]]
        static String unescape(StringView token) {
            String out;
            out.reserve(token.size());
            for (usize i = 0; i < token.size(); ++i) {
                if (token[i] != '~' || i + 1 >= token.size()) {
                    out += token[i];
                    continue;
                }
                const char next = token[i + 1];
                if (next == '1') {
                    out += '/';
                    ++i;
                } else if (next == '0') {
                    out += '~';
                    ++i;
                } else {
                    // A '~' not followed by 0 or 1 is left as-is; parse() has
                    // already rejected such a token, so this cannot arise from a
                    // constructed JsonPointer.
                    out += '~';
                }
            }
            return out;
        }

        /**
         * @brief Re-encodes a decoded token for @ref to_string.
         * @param token The decoded token.
         * @return The token with '~' and '/' escaped.
         *
         * '~' is escaped before '/': escaping '/' first would introduce a '~'
         * (as `~1`) that the '~' pass would then double-escape.
         */
        [[nodiscard]]
        static String escape(StringView token) {
            String out;
            out.reserve(token.size());
            for (const char c: token) {
                if (c == '~') {
                    out += "~0";
                } else if (c == '/') {
                    out += "~1";
                } else {
                    out += c;
                }
            }
            return out;
        }

        /**
         * @brief Whether every '~' in @p token introduces a valid `~0`/`~1` escape.
         * @param token The raw token to validate.
         * @return true if the escaping is well formed, false otherwise.
         */
        [[nodiscard]]
        static bool valid_escaping(StringView token) noexcept {
            for (usize i = 0; i < token.size(); ++i) {
                if (token[i] != '~') {
                    continue;
                }
                if (i + 1 >= token.size() || (token[i + 1] != '0' && token[i + 1] != '1')) {
                    return false;
                }
                ++i;
            }
            return true;
        }
    public:
        /**
         * @brief Constructs the whole-document pointer (no tokens).
         */
        JsonPointer() = default;

        /**
         * @brief Parses pointer text into a JsonPointer.
         * @param text The pointer, e.g. "/foo/0/bar" or "" for the whole document.
         * @return The parsed pointer.
         * @throws JsonPointerException if @p text is non-empty and does not begin
         * with '/', or if a token has a malformed '~' escape.
         */
        [[nodiscard]]
        THROWS(JsonPointerException)
        static JsonPointer parse(StringView text) {
            JsonPointer result;
            if (text.empty()) {
                return result;
            }
            if (text.front() != '/') {
                throw JsonPointerException(
                    Ops::fmt("JSON Pointer must be empty or begin with '/': {}", text)
                );
            }
            // Drop the leading '/', then split on '/'. "/a/b" -> {"a","b"};
            // "/" -> {""} (the member whose key is the empty string); a trailing
            // '/' likewise yields a final empty token.
            usize start = 1uz;
            while (true) {
                usize slash = text.find('/', start);
                const StringView raw = (slash == StringView::npos)
                    ? text.substr(start)
                    : text.substr(start, slash - start);
                if (!valid_escaping(raw)) {
                    throw JsonPointerException(
                        Ops::fmt("malformed '~' escape in JSON Pointer token: {}", raw)
                    );
                }
                result._tokens.push_back(unescape(raw));
                if (slash == StringView::npos) {
                    break;
                }
                start = slash + 1;
            }
            return result;
        }

        /**
         * @brief Parses pointer text, reporting failure as an empty Optional.
         * @param text The pointer text.
         * @return The parsed pointer, or an empty Optional if @p text is malformed.
         */
        [[nodiscard]]
        static Optional<JsonPointer> try_parse(StringView text) noexcept {
            try {
                return parse(text);
            } catch (const JsonPointerException&) {
                return nullopt;
            }
        }

        /**
         * @brief The decoded reference tokens, in order.
         * @return The tokens (empty for the whole-document pointer).
         */
        [[nodiscard]]
        const Vector<String>& tokens() const noexcept {
            return _tokens;
        }

        /**
         * @brief Whether this is the whole-document pointer (no tokens).
         * @return true if there are no tokens, false otherwise.
         */
        [[nodiscard]]
        bool empty() const noexcept {
            return _tokens.empty();
        }

        /**
         * @brief The number of reference tokens.
         * @return The token count.
         */
        [[nodiscard]]
        usize size() const noexcept {
            return _tokens.size();
        }

        /**
         * @brief Appends a reference token (used as-is, not re-parsed).
         * @param token The decoded token to append.
         * @return A reference to this pointer, for chaining.
         *
         * The token is a literal key or index, taken verbatim; any '/' or '~' in
         * it is data, and will be escaped by @ref to_string.
         */
        JsonPointer& push(StringView token) {
            _tokens.emplace_back(token);
            return *this;
        }

        /**
         * @brief Removes the last reference token, if any.
         * @return A reference to this pointer, for chaining.
         */
        JsonPointer& pop() {
            if (!_tokens.empty()) {
                _tokens.pop_back();
            }
            return *this;
        }

        /**
         * @brief Serializes back to RFC 6901 pointer text.
         * @return The pointer string ("" for the whole-document pointer), with
         * '~' and '/' in each token re-escaped.
         */
        [[nodiscard]]
        String to_string() const {
            String out;
            for (const String& token: _tokens) {
                out += '/';
                out += escape(token);
            }
            return out;
        }

        [[nodiscard]]
        bool operator==(const JsonPointer& other) const = default;
    };
}

/**
 * @namespace stdx::json
 * @brief Resolution of JSON Pointers against Json values.
 */
namespace stdx::json {
    /**
     * @internal
     * @brief Whether a token is a valid array index for a @p size-element array.
     * @param token The decoded token.
     * @param size The array length.
     * @return The index if @p token is all digits, has no leading zero (unless
     * it is exactly "0"), and is in [0, size); otherwise an empty Optional.
     *
     * RFC 6901 array indices are the unsigned integers with no leading zeros,
     * plus "-" (one past the end). "-" is only meaningful for insertion, so it
     * never resolves to an existing element and is rejected here.
     */
    [[nodiscard]]
    inline Optional<usize> array_index(StringView token, usize size) noexcept {
        if (token.empty()) {
            return nullopt;
        }
        if (token == "0") {
            return size > 0 ? Optional(0uz) : nullopt;
        }
        if (token.front() == '0') {
            return nullopt; // leading zero is not a valid index
        }
        usize value = 0uz;
        for (const char c: token) {
            if (c < '0' || c > '9') {
                return nullopt;
            }
            value = value * 10uz + static_cast<usize>(c - '0');
            if (value >= size) {
                // Still consume the rest to stay O(n), but it is out of range;
                // once past size it can only grow, so bail.
                return nullopt;
            }
        }
        return value;
    }

    /**
     * @internal
     * @brief Shared resolution for the const and mutable Json pointer lookups.
     * @tparam J Json or const Json.
     * @param root The value to resolve against.
     * @param pointer The parsed pointer.
     * @return A pointer to the addressed value, or nullptr.
     */
    template <typename J>
    [[nodiscard]]
    J* resolve_pointer(J* root, const JsonPointer& pointer) noexcept {
        J* current = root;
        for (const String& token: pointer.tokens()) {
            if (current->is_object()) {
                current = current->as_object().find(token);
                if (current == nullptr) {
                    return nullptr;
                }
            } else if (current->is_array()) {
                auto& array = current->as_array();
                const Optional<usize> index = array_index(token, array.size());
                if (!index.has_value()) {
                    return nullptr;
                }
                current = &array[*index];
            } else {
                // A token remains but the current value is a scalar: no match.
                return nullptr;
            }
        }
        return current;
    }

    inline const Json* Json::find(const JsonPointer& pointer) const noexcept {
        return resolve_pointer<const Json>(this, pointer);
    }

    inline Json* Json::find(const JsonPointer& pointer) noexcept {
        return resolve_pointer<Json>(this, pointer);
    }

    inline const Json* Json::find(StringView pointer) const noexcept {
        const Optional<JsonPointer> parsed = JsonPointer::try_parse(pointer);
        return parsed.has_value() ? find(*parsed) : nullptr;
    }

    inline Json* Json::find(StringView pointer) noexcept {
        const Optional<JsonPointer> parsed = JsonPointer::try_parse(pointer);
        return parsed.has_value() ? find(*parsed) : nullptr;
    }

    inline const Json& Json::at(const JsonPointer& pointer) const {
        const Json* found = find(pointer);
        if (found == nullptr) {
            throw JsonPointerException(
                Ops::fmt("JSON Pointer does not resolve: {}", pointer.to_string())
            );
        }
        return *found;
    }

    inline Json& Json::at(const JsonPointer& pointer) {
        Json* found = find(pointer);
        if (found == nullptr) {
            throw JsonPointerException(
                Ops::fmt("JSON Pointer does not resolve: {}", pointer.to_string())
            );
        }
        return *found;
    }

    inline bool Json::contains(const JsonPointer& pointer) const noexcept {
        return find(pointer) != nullptr;
    }
}

using stdx::json::JsonPointer;
using stdx::json::JsonPointerException;

/**
 * @namespace stdx::literals
 * @brief Standard library user-defined literals.
 */
export namespace stdx::literals::inline json_literals {
    /**
     * @brief Builds a JSON Pointer literal at the call site.
     * @param s The pointer text.
     * @param len Its length.
     * @return The parsed pointer.
     * @throws JsonPointerException if the literal is a malformed pointer.
     *
     * Lets a pointer be written inline: `doc.at("/users/0/name"_jptr)`. Throws on
     * malformed syntax, since a literal is written by the programmer.
     */
    [[nodiscard]]
    THROWS(JsonPointerException)
    inline JsonPointer operator""_jptr(const char* s, usize len) {
        return JsonPointer::parse(StringView(s, len));
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
    struct Formatter<JsonPointer> {
        constexpr auto parse(FormatParseContext& ctx) {
            return ctx.begin();
        }

        auto format(const JsonPointer& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<JsonPointer>: public Formatter<JsonPointer> {};

