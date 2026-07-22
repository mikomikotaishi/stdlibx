/**
 * @file sql.cppm
 * @module stdx:sql
 * @brief Implementation of the SQL library.
 *
 * This file contains the implementation of the SQL library, which wraps ODBC.
 */

module;

#include <version>

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
#include <sql.h>
#include <sqlext.h>
#endif

#include "Macros.hpp"

export module stdx:sql;

import :main;

using stdx::collections::Queue;
using stdx::collections::Vector;
using stdx::meta::DecayType;
using stdx::meta::IsSameValue;
using stdx::net::IPAddress;
using stdx::net::Uri;
using stdx::sync::ConditionVariable;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;
using stdx::sync::UniqueLock;
using stdx::time::Seconds;

#ifdef __cpp_impl_reflection
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::ReflectableAsClass;
#endif

#ifdef __cpp_lib_generator
using stdx::ranges::Generator;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

/**
 * @namespace stdx::sql
 * @brief Standard library extension SQL operations.
 */
namespace stdx::sql {
    /**
     * @internal
     * @struct ParamSlot
     * @brief Holds the value and ODBC indicator for a single bound parameter.
     *
     * Uses a Variant to store exactly one type per parameter slot, avoiding
     * the waste of parallel vectors sized for every parameter.
     */
    struct ParamSlot {
        Variant<Monostate, i32, i64, f64, String, u8> value;
        SQLLEN indicator = 0;
    };
}

/**
 * @namespace stdx::sql
 * @brief Standard library extension SQL operations.
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
        String _msg;
        String _state;
        Optional<i32> _code;
    public:
        /**
         * @brief Constructs a SQLException with a message.
         * @param message The error message.
         */
        explicit SQLException(StringView message):
            Exception(), _msg{message}, _state{"HY000"}, _code{} {}

        /**
         * @brief Constructs a SQLException with a message and SQLSTATE.
         * @param message The error message.
         * @param state The SQLSTATE code.
         */
        SQLException(StringView message, StringView state):
            Exception(), _msg{message}, _state{state}, _code{} {}

        /**
         * @brief Constructs a SQLException with a message, SQLSTATE, and error code.
         * @param message The error message.
         * @param state The SQLSTATE code.
         * @param code The native database error code.
         */
        SQLException(StringView message, StringView state, i32 code):
            Exception(), _msg{message}, _state{state}, _code{code} {}

        #ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
        /**
         * @brief Constructs a SQLException by extracting ODBC diagnostic information.
         * @param handle_type The ODBC handle type (SQL_HANDLE_ENV, SQL_HANDLE_DBC, SQL_HANDLE_STMT).
         * @param handle The ODBC handle to extract diagnostics from.
         * @param context A description of the operation that failed.
         *
         * Calls SQLGetDiagRec to retrieve the actual SQLSTATE, native error code,
         * and driver error message from the ODBC driver.
         */
        SQLException(SQLSMALLINT handle_type, SQLHANDLE handle, StringView context):
            Exception(), _state{"HY000"}, _code{} {
            SQLCHAR state_buf[6]{};
            SQLCHAR message_buf[1024]{};
            SQLINTEGER native_error{};
            SQLSMALLINT message_length{};

            SQLRETURN diag_ret = SQLGetDiagRec(
                handle_type, handle, 1,
                state_buf, &native_error,
                message_buf, sizeof(message_buf), &message_length
            );

            if (SQL_SUCCEEDED(diag_ret)) {
                _msg = Ops::fmt(
                    "{}: {}", context, reinterpret_cast<char*>(message_buf)
                );
                _state = String(reinterpret_cast<char*>(state_buf));
                _code = static_cast<i32>(native_error);
            } else {
                _msg = String(context);
            }
        }
        #endif

        /**
         * @brief Gets the error message.
         * @return The error message as a C-string.
         */
        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
        }

        /**
         * @brief Gets the SQLSTATE code.
         * @return The SQLSTATE code.
         */
        [[nodiscard]]
        const String& state() const noexcept {
            return _state;
        }

        /**
         * @brief Gets the native error code.
         * @return Optional containing the error code if available.
         */
        [[nodiscard]]
        Optional<i32> code() const noexcept {
            return _code;
        }
    };

    /**
     * @class SQLTransientException
     * @brief Exception for temporary SQL errors that may succeed if retried.
     *
     * Represents errors that occur due to transient conditions such as connection
     * timeouts, deadlocks, or temporary resource unavailability. Retrying the
     * operation may succeed.
     */
    class SQLTransientException : public SQLException {
    public:
        using SQLException::SQLException;
    };

    /**
     * @class SQLNonTransientException
     * @brief Exception for permanent SQL errors that will not succeed if retried.
     *
     * Represents errors that occur due to permanent conditions such as syntax
     * errors, constraint violations, or invalid operations. Retrying the
     * operation will not succeed.
     */
    class SQLNonTransientException : public SQLException {
    public:
        using SQLException::SQLException;
    };
}

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

/**
 * @namespace stdx::sql
 * @brief Standard library extension SQL operations.
 */
namespace stdx::sql {
    /**
     * @internal
     * @brief Determines if a SQLSTATE code represents a transient error.
     *
     * Transient errors are temporary conditions where retrying the operation
     * may succeed (e.g., connection lost, deadlock, timeout).
     *
     * @param state The SQLSTATE code (5 characters).
     * @return true if the error is transient, false if permanent or unknown.
     */
    [[nodiscard]]
    bool is_transient_sqlstate(StringView state) noexcept {
        if (state.size() < 2) return false;
        const char first = state[0];
        const char second = state[1];
        // 08xxx: Connection errors (transient)
        if (first == '0' && second == '8') {
            return true;
        }
        // 40xxx: Transaction rollback / deadlock (transient)
        if (first == '4' && second == '0') {
            return true;
        }
        // 57xxx: Operator intervention (transient)
        if (first == '5' && second == '7') {
            return true;
        }
        return false;
    }

    /**
     * @internal
     * @brief Throws the appropriate SQL exception based on ODBC diagnostics.
     *
     * Extracts SQLSTATE from the ODBC handle and throws either
     * SQLTransientException (for transient errors) or SQLNonTransientException
     * (for permanent errors).
     *
     * @param handle_type The ODBC handle type (SQL_HANDLE_ENV, SQL_HANDLE_DBC, SQL_HANDLE_STMT).
     * @param handle The ODBC handle to extract diagnostics from.
     * @param context A description of the operation that failed.
     * @throws SQLTransientException if the error is transient.
     * @throws SQLNonTransientException if the error is non-transient.
     */
    [[noreturn]]
    void throw_sql_exception(SQLSMALLINT handle_type, SQLHANDLE handle, StringView context) {
        SQLCHAR state_buf[6]{};
        SQLCHAR message_buf[1024]{};
        SQLINTEGER native_error{};
        SQLSMALLINT message_length{};

        String msg;
        String sql_state = "HY000";
        Optional<i32> error_code;

        SQLRETURN diag_ret = SQLGetDiagRec(
            handle_type, handle, 1,
            state_buf, &native_error,
            message_buf, sizeof(message_buf), &message_length
        );

        if (SQL_SUCCEEDED(diag_ret)) {
            msg = Ops::fmt(
                "{}: {}", context, reinterpret_cast<char*>(message_buf)
            );
            sql_state = String(reinterpret_cast<char*>(state_buf));
            error_code = static_cast<i32>(native_error);
        } else {
            msg = String(context);
        }

        if (is_transient_sqlstate(sql_state)) {
            throw SQLTransientException(msg, sql_state, error_code.value_or(-1));
        } else {
            throw SQLNonTransientException(msg, sql_state, error_code.value_or(-1));
        }
    }
}

/**
 * @namespace stdx::sql
 * @brief Standard library extension SQL operations.
 */
export namespace stdx::sql {
    /**
     * @enum SQLType
     * @brief Represents SQL data types.
     */
    enum class SQLType: i32 {
        UNKNOWN = SQL_UNKNOWN_TYPE,
        CHAR = SQL_CHAR,
        VARCHAR = SQL_VARCHAR,
        LONGVARCHAR = SQL_LONGVARCHAR,
        NUMERIC = SQL_NUMERIC,
        DECIMAL = SQL_DECIMAL,
        INTEGER = SQL_INTEGER,
        SMALLINT = SQL_SMALLINT,
        FLOAT = SQL_FLOAT,
        REAL = SQL_REAL,
        DOUBLE = SQL_DOUBLE,
        DATE = SQL_TYPE_DATE,
        TIME = SQL_TYPE_TIME,
        TIMESTAMP = SQL_TYPE_TIMESTAMP,
        BIT = SQL_BIT,
        TINYINT = SQL_TINYINT,
        BIGINT = SQL_BIGINT,
        BINARY = SQL_BINARY,
        VARBINARY = SQL_VARBINARY,
        LONGVARBINARY = SQL_LONGVARBINARY,
    };

    /**
     * @enum ColumnNullability
     * @brief Column nullability status.
     */
    enum class ColumnNullability: i32 {
        NO_NULLS = SQL_NO_NULLS,
        NULLABLE = SQL_NULLABLE,
        UNKNOWN = SQL_NULLABLE_UNKNOWN,
    };

    /**
     * @enum ResultSetType
     * @brief Determines cursor scrollability for a ResultSet.
     */
    enum class ResultSetType: i32 {
        FORWARD_ONLY = SQL_CURSOR_FORWARD_ONLY,
        SCROLL_INSENSITIVE = SQL_CURSOR_STATIC,
        SCROLL_SENSITIVE = SQL_CURSOR_KEYSET_DRIVEN,
    };

    /**
     * @class ResultSetMetaData
     * @brief Provides information about the columns in a ResultSet.
     *
     * Wraps ODBC SQLDescribeCol and SQLColAttribute to expose column metadata
     * such as type, display size, precision, scale, and nullability.
     */
    class ResultSetMetaData {
    private:
        SQLHSTMT _stmt;
        i32 _column_count;

        /**
         * @brief Retrieves a numeric column attribute.
         * @param column_index The column index (1-based).
         * @param attribute The ODBC column attribute identifier.
         * @return The numeric attribute value.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        SQLLEN numeric_attr(i32 column_index, SQLUSMALLINT attribute) const {
            SQLLEN value;
            SQLRETURN ret = SQLColAttribute(
                _stmt, static_cast<SQLUSMALLINT>(column_index),
                attribute, nullptr, 0, nullptr, &value
            );
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get column attribute");
            }
            return value;
        }

        /**
         * @brief Retrieves a string column attribute.
         * @param column_index The column index (1-based).
         * @param attribute The ODBC column attribute identifier.
         * @return The string attribute value.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String string_attr(i32 column_index, SQLUSMALLINT attribute) const {
            SQLCHAR buffer[256]{};
            SQLSMALLINT length;
            SQLRETURN ret = SQLColAttribute(
                _stmt, static_cast<SQLUSMALLINT>(column_index),
                attribute, buffer, sizeof(buffer), &length, nullptr
            );
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get column attribute");
            }
            return String(reinterpret_cast<char*>(buffer));
        }

        explicit ResultSetMetaData(SQLHSTMT stmt, i32 column_count) noexcept:
            _stmt{stmt}, _column_count{column_count} {}

        friend class ResultSet;
    public:
        /**
         * @brief Gets the number of columns.
         * @return The column count.
         */
        [[nodiscard]]
        i32 column_count() const noexcept {
            return _column_count;
        }

        /**
         * @brief Gets the name of a column.
         * @param column_index The column index (1-based).
         * @return The column name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String column_name(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_NAME);
        }

        /**
         * @brief Gets the SQL type of a column.
         * @param column_index The column index (1-based).
         * @return The column's SQL type.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        SQLType column_type(i32 column_index) const {
            return static_cast<SQLType>(numeric_attr(column_index, SQL_DESC_CONCISE_TYPE));
        }

        /**
         * @brief Gets the type name of a column as reported by the database.
         * @param column_index The column index (1-based).
         * @return The database-specific type name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String column_type_name(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_TYPE_NAME);
        }

        /**
         * @brief Gets the display size of a column.
         * @param column_index The column index (1-based).
         * @return The maximum number of characters needed to display the column.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 column_display_size(i32 column_index) const {
            return static_cast<i32>(numeric_attr(column_index, SQL_DESC_DISPLAY_SIZE));
        }

        /**
         * @brief Gets the precision of a column.
         * @param column_index The column index (1-based).
         * @return The column precision.
         * @throws SQLException on error.
         *
         * For numeric types this is the number of digits; for string types
         * this is the maximum character length.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 precision(i32 column_index) const {
            return static_cast<i32>(numeric_attr(column_index, SQL_DESC_PRECISION));
        }

        /**
         * @brief Gets the scale of a column (digits to the right of the decimal point).
         * @param column_index The column index (1-based).
         * @return The column scale.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 scale(i32 column_index) const {
            return static_cast<i32>(numeric_attr(column_index, SQL_DESC_SCALE));
        }

        /**
         * @brief Gets the nullability of a column.
         * @param column_index The column index (1-based).
         * @return The nullability status.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ColumnNullability is_nullable(i32 column_index) const {
            return static_cast<ColumnNullability>(numeric_attr(column_index, SQL_DESC_NULLABLE));
        }

        /**
         * @brief Gets the designated column's table name.
         * @param column_index The column index (1-based).
         * @return The table name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String table_name(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_TABLE_NAME);
        }

        /**
         * @brief Gets the catalog (database) name for the column's table.
         * @param column_index The column index (1-based).
         * @return The catalog name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String catalog_name(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_CATALOG_NAME);
        }

        /**
         * @brief Gets the schema name for the column's table.
         * @param column_index The column index (1-based).
         * @return The schema name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String schema_name(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_SCHEMA_NAME);
        }

        /**
         * @brief Gets the column's label (alias or name).
         * @param column_index The column index (1-based).
         * @return The column label.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String column_label(i32 column_index) const {
            return string_attr(column_index, SQL_DESC_LABEL);
        }

        /**
         * @brief Checks if the column is auto-incrementing.
         * @param column_index The column index (1-based).
         * @return true if the column is auto-increment.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool is_auto_increment(i32 column_index) const {
            return numeric_attr(column_index, SQL_DESC_AUTO_UNIQUE_VALUE) == SQL_TRUE;
        }

        /**
         * @brief Checks if the column is case-sensitive.
         * @param column_index The column index (1-based).
         * @return true if the column is case-sensitive.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool is_case_sensitive(i32 column_index) const {
            return numeric_attr(column_index, SQL_DESC_CASE_SENSITIVE) == SQL_TRUE;
        }

        /**
         * @brief Checks if the column can be used in a WHERE clause.
         * @param column_index The column index (1-based).
         * @return true if the column is searchable.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool is_searchable(i32 column_index) const {
            return numeric_attr(column_index, SQL_DESC_SEARCHABLE) != SQL_PRED_NONE;
        }

        /**
         * @brief Gets the octet (byte) length of the column.
         * @param column_index The column index (1-based).
         * @return The byte length of the column data.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 octet_length(i32 column_index) const {
            return static_cast<i32>(numeric_attr(column_index, SQL_DESC_OCTET_LENGTH));
        }
    };

    /**
     * @class ResultSet
     * @brief Represents a result set from a SQL query.
     *
     * Provides methods to navigate through query results and retrieve column values.
     */
    class ResultSet {
    private:
        SQLHSTMT _stmt;
        SQLLEN _last_indicator;
        i16 _column_count;
        bool _scrollable;
        bool _closed;
        // When true, this ResultSet owns the statement handle and frees it on
        // close (used by Connection::query, where no Statement outlives the rows).
        // Normally false: the parent Statement/PreparedStatement owns the handle.
        bool _owns_stmt = false;

        /**
         * @brief Checks if the result set is closed.
         * @throws SQLException if the result set is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("ResultSet is closed");
            }
        }

        /**
         * @brief Finds a column index by name.
         * @param column_name The column name to search for.
         * @return The 1-based column index.
         * @throws SQLException if the column name is not found.
         */
        THROWS(SQLException)
        i32 find_column(StringView column_name) const {
            check_closed();

            for (i32 i = 1; i <= static_cast<i32>(_column_count); ++i) {
                SQLCHAR name_buf[256];
                SQLSMALLINT name_length;

                SQLRETURN ret = SQLDescribeCol(
                    _stmt, static_cast<SQLUSMALLINT>(i), name_buf,
                    sizeof(name_buf), &name_length,
                    nullptr, nullptr, nullptr, nullptr
                );

                if (SQL_SUCCEEDED(ret) &&
                    StringView(reinterpret_cast<char*>(name_buf)) == column_name) {
                    return i;
                }
            }

            throw SQLException(Ops::fmt("Column not found: {}", column_name));
        }

        /**
         * @internal
         * @brief Constructs a ResultSet from an ODBC statement handle.
         * @param stmt The ODBC statement handle.
         * @throws SQLException if retrieving column count fails.
         *
         * This constructor is private and used by Statement and PreparedStatement.
         */
        THROWS(SQLException)
        explicit ResultSet(SQLHSTMT stmt, bool scrollable = false):
            _stmt{stmt}, _last_indicator{0}, _column_count{0}, _scrollable{scrollable}, _closed{false} {
            SQLSMALLINT col_count;
            SQLRETURN ret = SQLNumResultCols(this->_stmt, &col_count);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, this->_stmt, "Failed to get column count");
            }
            _column_count = static_cast<i16>(col_count);
        }

        friend class Statement;
        friend class PreparedStatement;
        friend class CallableStatement;
        friend class DatabaseMetaData;
        friend class Connection;

        /**
         * @internal
         * @brief Takes ownership of the underlying statement handle, so this
         * ResultSet frees it on close.
         *
         * Used by Connection::query, whose one-shot statement does not
         * outlive the returned result set.
         */
        void assume_ownership() noexcept {
            _owns_stmt = true;
        }

    public:
        /**
         * @brief Move constructor.
         */
        ResultSet(ResultSet&& other) noexcept:
            _stmt{other._stmt}, _last_indicator{other._last_indicator},
            _column_count{other._column_count}, _scrollable{other._scrollable},
            _closed{other._closed}, _owns_stmt{other._owns_stmt} {
            other._stmt = nullptr;
            other._closed = true;
            other._owns_stmt = false;
        }

        /**
         * @brief Move assignment operator.
         */
        ResultSet& operator=(ResultSet&& other) noexcept {
            if (this != &other) {
                close();
                _stmt = other._stmt;
                _last_indicator = other._last_indicator;
                _column_count = other._column_count;
                _scrollable = other._scrollable;
                _closed = other._closed;
                _owns_stmt = other._owns_stmt;
                other._stmt = nullptr;
                other._closed = true;
                other._owns_stmt = false;
            }
            return *this;
        }

        ResultSet(const ResultSet&) = delete("ResultSet is not copyable.");
        ResultSet& operator=(const ResultSet&) = delete("ResultSet is not copyable.");

        /**
         * @brief Destructor.
         */
        ~ResultSet() {
            close();
        }

        /**
         * @brief Moves the cursor to the next row.
         * @return true if the next row is valid, false if no more rows.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool next() {
            check_closed();
            SQLRETURN ret = SQLFetch(_stmt);
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch next row");
            }
            return true;
        }

        /**
         * @brief Moves the cursor to the previous row.
         * @return true if the previous row is valid, false if before first.
         * @throws SQLException if not scrollable or on error.
         *
         * Requires a scrollable result set (created from a scrollable Statement).
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool previous() {
            check_closed();
            if (!_scrollable) {
                throw SQLException("ResultSet is not scrollable");
            }
            SQLRETURN ret = SQLFetchScroll(_stmt, SQL_FETCH_PRIOR, 0);
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch previous row");
            }
            return true;
        }

        /**
         * @brief Moves the cursor to the first row.
         * @return true if the first row is valid, false if the result set is empty.
         * @throws SQLException if not scrollable or on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool first() {
            check_closed();
            if (!_scrollable) {
                throw SQLException("ResultSet is not scrollable");
            }
            SQLRETURN ret = SQLFetchScroll(_stmt, SQL_FETCH_FIRST, 0);
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch first row");
            }
            return true;
        }

        /**
         * @brief Moves the cursor to the last row.
         * @return true if the last row is valid, false if the result set is empty.
         * @throws SQLException if not scrollable or on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool last() {
            check_closed();
            if (!_scrollable) {
                throw SQLException("ResultSet is not scrollable");
            }
            SQLRETURN ret = SQLFetchScroll(_stmt, SQL_FETCH_LAST, 0);
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch last row");
            }
            return true;
        }

        /**
         * @brief Moves the cursor to the given absolute row number.
         * @param row The absolute row number.
         * @return true if the row is valid, false otherwise.
         * @throws SQLException if not scrollable or on error.
         *
         * Row numbers are 1-based. A negative value counts from the end
         * (-1 is the last row).
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool absolute(i64 row) {
            check_closed();
            if (!_scrollable) {
                throw SQLException("ResultSet is not scrollable");
            }
            SQLRETURN ret = SQLFetchScroll(_stmt, SQL_FETCH_ABSOLUTE, static_cast<SQLLEN>(row));
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch absolute row");
            }
            return true;
        }

        /**
         * @brief Moves the cursor by a relative number of rows from the current position.
         * @param rows The number of rows to move.
         * @return true if the resulting row is valid, false otherwise.
         * @throws SQLException if not scrollable or on error.
         *
         * A positive value moves forward, negative moves backward.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool relative(i64 rows) {
            check_closed();
            if (!_scrollable) {
                throw SQLException("ResultSet is not scrollable");
            }
            SQLRETURN ret = SQLFetchScroll(_stmt, SQL_FETCH_RELATIVE, static_cast<SQLLEN>(rows));
            if (ret == SQL_NO_DATA) {
                return false;
            }
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to fetch relative row");
            }
            return true;
        }

        /**
         * @brief Checks if this result set is scrollable.
         * @return true if scrollable, false if forward-only.
         */
        [[nodiscard]]
        bool is_scrollable() const noexcept {
            return _scrollable;
        }

        /**
         * @brief Gets a string value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the string value, or empty if NULL.
         * @throws SQLException on error.
         *
         * Handles strings longer than the internal buffer by looping over
         * SQLGetData until all data is retrieved.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<String> get_string(i32 column_index) {
            check_closed();

            String result;
            SQLLEN indicator;
            char buffer[4096];

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_CHAR,
                buffer, sizeof(buffer), &indicator
            );

            if (!SQL_SUCCEEDED(ret) && ret != SQL_SUCCESS_WITH_INFO) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get string data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            result.append(buffer);

            while (ret == SQL_SUCCESS_WITH_INFO) {
                ret = SQLGetData(
                    _stmt, column_index, SQL_C_CHAR,
                    buffer, sizeof(buffer), &indicator
                );

                if (!SQL_SUCCEEDED(ret) && ret != SQL_SUCCESS_WITH_INFO) {
                    throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get string data");
                }

                result.append(buffer);
            }

            return result;
        }

        /**
         * @copydoc get_string(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<String> get_string(StringView column_name) {
            return get_string(find_column(column_name));
        }

        /**
         * @brief Gets a short value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the short value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i16> get_short(i32 column_index) {
            check_closed();

            SQLSMALLINT value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_SSHORT,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get short data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<i16>(value);
        }

        /**
         * @copydoc get_short(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i16> get_short(StringView column_name) {
            return get_short(find_column(column_name));
        }

        /**
         * @brief Gets an integer value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the integer value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i32> get_int(i32 column_index) {
            check_closed();

            SQLINTEGER value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_SLONG,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get integer data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<i32>(value);
        }

        /**
         * @copydoc get_int(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i32> get_int(StringView column_name) {
            return get_int(find_column(column_name));
        }

        /**
         * @brief Gets a long value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the long value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i64> get_long(i32 column_index) {
            check_closed();

            SQLBIGINT value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_SBIGINT,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get long data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<i64>(value);
        }

        /**
         * @copydoc get_long(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<i64> get_long(StringView column_name) {
            return get_long(find_column(column_name));
        }

        /**
         * @brief Gets a float value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the float value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<f32> get_float(i32 column_index) {
            check_closed();

            SQLREAL value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_FLOAT,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get float data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<f32>(value);
        }

        /**
         * @copydoc get_float(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<f32> get_float(StringView column_name) {
            return get_float(find_column(column_name));
        }

        /**
         * @brief Gets a double value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the double value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<f64> get_double(i32 column_index) {
            check_closed();

            SQLDOUBLE value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_DOUBLE,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get double data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<f64>(value);
        }

        /**
         * @copydoc get_double(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<f64> get_double(StringView column_name) {
            return get_double(find_column(column_name));
        }

        /**
         * @brief Gets a boolean value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the boolean value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<bool> get_boolean(i32 column_index) {
            check_closed();

            SQLCHAR value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_BIT,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get boolean data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return value != 0;
        }

        /** @copydoc get_boolean(i32) */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<bool> get_boolean(StringView column_name) {
            return get_boolean(find_column(column_name));
        }

        /**
         * @brief Gets a byte value from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the byte value, or empty if NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<u8> get_byte(i32 column_index) {
            check_closed();

            SQLCHAR value;
            SQLLEN indicator;

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_UTINYINT,
                &value, sizeof(value), &indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get byte data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            return static_cast<u8>(value);
        }

        /** @copydoc get_byte(i32) */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<u8> get_byte(StringView column_name) {
            return get_byte(find_column(column_name));
        }

        /**
         * @brief Gets binary data from the specified column.
         * @param column_index The column index (1-based).
         * @return Optional containing the binary data, or empty if NULL.
         * @throws SQLException on error.
         *
         * Handles data longer than the internal buffer by looping over
         * SQLGetData until all data is retrieved.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<Vector<u8>> get_binary(i32 column_index) {
            check_closed();

            Vector<u8> result;
            SQLLEN indicator;
            u8 buffer[4096];

            SQLRETURN ret = SQLGetData(
                _stmt, column_index, SQL_C_BINARY,
                buffer, sizeof(buffer), &indicator
            );

            if (!SQL_SUCCEEDED(ret) && ret != SQL_SUCCESS_WITH_INFO) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get binary data");
            }

            _last_indicator = indicator;

            if (indicator == SQL_NULL_DATA) {
                return nullopt;
            }

            SQLLEN bytes_to_copy = (ret == SQL_SUCCESS_WITH_INFO)
                ? static_cast<SQLLEN>(sizeof(buffer))
                : indicator;
            result.insert(result.end(), buffer, buffer + bytes_to_copy);

            while (ret == SQL_SUCCESS_WITH_INFO) {
                ret = SQLGetData(
                    _stmt, column_index, SQL_C_BINARY,
                    buffer, sizeof(buffer), &indicator
                );

                if (!SQL_SUCCEEDED(ret) && ret != SQL_SUCCESS_WITH_INFO) {
                    throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get binary data");
                }

                bytes_to_copy = (ret == SQL_SUCCESS_WITH_INFO)
                    ? static_cast<SQLLEN>(sizeof(buffer))
                    : indicator;
                result.insert(result.end(), buffer, buffer + bytes_to_copy);
            }

            return result;
        }

        /**
         * @copydoc get_binary(i32)
         */
        [[nodiscard]]
        THROWS(SQLException)
        Optional<Vector<u8>> get_binary(StringView column_name) {
            return get_binary(find_column(column_name));
        }

        /**
         * @brief Checks if the last column read was NULL.
         * @return true if the last column was NULL, false otherwise.
         * @throws SQLException if the result set is closed.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool was_null() const {
            check_closed();
            return _last_indicator == SQL_NULL_DATA;
        }

        /**
         * @brief Gets the number of columns in the result set.
         * @return The number of columns.
         */
        [[nodiscard]]
        i32 column_count() const noexcept {
            return static_cast<i32>(_column_count);
        }

        /**
         * @brief Gets the name of the specified column.
         * @param column_index The column index (1-based).
         * @return The column name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String column_name(i32 column_index) {
            check_closed();

            SQLCHAR col_name[256];
            SQLSMALLINT name_length;

            SQLRETURN ret = SQLDescribeCol(
                _stmt, column_index, col_name,
                sizeof(col_name), &name_length,
                nullptr, nullptr, nullptr, nullptr
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get column name");
            }

            return String(reinterpret_cast<char*>(col_name));
        }

        /**
         * @brief Gets the metadata for this result set.
         * @return A ResultSetMetaData describing the columns.
         * @throws SQLException if the result set is closed.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSetMetaData meta_data() const {
            check_closed();
            return ResultSetMetaData(_stmt, static_cast<i32>(_column_count));
        }

        /**
         * @brief Closes the result set.
         */
        void close() noexcept {
            if (!_closed && _stmt != nullptr) {
                SQLCloseCursor(_stmt);
                if (_owns_stmt) {
                    SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                    _stmt = nullptr;
                }
                _closed = true;
            }
        }

        /**
         * @brief Checks if the result set is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
        }

        /**
         * @brief Generic typed column accessor by 1-based index.
         * @tparam T The column value type.
         * @param column_index The column index (1-based).
         * @return The column value, or nullopt if the column was SQL NULL.
         * @throws SQLException on error.
         *
         * Dispatches on T to the matching typed getter. Supported column types:
         * i16, i32, i64, f32, f64, bool, u8, String, Vector<u8>.
         */
        template <typename T>
        [[nodiscard]]
        THROWS(SQLException)
        Optional<T> get(i32 column_index) {
            if constexpr (IsSameValue<T, i16>) {
                return get_short(column_index);
            } else if constexpr (IsSameValue<T, i32>) {
                return get_int(column_index);
            } else if constexpr (IsSameValue<T, i64>) {
                return get_long(column_index);
            } else if constexpr (IsSameValue<T, f32>) {
                return get_float(column_index);
            } else if constexpr (IsSameValue<T, f64>) {
                return get_double(column_index);
            } else if constexpr (IsSameValue<T, bool>) {
                return get_boolean(column_index);
            } else if constexpr (IsSameValue<T, u8>) {
                return get_byte(column_index);
            } else if constexpr (IsSameValue<T, String>) {
                return get_string(column_index);
            } else if constexpr (IsSameValue<T, Vector<u8>>) {
                return get_binary(column_index);
            } else {
                static_assert(sizeof(T) == 0, "Unsupported column type for ResultSet::get");
            }
        }

        /**
         * @brief Generic typed column accessor by name.
         * @tparam T The column value type (see get(i32)).
         * @param column_name The column name.
         * @return The column value, or nullopt if the column was SQL NULL.
         * @throws SQLException on error.
         */
        template <typename T>
        [[nodiscard]]
        THROWS(SQLException)
        Optional<T> get(StringView column_name) {
            return get<T>(find_column(column_name));
        }

        /**
         * @class Row
         * @brief A view over the current row, valid only until the cursor advances.
         *
         * Yielded when iterating a ResultSet as a range. Column reads go straight
         * to the live cursor position via the owning ResultSet.
         */
        class Row {
        private:
            ResultSet* _rs;
        public:
            explicit Row(ResultSet* rs) noexcept:
                _rs{rs} {}

            /**
             * @brief Typed column accessor by 1-based index.
             */
            template <typename T>
            [[nodiscard]]
            THROWS(SQLException)
            Optional<T> get(i32 column_index) const {
                return _rs->get<T>(column_index);
            }

            /**
             * @brief Typed column accessor by name.
             */
            template <typename T>
            [[nodiscard]]
            THROWS(SQLException)
            Optional<T> get(StringView column_name) const {
                return _rs->get<T>(column_name);
            }

            /**
             * @brief Whether the most recently read column was SQL NULL.
             */
            [[nodiscard]]
            THROWS(SQLException)
            bool was_null() const {
                return _rs->was_null();
            }

            /**
             * @brief The number of columns in the row.
             */
            [[nodiscard]]
            i32 column_count() const noexcept {
                return _rs->column_count();
            }
        };

        /**
         * @class Iterator
         * @brief Single-pass input iterator over the rows of a ResultSet.
         *
         * An ODBC cursor is forward-only, so this models an input iterator: the
         * range may be traversed exactly once. Dereferencing yields a Row viewing
         * the current cursor position; incrementing fetches the next row, and the
         * iterator compares equal to the end sentinel once the cursor is exhausted.
         */
        class Iterator {
        private:
            ResultSet* _rs;  // nullptr once the cursor is exhausted (i.e. == end())
        public:
            using iterator_concept = InputIteratorTag;
            using iterator_category = InputIteratorTag;
            using value_type = Row;
            using difference_type = isize;

            explicit Iterator(ResultSet* rs) noexcept:
                _rs{rs} {}

            [[nodiscard]]
            Row operator*() const noexcept {
                return Row{_rs};
            }

            THROWS(SQLException)
            Iterator& operator++() {
                if (!_rs->next()) {
                    _rs = nullptr;
                }
                return *this;
            }

            THROWS(SQLException)
            void operator++(int) {
                ++*this;
            }

            [[nodiscard]]
            bool operator==(DefaultSentinel) const noexcept {
                return _rs == nullptr;
            }
        };

        /**
         * @brief Begins single-pass iteration, fetching the first row.
         * @return An Iterator positioned at the first row, or one equal to end()
         * if the result set is empty.
         * @throws SQLException on error.
         *
         * @note The result set is consumed as it is traversed. Iterate it once, and
         *       do not mix range iteration with manual next() calls.
         */
        [[nodiscard]]
        THROWS(SQLException)
        Iterator begin() {
            if (!next()) {
                return Iterator{nullptr};
            }
            return Iterator{this};
        }

        /**
         * @brief The end sentinel for range iteration.
         */
        [[nodiscard]]
        DefaultSentinel end() const noexcept {
            return {};
        }

        #ifdef __cpp_lib_generator
        /**
         * @brief Lazily yields each remaining row as a standalone range.
         * @return A Generator yielding a Row view of each successive row.
         * @throws SQLException on a fetch error, propagated out of advancement.
         *
         * Equivalent in traversal to begin()/end(), but produces a self-contained
         * range object that composes with range adaptors and can be returned or
         * stored independently of an explicit loop. The cursor is consumed as the
         * generator is advanced; the owning ResultSet must outlive the result.
         *
         * @note Synchronous: each step performs a blocking ODBC fetch. This is
         * lazy iteration, not asynchronous I/O. Traverse the result set
         * once, and do not interleave with manual next() or begin()/end().
         */
        [[nodiscard]]
        THROWS(SQLException)
        Generator<Row> rows() {
            while (next()) {
                co_yield Row(this);
            }
        }
        #endif
    };

    /**
     * @concept Bindable
     * @brief A type that can be bound as a SQL statement parameter.
     *
     * Satisfied by bool, nullptr (bound as SQL NULL), any integral or floating-
     * point type, and anything convertible to StringView. Mirrors the dispatch
     * performed by PreparedStatement::bind_one, so the variadic execute overloads
     * reject an unbindable argument at the call site rather than deep inside
     * instantiation.
     */
    template <typename T>
    concept Bindable =
        IsSameValue<DecayType<T>, bool>
        || IsSameValue<DecayType<T>, NullPointer>
        || Integral<DecayType<T>>
        || FloatingPoint<DecayType<T>>
        || ConvertibleTo<DecayType<T>, StringView>;

    /**
     * @class PreparedStatement
     * @brief Represents a precompiled SQL statement with parameter binding.
     *
     * Provides methods to bind parameters and execute parameterized SQL queries.
     */
    class PreparedStatement {
    private:
        Vector<Vector<ParamSlot>> _batch;
        Vector<ParamSlot> _params;
        String _sql;
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        bool _closed;

        /**
         * @brief Checks if the statement is closed.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("PreparedStatement is closed");
            }
        }

        /**
         * @brief Validates a 1-based parameter index.
         * @param parameter_index The parameter index (1-based).
         * @throws SQLException if the index is out of range.
         */
        THROWS(SQLException)
        void check_index(i32 parameter_index) const {
            if (parameter_index < 1 || parameter_index > static_cast<i32>(_params.size())) {
                throw SQLException("Invalid parameter index");
            }
        }

        /**
         * @brief Binds a single value to a 1-based parameter index, dispatching on
         * the C++ type to the appropriate set_* overload.
         * @tparam T The deduced argument type.
         * @param parameter_index The parameter index (1-based).
         * @param value The value to bind.
         * @throws SQLException on error.
         *
         * Integral types are bound as INTEGER or BIGINT by width, floating-point as
         * DOUBLE, bool as BIT, anything convertible to StringView as VARCHAR, and
         * nullptr as SQL NULL.
         */
        template <typename T>
        THROWS(SQLException)
        void bind_one(i32 parameter_index, T&& value) {
            using V = DecayType<T>;
            if constexpr (IsSameValue<V, bool>) {
                set_boolean(parameter_index, value);
            } else if constexpr (IsSameValue<V, NullPointer>) {
                set_null(parameter_index);
            } else if constexpr (Integral<V>) {
                if constexpr (sizeof(V) > sizeof(i32)) {
                    set_long(parameter_index, static_cast<i64>(value));
                } else {
                    set_int(parameter_index, static_cast<i32>(value));
                }
            } else if constexpr (FloatingPoint<V>) {
                set_double(parameter_index, static_cast<f64>(value));
            } else if constexpr (ConvertibleTo<V, StringView>) {
                set_string(parameter_index, StringView(value));
            } else {
                static_assert(sizeof(V) == 0, "Unsupported SQL parameter type");
            }
        }

        /**
         * @brief Binds all arguments positionally (1-based), left to right.
         * @tparam Args The deduced argument types.
         * @param args The values to bind, one per parameter marker.
         * @throws SQLException if the argument count does not match the number of
         * parameter markers in the prepared statement.
         */
        template <Bindable... Args>
        THROWS(SQLException)
        void bind_all(Args&&... args) {
            if (sizeof...(Args) != _params.size()) {
                throw SQLException("Argument count does not match parameter count");
            }
            i32 index = 1;
            (bind_one(index++, Ops::forward<Args>(args)), ...);
        }

        /**
         * @internal
         * @brief Constructs a PreparedStatement from an ODBC connection handle.
         * @param dbc The ODBC connection handle.
         * @param sql The SQL statement with parameter placeholders (?).
         * @throws SQLException if statement preparation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        PreparedStatement(SQLHDBC dbc, StringView sql):
            _sql{sql}, _dbc{dbc}, _stmt{nullptr}, _closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, dbc, "Failed to allocate statement handle");
            }

            ret = SQLPrepare(
                this->_stmt,
                reinterpret_cast<SQLCHAR*>(const_cast<char*>(this->_sql.c_str())),
                SQL_NTS
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, this->_stmt, "Failed to prepare statement");
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                throw ex;
            }

            SQLSMALLINT param_count;
            SQLNumParams(this->_stmt, &param_count);

            _params.resize(param_count);
        }

        /**
         * @internal
         * @brief Relinquishes the statement handle without freeing it.
         * Used by Connection::query to transfer handle ownership to the
         * ResultSet it returns; this PreparedStatement then destructs
         * harmlessly.
         */
        void release_handle() noexcept {
            _stmt = nullptr;
            _closed = true;
        }

        friend class Connection;
    public:
        /**
         * @brief Move constructor.
         */
        PreparedStatement(PreparedStatement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt},
            _params{Ops::move(other._params)},
            _batch{Ops::move(other._batch)},
            _sql{Ops::move(other._sql)}, _closed{other._closed} {
            other._stmt = nullptr;
            other._closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        PreparedStatement& operator=(PreparedStatement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _params = Ops::move(other._params);
                _batch = Ops::move(other._batch);
                _sql = Ops::move(other._sql);
                _closed = other._closed;
                other._stmt = nullptr;
                other._closed = true;
            }
            return *this;
        }

        PreparedStatement(const PreparedStatement&) = delete("PreparedStatement is not copyable.");
        PreparedStatement& operator=(const PreparedStatement&) = delete("PreparedStatement is not copyable.");

        /**
         * @brief Destructor.
         */
        ~PreparedStatement() {
            close();
        }

        /**
         * @brief Binds an integer parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_int(i32 parameter_index, i32 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                0, 0, &Ops::get<i32>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind integer parameter");
            }
        }

        /**
         * @brief Binds a long integer parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The long integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_long(i32 parameter_index, i64 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
                0, 0, &Ops::get<i64>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind long parameter");
            }
        }

        /**
         * @brief Binds a double parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The double value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_double(i32 parameter_index, f64 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                0, 0, &Ops::get<f64>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind double parameter");
            }
        }

        /**
         * @brief Binds a string parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The string value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_string(i32 parameter_index, StringView value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = String(value);
            const String& str = Ops::get<String>(slot.value);
            slot.indicator = static_cast<SQLLEN>(str.size());

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                str.size(), 0, const_cast<char*>(str.c_str()),
                str.size(), &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind string parameter");
            }
        }

        /**
         * @brief Binds a boolean parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The boolean value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_boolean(i32 parameter_index, bool value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = static_cast<u8>(value ? 1 : 0);
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0,
                &Ops::get<u8>(slot.value), 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind boolean parameter");
            }
        }

        /**
         * @brief Sets a parameter to NULL.
         * @param parameter_index The parameter index (1-based).
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_null(i32 parameter_index) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = Monostate{};
            slot.indicator = SQL_NULL_DATA;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                0, 0, nullptr, 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind NULL parameter");
            }
        }

        /**
         * @brief Executes the prepared query and returns a result set.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute prepared query");
            }

            return ResultSet(_stmt);
        }

        /**
         * @brief Executes the prepared update/insert/delete statement.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        i32 execute_update() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute prepared update");
            }

            SQLLEN row_count;
            ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Binds the given arguments to the parameter markers in order, then
         * executes the prepared query.
         * @tparam Args The deduced argument types (at least one).
         * @param args One value per parameter marker, in order.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         *
         * Each argument is bound positionally to the corresponding `?` marker,
         * dispatching on its C++ type. Because values are sent as bound parameters,
         * they are never interpreted as SQL - this is the injection-safe path.
         */
        template <Bindable... Args>
            requires (sizeof...(Args) >= 1)
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query(Args&&... args) {
            bind_all(Ops::forward<Args>(args)...);
            return execute_query();
        }

        /**
         * @brief Binds the given arguments to the parameter markers in order, then
         * executes the prepared update/insert/delete statement.
         * @tparam Args The deduced argument types (at least one).
         * @param args One value per parameter marker, in order.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        template <Bindable... Args>
            requires (sizeof...(Args) >= 1)
        THROWS(SQLException)
        i32 execute_update(Args&&... args) {
            bind_all(Ops::forward<Args>(args)...);
            return execute_update();
        }

        /**
         * @brief Adds the current set of parameters to the batch.
         * @throws SQLException if the statement is closed.
         *
         * Saves a snapshot of the current parameter values. After calling this,
         * new parameters can be set for the next batch entry. Call execute_batch()
         * to execute all accumulated entries.
         */
        THROWS(SQLException)
        void add_batch() {
            check_closed();
            _batch.push_back(_params);
        }

        /**
         * @brief Executes all batched parameter sets.
         * @return A vector of row counts, one per batch entry.
         * @throws SQLException on error (execution stops at the first failure).
         *
         * Iterates over each snapshot added via add_batch(), rebinds parameters,
         * and executes the statement. Returns the row counts for each execution.
         * The batch is cleared after execution.
         */
        THROWS(SQLException)
        Vector<i32> execute_batch() {
            check_closed();

            Vector<i32> results;
            results.reserve(_batch.size());

            for (Vector<ParamSlot>& entry: _batch) {
                SQLFreeStmt(_stmt, SQL_RESET_PARAMS);

                for (i32 i = 0; i < static_cast<i32>(entry.size()); ++i) {
                    ParamSlot& slot = entry[i];
                    i32 param_index = i + 1;

                    Ops::visit([&](auto& val) -> void {
                        if constexpr (IsSameValue<DecayType<decltype(val)>, Monostate>) {
                            slot.indicator = SQL_NULL_DATA;
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR, 0, 0,
                                nullptr, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, i32>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_SLONG, SQL_INTEGER, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, i64>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, f64>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_DOUBLE, SQL_DOUBLE, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, String>) {
                            slot.indicator = static_cast<SQLLEN>(val.size());
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                val.size(), 0, const_cast<char*>(val.c_str()),
                                val.size(), &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, u8>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_BIT, SQL_BIT, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        }
                    }, slot.value);
                }

                SQLRETURN ret = SQLExecute(_stmt);
                if (!SQL_SUCCEEDED(ret)) {
                    clear_batch();
                    throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute batch entry");
                }

                SQLLEN row_count;
                SQLRowCount(_stmt, &row_count);
                results.push_back(static_cast<i32>(row_count));
            }

            clear_batch();
            return results;
        }

        /**
         * @brief Clears all accumulated batch entries.
         */
        void clear_batch() noexcept {
            _batch.clear();
        }

        /**
         * @brief Clears all parameter bindings.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void clear_parameters() {
            check_closed();
            SQLFreeStmt(_stmt, SQL_RESET_PARAMS);
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!_closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                _closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
        }
    };

    /**
     * @class CallableStatement
     * @brief Represents a callable statement for executing stored procedures.
     *
     * Extends PreparedStatement semantics with support for registering and
     * retrieving OUT parameters. Uses the ODBC `{call procedure_name(?, ?)}` syntax.
     */
    class CallableStatement {
    private:
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        Vector<ParamSlot> _params;
        Vector<SQLSMALLINT> _param_directions;
        String _sql;
        bool _closed;

        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("CallableStatement is closed");
            }
        }

        THROWS(SQLException)
        void check_index(i32 parameter_index) const {
            if (parameter_index < 1 || parameter_index > static_cast<i32>(_params.size())) {
                throw SQLException("Invalid parameter index");
            }
        }

        /**
         * @internal
         * @brief Constructs a CallableStatement.
         * @param dbc The ODBC connection handle.
         * @param procedure_call The ODBC call syntax, e.g. "{call my_proc(?, ?)}".
         * @throws SQLException if statement preparation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        CallableStatement(SQLHDBC dbc, StringView procedure_call):
            _dbc{dbc}, _stmt{nullptr}, _params{}, _param_directions{},
            _sql{procedure_call}, _closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, dbc, "Failed to allocate statement handle");
            }

            ret = SQLPrepare(
                this->_stmt,
                reinterpret_cast<SQLCHAR*>(const_cast<char*>(this->_sql.c_str())),
                SQL_NTS
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, this->_stmt, "Failed to prepare callable statement");
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                throw ex;
            }

            SQLSMALLINT param_count;
            SQLNumParams(this->_stmt, &param_count);

            _params.resize(param_count);
            _param_directions.resize(param_count, SQL_PARAM_INPUT);
        }

        friend class Connection;

    public:
        /**
         * @brief Move constructor.
         */
        CallableStatement(CallableStatement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt},
            _params{Ops::move(other._params)},
            _param_directions{Ops::move(other._param_directions)},
            _sql{Ops::move(other._sql)}, _closed{other._closed} {
            other._stmt = nullptr;
            other._closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        CallableStatement& operator=(CallableStatement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _params = Ops::move(other._params);
                _param_directions = Ops::move(other._param_directions);
                _sql = Ops::move(other._sql);
                _closed = other._closed;
                other._stmt = nullptr;
                other._closed = true;
            }
            return *this;
        }

        CallableStatement(const CallableStatement&) = delete("CallableStatement is not copyable.");
        CallableStatement& operator=(const CallableStatement&) = delete("CallableStatement is not copyable.");

        ~CallableStatement() {
            close();
        }

        /**
         * @brief Registers a parameter as an OUT parameter.
         * @param parameter_index The parameter index (1-based).
         * @param type The SQL type of the OUT parameter.
         * @throws SQLException on error.
         *
         * Must be called before execute() for each OUT or INOUT parameter.
         * The SQL type tells the driver what to expect from the procedure.
         */
        THROWS(SQLException)
        void register_out_parameter(i32 parameter_index, SQLType type) {
            check_closed();
            check_index(parameter_index);

            _param_directions[parameter_index - 1] = SQL_PARAM_OUTPUT;
            ParamSlot& slot = _params[parameter_index - 1];
            slot.indicator = 0;

            SQLSMALLINT c_type = SQL_C_CHAR;
            SQLULEN col_size = 256;

            switch (type) {
                case SQLType::INTEGER:
                    slot.value = i32{0};
                    c_type = SQL_C_SLONG;
                    col_size = 0;
                    break;
                case SQLType::BIGINT:
                    slot.value = i64{0};
                    c_type = SQL_C_SBIGINT;
                    col_size = 0;
                    break;
                case SQLType::DOUBLE:
                case SQLType::FLOAT:
                    slot.value = f64{0.0};
                    c_type = SQL_C_DOUBLE;
                    col_size = 0;
                    break;
                case SQLType::BIT:
                    slot.value = u8{0};
                    c_type = SQL_C_BIT;
                    col_size = 0;
                    break;
                default:
                    slot.value = String(256, '\0');
                    c_type = SQL_C_CHAR;
                    col_size = 256;
                    slot.indicator = SQL_NTS;
                    break;
            }

            void* value_ptr = Ops::visit([](auto& val) -> void* {
                if constexpr (IsSameValue<DecayType<decltype(val)>, Monostate>) {
                    return nullptr;
                } else if constexpr (IsSameValue<DecayType<decltype(val)>, String>) {
                    return val.data();
                } else {
                    return &val;
                }
            }, slot.value);

            SQLLEN buffer_len = 0;
            if (holds_alternative<String>(slot.value)) {
                buffer_len = static_cast<SQLLEN>(Ops::get<String>(slot.value).size());
            }

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_OUTPUT,
                c_type, static_cast<SQLSMALLINT>(static_cast<i32>(type)),
                col_size, 0, value_ptr, buffer_len, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to register OUT parameter");
            }
        }

        /**
         * @brief Sets an integer IN parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_int(i32 parameter_index, i32 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;
            _param_directions[parameter_index - 1] = SQL_PARAM_INPUT;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                0, 0, &Ops::get<i32>(slot.value), 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind integer parameter");
            }
        }

        /**
         * @brief Sets a string IN parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The string value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_string(i32 parameter_index, StringView value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = String(value);
            const String& str = Ops::get<String>(slot.value);
            slot.indicator = static_cast<SQLLEN>(str.size());
            _param_directions[parameter_index - 1] = SQL_PARAM_INPUT;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                str.size(), 0, const_cast<char*>(str.c_str()),
                str.size(), &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind string parameter");
            }
        }

        /**
         * @brief Executes the callable statement.
         * @return true if the result is a ResultSet, false otherwise.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool execute() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute callable statement");
            }

            SQLSMALLINT col_count;
            ret = SQLNumResultCols(_stmt, &col_count);
            return SQL_SUCCEEDED(ret) && col_count > 0;
        }

        /**
         * @brief Gets the result set from the last execute() call.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet result_set() {
            check_closed();
            return ResultSet(_stmt);
        }

        /**
         * @brief Gets an integer OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The integer value.
         * @throws SQLException if the parameter is not an integer OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 get_int(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<i32>(slot.value);
        }

        /**
         * @brief Gets a long OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The long value.
         * @throws SQLException if the parameter is not a long OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i64 get_long(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<i64>(slot.value);
        }

        /**
         * @brief Gets a double OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The double value.
         * @throws SQLException if the parameter is not a double OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        f64 get_double(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<f64>(slot.value);
        }

        /**
         * @brief Gets a string OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The string value.
         * @throws SQLException if the parameter is not a string OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String get_string(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<String>(slot.value);
        }

        /**
         * @brief Checks if the last OUT parameter read was NULL.
         * @param parameter_index The parameter index (1-based).
         * @return true if the parameter value is NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool was_null(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);
            return _params[parameter_index - 1].indicator == SQL_NULL_DATA;
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!_closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                _closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
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
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        ResultSetType _rs_type;
        bool closed;

        /**
         * @brief Checks if the statement is closed.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (closed) {
                throw SQLException("Statement is closed");
            }
        }

        /**
         * @internal
         * @brief Constructs a Statement from an ODBC connection handle.
         * @param dbc The ODBC connection handle.
         * @throws SQLException if statement allocation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        explicit Statement(SQLHDBC dbc):
            _dbc{dbc}, _stmt{nullptr}, _rs_type{ResultSetType::FORWARD_ONLY}, closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, this->_dbc, &this->_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, this->_dbc, "Failed to allocate statement handle");
            }
        }

        /**
         * @internal
         * @brief Relinquishes the statement handle without freeing it.
         *
         * Used by Connection::query to transfer handle ownership to the
         * ResultSet it returns; this Statement then destructs harmlessly.
         */
        void release_handle() noexcept {
            _stmt = nullptr;
            closed = true;
        }

        friend class Connection;

    public:
        /**
         * @brief Move constructor.
         */
        Statement(Statement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt}, _rs_type{other._rs_type}, closed{other.closed} {
            other._stmt = nullptr;
            other.closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        Statement& operator=(Statement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _rs_type = other._rs_type;
                closed = other.closed;
                other._stmt = nullptr;
                other.closed = true;
            }
            return *this;
        }

        Statement(const Statement&) = delete("Statement is not copyable.");
        Statement& operator=(const Statement&) = delete("Statement is not copyable.");

        /**
         * @brief Destructor.
         */
        ~Statement() {
            close();
        }

        /**
         * @brief Executes a SQL query and returns a result set.
         * @param sql The SQL query to execute.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute query");
            }

            return ResultSet(_stmt, _rs_type != ResultSetType::FORWARD_ONLY);
        }

        /**
         * @brief Executes a SQL update/insert/delete statement.
         * @param sql The SQL statement to execute.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 execute_update(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute update");
            }

            SQLLEN row_count;
            ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Executes any SQL statement.
         * @param sql The SQL statement to execute.
         * @return true if the result is a ResultSet, false otherwise.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool execute(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute statement");
            }

            SQLSMALLINT column_count;
            ret = SQLNumResultCols(_stmt, &column_count);

            return SQL_SUCCEEDED(ret) && column_count > 0;
        }

        /**
         * @brief Gets the result set from the last execute() call.
         * @return A ResultSet containing the query results.
         * @throws SQLException if no result set is available.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet result_set() const {
            check_closed();
            return ResultSet(_stmt, _rs_type != ResultSetType::FORWARD_ONLY);
        }

        /**
         * @brief Gets the update count from the last execute_update() call.
         *
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 update_count() {
            check_closed();

            SQLLEN row_count;
            SQLRETURN ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Sets the result set type (scrollability) for subsequent queries.
         * @param type The desired result set type.
         * @throws SQLException on error.
         *
         * Must be called before execute_query(). Sets the ODBC cursor type
         * on the underlying statement handle.
         */
        THROWS(SQLException)
        void set_result_set_type(ResultSetType type) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_CURSOR_TYPE,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(static_cast<i32>(type))), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set cursor type");
            }

            _rs_type = type;
        }

        /**
         * @brief Sets the query timeout in seconds.
         * @param seconds The timeout in seconds.
         * @throws SQLException on error.
         *
         * If the query does not complete within the specified time, it is cancelled.
         * A value of 0 means no timeout (the default).
         */
        THROWS(SQLException)
        void set_query_timeout(i32 seconds) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_QUERY_TIMEOUT,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(seconds)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set query timeout");
            }
        }

        /**
         * @brief Sets the maximum number of rows to return from a query.
         * @param max The maximum number of rows.
         * @throws SQLException on error.
         *
         * A value of 0 means no limit (the default).
         */
        THROWS(SQLException)
        void set_max_rows(i32 max) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_MAX_ROWS,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(max)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set max rows");
            }
        }

        /**
         * @brief Sets the number of rows to fetch at a time (fetch size hint).
         * @param size The fetch size.
         * @throws SQLException on error.
         *
         * This is a hint to the driver for performance optimization.
         * A value of 0 means use the driver default.
         */
        THROWS(SQLException)
        void set_fetch_size(i32 size) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_ROW_ARRAY_SIZE,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(size)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set fetch size");
            }
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return closed;
        }
    };

    /**
     * @class DatabaseMetaData
     * @brief Provides information about the database as a whole.
     *
     * Wraps ODBC SQLGetInfo, SQLTables, and SQLColumns to expose database-level
     * metadata such as product name, supported features, and schema information.
     */
    class DatabaseMetaData {
    private:
        SQLHDBC _dbc;

        /**
         * @brief Retrieves a string info value from the connection.
         * @param info_type The ODBC info type identifier.
         * @return The string value.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String string_info(SQLUSMALLINT info_type) const {
            SQLCHAR buffer[256]{};
            SQLSMALLINT length;
            SQLRETURN ret = SQLGetInfo(_dbc, info_type, buffer, sizeof(buffer), &length);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to get database info");
            }
            return String(reinterpret_cast<char*>(buffer));
        }

        /**
         * @brief Retrieves a 16-bit integer info value from the connection.
         * @param info_type The ODBC info type identifier.
         * @return The integer value.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        u16 u16_info(SQLUSMALLINT info_type) const {
            SQLUSMALLINT value;
            SQLRETURN ret = SQLGetInfo(_dbc, info_type, &value, sizeof(value), nullptr);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to get database info");
            }
            return static_cast<u16>(value);
        }

        /**
         * @brief Retrieves a 32-bit integer info value from the connection.
         * @param info_type The ODBC info type identifier.
         * @return The integer value.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        u32 u32_info(SQLUSMALLINT info_type) const {
            SQLUINTEGER value;
            SQLRETURN ret = SQLGetInfo(_dbc, info_type, &value, sizeof(value), nullptr);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to get database info");
            }
            return static_cast<u32>(value);
        }

        explicit DatabaseMetaData(SQLHDBC dbc) noexcept:
            _dbc{dbc} {}

        friend class Connection;
    public:
        /**
         * @brief Gets the database product name.
         * @return The product name (e.g. "PostgreSQL", "MySQL", "Microsoft SQL Server").
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String database_product_name() const {
            return string_info(SQL_DBMS_NAME);
        }

        /**
         * @brief Gets the database product version.
         * @return The version string.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String database_product_version() const {
            return string_info(SQL_DBMS_VER);
        }

        /**
         * @brief Gets the ODBC driver name.
         * @return The driver name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String driver_name() const {
            return string_info(SQL_DRIVER_NAME);
        }

        /**
         * @brief Gets the ODBC driver version.
         * @return The driver version string.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String driver_version() const {
            return string_info(SQL_DRIVER_VER);
        }

        /**
         * @brief Gets the current database/catalog name.
         * @return The database name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String database_name() const {
            return string_info(SQL_DATABASE_NAME);
        }

        /**
         * @brief Gets the current user name.
         * @return The user name.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String user_name() const {
            return string_info(SQL_USER_NAME);
        }

        /**
         * @brief Gets the string used to quote identifiers.
         * @return The identifier quote string (e.g. "\"" or "`").
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String identifier_quote_string() const {
            return string_info(SQL_IDENTIFIER_QUOTE_CHAR);
        }

        /**
         * @brief Gets the term the database uses for "catalog".
         * @return The catalog term (e.g. "database").
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String catalog_term() const {
            return string_info(SQL_CATALOG_TERM);
        }

        /**
         * @brief Gets the term the database uses for "schema".
         * @return The schema term (e.g. "schema", "owner").
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String schema_term() const {
            return string_info(SQL_SCHEMA_TERM);
        }

        /**
         * @brief Gets the maximum number of columns allowed in a table.
         * @return The maximum column count, or 0 if unlimited/unknown.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 max_columns_in_table() const {
            return static_cast<i32>(u16_info(SQL_MAX_COLUMNS_IN_TABLE));
        }

        /**
         * @brief Gets the maximum length of a statement string.
         * @return The maximum statement length, or 0 if unlimited.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 max_statement_length() const {
            return static_cast<i32>(u32_info(SQL_MAX_STATEMENT_LEN));
        }

        /**
         * @brief Checks if transactions are supported.
         * @return true if the database supports transactions.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool supports_transactions() const {
            return u16_info(SQL_TXN_CAPABLE) != SQL_TC_NONE;
        }

        /**
         * @brief Retrieves a result set describing available tables.
         * @param catalog Catalog name pattern (or "" for all).
         * @param schema Schema name pattern (or "" for all).
         * @param table Table name pattern (or "" for all).
         * @param types Comma-separated table types (e.g. "TABLE,VIEW") or "" for all.
         * @return A ResultSet with columns: TABLE_CAT, TABLE_SCHEM, TABLE_NAME, TABLE_TYPE, REMARKS.
         * @throws SQLException on error.
         *
         * Wraps SQLTables. Pass empty strings to match all. Patterns may use
         * '%' and '_' wildcards.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet tables(
            StringView catalog,
            StringView schema,
            StringView table,
            StringView types
        ) const {
            SQLHSTMT stmt;
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to allocate statement for getTables");
            }

            String cat_str(catalog);
            String sch_str(schema);
            String tbl_str(table);
            String typ_str(types);

            ret = SQLTables(
                stmt,
                cat_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(cat_str.data()),
                cat_str.empty() ? 0 : static_cast<SQLSMALLINT>(cat_str.size()),
                sch_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(sch_str.data()),
                sch_str.empty() ? 0 : static_cast<SQLSMALLINT>(sch_str.size()),
                tbl_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(tbl_str.data()),
                tbl_str.empty() ? 0 : static_cast<SQLSMALLINT>(tbl_str.size()),
                typ_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(typ_str.data()),
                typ_str.empty() ? 0 : static_cast<SQLSMALLINT>(typ_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, stmt, "Failed to retrieve tables");
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                throw ex;
            }

            return ResultSet(stmt);
        }

        /**
         * @brief Retrieves a result set describing columns in specified tables.
         * @param catalog Catalog name pattern (or "" for all).
         * @param schema Schema name pattern (or "" for all).
         * @param table Table name pattern (or "" for all).
         * @param column Column name pattern (or "" for all).
         * @return A ResultSet with columns including: TABLE_CAT, TABLE_SCHEM, TABLE_NAME,
         * COLUMN_NAME, DATA_TYPE, TYPE_NAME, COLUMN_SIZE, NULLABLE, etc.
         * @throws SQLException on error.
         *
         * Wraps SQLColumns. Pass empty strings to match all. Patterns may use
         * '%' and '_' wildcards.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet columns(
            StringView catalog,
            StringView schema,
            StringView table,
            StringView column
        ) const {
            SQLHSTMT stmt;
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, _dbc, "Failed to allocate statement for getColumns");
            }

            String cat_str(catalog);
            String sch_str(schema);
            String tbl_str(table);
            String col_str(column);

            ret = SQLColumns(
                stmt,
                cat_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(cat_str.data()),
                cat_str.empty() ? 0 : static_cast<SQLSMALLINT>(cat_str.size()),
                sch_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(sch_str.data()),
                sch_str.empty() ? 0 : static_cast<SQLSMALLINT>(sch_str.size()),
                tbl_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(tbl_str.data()),
                tbl_str.empty() ? 0 : static_cast<SQLSMALLINT>(tbl_str.size()),
                col_str.empty() ? nullptr : reinterpret_cast<SQLCHAR*>(col_str.data()),
                col_str.empty() ? 0 : static_cast<SQLSMALLINT>(col_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, stmt, "Failed to retrieve columns");
                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                throw ex;
            }

            return ResultSet(stmt);
        }
    };

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
            if (!login_timeout) {
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
            (void)set_login_timeout(_dbc, login_timeout);

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
            (void)set_login_timeout(_dbc, login_timeout);

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

        Connection(const Connection&) = delete("Connection is not copyable.");
        Connection& operator=(const Connection&) = delete("Connection is not copyable.");

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
        [[=Throws<SQLException>()]]
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
        Transaction(const Transaction&) = delete("Transaction is not copyable.");
        Transaction& operator=(const Transaction&) = delete("Transaction is not copyable.");
        Transaction& operator=(Transaction&&) = delete("Transaction is not movable.");

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
        explicit DatabaseUrl(const Uri& uri) {
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
            if (!uri) {
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

    /**
     * @class DriverManager
     * @brief Manages database drivers and provides connection factory methods.
     *
     * Provides static methods to obtain database connections, similar to JDBC's DriverManager.
     */
    class DriverManager {
    public:
        DriverManager() = delete("DriverManager is a static class and cannot be instantiated.");

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

        DataSource(const DataSource&) = delete("DataSource is not copyable.");
        DataSource& operator=(const DataSource&) = delete("DataSource is not copyable.");

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
