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

export module stdx:util.logging.Logger;

import :core;
import :collections;
import :fmt;
import :mem;
import :meta;
import :time;
import :util.logging.Level;
import :util.logging.Sinks;
import :util.utility;

using stdx::collections::Vector;
using stdx::fmt::FormatString;
using stdx::mem::SharedPointer;
using stdx::meta::SourceLocation;
using stdx::time::LocalTime;
using stdx::time::SystemClock;
using stdx::time::TimePoint;
using stdx::time::temporal::Seconds;

/**
 * @namespace stdx::util::logging
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace stdx::util::logging {

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
        LocalTime<Seconds> currentTime = stdx::time::current_zone()->to_local(stdx::time::floor<Seconds>(now));
        return stdx::fmt::format("{:%Y-%m-%d %H:%M:%S}", currentTime);
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
        sinks.emplace_back(stdx::util::move(sink));
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
     */
    template <typename... Args>
    void log(Level level, const FormatString<Args...>& fmt, Args&&... args) const {
        log(level, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a message at the specified level with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param level The log level
     * @param fmt Format string
     * @param location Source location
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void log(
        Level level,
        const FormatString<Args...>& fmt,
        const SourceLocation& location,
        Args&&... args
    ) const {
        if (stdx::util::to_underlying(level) < stdx::util::to_underlying(minLevel)) {
            return;
        }

        String message = stdx::fmt::format(fmt, stdx::util::forward<Args>(args)...);
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
     */
    template <typename... Args>
    void trace(const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::TRACE, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a diagnostic/tracing message with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param location Source location
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void trace(const SourceLocation& location, const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::TRACE, fmt, location, stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a debug message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void debug(const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::DEBUG, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a debug message with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param location Source location
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void debug(const SourceLocation& location, const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::DEBUG, fmt, location, stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log an info message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void info(const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::INFO, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log an info message with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param location Source location
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void info(const SourceLocation& location, const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::INFO, fmt, location, stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a warning message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void warn(const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::WARNING, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log a warning message with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param location Source location
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void warn(const SourceLocation& location, const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::WARNING, fmt, location, stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log an error message.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void error(const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::ERROR, fmt, SourceLocation::current(), stdx::util::forward<Args>(args)...);
    }

    /**
     * @brief Log an error message with explicit source location.
     *
     * @tparam Args Template parameter pack for format arguments
     * @param location Source location
     * @param fmt Format string
     * @param args Arguments for the format string
     */
    template <typename... Args>
    void error(const SourceLocation& location, const FormatString<Args...>& fmt, Args&&... args) const {
        log(Level::ERROR, fmt, location, stdx::util::forward<Args>(args)...);
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
