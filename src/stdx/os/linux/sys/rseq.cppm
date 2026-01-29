/**
 * @file rseq.cppm
 * @module stdx:os.linux.sys.rseq
 * @brief Module file for Unix restartable sequences operations.
 *
 * This file contains the implementation of the Unix restartable sequences operations from the POSIX libraries,
 * located in <sys/rseq.h>.
 */

module;

#ifdef __linux__
#include <sys/rseq.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.rseq;
#else
export module stdlibx:os.linux.sys.rseq;
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

    using RestartableSequenceCpuIdState = ::rseq_cpu_id_state;
    using RestartableSequenceFlags = ::rseq_flags;
    using RestartableSequenceCsFlagsBit = ::rseq_cs_flags_bit;
    using RestartableSequenceCsFlags = ::rseq_cs_flags;
    using RestartableSequenceCs = ::rseq_cs;
    using RestartableSequence = ::rseq;
    #endif
}
