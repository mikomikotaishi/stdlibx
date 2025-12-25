/**
 * @file unix.cppm
 * @module stdx:os.unix
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix;
#else
export module stdlibx:os.unix;
#endif


export import :os.unix.aio;
export import :os.unix.arpa;
export import :os.unix.dirent;
export import :os.unix.dlfcn;
export import :os.unix.fcntl;
export import :os.unix.fmtmsg;
export import :os.unix.fnmatch;
export import :os.unix.ftw;
export import :os.unix.glob;
export import :os.unix.grp;
export import :os.unix.iconv;
export import :os.unix.langinfo;
export import :os.unix.libgen;
export import :os.unix.monetary;
export import :os.unix.mqueue;
export import :os.unix.ndbm;
export import :os.unix.net;
export import :os.unix.netdb;
export import :os.unix.netinet;
export import :os.unix.nl_types;
export import :os.unix.pthread;
export import :os.unix.pwd;
export import :os.unix.regex;
export import :os.unix.sched;
export import :os.unix.search;
export import :os.unix.semaphore;
export import :os.unix.spawn;
export import :os.unix.strings;
export import :os.unix.sys;
export import :os.unix.termios;
export import :os.unix.unistd;
export import :os.unix.utime;
export import :os.unix.utmpx;
export import :os.unix.wordexp;
