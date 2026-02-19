import stdx;

using stdx::io::OpenMode;
using stdx::mem::SharedPointer;
using stdx::util::logging::ConsoleSink;
using stdx::util::logging::FileSink;
using stdx::util::logging::Level;
using stdx::util::logging::Logger;
using stdx::util::logging::LoggerFactory;

using namespace stdx::core;

int main() {
    LoggerFactory::instance().init("./userdata/test_log.txt", true);

    SharedPointer<Logger> logger = LoggerFactory::instance().of("TestLogger");
    
    logger->trace("This is a TRACE message with value: {}", 42);
    logger->debug("This is a DEBUG message with string: {}", "test");
    logger->info("This is an INFO message");
    logger->warn("This is a WARNING message with float: {:.2f}", 3.14159);
    logger->error("This is an ERROR message with multiple args: {}, {}, {}", 1, 2, 3);

    SharedPointer<Logger> filteredLogger = LoggerFactory::instance().of("FilteredLogger");
    filteredLogger->of_level(Level::WARNING);

    filteredLogger->debug("This DEBUG should NOT appear");
    filteredLogger->info("This INFO should NOT appear");
    filteredLogger->warn("This WARNING SHOULD appear");
    filteredLogger->error("This ERROR SHOULD appear");

    SharedPointer<Logger> customLogger = LoggerFactory::instance().of("CustomLogger");
    SharedPointer<FileSink> customSink = stdx::mem::make_shared<FileSink>("./userdata/custom_log.txt", OpenMode::TRUNCATE);
    customLogger->add_sink(customSink);

    customLogger->info("This message goes to both global and custom sinks");
    customLogger->debug("Custom sink test with value: {}", 99);
    customSink->flush();

    SharedPointer<Logger> consoleLogger = stdx::mem::make_shared<Logger>("ConsoleOnly", Level::DEBUG, false);
    SharedPointer<ConsoleSink> consoleSink = stdx::mem::make_shared<ConsoleSink>(false);
    consoleLogger->add_sink(consoleSink);
    
    consoleLogger->info("This message should appear on stdout");
    consoleLogger->error("This error should appear on stdout (not stderr)");

    LoggerFactory::instance().flush_all();
    logger->flush();
}