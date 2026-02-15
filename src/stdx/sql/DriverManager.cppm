/**
 * @file DriverManager.cppm
 * @module stdx:sql.DriverManager
 * @brief Implementation of the DriverManager class.
 *
 * This file contains the implementation of the DriverManager class, which provides
 * static methods for obtaining database connections.
 */

module;

#include "Macros.hpp"

export module stdx:sql.DriverManager;

import :fmt;
import :mem;
import :sql.SQLException;
import :sql.Connection;

using stdx::mem::UniquePointer;

/**
 * @namespace stdx::sql
 * @brief Wrapper namespace for standard library extension SQL operations.
 */
export namespace stdx::sql {

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY

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
