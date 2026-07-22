#pragma once

using stdx::text::string::StringView;

/**
 * @namespace stdx::text
 * @brief Standard library text operations.
 */
export namespace stdx::text {
    #ifdef __cpp_lib_text_encoding
    /**
     * @class TextEncoding
     * @brief A RFC 3808/IANA registry of character encodings.
     */
    class [[nodiscard]] TextEncoding final {
    public:
        using Self = std::text_encoding; ///< The underlying standard encoding type.
        using AliasesView = std::text_encoding::aliases_view; ///< A view over an encoding's alias names.

        // The nested Id enum wrapper (registered IANA encodings).
        #include "text_encoding.id.inl"

        // The class-scope Id constants (TextEncoding::UTF8, TextEncoding::ASCII, ...).
        #include "text_encoding.constants.inl"

        static constexpr usize MAX_NAME_LENGTH = std::text_encoding::max_name_length; ///< The longest possible encoding name.
    private:
        Self encoding{}; ///< The wrapped encoding (defaults to Id::UNKNOWN).
    public:
        /**
         * @brief Constructs an encoding with @ref Id::UNKNOWN.
         */
        constexpr TextEncoding() noexcept = default;
        constexpr TextEncoding(const TextEncoding&) noexcept = default;
        constexpr TextEncoding& operator=(const TextEncoding&) noexcept = default;
        ~TextEncoding() = default;

        /**
         * @brief Wraps an existing std::text_encoding (implicit - the types are interchangeable).
         * @param encoding The encoding to wrap.
         */
        constexpr TextEncoding(const Self& encoding) noexcept:
            encoding{encoding} {}

        /**
         * @brief Constructs an encoding from a registered id (e.g. @ref UTF8).
         * @param id The IANA MIBenum identifier.
         */
        constexpr TextEncoding(Id id) noexcept:
            encoding{Self(id)} {}

        /**
         * @brief Constructs an encoding by looking @p name up in the IANA registry.
         * @param name The encoding name or one of its registered aliases.
         *
         * The lookup is case-insensitive and ignores non-alphanumeric characters.
         * If @p name is not registered, @ref mib is @ref Id::OTHER and @ref name
         * echoes @p name back (truncated to @ref MAX_NAME_LENGTH).
         */
        constexpr explicit TextEncoding(StringView name) noexcept:
            encoding{Self(name)} {}

        /**
         * @brief Converts back to the underlying std::text_encoding.
         * @return A copy of the wrapped encoding.
         */
        constexpr operator Self() const noexcept {
            return encoding;
        }

        /**
         * @brief The IANA MIBenum identifier of this encoding.
         * @return The @ref Id, or @ref Id::OTHER / @ref Id::UNKNOWN for an
         * unregistered / default-constructed encoding.
         */
        [[nodiscard]]
        constexpr Id mib() const noexcept {
            return encoding.mib();
        }

        /**
         * @brief The canonical (primary) name of this encoding.
         * @return The name, viewing the encoding's own storage (empty if unknown).
         */
        [[nodiscard]]
        constexpr StringView name() const noexcept {
            return StringView(encoding.name());
        }

        /**
         * @brief The registered aliases of this encoding.
         * @return A view over the alias names (empty for an unregistered encoding).
         */
        [[nodiscard]]
        constexpr AliasesView aliases() const noexcept {
            return encoding.aliases();
        }

        /**
         * @brief Whether two encodings denote the same character set.
         *
         * Registered encodings compare by MIBenum; two unregistered (@ref Id::OTHER)
         * encodings compare by name, case-insensitively.
         */
        [[nodiscard]]
        constexpr bool operator==(const TextEncoding&) const noexcept = default;

        /**
         * @brief Whether this encoding has the given id.
         */
        [[nodiscard]]
        constexpr bool operator==(Id id) const noexcept {
            return encoding == id;
        }

        constexpr TextEncoding::Self native() const noexcept {
            return encoding;
        }

        /**
         * @brief The encoding of the ordinary character literals in this translation unit.
         * @return The compiler's literal (execution) encoding, resolved at compile time.
         */
        [[nodiscard]]
        static consteval TextEncoding literal() noexcept {
            return TextEncoding(Self::literal());
        }

        /**
         * @brief The encoding of the current environment (from the "C" locale).
         * @return The environment encoding, determined at run time.
         */
        [[nodiscard]]
        static TextEncoding environment() {
            return TextEncoding(Self::environment());
        }

        /**
         * @brief Whether the environment encoding is @p id.
         * @param id The encoding to test for.
         * @return true if the current environment encoding matches @p id.
         */
        [[nodiscard]]
        static bool environment_is(Id id) {
            return environment() == id;
        }
    };

    #include "text_encoding.constants.deferred.inl"
    #endif
}

#ifdef __cpp_lib_text_encoding
using stdx::text::TextEncoding;

namespace stdx::core {
    template <>
    struct Hash<TextEncoding> {
        [[nodiscard]]
        usize operator()(const TextEncoding& value) const noexcept {
            return std::hash<TextEncoding::Self>()(value);
        }
    };
}

template <>
struct stdx::core::hash<TextEncoding> : public stdx::core::Hash<TextEncoding> {};
#endif
