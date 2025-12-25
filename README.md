# `stdlibx`

> **WARNING:** As this library is still under development, many features do not yet work, or requires inelegant workarounds or even inclusion of headers to resolve. These issues will only be resolved as development continues.

> **WARNING:** This library has only been tested for Clang, and may not work with GCC or MSVC. 
> Recent tests with GCC caused compiler crashes, likely due to poor module support. As such we only recommend using Clang.

## Overview
This is a project that re-exports the entire C++ standard library as modules. The motivation for creating this was that at the time, support for `import std;` was very poor, a personal dislike of ISO C++'s usage of snake_case for class names, and a dislike of the flat `std::` namespace.

The main features of this repackaging of the standard library are:
- Symbols follow Rust naming conventions (object type names in PascalCase, constants in UPPER_SNAKE_CASE, variables and methods in snake_case)
    - While I personally would have preferred methods to be in camelCase (like in Java), this would have been too much work to rename existing functions and methods, for very little gain
- Splitting the standard library into sub-namespaces (in constrast to the ISO C++ `std::` namespace which is largely flat). 
    - The divisions try to follow the Rust standard library modules, but also take inspiration from the Java standard library.
- Option to toggle between using `std::*` or `stdlib::*` names for symbols/modules (disabled by default as `std` is a reserved name by ISO C++ and library implementations)
- Option to use only `core` and `alloc` modules instead of the full standard library, similar to Rust
- Standard library extensions - features that (in my opinion) ought to be part of the C++ standard library, but are not. Currently largely unimplemented.

Requires a minimum of C++23. 

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Libraries
The `stdlibx` library offers the following namespaces/modules:

### Core library (module `core`, namespace `core::*`)
Core modules of the standard library, not requiring any memory allocation. Provides the most fundamental and low-level features of the C++ standard library. 

### Allocation library (module `alloc`, namespace `alloc::*`)
Allocation modules of the standard library, providing heap allocation and non-trivial data structures.

Disabled when `STDLIB_NO_ALLOC` is enabled.

### Standard library (module `std`, namespace `std::*`)
The full C++ standard library, containing everything provided by `core` and `alloc`, as well as additional functionality depending on operating system and runtime.

If `STDLIBX_USE_RESERVED_STD_IDENTIFIERS` is disabled, `std` is `stdlib` and `std::*` is `stdlib::*`. `STDLIBX_USE_RESERVED_STD_IDENTIFIERS` can further be more finely-controlled using `STDLIBX_NO_RESERVED_STD_MODULE` (modules) and `STDLIBX_NO_RESERVED_STD_NAMESPACE` (namespaces).

Disabled when `STDLIBX_NO_STD` (or `STDLIBX_NO_ALLOC`) are enabled.

### Extensions library (module `stdx`, namespace `stdx::*`)
Technically not "standard library" in the sense of the ISO C++ standard library, but contains features that (in my opinion) ought to be part of the C++ standard library, and are offered by standard libraries of other languages.

If `STDLIBX_USE_RESERVED_STD_IDENTIFIERS` is disabled, `stdx` is `stdlibx` and `stdx::*` is `stdlibx::*`. `STDLIBX_USE_RESERVED_STD_IDENTIFIERS` can further be more finely-controlled using `STDLIBX_NO_RESERVED_STD_MODULE` (modules) and `STDLIBX_NO_RESERVED_STD_NAMESPACE` (namespaces).

Disabled by default - enabled when `STDLIBX_EXTENSIONS` is enabled. 

> **NOTE:** Some parts of this library may be third-party or re-exports of existing libraries, and thus not entirely original code. Code that originates from third party will be adequately attributed, but if there are any issues or concerns, please do not hesitate to contact me.

Some third-party libraries used here include:
- TinyXML-2
- [p-ranav/glob](https://github.com/p-ranav/glob)

## Example
```cpp
import std;
import stdx;

using std::collections::Vector;
using std::fs::Begin;
using std::fs::DirectoryEntry;
using std::fs::DirectoryIterator;
using std::fs::End;
using std::fs::Path;
using stdx::linq::Query;

int main(int argc, char* argv[]) {
    Path dir = std::fs::current_path();

    Vector<DirectoryEntry> files{
        Begin(DirectoryIterator(dir)),
        End(DirectoryIterator(dir)),
        [](const DirectoryEntry& entry) -> bool { return std::fs::is_regular_file(entry); }
    };

    // Use LINQ-style query to find the largest .txt file
    DirectoryEntry result = Query<Vector<DirectoryEntry>>::from(files)
        .where([](const DirectoryEntry& entry) -> bool { return entry.path().extension() == ".txt"; })
        .order_by([](const DirectoryEntry& entry) -> i64 { return -std::fs::file_size(entry); })
        .select([](const DirectoryEntry& entry) -> Tuple<Path, u64> { return Tuple{entry.path(), std::fs::file_size(entry)}; })
        .first_or_default();

    if (result) {
        const auto& [path, size] = *result;
        std::io::println("Largest .txt file: {} ({} bytes)", path.string(), size);
    } else {
        std::io::println("No .txt files found in directory: {}", dir.string());
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

## Known issues
Most of these issues are believed to be possible to resolve using header units, however at the current time header units are not supported by CMake.

- The library is known to fail when trying to use foreach loops with containers such as `Vector`. It is not known why this happens. It can only be solved by including `<vector>`.
```cpp
#include <vector> // necessary (for some reason?)

import std;

using std::collections::Vector;

// ...
Vector<i32> v{1, 2, 3, 4, 5};
for (int i: v) {
    std::io::println("{}", i);
}
```
- A problem with `std::core::String` requiring inclusion of `<string>`
- A problem with `std::fs::DirectoryIterator` requiring inclusion of `<filesystem>`
