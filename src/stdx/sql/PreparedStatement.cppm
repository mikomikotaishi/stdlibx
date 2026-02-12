/**
 * @file PreparedStatement.cppm
 * @module stdx:sql.PreparedStatement
 * @brief Implementation of the PreparedStatement class.
 *
 * This file contains the implementation of the PreparedStatement class, which is used to
 * execute precompiled SQL statements with parameters.
 */

module;

#include "Macros.hpp"

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:sql.PreparedStatement;

import std;
#else
export module stdlibx:sql.PreparedStatement;

import stdlib;
#endif

import :sql.SQLException;
import :sql.ResultSet;

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
using std::collections::Vector;

namespace util = std::util;
#else
using stdlib::collections::Vector;

namespace util = stdlib::util;
#endif

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::sql {
#else
export namespace stdlibx::sql {
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

/**
 * @class PreparedStatement
 * @brief Represents a precompiled SQL statement with parameter binding.
 *
 * Provides methods to bind parameters and execute parameterized SQL queries.
 */
class PreparedStatement {
private:
    SQLHDBC dbc;
    SQLHSTMT stmt;
    Vector<i32> int_params;
    Vector<i64> long_params;
    Vector<f64> double_params;
    Vector<String> string_params;
    Vector<u8> bool_params;
    Vector<i64> indicators;
    String sql;
    bool closed;

    /**
     * @brief Checks if the statement is closed.
     *
     * @throws SQLException if the statement is closed.
     */
    void check_closed() const throws (SQLException) {
        if (closed) {
            throw SQLException("PreparedStatement is closed");
        }
    }

    /**
     * @brief Constructs a PreparedStatement from an ODBC connection handle.
     * @internal This constructor is private and used by Connection.
     *
     * @param dbc The ODBC connection handle.
     * @param sql The SQL statement with parameter placeholders (?).
     * @throws SQLException if statement preparation fails.
     */
    PreparedStatement(SQLHDBC dbc, StringView sql) throws (SQLException):
        dbc{dbc}, stmt{nullptr}, int_params{}, long_params{}, double_params{}, 
        string_params{}, bool_params{}, indicators{}, sql{sql}, closed{false} {
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to allocate statement handle");
        }

        ret = SQLPrepare(
            this->stmt, 
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(this->sql.c_str())),
            SQL_NTS
        );

        if (!SQL_SUCCEEDED(ret)) {
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
            throw SQLException("Failed to prepare statement");
        }

        SQLSMALLINT param_count;
        SQLNumParams(this->stmt, &param_count);

        int_params.resize(param_count);
        long_params.resize(param_count);
        double_params.resize(param_count);
        string_params.resize(param_count);
        bool_params.resize(param_count);
        indicators.resize(param_count);
    }

    friend class Connection;

public:
    /**
     * @brief Move constructor.
     */
    PreparedStatement(PreparedStatement&& other) noexcept:
        dbc{other.dbc}, stmt{other.stmt}, 
        int_params{util::move(other.int_params)}, long_params{util::move(other.long_params)},
        double_params{util::move(other.double_params)}, string_params{util::move(other.string_params)},
        bool_params{util::move(other.bool_params)}, indicators{util::move(other.indicators)},
        sql{util::move(other.sql)}, closed{other.closed} {
        other.stmt = nullptr;
        other.closed = true;
    }

    /**
     * @brief Move assignment operator.
     */
    PreparedStatement& operator=(PreparedStatement&& other) noexcept {
        if (this != &other) {
            close();
            dbc = other.dbc;
            stmt = other.stmt;
            int_params = util::move(other.int_params);
            long_params = util::move(other.long_params);
            double_params = util::move(other.double_params);
            string_params = util::move(other.string_params);
            bool_params = util::move(other.bool_params);
            indicators = util::move(other.indicators);
            sql = util::move(other.sql);
            closed = other.closed;
            other.stmt = nullptr;
            other.closed = true;
        }
        return *this;
    }

    PreparedStatement(const PreparedStatement&) = delete;
    PreparedStatement& operator=(const PreparedStatement&) = delete;

    /**
     * @brief Destructor.
     */
    ~PreparedStatement() {
        close();
    }

    /**
     * @brief Binds an integer parameter.
     *
     * @param parameter_index The parameter index (1-based).
     * @param value The integer value to bind.
     * @throws SQLException on error.
     */
    void set_int(i32 parameter_index, i32 value) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(int_params.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        int_params[parameter_index - 1] = value;
        indicators[parameter_index - 1] = 0;
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
            0, 0, &int_params[parameter_index - 1],
            0, reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind integer parameter");
        }
    }

    /**
     * @brief Binds a long integer parameter.
     *
     * @param parameter_index The parameter index (1-based).
     * @param value The long integer value to bind.
     * @throws SQLException on error.
     */
    void set_long(i32 parameter_index, i64 value) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(long_params.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        long_params[parameter_index - 1] = value;
        indicators[parameter_index - 1] = 0;
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
            0, 0, &long_params[parameter_index - 1],
            0, reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind long parameter");
        }
    }

    /**
     * @brief Binds a double parameter.
     *
     * @param parameter_index The parameter index (1-based).
     * @param value The double value to bind.
     * @throws SQLException on error.
     */
    void set_double(i32 parameter_index, f64 value) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(double_params.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        double_params[parameter_index - 1] = value;
        indicators[parameter_index - 1] = 0;
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
            0, 0, &double_params[parameter_index - 1],
            0, reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind double parameter");
        }
    }

    /**
     * @brief Binds a string parameter.
     *
     * @param parameter_index The parameter index (1-based).
     * @param value The string value to bind.
     * @throws SQLException on error.
     */
    void set_string(i32 parameter_index, StringView value) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(string_params.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        string_params[parameter_index - 1] = String(value);
        indicators[parameter_index - 1] = static_cast<i64>(string_params[parameter_index - 1].size());
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
            string_params[parameter_index - 1].size(), 0, const_cast<char*>(string_params[parameter_index - 1].c_str()),
            string_params[parameter_index - 1].size(), reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind string parameter");
        }
    }

    /**
     * @brief Binds a boolean parameter.
     *
     * @param parameter_index The parameter index (1-based).
     * @param value The boolean value to bind.
     * @throws SQLException on error.
     */
    void set_boolean(i32 parameter_index, bool value) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(bool_params.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        bool_params[parameter_index - 1] = value ? 1 : 0;
        indicators[parameter_index - 1] = 0;
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, 
            &bool_params[parameter_index - 1], 0, reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind boolean parameter");
        }
    }

    /**
     * @brief Sets a parameter to NULL.
     *
     * @param parameter_index The parameter index (1-based).
     * @throws SQLException on error.
     */
    void set_null(i32 parameter_index) throws (SQLException) {
        check_closed();
        
        if (parameter_index < 1 || parameter_index > static_cast<i32>(indicators.size())) {
            throw SQLException("Invalid parameter index");
        }
        
        indicators[parameter_index - 1] = SQL_NULL_DATA;
        
        SQLRETURN ret = SQLBindParameter(
            stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
            0, 0, nullptr, 0, reinterpret_cast<SQLLEN*>(&indicators[parameter_index - 1])
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to bind NULL parameter");
        }
    }

    /**
     * @brief Executes the prepared query and returns a result set.
     *
     * @return A ResultSet containing the query results.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    ResultSet execute_query() throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLExecute(stmt);
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to execute prepared query");
        }
        
        return ResultSet(stmt);
    }

    /**
     * @brief Executes the prepared update/insert/delete statement.
     *
     * @return The number of rows affected.
     * @throws SQLException on error.
     */
    i32 execute_update() throws (SQLException) {
        check_closed();
        
        SQLRETURN ret = SQLExecute(stmt);
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to execute prepared update");
        }
        
        SQLLEN row_count;
        ret = SQLRowCount(stmt, &row_count);
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get row count");
        }
        
        return static_cast<i32>(row_count);
    }

    /**
     * @brief Clears all parameter bindings.
     */
    void clear_parameters() {
        check_closed();
        SQLFreeStmt(stmt, SQL_RESET_PARAMS);
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
