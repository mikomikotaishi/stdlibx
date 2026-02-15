/**
 * @file LoggerFactory.cppm
 * @module stdx:util.logging.LoggerFactory
 * @brief Implementation of the LoggerFactory singleton.
 *
 * This file contains the LoggerFactory class which creates and manages
 * logger instances.
 */

module;

#if defined(__GNUC__) && !defined(__clang__)
#include <memory>
#endif

#include <string>

export module stdx:util.logging.LoggerFactory;

import :core;
import :collections;
import :fs;
import :io;
import :mem;
import :sync;
import :util.logging.Level;
import :util.logging.Logger;
import :util.logging.Sinks;

using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::io::OpenMode;
using stdx::mem::SharedPointer;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;

using namespace stdx::core;

/**
 * @namespace stdx::util::logging
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace stdx::util::logging {

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
