cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
message(STATUS "CMake version ${CMAKE_VERSION}")

set(STDLIBX_CXX_STANDARD 23 CACHE INTERNAL "stdlibx C++ standard (visible across directory scopes for FetchContent consumers)")
set(CMAKE_CXX_STANDARD ${STDLIBX_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON)

if(CMAKE_VERSION VERSION_LESS_EQUAL "3.31.7")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.2.3")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
endif()

option(BUILD_STATIC_LIBS "Build static libraries instead of shared" OFF)

set(STDLIB "default" CACHE STRING "C++ standard library to use (default, libstdc++, libc++)")
set_property(CACHE STDLIB PROPERTY STRINGS "default" "libstdc++" "libc++")

set(STDLIB_NAME_CLANG OFF)
set(STDLIB_NAME_GCC OFF)

# CXX compile flags must be set BEFORE project() so that CMake's import-std
# detection finds the correct standard library module source (e.g. libc++'s
# std.cppm instead of libstdc++'s std.cc). Linker flags are applied after
# project() to avoid breaking the C compiler test.
if(STDLIB STREQUAL "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(STDLIB_NAME_CLANG ON)
elseif(STDLIB STREQUAL "libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    set(STDLIB_NAME_GCC ON)
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
    # Must live in CMAKE_CXX_FLAGS, not add_compile_options(). -fcontracts puts
    # GCC into the C++26/contracts language dialect, which it bakes into every
    # module BMI. CMake's synthesised `import std` module target does NOT
    # inherit directory- or target-scope compile options, so under the older
    # import-std implementations it gets built as plain C++26; importing that
    # std BMI from our contracts TUs then fails with
    # "language dialect differs 'C++26', expected 'C++26/contracts'".
    # CMAKE_CXX_FLAGS is threaded into the std-module compile (it's how the
    # libc++/libstdc++ -stdlib= selection above reaches it), so it keeps the
    # std BMI in the matching dialect regardless of CMake version.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcontracts")
endif()

if(COMPILER_GCC
    AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16
    AND STDLIBX_CXX_STANDARD GREATER_EQUAL 26
    AND STDLIBX_ENABLE_REFLECTION)
    message(STATUS "Reflection enabled")
    # Same reasoning as -fcontracts above: keep it in CMAKE_CXX_FLAGS so the
    # std-module target is compiled in the matching dialect.
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

option(STDLIBX_ENABLE_SANITIZERS "Enable sanitisers in debug builds" OFF)
option(STDLIBX_USE_SANITIZER_ADDRESS "Enable address sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_KERNEL "Enable kernel address sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_HW "Enable hardware address sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_UNDEFINED "Enable undefined behaviour sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_THREAD "Enable thread sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_MEMORY "Enable memory sanitiser" OFF)
option(STDLIBX_USE_SANITIZER_LEAK "Enable leak sanitiser" OFF)

function(add_sanitisers target)
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

if(COMPILER_CLANG OR COMPILER_APPLE_CLANG)
    if(STDLIB_NAME_CLANG)
        message(STATUS "Standard library: libc++")
        add_link_options(-stdlib=libc++)
    elseif(STDLIB_NAME_GCC)
        message(STATUS "Standard library: libstdc++")
        add_link_options(-stdlib=libstdc++)
    else()
        if(APPLE)
            message(STATUS "Standard library: libc++ (default on macOS)")
            set(STDLIB_NAME_CLANG ON)
        elseif(UNIX)
            message(STATUS "Standard library: libstdc++ (default on Linux)")
            set(STDLIB_NAME_GCC ON)
        else()
            message(STATUS "Standard library: compiler default")
        endif()
    endif()
elseif(COMPILER_GCC)
    set(STDLIB_NAME_GCC ON)
    if(STDLIB STREQUAL "default")
        message(STATUS "Standard library: libstdc++ (GCC default)")
    else()
        message(WARNING "STDLIB option '${STDLIB}' is not applicable with GCC (always uses libstdc++)")
    endif()
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
option(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY "Enable compiler header modules in stdx::lua::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY "Enable GNU Multiple Precision Arithmetic modules in stdx::math::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY "Enable SQL ODBC wrapper modules in stdx::sql::*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY "Enable ZIP (zlib) wrapper modules in stdx::zip::*" ON)
option(STDLIBX_BUILD_EXAMPLES "Build the examples for stdlibx" ON)
option(STDLIBX_BUILD_TESTS "Build the tests for stdlibx" ON)

if(STDLIBX_NO_STD AND STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    set(STDLIBX_USE_RESERVED_STD_IDENTIFIERS OFF CACHE BOOL "Disabled because STDLIBX_NO_STD is ON" FORCE)
endif()

if(COMPILER_GCC AND NOT STDLIBX_EXPORT_IMPORT_STD)
    set(STDLIBX_EXPORT_IMPORT_STD ON CACHE BOOL "export import std is forced on GCC due to compiler bugs" FORCE)
endif()

if(COMPILER_GCC AND STDLIBX_ENABLE_REFLECTION)
    # SQL, Zip, and Microsoft GSL build and pass under GCC 16 + reflection
    # (verified: SqlTest 34/34 incl. the reflection Connection::query<T>,
    # ZipTest 15/15), so they are intentionally NOT forced off and stay ON.
    #
    # Lua, GMP, and Crypto (libsodium) still fail, but NOT due to the old
    # (now-fixed) StandardInteger main-module bug: their C headers pull libc
    # types (__mbstate_t, pthread_mutex_t, std::streampos, ...) into the global
    # module fragment that clash with `import std` ("conflicting imported
    # declaration"); GMP additionally trips __gmp_expr vs std::error_condition.
    # These are C++-modules / C-header interop issues - re-enable once sorted.
    set(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY OFF CACHE BOOL "C headers clash with import std under GCC 16 + reflection" FORCE)
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

