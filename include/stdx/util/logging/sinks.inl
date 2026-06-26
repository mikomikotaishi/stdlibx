#pragma once

using stdx::debug::SourceLocation;
using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;

/**
 * @namespace stdx::util::logging
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util::logging {

/**
 * @interface LogSink
 * @brief Abstract base class for log output destinations.
 */
class LogSink {
public:
    virtual ~LogSink() = default;

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
     * @brief Write a message verbatim, without timestamp/level/name decoration.
     *
     * @param message The message to write as-is
     */
    virtual void write_raw(StringView message) = 0;

    /**
     * @brief Flush any buffered output.
     */
    virtual void flush() = 0;
};

/**
 * @class FileSink
 * @brief Log sink that writes to a file.
 *
 * @implements LogSink
 */
class FileSink final: public LogSink {
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
    THROWS(IOException)
    explicit FileSink(const Path& path, OpenMode::Self mode = OpenMode::APPEND) {
        file = Pointers::unique<OutputFileStream>(path, mode);
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

    void write_raw(StringView message) override {
        ScopedLock<Mutex> lock(mutex);
        stdx::io::println(*file, "{}", message);
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
 * @implements LogSink
 */
class ConsoleSink final: public LogSink {
private:
    bool use_stderr;
    mutable Mutex mutex;
public:
    /**
     * @brief Constructor.
     * 
     * @param to_stderr If true, write to stderr; otherwise stdout
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
                System::err.println(
                    "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, logger_name,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            } else {
                System::out.println(
                    "[{}] {} [{}] [{}:{}:{}]: {}", 
                    timestamp, level, logger_name,
                    location.file_name(), location.line(), location.function_name(),
                    message
                );
            }
        } else {
            if (use_stderr) {
                System::err.println("[{}] {} [{}]: {}", timestamp, level, logger_name, message);
            } else {
                System::out.println("[{}] {} [{}]: {}", timestamp, level, logger_name, message);
            }
        }
    }

    void write_raw(StringView message) override {
        ScopedLock<Mutex> lock(mutex);
        if (use_stderr) {
            System::err.println("{}", message);
        } else {
            System::out.println("{}", message);
        }
    }

    void flush() override {
        ScopedLock<Mutex> lock(mutex);
        if (use_stderr) {
            stdx::io::cstdio::fflush(System::err);
        } else {
            stdx::io::cstdio::fflush(System::out);
        }
    }
};

}
