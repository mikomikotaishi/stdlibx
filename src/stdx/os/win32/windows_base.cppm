/**
 * @file windows_base.cppm
 * @module stdx:os.win32.windows.windows_base
 * @brief Module file for Win32 operations.
 *
 * This file contains the implementation of the Windows operations from the Windows API libraries,
 * located in <windows_base.h>.
 */

module;

#ifdef _WIN32
#include <windows_base.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.win32.windows.windows_base;
#else
export module stdlibx:os.win32.windows.windows_base;
#endif

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::win32 {
#else 
export namespace stdlibx::os::win32 {
#endif
    [[nodiscard]]
    constexpr bool HResultIsWin32(long x) noexcept {
        return ((((x) >> 16) & 0xFFFF) == 0x8);
    }

    [[nodiscard]]
    constexpr bool HResultIsFailure(long x) noexcept {
        return ((((x) >> 31) & 0x1) == 0x1);
    }

    [[nodiscard]]
    constexpr long HResultFacility(long x) noexcept {
        return (((x) >> 16) & 0xFFFF);
    }

    [[nodiscard]]
    constexpr long HResultCode(long x) noexcept {
        return ((x) & 0xFFFF);
    }

    [[nodiscard]]
    constexpr long HResultFromWin32(long x) noexcept {
        return (0x80070000 | (x));
    }

    #ifdef _WIN32
    using Bool = ::BOOL;
    using Char = ::CHAR;
    using Short = ::SHORT; // Renamed
    using Int = ::INT;
    using Long = ::LONG; // Renamed
    using UChar = ::UCHAR;
    using UShort = ::USHORT;
    using UInt = ::UINT;
    using ULong = ::ULONG;
    using Byte = ::BYTE; // Renamed
    using Word = ::WORD;
    using DWord = ::DWORD;
    using WChar = ::WCHAR;
    using PWChar = ::PWCHAR;
    using Atom = ::ATOM;
    using ULong32 = ::ULONG32;
    using DWord64 = ::DWORD64;
    using ULong64 = ::ULONG64;
    using Int32 = ::INT32;
    using Int64 = ::INT64;
    using DWordLong = ::DWORDLONG;

    using PChar = ::PCHAR;
    using PULong = ::PULONG;
    using PULong64 = ::PULONG64;
    using PDWord64 = ::PDWORD64;

    using LongLong = ::LONGLONG; // Renamed
    using ULongLong = ::ULONGLONG;

    using Void = ::VOID; // Renamed
    using PVoid = ::PVOID;
    using LPVoid = ::LPVOID;
    using PBool = ::PBOOL;
    using LPBool = ::LPBOOL;
    using PWord = ::PWORD;
    using PLong = ::PLONG;
    using LPLong = ::LPLONG;
    using PDWord = ::PDWORD;

    using Handle = ::HANDLE;
    using HInstance = ::HINSTANCE;
    using HWnd = ::HWND;
    using HModule = ::HMODULE;
    using HDc = ::HDC;
    using HGlrc = ::HGLRC;
    using HMenu = ::HMENU;
    using PHandle = ::PHANDLE;
    using LPHandle = ::LPHANDLE;

    using PWStr = ::PWSTR;
    using LPByte = ::LPBYTE;
    using LPDWord = ::LPDWORD;
    using LPCVoid = ::LPCVOID;

    using IntPtr = ::INT_PTR;
    using LongPtr = ::LONG_PTR;
    using UIntPtr = ::UINT_PTR;
    using ULongPtr = ::ULONG_PTR;
    using DWordPtr = ::DWORD_PTR;
    using PDWordPtr = ::PDWORD_PTR;

    using Size_t = ::SIZE_T;
    using SSize_t = ::SSIZE_T;

    using LPStr = ::LPSTR;
    using LPWStr = ::LPWSTR;
    using LPCStr = ::LPCStTR;
    using LPCWStr = ::LPCWSTR;
    using TChar = ::TCHAR;
    using TByte = ::TBYTE;
    using LPCTStr = ::LPCTSTR;
    using LPTStr = ::LPTSTR;

    using FarProc = ::FARPROC;
    using NearProc = ::NEARPROC;
    using Proc = ::PROC;

    using AccessMask = ::ACCESS_MASK;
    using PAccessMask = ::PACCESS_MASK;

    using HIcon = ::HICON;
    using HBrush = ::HBRUSH;
    using HCursor = ::HCURSOR;

    using HResult = ::HRESULT;
    using LResult = ::LRESULT;
    using LParam = ::LPARAM;
    using WParam = ::WPARAM;

    using HGdiObj = ::HGDIOBJ;

    using HKey = ::HKEY;
    using PHKey = ::PHKEY;
    using RegSam = ::REGSAM;
    
    using Overlapped = ::OVERLAPPED;
    using LPOverlapped = ::LPOVERLAPPED;

    using SecurityAttributes = ::SECURITY_ATTRIBUTES;
    using PSecurityAttributes = ::PSECURITY_ATTRIBUTES;
    using LPSecurityAttributes = ::LPSECURITY_ATTRIBUTES;

    using LargeInteger = ::LARGE_INTEGER;
    using PLargeInteger = ::PLARGE_INTEGER;
    using ULargeInteger = ::ULARGE_INTEGER;
    using PULargeInteger = ::PULARGE_INTEGER;

    using FileTime = ::FILETIME;
    using PFileTime = ::PFILETIME;
    using LPFileTime = ::LPFILETIME;
    
    using GUID = ::GUID;
    #endif
}
