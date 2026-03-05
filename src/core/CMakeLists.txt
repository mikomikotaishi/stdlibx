file(GLOB_RECURSE CORE_MODULES *.cppm)

if(BUILD_STATIC_LIBS)
    add_library(stdlibx_core STATIC)
else()
    add_library(stdlibx_core SHARED)
endif()

target_sources(stdlibx_core
    PUBLIC
    FILE_SET CXX_MODULES FILES
    ${CORE_MODULES}
)

configure_cpp_module_target(stdlibx_core)

target_include_directories(stdlibx_core 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
)

add_library(stdlibx::core ALIAS stdlibx_core)

if(COMPILER_APPLE_CLANG)
    target_compile_definitions(stdlibx_core PRIVATE COMPILER_APPLE_CLANG)
elseif(COMPILER_CLANG)
    target_compile_definitions(stdlibx_core PRIVATE COMPILER_CLANG)
elseif(COMPILER_GCC)
    target_compile_definitions(stdlibx_core PRIVATE COMPILER_GCC)
elseif(COMPILER_MSVC)
    target_compile_definitions(stdlibx_core PRIVATE COMPILER_MSVC)
endif()

if(ARCH_X86_64)
    target_compile_definitions(stdlibx_core PRIVATE ARCH_X86_64)
elseif(ARCH_X86)
    target_compile_definitions(stdlibx_core PRIVATE ARCH_X86)
elseif(ARCH_ARM64)
    target_compile_definitions(stdlibx_core PRIVATE ARCH_ARM64)
elseif(ARCH_ARM)
    target_compile_definitions(stdlibx_core PRIVATE ARCH_ARM)
endif()

if(STDLIBX_NO_STD)
    target_compile_definitions(stdlibx_core PRIVATE STDLIBX_NO_STD)
endif()

if(STDLIBX_NO_ALLOC)
    target_compile_definitions(stdlibx_core PRIVATE STDLIBX_NO_ALLOC)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx_core INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

set_target_properties(stdlibx_core PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(STDLIBX_NO_RESERVED_STD_MODULE)
    target_compile_definitions(stdlibx_core PRIVATE 
        STDLIBX_NO_RESERVED_STD_MODULE
        STDLIBX_NO_RESERVED_STD_NAMESPACE
    )
    target_compile_options(stdlibx_core PRIVATE 
        -Wno-reserved-module-identifier
    )
endif()
