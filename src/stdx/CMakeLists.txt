file(GLOB_RECURSE STDX_MODULES *.cppm)

add_library(stdlibx_stdx)
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

if(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    find_package(LLVM REQUIRED)
    target_link_libraries(stdlibx_stdx PUBLIC LLVMSupport LLVMCore)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    find_package(Lua REQUIRED)
    add_library(Lua::Lua INTERFACE IMPORTED)
    set_target_properties(
        Lua::Lua
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
    )
    target_link_libraries(stdlibx_stdx PUBLIC Lua::Lua)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    find_package(ODBC REQUIRED)
    target_link_libraries(stdlibx_stdx PUBLIC ODBC::ODBC)
endif()

set_target_properties(stdlibx_stdx PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF 
    CXX_MODULE_DYNDEP OFF
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx_stdx PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
endif()
