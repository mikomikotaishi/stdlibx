#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
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
#endif
