#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief The Berkeley Packet Filter - <net/bpf.h>.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/bpf.h>)
    using BpfProgram = ::bpf_program;
    using BpfInstruction = ::bpf_insn;
    using BpfHeader = ::bpf_hdr;
    using BpfStatistics = ::bpf_stat;
    using BpfVersion = ::bpf_version;
    #endif
}
