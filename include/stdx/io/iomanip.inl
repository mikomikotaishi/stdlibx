#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output manipulation operations.
 */
export namespace stdx::io {
    using std::resetiosflags;
    using std::setiosflags;
    using std::setbase;
    using std::setfill;
    using std::setprecision;
    using std::setw;
    using std::get_money;
    using std::put_money;
    using std::get_time;
    using std::put_time;
    using std::quoted;
}
