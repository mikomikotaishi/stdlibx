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

# The -stdlib flag is applied after project(), once the compiler is known, so
# that unsupported compiler/stdlib pairs fail with a clear message instead of
# breaking the compiler sanity check.
set(STDLIB_NAME_CLANG OFF)
set(STDLIB_NAME_GCC OFF)

# Upstream LLVM (e.g. Homebrew's) installs libc++ under lib/c++/, which is not
# on clang's -print-file-name search path, so CMake's automatic discovery of
# the std module metadata for 'import std' fails on macOS. CMake runs that
# discovery inside project() and caches a bogus relative path, so the correct
# location must be pinned here, before project(), by finding it relative to
# the compiler. Falls through harmlessly (leaving discovery to CMake) when the
# compiler is not a clang or ships no lib/c++ directory.
if(APPLE AND NOT CMAKE_CXX_STDLIB_MODULES_JSON)
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
        get_filename_component(_stdlibx_llvm_bin "${_stdlibx_cxx}" DIRECTORY)
        find_file(CMAKE_CXX_STDLIB_MODULES_JSON libc++.modules.json
            HINTS "${_stdlibx_llvm_bin}/../lib/c++" "${_stdlibx_llvm_bin}/../lib"
            NO_DEFAULT_PATH)
        if(CMAKE_CXX_STDLIB_MODULES_JSON)
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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
        add_link_options(-stdlib=libstdc++)
    elseif(STDLIB STREQUAL "libc++" OR APPLE)
        # Explicitly requested, or macOS - where libc++ is both the platform
        # default and the only C++ runtime the system ships.
        message(STATUS "Standard library: libc++")
        set(STDLIB_NAME_CLANG ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        add_link_options(-stdlib=libc++)
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
