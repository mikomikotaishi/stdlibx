# `stdlibx`

> **WARNING:** As this library is still under development, many features do not yet work, or requires inelegant workarounds or even inclusion of headers to resolve. These issues will only be resolved as development continues.

> **WARNING:** This library is primarily tested with Clang and GCC, and MSVC support is not yet confirmed.

## Overview
This is a project that re-exports the entire C++ standard library as modules, in an API more consistent with standard libraries of other languages. The main purpose is to create a standard library experience that is more enjoyable and familiar to use, while remaining compatible (to some degree) with the original standard library.

The motivation for creating this was that at the time, support for `import std;` was very poor (especially on CMake), a personal dislike of ISO C++'s usage of snake_case for class names, and a dislike of the flat `std::` namespace.

The main features of this repackaging of the standard library are:
- Symbols follow Rust naming conventions (object type names in PascalCase, constants in UPPER_SNAKE_CASE, variables and methods in snake_case)
    - While I personally would have preferred methods to be in camelCase (like in Java), this would have been too much work to rename existing functions and methods, for very little gain.
    - Perhaps in the future I might wrap all standard library classes using private inheritance, but this may be a gargantuan effort and not in the scope of the project at the present.
- Splitting the standard library into sub-namespaces (in constrast to the ISO C++ `std::` namespace which is largely flat). 
    - The divisions try to follow the Rust standard library modules, but also take inspiration from the Java standard library.
- Option to use only `core` and `alloc` modules instead of the full standard library, similar to Rust

Pros:
- A clean (in my opinion) API that is more in line with what C++ style should be (in my opinion), and other languages.
- More features that ought to be part of the standard library, in a clean, C++-style interface.
- Consistently updated (but is currently in-development and may be subject to unstable, API-breaking changes).
Cons:
- Non-standard, obviously. Features are obviously dependent on what is supported by vendors, and different compilers may have different challenges building this library.
- This library is developed independently. While we accept feature requests, pull requests, and improvements from everyone, do note that support is limited due to the limited resources.
- Often relies on cutting or bleeding-edge features, which may not be suitable for stability-priority projects.
- Seeing as this is a library on top of another library with additional features, it may increase compile times on your project, or even increase binary size. The library takes roughly 30 seconds to build on my (modest) hardware on both Clang and GCC.

Requires a minimum of C++23. This library is tested with libstdc++ (GCC) as the default standard library, and has not yet been tested with libc++ (Clang) or MSVC STL.

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Libraries
The `stdlibx` library offers the following namespaces/modules:

### Core library (module `core`, namespace `core::*`)
Core modules of the standard library, not requiring any memory allocation. Provides the most fundamental and low-level features of the C++ standard library. 

### Allocation library (module `alloc`, namespace `alloc::*`)
Allocation modules of the standard library, providing heap allocation and non-trivial data structures.

Disabled when `STDLIB_NO_ALLOC` is enabled.

### Standard library (module `stdx`, namespace `stdx::*`)
The full C++ standard library, containing everything provided by `core` and `alloc`, as well as additional functionality depending on operating system and runtime. Includes "extensions", i.e. features that are not officially part of the ISO C++ standard library, but ought to be (in my opinion) and have equivalent features standard libraries of other languages.

Disabled when `STDLIBX_NO_STD` (or `STDLIBX_NO_ALLOC`) are enabled.

> **NOTE:** Some parts of this library may be third-party or re-exports of existing libraries, and thus not entirely original code. Code that originates from third party will be adequately attributed, but if there are any issues or concerns, please do not hesitate to contact me.

Some third-party libraries used here include:
- TinyXML-2
- [p-ranav/glob](https://github.com/p-ranav/glob)

## Example
```cpp
import stdx;

using stdx::collections::Vector;
using stdx::fs::Begin;
using stdx::fs::DirectoryEntry;
using stdx::fs::DirectoryIterator;
using stdx::fs::End;
using stdx::fs::Path;
using stdx::linq::Query;

int main(int argc, char* argv[]) {
    Path dir = stdx::fs::current_path();

    Vector<DirectoryEntry> files{
        Begin(DirectoryIterator(dir)),
        End(DirectoryIterator(dir)),
        [](const DirectoryEntry& entry) -> bool { return stdx::fs::is_regular_file(entry); }
    };

    // Use LINQ-style query to find the largest .txt file
    DirectoryEntry result = Query<Vector<DirectoryEntry>>::from(files)
        .where([](const DirectoryEntry& entry) -> bool { return entry.path().extension() == ".txt"; })
        .order_by([](const DirectoryEntry& entry) -> i64 { return -stdx::fs::file_size(entry); })
        .select([](const DirectoryEntry& entry) -> Tuple<Path, u64> { return Tuple{entry.path(), stdx::fs::file_size(entry)}; })
        .first_or_default();

    if (result) {
        const auto& [path, size] = *result;
        stdx::io::println("Largest .txt file: {} ({} bytes)", path.string(), size);
    } else {
        stdx::io::println("No .txt files found in directory: {}", dir.string());
    }
}
```

## Build
This supports building using CMake and XMake.

Make (calls CMake):
```sh
make
```

CMake:
```sh
cmake -S . -B build -G Ninja
cmake --build build
```

XMake:
```sh
xmake
```

## Contributing
Pull requests are always welcome - if you like this library and wish to contribute or solve bugs, feel free to make improvements.
