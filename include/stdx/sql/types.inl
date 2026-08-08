#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
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
}
#endif
