#pragma once

using stdx::collections::Vector;
using stdx::time::Seconds;

#ifdef __cpp_impl_reflection
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::ReflectableAsClass;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
    /**
     * @class Transaction
     * @brief RAII guard for database transactions.
     *
     * Automatically rolls back on destruction if not explicitly committed.
     * Disables auto-commit on construction and restores it on destruction.
     */
    class Transaction;

    /**
     * @class Connection
     * @brief Represents a database connection.
     *
     * Provides methods to create statements, manage transactions, and interact with
     * a database.
     */
    class Connection {
    private:
        SQLHENV _env;
        SQLHDBC _dbc;
        bool _closed;
        bool _auto_commit;

        /**
         * @brief Checks if the connection is closed.
         * @throws SQLException if the connection is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("Connection is closed");
            }
        }

        /**
         * @internal
         * @brief Bounds how long a login may take, on a not-yet-connected handle.
         * @param dbc The connection handle to configure.
         * @param login_timeout How long to allow, or an empty Optional to leave the driver's default.
         * @return The result of the attribute call, or SQL_SUCCESS if no timeout was asked for.
         *
         * ODBC only honours a login deadline as a pre-connect handle attribute. The
         * `Connect Timeout` connection-string keyword is silently ignored by at
         * least the Microsoft driver, which otherwise spends a fixed 15 seconds
         * failing to reach a server that is not there - whether the port refuses
         * the connection or the host is unroutable.
         *
         * A zero timeout means "wait indefinitely" in ODBC, so it is a real value
         * and not the same as leaving the attribute untouched.
         */
        static SQLRETURN set_login_timeout(SQLHDBC dbc, Optional<Seconds> login_timeout) noexcept {
            if (!login_timeout.has_value()) {
                return SQL_SUCCESS;
            }
            return SQLSetConnectAttr(
                dbc,
                SQL_ATTR_LOGIN_TIMEOUT,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLULEN>(login_timeout->count())),
                0
            );
        }

        /**
         * @internal
         * @brief Constructs a Connection using a connection string.
         * @param conn_str The ODBC connection string.
         * @param login_timeout How long to allow the login to take, or an empty Optional for the driver's default.
         * @throws SQLException if connection fails.
         *
         * This constructor is private and used by DriverManager.
         */
        THROWS(SQLException)
        explicit Connection(StringView conn_str, Optional<Seconds> login_timeout = nullopt):
            _env{nullptr}, _dbc{nullptr}, _closed{false}, _auto_commit{true} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_env);
            if (!SQL_SUCCEEDED(ret)) {
                throw SQLException("Failed to allocate environment handle");
            }

            ret = SQLSetEnvAttr(_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_ENV, _env, "Failed to set ODBC version");
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }

            ret = SQLAllocHandle(SQL_HANDLE_DBC, _env, &_dbc);
            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_ENV, _env, "Failed to allocate connection handle");
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }

            // Best effort: a driver that does not implement the attribute (HYC00)
            // still connects, it just keeps its own deadline.
            static_cast<void>(set_login_timeout(_dbc, login_timeout));

            SQLCHAR out_conn_str[1024];
            SQLSMALLINT out_conn_str_len;

            String conn_string(conn_str);
            ret = SQLDriverConnect(
                _dbc,
                nullptr,
                reinterpret_cast<SQLCHAR*>(conn_string.data()),
                static_cast<SQLSMALLINT>(conn_string.size()),
                out_conn_str, sizeof(out_conn_str),
                &out_conn_str_len,
                SQL_DRIVER_NOPROMPT
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_DBC, _dbc, "Failed to connect to database");
                SQLFreeHandle(SQL_HANDLE_DBC, _dbc);
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }
        }

        /**
         * @internal
         * @brief Constructs a Connection using DSN, username and password directly.
         * @param dsn The Data Source Name.
         * @param username The database username.
         * @param password The database password.
         * @param login_timeout How long to allow the login to take, or an empty Optional for the driver's default.
         * @throws SQLException if connection fails.
         *
         * This constructor is private and used by DriverManager.
         */
        THROWS(SQLException)
        Connection(
            StringView dsn,
            StringView username,
            StringView password,
            Optional<Seconds> login_timeout = nullopt
        ):
            _env{nullptr}, _dbc{nullptr}, _closed{false}, _auto_commit{true} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_env);
            if (!SQL_SUCCEEDED(ret)) {
                throw SQLException("Failed to allocate environment handle");
            }

            ret = SQLSetEnvAttr(_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_ENV, _env, "Failed to set ODBC version");
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }

            ret = SQLAllocHandle(SQL_HANDLE_DBC, _env, &_dbc);
            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_ENV, _env, "Failed to allocate connection handle");
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }

            // Best effort: a driver that does not implement the attribute (HYC00)
            // still connects, it just keeps its own deadline.
            static_cast<void>(set_login_timeout(_dbc, login_timeout));

            String dsn_str(dsn);
            String user_str(username);
            String pass_str(password);

            ret = SQLConnect(
                _dbc,
                reinterpret_cast<SQLCHAR*>(dsn_str.data()),
                static_cast<SQLSMALLINT>(dsn_str.size()),
                reinterpret_cast<SQLCHAR*>(user_str.data()),
                static_cast<SQLSMALLINT>(user_str.size()),
                reinterpret_cast<SQLCHAR*>(pass_str.data()),
                static_cast<SQLSMALLINT>(pass_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_DBC, _dbc, "Failed to connect to database");
                SQLFreeHandle(SQL_HANDLE_DBC, _dbc);
                SQLFreeHandle(SQL_HANDLE_ENV, _env);
                throw ex;
            }
        }

        friend class DriverManager;
    public:
        /**
         * @brief Move constructor.
         */
        Connection(Connection&& other) noexcept:
            _env{other._env}, _dbc{other._dbc}, _closed{other._closed}, _auto_commit{other._auto_commit} {
            other._env = nullptr;
            other._dbc = nullptr;
            other._closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        Connection& operator=(Connection&& other) noexcept {
            if (this != &other) {
                close();
                _env = other._env;
                _dbc = other._dbc;
                _closed = other._closed;
                _auto_commit = other._auto_commit;
                other._env = nullptr;
                other._dbc = nullptr;
                other._closed = true;
            }
            return *this;
        }

        Connection(const Connection&) = DELETE_METHOD("Connection is not copyable.");
        Connection& operator=(const Connection&) = DELETE_METHOD("Connection is not copyable.");

        /**
         * @brief Destructor.
         */
        ~Connection() {
            close();
        }

        /**
         * @brief Creates a Statement object for executing SQL queries.
         * @return A Statement object.
         * @throws SQLException if statement creation fails.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Statement create_statement() {
            check_closed();
            return Statement(_dbc);
        }

        /**
         * @brief Creates a PreparedStatement object for executing parameterized queries.
         * @param sql The SQL statement with parameter placeholders (?).
         * @return A PreparedStatement object.
         * @throws SQLException if statement preparation fails.
         */
        [[nodiscard]]
        THROWS(SQLException)
        PreparedStatement prepare_statement(StringView sql) {
            check_closed();
            return PreparedStatement(_dbc, sql);
        }

        /**
         * @brief Executes a query in one call and returns its result set.
         * @tparam Args The deduced bound-parameter types.
         * @param sql The SQL query, with a `?` marker per argument.
         * @param args One value per marker, in order.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         *
         * The common path with no statement object to manage. Any bound arguments
         * are sent as parameters (injection-safe); with no arguments the SQL is
         * executed directly. The returned ResultSet owns its statement handle, so
         * it remains valid after this call.
         */
        template <Bindable... Args>
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet query(StringView sql, Args&&... args) {
            check_closed();
            if constexpr (sizeof...(Args) == 0) {
                Statement statement = create_statement();
                ResultSet result = statement.execute_query(sql);
                result.assume_ownership();
                statement.release_handle();
                return result;
            } else {
                PreparedStatement statement = prepare_statement(sql);
                ResultSet result = statement.execute_query(Ops::forward<Args>(args)...);
                result.assume_ownership();
                statement.release_handle();
                return result;
            }
        }

        #ifdef __cpp_impl_reflection
        /**
         * @brief Executes a query and maps each row to an instance of T.
         * @tparam T The row type to map each result row onto.
         * @tparam Args The deduced bound-parameter types.
         * @param sql The SQL query, with a `?` marker per argument.
         * @param args One value per marker, in order.
         * @return A vector of mapped rows.
         * @throws SQLException on error.
         *
         * T must be a default-constructible reflectable class. Its data members are
         * filled, in declaration order, from the result columns by position (the
         * Nth member from the Nth column). Each member type must be column-readable
         * (see ResultSet::get); a SQL NULL column leaves the member at its
         * default-constructed value.
         *
         * @note Requires reflection support (__cpp_lib_reflection).
         */
        template <ReflectableAsClass T, Bindable... Args>
        [[nodiscard]]
        [[=Throws<SQLException>]]
        Vector<T> query(StringView sql, Args&&... args) {
            static_assert(
                Ops::class_of<T>().is_default_constructible(),
                "Connection::query<T> requires T to be default-constructible"
            );
            constexpr AccessContext ctx = AccessContext::unchecked();

            ResultSet rs = query(sql, Ops::forward<Args>(args)...);

            Vector<T> rows;
            while (rs.next()) {
                T row{};
                i32 column = 1;
                template for (constexpr Field field: Ops::define_static_array(Ops::class_of<T>().fields(ctx))) {
                    using MemberType = typename [:field.type().value():];
                    if (Optional<MemberType> value = rs.get<MemberType>(column); value.has_value()) {
                        row.[:field.value():] = *value;
                    }
                    ++column;
                }
                rows.push_back(Ops::move(row));
            }
            return rows;
        }
        #endif

        /**
         * @brief Executes an update/insert/delete (or DDL) in one call.
         * @tparam Args The deduced bound-parameter types.
         * @param sql The SQL statement, with a `?` marker per argument.
         * @param args One value per marker, in order.
         * @return The number of rows affected.
         * @throws SQLException on error.
         *
         * Any bound arguments are sent as parameters (injection-safe); with no
         * arguments the SQL is executed directly, which suits DDL such as CREATE or
         * DROP. The return value (rows affected) may be ignored.
         */
        template <Bindable... Args>
        THROWS(SQLException)
        i32 execute(StringView sql, Args&&... args) {
            check_closed();
            if constexpr (sizeof...(Args) == 0) {
                Statement statement = create_statement();
                return statement.execute_update(sql);
            } else {
                PreparedStatement statement = prepare_statement(sql);
                return statement.execute_update(Ops::forward<Args>(args)...);
            }
        }

        /**
         * @brief Creates a CallableStatement for executing stored procedures.
         * @param call The ODBC call syntax string.
         * @return A CallableStatement object.
         * @throws SQLException if statement preparation fails.
         *
         * Uses the ODBC call escape syntax. Example:
         * @code
         * CallableStatement cs = conn.prepare_call("{call my_procedure(?, ?)}");
         * @endcode
         */
        [[nodiscard]]
        THROWS(SQLException)
        CallableStatement prepare_call(StringView call) {
            check_closed();
            return CallableStatement(_dbc, call);
        }

        /**
         * @brief Gets metadata about the database.
         * @return A DatabaseMetaData object for querying database properties.
         * @throws SQLException if the connection is closed.
         */
        [[nodiscard]]
        THROWS(SQLException)
        DatabaseMetaData metadata() {
            check_closed();
            return DatabaseMetaData(_dbc);
        }

        /**
         * @brief Begins a new transaction and returns an RAII guard.
         * @return A Transaction RAII guard.
         * @throws SQLException on error.
         *
         * Disables auto-commit for the duration of the transaction. The returned
         * Transaction object will automatically roll back on destruction if
         * commit() has not been called.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Transaction begin_transaction();

        /**
         * @brief Commits the current transaction.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void commit() {
            check_closed();

            SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, _dbc, SQL_COMMIT);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to commit transaction");
            }
        }

        /**
         * @brief Rolls back the current transaction.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void rollback() {
            check_closed();

            SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, _dbc, SQL_ROLLBACK);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to rollback transaction");
            }
        }

        /**
         * @brief Sets the auto-commit mode.
         * @param auto_commit_mode true to enable auto-commit, false to disable.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_auto_commit(bool auto_commit_mode) {
            check_closed();

            SQLRETURN ret = SQLSetConnectAttr(
                _dbc,
                SQL_ATTR_AUTOCOMMIT,
                reinterpret_cast<SQLPOINTER>(auto_commit_mode ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF),
                0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to set auto-commit mode");
            }

            _auto_commit = auto_commit_mode;
        }

        /**
         * @brief Gets the auto-commit mode.
         * @return true if auto-commit is enabled, false otherwise.
         */
        [[nodiscard]]
        bool auto_commit_enabled() const noexcept {
            return _auto_commit;
        }

        /**
         * @brief Closes the connection.
         */
        void close() noexcept {
            if (!_closed) {
                if (_dbc != nullptr) {
                    SQLDisconnect(_dbc);
                    SQLFreeHandle(SQL_HANDLE_DBC, _dbc);
                    _dbc = nullptr;
                }
                if (_env != nullptr) {
                    SQLFreeHandle(SQL_HANDLE_ENV, _env);
                    _env = nullptr;
                }
                _closed = true;
            }
        }

        /**
         * @brief Checks if the connection is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
        }
    };

    /**
     * @class Transaction
     * @brief RAII guard for database transactions.
     *
     * Automatically rolls back on destruction if not explicitly committed.
     * Disables auto-commit on construction and restores it on destruction.
     */
    class Transaction {
    private:
        Connection& _conn;
        bool _committed;
        bool _prev_auto_commit;

        THROWS(SQLException)
        explicit Transaction(Connection& conn):
            _conn{conn}, _committed{false}, _prev_auto_commit{conn.auto_commit_enabled()} {
            conn.set_auto_commit(false);
        }

        friend class Connection;
    public:
        Transaction(const Transaction&) = DELETE_METHOD("Transaction is not copyable.");
        Transaction& operator=(const Transaction&) = DELETE_METHOD("Transaction is not copyable.");
        Transaction& operator=(Transaction&&) = DELETE_METHOD("Transaction is not movable.");

        /**
         * @brief Move constructor.
         */
        Transaction(Transaction&& other) noexcept:
            _conn{other._conn}, _committed{other._committed},
            _prev_auto_commit{other._prev_auto_commit} {
            other._committed = true;
        }

        /**
         * @brief Destructor. Rolls back if not committed, then restores auto-commit.
         */
        ~Transaction() {
            if (!_committed) {
                try {
                    _conn.rollback();
                } catch (const SQLException& _) {
                    // do nothing
                }
            }
            try {
                _conn.set_auto_commit(_prev_auto_commit);
            } catch (const SQLException& _) {
                // do nothing
            }
        }

        /**
         * @brief Commits the transaction.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void commit() {
            _conn.commit();
            _committed = true;
        }

        /**
         * @brief Explicitly rolls back the transaction.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void rollback() {
            _conn.rollback();
            _committed = true;
        }
    };

    THROWS(SQLException)
    Transaction Connection::begin_transaction() {
        check_closed();
        return Transaction(*this);
    }
}
#endif
