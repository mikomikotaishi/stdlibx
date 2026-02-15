/**
 * @file Sinks.cppm
 * @module stdx:util.logging.Sinks
 * @brief Definition of log sink interfaces and implementations.
 *
 * This file contains the base ILogSink interface and concrete implementations
 * for different output targets (file, console, etc.).
 */

module;

#include "Macros.hpp"

export module stdx:util.logging.Sinks;

import :core;
import :fs;
import :io;
import :mem;
import :meta;
import :sync;
import :util.logging.Level;

using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::IOS;
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::io::Stderr;
using stdx::io::Stdout;
using stdx::mem::UniquePointer;
using stdx::meta::SourceLocation;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;

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

}
