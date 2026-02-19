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

find_package(ODBC QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY AND ODBC_FOUND)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC ODBC::ODBC)
elseif(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    message(WARNING "ODBC not found. Skipping SQL extension.")
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
