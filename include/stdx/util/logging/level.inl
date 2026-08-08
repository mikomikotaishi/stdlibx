#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::util::logging
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util::logging {
    /**
     * @enum SourceLocationFormat
     * @brief Controls the source-location fields written by log sinks.
     */
    enum class SourceLocationFormat: u8 {
        NONE, ///< No source location information.
        FILE_LINE, ///< File name and line number only.
        FILE_LINE_FUNCTION, ///< File name, line number, and function name.
        FULL, ///< Full file path, line, and compiler-provided function name.
    };

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
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(Level lvl, FormatContext& ctx) const {
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
struct stdx::fmt::formatter<Level>: public Formatter<Level> {};
