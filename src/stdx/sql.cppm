/**
 * @file sql.cppm
 * @module stdx:sql
 * @brief Implementation of the SQL library.
 *
 * This file contains the implementation of the SQL library, which wraps
 * ODBC in a JDBC-like interface for C++.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

#include "Macros.hpp"

export module stdx:sql;

import :main;

using stdx::collections::Vector;
using stdx::mem::UniquePointer;

using namespace stdx::core;

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
export namespace stdx::sql {

/**
 * @class SQLException
 * @brief Exception class for SQL operations.
 *
 * Represents errors that occur during SQL database operations, including
 * SQLSTATE codes and native error codes from the database.
 */
class SQLException : public Exception {
private:
    String msg;
    String state;
    Optional<i32> code;
public:
    /**
     * @brief Constructs a SQLException with a message.
     *
     * @param message The error message.
     */
    explicit SQLException(StringView message):
        Exception(), msg{message}, state{"HY000"}, code{} {}

    /**
     * @brief Constructs a SQLException with a message and SQLSTATE.
     *
     * @param message The error message.
     * @param state The SQLSTATE code.
     */
    SQLException(StringView message, StringView state):
        Exception(), msg{message}, state{state}, code{} {}

    /**
     * @brief Constructs a SQLException with a message, SQLSTATE, and error code.
     *
     * @param message The error message.
     * @param state The SQLSTATE code.
     * @param code The native database error code.
     */
    SQLException(StringView message, StringView state, i32 code):
        Exception(), msg{message}, state{state}, code{code} {}

    /**
     * @brief Gets the error message.
     *
     * @return The error message as a C-string.
     */
    [[nodiscard]]
    const char* what() const noexcept override {
        return msg.c_str();
    }

    /**
     * @brief Gets the SQLSTATE code.
     *
     * @return The SQLSTATE code.
     */
    [[nodiscard]]
    const String& sql_state() const noexcept {
        return state;
    }

    /**
     * @brief Gets the native error code.
     *
     * @return Optional containing the error code if available.
     */
    [[nodiscard]]
    Optional<i32> error_code() const noexcept {
        return code;
    }
};

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

/**
 * @class ResultSet
 * @brief Represents a result set from a SQL query.
 *
 * Provides methods to navigate through query results and retrieve column values.
 */
class ResultSet {
private:
    SQLHSTMT stmt;
    SQLLEN last_indicator;
    i16 column_count;
    bool closed;

    /**
     * @brief Checks if the result set is closed.
     *
     * @throws SQLException if the result set is closed.
     */
    void check_closed() const throws (SQLException) {
        if (closed) {
            throw SQLException("ResultSet is closed");
        }
    }

    /**
     * @brief Constructs a ResultSet from an ODBC statement handle.
     * @internal This constructor is private and used by Statement and PreparedStatement.
     *
     * @param stmt The ODBC statement handle.
     * @throws SQLException if retrieving column count fails.
     */
    explicit ResultSet(SQLHSTMT stmt) throws (SQLException):
        stmt{stmt}, last_indicator{0}, column_count{0}, closed{false} {
        SQLSMALLINT col_count;
        SQLRETURN ret = SQLNumResultCols(this->stmt, &col_count);
        column_count = static_cast<i16>(col_count);
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get column count");
        }
    }

    friend class Statement;
    friend class PreparedStatement;

public:
    /**
     * @brief Move constructor.
     */
    ResultSet(ResultSet&& other) noexcept:
        stmt{other.stmt}, last_indicator{other.last_indicator}, 
        column_count{other.column_count}, closed{other.closed} {
        other.stmt = nullptr;
        other.closed = true;
    }

    /**
     * @brief Move assignment operator.
     */
    ResultSet& operator=(ResultSet&& other) noexcept {
        if (this != &other) {
            close();
            stmt = other.stmt;
            last_indicator = other.last_indicator;
            closed = other.closed;
            column_count = other.column_count;
            other.stmt = nullptr;
            other.closed = true;
        }
        return *this;
    }

    ResultSet(const ResultSet&) = delete;
    ResultSet& operator=(const ResultSet&) = delete;

    /**
     * @brief Destructor.
     */
    ~ResultSet() {
        close();
    }

    /**
     * @brief Moves the cursor to the next row.
     *
     * @return true if the next row is valid, false if no more rows.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    bool next() throws (SQLException) {
        check_closed();
        SQLRETURN ret = SQLFetch(stmt);
        if (ret == SQL_NO_DATA) {
            return false;
        }
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to fetch next row");
        }
        return true;
    }

    /**
     * @brief Gets a string value from the specified column.
     *
     * @param column_index The column index (1-based).
     * @return Optional containing the string value, or empty if NULL.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    Optional<String> get_string(i32 column_index) throws (SQLException) {
        check_closed();

        SQLLEN indicator;
        char buffer[4096];

        SQLRETURN ret = SQLGetData(
            stmt, column_index, SQL_C_CHAR, 
            buffer, sizeof(buffer), &indicator
        );

        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get string data");
        }
        
        last_indicator = indicator;
        
        if (indicator == SQL_NULL_DATA) {
            return {};
        }
        
        return String(buffer);
    }

    /**
     * @brief Gets an integer value from the specified column.
     *
     * @param column_index The column index (1-based).
     * @return Optional containing the integer value, or empty if NULL.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    Optional<i32> get_int(i32 column_index) throws (SQLException) {
        check_closed();
        
        SQLINTEGER value;
        SQLLEN indicator;
        
        SQLRETURN ret = SQLGetData(
            stmt, column_index, SQL_C_SLONG,
            &value, sizeof(value), &indicator
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get integer data");
        }
        
        last_indicator = indicator;
        
        if (indicator == SQL_NULL_DATA) {
            return {};
        }
        
        return static_cast<i32>(value);
    }

    /**
     * @brief Gets a long value from the specified column.
     *
     * @param column_index The column index (1-based).
     * @return Optional containing the long value, or empty if NULL.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    Optional<i64> get_long(i32 column_index) throws (SQLException) {
        check_closed();
        
        SQLBIGINT value;
        SQLLEN indicator;
        
        SQLRETURN ret = SQLGetData(
            stmt, column_index, SQL_C_SBIGINT,
            &value, sizeof(value), &indicator
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get long data");
        }
        
        last_indicator = indicator;
        
        if (indicator == SQL_NULL_DATA) {
            return {};
        }
        
        return static_cast<i64>(value);
    }

    /**
     * @brief Gets a double value from the specified column.
     *
     * @param column_index The column index (1-based).
     * @return Optional containing the double value, or empty if NULL.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    Optional<f64> get_double(i32 column_index) throws (SQLException) {
        check_closed();
        
        SQLDOUBLE value;
        SQLLEN indicator;
        
        SQLRETURN ret = SQLGetData(
            stmt, column_index, SQL_C_DOUBLE,
            &value, sizeof(value), &indicator
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get double data");
        }
        
        last_indicator = indicator;
        
        if (indicator == SQL_NULL_DATA) {
            return {};
        }
        
        return static_cast<f64>(value);
    }

    /**
     * @brief Gets a boolean value from the specified column.
     *
     * @param column_index The column index (1-based).
     * @return Optional containing the boolean value, or empty if NULL.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    Optional<bool> get_boolean(i32 column_index) throws (SQLException) {
        check_closed();
        
        SQLCHAR value;
        SQLLEN indicator;
        
        SQLRETURN ret = SQLGetData(
            stmt, column_index, SQL_C_BIT,
            &value, sizeof(value), &indicator
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get boolean data");
        }
        
        last_indicator = indicator;
        
        if (indicator == SQL_NULL_DATA) {
            return {};
        }
        
        return value != 0;
    }

    /**
     * @brief Checks if the last column read was NULL.
     *
     * @return true if the last column was NULL, false otherwise.
     * @throws SQLException if the result set is closed.
     */
    [[nodiscard]]
    bool was_null() const throws (SQLException) {
        check_closed();
        return last_indicator == SQL_NULL_DATA;
    }

    /**
     * @brief Gets the number of columns in the result set.
     *
     * @return The number of columns.
     */
    [[nodiscard]]
    i32 get_column_count() const noexcept {
        return static_cast<i32>(column_count);
    }

    /**
     * @brief Gets the name of the specified column.
     *
     * @param column_index The column index (1-based).
     * @return The column name.
     * @throws SQLException on error.
     */
    [[nodiscard]]
    String get_column_name(i32 column_index) throws (SQLException) {
        check_closed();
        
        SQLCHAR column_name[256];
        SQLSMALLINT name_length;
        
        SQLRETURN ret = SQLDescribeCol(
            stmt, column_index, column_name,
            sizeof(column_name), &name_length,
            nullptr, nullptr, nullptr, nullptr
        );
        
        if (!SQL_SUCCEEDED(ret)) {
            throw SQLException("Failed to get column name");
        }
        
        return String(reinterpret_cast<char*>(column_name));
    }

    /**
     * @brief Closes the result set.
     */
    void close() noexcept {
        if (!closed && stmt != nullptr) {
            SQLCloseCursor(stmt);
            closed = true;
        }
    }

    /**
     * @brief Checks if the result set is closed.
     *
     * @return true if closed, false otherwise.
     */
    [[nodiscard]]
    bool is_closed() const noexcept {
        return closed;
    }
};

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
        int_params{stdx::util::move(other.int_params)}, long_params{stdx::util::move(other.long_params)},
        double_params{stdx::util::move(other.double_params)}, string_params{stdx::util::move(other.string_params)},
        bool_params{stdx::util::move(other.bool_params)}, indicators{stdx::util::move(other.indicators)},
        sql{stdx::util::move(other.sql)}, closed{other.closed} {
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
            int_params = stdx::util::move(other.int_params);
            long_params = stdx::util::move(other.long_params);
            double_params = stdx::util::move(other.double_params);
            string_params = stdx::util::move(other.string_params);
            bool_params = stdx::util::move(other.bool_params);
            indicators = stdx::util::move(other.indicators);
            sql = stdx::util::move(other.sql);
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

/**
 * @class DriverManager
 * @brief Manages database drivers and provides connection factory methods.
 *
 * Provides static methods to obtain database connections, similar to JDBC's DriverManager.
 */
class DriverManager {
public:
    DriverManager() = delete;
    DriverManager(const DriverManager&) = delete;
    DriverManager& operator=(const DriverManager&) = delete;

    /**
     * @brief Establishes a connection to the database.
     *
     * @param conn_str The ODBC connection string.
     * @return A Connection object.
     * @throws SQLException if the connection fails.
     *
     * Example connection strings:
     * - "Driver={SQL Server};Server=localhost;Database=mydb;Uid=user;Pwd=pass;"
     * - "DSN=mydsn;Uid=user;Pwd=pass;"
     * - "Driver={PostgreSQL Unicode};Server=localhost;Port=5432;Database=mydb;Uid=user;Pwd=pass;"
     */
    [[nodiscard]]
    static Connection get_connection(StringView conn_str) throws (SQLException) {
        return Connection(conn_str);
    }

    /**
     * @brief Establishes a connection to the database using DSN.
     *
     * @param dsn The Data Source Name.
     * @param username The database username.
     * @param password The database password.
     * @return A Connection object.
     * @throws SQLException if the connection fails.
     */
    [[nodiscard]]
    static Connection get_connection(StringView dsn, StringView username, StringView password) throws (SQLException) {
        String conn_str = stdx::fmt::format("DSN={};Uid={};Pwd={};", dsn, username, password);
        return Connection(conn_str);
    }

    /**
     * @brief Establishes a connection to a SQL Server database.
     *
     * @param server The server hostname or IP address.
     * @param database The database name.
     * @param username The database username.
     * @param password The database password.
     * @return A Connection object.
     * @throws SQLException if the connection fails.
     */
    [[nodiscard]]
    static Connection get_sql_server_connection(StringView server, StringView database, StringView username, StringView password) throws (SQLException) {
        String conn_str = stdx::fmt::format("Driver={{SQL Server}};Server={};Database={};Uid={};Pwd={};", server, database, username, password);
        return Connection(conn_str);
    }

    /**
     * @brief Establishes a connection to a PostgreSQL database.
     *
     * @param server The server hostname or IP address.
     * @param database The database name.
     * @param username The database username.
     * @param password The database password.
     * @param port The server port (default: 5432).
     * @return A Connection object.
     * @throws SQLException if the connection fails.
     */
    [[nodiscard]]
    static Connection get_postgre_sql_connection(StringView server, StringView database, StringView username, StringView password, int port = 5432) throws (SQLException) {
        String conn_str = stdx::fmt::format("Driver={{PostgreSQL Unicode}};Server={};Port={};Database={};Uid={};Pwd={};", server, port, database, username, password);
        return Connection(conn_str);
    }

    /**
     * @brief Establishes a connection to a MySQL database.
     *
     * @param server The server hostname or IP address.
     * @param database The database name.
     * @param username The database username.
     * @param password The database password.
     * @param port The server port (default: 3306).
     * @return A Connection object.
     * @throws SQLException if the connection fails.
     */
    [[nodiscard]]
    static Connection get_mysql_connection(StringView server, StringView database, StringView username, StringView password, int port = 3306) throws (SQLException) {
        String conn_str = stdx::fmt::format("Driver={{MySQL ODBC 8.0 Unicode Driver}};Server={};Port={};Database={};User={};Password={};", server, port, database, username, password);
        return Connection(conn_str);
    }
};

#endif

}
