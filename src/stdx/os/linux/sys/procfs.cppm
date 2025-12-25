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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.procfs;
#else
export module stdlibx:os.linux.sys.procfs;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ElfSigInfo = ::elf_siginfo;
    using ElfPrStatus = ::elf_prstatus;
    using ElfPrpsInfo = ::elf_prpsinfo;
    #endif 

    using ElfSignatureInfo = ::elf_siginfo;
    using ElfProcessStatus = ::elf_prstatus;
    using ElfProcessStateInfo = ::elf_prpsinfo;
    #endif
}
