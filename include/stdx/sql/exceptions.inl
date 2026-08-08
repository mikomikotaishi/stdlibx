#pragma once

/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
    /**
     * @class SQLException
     * @brief Exception class for SQL operations.
     *
     * Represents errors that occur during SQL database operations, including
     * SQLSTATE codes and native error codes from the database.
     */
    class SQLException: public Exception {
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
    class SQLTransientException: public SQLException {
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
    class SQLNonTransientException: public SQLException {
    public:
        using SQLException::SQLException;
    };
}
