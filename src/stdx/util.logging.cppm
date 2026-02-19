/**
 * @file Level.cppm
 * @module stdx:util.logging
 * @brief Definition of the logging library
 *
 * This file contains the implementation of the logging library, which provides a simple logging framework.
 */

module;

#include "Macros.hpp"

export module stdx:util.logging;

import :main;

using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::fmt::FormatContext;
using stdx::fmt::FormatParseContext;
using stdx::fmt::FormatString;
using stdx::fmt::Formatter;
using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::IOS;
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::io::Stderr;
using stdx::io::Stdout;
using stdx::mem::SharedPointer;
using stdx::mem::UniquePointer;
using stdx::meta::SourceLocation;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;
using stdx::time::Instant;
using stdx::time::LocalTime;
using stdx::time::SystemClock;
using stdx::time::temporal::Seconds;

using namespace stdx::core;

/**
 * @namespace stdx::util::logging
 * @brief Wrapper namespace for standard library extension utility operations.
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

template <>
struct stdx::fmt::Formatter<Level> {
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
                stdx::sys::unreachable();
        }
        return stdx::fmt::format_to(ctx.out(), "{:11}", name);
    }
};

template <>
struct stdx::fmt::formatter<Level> : public Formatter<Level> {};

/**
 * @namespace stdx::util::logging
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace stdx::util::logging {

/**
 * @interface LogSink
 * @brief Abstract base class for log output destinations.
 */
class ILogSink {
public:
    virtual ~ILogSink() = default;

    /**
     * @brief Write a formatted log message to the sink.
     * 
     * @param timestamp The timestamp string
     * @param level The log level
     * @param logger_name The name of the logger
     * @param message The formatted message
     * @param enable_source_location Whether to include source location in output
     * @param location The source location (if enabled)
     */
    virtual void write(
        StringView timestamp,
        Level level,
        StringView logger_name,
        StringView message, 
        bool enable_source_location = false, 
        const SourceLocation& location = SourceLocation::current()
    ) = 0;

    /**
     * @brief Flush any buffered output.
     */
    virtual void flush() = 0;
};

/**
 * @class FileSink
 * @brief Log sink that writes to a file.
 *
 * @implements ILogSink
 */
class FileSink final: public ILogSink {
private:
    UniquePointer<OutputFileStream> file;
    mutable Mutex mutex;
public:
    /**
     * @brief Constructor that opens a file for logging.
     * 
     * @param path Path to the log file
     * @param mode Open mode (default: APPEND)
     * @throws IOException
     */
    explicit FileSink(StringView path, OpenMode::Self mode = OpenMode::APPEND) throws (IOException) {
        file = stdx::mem::make_unique<OutputFileStream>(Path(path), mode);
        if (!file->is_open()) {
            throw IOException("Failed to open log file");
        }
    }

    void write(
        StringView timestamp,
        Level level,
        StringView logger_name,
        StringView message,
        bool enable_source_location = false,
        const SourceLocation& location = SourceLocation::current()
    ) override {
        ScopedLock<Mutex> lock(mutex);
        if (enable_source_location) {
            stdx::io::println(
                *file, "[{}] {} [{}] [{}:{}:{}]: {}", 
                timestamp, level, logger_name, 
                location.file_name(), location.line(), location.function_name(),
                message
            );
        } else {
            stdx::io::println(*file, "[{}] {} [{}]: {}", timestamp, level, logger_name, message);
        }
    }

    void flush() override {
        ScopedLock<Mutex> lock(mutex);
        file->flush();
    }
};

/**
 * @class ConsoleSink
 * @brief Log sink that writes to console (stdout/stderr).
 *
 * @implements ILogSink
 */
class ConsoleSink final: public ILogSink {
private:
    bool use_stderr;
    mutable Mutex mutex;
public:
    /**
     * @brief Constructor.
     * 
     * @param toStderr If true, write to stderr; otherwise stdout
     */
    explicit ConsoleSink(bool to_stderr = true):
        use_stderr{to_stderr} {}

    void write(
        StringView timestamp,
        Level level,
        StringView logger_name,
        StringView message,
        bool enable_source_location = false,
        const SourceLocation& location = SourceLocation::current()
    ) override {
        ScopedLock<Mutex> lock(mutex);
        if (enable_source_location) {
            if (use_stderr) {
                stdx::io::println(
                    Stderr, "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, logger_name,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            } else {
                stdx::io::println(
                    Stdout, "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, logger_name,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            }
        } else {
            if (use_stderr) {
                stdx::io::println(Stderr, "[{}] {} [{}]: {}", timestamp, level, logger_name, message);
            } else {
                stdx::io::println(Stdout, "[{}] {} [{}]: {}", timestamp, level, logger_name, message);
            }
        }
    }

    void flush() override {
        ScopedLock<Mutex> lock(mutex);
        if (use_stderr) {
            stdx::io::fflush(Stderr);
        } else {
            stdx::io::fflush(Stdout);
        }
    }
};

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
        Instant<SystemClock> now = SystemClock::now();
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

/**
 * @class LoggerFactory
 * @brief Factory for creating and managing logger instances.
 * 
 * The LoggerFactory is a singleton that creates logger instances by name
 * and manages global sinks that all loggers can use.
 */
class LoggerFactory final {
private:
    HashMap<String, SharedPointer<Logger>> loggers;
    Vector<SharedPointer<ILogSink>> globalSinks;
    mutable Mutex mutex;
    Level defaultLevel = Level::DEBUG;
    bool enableSourceLocation = false;

    /**
     * @brief Private constructor for singleton pattern.
     */
    LoggerFactory() = default;

    /**
     * @brief Private destructor.
     */
    ~LoggerFactory() = default;
public:
    /**
     * @brief Deleted copy constructor.
     */
    LoggerFactory(const LoggerFactory&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    LoggerFactory(LoggerFactory&&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    LoggerFactory& operator=(const LoggerFactory&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    LoggerFactory& operator=(LoggerFactory&&) = delete;

    /**
     * @brief Get the singleton instance.
     * 
     * @return Reference to the singleton instance
     */
    [[nodiscard]]
    static LoggerFactory& instance() noexcept {
        static LoggerFactory lf;
        return lf;
    }

    /**
     * @brief Add a global sink that all loggers will use.
     * 
     * @param sink The sink to add
     */
    LoggerFactory& add_global_sink(SharedPointer<ILogSink> sink) {
        ScopedLock<Mutex> lock(mutex);
        globalSinks.push_back(sink);
        
        for (auto& [_, logger]: loggers) {
            logger->add_sink(sink);
        }
        return *this;
    }

    /**
     * @brief Set the default log level for new loggers.
     * 
     * @param level The default level
     */
    LoggerFactory& of_default_level(Level level) noexcept {
        ScopedLock<Mutex> lock(mutex);
        defaultLevel = level;
        return *this;
    }

    /**
     * @brief Enable or disable source location tracking for all loggers.
     * 
     * @param enable Whether to enable source location tracking
     */
    LoggerFactory& with_source_location(bool enable) noexcept {
        ScopedLock<Mutex> lock(mutex);
        enableSourceLocation = enable;
        return *this;
    }

    /**
     * @brief Get or create a logger with the given name.
     * 
     * @param name The logger name (typically class name)
     * @return Shared pointer to the logger
     */
    [[nodiscard]]
    SharedPointer<Logger> of(StringView name) {
        ScopedLock<Mutex> lock(mutex);
        
        String key(name);
        if (auto it = loggers.find(key); it != loggers.end()) {
            return it->second;
        }

        SharedPointer<Logger> logger = stdx::mem::make_shared<Logger>(key, defaultLevel, enableSourceLocation);
        
        for (const SharedPointer<ILogSink>& sink: globalSinks) {
            logger->add_sink(sink);
        }

        loggers[key] = logger;
        return logger;
    }

    /**
     * @brief Initialise the logging system with default file sink.
     * 
     * @param logFilePath Path to the log file
     * @param enableConsole Whether to also log to console
     */
    void init(StringView logFilePath = "./userdata/debug.txt", bool enableConsole = false) {
        stdx::fs::create_directories("./userdata");
        SharedPointer<FileSink> fileSink = stdx::mem::make_shared<FileSink>(logFilePath, OpenMode::TRUNCATE);
        add_global_sink(fileSink);

        if (enableConsole) {
            SharedPointer<ConsoleSink> consoleSink = stdx::mem::make_shared<ConsoleSink>(true);
            add_global_sink(consoleSink);
        }

        SharedPointer<Logger> initLogger = of("LoggerFactory");
        initLogger->log(Level::INFO, "====================BEGIN DEBUG LOG====================");
        initLogger->log(Level::INFO, "Logging system initialised");
        initLogger->log(Level::INFO, "=======================================================");
    }

    /**
     * @brief Flush all loggers.
     */
    void flush_all() {
        ScopedLock<Mutex> lock(mutex);
        for (const SharedPointer<ILogSink>& sink: globalSinks) {
            sink->flush();
        }
    }
};

}
