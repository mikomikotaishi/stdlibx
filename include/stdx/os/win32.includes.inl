#pragma once

#ifdef _WIN32
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <intrin.h>
#include <io.h>
#include <shellapi.h>
#include <commctrl.h>
#include <wincrypt.h>
#include <wincodec.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include <windowsx.h>

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
#endif
