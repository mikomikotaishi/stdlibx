cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_MODULE_STD ON)

option(BUILD_STATIC_LIBS "Build static libraries instead of shared" OFF)

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
    LANGUAGES CXX
    VERSION 0.0.1
)

function(configure_cpp_module_target target)
    set_target_properties(${target} PROPERTIES 
        CXX_EXTENSIONS OFF 
        CXX_MODULE_BMI_DIRECTORY "${CMAKE_CXX_MODULE_BMI_DIRECTORY}"
        CMAKE_CXX_STANDARD 23
        CMAKE_CXX_MODULE_STD ON
    )
endfunction()

option(ENABLE_SANITIZERS "Enable sanitisers in debug builds" OFF)
option(USE_SANITIZER_ADDRESS "Enable address sanitiser" OFF)
option(USE_SANITIZER_KERNEL "Enable kernel address sanitiser" OFF)
option(USE_SANITIZER_HW "Enable hardware address sanitiser" OFF)
option(USE_SANITIZER_UNDEFINED "Enable undefined behaviour sanitiser" OFF)
option(USE_SANITIZER_THREAD "Enable thread sanitiser" OFF)
option(USE_SANITIZER_MEMORY "Enable memory sanitiser" OFF)
option(USE_SANITIZER_LEAK "Enable leak sanitiser" OFF)

function(add_sanitisers target)
    if(NOT ENABLE_SANITIZERS)
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        set(SANITIZER_FLAGS "")
        
        if(USE_SANITIZER_ADDRESS)
            list(APPEND SANITIZER_FLAGS "-fsanitize=address")
        endif()

        if(USE_SANITIZER_KERNEL)
            list(APPEND SANITIZER_FLAGS "-fsanitize=kernel-address")
        endif()

        if(USE_SANITIZER_HW)
            list(APPEND SANITIZER_FLAGS "-fsanitize=hwaddress")
        endif()

        if(USE_SANITIZER_UNDEFINED)
            list(APPEND SANITIZER_FLAGS "-fsanitize=undefined")
        endif()

        if(USE_SANITIZER_THREAD)
            list(APPEND SANITIZER_FLAGS "-fsanitize=thread")
        endif()

        if(USE_SANITIZER_MEMORY)
            list(APPEND SANITIZER_FLAGS "-fsanitize=memory")
        endif()
        
        if(USE_SANITIZER_LEAK)
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
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        add_compile_options(-O3)
    elseif(MSVC)
        add_compile_options(/O2)
    endif()
    
    set(ENABLE_SANITIZERS OFF)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libstdc++>)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:--gcc-toolchain=/usr>)
    add_link_options(-stdlib=libstdc++ --gcc-toolchain=/usr)
    # add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libstdc++>)
    # add_link_options(-stdlib=libc++ -lc)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

option(STDLIBX_NO_STD "Disable the stdx.* modules and only use core.* and alloc.*" OFF)
option(STDLIBX_NO_ALLOC "Disable the alloc.* modules and only use core.*" OFF)
option(STDLIBX_IMPLICIT_USING_CORE "Make stdx.core.* modules implicitly load symbols into scope by importing" ON)
option(STDLIBX_COMPILE_EXPERIMENTAL_HEADERS "Enable experimental standard library headers" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY "Enable compiler header modules in stdx.compiler.*" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY "Enable compiler header modules in stdx.lua.*" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY "Enable SQL ODBC wrapper modules in stdx.sql.*" ON)
option(STDLIBX_BUILD_TESTS "Build the tests for stdlibx" ON)

if(STDLIBX_NO_STD)
    set(STDLIBX_USE_RESERVED_STD_IDENTIFIERS OFF CACHE BOOL "Disabled because STDLIBX_NO_STD is ON" FORCE)
endif()

if(STDLIBX_NO_ALLOC)
    set(STDLIBX_NO_STD ON CACHE BOOL "Implied because STDLIBX_NO_ALLOC disables stdx.*" FORCE)
endif()

add_library(stdlibx INTERFACE)

add_subdirectory(src)

target_include_directories(stdlibx 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_USE_RESERVED_STD_IDENTIFIERS)
endif()

if(STDLIBX_NO_STD)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_NO_STD)
endif()

if(STDLIBX_NO_ALLOC)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_NO_ALLOC)
endif()

if(STDLIBX_IMPLICIT_USING_CORE)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_COMPILE_EXPERIMENTAL_HEADERS)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY)
endif()

set_target_properties(stdlibx PROPERTIES 
    EXPORT_NAME stdlibx
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(COMMAND configure_cpp_module_target)
    configure_cpp_module_target(stdlibx)
endif()

add_library(stdlibx::stdlibx ALIAS stdlibx)

if(STDLIBX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

