/**
 * @file windows.base.cppm
 * @module stdx:os.win32.windows.windows.base
 * @brief Module file for Win32 operations.
 *
 * This file contains the implementation of the Windows operations from the Windows API libraries,
 * located in <windows_base.h>.
 */

module;

#ifdef _WIN32
#include <windows_base.h>
#endif

export module stdx:os.win32.windows.base;

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 */
export namespace stdx::os::win32 {
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
    using Win32Bool = ::BOOL;
    using Win32Char = ::CHAR;
    using Win32Short = ::SHORT;
    using Win32Int = ::INT;
    using Win32Long = ::LONG;
    using Win32UChar = ::UCHAR;
    using Win32UShort = ::USHORT;
    using Win32UInt = ::UINT;
    using Win32ULong = ::ULONG;
    using Win32Byte = ::BYTE;
    using Win32Word = ::WORD;
    using Win32DWord = ::DWORD;
    using Win32WChar = ::WCHAR;
    using Win32PWChar = ::PWCHAR;
    using Win32Atom = ::ATOM;
    using Win32ULong32 = ::ULONG32;
    using Win32DWord64 = ::DWORD64;
    using Win32ULong64 = ::ULONG64;
    using Win32Int32 = ::INT32;
    using Win32Int64 = ::INT64;
    using Win32DWordLong = ::DWORDLONG;

    using Win32PChar = ::PCHAR;
    using Win32PULong = ::PULONG;
    using Win32PULong64 = ::PULONG64;
    using Win32PDWord64 = ::PDWORD64;

    using Win32LongLong = ::LONGLONG;
    using Win32ULongLong = ::ULONGLONG;

    using Win32Void = ::VOID;
    using Win32PVoid = ::PVOID;
    using Win32LPVoid = ::LPVOID;
    using Win32PBool = ::PBOOL;
    using Win32LPBool = ::LPBOOL;
    using Win32PWord = ::PWORD;
    using Win32PLong = ::PLONG;
    using Win32LPLong = ::LPLONG;
    using Win32PDWord = ::PDWORD;

    using Win32Handle = ::HANDLE;
    using Win32HInstance = ::HINSTANCE;
    using Win32HWnd = ::HWND;
    using Win32HModule = ::HMODULE;
    using Win32HDc = ::HDC;
    using Win32HGlrc = ::HGLRC;
    using Win32HMenu = ::HMENU;
    using Win32PHandle = ::PHANDLE;
    using Win32LPHandle = ::LPHANDLE;

    using Win32PWStr = ::PWSTR;
    using Win32LPByte = ::LPBYTE;
    using Win32LPDWord = ::LPDWORD;
    using Win32LPCVoid = ::LPCVOID;

    using Win32IntPtr = ::INT_PTR;
    using Win32LongPtr = ::LONG_PTR;
    using Win32UIntPtr = ::UINT_PTR;
    using Win32ULongPtr = ::ULONG_PTR;
    using Win32DWordPtr = ::DWORD_PTR;
    using Win32PDWordPtr = ::PDWORD_PTR;

    using Win32Size = ::SIZE_T;
    using Win32SSize = ::SSIZE_T;

    using Win32LPStr = ::LPSTR;
    using Win32LPWStr = ::LPWSTR;
    using Win32LPCStr = ::LPCSTR;
    using Win32LPCWStr = ::LPCWSTR;
    using Win32TChar = ::TCHAR;
    using Win32TByte = ::TBYTE;
    using Win32LPCTStr = ::LPCTSTR;
    using Win32LPTStr = ::LPTSTR;

    using Win32FarProc = ::FARPROC;
    using Win32NearProc = ::NEARPROC;
    using Win32Proc = ::PROC;

    using Win32AccessMask = ::ACCESS_MASK;
    using Win32PAccessMask = ::PACCESS_MASK;

    using Win32HIcon = ::HICON;
    using Win32HBrush = ::HBRUSH;
    using Win32HCursor = ::HCURSOR;

    using Win32HResult = ::HRESULT;
    using Win32LResult = ::LRESULT;
    using Win32LParam = ::LPARAM;
    using Win32WParam = ::WPARAM;

    using Win32HGdiObj = ::HGDIOBJ;

    using Win32HKey = ::HKEY;
    using Win32PHKey = ::PHKEY;
    using Win32RegSam = ::REGSAM;
    
    using Win32Overlapped = ::OVERLAPPED;
    using Win32LPOverlapped = ::LPOVERLAPPED;

    using Win32SecurityAttributes = ::SECURITY_ATTRIBUTES;
    using Win32PSecurityAttributes = ::PSECURITY_ATTRIBUTES;
    using Win32LPSecurityAttributes = ::LPSECURITY_ATTRIBUTES;

    using Win32LargeInteger = ::LARGE_INTEGER;
    using Win32PLargeInteger = ::PLARGE_INTEGER;
    using Win32ULargeInteger = ::ULARGE_INTEGER;
    using Win32PULargeInteger = ::PULARGE_INTEGER;

    using Win32FileTime = ::FILETIME;
    using Win32PFileTime = ::PFILETIME;
    using Win32LPFileTime = ::LPFILETIME;
    
    using Win32Guid = ::GUID;
    #endif
}
