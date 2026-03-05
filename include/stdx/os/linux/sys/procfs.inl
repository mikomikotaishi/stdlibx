#pragma once

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
