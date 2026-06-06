#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/procfs.h>)
    using ElfSignatureInfo = ::elf_siginfo;
    using ElfProcessStatus = ::elf_prstatus;
    using ElfProcessStateInfo = ::elf_prpsinfo;
    #endif
}
