#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Debugger and DbgHelp symbol/stack API wrappers (dbghelp.h + debugapi.h).
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // DbgHelp value types and callbacks
    using SymEnumerateSymbolsCallback = ::PSYM_ENUMERATESYMBOLS_CALLBACK;
    using SymEnumerateSymbolsCallbackW = ::PSYM_ENUMERATESYMBOLS_CALLBACKW;
    using SymbolInfo = ::SYMBOL_INFO;
    using SymbolInfoPointer = ::PSYMBOL_INFO;
    using SymbolInfoW = ::SYMBOL_INFOW;
    using SymbolInfoWPointer = ::PSYMBOL_INFOW;
    using ImageHlpLine64 = ::IMAGEHLP_LINE64;
    using ImageHlpLine64Pointer = ::PIMAGEHLP_LINE64;

    // Core debugger control
    using ::DebugBreak;
    using ::IsDebuggerPresent;
    using ::CheckRemoteDebuggerPresent;
    using ::OutputDebugStringA;
    using ::OutputDebugStringW;
    using ::DebugActiveProcess;
    using ::DebugActiveProcessStop;

    // Context
    using ::GetThreadContext;
    using ::SetThreadContext;

    // Console (kept here from the original windows.inl placement)
    using ::GetConsoleMode;
    using ::SetConsoleMode;
    using ::GetStdHandle;

    // DbgHelp symbols
    using ::SymSetOptions;
    using ::SymGetOptions;
    using ::SymInitialize;
    using ::SymInitializeW;
    using ::SymCleanup;
    using ::SymFromAddr;
    using ::SymFromAddrW;
    using ::SymFromName;
    using ::SymFromNameW;
    using ::SymGetLineFromAddr64;
    using ::SymGetLineFromAddrW64;
    using ::SymGetModuleBase64;
    using ::SymFunctionTableAccess64;
    using ::SymLoadModule64;
    using ::SymLoadModuleExW;
    using ::SymUnloadModule64;
    using ::SymEnumSymbols;
    using ::SymEnumSymbolsW;
    using ::SymRefreshModuleList;
    using ::SymGetSearchPath;
    using ::SymGetSearchPathW;
    using ::SymSetSearchPath;
    using ::SymSetSearchPathW;
    using ::StackWalk64;
    using ::MiniDumpWriteDump;
    #endif
}
