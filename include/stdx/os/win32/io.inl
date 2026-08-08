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
    #if defined(_WIN32) && __has_include(<io.h>)
    using FSizeT = ::_fsize_t;

    // Prefixed Crt because Windows has two unrelated find-data families and both
    // want the same name: the CRT's _wfinddata_t here, and fileapi.inl's
    // WIN32_FIND_DATAW, which is a different struct reached through a different
    // API. Unprefixed, the two FindDataW aliases were a redefinition error on
    // MSVC. The whole family carries the prefix rather than only the one that
    // collided, so the set stays spelled one way.
    using CrtFindDataA = ::_finddata_t;
    using CrtFindDataW = ::_wfinddata_t;
    using CrtFindData32 = ::_finddata32_t;
    // Two underscores, unlike every sibling: the CRT spells the 64-bit-time
    // narrow struct __finddata64_t and has no _finddata64_t at all, while the
    // wide one is _wfinddata64_t with one. _finddatai64_t below is a macro for
    // this same struct, so the two aliases name one type.
    using CrtFindData64 = ::__finddata64_t;
    using CrtFindDataI64 = ::_finddatai64_t;
    using CrtFindDataW32 = ::_wfinddata32_t;
    using CrtFindDataW64 = ::_wfinddata64_t;
    using CrtFindDataWI64 = ::_wfinddatai64_t;

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
    using ::_fileno;
    using ::_isatty;
    using ::_setmode;
    using ::_locking;

    // Wide-path CRT stream openers (<stdio.h>): the narrow fopen/freopen take
    // the ACP, so wide paths - fs::path::c_str() on Windows - go through these.
    using ::_wfopen;
    using ::_wfreopen;

    // The UCRT has no stdin/stdout/stderr variables to link against - they are
    // macros over this accessor (0, 1, 2), which is what code that undefines
    // those macros has to call instead.
    using ::__acrt_iob_func;

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
