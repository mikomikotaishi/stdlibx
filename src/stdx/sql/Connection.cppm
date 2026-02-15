/**
 * @file Connection.cppm
 * @module stdx:sql.Connection
 * @brief Implementation of the Connection class.
 *
 * This file contains the implementation of the Connection class, which represents
 * a database connection using ODBC.
 */

module;

#include "Macros.hpp"

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

export module stdx:sql.Connection;

import :mem;
import :sql.SQLException;
import :sql.Statement;
import :sql.PreparedStatement;

using stdx::mem::UniquePointer;

using namespace stdx::core;

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
export namespace stdx::sql {

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

/**
 * @class Connection
 * @brief Represents a database connection.
 *
 * Provides methods to create statements, manage transactions, and interact with
 * a database.
 */
class Connection {
private:
    SQLHENV env;
    SQLHDBC dbc;
    bool closed;
    bool auto_commit;

    /**
     * @brief Checks if the connection is closed.
     *
     * @throws SQLException if the connection is closed.
     */
    void check_closed() const throws (SQLException) {
        if (closed) {
            throw SQLException("Connection is closed");
        }
    }

    /**
     * @brief Constructs a Connection using a connection string.
     * @internal This constructor can be private or public depending on desired API style.
     *
     * @param conn_str The ODBC connection string.
     * @throws SQLException if connection fails.
     */
    explicit Connection(StringView conn_str) throws (SQLException):
        env{nullptr}, dbc{nullptr}, closed{false}, auto_commit{true} {
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to allocate environment handle");
        }

        ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);
        if (!SQL_SUCCEEDED(ret)) {
            SQLFreeHandle(SQL_HANDLE_ENV, env);
            throw SQLException("Failed to set ODBC version");
        }

        ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
        if (!SQL_SUCCEEDED(ret)) {
            SQLFreeHandle(SQL_HANDLE_ENV, env);
            throw SQLException("Failed to allocate connection handle");
        }

        SQLCHAR out_conn_str[1024];
        SQLSMALLINT out_conn_str_len;
        
        ret = SQLDriverConnect(
            dbc, 
            nullptr,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(conn_str.data())),
            SQL_NTS,
            out_conn_str, sizeof(out_conn_str),
            &out_conn_str_len,
            SQL_DRIVER_NOPROMPT
        );

        if (!SQL_SUCCEEDED(ret)) {
            SQLFreeHandle(SQL_HANDLE_DBC, dbc);
            SQLFreeHandle(SQL_HANDLE_ENV, env);
            throw SQLException("Failed to connect to database");
        }
    }

    friend class DriverManager;

public:
    /**
     * @brief Move constructor.
     */
    Connection(Connection&& other) noexcept:
        env{other.env}, dbc{other.dbc}, closed{other.closed}, auto_commit{other.auto_commit} {
        other.env = nullptr;
        other.dbc = nullptr;
        other.closed = true;
    }

    /**
     * @brief Move assignment operator.
     */
    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            close();
            env = other.env;
            dbc = other.dbc;
            closed = other.closed;
            auto_commit = other.auto_commit;
            other.env = nullptr;
            other.dbc = nullptr;
            other.closed = true;
        }
        return *this;
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    /**
     * @brief Destructor.
     */
    ~Connection() {
        close();
    }

    /**
     * @brief Creates a Statement object for executing SQL queries.
     *
     * @return A Statement object.
     * @throws SQLException if statement creation fails.
     */
    [[nodiscard]]
    Statement create_statement() throws (SQLException) {
        check_closed();
        return Statement(dbc);
    }

    /**
     * @brief Creates a PreparedStatement object for executing parameterized queries.
     *
     * @param sql The SQL statement with parameter placeholders (?).
     * @return A PreparedStatement object.
     * @throws SQLException if statement preparation fails.
     */
    [[nodiscard]]
    PreparedStatement prepare_statement(StringView sql) throws (SQLException) {
        check_closed();
        return PreparedStatement(dbc, sql);
    }

    /**
     * @brief Commits the current transaction.
     *
     * @throws SQLException on error.
     */
    void commit() {
        check_closed();
        
        SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to commit transaction");
        }
    }

    /**
     * @brief Rolls back the current transaction.
     *
     * @throws SQLException on error.
     */
    void rollback() throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_ROLLBACK);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to rollback transaction");
        }
    }

    /**
     * @brief Sets the auto-commit mode.
     *
     * @param auto_commit_mode true to enable auto-commit, false to disable.
     * @throws SQLException on error.
     */
    void set_auto_commit(bool auto_commit_mode) throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLSetConnectAttr(
            dbc,
            SQL_ATTR_AUTOCOMMIT,
            reinterpret_cast<SQLPOINTER>(auto_commit_mode ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF),
            0
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to set auto-commit mode");
        }
        
        auto_commit = auto_commit_mode;
    }

    /**
     * @brief Gets the auto-commit mode.
     *
     * @return true if auto-commit is enabled, false otherwise.
     */
    [[nodiscard]]
    bool auto_commit_enabled() const noexcept {
        return auto_commit;
    }

    /**
     * @brief Closes the connection.
     */
    void close() noexcept {
        if (!closed) {
            if (dbc != nullptr) {
                SQLDisconnect(dbc);
                SQLFreeHandle(SQL_HANDLE_DBC, dbc);
                dbc = nullptr;
            }
            if (env != nullptr) {
                SQLFreeHandle(SQL_HANDLE_ENV, env);
                env = nullptr;
            }
            closed = true;
        }
    }

    /**
     * @brief Checks if the connection is closed.
     *
     * @return true if closed, false otherwise.
     */
    [[nodiscard]]
    bool is_closed() const noexcept {
        return closed;
    }
};

#endif

}
