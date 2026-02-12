cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_MODULE_STD ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR BUILD_SHARED_LIBS STREQUAL "OFF")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
    
    message(STATUS "Configuring for static linking (single executable)")
endif()

project(stdlibx
    LANGUAGES CXX
    VERSION 0.0.1
)

function(configure_cpp_module_target target)
    set_target_properties(${target} PROPERTIES 
        CXX_EXTENSIONS OFF 
        CXX_MODULE_DYNDEP OFF
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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libstdc++>)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:--gcc-toolchain=/usr>)
    add_link_options(-stdlib=libstdc++ --gcc-toolchain=/usr)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
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

option(STDLIBX_USE_RESERVED_STD_IDENTIFIERS "Force usage of reserved module names std.* and std::* namespace (instead of std.* and std::*) and suppress related warnings" ON)
option(STDLIBX_NO_STD "Disable the std.* modules and only use core.* and alloc.*" OFF)
option(STDLIBX_NO_ALLOC "Disable the alloc.* modules and only use core.*" OFF)
option(STDLIBX_IMPLICIT_USING_CORE "Make std.core.* modules implicitly load symbols into scope by importing" ON)
option(STDLIBX_EXTENSIONS "Enable the stdx.* modules" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY "Enable compiler header modules in stdx.compiler.*" OFF)
option(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY "Enable compiler header modules in stdx.lua.*" ON)
option(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY "Enable SQL ODBC wrapper modules in stdx.sql.*" ON)

if(STDLIBX_NO_STD)
    set(STDLIBX_USE_RESERVED_STD_IDENTIFIERS OFF CACHE BOOL "Disabled because STDLIBX_NO_STD is ON" FORCE)
endif()

if(STDLIBX_NO_ALLOC)
    set(STDLIBX_NO_STD ON CACHE BOOL "Implied because STDLIBX_NO_ALLOC disables std.*" FORCE)
endif()

if(NOT STDLIBX_EXTENSIONS)
    set(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY OFF CACHE BOOL "Disabled because STDLIBX_EXTENSIONS is OFF" FORCE)
    set(STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY OFF CACHE BOOL "Disabled because STDLIBX_EXTENSIONS is OFF" FORCE)
endif()

if(STDLIBX_NO_STD AND STDLIBX_ENABLE_COMPAT_NAMES)
    message(WARNING "STDLIBX_ENABLE_COMPAT_NAMES has no effect when STDLIBX_NO_STD is ON")
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

if(STDLIBX_ENABLE_COMPAT_NAMES)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_ENABLE_COMPAT_NAMES)
endif()

if(STDLIBX_EXTENSIONS)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS)
endif()

if(STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
    target_compile_definitions(stdlibx INTERFACE STDLIBX_EXTENSIONS_COMPILE_COMPILER_LIBRARY)
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

