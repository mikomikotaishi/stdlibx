#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Wrapper namespace for Windows API operations.
 *
 * Type aliases drop Win32 Hungarian notation:
 *   - HXxx (handles)         -> XxxHandle               (HWND -> WindowHandle)
 *   - PT / LPT (pointer-to)  -> WinTPointer / TPointer  (PVOID -> WinVoidPointer)
 *   - T_PTR (ptr-sized int)  -> WinTPtr                 (INT_PTR -> WinIntPtr)
 *   - LPSTR / LPWSTR / ...   -> WinAString / WinWString / ...
 *   - Cryptic abbreviations  expanded (Wnd, Dc, Glrc, ...)
 *
 * Convention for the Win prefix:
 *   - Primitives, explicit-width ints, pointer-sized ints, raw-pointer
 *     aliases, and raw-string aliases all carry Win to disambiguate from
 *     stdx::core wrapper classes (Boolean, Byte, Short, Long, Float, ...)
 *     and from generic spellings (AString, IntPtr, VoidPointer).
 *   - Handles, callbacks, MSG/security/structure aliases stay unprefixed
 *     since their names (WindowHandle, Overlapped, FileTime, WindowProc, ...)
 *     already self-identify as Win32.
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
    // Primitive integers and characters
    using WinBool = ::BOOL;
    using WinBoolean = ::BOOLEAN;
    using WinChar = ::CHAR;
    using WinShort = ::SHORT;
    using WinInt = ::INT;
    using WinLong = ::LONG;
    using WinLongLong = ::LONGLONG;
    using WinUChar = ::UCHAR;
    using WinUShort = ::USHORT;
    using WinUInt = ::UINT;
    using WinULong = ::ULONG;
    using WinULongLong = ::ULONGLONG;
    using WinByte = ::BYTE;
    using WinWord = ::WORD;
    using WinDWord = ::DWORD;
    using WinDWordLong = ::DWORDLONG;
    using WinQWord = ::DWORDLONG;
    using WinWChar = ::WCHAR;
    using WinFloat = ::FLOAT;
    using WinAtom = ::ATOM;
    using WinVoid = ::VOID;

    // Explicit-width integers
    using WinInt8 = ::INT8;
    using WinInt16 = ::INT16;
    using WinInt32 = ::INT32;
    using WinInt64 = ::INT64;
    using WinUInt8 = ::UINT8;
    using WinUInt16 = ::UINT16;
    using WinUInt32 = ::UINT32;
    using WinUInt64 = ::UINT64;
    using WinLong32 = ::LONG32;
    using WinLong64 = ::LONG64;
    using WinULong32 = ::ULONG32;
    using WinULong64 = ::ULONG64;
    using WinDWord32 = ::DWORD32;
    using WinDWord64 = ::DWORD64;

    // Pointer-sized integers (Win32 _PTR types; matches C# IntPtr convention)
    using WinIntPtr = ::INT_PTR;
    using WinUIntPtr = ::UINT_PTR;
    using WinLongPtr = ::LONG_PTR;
    using WinULongPtr = ::ULONG_PTR;
    using WinDWordPtr = ::DWORD_PTR;
    using WinHalfPtr = ::HALF_PTR;
    using WinUHalfPtr = ::UHALF_PTR;
    using WinSizeT = ::SIZE_T;
    using WinSSizeT = ::SSIZE_T;

    // Pointer-to-T (collapsed PXxx / LPXxx aliases)
    using WinVoidPointer = ::PVOID;
    using WinConstVoidPointer = ::LPCVOID;
    using WinBoolPointer = ::PBOOL;
    using WinBooleanPointer = ::PBOOLEAN;
    using WinBytePointer = ::PBYTE;
    using WinCharPointer = ::PCHAR;
    using WinWCharPointer = ::PWCHAR;
    using WinShortPointer = ::PSHORT;
    using WinIntPointer = ::PINT;
    using WinUIntPointer = ::PUINT;
    using WinLongPointer = ::PLONG;
    using WinULongPointer = ::PULONG;
    using WinLongLongPointer = ::PLONGLONG;
    using WinULongLongPointer = ::PULONGLONG;
    using WinWordPointer = ::PWORD;
    using WinDWordPointer = ::PDWORD;
    using WinDWord64Pointer = ::PDWORD64;
    using WinULong64Pointer = ::PULONG64;
    using WinDWordLongPointer = ::PDWORDLONG;
    using WinFloatPointer = ::PFLOAT;
    using WinHandlePointer = ::PHANDLE;
    using WinDWordPtrPointer = ::PDWORD_PTR;
    using WinSizeTPointer = ::PSIZE_T;

    // Null-terminated raw string buffers
    using WinAString = ::LPSTR;
    using WinConstAString = ::LPCSTR;
    using WinWString = ::LPWSTR;
    using WinConstWString = ::LPCWSTR;
    using WinTString = ::LPTSTR;
    using WinConstTString = ::LPCTSTR;
    using WinTChar = ::TCHAR;
    using WinTByte = ::TBYTE;

    // Handles
    using Handle = ::HANDLE;
    using WindowHandle = ::HWND;
    using InstanceHandle = ::HINSTANCE;
    using ModuleHandle = ::HMODULE;
    using DeviceContextHandle = ::HDC;
    using GlRenderContextHandle = ::HGLRC;
    using MenuHandle = ::HMENU;
    using IconHandle = ::HICON;
    using CursorHandle = ::HCURSOR;
    using BrushHandle = ::HBRUSH;
    using PenHandle = ::HPEN;
    using FontHandle = ::HFONT;
    using BitmapHandle = ::HBITMAP;
    using PaletteHandle = ::HPALETTE;
    using RegionHandle = ::HRGN;
    using MonitorHandle = ::HMONITOR;
    using HookHandle = ::HHOOK;
    using AcceleratorTableHandle = ::HACCEL;
    using GdiObjectHandle = ::HGDIOBJ;
    using RegistryKeyHandle = ::HKEY;
    using RegistryKeyHandlePointer = ::PHKEY;
    using GlobalHandle = ::HGLOBAL;
    using LocalHandle = ::HLOCAL;
    using FileHandle = ::HFILE;
    using ColorSpaceHandle = ::HCOLORSPACE;
    using DeferredWindowPositionHandle = ::HDWP;

    // Callable types
    using FarProc = ::FARPROC;
    using NearProc = ::NEARPROC;
    using Proc = ::PROC;
    using WindowProc = ::WNDPROC;
    using DialogProc = ::DLGPROC;
    using HookProc = ::HOOKPROC;

    // Window messaging
    using LParam = ::LPARAM;
    using WParam = ::WPARAM;
    using LResult = ::LRESULT;
    using HResult = ::HRESULT;
    using Message = ::MSG;
    using MessagePointer = ::PMSG;

    // Security
    using AccessMask = ::ACCESS_MASK;
    using AccessMaskPointer = ::PACCESS_MASK;
    using RegistryAccessMask = ::REGSAM;
    using SecurityDescriptorPointer = ::PSECURITY_DESCRIPTOR;
    using SecurityIdPointer = ::PSID;
    using Luid = ::LUID;
    using LuidPointer = ::PLUID;

    // Common structures
    using Overlapped = ::OVERLAPPED;
    using OverlappedPointer = ::LPOVERLAPPED;
    using SecurityAttributes = ::SECURITY_ATTRIBUTES;
    using SecurityAttributesPointer = ::LPSECURITY_ATTRIBUTES;
    using LargeInteger = ::LARGE_INTEGER;
    using LargeIntegerPointer = ::PLARGE_INTEGER;
    using ULargeInteger = ::ULARGE_INTEGER;
    using ULargeIntegerPointer = ::PULARGE_INTEGER;
    using FileTime = ::FILETIME;
    using FileTimePointer = ::PFILETIME;
    using SystemTime = ::SYSTEMTIME;
    using SystemTimePointer = ::PSYSTEMTIME;
    using Guid = ::GUID;
    using GuidPointer = ::LPGUID;
    using Point = ::POINT;
    using PointPointer = ::PPOINT;
    using PointLong = ::POINTL;
    using Rect = ::RECT;
    using RectPointer = ::PRECT;
    using RectLong = ::RECTL;
    using Size2D = ::SIZE;
    using Size2DPointer = ::PSIZE;
    using Size2DLong = ::SIZEL;
    using ColorRef = ::COLORREF;
    using ColorRefPointer = ::LPCOLORREF;

    // Locale
    using LangId = ::LANGID;
    using Lcid = ::LCID;

    // Update sequence number
    using Usn = ::USN;
    #endif
}
