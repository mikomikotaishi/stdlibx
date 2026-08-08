#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::debug::SourceLocation;
using stdx::fs::Path;
using stdx::io::InputFileStream;
using stdx::io::OpenMode;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::util::logging::ConsoleSink;
using stdx::util::logging::Level;
using stdx::util::logging::LogSink;
using stdx::util::logging::Logger;
using stdx::util::logging::LoggerFactory;
using stdx::util::logging::SourceLocationFormat;

using namespace stdx::test;

/**
 * @struct Entry
 * @brief One call that reached a sink.
 */
struct Entry {
    String timestamp; ///< The stamp the logger produced.
    Level level; ///< The level it was logged at.
    String logger; ///< The name of the logger that produced it.
    String message; ///< The formatted message.
    SourceLocationFormat format = SourceLocationFormat::NONE; ///< The format to present the source location as.
    String file; ///< The file the location named.
    u32 line = 0; ///< The line the location named.
    bool raw = false; ///< Whether it arrived through write_raw.
};

/**
 * @class MemorySink
 * @brief A sink that records instead of writing.
 *
 * Everything else that implements LogSink needs a file or a console to be
 * observed at all, which is why none of this was tested. This one records what
 * it was handed so the assertions can be about the logger's decisions rather
 * than about the shape of a line of text.
 */
class MemorySink final: public LogSink {
private:
    Vector<Entry> _entries; ///< Every call in arrival order.
    usize _flushes = 0; ///< How many times flush() was called.
public:
    void write(
        StringView timestamp,
        Level level,
        StringView logger_name,
        StringView message,
        SourceLocationFormat format = SourceLocationFormat::NONE,
        const SourceLocation& location = SourceLocation::current()
    ) override {
        _entries.push_back(Entry {
            .timestamp = String(timestamp),
            .level = level,
            .logger = String(logger_name),
            .message = String(message),
            .format = format,
            .file = String(location.file_name()),
            .line = location.line(),
            .raw = false,
        });
    }

    void write_raw(StringView message) override {
        Entry entry {
            .message = String(message),
            .raw = true,
        };
        _entries.push_back(Ops::move(entry));
    }

    void flush() override {
        ++_flushes;
    }

    [[nodiscard]]
    const Vector<Entry>& entries() const noexcept {
        return _entries;
    }

    [[nodiscard]]
    usize flushes() const noexcept {
        return _flushes;
    }
};

/**
 * @brief A logger writing to a fresh MemorySink, and that sink.
 */
struct Rig {
    SharedPointer<MemorySink> sink;
    Logger logger;
};

[[nodiscard]]
static Rig rig(Level minimum = Level::TRACE, SourceLocationFormat format = SourceLocationFormat::NONE) {
    SharedPointer<MemorySink> sink = Pointers::shared<MemorySink>();
    Logger logger("test", minimum, format);
    logger.add_sink(sink);
    return Rig {
        .sink = Ops::move(sink),
        .logger = Ops::move(logger),
    };
}

/**
 * @brief Tests that the minimum level drops everything below it and keeps the rest.
 *
 * The whole point of a level: the comparison is on the enumerator's underlying
 * value, so the declaration order in Level is load-bearing and a reordering
 * would silently invert the filter.
 */
void test_logging_level_filtering() {
    Rig r = rig(Level::WARNING);
    r.logger.trace("t");
    r.logger.debug("d");
    r.logger.info("i");
    r.logger.warn("w");
    r.logger.error("e");

    const Vector<Entry>& got = r.sink->entries();
    expect_eq(got.size(), 2uz, "only WARNING and above survive a WARNING minimum");
    if (got.size() == 2) {
        expect_eq(got[0].level, Level::WARNING, "the warning is kept");
        expect_eq(got[1].level, Level::ERROR, "and the error");
    }

    Rig all = rig(Level::TRACE);
    all.logger.trace("t");
    all.logger.error("e");
    expect_eq(all.sink->entries().size(), 2uz, "a TRACE minimum keeps everything");
}

/**
 * @brief Tests that each convenience method logs at its own level.
 */
void test_logging_levels_are_distinct() {
    Rig r = rig();
    r.logger.trace("a");
    r.logger.debug("b");
    r.logger.info("c");
    r.logger.warn("d");
    r.logger.error("e");

    const Vector<Entry>& got = r.sink->entries();
    require_eq(got.size(), 5uz, "every level reaches the sink");
    expect_eq(got[0].level, Level::TRACE, "trace() logs at TRACE");
    expect_eq(got[1].level, Level::DEBUG, "debug() logs at DEBUG");
    expect_eq(got[2].level, Level::INFO, "info() logs at INFO");
    expect_eq(got[3].level, Level::WARNING, "warn() logs at WARNING");
    expect_eq(got[4].level, Level::ERROR, "error() logs at ERROR");
}

/**
 * @brief Tests that format arguments are substituted, and the logger's name travels with the entry.
 */
void test_logging_formatting() {
    Rig r = rig();
    r.logger.info("{} plus {} is {}", 2, 3, 5);
    r.logger.warn("no arguments");

    const Vector<Entry>& got = r.sink->entries();
    require_eq(got.size(), 2uz, "both messages arrive");
    expect_eq(got[0].message, "2 plus 3 is 5", "arguments are substituted");
    expect_eq(got[1].message, "no arguments", "a message with no arguments is passed through");
    expect_eq(got[0].logger, "test", "the sink is told which logger produced it");
    expect(!got[0].timestamp.empty(), "and is given a timestamp");
}

/**
 * @brief Tests that of_level narrows an already-built logger.
 */
void test_logging_level_can_be_changed() {
    Rig r = rig(Level::TRACE);
    r.logger.trace("kept");
    r.logger.of_level(Level::ERROR);
    r.logger.trace("dropped");
    r.logger.error("kept too");

    const Vector<Entry>& got = r.sink->entries();
    expect_eq(got.size(), 2uz, "raising the minimum takes effect immediately");
    if (got.size() == 2) {
        expect_eq(got[1].message, "kept too", "and only the surviving message gets through");
    }
}

/**
 * @brief Tests that every attached sink sees every message.
 */
void test_logging_multiple_sinks() {
    SharedPointer<MemorySink> first = Pointers::shared<MemorySink>();
    SharedPointer<MemorySink> second = Pointers::shared<MemorySink>();

    Logger logger("multi", Level::TRACE);
    logger.add_sink(first).add_sink(second);
    logger.info("fan out");

    expect_eq(first->entries().size(), 1uz, "the first sink is written to");
    expect_eq(second->entries().size(), 1uz, "and so is the second");

    logger.flush();
    expect_eq(first->flushes(), 1uz, "flush reaches the first sink");
    expect_eq(second->flushes(), 1uz, "and the second");
}

/**
 * @brief Tests that a logger with no sinks is harmless.
 */
void test_logging_no_sinks() {
    Logger logger("orphan", Level::TRACE);
    expect_no_throw([&] -> void { logger.info("into the void"); }, "logging with no sink is a no-op");
    expect_no_throw([&] -> void { logger.flush(); }, "and so is flushing");
}

/**
 * @brief Tests that source location, when enabled, names the caller.
 *
 * The reason to enable it at all is to find out where a message came from. A
 * location that names the logging library instead is worse than none, because
 * it looks like an answer.
 */
void test_logging_source_location() {
    Rig off = rig(Level::TRACE, SourceLocationFormat::NONE);
    off.logger.info("no location");
    require_eq(off.sink->entries().size(), 1uz, "the message arrives");
    expect(off.sink->entries()[0].format == SourceLocationFormat::NONE, "location is NONE by default");

    Rig on = rig(Level::TRACE, SourceLocationFormat::FULL);
    const SourceLocation here = SourceLocation::current();
    on.logger.info("with location");

    require_eq(on.sink->entries().size(), 1uz, "the message arrives");
    const Entry& entry = on.sink->entries()[0];
    expect(entry.format != SourceLocationFormat::NONE, "location is reported not as NONE");
    expect_eq(
        Path(entry.file).filename(),
        Path(here.file_name()).filename(),
        "the location names the calling file, not the logging library"
    );

    Rig explicitly = rig(Level::TRACE, SourceLocationFormat::FULL);
    explicitly.logger.info(here, "forwarded");
    require_eq(explicitly.sink->entries().size(), 1uz, "the forwarded message arrives");
    expect_eq(
        explicitly.sink->entries()[0].line,
        here.line(),
        "an explicitly supplied location is used verbatim"
    );
}

/**
 * @brief Tests that Level formats to its short display name by default, and
 * to its qualified enumerator name under the '?' flag, and that standard
 * fill/align/width still apply in either case.
 */
void test_logging_level_formatting() {
    expect_eq(Ops::fmt("{}", Level::TRACE), "Trace", "TRACE's short name is Trace");
    expect_eq(Ops::fmt("{}", Level::DEBUG), "Debug", "DEBUG's short name is Debug");
    expect_eq(Ops::fmt("{}", Level::INFO), "Info", "INFO's short name is Info");
    expect_eq(Ops::fmt("{}", Level::WARNING), "Warning", "WARNING's short name is Warning");
    expect_eq(Ops::fmt("{}", Level::ERROR), "Error", "ERROR's short name is Error");

    expect_eq(Ops::fmt("{:?}", Level::TRACE), "Level::TRACE", "'?' renders the qualified enumerator name");
    expect_eq(Ops::fmt("{:?}", Level::WARNING), "Level::WARNING", "including WARNING");

    expect_eq(Ops::fmt("{:!}", Level::INFO), "[INFO]:", "'!' renders the original bracketed, all-caps form");
    expect_eq(Ops::fmt("{:!}", Level::WARNING), "[WARNING]:", "including WARNING");

    expect_eq(Ops::fmt("{:_>10}", Level::INFO), "______Info", "plain fill/align/width still applies to the short name");
    expect_eq(
        Ops::fmt("{:_>17?}", Level::WARNING), "___Level::WARNING",
        "fill/align/width composes with '?' too"
    );
    expect_eq(
        Ops::fmt("{:_>10!}", Level::INFO), "___[INFO]:",
        "fill/align/width composes with '!' too - this is what every sink actually uses"
    );

    // The point of the exercise: Level now formats without a bespoke path, so
    // it participates in the comparison assertions like any other type.
    expect_lt(Level::TRACE, Level::DEBUG, "Level orders by its declaration order");
    expect_ge(Level::ERROR, Level::ERROR, "and compares equal to itself");
}

/**
 * @brief Tests that the factory hands back the same logger for the same name.
 */
void test_logging_factory_caches() {
    LoggerFactory logging = LoggerFactory::Builder()
        .of_default_level(Level::WARNING)
        .build();

    SharedPointer<Logger> first = logging.of("alpha");
    SharedPointer<Logger> again = logging.of("alpha");
    SharedPointer<Logger> other = logging.of("beta");

    expect(first.get() == again.get(), "the same name yields the same logger");
    expect(first.get() != other.get(), "a different name yields a different one");
    expect_eq(String(first->name()), "alpha", "the logger is named after its key");
}

/**
 * @brief Tests that the builder's settings reach the loggers it produces.
 */
void test_logging_factory_applies_settings() {
    SharedPointer<MemorySink> sink = Pointers::shared<MemorySink>();
    LoggerFactory logging = LoggerFactory::Builder()
        .of_default_level(Level::ERROR)
        .with_sink(sink)
        .build();

    SharedPointer<Logger> logger = logging.of("configured");
    logger->info("dropped");
    logger->error("kept");

    const Vector<Entry>& got = sink->entries();
    expect_eq(got.size(), 1uz, "the factory's default level is applied to its loggers");
    if (!got.empty()) {
        expect_eq(got[0].message, "kept", "and the shared sink receives what survives");
        expect_eq(got[0].logger, "configured", "under the requested name");
    }

    logging.flush_all();
    expect_eq(sink->flushes(), 1uz, "flush_all reaches the global sinks");
}

/**
 * @brief Tests that the banner is written verbatim, outside the entry format.
 */
void test_logging_factory_banner() {
    SharedPointer<MemorySink> quiet = Pointers::shared<MemorySink>();
    LoggerFactory without = LoggerFactory::Builder()
        .with_sink(quiet)
        .build();
    expect(quiet->entries().empty(), "no banner is written unless asked for");

    SharedPointer<MemorySink> loud = Pointers::shared<MemorySink>();
    LoggerFactory with = LoggerFactory::Builder()
        .with_sink(loud)
        .with_banner()
        .build();

    const Vector<Entry>& got = loud->entries();
    expect_eq(got.size(), 3uz, "the banner is three lines");
    if (got.size() == 3) {
        expect(got[0].raw, "written raw, without a level or timestamp");
        expect(
            got[1].message.find("Logging system initialized") != String::npos,
            "and says what it is"
        );
    }
}

/**
 * @brief Tests that a file sink actually writes, and that missing directories are created.
 */
void test_logging_factory_file_sink() {
    const Path root = stdx::fs::temp_directory_path() / "stdx_logging_test";
    const Path log = root / "nested" / "run.log";
    stdx::fs::remove_all(root);

    {
        LoggerFactory logging = LoggerFactory::Builder()
            .to_file(log, OpenMode::TRUNCATE)
            .of_default_level(Level::TRACE)
            .of_source_location_format(SourceLocationFormat::FILE_LINE)
            .build();

        expect(stdx::fs::exists(log), "build() creates missing parent directories and opens the file");

        SharedPointer<Logger> logger = logging.of("file");
        logger->warn("to disk {}", 1);
        logging.flush_all();
    }

    InputFileStream input(log);
    String contents((InputStreamBufferIterator<char>(input)), InputStreamBufferIterator<char>());

    expect(contents.find("to disk 1") != String::npos, "the message reaches the file");
    expect(contents.find("[WARNING]:") != String::npos, "with its level, bracketed and in caps");
    expect(contents.find("[file]") != String::npos, "and its logger name");
    expect(
        contents.find("[LoggingTest.cpp:") != String::npos,
        "compact source locations keep the file name and line"
    );
    expect(
        contents.find("test_logging_factory_file_sink") == String::npos,
        "compact source locations omit the verbose function name"
    );

    stdx::fs::remove_all(root);
}

/**
 * @brief Tests that a sink prints Level in its original bracketed, all-caps
 * form ("[INFO]:"), padded so every level lines up regardless of name
 * length - independent of how Level formats on its own (see
 * test_logging_level_formatting), which sinks do not use.
 *
 * "[INFO]:" (7 characters) and "[WARNING]:" (10 characters) are logged back
 * to back; both lines must place the following logger-name bracket at the
 * same column, which only holds if the shorter one was padded out to match
 * the longest ("[WARNING]:").
 */
void test_logging_pads_level_column() {
    const Path root = stdx::fs::temp_directory_path() / "stdx_logging_pad_test";
    const Path log = root / "run.log";
    stdx::fs::remove_all(root);

    {
        LoggerFactory logging = LoggerFactory::Builder()
            .to_file(log, OpenMode::TRUNCATE)
            .of_default_level(Level::TRACE)
            .of_source_location_format(SourceLocationFormat::NONE)
            .build();

        SharedPointer<Logger> logger = logging.of("pad");
        logger->info("short level name");
        logger->warn("longest level name");
        logging.flush_all();
    }

    InputFileStream input(log);
    String contents((InputStreamBufferIterator<char>(input)), InputStreamBufferIterator<char>());

    const usize info_line = contents.find("short level name");
    const usize warn_line = contents.find("longest level name");
    require(info_line != String::npos && warn_line != String::npos, "both lines were written");

    const usize info_bracket = contents.rfind('[', info_line);
    const usize warn_bracket = contents.rfind('[', warn_line);
    require(info_bracket != String::npos && warn_bracket != String::npos, "both lines have a logger-name bracket");

    expect_eq(
        info_line - info_bracket, warn_line - warn_bracket,
        "the message starts the same distance after '[pad]' on both lines, so [INFO]: was padded out to [WARNING]:'s width"
    );
    expect(
        contents.find("[INFO]:    [pad]") != String::npos,
        "[INFO]: (7 characters) is padded to 10 columns before the logger-name bracket"
    );
    expect(
        contents.find("[WARNING]: [pad]") != String::npos,
        "[WARNING]: (10 characters) needs no padding and is followed directly by a space and the bracket"
    );

    stdx::fs::remove_all(root);
}

/**
 * @brief Tests that a log line reaches the file as it is written, not only
 * once the sink is flushed or destroyed.
 */
void test_logging_file_sink_writes_without_explicit_flush() {
    const Path root = stdx::fs::temp_directory_path() / "stdx_logging_autoflush_test";
    const Path log = root / "run.log";
    stdx::fs::remove_all(root);

    LoggerFactory logging = LoggerFactory::Builder()
        .to_file(log, OpenMode::TRUNCATE)
        .of_default_level(Level::TRACE)
        .of_source_location_format(SourceLocationFormat::NONE)
        .build();

    SharedPointer<Logger> logger = logging.of("live");
    logger->info("visible while still running");

    InputFileStream input(log);
    String contents((InputStreamBufferIterator<char>(input)), InputStreamBufferIterator<char>());

    expect(
        contents.find("visible while still running") != String::npos,
        "the line is visible through a separate file handle before any explicit flush"
    );

    stdx::fs::remove_all(root);
}

/**
 * @brief Tests that a console sink can be built and written to without throwing.
 */
void test_logging_console_sink() {
    ConsoleSink sink(true);
    expect_no_throw([&] -> void { sink.write_raw(""); }, "a console sink writes without throwing");
    expect_no_throw([&] -> void { sink.flush(); }, "and flushes");
}

/**
 * @brief Tests that a logger can be named after a type rather than a string.
 */
void test_logging_factory_reflected_name() {
    #ifdef __cpp_impl_reflection
    struct Widget {};

    LoggerFactory logging = LoggerFactory::Builder().build();
    SharedPointer<Logger> logger = logging.of<Widget>();

    expect_eq(String(logger->name()), "Widget", "the logger is named after the type's own identifier");
    expect(
        logging.of("Widget").get() == logger.get(),
        "and shares the cache entry with the string spelling"
    );
    #else
    skip("reflection is not enabled in this build");
    #endif
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Logging.level_filtering", test_logging_level_filtering},
        {"Logging.levels_are_distinct", test_logging_levels_are_distinct},
        {"Logging.formatting", test_logging_formatting},
        {"Logging.level_can_be_changed", test_logging_level_can_be_changed},
        {"Logging.multiple_sinks", test_logging_multiple_sinks},
        {"Logging.no_sinks", test_logging_no_sinks},
        {"Logging.source_location", test_logging_source_location},
        {"Logging.level_formatting", test_logging_level_formatting},
        {"Logging.factory_caches", test_logging_factory_caches},
        {"Logging.factory_applies_settings", test_logging_factory_applies_settings},
        {"Logging.factory_banner", test_logging_factory_banner},
        {"Logging.factory_file_sink", test_logging_factory_file_sink},
        {"Logging.pads_level_column", test_logging_pads_level_column},
        {"Logging.file_sink_writes_without_explicit_flush", test_logging_file_sink_writes_without_explicit_flush},
        {"Logging.console_sink", test_logging_console_sink},
        {"Logging.factory_reflected_name", test_logging_factory_reflected_name},
    });
}
