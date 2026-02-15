/**
 * @file Statement.cppm
 * @module stdx:sql.Statement
 * @brief Implementation of the Statement class.
 *
 * This file contains the implementation of the Statement class, which is used to
 * execute SQL statements and retrieve results.
 */

module;

#include "Macros.hpp"

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

export module stdx:sql.Statement;

import :mem;
import :sql.SQLException;
import :sql.ResultSet;

using stdx::mem::UniquePointer;

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
export namespace stdx::sql {

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

/**
 * @class Statement
 * @brief Represents a SQL statement to be executed.
 *
 * Provides methods to execute SQL queries and updates.
 */
class Statement {
private:
    SQLHDBC dbc;
    SQLHSTMT stmt;
    bool closed;

    /**
     * @brief Checks if the statement is closed.
     *
     * @throw SQLException if the statement is closed.
     */
    void check_closed() const {
        if (closed) {
            throw SQLException("Statement is closed");
        }
    }

    /**
     * @brief Constructs a Statement from an ODBC connection handle.
     * @internal This constructor is private and used by Connection.
     *
     * @param dbc The ODBC connection handle.
     * @throw SQLException if statement allocation fails.
     */
    explicit Statement(SQLHDBC dbc) throws (SQLException):
        dbc{dbc}, stmt{nullptr}, closed{false} {
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, this->dbc, &this->stmt);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to allocate statement handle");
        }
    }

    friend class Connection;

public:
    /**
     * @brief Move constructor.
     */
    Statement(Statement&& other) noexcept:
        dbc{other.dbc}, stmt{other.stmt}, closed{other.closed} {
        other.stmt = nullptr;
        other.closed = true;
    }

    /**
     * @brief Move assignment operator.
     */
    Statement& operator=(Statement&& other) noexcept {
        if (this != &other) {
            close();
            dbc = other.dbc;
            stmt = other.stmt;
            closed = other.closed;
            other.stmt = nullptr;
            other.closed = true;
        }
        return *this;
    }

    // Disable copy operations
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    /**
     * @brief Destructor.
     */
    ~Statement() {
        close();
    }

    /**
     * @brief Executes a SQL query and returns a result set.
     *
     * @param sql The SQL query to execute.
     * @return A ResultSet containing the query results.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    ResultSet execute_query(StringView sql) throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLExecDirect(
            stmt, 
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.data())),
            SQL_NTS
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to execute query");
        }
        
        return ResultSet(stmt);
    }

    /**
     * @brief Executes a SQL update/insert/delete statement.
     *
     * @param sql The SQL statement to execute.
     * @return The number of rows affected.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    i32 execute_update(StringView sql) throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLExecDirect(
            stmt,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.data())),
            SQL_NTS
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to execute update");
        }
        
        SQLLEN row_count;
        ret = SQLRowCount(stmt, &row_count);
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get row count");
        }
        
        return static_cast<i32>(row_count);
    }

    /**
     * @brief Executes any SQL statement.
     *
     * @param sql The SQL statement to execute.
     * @return true if the result is a ResultSet, false otherwise.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    bool execute(StringView sql) throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLExecDirect(
            stmt,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.data())),
            SQL_NTS
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to execute statement");
        }
        
        SQLSMALLINT column_count;
        ret = SQLNumResultCols(stmt, &column_count);
        
        return SQL_SUCCEEDED(ret) && column_count > 0;
    }

    /**
     * @brief Gets the result set from the last execute() call.
     *
     * @return A ResultSet containing the query results.
     * @throws SQLException if no result set is available.
     */
    [[nodiscard]]
    ResultSet get_result_set() throws (SQLException) {
        check_closed();
        return ResultSet(stmt);
    }

    /**
     * @brief Gets the update count from the last execute_update() call.
     *
     * @return The number of rows affected.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    i32 update_count() throws (SQLException) {
        check_closed();
        
        SQLLEN row_count;
        SQLRETURN ret = SQLRowCount(stmt, &row_count);
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get row count");
        }
        
        return static_cast<i32>(row_count);
    }

    /**
     * @brief Closes the statement.
     */
    void close() noexcept {
        if (!closed && stmt != nullptr) {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            stmt = nullptr;
            closed = true;
        }
    }

    /**
     * @brief Checks if the statement is closed.
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
