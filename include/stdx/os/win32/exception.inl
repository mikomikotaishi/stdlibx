#pragma once

/**
 * @namespace stdx::os::win32
 * @brief CONTEXT / EXCEPTION_* / NT_TIB and exception-filter wrappers.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // Thread Information Block
    using NtThreadInformationBlock = ::NT_TIB;
    using NtThreadInformationBlockPointer = ::PNT_TIB;

    // FPU / SSE register save areas
    #ifdef _WIN64
    using M128A = ::M128A;
    using M128APointer = ::PM128A;
    #endif

    using XSaveFormat = ::XSAVE_FORMAT;
    using XSaveFormatPointer = ::PXSAVE_FORMAT;
    using XmmSaveArea32 = ::XMM_SAVE_AREA32;
    using XmmSaveArea32Pointer = ::PXMM_SAVE_AREA32;
    using FloatingSaveArea = ::FLOATING_SAVE_AREA;
    using FloatingSaveAreaPointer = ::PFLOATING_SAVE_AREA;

    // Thread context (full register state)
    using Context = ::CONTEXT;
    using ContextPointer = ::PCONTEXT;

    // Exception records
    using ExceptionRecord = ::EXCEPTION_RECORD;
    using ExceptionRecordPointer = ::PEXCEPTION_RECORD;
    using ExceptionPointers = ::EXCEPTION_POINTERS;
    using ExceptionPointersPointer = ::PEXCEPTION_POINTERS;

    // Exception filter callbacks
    using TopLevelExceptionFilter = ::PTOP_LEVEL_EXCEPTION_FILTER;
    using VectoredExceptionHandler = ::PVECTORED_EXCEPTION_HANDLER;

    // Exception-handling functions
    using ::RaiseException;
    using ::SetUnhandledExceptionFilter;
    using ::UnhandledExceptionFilter;
    using ::AddVectoredExceptionHandler;
    using ::AddVectoredContinueHandler;
    using ::RemoveVectoredExceptionHandler;
    using ::RemoveVectoredContinueHandler;
    using ::RtlCaptureContext;
    using ::RtlLookupFunctionEntry;
    using ::RtlVirtualUnwind;
    #endif
}
