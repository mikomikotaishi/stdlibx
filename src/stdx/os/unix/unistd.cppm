/**
 * @file unistd.cppm
 * @module stdx:os.unix.unistd
 * @brief Module file for Unix essential POSIX function operations.
 *
 * This file contains the implementation of the Unix essential POSIX function operations from the POSIX libraries,
 * located in <unistd.h>.
 */

module;

#ifdef __unix__
#include <unistd.h>
#endif

#undef STDIN_FILENO
#undef STDOUT_FILENO
#undef STDERR_FILENO

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.unistd;
#else
export module stdlibx:os.unix.unistd;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    inline constexpr int STDIN_FILENO = 0;
    inline constexpr int STDOUT_FILENO = 1;
    inline constexpr int STDERR_FILENO = 2;

    #ifdef __unix__
    using ::access;
    using ::euidaccess;
    using ::eaccess;
    using ::execveat;
    using ::faccessat;
    using ::lseek;
    using ::lseek64;
    using ::close;
    using ::closefrom;
    using ::read;
    using ::write;
    using ::pread;
    using ::pwrite;
    using ::pread64;
    using ::pwrite64;
    using ::pipe;
    using ::pipe2;
    using ::alarm;
    using ::sleep;
    using ::ualarm;
    using ::usleep;
    using ::pause;
    using ::chown;
    using ::fchown;
    using ::lchown;
    using ::fchownat;
    using ::chdir;
    using ::fchdir;
    using ::getcwd;
    using ::get_current_dir_name;
    using ::getwd;
    using ::dup;
    using ::dup2;
    using ::dup3;
    using ::execve;
    using ::fexecve;
    using ::execv;
    using ::execle;
    using ::execl;
    using ::execvp;
    using ::execlp;
    using ::execvpe;
    using ::nice;
    using ::_exit;
    using ::pathconf;
    using ::fpathconf;
    using ::sysconf;
    using ::confstr;
    using ::getpid;
    using ::getppid;
    using ::getpgrp;
    using ::getpgid;
    using ::setpgid;
    using ::setpgrp;
    using ::setsid;
    using ::getsid;
    using ::getuid;
    using ::geteuid;
    using ::getgid;
    using ::getegid;
    using ::getgroups;
    using ::group_member;
    using ::setuid;
    using ::setreuid;
    using ::seteuid;
    using ::setgid;
    using ::setregid;
    using ::setegid;
    using ::getresuid;
    using ::getresgid;
    using ::setresuid;
    using ::setresgid;
    using ::fork;
    using ::vfork;
    using ::_Fork;
    using ::ttyname;
    using ::ttyname_r;
    using ::isatty;
    using ::ttyslot;
    using ::link;
    using ::linkat;
    using ::symlink;
    using ::readlink;
    using ::symlinkat;
    using ::readlinkat;
    using ::unlink;
    using ::unlinkat;
    using ::rmdir;
    using ::tcgetpgrp;
    using ::tcsetpgrp;
    using ::getlogin;
    using ::getlogin_r;
    using ::setlogin;
    using ::gethostname;
    using ::sethostname;
    using ::sethostid;
    using ::getdomainname;
    using ::setdomainname;
    using ::vhangup;
    using ::revoke;
    using ::profil;
    using ::acct;
    using ::getusershell;
    using ::endusershell;
    using ::setusershell;
    using ::daemon;
    using ::chroot;
    using ::getpass;
    using ::fsync;
    using ::syncfs;
    using ::gethostid;
    using ::sync;
    using ::getpagesize;
    using ::getdtablesize;
    using ::truncate;
    using ::truncate64;
    using ::ftruncate;
    using ::ftruncate64;
    using ::brk;
    using ::sbrk;
    using ::syscall;
    using ::lockf;
    using ::lockf64;
    using ::copy_file_range;
    using ::fdatasync;
    using ::crypt;
    using ::swab;
    using ::getentropy;
    using ::close_range;
    using ::gettid;
    #endif
}
