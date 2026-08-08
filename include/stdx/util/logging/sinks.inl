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
         * @brief Write a log message using a source-location format.
         * @param timestamp The timestamp string
         * @param level The log level
         * @param logger_name The name of the logger
         * @param message The formatted message
         * @param format The source-location format
         * @param location The source location (if enabled)
         *
         * The default keeps existing custom sinks source-compatible and uses
         * their full source-location behavior.
         */
        virtual void write(
            StringView timestamp,
            Level level,
            StringView logger_name,
            StringView message,
            SourceLocationFormat format,
            const SourceLocation& location = SourceLocation::current()
        ) = 0;

        /**
         * @brief Write a message verbatim, without timestamp/level/name decoration.
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
     * @implements LogSink
     */
    class FileSink final: public LogSink {
    private:
        UniquePointer<OutputFileStream> _file;
        mutable Mutex mutex;
    public:
        /**
         * @brief Constructor that opens a file for logging.
         * @param path Path to the log file
         * @param mode Open mode (default: APPEND)
         * @throws IOException
         */
        THROWS(IOException)
        explicit FileSink(const Path& path, OpenMode::Self mode = OpenMode::APPEND):
            _file{Pointers::unique<OutputFileStream>(path, mode)} {
            if (!_file->is_open()) {
                throw IOException("Failed to open log file");
            }
        }

        void write(
            StringView timestamp,
            Level level,
            StringView logger_name,
            StringView message,
            SourceLocationFormat format,
            const SourceLocation& location = SourceLocation::current()
        ) override {
            ScopedLock<Mutex> lock(mutex);
            switch (format) {
                case SourceLocationFormat::NONE:
                    stdx::io::println(*_file, "[{}] {} [{}]: {}", timestamp, level, logger_name, message);
                    break;
                case SourceLocationFormat::FILE_LINE:
                    stdx::io::println(
                        *_file, "[{}] {} [{}] [{}:{}]: {}",
                        timestamp, level, logger_name,
                        Path(location.file_name()).filename(), location.line(), message
                    );
                    break;
                case SourceLocationFormat::FILE_LINE_FUNCTION:
                    stdx::io::println(
                        *_file, "[{}] {} [{}] [{}:{}:{}]: {}",
                        timestamp, level, logger_name,
                        Path(location.file_name()).filename(), location.line(),
                        location.function_name(), message
                    );
                    break;
                case SourceLocationFormat::FULL:
                    stdx::io::println(
                        *_file, "[{}] {} [{}] [{}:{}:{}]: {}",
                        timestamp, level, logger_name,
                        location.file_name(), location.line(), location.function_name(),
                        message
                    );
                    break;
            }
        }

        void write_raw(StringView message) override {
            ScopedLock<Mutex> lock(mutex);
            stdx::io::println(*_file, "{}", message);
        }

        void flush() override {
            ScopedLock<Mutex> lock(mutex);
            _file->flush();
        }
    };

    /**
     * @class ConsoleSink
     * @brief Log sink that writes to console (stdout/stderr).
     * @implements LogSink
     */
    class ConsoleSink final: public LogSink {
    private:
        bool _use_stderr;
        mutable Mutex _mutex;
    public:
        /**
         * @brief Constructor.
         * @param to_stderr If true, write to stderr; otherwise stdout
         */
        explicit ConsoleSink(bool to_stderr = true):
            _use_stderr{to_stderr} {}

        void write(
            StringView timestamp,
            Level level,
            StringView logger_name,
            StringView message,
            SourceLocationFormat format,
            const SourceLocation& location = SourceLocation::current()
        ) override {
            ScopedLock<Mutex> lock(_mutex);
            auto output = [&](auto& stream) -> void {
                switch (format) {
                    case SourceLocationFormat::NONE:
                        stream.println("[{}] {} [{}]: {}", timestamp, level, logger_name, message);
                        break;
                    case SourceLocationFormat::FILE_LINE:
                        stream.println(
                            "[{}] {} [{}] [{}:{}]: {}",
                            timestamp, level, logger_name,
                            Path(location.file_name()).filename(), location.line(), message
                        );
                        break;
                    case SourceLocationFormat::FILE_LINE_FUNCTION:
                        stream.println(
                            "[{}] {} [{}] [{}:{}:{}]: {}",
                            timestamp, level, logger_name,
                            Path(location.file_name()).filename(), location.line(),
                            location.function_name(), message
                        );
                        break;
                    case SourceLocationFormat::FULL:
                        stream.println(
                            "[{}] {} [{}] [{}:{}:{}]: {}",
                            timestamp, level, logger_name,
                            location.file_name(), location.line(), location.function_name(),
                            message
                        );
                        break;
                }
            };

            if (_use_stderr) {
                output(System::err);
            } else {
                output(System::out);
            }
        }

        void write_raw(StringView message) override {
            ScopedLock<Mutex> lock(_mutex);
            if (_use_stderr) {
                System::err.println("{}", message);
            } else {
                System::out.println("{}", message);
            }
        }

        void flush() override {
            ScopedLock<Mutex> lock(_mutex);
            if (_use_stderr) {
                stdx::io::cstdio::fflush(System::err);
            } else {
                stdx::io::cstdio::fflush(System::out);
            }
        }
    };
}
