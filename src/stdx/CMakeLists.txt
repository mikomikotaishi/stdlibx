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

target_include_directories(stdlibx_stdx 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

if(STDLIB_NAME_GCC)
    target_link_libraries(stdlibx_stdx PUBLIC stdc++exp)
endif()

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

if(STDLIBX_EXPORT_IMPORT_STD)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXPORT_IMPORT_STD)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_IMPLICIT_USING_LITERALS)
    target_compile_definitions(stdlibx_stdx INTERFACE STDLIBX_IMPLICIT_USING_LITERALS)
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

if(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY AND Lua_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC Lua::Lua)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY AND Sodium_FOUND)
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
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ${_SODIUM_TARGET})
endif()

if(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY)
    target_link_libraries(stdlibx_stdx PRIVATE Microsoft.GSL::GSL)
endif()

find_package(ODBC QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY AND ODBC_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ODBC::ODBC)
elseif(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    message(WARNING "ODBC not found. Skipping SQL extension.")
endif()

if(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    if(GMP_FOUND)
        target_include_directories(stdlibx_stdx PUBLIC ${GMP_INCLUDE_DIRS})
    else()
        add_dependencies(stdlibx_stdx gmp_external)
    endif()

    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC GMP::GMPXX GMP::GMP)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY)

    if(ZLIB_FOUND)
        target_include_directories(stdlibx_stdx PUBLIC ${ZLIB_INCLUDE_DIRS})
    endif()

    target_link_libraries(stdlibx_stdx PUBLIC ZLIB::ZLIB)
endif()

set_target_properties(stdlibx_stdx PROPERTIES
    CXX_STANDARD ${STDLIBX_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx_stdx PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
endif()
