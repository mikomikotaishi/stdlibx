import stdx;

using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::fs::DirectoryEntry;
using stdx::fs::DirectoryIterator;
using stdx::fs::Path;
using stdx::io::IOException;
using stdx::io::IOState;
using stdx::io::OutputFileStream;
using stdx::io::TextStyle;
using stdx::linq::Query;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::mem::UniquePointer;
using stdx::thread::JoiningThread;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

#ifdef __GNUC__
using namespace stdx::core;
#endif

enum class Status {
    SUCCESS,
    FAILURE,
};

Expected<u32, Status> perform_operation(bool succeed) {
    if (succeed) {
        return 42;
    } else {
        return Unexpected(Status::FAILURE);
    }
}

int main() {
    ArgumentParser parser("test_program", "1.0", DefaultArguments::NONE);
    parser.add_argument("--name").default_value("world");
    parser.add_argument("--count").default_value(1).scan<'d', i32>();
    parser.add_argument("--verbose").flag();

    parser.parse_args({"test_program", "--name", "stdlibx", "--count", "3", "--verbose"});

    System::out.println("[argparse] name    = {}", parser.get("--name"));
    System::out.println("[argparse] count   = {}", parser.get<i32>("--count"));
    System::out.println("[argparse] verbose = {}", parser.get<bool>("--verbose"));

    JoiningThread t;
    if (t.joinable()) {
        System::out.println("t.joinable() returned true");
    }

    String s = "This is a test string";
    System::out.println("{}", s);

    System::out.printf("Hello, world!%n");
    System::out.printf("Formatted number: %d, hex: %x, float: %.2f%n", 42, 255, 3.14159);

    Vector<i32> v{1, 2, 3, 4, 5};
    for (usize i = 0; i < v.size(); ++i) {
        System::out.println("v[{}] = {}", i, v[i]);
    }
    for (int i: v) {
        System::out.println("i = {}", i);
    }

    if (Expected<u32, Status> result = perform_operation(true)) {
        System::out.println("Operation succeeded");
    } else {
        System::err.println("Operation failed");
    }

    System::out.println("sin(1) = {}, cos(1) = {}, ζ(2) = {}", Math::sin(1), Math::cos(1), Math::riemann_zeta(2));

    Random rng;
    stdx::io::println(TextStyle().fg(TextStyle::Color::GREEN), "Random integer between 1 and 10: {}", rng.next(1, 11));
    stdx::io::println(
        fg(TextStyle::Color::RED) | emphasis(TextStyle::Emphasis::BOLD) | bg(TextStyle::Color::BLACK), 
        "Random decimal between 0.0 and 10.0: {}", 
        rng.next(0.0, 10.0)
    );
    System::out.printf("Random number between 0.0 and 1.0: %f%n", rng.next_unit());
    System::out.printf("Random number from Gaussian distribution: %f%n", rng.next_gaussian());
    rng.shuffle(v);
    System::out.printf("Shuffled vector: %s%n", v);

    UniquePointer<OutputFileStream> file = Pointers::unique<OutputFileStream>(Path("./example.txt"));
    file->exceptions(IOState::STREAM_ERROR | IOState::IO_OPERATION_FAIL);
    try {
        stdx::io::println(*file, "Here is some text");
    } catch (const IOException& e) {
        System::err.println("Failed to write!");
    }

    System::out.printf("All environment variables: %s%n", System::getenv());

    System::out.print(v);
    System::out.println();
    System::out.printf(v);
    System::out.println();
    System::out.println(v);
    System::out.println(123);

    System::out.flush();

    Path dir = stdx::fs::current_path();

    Vector<DirectoryEntry> files;
    for (const DirectoryEntry& entry: DirectoryIterator(dir)) {
        if (stdx::fs::is_regular_file(entry)) {
            files.push_back(entry);
        }
    }

    System::out.printf(
        "Files in current directory (%s)%n:%s%n", 
        dir.string(),
        Query(files).select([](const DirectoryEntry& e) -> String { return e.path().string(); }).to<Vector>()
    );

    System::out.println(
        "The current time is {}, or {}",
        System::current_time_millis(),
        System::current_time_as_string()
    );
}
