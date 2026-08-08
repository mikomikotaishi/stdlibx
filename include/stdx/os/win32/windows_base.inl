#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Windows API operations.
 *
 * Type aliases drop Win32 Hungarian notation:
 *   - HXxx (handles)         -> XxxHandle               (HWND -> WindowHandle)
 *   - PT / LPT (pointer-to)  -> TPointer / TPointer  (PVOID -> VoidPointer)
 *   - T_PTR (ptr-sized int)  -> TPtr                 (INT_PTR -> IntPtr)
 *   - LPSTR / LPWSTR / ...   -> AString / WString / ...
 *   - Cryptic abbreviations  expanded (Wnd, Dc, Glrc, ...)
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

    #if defined(_WIN32) && __has_include(<windows.h>)
    // Primitive integers and characters
    using Bool = ::BOOL;
    using Boolean = ::BOOLEAN;
    using Char = ::CHAR;
    using Short = ::SHORT;
    using Int = ::INT;
    using Long = ::LONG;
    using LongLong = ::LONGLONG;
    using UChar = ::UCHAR;
    using UShort = ::USHORT;
    using UInt = ::UINT;
    using ULong = ::ULONG;
    using ULongLong = ::ULONGLONG;
    using Byte = ::BYTE;
    using Word = ::WORD;
    using DWord = ::DWORD;
    using DWordLong = ::DWORDLONG;
    using QWord = ::DWORDLONG;
    using WChar = ::WCHAR;
    using Float = ::FLOAT;
    using Atom = ::ATOM;

    // Explicit-width integers
    using Int8 = ::INT8;
    using Int16 = ::INT16;
    using Int32 = ::INT32;
    using Int64 = ::INT64;
    using UInt8 = ::UINT8;
    using UInt16 = ::UINT16;
    using UInt32 = ::UINT32;
    using UInt64 = ::UINT64;
    using Long32 = ::LONG32;
    using Long64 = ::LONG64;
    using ULong32 = ::ULONG32;
    using ULong64 = ::ULONG64;
    using DWord32 = ::DWORD32;
    using DWord64 = ::DWORD64;

    // Pointer-sized integers (Win32 _PTR types; matches C# IntPtr convention)
    using IntPtr = ::INT_PTR;
    using UIntPtr = ::UINT_PTR;
    using LongPtr = ::LONG_PTR;
    using ULongPtr = ::ULONG_PTR;
    using DWordPtr = ::DWORD_PTR;
    using HalfPtr = ::HALF_PTR;
    using UHalfPtr = ::UHALF_PTR;
    using SizeT = ::SIZE_T;
    using SSizeT = ::SSIZE_T;

    // Pointer-to-T (collapsed PXxx / LPXxx aliases)
    using VoidPointer = ::PVOID;
    using ConstVoidPointer = ::LPCVOID;
    using BoolPointer = ::PBOOL;
    using BooleanPointer = ::PBOOLEAN;
    using BytePointer = ::PBYTE;
    using CharPointer = ::PCHAR;
    using WCharPointer = ::PWCHAR;
    using ShortPointer = ::PSHORT;
    using IntPointer = ::PINT;
    using UIntPointer = ::PUINT;
    using LongPointer = ::PLONG;
    using ULongPointer = ::PULONG;
    using LongLongPointer = ::PLONGLONG;
    using ULongLongPointer = ::PULONGLONG;
    using WordPointer = ::PWORD;
    using DWordPointer = ::PDWORD;
    using DWord64Pointer = ::PDWORD64;
    using ULong64Pointer = ::PULONG64;
    using DWordLongPointer = ::PDWORDLONG;
    using FloatPointer = ::PFLOAT;
    using HandlePointer = ::PHANDLE;
    using DWordPtrPointer = ::PDWORD_PTR;
    using SizeTPointer = ::PSIZE_T;

    // Null-terminated raw string buffers
    using AString = ::LPSTR;
    using ConstAString = ::LPCSTR;
    using WString = ::LPWSTR;
    using ConstWString = ::LPCWSTR;
    using TString = ::LPTSTR;
    using ConstTString = ::LPCTSTR;
    using TChar = ::TCHAR;
    using TByte = ::TBYTE;

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
