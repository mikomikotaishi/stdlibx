#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Module/library loading (libloaderapi.h).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<libloaderapi.h>)
    using DllDirectoryCookie = ::DLL_DIRECTORY_COOKIE;
    using EnumResLangProc = ::ENUMRESLANGPROCA;
    using EnumResLangProcW = ::ENUMRESLANGPROCW;
    using EnumResNameProc = ::ENUMRESNAMEPROCA;
    using EnumResNameProcW = ::ENUMRESNAMEPROCW;
    using EnumResTypeProc = ::ENUMRESTYPEPROCA;
    using EnumResTypeProcW = ::ENUMRESTYPEPROCW;

    // Library loading
    using ::LoadLibraryA;
    using ::LoadLibraryW;
    using ::LoadLibraryExA;
    using ::LoadLibraryExW;
    using ::FreeLibrary;
    using ::FreeLibraryAndExitThread;
    using ::DisableThreadLibraryCalls;

    // Module info
    using ::GetModuleHandleA;
    using ::GetModuleHandleW;
    using ::GetModuleHandleExA;
    using ::GetModuleHandleExW;
    using ::GetModuleFileNameA;
    using ::GetModuleFileNameW;

    // Symbol lookup
    using ::GetProcAddress;

    // DLL search path management
    using ::AddDllDirectory;
    using ::RemoveDllDirectory;
    using ::SetDefaultDllDirectories;
    using ::SetDllDirectoryA;
    using ::SetDllDirectoryW;
    using ::GetDllDirectoryA;
    using ::GetDllDirectoryW;

    // Resource loading
    using ::FindResourceA;
    using ::FindResourceW;
    using ::FindResourceExA;
    using ::FindResourceExW;
    using ::LoadResource;
    using ::LockResource;
    using ::SizeofResource;
    using ::FreeResource;
    using ::EnumResourceNamesA;
    using ::EnumResourceNamesW;
    using ::EnumResourceTypesA;
    using ::EnumResourceTypesW;
    using ::EnumResourceLanguagesA;
    using ::EnumResourceLanguagesW;
    using ::LoadStringA;
    using ::LoadStringW;
    #endif
}
