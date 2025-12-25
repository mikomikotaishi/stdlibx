/**
 * @file fstream.cppm
 * @module std:io.iomanip
 * @brief Module file for standard library input/output manipulation stream operations.
 *
 * This file contains the implementation of the input/output manipulation operations in the standard library.
 */

module;

#include <iomanip>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.iomanip;
#else
export module stdlib:io.iomanip;
#endif

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library input/output manipulation operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
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
