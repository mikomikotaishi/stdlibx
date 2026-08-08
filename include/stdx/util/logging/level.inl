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
    /**
     * @brief Formats a Level as its short display name by default ("Info"),
     * as its qualified enumerator name under the '?' flag ("Level::INFO"),
     * or as its original bracketed, all-caps form under the '!' flag
     * ("[INFO]:").
     */
    template <>
    struct Formatter<Level> {
    private:
        Formatter<StringView> _inner; ///< Parses/applies the standard fill/align/width once '?'/'!' are stripped out.
        String _spec; ///< The spec with '?'/'!' removed, owned here so _inner's parsed iterators stay valid into format().
        bool _debug = false; ///< Whether '?' appeared anywhere in the spec.
        bool _bracketed = false; ///< Whether '!' appeared anywhere in the spec.
    public:
        [[nodiscard]]
        THROWS(FormatException)
        constexpr auto parse(FormatParseContext& ctx) {
            auto it = ctx.begin();
            const auto end = ctx.end();
            while (it != end && *it != '}') {
                if (*it == '?') {
                    _debug = true;
                } else if (*it == '!') {
                    _bracketed = true;
                } else {
                    _spec.push_back(*it);
                }
                ++it;
            }

            FormatParseContext inner_ctx(_spec);
            _inner.parse(inner_ctx);

            return it;
        }

        auto format(Level lvl, FormatContext& ctx) const {
            StringView name;
            if (_bracketed) {
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
            } else if (_debug) {
                switch (lvl) {
                    case Level::TRACE:
                        name = "Level::TRACE";
                        break;
                    case Level::DEBUG:
                        name = "Level::DEBUG";
                        break;
                    case Level::INFO:
                        name = "Level::INFO";
                        break;
                    case Level::WARNING:
                        name = "Level::WARNING";
                        break;
                    case Level::ERROR:
                        name = "Level::ERROR";
                        break;
                }
            } else {
                switch (lvl) {
                    case Level::TRACE:
                        name = "Trace";
                        break;
                    case Level::DEBUG:
                        name = "Debug";
                        break;
                    case Level::INFO:
                        name = "Info";
                        break;
                    case Level::WARNING:
                        name = "Warning";
                        break;
                    case Level::ERROR:
                        name = "Error";
                        break;
                }
            }
            return _inner.format(name, ctx);
        }
    };
}

template <>
struct stdx::fmt::formatter<Level>: public Formatter<Level> {};
