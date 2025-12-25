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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util.logging.Sinks;

import std;
#else
export module stdlibx:util.logging.Sinks;

import stdlib;
#endif

import :util.logging.Level;

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
using std::fs::Path;
using std::io::IOException;
using std::io::IOS;
using std::io::OpenMode;
using std::io::OutputFileStream;
using std::io::Stderr;
using std::io::Stdout;
using std::mem::UniquePointer;
using std::meta::SourceLocation;
using std::sync::Mutex;
using std::sync::ScopedLock;

namespace io = std::io;
namespace fs = std::fs;
namespace mem = std::mem;
#else
using stdlib::fs::Path;
using stdlib::io::IOException;
using stdlib::io::IOS;
using stdlib::io::OpenMode;
using stdlib::io::OutputFileStream;
using stdlib::io::Stderr;
using stdlib::io::Stdout;
using stdlib::mem::UniquePointer;
using stdlib::meta::SourceLocation;
using stdlib::sync::Mutex;
using stdlib::sync::ScopedLock;

namespace io = stdlib::io;
namespace fs = stdlib::fs;
namespace mem = stdlib::mem;
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
     * @param loggerName The name of the logger
     * @param message The formatted message
     * @param enableSourceLocation Whether to include source location in output
     * @param location The source location (if enabled)
     */
    virtual void write(
        StringView timestamp,
        Level level,
        StringView loggerName,
        StringView message, 
        bool enableSourceLocation = false, 
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
        file = mem::make_unique<OutputFileStream>(Path(path), mode);
        if (!file->is_open()) {
            throw IOException("Failed to open log file");
        }
    }

    void write(
        StringView timestamp,
        Level level,
        StringView loggerName,
        StringView message,
        bool enableSourceLocation = false,
        const SourceLocation& location = SourceLocation::current()
    ) override {
        ScopedLock<Mutex> lock(mutex);
        if (enableSourceLocation) {
            io::println(
                *file, "[{}] {} [{}] [{}:{}:{}]: {}", 
                timestamp, level, loggerName, 
                location.file_name(), location.line(), location.function_name(),
                message
            );
        } else {
            io::println(*file, "[{}] {} [{}]: {}", timestamp, level, loggerName, message);
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
    bool useStderr;
    mutable Mutex mutex;
public:
    /**
     * @brief Constructor.
     * 
     * @param toStderr If true, write to stderr; otherwise stdout
     */
    explicit ConsoleSink(bool toStderr = true):
        useStderr{toStderr} {}

    void write(
        StringView timestamp,
        Level level,
        StringView loggerName,StringView message,
        bool enableSourceLocation = false,
        const SourceLocation& location = SourceLocation::current()
    ) override {
        ScopedLock<Mutex> lock(mutex);
        if (enableSourceLocation) {
            if (useStderr) {
                io::println(
                    Stderr, "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, loggerName,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            } else {
                io::println(
                    Stdout, "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, loggerName,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            }
        } else {
            if (useStderr) {
                io::println(Stderr, "[{}] {} [{}]: {}", timestamp, level, loggerName, message);
            } else {
                io::println(Stdout, "[{}] {} [{}]: {}", timestamp, level, loggerName, message);
            }
        }
    }

    void flush() override {
        ScopedLock<Mutex> lock(mutex);
        if (useStderr) {
            io::fflush(Stderr);
        } else {
            io::fflush(Stdout);
        }
    }
};

}
