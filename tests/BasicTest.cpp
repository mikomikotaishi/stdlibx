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
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::linq::Query;
using stdx::mem::SharedPointer;
using stdx::mem::UniquePointer;
using stdx::thread::Thread;

using namespace stdx::core;

int main() {
    Thread t;
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
