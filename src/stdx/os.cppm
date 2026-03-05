/**
 * @file os.cppm
 * @module stdx:os
 * @brief Module file for operating system operations.
 *
 * This file contains the imports for the operating system operations.
 */

module;

#define STDLIBX_EXTENSIONS_COMPILE_OS_LIBRARY

#ifdef __unix__
#include <aio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <fmtmsg.h>
#include <fnmatch.h>
#include <ftw.h>
#include <glob.h>
#include <grp.h>
#include <iconv.h>
#include <langinfo.h>
#include <libgen.h>
#include <monetary.h>
#include <mqueue.h>
#include <ndbm.h>
#include <netdb.h>
#include <nl_types.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <sched.h>
#include <search.h>
#include <semaphore.h>
#include <spawn.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <utime.h>
#include <utmpx.h>
#include <wordexp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/msg.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/ttydefaults.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_OS_LIBRARY

#ifdef __linux__
#include <arpa/nameser.h>
#include <arpa/telnet.h>
#include <arpa/tftp.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/if_packet.h>
#include <net/if_ppp.h>
#include <net/if_shaper.h>
#include <net/ppp_defs.h>
#include <net/route.h>
#include <netinet/ether.h>
#include <netinet/icmp6.h>
#include <netinet/if_ether.h>
#include <netinet/if_fddi.h>
#include <netinet/if_tr.h>
#include <netinet/igmp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <sys/acct.h>
#include <sys/acl.h>
#include <sys/auxv.h>
#include <sys/capability.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/fanotify.h>
#include <sys/file.h>
#include <sys/fsuid.h>
#include <sys/gmon.h>
#include <sys/gmon_out.h>
#include <sys/inotify.h>
#include <sys/io.h>
#include <sys/klog.h>
#include <sys/mtio.h>
#include <sys/perm.h>
#include <sys/personality.h>
#include <sys/pidfd.h>
#include <sys/poll.h>
#include <sys/prctl.h>
#include <sys/procfs.h>
#include <sys/profil.h>
#include <sys/psx_syscall.h>
#include <sys/ptrace.h>
#include <sys/quota.h>
#include <sys/random.h>
#include <sys/raw.h>
#include <sys/reboot.h>
#include <sys/rseq.h>
#include <sys/sendfile.h>
#include <sys/signalfd.h>
#include <sys/single_threaded.h>
#include <sys/statfs.h>
#include <sys/swap.h>
#include <sys/sysinfo.h>
#include <sys/syslog.h>
#include <sys/timeb.h>
#include <sys/timerfd.h>
#include <sys/timex.h>
#include <sys/ttychars.h>
#include <sys/ucontext.h>
#include <sys/user.h>
#include <sys/vlimit.h>
#include <sys/xattr.h>
#include <sys/platform/x86.h>

#include <linux/sysinfo.h>
#include <linux/posix_types.h>
#endif

#ifdef _WIN32
#define NOMINMAX

#include <intrin.h>
#include <windows.h>
#include <windows_base.h>

#undef InterlockedExchange8
#undef InterlockedExchangeAdd8
#undef InterlockedExchangeAnd8
#undef InterlockedExchangeOr8
#undef InterlockedExchangeXor8
#undef InterlockedDecrement8
#undef InterlockedIncrement8
#undef InterlockedCompareExchange8
#undef InterlockedExchange16
#undef InterlockedExchangeAdd16
#undef InterlockedExchangeOr16
#undef InterlockedExchangeXor16
#undef InterlockedDecrement16
#undef InterlockedIncrement16
#undef InterlockedCompareExchange16
#undef InterlockedExchange
#undef InterlockedExchangeAdd
#undef InterlockedExchangeAnd
#undef InterlockedExchangeOr
#undef InterlockedExchangeXor
#undef InterlockedDecrement
#undef InterlockedIncrement 
#undef InterlockedCompareExchange 

#ifdef _AMD64_
#undef InterlockedExchange64
#undef InterlockedExchangeAdd64
#undef InterlockedExchangeAnd64
#undef InterlockedExchangeOr64
#undef InterlockedExchangeXor64
#undef InterlockedDecrement64
#undef InterlockedIncrement64
#undef InterlockedCompareExchange64
#undef InterlockedCompareExchange128
#endif
#endif

#endif

export module stdx:os;

export import :os.constants;

import core;

using namespace core;

#include "stdx/os/unix/aio.inl"
#include "stdx/os/unix/dirent.inl"
#include "stdx/os/unix/dlfcn.inl"
#include "stdx/os/unix/fmtmsg.inl"
#include "stdx/os/unix/fnmatch.inl"
#include "stdx/os/unix/ftw.inl"
#include "stdx/os/unix/glob.inl"
#include "stdx/os/unix/grp.inl"
#include "stdx/os/unix/iconv.inl"
#include "stdx/os/unix/langinfo.inl"
#include "stdx/os/unix/libgen.inl"
#include "stdx/os/unix/monetary.inl"
#include "stdx/os/unix/mqueue.inl"
#include "stdx/os/unix/ndbm.inl"
#include "stdx/os/unix/netdb.inl"
#include "stdx/os/unix/nl_types.inl"
#include "stdx/os/unix/pthread.inl"
#include "stdx/os/unix/pwd.inl"
#include "stdx/os/unix/regex.inl"
#include "stdx/os/unix/sched.inl"
#include "stdx/os/unix/search.inl"
#include "stdx/os/unix/semaphore.inl"
#include "stdx/os/unix/spawn.inl"
#include "stdx/os/unix/strings.inl"
#include "stdx/os/unix/termios.inl"
#include "stdx/os/unix/unistd.inl"
#include "stdx/os/unix/utime.inl"
#include "stdx/os/unix/utmpx.inl"
#include "stdx/os/unix/wordexp.inl"
#include "stdx/os/unix/arpa/inet.inl"
#include "stdx/os/unix/net/if.inl"
#include "stdx/os/unix/netinet/in.inl"
#include "stdx/os/unix/netinet/tcp.inl"
#include "stdx/os/unix/sys/ioctl.inl"
#include "stdx/os/unix/sys/ipc.inl"
#include "stdx/os/unix/sys/mman.inl"
#include "stdx/os/unix/sys/mount.inl"
#include "stdx/os/unix/sys/msg.inl"
#include "stdx/os/unix/sys/resource.inl"
#include "stdx/os/unix/sys/select.inl"
#include "stdx/os/unix/sys/sem.inl"
#include "stdx/os/unix/sys/shm.inl"
#include "stdx/os/unix/sys/socket.inl"
#include "stdx/os/unix/sys/stat.inl"
#include "stdx/os/unix/sys/statvfs.inl"
#include "stdx/os/unix/sys/time.inl"
#include "stdx/os/unix/sys/times.inl"
#include "stdx/os/unix/sys/uio.inl"
#include "stdx/os/unix/sys/un.inl"
#include "stdx/os/unix/sys/utsname.inl"
#include "stdx/os/unix/sys/wait.inl"

#ifdef STDLIBX_EXTENSIONS_COMPILE_OS_LIBRARY

#include "stdx/os/linux/posix_types.inl"
#include "stdx/os/linux/sysinfo.inl"

#include "stdx/os/linux/arpa/nameser.inl"
#include "stdx/os/linux/arpa/telnet.inl"
#include "stdx/os/linux/arpa/tftp.inl"
#include "stdx/os/linux/net/ethernet.inl"
#include "stdx/os/linux/net/if_arp.inl"
#include "stdx/os/linux/net/if_packet.inl"
#include "stdx/os/linux/net/if_ppp.inl"
#include "stdx/os/linux/net/if_shaper.inl"
#include "stdx/os/linux/net/ppp_defs.inl"
#include "stdx/os/linux/net/route.inl"
#include "stdx/os/linux/netinet/ether.inl"
#include "stdx/os/linux/netinet/icmp6.inl"
#include "stdx/os/linux/netinet/if_ether.inl"
#include "stdx/os/linux/netinet/if_fddi.inl"
#include "stdx/os/linux/netinet/if_tr.inl"
#include "stdx/os/linux/netinet/igmp.inl"
#include "stdx/os/linux/netinet/ip.inl"
#include "stdx/os/linux/netinet/ip_icmp.inl"
#include "stdx/os/linux/netinet/ip6.inl"
#include "stdx/os/linux/netinet/udp.inl"
#include "stdx/os/linux/sys/acct.inl"
#include "stdx/os/linux/sys/acl.inl"
#include "stdx/os/linux/sys/auxv.inl"
#include "stdx/os/linux/sys/capability.inl"
#include "stdx/os/linux/sys/epoll.inl"
#include "stdx/os/linux/sys/eventfd.inl"
#include "stdx/os/linux/sys/fanotify.inl"
#include "stdx/os/linux/sys/file.inl"
#include "stdx/os/linux/sys/fsuid.inl"
#include "stdx/os/linux/sys/gmon.inl"
#include "stdx/os/linux/sys/gmon_out.inl"
#include "stdx/os/linux/sys/inotify.inl"
#include "stdx/os/linux/sys/io.inl"
#include "stdx/os/linux/sys/klogctl.inl"
#include "stdx/os/linux/sys/mtio.inl"
#include "stdx/os/linux/sys/perm.inl"
#include "stdx/os/linux/sys/personality.inl"
#include "stdx/os/linux/sys/pidfd.inl"
#include "stdx/os/linux/sys/poll.inl"
#include "stdx/os/linux/sys/prctl.inl"
#include "stdx/os/linux/sys/procfs.inl"
#include "stdx/os/linux/sys/profil.inl"
#include "stdx/os/linux/sys/psx_syscall.inl"
#include "stdx/os/linux/sys/ptrace.inl"
#include "stdx/os/linux/sys/quota.inl"
#include "stdx/os/linux/sys/random.inl"
#include "stdx/os/linux/sys/raw.inl"
#include "stdx/os/linux/sys/reboot.inl"
#include "stdx/os/linux/sys/rseq.inl"
#include "stdx/os/linux/sys/sendfile.inl"
#include "stdx/os/linux/sys/signalfd.inl"
#include "stdx/os/linux/sys/single_threaded.inl"
#include "stdx/os/linux/sys/statfs.inl"
#include "stdx/os/linux/sys/swap.inl"
#include "stdx/os/linux/sys/sysinfo.inl"
#include "stdx/os/linux/sys/syslog.inl"
#include "stdx/os/linux/sys/timeb.inl"
#include "stdx/os/linux/sys/timerfd.inl"
#include "stdx/os/linux/sys/timex.inl"
#include "stdx/os/linux/sys/ttychars.inl"
#include "stdx/os/linux/sys/ucontext.inl"
#include "stdx/os/linux/sys/user.inl"
#include "stdx/os/linux/sys/vlimit.inl"
#include "stdx/os/linux/sys/xattr.inl"

#include "stdx/os/win32/intrin.inl"
#include "stdx/os/win32/windows.inl"

#endif
