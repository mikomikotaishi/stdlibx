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

#include "stdx/sql/details.inl"
#include "stdx/sql/exceptions.inl"
#include "stdx/sql/details.impl.inl"
#include "stdx/sql/types.inl"
#include "stdx/sql/metadata.inl"
#include "stdx/sql/resultset.inl"
#include "stdx/sql/statement.inl"
#include "stdx/sql/catalog.inl"
#include "stdx/sql/connection.inl"
#include "stdx/sql/url.inl"
#include "stdx/sql/driver.inl"
