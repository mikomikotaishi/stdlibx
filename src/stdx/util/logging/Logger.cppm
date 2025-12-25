/**
 * @file Logger.cppm
 * @module stdx:util.logging.Logger
 * @brief Implementation of individual Logger instances.
 *
 * This file contains the Logger class which represents a named logger instance
 * that can write to multiple sinks.
 */

module;

#include <vector>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util.logging.Logger;

import std;
#else
export module stdlibx:util.logging.Logger;

import stdlib;
#endif

import core;

import :util.logging.Level;
import :util.logging.Sinks;

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
using std::collections::Vector;
using std::fmt::FormatString;
using std::mem::SharedPointer;
using std::meta::SourceLocation;
using std::time::LocalTime;
using std::time::SystemClock;
using std::time::TimePoint;
using std::time::temporal::Seconds;

namespace fmt = std::fmt;
namespace mem = std::mem;
namespace time = std::time;
#else
using stdlib::collections::Vector;
using stdlib::fmt::FormatString;
using stdlib::mem::SharedPointer;
using stdlib::meta::SourceLocation;
using stdlib::time::LocalTime;
using stdlib::time::SystemClock;
using stdlib::time::TimePoint;
using stdlib::time::temporal::Seconds;

namespace fmt = stdlib::fmt;
namespace mem = stdlib::mem;
namespace time = stdlib::time;
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
 * @class Logger
 * @brief Named logger instance that writes to configured sinks.
 */
class Logger final {
private:
    String loggerName;
    Vector<SharedPointer<ILogSink>> sinks;
    Level minLevel;
    bool enableSourceLocation;

    /**
     * @brief Get the current time as a string.
     * @return The current time formatted as a string.
     */
    [[nodiscard]]
    static String current_time_as_string() {
        TimePoint<SystemClock> now = SystemClock::now();
        LocalTime<Seconds> currentTime = time::current_zone()->to_local(time::floor<Seconds>(now));
        return fmt::format("{:%Y-%m-%d %H:%M:%S}", currentTime);
    }
public:
    /**
     * @brief Constructor.
     *
     * @param loggerName The name of this logger
     * @param minimumLevel Minimum log level to output (default: DEBUG)
     * @param enableSourceLocation Whether to capture source location (default: false)
     */
    explicit Logger(StringView loggerName, Level minimumLevel = Level::DEBUG, bool enableSourceLocation = false):
        loggerName{String(loggerName)}, minLevel{minimumLevel}, enableSourceLocation{enableSourceLocation} {}

    /**
     * @brief Add a sink to this logger.
     *
     * @param sink The sink to add
     */
    Logger& add_sink(SharedPointer<ILogSink> sink) {
        sinks.emplace_back(core::util::move(sink));
        return *this;
    }

    /**
     * @brief Set the minimum log level for this logger.
     *
     * @param level The minimum level
     */
    Logger& of_level(Level level) noexcept {
        minLevel = level;
        return *this;
    }

    /**
     * @brief Get the logger name.
     *
     * @return The logger name
     */
    [[nodiscard]]
    StringView name() const noexcept {
        return loggerName;
    }

    /**
     * @brief Log a message at the specified level.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param level The log level
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void log(
        Level level,
        const FormatString<Args...>& fmt,
        Args&&... args,
        const SourceLocation& location = SourceLocation::current()
    ) const {
        if (core::util::to_underlying(level) < core::util::to_underlying(minLevel)) {
            return;
        }

        String message = fmt::format(fmt, core::util::forward<Args>(args)...);
        String timestamp = current_time_as_string();

        for (const SharedPointer<ILogSink>& sink: sinks) {
            sink->write(timestamp, level, loggerName, message, enableSourceLocation, location);
        }
    }

    /**
     * @brief Log a diagnostic/tracing message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void trace(
        const FormatString<Args...>& fmt,
        Args&&... args,
        const SourceLocation& location = SourceLocation::current()
    ) const {
        log(Level::TRACE, fmt, core::util::forward<Args>(args)..., location);
    }

    /**
     * @brief Log a debug message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void debug(const FormatString<Args...>& fmt, Args&&... args,
              const SourceLocation& location = SourceLocation::current()) const {
        log(Level::DEBUG, fmt, core::util::forward<Args>(args)..., location);
    }

    /**
     * @brief Log an info message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void info(
        const FormatString<Args...>& fmt,
        Args&&... args,
        const SourceLocation& location = SourceLocation::current()
    ) const {
        log(Level::INFO, fmt, core::util::forward<Args>(args)..., location);
    }

    /**
     * @brief Log a warning message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void warn(
        const FormatString<Args...>& fmt,
        Args&&... args,
        const SourceLocation& location = SourceLocation::current()
    ) const {
        log(Level::WARNING, fmt, core::util::forward<Args>(args)..., location);
    }

    /**
     * @brief Log an error message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     * @param location Source location (automatically captured at call site)
     */
    template <typename... Args>
    void error(
        const FormatString<Args...>& fmt,
        Args&&... args,
        const SourceLocation& location = SourceLocation::current()
    ) const {
        log(Level::ERROR, fmt, core::util::forward<Args>(args)..., location);
    }

    /**
     * @brief Flush all sinks.
     */
    void flush() const {
        for (const SharedPointer<ILogSink>& sink: sinks) {
            sink->flush();
        }
    }
};

}
