/**
 * @file sys.cppm
 * @module stdx:os.linux.sys
 * @brief Module file for Unix system operations.
 *
 * This file contains the implementation of the Unix system operations from the POSIX libraries,
 * located in all <sys/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys;
#else
export module stdlibx:os.linux.sys;
#endif

export import :os.linux.sys.acct;
export import :os.linux.sys.acl;
export import :os.linux.sys.auxv;
export import :os.linux.sys.capability;
export import :os.linux.sys.epoll;
export import :os.linux.sys.eventfd;
export import :os.linux.sys.fanotify;
export import :os.linux.sys.file;
export import :os.linux.sys.fsuid;
export import :os.linux.sys.gmon;
export import :os.linux.sys.gmon_out;
export import :os.linux.sys.inotify;
export import :os.linux.sys.io;
export import :os.linux.sys.klog;
export import :os.linux.sys.mtio;
export import :os.linux.sys.perm;
export import :os.linux.sys.personality;
export import :os.linux.sys.pidfd;
export import :os.linux.sys.poll;
export import :os.linux.sys.prctl;
export import :os.linux.sys.procfs;
export import :os.linux.sys.profil;
export import :os.linux.sys.psx_syscall;
export import :os.linux.sys.ptrace;
export import :os.linux.sys.quota;
export import :os.linux.sys.random;
export import :os.linux.sys.raw;
export import :os.linux.sys.reboot;
export import :os.linux.sys.rseq;
export import :os.linux.sys.sendfile;
export import :os.linux.sys.signalfd;
export import :os.linux.sys.single_threaded;
export import :os.linux.sys.statfs;
export import :os.linux.sys.swap;
export import :os.linux.sys.sysinfo;
export import :os.linux.sys.syslog;
export import :os.linux.sys.timeb;
export import :os.linux.sys.timerfd;
export import :os.linux.sys.timex;
export import :os.linux.sys.ttychars;
export import :os.linux.sys.ucontext;
export import :os.linux.sys.user;
export import :os.linux.sys.vlimit;
export import :os.linux.sys.xattr;
export import :os.linux.sys.platform.x86;
