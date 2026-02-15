/**
 * @file fstream.cppm
 * @module stdx:io.iomanip
 * @brief Module file for standard library input/output manipulation stream operations.
 *
 * This file contains the implementation of the input/output manipulation operations in the standard library.
 */

module;

#include <iomanip>

export module stdx:io.iomanip;

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
