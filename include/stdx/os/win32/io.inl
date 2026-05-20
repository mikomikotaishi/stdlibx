#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Low-level CRT file I/O wrappers (Microsoft <io.h>).
 *
 * These are the POSIX-style underscore-prefixed CRT functions exposed by
 * <io.h> (e.g. _open, _read, _write). For Win32 API file I/O (CreateFile,
 * ReadFile, WriteFile, ...) see windows.inl.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    using FSizeT = ::_fsize_t;
    using FindDataA = ::_finddata_t;
    using FindDataW = ::_wfinddata_t;
    using FindData32 = ::_finddata32_t;
    using FindData64 = ::_finddata64_t;
    using FindDataI64 = ::_finddatai64_t;
    using FindDataW32 = ::_wfinddata32_t;
    using FindDataW64 = ::_wfinddata64_t;
    using FindDataWI64 = ::_wfinddatai64_t;

    using ::_open;
    using ::_close;
    using ::_read;
    using ::_write;
    using ::_lseek;
    using ::_lseeki64;
    using ::_tell;
    using ::_telli64;
    using ::_eof;
    using ::_chsize;
    using ::_chsize_s;
    using ::_dup;
    using ::_dup2;
    using ::_pipe;
    using ::_get_osfhandle;
    using ::_open_osfhandle;
    using ::_filelength;
    using ::_filelengthi64;
    using ::_isatty;
    using ::_setmode;
    using ::_locking;

    // File system traversal
    using ::_access;
    using ::_chmod;
    using ::_unlink;
    using ::_mktemp;
    using ::_mktemp_s;
    using ::_findfirst32;
    using ::_findnext32;
    using ::_findfirst64;
    using ::_findnext64;
    using ::_findclose;

    // Wide-char variants
    using ::_waccess;
    using ::_wchmod;
    using ::_wunlink;
    using ::_wmktemp;
    using ::_wfindfirst32;
    using ::_wfindnext32;
    using ::_wfindfirst64;
    using ::_wfindnext64;
    #endif
}
