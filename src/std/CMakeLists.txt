file(GLOB_RECURSE STD_MODULES *.cppm)
# Name the library stdlibx_std and not std, to ensure it does not conflict with the real ISO C++ std module
add_library(stdlibx_std)
target_sources(stdlibx_std
    PUBLIC
    FILE_SET CXX_MODULES FILES
    ${STD_MODULES}
)

configure_cpp_module_target(stdlibx_std)

target_link_libraries(stdlibx_std PUBLIC stdlibx_core stdlibx_alloc)

target_include_directories(stdlibx_std 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

add_library(stdlibx::std ALIAS stdlibx_std)

if(STDLIBX_NO_STD)
    target_compile_definitions(stdlibx_std PRIVATE STDLIBX_NO_STD)
endif()

if(STDLIBX_NO_ALLOC)
    target_compile_definitions(stdlibx_std PRIVATE STDLIBX_NO_ALLOC)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx_std INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_ENABLE_COMPAT_NAMES)
    target_compile_definitions(stdlibx_std PRIVATE STDLIBX_ENABLE_COMPAT_NAMES)
endif()

if(STDLIBX_EXPERIMENTAL_STD)
    target_compile_definitions(stdlibx_std PRIVATE STDLIBX_EXPERIMENTAL_STD)
endif()

set_target_properties(stdlibx_std PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF 
    CXX_MODULE_DYNDEP OFF
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx_std PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
    target_compile_options(stdlibx_std PRIVATE 
        -Wno-reserved-module-identifier
    )
endif()
