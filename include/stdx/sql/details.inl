#pragma once

/**
 * @namespace stdx::sql
 * @brief Standard library extension SQL operations.
 */
namespace stdx::sql {
    /**
     * @internal
     * @struct ParamSlot
     * @brief Holds the value and ODBC indicator for a single bound parameter.
     *
     * Uses a Variant to store exactly one type per parameter slot, avoiding
     * the waste of parallel vectors sized for every parameter.
     */
    struct ParamSlot {
        Variant<Monostate, i32, i64, f64, String, u8> value;
        SQLLEN indicator = 0;
    };
}
