#pragma once

using stdx::collections::Vector;
using stdx::net::Uri;
using stdx::time::Seconds;

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
    /**
     * @enum Encryption
     * @brief The transport security to ask a driver for.
     *
     * ODBC standardises no encryption keyword, so every driver spells this its
     * own way - and a driver silently drops keywords it does not recognise,
     * which for a security setting is an unnoticed downgrade rather than an
     * error. The library therefore writes these modes only for drivers whose
     * spelling it knows, and refuses rather than guessing for the rest.
     */
    enum class Encryption: u8 {
        DRIVER_DEFAULT, ///< Write nothing and let the driver decide.
        DISABLED, ///< Do not encrypt the connection.
        TRUSTED, ///< Encrypt, accepting whatever certificate the server offers.
        VERIFIED, ///< Encrypt, and verify the server's certificate.
    };

    /**
     * @struct ConnectionOptions
     * @brief The optional parts of a component-wise connection request.
     */
    struct ConnectionOptions {
        i32 port = 0; ///< The port, or 0 to leave the driver's default.
        Optional<Seconds> login_timeout; ///< How long to allow the login to take, or an empty Optional for the driver's default.
        Encryption encryption = Encryption::DRIVER_DEFAULT; ///< The transport security to ask for.
        String extra; ///< Driver-specific keywords appended verbatim, e.g. "MARS_Connection=yes;".
    };

    /**
     * @class DatabaseUrl
     * @brief The parts of a database URL, decoded, with no driver decision taken.
     *
     * Splits a URL of the shape
     * `scheme://user:password@host:port/database?parameters` into pieces fit to
     * hand to @ref DriverManager::connection - the form configuration arrives in
     * when it comes from a `DATABASE_URL` variable or a settings file.
     *
     * It resolves percent-escapes, and decides nothing else. In particular the
     * scheme is **not** mapped to an ODBC driver name and the parameters are
     * **not** mapped to driver keywords, because neither mapping is in the URL to
     * be read: "postgresql" does not say whether the installed driver is called
     * "PostgreSQL Unicode", "PostgreSQL ANSI" or "psqlODBC", and what libpq
     * spells `sslmode=require` the Microsoft driver spells `Encrypt=yes`.
     * Guessing either would put a working-looking connection where a refusal
     * belongs, so the caller still names the driver and still chooses the
     * @ref Encryption mode:
     *
     * ```
     * const DatabaseUrl url("postgresql://user:pass@db.example.com:5432/store");
     * Connection c = DriverManager::connection(
     *     "PostgreSQL Unicode", url.host(), url.database(), url.username(), url.password(),
     *     {.port = url.port(), .encryption = Encryption::VERIFIED}
     * );
     * ```
     */
    class [[nodiscard]] DatabaseUrl {
    private:
        String _scheme; ///< The scheme as written, e.g. "postgresql". Informational; nothing here reads it.
        String _host; ///< The host or address, decoded. Empty for a file-backed URL such as "sqlite:///tmp/x.db".
        String _path; ///< The path, decoded, exactly as the URL gave it - leading '/' and all.
        String _username; ///< The user name from the userinfo, decoded.
        String _password; ///< The password from the userinfo, decoded.
        Vector<Pair<String, String>> _parameters; ///< The query parameters, each name and value decoded on its own.
        i32 _port = 0; ///< The port, or 0 if the URL gave none, matching ConnectionOptions::port.

        /**
         * @internal
         * @brief Splits a query string into decoded name/value pairs.
         *
         * @param query The raw query, without the leading '?'.
         * @param out The vector to append the pairs to.
         *
         * Splits first and decodes afterwards, one half at a time, so an escaped
         * '&' or '=' inside a value stays inside it instead of becoming the
         * separator it was escaped to avoid being.
         */
        static void parse_parameters(StringView query, Vector<Pair<String, String>>& out) {
            usize start = 0;
            while (true) {
                const auto ampersand = query.find('&', start);
                const StringView pair = (ampersand == StringView::npos)
                    ? query.substr(start)
                    : query.substr(start, ampersand - start);
                if (!pair.empty()) {
                    const auto equals = pair.find('=');
                    if (equals == StringView::npos) {
                        out.emplace_back(Uri::percent_decode(pair), String());
                    } else {
                        out.emplace_back(
                            Uri::percent_decode(pair.substr(0, equals)),
                            Uri::percent_decode(pair.substr(equals + 1))
                        );
                    }
                }
                if (ampersand == StringView::npos) {
                    return;
                }
                start = ampersand + 1;
            }
        }
    public:
        DatabaseUrl() = default;

        /**
         * @brief Takes apart a database URL.
         * @param uri The URL to read.
         * @throws SQLException if @p uri has no scheme.
         */
        THROWS(SQLException)
        DatabaseUrl(const Uri& uri) {
            if (!uri.is_absolute()) {
                throw SQLException(Ops::fmt(
                    "'{}' is a relative reference, not a database URL: a scheme is required, "
                    "as in \"postgresql://host/database\"",
                    uri.to_string()
                ));
            }
            this->_scheme = String(uri.scheme());
            this->_host = Uri::percent_decode(uri.host());
            this->_path = uri.decoded_path();
            if (const Optional<u16> port = uri.port()) {
                this->_port = static_cast<i32>(*port);
            }

            // The raw userinfo is split before either half is decoded. Decoding
            // first would promote an escaped ':' inside the password to the
            // separator, and the credential would divide in the wrong place.
            const StringView userinfo = uri.userinfo();
            if (const auto colon = userinfo.find(':'); colon != StringView::npos) {
                this->_username = Uri::percent_decode(userinfo.substr(0, colon));
                this->_password = Uri::percent_decode(userinfo.substr(colon + 1));
            } else {
                this->_username = Uri::percent_decode(userinfo);
            }

            if (const Optional<StringView> query = uri.query()) {
                parse_parameters(*query, this->_parameters);
            }
        }

        /**
         * @brief Takes apart a database URL.
         * @param uri The URL to read, as a string.
         * @throws SQLException if @p uri has no scheme.
         */
        THROWS(SQLException)
        explicit DatabaseUrl(StringView uri):
            DatabaseUrl(Uri(uri)) {}

        /**
         * @brief Takes apart a database URL, reporting failure rather than throwing.
         * @param uri The URL to read.
         * @return The parts, or an empty Optional if @p uri has no scheme.
         */
        [[nodiscard]]
        static Optional<DatabaseUrl> parse(const Uri& uri) noexcept {
            try {
                return DatabaseUrl(uri);
            } catch (const SQLException& _) {
                return nullopt;
            }
        }

        /**
         * @brief Parses URL text and takes it apart.
         * @param text The URL text.
         * @return The parts, or an empty Optional if @p text is not a URL with a scheme.
         */
        [[nodiscard]]
        static Optional<DatabaseUrl> parse(StringView text) noexcept {
            const Optional<Uri> uri = Uri::parse(text);
            if (!uri.has_value()) {
                return nullopt;
            }
            return parse(*uri);
        }

        /**
         * @brief The scheme, e.g. "postgresql".
         * @return The scheme as written.
         *
         * Offered so a caller can choose a driver from it. Nothing in this
         * library reads it.
         */
        [[nodiscard]]
        StringView scheme() const noexcept {
            return StringView(_scheme);
        }

        /**
         * @brief The host or address.
         * @return The decoded host, empty if the URL had no authority.
         */
        [[nodiscard]]
        StringView host() const noexcept {
            return StringView(_host);
        }

        /**
         * @brief The port.
         * @return The port, or 0 if the URL gave none.
         *
         * 0 means "unspecified" here exactly as it does in
         * @ref ConnectionOptions::port, so it can be passed straight through.
         */
        [[nodiscard]]
        i32 port() const noexcept {
            return _port;
        }

        /**
         * @brief The path, exactly as the URL gave it.
         * @return The decoded path, leading '/' included.
         *
         * This is what a file-backed driver wants: "sqlite:///tmp/store.db" has
         * the path "/tmp/store.db", and the leading slash is part of the file
         * name rather than a separator.
         */
        [[nodiscard]]
        StringView path() const noexcept {
            return StringView(_path);
        }

        /**
         * @brief The database name.
         * @return The path with one leading '/' removed.
         *
         * The convention libpq and JDBC share: in
         * "postgresql://host/store" the database is "store". For a URL whose
         * database is an absolute file path, use @ref path instead - stripping
         * the slash there would turn "/tmp/store.db" into a relative name.
         */
        [[nodiscard]]
        StringView database() const noexcept {
            const StringView path = _path;
            return path.starts_with('/') ? path.substr(1) : path;
        }

        /**
         * @brief The user name.
         * @return The decoded user name, empty if the URL carried none.
         */
        [[nodiscard]]
        StringView username() const noexcept {
            return StringView(_username);
        }

        /**
         * @brief The password.
         * @return The decoded password, empty if the URL carried none.
         */
        [[nodiscard]]
        StringView password() const noexcept {
            return StringView(_password);
        }

        /**
         * @brief Every query parameter, in the order the URL listed them.
         * @return The decoded name/value pairs.
         *
         * Repeated names are kept as they were given rather than collapsed,
         * because which one wins is the driver's business, not this class's.
         */
        [[nodiscard]]
        Span<const Pair<String, String>> parameters() const noexcept {
            return Span<const Pair<String, String>>(_parameters.data(), _parameters.size());
        }

        /**
         * @brief The first value given for @p name.
         * @param name The parameter name, matched exactly.
         * @return The value, or an empty Optional if the URL gave no such parameter.
         *
         * Matching is case-sensitive: URL parameter names are the driver's own
         * vocabulary and this class does not know which of them fold case.
         */
        [[nodiscard]]
        Optional<StringView> parameter(StringView name) const noexcept {
            for (const Pair<String, String>& entry: _parameters) {
                if (entry.first == name) {
                    return StringView(entry.second);
                }
            }
            return nullopt;
        }
    };
}
#endif
