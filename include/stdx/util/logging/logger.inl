#pragma once

using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::debug::SourceLocation;
using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::OpenMode;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;

#ifdef __cpp_impl_reflection
using stdx::meta::reflect::Class;
using stdx::meta::reflect::ReflectableClass;
#endif

/**
 * @namespace stdx::util::logging
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util::logging {
    /**
     * @class Logger
     * @brief Named logger instance that writes to configured sinks.
     */
    class Logger final {
    private:
        String logger_name;
        Vector<SharedPointer<LogSink>> sinks;
        Level min_level = Level::DEBUG;
        bool enable_source_location =  false;
    public:
        /**
         * @brief Constructor.
         *
         * @param logger_name The name of this logger
         * @param minimum_level Minimum log level to output (default: DEBUG)
         * @param enable_source_location Whether to capture source location (default: false)
         */
        explicit Logger(StringView logger_name, Level minimum_level = Level::DEBUG, bool enable_source_location = false):
            logger_name{String(logger_name)}, min_level{minimum_level}, enable_source_location{enable_source_location} {}

        /**
         * @brief Add a sink to this logger.
         *
         * @param sink The sink to add
         */
        Logger& add_sink(SharedPointer<LogSink> sink) {
            sinks.emplace_back(Ops::move(sink));
            return *this;
        }

        /**
         * @brief Set the minimum log level for this logger.
         *
         * @param level The minimum level
         */
        Logger& of_level(Level level) noexcept {
            min_level = level;
            return *this;
        }

        /**
         * @brief Get the logger name.
         *
         * @return The logger name
         */
        [[nodiscard]]
        StringView name() const noexcept {
            return logger_name;
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
            log(level, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            if (Ops::to_underlying(level) < Ops::to_underlying(min_level)) {
                return;
            }

            String message = stdx::fmt::format(fmt, Ops::forward<Args>(args)...);
            String timestamp = System::current_time_formatted();

            for (const SharedPointer<LogSink>& sink: sinks) {
                sink->write(timestamp, level, logger_name, message, enable_source_location, location);
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
            log(Level::TRACE, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            log(Level::TRACE, fmt, location, Ops::forward<Args>(args)...);
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
            log(Level::DEBUG, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            log(Level::DEBUG, fmt, location, Ops::forward<Args>(args)...);
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
            log(Level::INFO, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            log(Level::INFO, fmt, location, Ops::forward<Args>(args)...);
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
            log(Level::WARNING, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            log(Level::WARNING, fmt, location, Ops::forward<Args>(args)...);
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
            log(Level::ERROR, fmt, SourceLocation::current(), Ops::forward<Args>(args)...);
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
            log(Level::ERROR, fmt, location, Ops::forward<Args>(args)...);
        }

        /**
         * @brief Flush all sinks.
         */
        void flush() const {
            for (const SharedPointer<LogSink>& sink: sinks) {
                sink->flush();
            }
        }
    };

    /**
     * @class LoggerFactory
     * @brief Factory for creating and managing logger instances.
     *
     * Creates logger instances by name that share the factory's sinks and
     * settings. Configuration is fixed at construction through
     * {@code LoggerFactory::Builder}, so every logger the factory creates sees
     * identical settings. There is no process-wide instance: the application
     * owns its factories, typically one built in {@code main()} (or provided by
     * a {@code stdx::inject} configuration) and shared by reference or
     * {@code SharedPointer}.
     *
     * @code{.cpp}
     * LoggerFactory logging = LoggerFactory::Builder()
     *     .of_default_level(Level::INFO)
     *     .with_file("./logs/app.log")
     *     .with_console()
     *     .build();
     *
     * SharedPointer<Logger> logger = logging.of("Main");
     * @endcode
     */
    class LoggerFactory final {
    public:
        /**
         * @class LoggerFactory::Builder
         * @brief Fluent, single-use configuration for a {@code LoggerFactory}.
         *
         * Collects sinks and settings, then produces the factory with
         * {@code build()} (or {@code build_shared()}). Nothing is opened or written
         * until then; a builder may be discarded without side effects.
         */
        class Builder {
        private:
            friend class LoggerFactory;

            /**
             * @struct FileSpec
             * @brief A log file to open at build time.
             */
            struct FileSpec {
                Path path; ///< Path to the log file.
                OpenMode::Self mode; ///< Open mode for the log file.
            };

            Vector<SharedPointer<LogSink>> sinks;
            Vector<FileSpec> files;
            Level default_level = Level::DEBUG;
            bool enable_source_location = false;
            bool console = false;
            bool console_stderr = true;
            bool banner = false;
        public:
            Builder() = default;

            /**
             * @brief Set the default log level for the factory's loggers.
             *
             * @param level The default level
             */
            Builder& of_default_level(Level level) noexcept {
                default_level = level;
                return *this;
            }

            /**
             * @brief Enable or disable source location tracking for the factory's loggers.
             *
             * @param enable Whether to enable source location tracking
             */
            Builder& trace_source(bool enable = true) noexcept {
                enable_source_location = enable;
                return *this;
            }

            /**
             * @brief Add a sink that all of the factory's loggers will use.
             *
             * @param sink The sink to add
             */
            Builder& with_sink(SharedPointer<LogSink> sink) {
                sinks.emplace_back(Ops::move(sink));
                return *this;
            }

            /**
             * @brief Log to a file, opened when the factory is built.
             *
             * Missing parent directories are created at build time.
             *
             * @param path Path to the log file
             * @param mode Open mode (default: TRUNCATE)
             */
            Builder& with_file(Path path, OpenMode::Self mode = OpenMode::TRUNCATE) {
                files.emplace_back(Ops::move(path), mode);
                return *this;
            }

            /**
             * @brief Log to the console.
             *
             * @param to_stderr If true, write to stderr; otherwise stdout
             */
            Builder& with_console(bool to_stderr = true) noexcept {
                console = true;
                console_stderr = to_stderr;
                return *this;
            }

            /**
             * @brief Write a begin-of-log banner to every sink when the factory is built.
             *
             * @param enable Whether to write the banner
             */
            Builder& with_banner(bool enable = true) noexcept {
                banner = enable;
                return *this;
            }

            /**
             * @brief Builds the configured factory.
             *
             * @return The new factory.
             * @throws IOException if a log file cannot be opened.
             */
            [[nodiscard]]
            THROWS(IOException)
            LoggerFactory build() const {
                return LoggerFactory(*this);
            }

            /**
             * @brief Builds the configured factory on the heap.
             *
             * Convenient for injector-owned factories and other shared-ownership use.
             *
             * @return A {@code SharedPointer} to the new factory.
             * @throws IOException if a log file cannot be opened.
             */
            [[nodiscard]]
            THROWS(IOException)
            SharedPointer<LoggerFactory> build_shared() const {
                return SharedPointer<LoggerFactory>(new LoggerFactory(*this));
            }
        };
    private:
        HashMap<String, SharedPointer<Logger>> loggers;
        Vector<SharedPointer<LogSink>> global_sinks;
        mutable Mutex mutex;
        Level default_level;
        bool enable_source_location;

        /**
         * @brief Constructs the factory from a builder's configuration.
         *
         * Opens the builder's log files (creating missing parent directories)
         * and writes the banner if requested.
         *
         * @throws IOException if a log file cannot be opened.
         */
        THROWS(IOException)
        explicit LoggerFactory(const Builder& builder):
            global_sinks{builder.sinks},
            default_level{builder.default_level},
            enable_source_location{builder.enable_source_location} {
            for (const Builder::FileSpec& file: builder.files) {
                if (Path parent = file.path.parent_path(); !parent.empty()) {
                    stdx::fs::create_directories(parent);
                }
                global_sinks.push_back(Pointers::shared<FileSink>(file.path, file.mode));
            }

            if (builder.console) {
                global_sinks.push_back(Pointers::shared<ConsoleSink>(builder.console_stderr));
            }

            if (builder.banner) {
                // The banner is written verbatim, outside the log-entry format.
                String init_message = stdx::fmt::format("Logging system initialized (at {})", System::current_time_formatted());
                for (const SharedPointer<LogSink>& sink: global_sinks) {
                    sink->write_raw("====================BEGIN DEBUG LOG====================");
                    sink->write_raw(init_message);
                    sink->write_raw("=======================================================");
                }
            }
        }
    public:
        ~LoggerFactory() = default;

        LoggerFactory(const LoggerFactory&) = delete("LoggerFactory is not copyable.");
        LoggerFactory& operator=(const LoggerFactory&) = delete("LoggerFactory is not copyable.");
        LoggerFactory(LoggerFactory&&) = delete("LoggerFactory is not movable.");
        LoggerFactory& operator=(LoggerFactory&&) = delete("LoggerFactory is not movable.");

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

            SharedPointer<Logger> logger = Pointers::shared<Logger>(
                key,
                default_level,
                enable_source_location
            );

            for (const SharedPointer<LogSink>& sink: global_sinks) {
                logger->add_sink(sink);
            }

            loggers[key] = logger;
            return logger;
        }

        #ifdef __cpp_impl_reflection
        /**
         * @brief Get or create a logger named after the type {@code T}.
         *
         * Derives the logger name from {@code T}'s own (unqualified) identifier
         * via reflection, so {@code factory.of<MyClass>()} is equivalent to
         * {@code factory.of("MyClass")} but stays correct across renames.
         *
         * @tparam T The class type to name the logger after
         * @return Shared pointer to the logger
         */
        template <ReflectableClass T>
        [[nodiscard]]
        SharedPointer<Logger> of() {
            return of(Class<T>().name().value());
        }
        #endif

        /**
         * @brief Flush all loggers.
         */
        void flush_all() {
            ScopedLock<Mutex> lock(mutex);
            for (const SharedPointer<LogSink>& sink: global_sinks) {
                sink->flush();
            }
        }
    };
}
