#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using RestartableSequenceCpuIdState = ::rseq_cpu_id_state;
    using RestartableSequenceFlags = ::rseq_flags;
    using RestartableSequenceCsFlagsBit = ::rseq_cs_flags_bit;
    using RestartableSequenceCsFlags = ::rseq_cs_flags;
    using RestartableSequenceCs = ::rseq_cs;
    using RestartableSequence = ::rseq;
    #endif
}
