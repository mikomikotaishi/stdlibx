#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Shell API (shellapi.h) wrappers.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<shellapi.h>)
    // File-op flags
    using FileOpFlags = ::FILEOP_FLAGS;
    using PrinterOpFlags = ::PRINTEROP_FLAGS;

    // Drag-and-drop
    using DragInfo = ::DRAGINFOA;
    using DragInfoW = ::DRAGINFOW;
    using DropHandle = ::HDROP;

    // Shell file operations
    using ShellFileOpStruct = ::SHFILEOPSTRUCTA;
    using ShellFileOpStructW = ::SHFILEOPSTRUCTW;
    using ShellExecuteInfo = ::SHELLEXECUTEINFOA;
    using ShellExecuteInfoW = ::SHELLEXECUTEINFOW;
    using ShellExecuteInfoPointer = ::LPSHELLEXECUTEINFOA;
    using ShellExecuteInfoWPointer = ::LPSHELLEXECUTEINFOW;
    using ShellNameMapping = ::SHNAMEMAPPINGA;
    using ShellNameMappingW = ::SHNAMEMAPPINGW;
    using ShellQueryRecycleBinInfo = ::SHQUERYRBINFO;

    // Notify icon (tray)
    using NotifyIconData = ::NOTIFYICONDATAA;
    using NotifyIconDataW = ::NOTIFYICONDATAW;
    using NotifyIconIdentifier = ::NOTIFYICONIDENTIFIER;

    // AppBar
    using AppBarData = ::APPBARDATA;

    // File-info
    using ShellFileInfo = ::SHFILEINFOA;
    using ShellFileInfoW = ::SHFILEINFOW;
    using ShellStockIconInfo = ::SHSTOCKICONINFO;

    // Functions (W/A pairs intentionally exposed)
    using ::ShellExecuteA;
    using ::ShellExecuteW;
    using ::ShellExecuteExA;
    using ::ShellExecuteExW;
    using ::SHFileOperationA;
    using ::SHFileOperationW;
    using ::SHGetFileInfoA;
    using ::SHGetFileInfoW;
    using ::SHGetStockIconInfo;
    using ::SHGetSpecialFolderLocation;
    using ::SHGetSpecialFolderPathA;
    using ::SHGetSpecialFolderPathW;
    using ::SHEmptyRecycleBinA;
    using ::SHEmptyRecycleBinW;
    using ::SHQueryRecycleBinA;
    using ::SHQueryRecycleBinW;
    using ::Shell_NotifyIconA;
    using ::Shell_NotifyIconW;
    using ::DragAcceptFiles;
    using ::DragFinish;
    using ::DragQueryFileA;
    using ::DragQueryFileW;
    using ::DragQueryPoint;
    using ::ExtractIconA;
    using ::ExtractIconW;
    using ::ExtractIconExA;
    using ::ExtractIconExW;
    using ::CommandLineToArgvW;
    #endif
}
