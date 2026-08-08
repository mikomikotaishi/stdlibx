#pragma once

/**
 * @namespace stdx::os::win32
 * @brief MSVC/MinGW Interlocked atomic intrinsics (from <intrin.h>).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<intrin.h>)
    using ::_InterlockedExchange8;
    using ::_InterlockedExchangeAdd8;
    using ::_InterlockedAnd8;
    using ::_InterlockedOr8;
    using ::_InterlockedXor8;
    using ::_InterlockedCompareExchange8;

    using ::_InterlockedExchange16;
    using ::_InterlockedExchangeAdd16;
    using ::_InterlockedAnd16;
    using ::_InterlockedOr16;
    using ::_InterlockedXor16;
    using ::_InterlockedIncrement16;
    using ::_InterlockedDecrement16;
    using ::_InterlockedCompareExchange16;

    using ::_InterlockedExchange;
    using ::_InterlockedExchangeAdd;
    using ::_InterlockedAnd;
    using ::_InterlockedOr;
    using ::_InterlockedXor;
    using ::_InterlockedIncrement;
    using ::_InterlockedDecrement;
    using ::_InterlockedCompareExchange;

    // Guarded on the compiler's target macros, not on winnt.h's _AMD64_. _AMD64_
    // means "x64 SDK headers" and is unset on ARM64, where every operation below
    // is in fact available - so the old guard hid the whole block on
    // Windows-on-ARM. _InterlockedCompareExchange128 has the same availability
    // (x64 and ARM64) and needs no narrower guard of its own.
    #if defined(_M_X64) || defined(_M_ARM64) || defined(__x86_64__) || defined(__aarch64__)
    using ::_InterlockedExchange64;
    using ::_InterlockedExchangeAdd64;
    using ::_InterlockedAnd64;
    using ::_InterlockedOr64;
    using ::_InterlockedXor64;
    using ::_InterlockedIncrement64;
    using ::_InterlockedDecrement64;
    using ::_InterlockedCompareExchange64;
    using ::_InterlockedCompareExchange128;
    #endif
    #endif
}
