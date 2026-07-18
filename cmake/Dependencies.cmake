find_package(Lua QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_LUA_LIBRARY AND Lua_FOUND)
    if(NOT TARGET Lua::Lua)
        add_library(Lua::Lua INTERFACE IMPORTED)
        set_target_properties(Lua::Lua PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
        )
    endif()
else()
    message(WARNING "Lua not found. Skipping Lua extension.")
endif()

find_package(Microsoft.GSL QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY AND Microsoft.GSL_FOUND)
    message(STATUS "Found Microsoft GSL")
elseif(STDLIBX_EXTENSIONS_COMPILE_MICROSOFT_GUIDELINES_SUPPORT_LIBRARY)
    message(STATUS "Microsoft GSL not found. Fetching Microsoft GSL from microsoft/gsl...")
    FetchContent_Declare(MSGSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.2.2"
        GIT_SHALLOW ON
    )

    FetchContent_MakeAvailable(MSGSL)
endif()

find_package(Sodium QUIET MODULE)
if(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY AND Sodium_FOUND)
    if(sodium_VERSION)
        message(STATUS "Found Sodium: ${sodium_LIBRARY_RELEASE} (found version \"${sodium_VERSION}\")")
    else()
        message(STATUS "Found Sodium: ${sodium_LIBRARY_RELEASE}")
    endif()
elseif(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
    message(STATUS "Sodium not found. Fetching libsodium from jedisct1/libsodium...")

    set(SODIUM_DISABLE_TESTS ON CACHE BOOL "Disable libsodium tests" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "Disable third-party tests" FORCE)

    FetchContent_Declare(
        libsodium
        GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
        GIT_TAG 1.0.22
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
endif()

find_package(GMP QUIET MODULE)
if(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY AND GMP_FOUND)
    if(GMP_VERSION)
        message(STATUS "Found GMP: ${GMP_CXX_LIBRARIES} (found version \"${GMP_VERSION}\")")
    else()
        message(STATUS "Found GMP: ${GMP_CXX_LIBRARIES}")
    endif()
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
endif()

find_package(ZLIB QUIET)
if(STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY AND ZLIB_FOUND)
    if(ZLIB_VERSION)
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES} (found version \"${ZLIB_VERSION}\")")
    else()
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES}")
    endif()
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
endif()

if(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    find_package(stdexec QUIET CONFIG)
    if(stdexec_FOUND)
        message(STATUS "Found stdexec (CMake package)")
    else()
        find_path(STDEXEC_INCLUDE_DIR NAMES stdexec/execution.hpp)
        if(STDEXEC_INCLUDE_DIR)
            message(STATUS "Found stdexec: ${STDEXEC_INCLUDE_DIR} (system headers)")
            if(NOT TARGET STDEXEC::stdexec)
                add_library(STDEXEC::stdexec INTERFACE IMPORTED GLOBAL)
                set_target_properties(STDEXEC::stdexec PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${STDEXEC_INCLUDE_DIR}"
                )
            endif()
        else()
            message(STATUS "stdexec not found. Fetching stdexec from NVIDIA/stdexec (tag nvhpc-26.05)...")

            set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "Disable stdexec tests" FORCE)
            set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "Disable stdexec examples" FORCE)
            set(BUILD_TESTING OFF CACHE BOOL "Disable third-party tests" FORCE)
            set(STDEXEC_BUILD_PARALLEL_SCHEDULER ON CACHE BOOL "Build stdexec's parallel-scheduler backend" FORCE)

            FetchContent_Declare(
                stdexec
                GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
                GIT_TAG nvhpc-26.05
                GIT_SHALLOW TRUE
            )
            FetchContent_MakeAvailable(stdexec)

            if(NOT TARGET STDEXEC::stdexec AND TARGET stdexec)
                add_library(STDEXEC::stdexec ALIAS stdexec)
            endif()

            if(NOT TARGET STDEXEC::stdexec)
                message(FATAL_ERROR "Fetched stdexec, but no usable STDEXEC::stdexec target was found.")
            endif()

            FetchContent_GetProperties(stdexec SOURCE_DIR stdexec_SOURCE_DIR)
            message(STATUS "Found stdexec: ${stdexec_SOURCE_DIR} (fetched tag nvhpc-26.05)")
        endif()
    endif()

    if(TARGET STDEXEC::stdexec)
        get_target_property(_stdexec_aliased STDEXEC::stdexec ALIASED_TARGET)
        if(_stdexec_aliased)
            set(_stdexec_real "${_stdexec_aliased}")
        else()
            set(_stdexec_real STDEXEC::stdexec)
        endif()
        get_target_property(_stdexec_type "${_stdexec_real}" TYPE)
        if(_stdexec_type STREQUAL "INTERFACE_LIBRARY")
            set(STDLIBX_STDEXEC_NEEDS_BACKEND TRUE CACHE INTERNAL
                "stdexec is headers-only; compile its parallel-scheduler backend ourselves")
        else()
            set(STDLIBX_STDEXEC_NEEDS_BACKEND FALSE CACHE INTERNAL
                "stdexec ships a compiled parallel-scheduler backend")
        endif()
        message(STATUS "stdexec parallel-scheduler backend: compile-our-own = ${STDLIBX_STDEXEC_NEEDS_BACKEND}")
    endif()
endif()
