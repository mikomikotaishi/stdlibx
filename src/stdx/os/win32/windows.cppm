/**
 * @file windows.cppm
 * @module stdx:os.win32.windows
 * @brief Module file for Win32 operations.
 *
 * This file contains the implementation of the Windows operations from the Windows API libraries,
 * located in <windows.h>.
 */

module;

#ifdef _WIN32
#include <windows.h>
#endif

#undef InterlockedExchange8
#undef InterlockedExchangeAdd8
#undef InterlockedExchangeAnd8
#undef InterlockedExchangeOr8
#undef InterlockedExchangeXor8
#undef InterlockedDecrement8
#undef InterlockedIncrement8
#undef InterlockedCompareExchange8
#undef InterlockedExchange16
#undef InterlockedExchangeAdd16
#undef InterlockedExchangeOr16
#undef InterlockedExchangeXor16
#undef InterlockedDecrement16
#undef InterlockedIncrement16
#undef InterlockedCompareExchange16
#undef InterlockedExchange
#undef InterlockedExchangeAdd
#undef InterlockedExchangeAnd
#undef InterlockedExchangeOr
#undef InterlockedExchangeXor
#undef InterlockedDecrement
#undef InterlockedIncrement 
#undef InterlockedCompareExchange 

#ifdef _AMD64_
#undef InterlockedExchange64
#undef InterlockedExchangeAdd64
#undef InterlockedExchangeAnd64
#undef InterlockedExchangeOr64
#undef InterlockedExchangeXor64
#undef InterlockedDecrement64
#undef InterlockedIncrement64
#undef InterlockedCompareExchange64
#undef InterlockedCompareExchange128
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.win32.windows;
#else
export module stdlibx:os.win32.windows;
#endif

import :os.win32.windows.constants;
import :os.win32.windows.windows_base;

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::win32 {
#else 
export namespace stdlibx::os::win32 {
#endif
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

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    enum class SymTag {
        Null = static_cast<int>(SymTagEnum::SymTagNull),
        Exe = static_cast<int>(SymTagEnum::SymTagExe),
        Compiland = static_cast<int>(SymTagEnum::SymTagCompiland),
        CompilandDetails = static_cast<int>(SymTagEnum::SymTagCompilandDetails),
        CompilandEnv = static_cast<int>(SymTagEnum::SymTagCompilandEnv),
        Function = static_cast<int>(SymTagEnum::SymTagFunction),
        Block = static_cast<int>(SymTagEnum::SymTagBlock),
        Data = static_cast<int>(SymTagEnum::SymTagData),
        Annotation = static_cast<int>(SymTagEnum::SymTagAnnotation),
        Label = static_cast<int>(SymTagEnum::SymTagLabel),
        PublicSymbol = static_cast<int>(SymTagEnum::SymTagPublicSymbol),
        UDT = static_cast<int>(SymTagEnum::SymTagUDT),
        Enum = static_cast<int>(SymTagEnum::SymTagEnum),
        FunctionType = static_cast<int>(SymTagEnum::SymTagFunctionType),
        PointerType = static_cast<int>(SymTagEnum::SymTagPointerType),
        ArrayType = static_cast<int>(SymTagEnum::SymTagArrayType),
        BaseType = static_cast<int>(SymTagEnum::SymTagBaseType),
        Typedef = static_cast<int>(SymTagEnum::SymTagTypedef),
        BaseClass = static_cast<int>(SymTagEnum::SymTagBaseClass),
        Friend = static_cast<int>(SymTagEnum::SymTagFriend),
        FunctionArgType = static_cast<int>(SymTagEnum::SymTagFunctionArgType),
        FuncDebugStart = static_cast<int>(SymTagEnum::SymTagFuncDebugStart),
        FuncDebugEnd = static_cast<int>(SymTagEnum::SymTagFuncDebugEnd),
        UsingNamespace = static_cast<int>(SymTagEnum::SymTagUsingNamespace),
        VTableShape = static_cast<int>(SymTagEnum::SymTagVTableShape),
        VTable = static_cast<int>(SymTagEnum::SymTagVTable),
        Custom = static_cast<int>(SymTagEnum::SymTagCustom),
        Thunk = static_cast<int>(SymTagEnum::SymTagThunk),
        CustomType = static_cast<int>(SymTagEnum::SymTagCustomType),
        ManagedType = static_cast<int>(SymTagEnum::SymTagManagedType),
        Dimension = static_cast<int>(SymTagEnum::SymTagDimension),
        CallSite = static_cast<int>(SymTagEnum::SymTagCallSite),
        InlineSite = static_cast<int>(SymTagEnum::SymTagInlineSite),
        BaseInterface = static_cast<int>(SymTagEnum::SymTagBaseInterface),
        VectorType = static_cast<int>(SymTagEnum::SymTagVectorType),
        MatrixType = static_cast<int>(SymTagEnum::SymTagMatrixType),
        HLSLType = static_cast<int>(SymTagEnum::SymTagHLSLType)
    };
    #else
    enum class SymTag {
        NULL = static_cast<int>(SymTagEnum::SymTagNull),
        EXE = static_cast<int>(SymTagEnum::SymTagExe),
        COMPILAND = static_cast<int>(SymTagEnum::SymTagCompiland),
        COMPILAND_DETAILS = static_cast<int>(SymTagEnum::SymTagCompilandDetails),
        COMPILAND_ENV = static_cast<int>(SymTagEnum::SymTagCompilandEnv),
        FUNCTION = static_cast<int>(SymTagEnum::SymTagFunction),
        BLOCK = static_cast<int>(SymTagEnum::SymTagBlock),
        DATA = static_cast<int>(SymTagEnum::SymTagData),
        ANNOTATION = static_cast<int>(SymTagEnum::SymTagAnnotation),
        LABEL = static_cast<int>(SymTagEnum::SymTagLabel),
        PUBLIC_SYMBOL = static_cast<int>(SymTagEnum::SymTagPublicSymbol),
        UDT = static_cast<int>(SymTagEnum::SymTagUDT),
        ENUM = static_cast<int>(SymTagEnum::SymTagEnum),
        FUNCTION_TYPE = static_cast<int>(SymTagEnum::SymTagFunctionType),
        POINTER_TYPE = static_cast<int>(SymTagEnum::SymTagPointerType),
        ARRAY_TYPE = static_cast<int>(SymTagEnum::SymTagArrayType),
        BASE_TYPE = static_cast<int>(SymTagEnum::SymTagBaseType),
        TYPEDEF = static_cast<int>(SymTagEnum::SymTagTypedef),
        BASE_CLASS = static_cast<int>(SymTagEnum::SymTagBaseClass),
        FRIEND = static_cast<int>(SymTagEnum::SymTagFriend),
        FUNCTION_ARG_TYPE = static_cast<int>(SymTagEnum::SymTagFunctionArgType),
        FUNC_DEBUG_START = static_cast<int>(SymTagEnum::SymTagFuncDebugStart),
        FUNC_DEBUG_END = static_cast<int>(SymTagEnum::SymTagFuncDebugEnd),
        USING_NAMESPACE = static_cast<int>(SymTagEnum::SymTagUsingNamespace),
        VTABLE_SHAPE = static_cast<int>(SymTagEnum::SymTagVTableShape),
        VTABLE = static_cast<int>(SymTagEnum::SymTagVTable),
        CUSTOM = static_cast<int>(SymTagEnum::SymTagCustom),
        THUNK = static_cast<int>(SymTagEnum::SymTagThunk),
        CUSTOM_TYPE = static_cast<int>(SymTagEnum::SymTagCustomType),
        MANAGED_TYPE = static_cast<int>(SymTagEnum::SymTagManagedType),
        DIMENSION = static_cast<int>(SymTagEnum::SymTagDimension),
        CALL_SITE = static_cast<int>(SymTagEnum::SymTagCallSite),
        INLINE_SITE = static_cast<int>(SymTagEnum::SymTagInlineSite),
        BASE_INTERFACE = static_cast<int>(SymTagEnum::SymTagBaseInterface),
        VECTOR_TYPE = static_cast<int>(SymTagEnum::SymTagVectorType),
        MATRIX_TYPE = static_cast<int>(SymTagEnum::SymTagMatrixType),
        HLSL_TYPE = static_cast<int>(SymTagEnum::SymTagHLSLType)
    };
    #endif

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
