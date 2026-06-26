#pragma once

/**
 * @namespace stdx::util::logging
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util::logging {
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

using stdx::util::logging::Level;

namespace stdx::fmt {
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
            }
            return format_to(ctx.out(), "{:11}", name);
        }
    };
}

template <>
struct stdx::fmt::formatter<Level> : stdx::fmt::Formatter<Level> {};
