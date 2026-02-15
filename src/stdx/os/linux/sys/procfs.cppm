/**
 * @file procfs.cppm
 * @module stdx:os.linux.sys.procfs
 * @brief Module file for Unix core file/libthread_db operations.
 *
 * This file contains the implementation of the Unix core file/libthread_db operations from the POSIX libraries,
 * located in <sys/procfs.h>.
 */

module;

#ifdef __linux__
#include <sys/procfs.h>
#endif

export module stdx:os.linux.sys.procfs;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ElfSignatureInfo = ::elf_siginfo;
    using ElfProcessStatus = ::elf_prstatus;
    using ElfProcessStateInfo = ::elf_prpsinfo;
    #endif
}
