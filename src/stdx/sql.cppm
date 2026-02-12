/**
 * @file sql.cppm
 * @module stdx:sql
 * @brief Import of SQL modules.
 *
 * This file imports the modules for the SQL handling library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:sql;
#else
export module stdlibx:sql;
#endif

export import :sql.Connection;
export import :sql.DriverManager;
export import :sql.PreparedStatement;
export import :sql.ResultSet;
export import :sql.SQLException;
export import :sql.Statement;
