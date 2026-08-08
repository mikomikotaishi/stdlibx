#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
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
}
#endif
