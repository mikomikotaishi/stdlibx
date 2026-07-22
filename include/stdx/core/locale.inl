#pragma once

using stdx::text::string::String;
using stdx::text::string::StringView;
using stdx::text::string::BasicString;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @class LocaleException
     * @brief Thrown when a named locale cannot be provided by the host.
     * @extends RuntimeException
     *
     * Mirrors the fact that, unlike Java (which bundles its own locale data),
     * std::locale delegates to the host's installed locale database: a name
     * such as "ja_JP" only constructs if the platform actually has it.
     */
    class LocaleException: public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
    };

    /**
     * @class Locale
     * @brief A thin, value-semantic wrapper over std::locale.
     *
     * @note Only @ref classic and @ref system are guaranteed to succeed. The
     * predefined identifiers (@ref us, @ref japan, ...) are canonical *names*,
     * not guaranteed-installed locales - constructing one depends on the host's
     * locale database and may yield nothing (@ref of).
     */
    class [[nodiscard]] Locale {
    public:
        using Self = std::locale; ///< The underlying standard locale type.
        using Id = std::locale::id; ///< The facet-identification type.
        using Facet = std::locale::facet; ///< The base facet type.
        
        class [[nodiscard]] Category final {
        public:
            using Self = std::locale::category;

            static constexpr Self NONE = std::locale::none;
            static constexpr Self COLLATE = std::locale::collate;
            static constexpr Self CTYPE = std::locale::ctype;
            static constexpr Self MONETARY = std::locale::monetary;
            static constexpr Self NUMERIC = std::locale::numeric;
            static constexpr Self TIME = std::locale::time;
            static constexpr Self MESSAGES = std::locale::messages;
            static constexpr Self ALL = std::locale::all;
        private:
            const Self value = NONE;
        public:
            constexpr Category(Self value) noexcept:
                value{value} {}

            constexpr operator Self() const noexcept {
                return value;
            }
        };
    private:
        Self locale{}; ///< The wrapped locale (defaults to a copy of the global locale).

        /**
         * @brief Attempt to construct a std::locale from @p name without throwing.
         *
         * Tries the name verbatim first; on POSIX, if that fails and the name
         * carries no explicit codeset, retries with a ".UTF-8" suffix (the
         * common form of installed locales there).
         */
        [[nodiscard]]
        static Optional<Self> try_build(StringView name) noexcept {
            try {
                return Self(String(name).c_str());
            } catch (...) {
            }
            #ifdef __unix__
            if (!name.empty() && name != "C" && name != "POSIX"
                && name.find('.') == StringView::npos) {
                try {
                    String with_codeset(name);
                    with_codeset += ".UTF-8";
                    return Self(with_codeset.c_str());
                } catch (...) {
                }
            }
            #endif
            return nullopt;
        }
    public:
        /**
         * @brief Constructs a Locale that copies the current global locale.
         */
        Locale() noexcept = default;
        Locale(const Locale&) = default;
        Locale& operator=(const Locale&) noexcept = default;
        ~Locale() = default;

        /**
         * @brief Wraps an existing std::locale (implicit - the types are interchangeable).
         * @param locale The locale to wrap.
         */
        Locale(const Self& loc) noexcept:
            locale{loc} {}

        /**
         * @brief Constructs a Locale from a name.
         * @param name The locale name (e.g. "en_US.UTF-8").
         * @throws LocaleException if the host has no such locale.
         */
        THROWS(LocaleException)
        explicit Locale(const char* name) try:
            locale{name} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", name));
        }

        /**
         * @brief Constructs a Locale from a name.
         * @param name The locale name (e.g. "en_US.UTF-8").
         * @throws LocaleException if the host has no such locale.
         */
        THROWS(LocaleException)
        explicit Locale(const String& name) try:
            locale{name} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", name));
        }

        /**
         * @brief Constructs a Locale from @p other with the named categories replaced.
         * @param other The locale to copy the untouched categories from.
         * @param name The locale name supplying the replaced categories.
         * @param categories The categories to take from @p name.
         * @throws LocaleException if the host has no such locale.
         */
        THROWS(LocaleException)
        Locale(const Locale& other, const char* name, Category categories) try:
            locale{Self(other.locale, name, categories)} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", name));
        }

        /**
         * @brief Constructs a Locale from @p other with the named categories replaced.
         * @param other The locale to copy the untouched categories from.
         * @param name The locale name supplying the replaced categories.
         * @param categories The categories to take from @p name.
         * @throws LocaleException if the host has no such locale.
         */
        THROWS(LocaleException)
        Locale(const Locale& other, const String& name, Category categories) try:
            locale{Self(other.locale, name, categories)} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", name));
        }

        /**
         * @brief Constructs a Locale from @p other with categories taken from @p one.
         * @param other The locale to copy the untouched categories from.
         * @param one The locale supplying the replaced categories.
         * @param categories The categories to take from @p one.
         * @throws LocaleException if the host has no such locale.
         */
        THROWS(LocaleException)
        Locale(const Locale& other, const Locale& one, Category categories) try:
            locale{Self(other.locale, one.locale, categories)} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", one.name()));
        }

        /**
         * @brief Constructs a Locale from @p other with @p facet installed.
         * @tparam F The facet type.
         * @param other The locale to copy.
         * @param facet The facet to install (ownership is transferred to the locale).
         * @throws LocaleException if the host has no such locale.
         */
        template <typename F>
        THROWS(LocaleException)
        Locale(const Locale& other, F* facet) try:
            locale{Self(other.locale, facet)} {
        } catch (...) {
            throw LocaleException(std::format("unsupported locale: {}", other.name()));
        }

        /**
         * @brief Converts back to the underlying std::locale.
         * @return A copy of the wrapped locale.
         */
        constexpr operator Self() const noexcept {
            return locale;
        }

        /**
         * @brief The name of the locale.
         * @return The locale name, or "*" if it has no name.
         */
        [[nodiscard]]
        String name() const noexcept {
            return locale.name();
        }

        #ifdef __cpp_lib_text_encoding
        /**
         * @brief The character encoding of the locale.
         *
         * std::locale exposes no encoding directly, so this derives it from the
         * codeset suffix of the locale name (e.g. "en_US.UTF-8" -> UTF-8). A
         * locale whose name carries no codeset (such as the classic "C" locale)
         * reports an unknown encoding.
         * @return The locale's encoding, or an unknown encoding if none is named.
         */
        [[nodiscard]]
        text::TextEncoding encoding() const noexcept {
            const String locale_name = locale.name();
            const auto dot = locale_name.find('.');
            if (dot == String::npos) {
                return text::TextEncoding();
            }
            StringView codeset = StringView(locale_name).substr(dot + 1);
            if (const auto at = codeset.find('@'); at != StringView::npos) {
                codeset = codeset.substr(0, at);
            }
            return text::TextEncoding(codeset);
        }
        #endif

        /**
         * @brief Constructs a locale combining this one with a facet from @p other.
         * @tparam F The facet type to take from @p other.
         * @param other The locale supplying the facet.
         * @return The combined locale.
         */
        template <typename F>
        [[nodiscard]]
        Locale combine(const Locale& other) const {
            return Locale(locale.combine<F>(other.locale));
        }

        /**
         * @brief Lexicographically compares two strings using this locale's collate facet.
         * @return true if @p lhs orders before @p rhs.
         */
        template <typename Char, typename Traits, typename Alloc>
        [[nodiscard]]
        bool operator()(
            const BasicString<Char, Traits, Alloc>& lhs,
            const BasicString<Char, Traits, Alloc>& rhs
        ) const {
            return locale(lhs, rhs);
        }

        /**
         * @brief Whether two locales are the same (or copies of one another).
         */
        [[nodiscard]]
        bool operator==(const Locale& other) const noexcept {
            return locale == other.locale;
        }

        /**
         * @brief The "C" locale - locale-independent, always available.
         * @return A reference to the classic locale.
         */
        [[nodiscard]]
        static const Locale& classic() noexcept {
            static const Locale instance{Self::classic()};
            return instance;
        }

        /**
         * @brief The environment's preferred locale (from LC_* /LANG), or the
         * classic locale if the environment names one the host lacks.
         * @return The system default locale; never throws.
         */
        [[nodiscard]]
        static Locale system() noexcept {
            if (Optional<Self> built = try_build(""); built) {
                return Locale(*built);
            }
            return classic();
        }

        /**
         * @brief Installs @p locale as the global locale.
         * @param locale The locale to make global.
         * @return The previous global locale.
         */
        [[nodiscard]]
        static Locale global(const Locale& loc) {
            return Locale(Self::global(loc.locale));
        }

        /**
         * @brief Builds a locale from @p name if the host provides it.
         * @param name A locale name or one of the predefined identifiers (@ref US, ...).
         * @return The locale, or an empty optional if the host has no match.
         */
        [[nodiscard]]
        static Optional<Locale> of(StringView name) noexcept {
            if (Optional<Self> built = try_build(name); built) {
                return Locale(*built);
            }
            return nullopt;
        }

        /**
         * @name Predefined locales
         *
         * Each accessor returns a reference to a `const Optional<Locale>` built
         * once, on first use (thread-safe), via @ref of - so only the locales
         * actually requested are constructed, nothing is built at load time, and
         * an empty result reflects a host that lacks that locale. Unlike
         * java.util.Locale's always-present constants these can be empty:
         * std::locale draws on the host's installed locale database, not on
         * bundled data.
         * @{
         */
        #define STDLIBX_DEFINE_LOCALE(accessor, locale_name) \
            [[nodiscard]] \
            static const Optional<Locale>& accessor() { \
                static const Optional<Locale> instance = of(locale_name); \
                return instance; \
            }

        STDLIBX_DEFINE_LOCALE(root, "C")
        STDLIBX_DEFINE_LOCALE(english, "en")
        STDLIBX_DEFINE_LOCALE(french, "fr")
        STDLIBX_DEFINE_LOCALE(german, "de")
        STDLIBX_DEFINE_LOCALE(italian, "it")
        STDLIBX_DEFINE_LOCALE(japanese, "ja")
        STDLIBX_DEFINE_LOCALE(korean, "ko")
        STDLIBX_DEFINE_LOCALE(chinese, "zh")
        STDLIBX_DEFINE_LOCALE(simplified_chinese, "zh_CN")
        STDLIBX_DEFINE_LOCALE(traditional_chinese, "zh_TW")
        STDLIBX_DEFINE_LOCALE(france, "fr_FR")
        STDLIBX_DEFINE_LOCALE(germany, "de_DE")
        STDLIBX_DEFINE_LOCALE(italy, "it_IT")
        STDLIBX_DEFINE_LOCALE(japan, "ja_JP")
        STDLIBX_DEFINE_LOCALE(korea, "ko_KR")
        STDLIBX_DEFINE_LOCALE(china, "zh_CN")
        STDLIBX_DEFINE_LOCALE(prc, "zh_CN")
        STDLIBX_DEFINE_LOCALE(taiwan, "zh_TW")
        STDLIBX_DEFINE_LOCALE(uk, "en_GB")
        STDLIBX_DEFINE_LOCALE(us, "en_US")
        STDLIBX_DEFINE_LOCALE(canada, "en_CA")
        STDLIBX_DEFINE_LOCALE(canada_french, "fr_CA")

        #undef STDLIBX_DEFINE_LOCALE
        /** @} */
    };
}
