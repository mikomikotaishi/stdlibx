/**
 * @file Level.cppm
 * @module stdx:util.logging.Level
 * @brief Definition of the Level enum class.
 *
 * This file contains the definition of the Level enum class, which represents logging message level.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util.logging.Level;

import std;

using std::fmt::FormatContext;
using std::fmt::FormatParseContext;
using std::fmt::Formatter;

namespace fmt = std::fmt;
namespace sys = std::sys;
#else
export module stdlibx:util.logging.Level;

import std;

using stdlib::fmt::FormatContext;
using stdlib::fmt::FormatParseContext;
using stdlib::fmt::Formatter;

namespace fmt = stdlib::fmt;
namespace sys = stdlib::sys;
#endif

/**
 * @namespace stdx::util::logging
 * @brief Wrapper namespace for standard library extension utility operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::util::logging {
#else
export namespace stdlibx::util::logging {
#endif

/**
 * @enum Level
 * @brief Enumeration for log message levels.
 * 
 * The Level enumeration defines the logger message level.
 */
enum class Level: u8 {
    TRACE, ///< A diagnostic tracing message type
    DEBUG, ///< A debug message type
    INFO, ///< An info message type
    WARNING, ///< A warning message type
    ERROR ///< An error message type
};

}

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
using stdx::util::logging::Level;
#else
using stdlibx::util::logging::Level;
#endif

template <>
struct Formatter<Level> {
    static constexpr const char* parse(FormatParseContext& ctx) noexcept {
        return ctx.begin();
    }

    static FormatContext::iterator format(Level lvl, FormatContext& ctx) {
        StringView name;
        switch (lvl) {
            case Level::TRACE:
                name = "[TRACE]:";
                break;
            case Level::DEBUG:
                name = "[DEBUG]:";
                break;
            case Level::INFO:
                name = "[INFO]:";
                break;
            case Level::WARNING:
                name = "[WARNING]:";
                break;
            case Level::ERROR:
                name = "[ERROR]:";
                break;
            default:
                sys::unreachable();
        }
        return fmt::format_to(ctx.out(), "{:11}", name);
    }
};

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
template <>
struct std::fmt::formatter<Level> : public Formatter<Level> {};
#else
template <>
struct stdlib::fmt::formatter<Level> : public Formatter<Level> {};
#endif
