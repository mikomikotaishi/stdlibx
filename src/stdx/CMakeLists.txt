file(GLOB_RECURSE STDX_MODULES *.cppm)

if(BUILD_STATIC_LIBS)
    add_library(stdlibx_stdx STATIC)
else()
    add_library(stdlibx_stdx SHARED)
endif()

target_sources(stdlibx_stdx
    PUBLIC
    FILE_SET CXX_MODULES FILES
    ${STDX_MODULES}
)

configure_cpp_module_target(stdlibx_stdx)

target_link_libraries(stdlibx_stdx PUBLIC stdlibx_core stdlibx_alloc)

target_include_directories(stdlibx_stdx 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

add_library(stdlibx::stdx ALIAS stdlibx_stdx)

if(COMPILER_APPLE_CLANG)
    target_compile_definitions(stdlibx_stdx PRIVATE COMPILER_APPLE_CLANG)
elseif(COMPILER_CLANG)
    target_compile_definitions(stdlibx_stdx PRIVATE COMPILER_CLANG)
elseif(COMPILER_GCC)
    target_compile_definitions(stdlibx_stdx PRIVATE COMPILER_GCC)
elseif(COMPILER_MSVC)
    target_compile_definitions(stdlibx_stdx PRIVATE COMPILER_MSVC)
endif()

if(ARCH_X86_64)
    target_compile_definitions(stdlibx_stdx PRIVATE ARCH_X86_64)
elseif(ARCH_X86)
    target_compile_definitions(stdlibx_stdx PRIVATE ARCH_X86)
elseif(ARCH_ARM64)
    target_compile_definitions(stdlibx_stdx PRIVATE ARCH_ARM64)
elseif(ARCH_ARM)
    target_compile_definitions(stdlibx_stdx PRIVATE ARCH_ARM)
endif()

if(STDLIBX_NO_STD)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_NO_STD)
endif()

if(STDLIBX_NO_ALLOC)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_NO_ALLOC)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx_stdx INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_EXPERIMENTAL_STD)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXPERIMENTAL_STD)
endif()

if(STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_ARCH_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_OS_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_OS_LIBRARY)
endif()

find_package(LLVM QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY AND LLVM_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC LLVMSupport LLVMCore)
elseif(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    message(WARNING "LLVM not found. Skipping compiler extension.")
endif()

find_package(Lua QUIET)
if(Lua_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)

    if(NOT TARGET Lua::Lua)
        add_library(Lua::Lua INTERFACE IMPORTED)
        set_target_properties(Lua::Lua PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
        )
    endif()
    target_link_libraries(stdlibx_stdx PUBLIC Lua::Lua)
else()
    message(WARNING "Lua not found. Skipping Lua extension.")
endif()

find_package(Sodium QUIET MODULE)
if(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY AND Sodium_FOUND)
    if(sodium_VERSION)
        message(STATUS "Found Sodium: ${sodium_LIBRARY_RELEASE} (found version \"${sodium_VERSION}\")")
    else()
        message(STATUS "Found Sodium: ${sodium_LIBRARY_RELEASE}")
    endif()

    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)

    if(DEFINED sodium_INCLUDE_DIR)
        target_include_directories(stdlibx_stdx PUBLIC ${sodium_INCLUDE_DIR})
    endif()

    if(TARGET sodium)
        target_link_libraries(stdlibx_stdx PUBLIC sodium)
    elseif(DEFINED sodium_LIBRARIES)
        target_link_libraries(stdlibx_stdx PUBLIC ${sodium_LIBRARIES})
    else()
        message(FATAL_ERROR "Sodium found, but no usable target or libraries were provided.")
    endif()
elseif(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
    message(STATUS "Sodium not found. Fetching libsodium from jedisct1/libsodium...")

    set(SODIUM_DISABLE_TESTS ON CACHE BOOL "Disable libsodium tests" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "Disable third-party tests" FORCE)

    FetchContent_Declare(
        libsodium
        GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
        GIT_TAG 1.0.21
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(libsodium)

    set(_SODIUM_TARGET "")
    if(TARGET sodium)
        set(_SODIUM_TARGET sodium)
    elseif(TARGET libsodium)
        set(_SODIUM_TARGET libsodium)
    elseif(TARGET libsodium-static)
        set(_SODIUM_TARGET libsodium-static)
    endif()

    if(_SODIUM_TARGET STREQUAL "")
        message(FATAL_ERROR "Fetched libsodium, but no usable target was found.")
    endif()

    FetchContent_GetProperties(libsodium SOURCE_DIR libsodium_SOURCE_DIR BINARY_DIR libsodium_BINARY_DIR)
    message(STATUS "Found Sodium: ${libsodium_BINARY_DIR} (found version \"1.0.21\")")

    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ${_SODIUM_TARGET})
endif()

find_package(ODBC QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY AND ODBC_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ODBC::ODBC)
elseif(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    message(WARNING "ODBC not found. Skipping SQL extension.")
endif()

find_package(GMP QUIET MODULE)
if(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY AND GMP_FOUND)
    if(GMP_VERSION)
        message(STATUS "Found GMP: ${GMP_CXX_LIBRARIES} (found version \"${GMP_VERSION}\")")
    else()
        message(STATUS "Found GMP: ${GMP_CXX_LIBRARIES}")
    endif()
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    target_include_directories(stdlibx_stdx PUBLIC ${GMP_INCLUDE_DIRS})
    target_link_libraries(stdlibx_stdx PUBLIC GMP::GMPXX GMP::GMP)
elseif(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    message(STATUS "GMP not found. Building GMP from official source tarball...")
    include(ExternalProject)

    ExternalProject_Add(
        gmp_external
        URL https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-cxx --disable-shared --enable-static --with-pic
        BUILD_COMMAND make
        INSTALL_COMMAND make install
    )

    ExternalProject_Get_Property(gmp_external INSTALL_DIR)
    set(GMP_FALLBACK_INCLUDE_DIR "${INSTALL_DIR}/include")
    set(GMP_FALLBACK_C_LIBRARY "${INSTALL_DIR}/lib/libgmp.a")
    set(GMP_FALLBACK_CXX_LIBRARY "${INSTALL_DIR}/lib/libgmpxx.a")
    set(GMP_FALLBACK_VERSION "6.3.0")
    message(STATUS "Found GMP: ${GMP_FALLBACK_CXX_LIBRARY} (found version \"${GMP_FALLBACK_VERSION}\")")

    add_library(GMP::GMP UNKNOWN IMPORTED GLOBAL)
    set_target_properties(GMP::GMP PROPERTIES
        IMPORTED_LOCATION "${GMP_FALLBACK_C_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${GMP_FALLBACK_INCLUDE_DIR}"
    )

    add_library(GMP::GMPXX UNKNOWN IMPORTED GLOBAL)
    set_target_properties(GMP::GMPXX PROPERTIES
        IMPORTED_LOCATION "${GMP_FALLBACK_CXX_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${GMP_FALLBACK_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "GMP::GMP"
    )

    add_dependencies(GMP::GMP gmp_external)
    add_dependencies(GMP::GMPXX gmp_external)
    add_dependencies(stdlibx_stdx gmp_external)

    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC GMP::GMPXX GMP::GMP)
endif()

find_package(ZLIB QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY AND ZLIB_FOUND)
    if(ZLIB_VERSION)
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES} (found version \"${ZLIB_VERSION}\")")
    else()
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES}")
    endif()
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
    target_include_directories(stdlibx_stdx PUBLIC ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(stdlibx_stdx PUBLIC ZLIB::ZLIB)
elseif(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
    message(STATUS "ZLIB not found. Fetching ZLIB from madler/zlib...")

    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG 1.3.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(zlib)

    if(NOT TARGET ZLIB::ZLIB)
        if(TARGET zlibstatic)
            add_library(ZLIB::ZLIB ALIAS zlibstatic)
        elseif(TARGET zlib)
            add_library(ZLIB::ZLIB ALIAS zlib)
        endif()
    endif()

    if(NOT TARGET ZLIB::ZLIB)
        message(FATAL_ERROR "Fetched zlib, but no usable ZLIB target was found.")
    endif()

    FetchContent_GetProperties(zlib SOURCE_DIR zlib_SOURCE_DIR BINARY_DIR zlib_BINARY_DIR)
    message(STATUS "Found ZLIB: ${zlib_BINARY_DIR} (found version \"1.3.2\")")

    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ZLIB::ZLIB)
endif()

set_target_properties(stdlibx_stdx PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx_stdx PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
endif()
