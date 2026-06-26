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

void simple_logging(LoggerFactory& logging) {
    SharedPointer<Logger> logger = logging.of("TestLogger");

    logger->trace("This is a TRACE message with value: {}", 42);
    logger->debug("This is a DEBUG message with string: {}", "test");
    logger->info("This is an INFO message");
    logger->warn("This is a WARNING message with float: {:.2f}", 3.14159);
    logger->error("This is an ERROR message with multiple args: {}, {}, {}", 1, 2, 3);

    logger->flush();
}

void filtered_logging(LoggerFactory& logging) {
    SharedPointer<Logger> filtered_logger = logging.of("FilteredLogger");
    filtered_logger->of_level(Level::WARNING);

    filtered_logger->debug("This DEBUG should NOT appear");
    filtered_logger->info("This INFO should NOT appear");
    filtered_logger->warn("This WARNING SHOULD appear");
    filtered_logger->error("This ERROR SHOULD appear");
}

void console_only_logging(LoggerFactory& logging) {
    SharedPointer<Logger> console_logger = logging.of("ConsoleOnly");
    SharedPointer<ConsoleSink> console_sink = Pointers::shared<ConsoleSink>(false);
    console_logger->add_sink(console_sink);

    console_logger->info("This message should appear on stdout");
    console_logger->error("This error should appear on stdout (not stderr)");
}

void custom_logging(LoggerFactory& logging) {
    SharedPointer<Logger> custom_logger = logging.of("CustomLogger");
    SharedPointer<FileSink> custom_sink = Pointers::shared<FileSink>("./userdata/custom_log.txt", OpenMode::TRUNCATE);
    custom_logger->add_sink(custom_sink);

    custom_logger->info("This message goes to both global and custom sinks");
    custom_logger->debug("Custom sink test with value: {}", 99);
    custom_sink->flush();
}

#ifdef __cpp_impl_reflection
namespace mmt::foo {
    class Bar {
    private:
        SharedPointer<Logger> logger;
        String name;
        String address;
    public:
        explicit Bar(LoggerFactory& logging, String name, String address):
            logger{logging.of<Bar>()}, name{Ops::move(name)}, address{Ops::move(address)} {
            logger->info("Bar constructed");
        }

        ~Bar() {
            logger->info("Bar destroyed");
        }

        void do_something() {
            logger->info("Bar is doing something");
        }

        void change_name(const String& s) {
            logger->info("Changing name from '{}' to '{}'", name, s);
            name = s;
        }

        void change_address(const String& s) {
            logger->info("Changing address from '{}' to '{}'", address, s);
            address = s;
        }
    };
}

using mmt::foo::Bar;
#endif

int main(int argc, char* argv[]) {
    LoggerFactory logging = LoggerFactory::Builder()
        .with_file("./userdata/test_log.txt")
        .with_console()
        .with_banner()
        .build();

    simple_logging(logging);
    filtered_logging(logging);
    console_only_logging(logging);
    custom_logging(logging);

    #ifdef __cpp_impl_reflection
    Bar bar(logging, "John Doe", "123 Main St");
    bar.do_something();
    bar.change_name("Jane Smith");
    bar.change_address("456 Elm St");
    #endif

    logging.flush_all();
}
