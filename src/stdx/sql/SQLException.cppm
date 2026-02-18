/**
 * @file SQLException.cppm
 * @module stdx:sql.SQLException
 * @brief Implementation of the SQLException class.
 *
 * This file contains the implementation of the SQLException class, which represents
 * database access errors and other errors related to SQL operations.
 */

module;

#include "Macros.hpp"

export module stdx:sql.SQLException;

import :main;

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

}
