import stdx;

using stdx::io::OpenMode;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::util::logging::ConsoleSink;
using stdx::util::logging::FileSink;
using stdx::util::logging::Level;
using stdx::util::logging::Logger;
using stdx::util::logging::LoggerFactory;

#ifdef __GNUC__
using namespace stdx::core;
#endif

int main() {
    LoggerFactory::instance()
        .trace_source(true)
        .init("./userdata/test_log.txt", true);

    SharedPointer<Logger> logger = LoggerFactory::instance().of("TestLogger");
    
    logger->trace("This is a TRACE message with value: {}", 42);
    logger->debug("This is a DEBUG message with string: {}", "test");
    logger->info("This is an INFO message");
    logger->warn("This is a WARNING message with float: {:.2f}", 3.14159);
    logger->error("This is an ERROR message with multiple args: {}, {}, {}", 1, 2, 3);

    SharedPointer<Logger> filtered_logger = LoggerFactory::instance().of("FilteredLogger");
    filtered_logger->of_level(Level::WARNING);

    filtered_logger->debug("This DEBUG should NOT appear");
    filtered_logger->info("This INFO should NOT appear");
    filtered_logger->warn("This WARNING SHOULD appear");
    filtered_logger->error("This ERROR SHOULD appear");

    SharedPointer<Logger> custom_logger = LoggerFactory::instance().of("CustomLogger");
    SharedPointer<FileSink> custom_sink = Pointers::shared<FileSink>("./userdata/custom_log.txt", OpenMode::TRUNCATE);
    custom_logger->add_sink(custom_sink);

    custom_logger->info("This message goes to both global and custom sinks");
    custom_logger->debug("Custom sink test with value: {}", 99);
    custom_sink->flush();

    SharedPointer<Logger> console_logger = Pointers::shared<Logger>("ConsoleOnly", Level::DEBUG, false);
    SharedPointer<ConsoleSink> console_sink = Pointers::shared<ConsoleSink>(false);
    console_logger->add_sink(console_sink);
    
    console_logger->info("This message should appear on stdout");
    console_logger->error("This error should appear on stdout (not stderr)");

    LoggerFactory::instance().flush_all();
    logger->flush();
}
