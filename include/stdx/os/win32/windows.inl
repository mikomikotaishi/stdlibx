#pragma once

#include "windows_base.inl"

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    using TagThreadEntry32 = ::tagTHREADENTRY32;
    using PThreadEntry32 = ::PTHREADENTRY32;

    using ::DebugBreak;
    using ::IsDebuggerPresent;
    using ::CheckRemoteDebuggerPresent;
    using ::OutputDebugStringA;
    using ::OutputDebugStringW;
    using ::GetThreadContext;
    using ::DebugActiveProcess;
    using ::DebugActiveProcessStop;
    using ::SymSetOptions;
    using ::SymInitialize;
    using ::SymInitializeW;
    using ::SymFromAddr;
    using ::SymFromAddrW;
    using ::SymFunctionTableAccess64;
    using ::SymGetModuleBase64;
    
    // for the moment
    using ::GetConsoleMode;
    using ::SetConsoleMode;
    using ::GetStdHandle;
    
    using PSymEnumerateSymbolsCallback = PSYM_ENUMERATESYMBOLS_CALLBACK;
    using PSymEnumerateSymbolsCallbackW = PSYM_ENUMERATESYMBOLS_CALLBACKW;

    using InterlockedExchange8 = ::_InterlockedExchange8;
    using InterlockedExchangeAdd8 = ::_InterlockedExchangeAdd8;
    using InterlockedExchangeAnd8 = ::_InterlockedExchangeAnd8;
    using InterlockedExchangeOr8 = ::_InterlockedExchangeOr8;
    using InterlockedExchangeXor8 = ::_InterlockedExchangeXor8;
    using InterlockedDecrement8 = ::_InterlockedDecrement8;
    using InterlockedIncrement8 = ::_InterlockedIncrement8;
    using InterlockedCompareExchange8 = ::_InterlockedCompareExchange8;
    using InterlockedExchange16 = ::_InterlockedExchange16;
    using InterlockedExchangeAdd16 = ::_InterlockedExchangeAdd16;
    using InterlockedExchangeAnd16 = ::_InterlockedExchangeAnd16;
    using InterlockedExchangeOr16 = ::_InterlockedExchangeOr16;
    using InterlockedExchangeXor16 = ::_InterlockedExchangeXor16;
    using InterlockedDecrement16 = ::_InterlockedDecrement16;
    using InterlockedIncrement16 = ::_InterlockedIncrement16;
    using InterlockedCompareExchange16 = ::_InterlockedCompareExchange16;
    using InterlockedExchange = ::_InterlockedExchange;
    using InterlockedExchangeAdd = ::_InterlockedExchangeAdd;
    using InterlockedExchangeAnd = ::_InterlockedExchangeAnd;
    using InterlockedExchangeOr = ::_InterlockedExchangeOr;
    using InterlockedExchangeXor = ::_InterlockedExchangeXor;
    using InterlockedDecrement = ::_InterlockedDecrement;
    using InterlockedIncrement = ::_InterlockedIncrement;
    using InterlockedCompareExchange = ::_InterlockedCompareExchange;

    #ifdef _AMD64_
    using InterlockedExchange64 = ::_InterlockedExchange64;
    using InterlockedExchangeAdd64 = ::_InterlockedExchangeAdd64;
    using InterlockedExchangeAnd64 = ::_InterlockedExchangeAnd64;
    using InterlockedExchangeOr64 = ::_InterlockedExchangeOr64;
    using InterlockedExchangeXor64 = ::_InterlockedExchangeXor64;
    using InterlockedDecrement64 = ::_InterlockedDecrement64;
    using InterlockedIncrement64 = ::_InterlockedIncrement64;
    using InterlockedCompareExchange64 = ::_InterlockedCompareExchange64;
    using InterlockedCompareExchange128 = ::_InterlockedCompareExchange128;
    #endif

    using NTThreadInformatonBlock = ::NT_TIB;
    using LPNTThreadInformatonBlock = ::LPNT_TIB;
    using PNTThreadInformatonBlock = ::PNT_TIB;

    #ifdef _WIN64
    using M128A = ::M128A;
    using PM128A = ::PM128A;
    #endif

    using XSaveFormat = ::XSAVE_FORMAT;
    using PXSaveFormat = ::PXSAVE_FORMAT;
    using XMMSaveArea32 = ::XMM_SAVE_AREA32;
    using PXMMSaveArea32 = ::PXMM_SAVE_AREA32;

    using Context = ::CONTEXT;
    using PContext = ::PCONTEXT;
    using LPContext = ::LPCONTEXT;

    using FloatingSaveArea = ::FLOATING_SAVE_AREA;
    using PFloatingSaveArea = ::PFLOATING_SAVE_AREA;

    using ExceptionRecord = ::EXCEPTION_RECORD;
    using PExceptionRecord = ::PEXCEPTION_RECORD;
    
    using ExceptionPointers = ::EXCEPTION_POINTERS;
    using PExceptionPointers = ::PEXCEPTION_POINTERS;
    using PTopLevelExceptionFilter = ::PTOP_LEVEL_EXCEPTION_FILTER;
    using LPTopLevelExceptionFilter = ::LPTOP_LEVEL_EXCEPTION_FILTER;
    using PVectoredExceptionHandler = ::PVectoredExceptionHandler;

    using SymbolInfo = ::SYMBOL_INFO;
    using PSymbolInfo = ::PSYMBOL_INFO;
    using SymbolInfoW = ::SYMBOL_INFOW;
    using PSymbolInfoW = ::PSYMBOL_INFOW;

    using ImageHlpLine64 = ::IMAGEHLP_LINE64;
    using PImageHlpLine64 = ::PIMAGEHLP_LINE64;
    #endif
}
