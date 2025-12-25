/**
 * @file x86.cppm
 * @module stdx:os.linux.sys.platform.x86
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/platform/x86.h>.
 */

module;

#ifdef __linux__
#include <sys/platform/x86.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.platform.x86;
#else
export module stdlibx:os.linux.sys.platform.x86;
#endif

/**
 * @namespace stdx::os::linux::sys::platform
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys::platform {
#else 
export namespace stdlibx::os::linux::sys::platform {
#endif
    #ifdef __linux__
    using CpuIdFeature = ::cpuid_feature;
    enum class CpuIdRegisterIndex {
        INDEX_EAX = static_cast<int>(cpuid_register_index::cpuid_register_index_eax),
        INDEX_EBX = static_cast<int>(cpuid_register_index::cpuid_register_index_ebx),
        INDEX_ECX = static_cast<int>(cpuid_register_index::cpuid_register_index_ecx),
        INDEX_EDX = static_cast<int>(cpuid_register_index::cpuid_register_index_edx)
    };

    inline bool x86_cpu_present(unsigned int index) {
        constexpr unsigned int BITS_PER_UINT = 8 * sizeof(unsigned int);
        constexpr unsigned int BITS_PER_LEAF = BITS_PER_UINT * 4;

        const cpuid_feature* leaf = __x86_get_cpuid_feature_leaf(index / BITS_PER_LEAF);

        unsigned int reg_index = index % BITS_PER_LEAF;
        unsigned int bit_index = reg_index % BITS_PER_UINT;
        reg_index /= BITS_PER_UINT;

        return leaf->cpuid_array[reg_index] & (1u << bit_index);
    }

    inline bool x86_cpu_active(unsigned int index) {
        if (index == x86_cpu_IBT || index == x86_cpu_SHSTK) {
            return x86_cpu_cet_active(index);
        }

        constexpr unsigned int BITS_PER_UINT = 8 * sizeof(unsigned int);
        constexpr unsigned int BITS_PER_LEAF = BITS_PER_UINT * 4;

        const cpuid_feature* leaf = __x86_get_cpuid_feature_leaf(index / BITS_PER_LEAF);

        unsigned int reg_index = index % BITS_PER_LEAF;
        unsigned int bit_index = reg_index % BITS_PER_UINT;
        reg_index /= BITS_PER_UINT;

        return leaf->active_array[reg_index] & (1u << bit_index);
    }
    #endif
}
