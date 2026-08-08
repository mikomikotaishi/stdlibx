#pragma once

using stdx::collections::Vector;
using stdx::meta::IsSameValue;

#ifdef __cpp_lib_generator
using stdx::ranges::Generator;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
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

        ResultSet(const ResultSet&) = DELETE_METHOD("ResultSet is not copyable.");
        ResultSet& operator=(const ResultSet&) = DELETE_METHOD("ResultSet is not copyable.");

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
}
#endif
