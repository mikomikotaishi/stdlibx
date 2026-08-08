#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
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
}
#endif
