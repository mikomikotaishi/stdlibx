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
    # PUBLIC: needed by stdx.cppm itself (controls export using namespace
    # stdx::core) AND useful for consumers - they can check the macro to
    # decide whether to redeclare the using-directive in their own TU.
    target_compile_definitions(stdlibx_stdx PUBLIC STDLIBX_IMPLICIT_USING_CORE)
endif()

if(STDLIBX_IMPLICIT_USING_LITERALS)
    target_compile_definitions(stdlibx_stdx PUBLIC STDLIBX_IMPLICIT_USING_LITERALS)
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

if(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY AND TARGET STDEXEC::stdexec)
    target_compile_definitions(stdlibx_stdx PRIVATE STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    target_link_libraries(stdlibx_stdx PUBLIC STDEXEC::stdexec)

    if(STDLIBX_STDEXEC_NEEDS_BACKEND)
        add_library(stdlibx_stdexec_backend OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/exec_details/exec_parallel_scheduler_backend.cpp)
        set_target_properties(stdlibx_stdexec_backend PROPERTIES
            CXX_STANDARD 26
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            CXX_SCAN_FOR_MODULES OFF
            POSITION_INDEPENDENT_CODE ON)
        target_link_libraries(stdlibx_stdexec_backend PRIVATE STDEXEC::stdexec)
        target_sources(stdlibx_stdx PRIVATE $<TARGET_OBJECTS:stdlibx_stdexec_backend>)
    endif()
elseif(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    message(WARNING "stdexec not found. Skipping stdexec extension.")
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

# Platform audio / OS-API libraries. The os module's .inl wrappers
# re-export raw C functions (snd_pcm_open, CreateFile, AudioObjectGetPropertyData,
# MIDIClientCreate, ...) via using-declarations - those declarations need
# the underlying symbols at link time. Use PUBLIC so downstream consumers
# of stdlibx_stdx pick them up transitively.

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # ALSA - PCM (sampled audio) + sequencer + raw MIDI. CMake ships
    # FindALSA, which provides ALSA::ALSA and ALSA_FOUND.
    find_package(ALSA QUIET)
    if(ALSA_FOUND)
        message(STATUS "ALSA found - linking stdx::os::linux::alsa to ALSA::ALSA")
        target_link_libraries(stdlibx_stdx PUBLIC ALSA::ALSA)
    else()
        message(WARNING "ALSA not found - stdx::audio::sampled on Linux will be a stub.\n"
                        "Install libasound2-dev / alsa-lib-devel and reconfigure.")
    endif()
elseif(APPLE)
    # macOS / iOS frameworks. Audio/MIDI live in CoreAudio + AudioUnit +
    # AudioToolbox + CoreMIDI; CoreFoundation underpins everything.
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation REQUIRED)
    find_library(COREAUDIO_FRAMEWORK     CoreAudio     REQUIRED)
    find_library(AUDIOUNIT_FRAMEWORK     AudioUnit     REQUIRED)
    find_library(AUDIOTOOLBOX_FRAMEWORK  AudioToolbox  REQUIRED)
    find_library(COREMIDI_FRAMEWORK      CoreMIDI      REQUIRED)
    target_link_libraries(stdlibx_stdx PUBLIC
        ${COREFOUNDATION_FRAMEWORK}
        ${COREAUDIO_FRAMEWORK}
        ${AUDIOUNIT_FRAMEWORK}
        ${AUDIOTOOLBOX_FRAMEWORK}
        ${COREMIDI_FRAMEWORK}
    )
elseif(WIN32)
    # Win32 system libraries grouped by what the os/win32 wrappers touch:
    #   winmm    : mmsystem.h (waveOut/waveIn + midiOut/midiIn) - used
    #              by stdx/os/win32/mmsystem.inl, also the WinMM MIDI
    #              backend the audio module will use.
    #   ole32    : CoCreateInstance + COM ABI used by WASAPI
    #              (IMMDeviceEnumerator, IAudioClient, ...) and WIC.
    #   propsys  : PropVariant / IPropertyStore (needed by WASAPI device
    #              enumeration for FriendlyName etc.).
    #   uuid     : Statically-linked GUID definitions (CLSID_MMDeviceEnumerator,
    #              IID_IMMDeviceEnumerator, ...).
    #   mmdevapi : IMMDevice / IMMDeviceEnumerator implementation library.
    #   ws2_32   : Winsock 2 (stdx/os/win32/winsock.inl).
    #   shell32  : ShellExecute, SHFileOperation, ... (shellapi.inl).
    #   comctl32 : Common controls (commctrl.inl).
    #   crypt32  : CryptoAPI (wincrypt.inl).
    #   dwmapi   : Desktop Window Manager (dwmapi.inl).
    #   dbghelp  : Sym* + StackWalk64 + MiniDumpWriteDump (dbghelp.inl).
    #   user32 / gdi32 / advapi32 : the rest of the Win32 surface
    #              (windows.h functions reached via windows_base.inl,
    #              fileapi.inl, process.inl, synchapi.inl, etc.).
    target_link_libraries(stdlibx_stdx PUBLIC
        winmm
        ole32
        oleaut32
        propsys
        uuid
        mmdevapi
        ws2_32
        shell32
        comctl32
        crypt32
        dwmapi
        dbghelp
        user32
        gdi32
        advapi32
        kernel32
    )
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
