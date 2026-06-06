#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/rseq.h>)
    using RestartableSequenceCpuIdState = ::rseq_cpu_id_state;
    using RestartableSequenceFlags = ::rseq_flags;
    using RestartableSequenceCsFlagsBit = ::rseq_cs_flags_bit;
    using RestartableSequenceCsFlags = ::rseq_cs_flags;
    using RestartableSequenceCs = ::rseq_cs;
    using RestartableSequence = ::rseq;
    #endif
}
