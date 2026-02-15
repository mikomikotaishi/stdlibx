/**
 * @file ResultSet.cppm
 * @module stdx:sql.ResultSet
 * @brief Implementation of the ResultSet class.
 *
 * This file contains the implementation of the ResultSet class, which represents
 * a result set from a SQL query.
 */

module;

#include "Macros.hpp"

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

export module stdx:sql.ResultSet;

import :core;
import :collections;
import :sql.SQLException;

using stdx::collections::Vector;

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
export namespace stdx::sql {

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

#endif

}
