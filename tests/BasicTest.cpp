import stdx;

using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::fs::DirectoryEntry;
using stdx::fs::DirectoryIterator;
using stdx::fs::Path;
using stdx::io::File;
using stdx::io::IOException;
using stdx::io::IOState;
using stdx::io::OutputFileStream;
using stdx::mem::SharedPointer;
using stdx::mem::UniquePointer;
using stdx::thread::JoiningThread;

using namespace stdx::core;

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
    JoiningThread t;
    if (t.joinable()) {
        stdx::io::println("t.joinable() returned true");
    }

    String s = "This is a test string";
    stdx::io::println("{}", s);

    Vector<i32> v{1, 2, 3, 4, 5};
    for (usize i = 0; i < v.size(); ++i) {
        stdx::io::println("v[{}] = {}", i, v[i]);
    }
    for (int i: v) {
        stdx::io::println("i = {}", i);
    }

    if (auto result = perform_operation(true)) {
        stdx::io::println("Operation succeeded");
    } else {
        stdx::io::println("Operation failed");
    }

    stdx::io::println("sin(1) = {}, cos(1) = {}, ζ(2) = {}", Math::sin(1), Math::cos(1), Math::riemann_zeta(2));

    Random rng;
    stdx::io::println("Random integer between 1 and 10: {}", rng.next(1, 11));
    stdx::io::println("Random decimal between 0.0 and 10.0: {}", rng.next(0.0, 10.0));
    stdx::io::println("Random number between 0.0 and 1.0: {}", rng.next_unit());
    stdx::io::println("Random number from Gaussian distribution: {}", rng.next_gaussian());
    rng.shuffle(v);
    stdx::io::println("Shuffled vector: {}", v);

    UniquePointer<OutputFileStream> file = stdx::mem::make_unique<OutputFileStream>(Path("./example.txt"));
    file->exceptions(IOState::STREAM_ERROR | IOState::IO_OPERATION_FAIL);
    try {
        stdx::io::println(*file, "Here is some text");
    } catch (const IOException& e) {
        stdx::io::println(File::stderr(), "Failed to write!");
    }

    Path dir = stdx::fs::current_path();

    Vector<DirectoryEntry> files;
    for (const DirectoryEntry& entry: DirectoryIterator(dir)) {
        if (stdx::fs::is_regular_file(entry)) {
            files.push_back(entry);
        }
    }
}
