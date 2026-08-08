#pragma once

using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::debug::SourceLocation;
using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::OpenMode;
using stdx::meta::TypeIdentityType;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::sync::Mutex;
using stdx::sync::ScopedLock;

#ifdef __cpp_impl_reflection
using stdx::meta::reflect::Class;
using stdx::meta::reflect::ReflectableAsClass;
#endif

/**
 * @namespace stdx::util::logging
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util::logging {
    /**
     * @class LogFormat
     * @brief A format string that remembers where it was written.
     *
     * A logging call cannot take a defaulted SourceLocation parameter, because
     * the parameter pack for the format arguments has to come last and a
     * default cannot follow it. Calling SourceLocation::current() inside the
     * logging function instead captures this file, so every message reports the
     * logging library rather than the caller - an answer that looks right and
     * never is.
     *
     * Wrapping the format string moves the default into a constructor argument,
     * where it is evaluated at the call site. Spell the parameter
     * {@code LogFormat<TypeIdentityType<Args>...>} so it stays a non-deduced
     * context and {@code Args} continue to deduce from the trailing arguments.
     *
     * @tparam Args The format argument types.
     */
    template <typename... Args>
    class [[nodiscard]] LogFormat final {
    private:
        FormatString<Args...> _format; ///< The checked format string.
        SourceLocation _location; ///< Where the call was written.
    public:
        /**
         * @brief Captures @p format and the location of the call that wrote it.
         * @param format The format string, checked at compile time as usual.
         * @param location Defaulted, and therefore evaluated at the call site.
         */
        template <typename S>
            requires ConvertibleTo<const S&, StringView>
        consteval LogFormat(const S& format, SourceLocation location = SourceLocation::current()):
            _format{format}, _location{location} {}

        /**
         * @brief The format string.
         */
        [[nodiscard]]
        constexpr const FormatString<Args...>& format() const noexcept {
            return _format;
        }

        /**
         * @brief Where the logging call was written.
         */
        [[nodiscard]]
        constexpr const SourceLocation& location() const noexcept {
            return _location;
        }
    };

    /**
     * @class Logger
     * @brief Named logger instance that writes to configured sinks.
     */
    class Logger final {
    private:
        String _name;
        Vector<SharedPointer<LogSink>> _sinks;
        Level _min_level = Level::DEBUG;
        bool _enable_source_location =  false;
    public:
        /**
         * @brief Constructor.
         * @param logger_name The name of this logger
         * @param minimum_level Minimum log level to output (default: DEBUG)
         * @param enable_source_location Whether to capture source location (default: false)
         */
        explicit Logger(StringView name, Level minimum_level = Level::DEBUG, bool enable_source_location = false):
            _name{String(name)}, _min_level{minimum_level}, _enable_source_location{enable_source_location} {}

        /**
         * @brief Add a sink to this logger.
         * @param sink The sink to add
         */
        Logger& add_sink(SharedPointer<LogSink> sink) {
            _sinks.emplace_back(Ops::move(sink));
            return *this;
        }

        /**
         * @brief Set the minimum log level for this logger.
         * @param level The minimum level
         */
        Logger& of_level(Level level) noexcept {
            _min_level = level;
            return *this;
        }

        /**
         * @brief Get the logger name.
         * @return The logger name
         */
        [[nodiscard]]
        StringView name() const noexcept {
            return _name;
        }

        /**
         * @brief Log a message at the specified level.
         * @tparam Args Template parameter pack for format arguments
         * @param level The log level
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void log(Level level, LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(level, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log a message at the specified level with explicit source location.
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
            if (Ops::to_underlying(level) < Ops::to_underlying(_min_level)) {
                return;
            }

            String message = Ops::fmt(fmt, Ops::forward<Args>(args)...);
            String timestamp = System::local_timestamp();

            for (const SharedPointer<LogSink>& sink: _sinks) {
                sink->write(timestamp, level, _name, message, _enable_source_location, location);
            }
        }

        /**
         * @brief Log a diagnostic/tracing message.
         * @tparam Args Template parameter pack for format arguments
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void trace(LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(Level::TRACE, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log a diagnostic/tracing message with explicit source location.
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
         * @tparam Args Template parameter pack for format arguments
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void debug(LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(Level::DEBUG, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log a debug message with explicit source location.
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
         * @tparam Args Template parameter pack for format arguments
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void info(LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(Level::INFO, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log an info message with explicit source location.
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
         * @tparam Args Template parameter pack for format arguments
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void warn(LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(Level::WARNING, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log a warning message with explicit source location.
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
         * @tparam Args Template parameter pack for format arguments
         * @param fmt Format string
         * @param args Arguments for the format string
         */
        template <typename... Args>
        void error(LogFormat<TypeIdentityType<Args>...> fmt, Args&&... args) const {
            log(Level::ERROR, fmt.format(), fmt.location(), Ops::forward<Args>(args)...);
        }

        /**
         * @brief Log an error message with explicit source location.
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
            for (const SharedPointer<LogSink>& sink: _sinks) {
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

            Vector<SharedPointer<LogSink>> _sinks;
            Vector<FileSpec> _files;
            Level _default_level = Level::DEBUG;
            bool _enable_source_location = false;
            bool _console = false;
            bool _console_stderr = true;
            bool _banner = false;
        public:
            Builder() = default;

            /**
             * @brief Set the default log level for the factory's loggers.
             * @param level The default level
             */
            Builder& of_default_level(Level level) noexcept {
                _default_level = level;
                return *this;
            }

            /**
             * @brief Enable or disable source location tracking for the factory's loggers.
             * @param enable Whether to enable source location tracking
             */
            Builder& trace_source(bool enable = true) noexcept {
                _enable_source_location = enable;
                return *this;
            }

            /**
             * @brief Add a sink that all of the factory's loggers will use.
             * @param sink The sink to add
             */
            Builder& with_sink(SharedPointer<LogSink> sink) {
                _sinks.emplace_back(Ops::move(sink));
                return *this;
            }

            /**
             * @brief Log to a file, opened when the factory is built.
             * @param path Path to the log file
             * @param mode Open mode (default: TRUNCATE)
             *
             * Missing parent directories are created at build time.
             */
            Builder& with_file(Path path, OpenMode::Self mode = OpenMode::TRUNCATE) {
                _files.emplace_back(Ops::move(path), mode);
                return *this;
            }

            /**
             * @brief Log to the console.
             * @param to_stderr If true, write to stderr; otherwise stdout
             */
            Builder& with_console(bool to_stderr = true) noexcept {
                _console = true;
                _console_stderr = to_stderr;
                return *this;
            }

            /**
             * @brief Write a begin-of-log banner to every sink when the factory is built.
             * @param enable Whether to write the banner
             */
            Builder& with_banner(bool enable = true) noexcept {
                _banner = enable;
                return *this;
            }

            /**
             * @brief Builds the configured factory.
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
             * @return A {@code SharedPointer} to the new factory.
             * @throws IOException if a log file cannot be opened.
             *
             * Convenient for injector-owned factories and other shared-ownership use.
             */
            [[nodiscard]]
            THROWS(IOException)
            SharedPointer<LoggerFactory> build_shared() const {
                return SharedPointer<LoggerFactory>(new LoggerFactory(*this));
            }
        };
    private:
        HashMap<String, SharedPointer<Logger>> _loggers;
        Vector<SharedPointer<LogSink>> _global_sinks;
        mutable Mutex _mutex;
        Level _default_level;
        bool _enable_source_location;

        /**
         * @brief Constructs the factory from a builder's configuration.
         * @throws IOException if a log file cannot be opened.
         *
         * Opens the builder's log files (creating missing parent directories)
         * and writes the banner if requested.
         */
        THROWS(IOException)
        explicit LoggerFactory(const Builder& builder):
            _global_sinks{builder._sinks},
            _default_level{builder._default_level},
            _enable_source_location{builder._enable_source_location} {
            for (const Builder::FileSpec& file: builder._files) {
                if (Path parent = file.path.parent_path(); !parent.empty()) {
                    stdx::fs::create_directories(parent);
                }
                _global_sinks.push_back(Pointers::shared<FileSink>(file.path, file.mode));
            }

            if (builder._console) {
                _global_sinks.push_back(Pointers::shared<ConsoleSink>(builder._console_stderr));
            }

            if (builder._banner) {
                String init_message = Ops::fmt("Logging system initialized (at {})", System::local_timestamp());
                for (const SharedPointer<LogSink>& sink: _global_sinks) {
                    sink->write_raw("====================BEGIN DEBUG LOG====================");
                    sink->write_raw(init_message);
                    sink->write_raw("=======================================================");
                }
            }
        }
    public:
        ~LoggerFactory() = default;

        LoggerFactory(const LoggerFactory&) = DELETE_METHOD("LoggerFactory is not copyable.");
        LoggerFactory& operator=(const LoggerFactory&) = DELETE_METHOD("LoggerFactory is not copyable.");
        LoggerFactory(LoggerFactory&&) = DELETE_METHOD("LoggerFactory is not movable.");
        LoggerFactory& operator=(LoggerFactory&&) = DELETE_METHOD("LoggerFactory is not movable.");

        /**
         * @brief Get or create a logger with the given name.
         * @param name The logger name (typically class name)
         * @return Shared pointer to the logger
         */
        [[nodiscard]]
        SharedPointer<Logger> of(StringView name) {
            ScopedLock<Mutex> lock(_mutex);

            String key(name);
            if (auto it = _loggers.find(key); it != _loggers.end()) {
                return it->second;
            }

            SharedPointer<Logger> logger = Pointers::shared<Logger>(
                key,
                _default_level,
                _enable_source_location
            );

            for (const SharedPointer<LogSink>& sink: _global_sinks) {
                logger->add_sink(sink);
            }

            _loggers[key] = logger;
            return logger;
        }

        #ifdef __cpp_impl_reflection
        /**
         * @brief Get or create a logger named after the type {@code T}.
         * @tparam T The class type to name the logger after
         * @return Shared pointer to the logger
         *
         * Derives the logger name from {@code T}'s own (unqualified) identifier
         * via reflection, so {@code factory.of<MyClass>()} is equivalent to
         * {@code factory.of("MyClass")} but stays correct across renames.
         */
        template <ReflectableAsClass T>
        [[nodiscard]]
        SharedPointer<Logger> of() {
            return of(Class<T>().name().value());
        }
        #endif

        /**
         * @brief Flush all loggers.
         */
        void flush_all() {
            ScopedLock<Mutex> lock(_mutex);
            for (const SharedPointer<LogSink>& sink: _global_sinks) {
                sink->flush();
            }
        }
    };
}
