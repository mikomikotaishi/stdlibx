#pragma once

/**
 * @namespace stdx::os::win32
 * @brief MSVC/MinGW Interlocked atomic intrinsics (from <intrin.h>).
 *
 * Function names are re-exported as the public Interlocked* spellings
 * (without the leading underscore that MinGW exposes).
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
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
    #endif
}
