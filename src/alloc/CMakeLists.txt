file(GLOB_RECURSE ALLOC_MODULES *.cppm)
add_library(stdlibx_alloc)
target_sources(stdlibx_alloc
    PUBLIC
    FILE_SET CXX_MODULES FILES
    ${ALLOC_MODULES}
)

configure_cpp_module_target(stdlibx_alloc)

target_link_libraries(stdlibx_alloc PUBLIC stdlibx_core)

target_include_directories(stdlibx_alloc 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

add_library(stdlibx::alloc ALIAS stdlibx_alloc)

if(STDLIBX_NO_STD)
    target_compile_definitions(stdlibx_alloc PRIVATE STDLIBX_NO_STD)
endif()

if(STDLIBX_NO_ALLOC)
    target_compile_definitions(stdlibx_alloc PRIVATE STDLIBX_NO_ALLOC)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx_alloc INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_ENABLE_COMPAT_NAMES)
    target_compile_definitions(stdlibx_alloc PRIVATE STDLIBX_ENABLE_COMPAT_NAMES)
endif()

set_target_properties(stdlibx_alloc PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF 
    CXX_MODULE_DYNDEP OFF
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx_alloc PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
    target_compile_options(stdlibx_alloc PRIVATE 
        -Wno-reserved-module-identifier
    )
endif()
