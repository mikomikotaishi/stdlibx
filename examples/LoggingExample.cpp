import stdx;

using stdx::io::OpenMode;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::util::logging::ConsoleSink;
using stdx::util::logging::FileSink;
using stdx::util::logging::Level;
using stdx::util::logging::Logger;
using stdx::util::logging::LoggerFactory;

void simpleLogging(LoggerFactory& logging) {
    SharedPointer<Logger> logger = logging.of("TestLogger");

    logger->trace("This is a TRACE message with value: {}", 42);
    logger->debug("This is a DEBUG message with string: {}", "test");
    logger->info("This is an INFO message");
    logger->warn("This is a WARNING message with float: {:.2f}", 3.14159);
    logger->error("This is an ERROR message with multiple args: {}, {}, {}", 1, 2, 3);

    logger->flush();
}

void filteredLogging(LoggerFactory& logging) {
    SharedPointer<Logger> filteredLogger = logging.of("FilteredLogger");
    filteredLogger->of_level(Level::WARNING);

    filteredLogger->debug("This DEBUG should NOT appear");
    filteredLogger->info("This INFO should NOT appear");
    filteredLogger->warn("This WARNING SHOULD appear");
    filteredLogger->error("This ERROR SHOULD appear");
}

void consoleOnlyLogging(LoggerFactory& logging) {
    SharedPointer<Logger> consoleLogger = logging.of("ConsoleOnly");
    SharedPointer<ConsoleSink> consoleSink = Pointers::shared<ConsoleSink>(false);
    consoleLogger->add_sink(consoleSink);

    consoleLogger->info("This message should appear on stdout");
    consoleLogger->error("This error should appear on stdout (not stderr)");
}

void customLogging(LoggerFactory& logging) {
    SharedPointer<Logger> customLogger = logging.of("CustomLogger");
    SharedPointer<FileSink> customSink = Pointers::shared<FileSink>("./userdata/custom_log.txt", OpenMode::TRUNCATE);
    customLogger->add_sink(customSink);

    customLogger->info("This message goes to both global and custom sinks");
    customLogger->debug("Custom sink test with value: {}", 99);
    customSink->flush();
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

        void doSomething() {
            logger->info("Bar is doing something");
        }

        [[nodiscard]]
        String getName() const noexcept {
            logger->info("Returning name: '{}'", name);
            return name;
        }

        void setName(const String& s) {
            logger->info("Changing name from '{}' to '{}'", name, s);
            name = s;
        }

        [[nodiscard]]
        String getAddress() const noexcept {
            logger->info("Returning address: '{}'", address);
            return address;
        }

        void setAddress(const String& s) {
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

    simpleLogging(logging);
    filteredLogging(logging);
    consoleOnlyLogging(logging);
    customLogging(logging);

    #ifdef __cpp_impl_reflection
    Bar bar(logging, "John Doe", "123 Main St");
    bar.doSomething();
    System::out.println("Bar({}, {})", bar.getName(), bar.getAddress());

    bar.setName("Jane Smith");
    bar.setAddress("456 Elm St");

    System::out.println("Bar({}, {})", bar.getName(), bar.getAddress());
    #endif

    logging.flush_all();
}
