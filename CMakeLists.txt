cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
message(STATUS "CMake version ${CMAKE_VERSION}")

set(STDLIBX_CXX_STANDARD 23 CACHE INTERNAL "stdlibx C++ standard (visible across directory scopes for FetchContent consumers)")
set(CMAKE_CXX_STANDARD ${STDLIBX_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)

# clang-22 flakily segfaults in its module AST reader when several heavy
# stdexec-coroutine consumer TUs (FlowExample/FlowTest/ExecTaskTest/ExecAsyncTest)
# deserialize the stdx module BMI concurrently. It is a build-time concurrency bug,
# not a source bug (each TU compiles fine alone). This depth-1 Ninja job pool is
# assigned to those specific targets (see tests/ and examples/ CMakeLists) so at
# most one compiles at a time; the rest of the build stays fully parallel.
set_property(GLOBAL PROPERTY JOB_POOLS stdexec_serial=1)

if(CMAKE_VERSION VERSION_LESS_EQUAL "3.31.7")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.2.3")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.3.4")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
endif()

option(BUILD_STATIC_LIBS "Build static libraries instead of shared" OFF)

set(STDLIB "default" CACHE STRING "C++ standard library to use (default, libstdc++, libc++)")
set_property(CACHE STDLIB PROPERTY STRINGS "default" "libstdc++" "libc++")

if(NOT STDLIB MATCHES "^(default|libstdc\\+\\+|libc\\+\\+)$")
    message(FATAL_ERROR "Unknown STDLIB value '${STDLIB}' (expected: default, libstdc++, libc++)")
endif()

set(STDLIB_NAME_CLANG OFF)
set(STDLIB_NAME_GCC OFF)

# Which standard library CMake found is cached, and it is cached inside the
# compiler-identification results rather than as an ordinary entry - so a
# reconfigure that changes STDLIB reuses the answer from the previous one, and
# the `import std` metadata goes on pointing at the library that is no longer in
# use. That is not a cosmetic staleness: pairing libstdc++'s std.cc with
# -stdlib=libc++ fails on its first `#include <bits/stdc++.h>`, in a file
# nothing in this project named.
#
# Removing the detection results forces project() to redo it. Blunt, but the
# alternative is a build directory that is quietly wrong until someone reads a
# confusing error, which is exactly how this was found.
# The marker being absent counts as a mismatch: a build directory configured
# before this check existed carries detection results of unknown provenance, and
# a fresh one has nothing to delete, so re-detecting is right either way.
if(NOT DEFINED STDLIBX_CONFIGURED_STDLIB OR NOT STDLIBX_CONFIGURED_STDLIB STREQUAL STDLIB)
    message(STATUS "Detecting the compiler for STDLIB=${STDLIB} so 'import std' matches")
    file(GLOB _stdlibx_stale_detection "${CMAKE_BINARY_DIR}/CMakeFiles/*/CMakeCXXCompiler.cmake")
    if(_stdlibx_stale_detection)
        file(REMOVE ${_stdlibx_stale_detection})
    endif()
    # Only the location this project discovered for itself, which was found
    # relative to one standard library and means nothing for another. A
    # CMAKE_CXX_STDLIB_MODULES_JSON given on the command line is deliberately
    # left standing: it was chosen for this configure, not carried in from a
    # previous one.
    unset(STDLIBX_LIBCXX_MODULES_JSON CACHE)
    # Likewise for the libc++ runtime directory: it was found relative to one
    # toolchain's bin/, so a reconfigure onto a different compiler must look again
    # rather than link the previous one's dylib.
    unset(STDLIBX_LIBCXX_DYLIB CACHE)
endif()

set(STDLIBX_CONFIGURED_STDLIB "${STDLIB}" CACHE INTERNAL "The STDLIB this build directory was configured with")

# An explicit -stdlib has to reach CMAKE_CXX_FLAGS before project(), not after.
# CMake works out which standard library is in play by preprocessing a probe
# header with the flags it holds during compiler identification, and then picks
# the `import std` module metadata to match. Applied any later, that probe sees
# clang's own default - libstdc++ on most Linux distributions - and CMake pairs
# libstdc++'s std.cc with -stdlib=libc++, which dies on its first
# `#include <bits/stdc++.h>` while scanning a file nothing in this project named.
#
# Setting CMAKE_CXX_STANDARD_LIBRARY here instead does not work: compiler
# identification assigns that variable unconditionally and overwrites it.
#
# The compiler is asked what it is rather than judged by its name, because
# CMAKE_CXX_COMPILER_ID does not exist until project() runs and a plain `c++` is
# either compiler depending on the system.
set(STDLIB_FLAG_APPLIED OFF)

if(NOT STDLIB STREQUAL "default")
    set(_stdlibx_early_cxx "${CMAKE_CXX_COMPILER}")
    if(NOT _stdlibx_early_cxx AND DEFINED ENV{CXX})
        set(_stdlibx_early_cxx "$ENV{CXX}")
    endif()

    if(_stdlibx_early_cxx)
        execute_process(
            COMMAND "${_stdlibx_early_cxx}" --version
            OUTPUT_VARIABLE _stdlibx_early_cxx_version
            ERROR_VARIABLE _stdlibx_early_cxx_version
            RESULT_VARIABLE _stdlibx_early_cxx_result
            OUTPUT_STRIP_TRAILING_WHITESPACE)

        # Only clang takes -stdlib. Handing it to GCC would break the compiler
        # sanity check inside project(), where nothing can explain why; an
        # unsupported pairing falls through to the checks after project() that
        # can say what went wrong.
        if(_stdlibx_early_cxx_result EQUAL 0 AND _stdlibx_early_cxx_version MATCHES "[Cc]lang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=${STDLIB}")
            set(STDLIB_FLAG_APPLIED ON)
        endif()
    endif()
endif()

# CMake locates the 'import std' metadata by asking the compiler for
# -print-file-name=libc++.modules.json, and on two of the toolchains this project
# is built with that answer is unusable.
#
# Upstream LLVM - Homebrew's, and any self-built one - installs libc++ under
# lib/c++/, which is not on that search path at all. The query comes back empty
# and macOS gets no `import std`.
#
# apt.llvm.org's libc++-N-dev is the worse case, because it answers. It ships the
# same libc++.modules.json twice, and the copy clang finds first sits in
# /usr/lib/x86_64-linux-gnu while its source-path is written relative to where
# the other copy lives - so it resolves to /usr/lib/share/libc++/v1/std.cppm,
# which does not exist. Configuration then fails on a missing source file, naming
# a path that appears nowhere in this project.
#
# One rule answers both: the metadata that describes a compiler's own libc++
# lives next to that compiler. Resolving symlinks first is what makes that true
# on Debian, where clang++-N in /usr/bin points into /usr/lib/llvm-N.
#
# Pinned before project() because CMake runs the query inside it and bakes the
# result into the cached compiler-detection file. A value given on the command
# line is left alone, so a toolchain this does not know about stays configurable
# by hand - which is how the libstdc++ side of the same problem is handled, where
# the metadata belongs to a GCC that clang may not be the one to name.
if(NOT CMAKE_CXX_STDLIB_MODULES_JSON
   AND (STDLIB STREQUAL "libc++" OR (APPLE AND STDLIB STREQUAL "default")))
    set(_stdlibx_cxx "${CMAKE_CXX_COMPILER}")
    if(NOT _stdlibx_cxx AND DEFINED ENV{CXX})
        set(_stdlibx_cxx "$ENV{CXX}")
    endif()
    if(_stdlibx_cxx MATCHES "clang")
        if(NOT IS_ABSOLUTE "${_stdlibx_cxx}")
            find_program(_stdlibx_cxx_path "${_stdlibx_cxx}")
            if(_stdlibx_cxx_path)
                set(_stdlibx_cxx "${_stdlibx_cxx_path}")
            endif()
        endif()
        get_filename_component(_stdlibx_llvm_bin "${_stdlibx_cxx}" REALPATH)
        get_filename_component(_stdlibx_llvm_bin "${_stdlibx_llvm_bin}" DIRECTORY)
        find_file(STDLIBX_LIBCXX_MODULES_JSON libc++.modules.json
            HINTS "${_stdlibx_llvm_bin}/../lib/c++" "${_stdlibx_llvm_bin}/../lib"
            NO_DEFAULT_PATH)
        if(STDLIBX_LIBCXX_MODULES_JSON)
            set(CMAKE_CXX_STDLIB_MODULES_JSON "${STDLIBX_LIBCXX_MODULES_JSON}")
            message(STATUS "libc++ std module metadata: ${CMAKE_CXX_STDLIB_MODULES_JSON}")
        endif()
    endif()
endif()

if(BUILD_STATIC_LIBS)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

    if(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif()
    
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
    
    message(STATUS "Configuring for static library build")
else()
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
    message(STATUS "Configuring for shared library build")
endif()

project(stdlibx
    LANGUAGES C CXX
    VERSION 0.0.1
)

# find_package() on macOS keeps finding headers inside the SDK, because the SDK
# ships sql.h and zlib.h of its own, and hands back <sdk>/usr/include as the
# directory to add. Adding it is what breaks the build: it arrives as -isystem,
# which places it ahead of the compiler's own resource directory, so <stddef.h>
# and <stdint.h> resolve to the SDK's shims instead of clang's real headers. The
# first file to include one - arch/arm.cppm, by way of arm_neon.h - then fails on
# '__uint32_t' not naming a type, deep inside the SDK, with nothing in the message
# pointing back at a find_package() result.
#
# Declaring the directory implicit is the fix, because CMake omits implicit
# directories from every command line it generates, whatever contributed them.
# That matters: the offending path arrives through an imported target's
# INTERFACE_INCLUDE_DIRECTORIES, so stripping it package by package means knowing
# which package, and being wrong about that is silent.
#
# It is also true rather than a trick. The compiler already searches the SDK, in
# the right place in its own order, so naming the directory on a command line can
# only move it earlier than the compiler intended.
#
# The SDK is located without reference to CMAKE_OSX_SYSROOT alone: with a
# Homebrew clang rather than AppleClang, CMake emits no -isysroot and leaves that
# variable empty, which is exactly how an earlier attempt at this silently did
# nothing. xcrun is asked as well, and both answers are used.
if(APPLE)
    set(_stdlibx_sdk_roots "")
    if(CMAKE_OSX_SYSROOT AND IS_DIRECTORY "${CMAKE_OSX_SYSROOT}")
        list(APPEND _stdlibx_sdk_roots "${CMAKE_OSX_SYSROOT}")
    endif()

    find_program(_stdlibx_xcrun xcrun)
    if(_stdlibx_xcrun)
        execute_process(
            COMMAND "${_stdlibx_xcrun}" --show-sdk-path
            OUTPUT_VARIABLE _stdlibx_xcrun_sdk
            ERROR_VARIABLE _stdlibx_xcrun_sdk_err
            RESULT_VARIABLE _stdlibx_xcrun_res
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(_stdlibx_xcrun_res EQUAL 0 AND IS_DIRECTORY "${_stdlibx_xcrun_sdk}")
            list(APPEND _stdlibx_sdk_roots "${_stdlibx_xcrun_sdk}")
        endif()
    endif()

    list(REMOVE_DUPLICATES _stdlibx_sdk_roots)
    foreach(_stdlibx_sdk IN LISTS _stdlibx_sdk_roots)
        message(STATUS "Treating SDK headers as implicit: ${_stdlibx_sdk}/usr/include")
        list(APPEND CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "${_stdlibx_sdk}/usr/include")
        list(APPEND CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "${_stdlibx_sdk}/usr/include")
    endforeach()

    if(NOT _stdlibx_sdk_roots)
        message(WARNING
            "Could not locate the macOS SDK, so SDK headers cannot be marked implicit.\n"
            "If the build fails inside the SDK on a type like __uint32_t, pass\n"
            "-DCMAKE_OSX_SYSROOT=$(xcrun --show-sdk-path) and reconfigure.")
    endif()

    # CoreFoundation is not C++-clean. Its CF_ENUM macro expands to
    #
    #     typedef enum Name : Type Name; enum Name : Type { ... }
    #
    # and that first part - a non-defining declaration of an enum with a fixed
    # underlying type - is ill-formed in C++, so clang rejects it as an error by
    # default. Nothing in this project can be written differently to avoid it;
    # including <CoreFoundation/CoreFoundation.h> at all is enough, and every
    # CF_ENUM in every header reached from it fails the same way. Silencing the one
    # diagnostic is what Apple's own C++ consumers do.
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wno-elaborated-enum-base)
    endif()
endif()

set(COMPILER_APPLE_CLANG OFF)
set(COMPILER_CLANG OFF)
set(COMPILER_GCC OFF)
set(COMPILER_MSVC OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(COMPILER_APPLE_CLANG ON)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_CLANG ON)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GCC ON)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_MSVC ON)
endif()

option(STDLIBX_FULL_DEBUG_INFO "Emit full debug info under Clang with libc++, which can crash the frontend on module-heavy TUs" OFF)

# stdlibx is built entirely on `import std` (CMAKE_CXX_MODULE_STD), which only
# these toolchains can provide: upstream LLVM Clang 18.1.2+ with libc++,
# GCC 15+ with libstdc++, and MSVC. Anything else fails deep inside the module
# scan or a try-compile, so reject it here where the message can say why.
if(COMPILER_APPLE_CLANG)
    message(FATAL_ERROR
        "Apple Clang cannot build stdlibx: Apple's toolchain does not ship the std "
        "module sources needed for 'import std'. Use upstream LLVM Clang "
        "(brew install llvm, then set CC/CXX to its clang/clang++) or GCC 15+ "
        "(brew install gcc).")
endif()

# Two different numbers, previously conflated. The FATAL_ERROR floors below are
# what 'import std' needs in order to work at all - they are the compiler's
# requirement, not this library's. The TESTED versions are what stdlibx is
# actually built and run against, and are the only ones CI covers.
#
# Keeping both, rather than raising the floor to the tested version, leaves the
# door open to anyone willing to find out whether an in-between compiler works -
# while making sure they are told they are the one finding out.
set(STDLIBX_TESTED_CLANG_VERSION 22)
set(STDLIBX_TESTED_GCC_VERSION 16)

if(COMPILER_CLANG AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18.1.2)
    message(FATAL_ERROR
        "Clang ${CMAKE_CXX_COMPILER_VERSION} cannot build stdlibx: "
        "'import std' requires Clang 18.1.2 or newer.")
endif()

if(COMPILER_GCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15)
    message(FATAL_ERROR
        "GCC ${CMAKE_CXX_COMPILER_VERSION} cannot build stdlibx: "
        "'import std' requires GCC 15 or newer.")
endif()

if(COMPILER_CLANG AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${STDLIBX_TESTED_CLANG_VERSION})
    message(WARNING
        "Clang ${CMAKE_CXX_COMPILER_VERSION} is newer than 'import std' requires but "
        "older than stdlibx is tested with (Clang ${STDLIBX_TESTED_CLANG_VERSION}). "
        "The build may fail for reasons the check above cannot anticipate.")
endif()

if(COMPILER_GCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${STDLIBX_TESTED_GCC_VERSION})
    message(WARNING
        "GCC ${CMAKE_CXX_COMPILER_VERSION} is newer than 'import std' requires but "
        "older than stdlibx is tested with (GCC ${STDLIBX_TESTED_GCC_VERSION}). Note "
        "that below GCC 16 the build falls back to C++23, which also switches off "
        "reflection and contracts - so this produces a materially different library "
        "from the tested one, not merely an older-compiler build of it.")
endif()

if(COMPILER_MSVC)
    message(WARNING
        "MSVC has never built stdlibx successfully. The Windows paths in stdx::net "
        "and stdx::os are written but have never been compiled or executed.")
endif()

if(APPLE AND COMPILER_GCC)
    message(STATUS "GCC on macOS is experimental; the supported macOS toolchain is upstream LLVM Clang + libc++")
endif()

set(ARCH_X86_64 OFF)
set(ARCH_X86 OFF)
set(ARCH_ARM64 OFF)
set(ARCH_ARM OFF)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
    set(ARCH_X86_64 ON)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386|x86")
    set(ARCH_X86 ON)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARCH_ARM64 ON)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|armv7l")
    set(ARCH_ARM ON)
endif()

if("cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    if(COMPILER_GCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
        message(STATUS "C++ standard: C++23 (C++26 build crashes on versions of GCC less than 16)")
    else()
        set(STDLIBX_CXX_STANDARD 26 CACHE INTERNAL "stdlibx C++ standard (visible across directory scopes for FetchContent consumers)")
        set(CMAKE_CXX_STANDARD 26)
        message(STATUS "C++ standard: C++26 (auto-detected)")
    endif()
else()
    message(STATUS "C++ standard: C++23 (C++26 not advertised by ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")
endif()

option(STDLIBX_ENABLE_CONTRACTS "Enable C++26 contracts" ON)
option(STDLIBX_ENABLE_REFLECTION "Enable C++26 reflection" ON)

if(COMPILER_GCC
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16
    AND STDLIBX_CXX_STANDARD GREATER_EQUAL 26
    AND STDLIBX_ENABLE_CONTRACTS)
    message(STATUS "Contracts enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcontracts")
endif()

if(COMPILER_GCC
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16
    AND STDLIBX_CXX_STANDARD GREATER_EQUAL 26
    AND STDLIBX_ENABLE_REFLECTION)
    message(STATUS "Reflection enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -freflection")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

function(configure_cpp_module_target target)
    set_target_properties(${target} PROPERTIES
        CXX_EXTENSIONS OFF
        CXX_MODULE_BMI_DIRECTORY "${CMAKE_CXX_MODULE_BMI_DIRECTORY}"
        CXX_STANDARD "${STDLIBX_CXX_STANDARD}"
        CXX_STANDARD_REQUIRED ON
        CXX_MODULE_STD ON
    )
endfunction()

option(STDLIBX_ENABLE_SANITIZERS "Enable sanitizers in debug builds" OFF)
option(STDLIBX_USE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_KERNEL "Enable kernel address sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_HW "Enable hardware address sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_UNDEFINED "Enable undefined behavior sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
option(STDLIBX_USE_SANITIZER_LEAK "Enable leak sanitizer" OFF)

function(add_sanitizers target)
    if(NOT STDLIBX_ENABLE_SANITIZERS)
        return()
    endif()

    if(COMPILER_GCC OR COMPILER_CLANG OR COMPILER_APPLE_CLANG)
        set(SANITIZER_FLAGS "")
        
        if(STDLIBX_USE_SANITIZER_ADDRESS)
            list(APPEND SANITIZER_FLAGS "-fsanitize=address")
        endif()

        if(STDLIBX_USE_SANITIZER_KERNEL)
            list(APPEND SANITIZER_FLAGS "-fsanitize=kernel-address")
        endif()

        if(STDLIBX_USE_SANITIZER_HW)
            list(APPEND SANITIZER_FLAGS "-fsanitize=hwaddress")
        endif()

        if(STDLIBX_USE_SANITIZER_UNDEFINED)
            list(APPEND SANITIZER_FLAGS "-fsanitize=undefined")
        endif()

        if(STDLIBX_USE_SANITIZER_THREAD)
            list(APPEND SANITIZER_FLAGS "-fsanitize=thread")
        endif()

        if(STDLIBX_USE_SANITIZER_MEMORY)
            list(APPEND SANITIZER_FLAGS "-fsanitize=memory")
        endif()
        
        if(STDLIBX_USE_SANITIZER_LEAK)
            list(APPEND SANITIZER_FLAGS "-fsanitize=leak")
        endif()
        
        if(SANITIZER_FLAGS)
            target_compile_options(${target} PRIVATE ${SANITIZER_FLAGS})
            target_link_options(${target} PRIVATE ${SANITIZER_FLAGS})
        endif()
    endif()
endfunction()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    
    if(COMPILER_GCC OR COMPILER_CLANG OR COMPILER_APPLE_CLANG)
        add_compile_options(-O3)
    elseif(COMPILER_MSVC)
        add_compile_options(/O2)
    endif()
    
    set(ENABLE_SANITIZERS OFF)
endif()

if(COMPILER_CLANG)
    if(STDLIB STREQUAL "libstdc++")
        if(APPLE)
            message(FATAL_ERROR
                "STDLIB=libstdc++ is not usable with Clang on macOS: modern "
                "macOS ships no GNU libstdc++ to compile or link against. Use "
                "STDLIB=libc++ (or default), or build with GCC 15+ "
                "(brew install gcc) for libstdc++ coverage.")
        endif()
        # Clang + libstdc++ 'import std' works wherever a GCC 15+ libstdc++
        # is installed: CMake locates its libstdc++.modules.json through
        # clang's -print-file-name.
        message(STATUS "Standard library: libstdc++")
        set(STDLIB_NAME_GCC ON)
        # Already on CMAKE_CXX_FLAGS if it was applied before project(), which is
        # where it has to be for the std module metadata to match.
        if(NOT STDLIB_FLAG_APPLIED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
        endif()
        add_link_options(-stdlib=libstdc++)
    elseif(STDLIB STREQUAL "libc++" OR APPLE)
        # Explicitly requested, or macOS - where libc++ is both the platform
        # default and the only C++ runtime the system ships.
        message(STATUS "Standard library: libc++")
        set(STDLIB_NAME_CLANG ON)
        # As above. On macOS with STDLIB=default nothing was applied early, and
        # nothing needs to be: libc++ is already clang's default there, so the
        # detection probe reaches the right answer on its own.
        if(NOT STDLIB_FLAG_APPLIED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        endif()
        add_link_options(-stdlib=libc++)

        # -stdlib=libc++ picks the compiler's headers but still links the OS's
        # dylib, so on macOS an upstream LLVM compiles against its own libc++ and
        # links one several releases older. Anything whose implementation lives in
        # the library then reads as unavailable - std::from_chars for float and
        # double is annotated as introduced in the libc++ that shipped with macOS
        # 26, because __from_chars_floating_point is a dylib export and not a
        # header inline. Link the toolchain's own libc++ so the symbol is the one
        # these headers describe.
        #
        # Both halves are required and neither works alone. Without -L/-rpath the
        # macro would silence the diagnostic while the old dylib was still linked,
        # trading a compile error for a link error. Without the macro the attribute
        # still fires: clang judges it against the deployment target and cannot
        # know which dylib the link will use.
        #
        # The macro goes through CMAKE_CXX_FLAGS rather than
        # add_compile_definitions because the attribute is baked into the std
        # module's BMI, and CMake's synthesized @cmake_cxx_std target picks up
        # CMAKE_CXX_FLAGS but not this directory's compile definitions - the same
        # channel the hardening defines below rely on.
        #
        # Only reached for upstream Clang; COMPILER_CLANG excludes AppleClang,
        # whose headers and dylib match and whose markup is therefore correct.
        if(APPLE)
            get_filename_component(_stdlibx_llvm_bindir "${CMAKE_CXX_COMPILER}" REALPATH)
            get_filename_component(_stdlibx_llvm_bindir "${_stdlibx_llvm_bindir}" DIRECTORY)
            find_file(STDLIBX_LIBCXX_DYLIB libc++.dylib
                HINTS "${_stdlibx_llvm_bindir}/../lib/c++" "${_stdlibx_llvm_bindir}/../lib"
                NO_DEFAULT_PATH)
            if(STDLIBX_LIBCXX_DYLIB)
                get_filename_component(_stdlibx_libcxx_dir "${STDLIBX_LIBCXX_DYLIB}" DIRECTORY)
                message(STATUS "libc++ runtime: ${_stdlibx_libcxx_dir} (availability markup disabled)")
                add_link_options("-L${_stdlibx_libcxx_dir}" "-Wl,-rpath,${_stdlibx_libcxx_dir}")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LIBCPP_DISABLE_AVAILABILITY")
            else()
                message(WARNING
                    "No libc++.dylib found alongside ${CMAKE_CXX_COMPILER}, so the "
                    "build will link the OS libc++ instead of this toolchain's. "
                    "Library-side features newer than the deployment target will be "
                    "reported unavailable - notably std::from_chars for float and "
                    "double, which needs the libc++ from macOS 26.")
            endif()
        endif()
    elseif(UNIX)
        # Clang's own default on Linux and the BSDs is the system libstdc++;
        # no -stdlib flag needed.
        message(STATUS "Standard library: libstdc++ (Clang default on this platform)")
        set(STDLIB_NAME_GCC ON)
    else()
        message(STATUS "Standard library: compiler default")
    endif()
elseif(COMPILER_GCC)
    if(STDLIB STREQUAL "libc++")
        message(FATAL_ERROR
            "STDLIB=libc++ is not usable with GCC: 'import std' is only "
            "supported with libstdc++ under GCC. Use STDLIB=libstdc++ (or "
            "default), or build with LLVM Clang for libc++ coverage.")
    endif()
    set(STDLIB_NAME_GCC ON)
    message(STATUS "Standard library: libstdc++ (GCC default)")
elseif(COMPILER_MSVC)
    if(STDLIB STREQUAL "default")
        message(STATUS "Standard library: MSVC STL (default)")
    else()
        message(WARNING "STDLIB option '${STDLIB}' is not applicable with MSVC")
    endif()
else()
    if(NOT STDLIB STREQUAL "default")
        message(WARNING "STDLIB option '${STDLIB}' may not be supported with ${CMAKE_CXX_COMPILER_ID}")
    else()
        message(STATUS "Standard library: compiler default")
    endif()
endif()

# Full debug info makes Clang pull the whole type graph out of the module BMIs
# to describe it in DWARF, and its AST reader recurses deep enough doing so to
# run out of frontend stack. It arrives as `clang frontend command failed with
# exit code 139` on whichever translation unit happens to sit deepest - a crash
# with no diagnostic, and one that moves as the module graph changes.
#
# Line tables carry file and line, so backtraces and sanitizer reports still
# name real source locations; what goes is the description of variables and
# types, and with it the ability to inspect them in a debugger.
#
# Narrow on purpose. Measured on examples/FlowExample.cpp under Clang 22, 10
# runs each:
#
#   clang + libc++     -g   8 failures    -g1   0 failures
#   clang + libstdc++  -g   0 failures    (whole project builds and tests clean)
#
# so only libc++ pays, and GCC is untouched. The process stack limit is not a
# lever either way: the failure rate is unchanged from 8MB through to unlimited,
# because the frontend runs on a thread whose stack Clang sizes itself.
if(COMPILER_CLANG AND STDLIB_NAME_CLANG AND NOT STDLIBX_FULL_DEBUG_INFO)
    set(CMAKE_CXX_FLAGS_DEBUG "-g1")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g1 -DNDEBUG")
    message(STATUS "Debug info: line tables only (Clang with libc++; -DSTDLIBX_FULL_DEBUG_INFO=ON to override)")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

option(STDLIBX_EXPORT_IMPORT_STD "Enable export import std" OFF)
option(STDLIBX_IMPLICIT_USING_CORE "Make the stdx::core::* namespace implicitly imported at module import" ON)
option(STDLIBX_IMPLICIT_USING_LITERALS "Make the stdx::literals::* namespace implicitly imported at module import" ON)
option(STDLIBX_COMPILE_EXPERIMENTAL_HEADERS "Enable experimental standard library headers" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY "Enable architecture intrinsics header modules in stdx::arch::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY "Enable libsodium modules in stdx::crypto::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY "Enable Microsoft Guidelines Support Library modules in stdx::core::gsl::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY "Enable Nvidia stdexec Library modules in stdx::exec::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY "Enable compiler header modules in stdx::lua::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY "Enable GNU Multiple Precision Arithmetic modules in stdx::math::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY "Enable SQL ODBC wrapper modules in stdx::sql::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY "Enable ZIP (zlib) wrapper modules in stdx::zip::*" ON)
option(STDLIBX_BUILD_EXAMPLES "Build the examples for stdlibx" ON)
option(STDLIBX_BUILD_TESTS "Build the tests for stdlibx" ON)
option(STDLIBX_USE_HARDENED_LIBRARY "Use the hardened standard library" ON)

set(STDLIBX_HARDENED_LIBRARY_MODE "extensive" CACHE STRING "Standard library hardening level (fast, extensive, debug)")
set_property(CACHE STDLIBX_HARDENED_LIBRARY_MODE PROPERTY STRINGS "fast" "extensive" "debug")

if(STDLIBX_NO_STD AND STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    set(STDLIBX_USE_RESERVED_STD_IDENTIFIERS OFF CACHE BOOL "Disabled because STDLIBX_NO_STD is ON" FORCE)
endif()

if(COMPILER_GCC AND NOT STDLIBX_EXPORT_IMPORT_STD)
    set(STDLIBX_EXPORT_IMPORT_STD ON CACHE BOOL "export import std is forced on GCC due to compiler bugs" FORCE)
endif()

if(COMPILER_GCC AND STDLIBX_ENABLE_REFLECTION)
    set(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY OFF CACHE BOOL "Internal compiler error occurs on GCC" FORCE)
endif()

#   libc++     -> _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_{FAST,EXTENSIVE,DEBUG}
#   libstdc++  -> _GLIBCXX_ASSERTIONS  (+ _GLIBCXX_DEBUG for the debug tier)
#   MSVC STL   -> _MSVC_STL_HARDENING=1  (+ _MSVC_STL_DESTRUCTOR_TOMBSTONES for debug)
set(STDLIBX_HARDENING_DEFS "")

if(STDLIBX_USE_HARDENED_LIBRARY)
    if(NOT STDLIBX_HARDENED_LIBRARY_MODE MATCHES "^(fast|extensive|debug)$")
        message(WARNING "Unknown STDLIBX_HARDENED_LIBRARY_MODE '${STDLIBX_HARDENED_LIBRARY_MODE}'; falling back to 'extensive'")
        set(STDLIBX_HARDENED_LIBRARY_MODE "extensive")
    endif()

    if(STDLIB_NAME_CLANG)
        # libc++: fast/extensive/debug all share one ABI, so the mode is free
        # to choose without rebuilding the library.
        if(STDLIBX_HARDENED_LIBRARY_MODE STREQUAL "fast")
            list(APPEND STDLIBX_HARDENING_DEFS "_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_FAST")
        elseif(STDLIBX_HARDENED_LIBRARY_MODE STREQUAL "debug")
            list(APPEND STDLIBX_HARDENING_DEFS "_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG")
        else()
            list(APPEND STDLIBX_HARDENING_DEFS "_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
        endif()
    elseif(STDLIB_NAME_GCC)
        # libstdc++ exposes a single non-ABI-breaking tier (_GLIBCXX_ASSERTIONS)
        # that covers both fast and extensive. Its full debug-container mode
        # (_GLIBCXX_DEBUG) DOES change ABI and must not be mixed with unhardened
        # translation units or the distro-prebuilt libstdc++.
        list(APPEND STDLIBX_HARDENING_DEFS "_GLIBCXX_ASSERTIONS")
        if(STDLIBX_HARDENED_LIBRARY_MODE STREQUAL "debug")
            list(APPEND STDLIBX_HARDENING_DEFS "_GLIBCXX_DEBUG")
            message(WARNING "STDLIBX_HARDENED_LIBRARY_MODE=debug enables _GLIBCXX_DEBUG, which is ABI-incompatible with unhardened libstdc++ code; every linked translation unit must be built the same way.")
        endif()
    elseif(COMPILER_MSVC)
        # MSVC STL has a single hardening switch; until contracts land it
        # reports violations via __fastfail().
        list(APPEND STDLIBX_HARDENING_DEFS "_MSVC_STL_HARDENING=1")
        if(STDLIBX_HARDENED_LIBRARY_MODE STREQUAL "debug")
            list(APPEND STDLIBX_HARDENING_DEFS "_MSVC_STL_DESTRUCTOR_TOMBSTONES=1")
        endif()
    else()
        message(STATUS "Standard library hardening requested, but no known hardening switch for this standard library; skipping")
    endif()

    if(STDLIBX_HARDENING_DEFS)
        message(STATUS "Standard library hardening: ON (mode '${STDLIBX_HARDENED_LIBRARY_MODE}')")
        foreach(def IN LISTS STDLIBX_HARDENING_DEFS)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${def}")
        endforeach()
    endif()
else()
    message(STATUS "Standard library hardening: OFF")
endif()

add_library(stdlibx INTERFACE)

add_subdirectory(src)

target_include_directories(stdlibx 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(COMPILER_APPLE_CLANG)
    target_compile_definitions(stdlibx INTERFACE COMPILER_APPLE_CLANG)
elseif(COMPILER_CLANG)
    target_compile_definitions(stdlibx INTERFACE COMPILER_CLANG)
elseif(COMPILER_GCC)
    target_compile_definitions(stdlibx INTERFACE COMPILER_GCC)
elseif(COMPILER_MSVC)
    target_compile_definitions(stdlibx INTERFACE COMPILER_MSVC)
endif()

if(ARCH_X86_64)
    target_compile_definitions(stdlibx INTERFACE ARCH_X86_64)
elseif(ARCH_X86)
    target_compile_definitions(stdlibx INTERFACE ARCH_X86)
elseif(ARCH_ARM64)
    target_compile_definitions(stdlibx INTERFACE ARCH_ARM64)
elseif(ARCH_ARM)
    target_compile_definitions(stdlibx INTERFACE ARCH_ARM)
endif()

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
endif()

if(STDLIBX_NO_EXPORT_IMPORT_STD)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_NO_EXPORT_IMPORT_STD)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_IMPLICIT_USING_LITERALS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_IMPLICIT_USING_LITERALS)
endif()

if(STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY AND TARGET STDEXEC::stdexec)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
endif()

if(STDLIBX_HARDENING_DEFS)
    target_compile_definitions(stdlibx INTERFACE ${STDLIBX_HARDENING_DEFS})
endif()

if(COMPILER_GCC
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16
    AND STDLIBX_CXX_STANDARD GREATER_EQUAL 26
    AND STDLIBX_ENABLE_CONTRACTS)
    target_compile_options(stdlibx INTERFACE -fcontracts)
endif()

if(COMPILER_GCC
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16
    AND STDLIBX_CXX_STANDARD GREATER_EQUAL 26
    AND STDLIBX_ENABLE_REFLECTION)
    target_compile_options(stdlibx INTERFACE -freflection)
endif()

set_target_properties(stdlibx PROPERTIES
    EXPORT_NAME stdlibx
    CXX_STANDARD ${STDLIBX_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

if(COMMAND configure_cpp_module_target)
    configure_cpp_module_target(stdlibx)
endif()

add_library(stdlibx::stdlibx ALIAS stdlibx)

if(STDLIBX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(STDLIBX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
