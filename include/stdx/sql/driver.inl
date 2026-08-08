#pragma once

using stdx::collections::Queue;
using stdx::net::IPAddress;
using stdx::sync::ConditionVariable;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;
using stdx::sync::UniqueLock;
using stdx::time::Seconds;

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
    /**
     * @class DriverManager
     * @brief Manages database drivers and provides connection factory methods.
     *
     * Provides static methods to obtain database connections, similar to JDBC's DriverManager.
     */
    class DriverManager {
    public:
        DriverManager() = DELETE_METHOD("DriverManager is a static class and cannot be instantiated.");

        /**
         * @brief Establishes a connection to the database.
         * @param conn_str The ODBC connection string.
         * @param login_timeout How long to allow the login to take, or an empty Optional for the driver's default.
         * @return A Connection object.
         * @throws SQLException if the connection fails.
         *
         * Example connection strings:
         * - "Driver={SQL Server};Server=localhost;Database=mydb;Uid=user;Pwd=pass;"
         * - "DSN=mydsn;Uid=user;Pwd=pass;"
         * - "Driver={PostgreSQL Unicode};Server=localhost;Port=5432;Database=mydb;Uid=user;Pwd=pass;"
         *
         * Pass a @p login_timeout when an unreachable server should be given up on
         * quickly. Drivers set their own default otherwise, and it can be long: the
         * Microsoft SQL Server driver waits 15 seconds. The `Connect Timeout`
         * connection-string keyword does not do this - it is ignored.
         */
        [[nodiscard]]
        THROWS(SQLException)
        static Connection connection(StringView conn_str, Optional<Seconds> login_timeout = nullopt) {
            return Connection(conn_str, login_timeout);
        }

        /**
         * @brief Establishes a connection to the database using DSN.
         * @param dsn The Data Source Name.
         * @param username The database username.
         * @param password The database password.
         * @param login_timeout How long to allow the login to take, or an empty Optional for the driver's default.
         * @return A Connection object.
         * @throws SQLException if the connection fails.
         *
         * Uses SQLConnect directly for proper DSN-based authentication.
         */
        [[nodiscard]]
        THROWS(SQLException)
        static Connection connection(
            StringView dsn,
            StringView username,
            StringView password,
            Optional<Seconds> login_timeout = nullopt
        ) {
            return Connection(dsn, username, password, login_timeout);
        }

        /**
         * @brief Establishes a connection using a driver, server, database, and credentials.
         * @param driver The ODBC driver name (e.g. "SQL Server", "PostgreSQL Unicode",
         * "MySQL ODBC 8.0 Unicode Driver").
         * @param server The server hostname or IP address.
         * @param database The database name.
         * @param username The database username.
         * @param password The database password.
         * @param options The port, login timeout, transport security, and any extra keywords.
         * @return A Connection object.
         * @throws SQLException if the connection fails, or if @p options asks for an
         * encryption mode that @p driver has no known spelling for.
         *
         * Builds an ODBC connection string from the given components. This is the
         * preferred way to connect when you know the driver name installed on the system.
         *
         * The driver name is needed rather than just a host and port because the
         * spelling of both the port and the encryption keywords belongs to the
         * driver, not to ODBC - see @ref server_clause and @ref encryption_clause.
         * Anything this overload cannot express portably goes in
         * @ref ConnectionOptions::extra verbatim.
         *
         * @p database, @p username and @p password are quoted as needed, so a
         * password holding a ';' reaches the driver whole instead of being cut
         * short and having its tail read as further keywords - see
         * @ref escape_value. @p options.extra is *not* quoted: it is keywords by
         * contract, and quoting it would defeat its purpose.
         */
        [[nodiscard]]
        THROWS(SQLException)
        static Connection connection(
            StringView driver,
            StringView server,
            StringView database,
            StringView username,
            StringView password,
            const ConnectionOptions& options = {}
        ) {
            const String conn_str = Ops::fmt(
                "Driver={{{}}};{}Database={};Uid={};Pwd={};{}{}",
                driver,
                server_clause(driver, server, options.port),
                escape_value(database),
                escape_value(username),
                escape_value(password),
                encryption_clause(driver, options.encryption),
                options.extra
            );
            return Connection(conn_str, options.login_timeout);
        }

        /**
         * @brief Establishes a connection to a server at a known address.
         * @param driver The ODBC driver name.
         * @param address The server's IP address.
         * @param database The database name.
         * @param username The database username.
         * @param password The database password.
         * @param options The port, login timeout, transport security, and any extra keywords.
         * @return A Connection object.
         * @throws SQLException if the connection fails, or if @p options asks for an
         * encryption mode that @p driver has no known spelling for.
         *
         * Writes @p address the way drivers read it, which for IPv6 is the bare
         * RFC 5952 literal that IPAddress::to_string already produces. Prefer
         * this to formatting the address yourself: the habitual URI spelling
         * "[::1]:1433" is wrong twice over, in the brackets and in the colon,
         * and a driver reports neither as a syntax error.
         *
         * Connecting by address rather than by name generally rules out
         * Encryption::VERIFIED, because server certificates carry DNS names and
         * seldom an address.
         */
        [[nodiscard]]
        THROWS(SQLException)
        static Connection connection(
            StringView driver,
            const IPAddress& address,
            StringView database,
            StringView username,
            StringView password,
            const ConnectionOptions& options = {}
        ) {
            return connection(driver, address.to_string(), database, username, password, options);
        }
    private:
        /**
         * @internal
         * @brief Whether @p haystack contains @p needle, ignoring ASCII case.
         *
         * @param haystack The text to search.
         * @param needle The text to search for.
         * @return true if @p needle occurs in @p haystack, false otherwise.
         */
        [[nodiscard]]
        static bool contains_ignoring_ascii_case(StringView haystack, StringView needle) noexcept {
            const auto lower = [](char c) noexcept -> char {
                return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
            };
            if (needle.size() > haystack.size()) {
                return false;
            }
            for (usize start = 0; start + needle.size() <= haystack.size(); ++start) {
                usize offset = 0;
                while (offset < needle.size() && lower(haystack[start + offset]) == lower(needle[offset])) {
                    ++offset;
                }
                if (offset == needle.size()) {
                    return true;
                }
            }
            return false;
        }

        /**
         * @internal
         * @brief Whether @p value has to be brace-quoted to survive the connection-string grammar.
         *
         * @param value The value to inspect.
         * @return true if @p value must be quoted, false if it can be written as it stands.
         *
         * The reserved set is the one Microsoft documents for SQLDriverConnect:
         * an attribute value holding any of `[]{}(),;?*=!@` should be enclosed in
         * braces. An empty value, or one padded with blanks, needs the braces
         * for a different reason - to mark where it starts and stops.
         */
        [[nodiscard]]
        static bool needs_bracing(StringView value) noexcept {
            static constexpr StringView RESERVED = "[]{}(),;?*=!@";
            if (value.empty()) {
                return true;
            }
            if (value.front() == ' ' || value.front() == '\t' || value.back() == ' ' || value.back() == '\t') {
                return true;
            }
            return value.find_first_of(RESERVED) != StringView::npos;
        }

        /**
         * @internal
         * @brief @p value written so a driver reads back exactly what was passed in.
         *
         * @param value The password, user name, or database name to write.
         * @return The value, brace-quoted if it needs to be.
         *
         * ';' separates one keyword from the next, so a value carrying one is not
         * merely truncated - everything past it is parsed as further keywords. A
         * password ending in ";Encrypt=no" would negotiate a plaintext
         * connection, which is the failure @ref encryption_clause exists to
         * refuse; ";Trusted_Connection=yes" would change how the caller
         * authenticates. Values that reach here from a configuration file or an
         * environment variable are exactly the ones that must not be able to do
         * that.
         *
         * Inside braces a '}' would otherwise end the value early, so it is
         * doubled - verified against ODBC Driver 18, which reads "{a}}b}" back as
         * "a}b".
         */
        [[nodiscard]]
        static String escape_value(StringView value) {
            if (!needs_bracing(value)) {
                return String(value);
            }
            String quoted;
            quoted.reserve(value.size() + 2);
            quoted += '{';
            for (const char character: value) {
                quoted += character;
                if (character == '}') {
                    quoted += '}';
                }
            }
            quoted += '}';
            return quoted;
        }

        /**
         * @internal
         * @brief The "Server=..." part of a connection string, spelled the way @p driver expects.
         *
         * @param driver The ODBC driver name.
         * @param server The server hostname or IP address.
         * @param port The port, or 0 to leave the driver's default.
         * @return The clause, including its trailing ';'.
         *
         * ODBC itself defines no port keyword - only DSN, DRIVER, UID, PWD,
         * FILEDSN and SAVEFILE are standard, and everything else belongs to the
         * driver. Microsoft's SQL Server drivers take "Server=host,port" and
         * have no Port keyword at all; the PostgreSQL and MySQL drivers take a
         * separate "Port=". Getting this wrong does not fail loudly, because a
         * driver silently drops keywords it does not recognise: a SQL Server
         * connection carrying "Port=9999" connects to 1433 without a word.
         *
         * An IPv6 literal goes in bare - "::1", not the URI form "[::1]".
         * Drivers do not parse the brackets: Driver 18 resolves "[::1]" as a
         * hostname, fails, and reports it as a login timeout after waiting out
         * the whole deadline, so this rejects the form up front instead.
         */
        THROWS(SQLException)
        [[nodiscard]]
        static String server_clause(StringView driver, StringView server, i32 port) {
            // Not brace-quoted like the other values: for SQL Server the comma
            // before the port is part of the grammar of this clause, and quoting
            // would fold it into the host name. A host cannot legitimately hold
            // any of these anyway, so they are refused instead.
            if (server.find_first_of(";{}") != StringView::npos) {
                throw SQLException(Ops::fmt(
                    "the server '{}' contains a character the connection string reserves: "
                    "a host name or address cannot hold ';', '{{' or '}}'",
                    server
                ));
            }
            if (server.starts_with('[')) {
                throw SQLException(Ops::fmt(
                    "bracketed IPv6 literal '{}': drivers read the bare literal, so drop the "
                    "brackets and pass the port as ConnectionOptions::port",
                    server
                ));
            }
            if (port <= 0) {
                return Ops::fmt("Server={};", server);
            }
            if (contains_ignoring_ascii_case(driver, "sql server")) {
                return Ops::fmt("Server={},{};", server, port);
            }
            return Ops::fmt("Server={};Port={};", server, port);
        }

        /**
         * @internal
         * @brief The encryption keywords for @p encryption, spelled the way @p driver expects.
         * @param driver The ODBC driver name.
         * @param encryption The transport security to ask for.
         * @return The keywords, each with its trailing ';', or an empty string for Encryption::DRIVER_DEFAULT.
         * @throws SQLException if @p driver has no spelling known to this library.
         *
         * Unlike the port, a misspelled encryption keyword is a security problem
         * rather than a routing one: the driver drops what it does not recognise
         * and connects in the clear, with nothing in the result to say so. This
         * refuses to guess for that reason - a caller on a driver the library
         * does not know can still spell it out through
         * @ref ConnectionOptions::extra, which is at least honest about who is
         * responsible for getting it right.
         */
        THROWS(SQLException)
        [[nodiscard]]
        static String encryption_clause(StringView driver, Encryption encryption) {
            if (encryption == Encryption::DRIVER_DEFAULT) {
                return ""s;
            }
            if (contains_ignoring_ascii_case(driver, "sql server")) {
                switch (encryption) {
                    case Encryption::DISABLED:
                        return "Encrypt=no;";
                    case Encryption::TRUSTED:
                        return "Encrypt=yes;TrustServerCertificate=yes;";
                    case Encryption::VERIFIED:
                        return "Encrypt=yes;TrustServerCertificate=no;";
                    case Encryption::DRIVER_DEFAULT:
                        break;
                }
            }
            throw SQLException(Ops::fmt(
                "no known encryption spelling for driver '{}': set the keywords through "
                "ConnectionOptions::extra, or build the connection string yourself",
                driver
            ));
        }
    };

    /**
     * @class DataSource
     * @brief A simple connection pool.
     *
     * Maintains a pool of reusable Connection objects. When a connection is
     * requested, a pooled connection is returned if available; otherwise a new
     * one is created (up to the configured maximum). Connections should be
     * returned to the pool via return_connection() instead of being closed.
     *
     * Thread-safe: all public methods are synchronized.
     */
    class DataSource {
    private:
        String _connection_string;
        Queue<Connection> _pool;
        Mutex _mutex;
        ConditionVariable _cv;
        i32 _max_size;
        i32 _active = 0;
    public:
        /**
         * @brief Constructs a DataSource with a connection string and pool size.
         * @param conn_str The ODBC connection string used to create new connections.
         * @param max_size The maximum number of connections in the pool (default: 10).
         */
        explicit DataSource(StringView conn_str, i32 max_size = 10):
            _connection_string{conn_str}, _max_size{max_size}, _active{0} {}

        DataSource(const DataSource&) = DELETE_METHOD("DataSource is not copyable.");
        DataSource& operator=(const DataSource&) = DELETE_METHOD("DataSource is not copyable.");

        /**
         * @brief Gets a connection from the pool.
         * @return A Connection object.
         * @throws SQLException if connection creation fails.
         *
         * If a pooled connection is available, it is returned immediately. If the
         * pool is empty but the active count is below the maximum, a new connection
         * is created. Otherwise, this method blocks until a connection is returned.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Connection connection() {
            UniqueLock<Mutex> lock(_mutex);

            while (_pool.empty() && _active >= _max_size) {
                _cv.wait(lock);
            }

            if (!_pool.empty()) {
                Connection conn = Ops::move(_pool.front());
                _pool.pop();

                if (conn.is_closed()) {
                    --_active;
                    lock.unlock();
                    return connection();
                }

                return conn;
            }

            ++_active;
            lock.unlock();

            try {
                return DriverManager::connection(_connection_string);
            } catch (const SQLException& _) {
                ScopedLock<Mutex> guard(_mutex);
                --_active;
                _cv.notify_one();
                throw;
            }
        }

        /**
         * @brief Returns a connection to the pool for reuse.
         * @param conn The connection to return.
         *
         * If the connection is still open, it is placed back in the pool.
         * If the connection is closed, the active count is decremented.
         */
        void return_connection(Connection conn) {
            ScopedLock<Mutex> lock(_mutex);

            if (conn.is_closed()) {
                --_active;
            } else {
                _pool.push(Ops::move(conn));
            }

            _cv.notify_one();
        }

        /**
         * @brief Gets the number of currently active (checked-out) connections.
         * @return The active connection count.
         */
        [[nodiscard]]
        i32 active_count() const {
            return _active;
        }

        /**
         * @brief Gets the number of idle connections in the pool.
         * @return The idle connection count.
         */
        [[nodiscard]]
        i32 idle_count() const {
            return static_cast<i32>(_pool.size());
        }

        /**
         * @brief Gets the maximum pool size.
         * @return The maximum number of connections allowed.
         */
        [[nodiscard]]
        i32 max_pool_size() const noexcept {
            return _max_size;
        }

        /**
         * @brief Closes all idle connections in the pool.
         */
        void close() {
            ScopedLock<Mutex> lock(_mutex);

            while (!_pool.empty()) {
                _pool.front().close();
                _pool.pop();
                --_active;
            }
        }
    };
}
#endif
