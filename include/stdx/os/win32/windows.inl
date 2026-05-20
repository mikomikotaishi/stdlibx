#pragma once

/**
 * @file windows.inl
 * @brief Umbrella aggregator for the stdx::os::win32 wrapper layer.
 *
 * Each Win32 subsystem lives in its own .inl. This file simply pulls them
 * together so a downstream consumer can #include "stdx/os/win32/windows.inl"
 * and get the whole namespace.
 */

// Core types
#include "windows_base.inl"

// Splits formerly inline in windows.inl
#include "interlocked.inl"
#include "dbghelp.inl"
#include "exception.inl"

// Kernel base API
#include "errhandlingapi.inl"
#include "fileapi.inl"
#include "io.inl"
#include "libloader.inl"
#include "memoryapi.inl"
#include "process.inl"
#include "synchapi.inl"

// User-mode subsystems
#include "shellapi.inl"
#include "commctrl.inl"
#include "wincrypt.inl"
#include "wincodec.inl"
#include "dwmapi.inl"
#include "mmsystem.inl"
#include "winsock.inl"
