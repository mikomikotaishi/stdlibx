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

export module stdx:os.linux.sys.platform.x86;

namespace __detail::stdx::os::linux::sys::platform {
    [[nodiscard]]
    inline bool x86_cpu_cet_active_ext(unsigned int index) noexcept {
        #ifdef __x86_64__
        unsigned int feature1 = 0;
        # ifdef __LP64__
        asm ("mov %%fs:72, %0" : "=r" (feature1));
        # else
        asm ("mov %%fs:40, %0" : "=r" (feature1));
        # endif
        if (index == x86_cpu_IBT) {
            return feature1 & x86_feature_1_ibt;
        } else {
            return feature1 & x86_feature_1_shstk;
        }
        #else
        return false;
        #endif
    }
}

/**
 * @namespace stdx::os::linux::sys::platform
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys::platform {
    #ifdef __linux__
    using CpuIdFeature = ::cpuid_feature;
    enum class CpuIdRegisterIndex: unsigned char {
        INDEX_EAX = static_cast<int>(cpuid_register_index::cpuid_register_index_eax),
        INDEX_EBX = static_cast<int>(cpuid_register_index::cpuid_register_index_ebx),
        INDEX_ECX = static_cast<int>(cpuid_register_index::cpuid_register_index_ecx),
        INDEX_EDX = static_cast<int>(cpuid_register_index::cpuid_register_index_edx)
    };

    [[nodiscard]]
    inline bool x86_cpu_present(unsigned int index) noexcept {
        constexpr unsigned int BITS_PER_UINT = 8 * sizeof(unsigned int);
        constexpr unsigned int BITS_PER_LEAF = BITS_PER_UINT * 4;

        const cpuid_feature* leaf = __x86_get_cpuid_feature_leaf(index / BITS_PER_LEAF);

        unsigned int reg_index = index % BITS_PER_LEAF;
        unsigned int bit_index = reg_index % BITS_PER_UINT;
        reg_index /= BITS_PER_UINT;

        return leaf->cpuid_array[reg_index] & (1u << bit_index);
    }

    [[nodiscard]]
    inline bool x86_cpu_active(unsigned int index) noexcept {
        if (index == x86_cpu_IBT || index == x86_cpu_SHSTK) {
            return __detail::stdx::os::linux::sys::platform::x86_cpu_cet_active_ext(index);
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
