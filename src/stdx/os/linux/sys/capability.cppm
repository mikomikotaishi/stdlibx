/**
 * @file capability.cppm
 * @module stdx:os.linux.sys.capability
 * @brief Module file for user library prototype information.
 *
 * This file complements the kernel file by providing prototype information for the user library,
 * located in <sys/capability.h>.
 */

module;

#ifdef __linux__
#include <sys/capability.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.capability;
#else
export module stdlibx:os.linux.sys.capability;
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
    using Cap_t = ::cap_t;
    using CapValue_t = ::cap_value_t;

    enum class CapFlag_t {
        EFFECTIVE = 0, ///< Specifies the effective flag
        PERMITTED = 1, ///< Specifies the permitted flag
        INHERITABLE = 2 ///< Specifies the inheritable flag
    };

    enum class CapIABVector_t {
        INH = 2,
        AMB = 3,
        BOUND = 4
    };

    enum class CapValue_t {
        CLEAR = 0, ///< The flag is cleared/disabled
        SET = 1 ///< The flag is set/enabled
    };

    using CapIAB_s = ::cap_iab_s;
    using CapIAB_t = ::cap_iab_t;
    using CapMode_t = ::cap_mode_t;
    using CapLaunch_s = ::cap_launch_s;
    using CapLaunch_t = ::cap_launch_t;
    using CapUserHeader_t = ::cap_user_header_t;
    using VFSCapData = ::vfs_cap_data;
    using VFSNSCapData = ::vfs_ns_cap_data;
    #endif

    using Capability_t = ::cap_t;
    using CapabilityValue_t = ::cap_value_t;
    
    enum class CapabilityFlag {
        EFFECTIVE = 0, ///< Specifies the effective flag
        PERMITTED = 1, ///< Specifies the permitted flag
        INHERITABLE = 2 ///< Specifies the inheritable flag
    };

    enum class CapabilityIABVector {
        INH = 2,
        AMB = 3,
        BOUND = 4
    };

    enum class CapFlagValue {
        CLEAR = 0, ///< The flag is cleared/disabled
        SET = 1 ///< The flag is set/enabled
    };

    using CapabilityIABStruct = ::cap_iab_s;
    using CapabilityIAB = ::cap_iab_t;
    using CapabilityMode_t = ::cap_mode_t;
    using CapabilityLaunchStruct = ::cap_launch_s;
    using CapabilityLaunch = ::cap_launch_t;
    using CapabilityUserHeader = ::cap_user_header_t;
    using VFSCapabilityData = ::vfs_cap_data;
    using VFSNSCapabilityData = ::vfs_ns_cap_data;

    using ::cap_max_bits;
    using ::cap_proc_root;

    using ::cap_dup;
    using ::cap_free;
    using ::cap_init;
    using ::cap_iab_dup;
    using ::cap_iab_init;

    using ::cap_get_flag;
    using ::cap_set_flag;
    using ::cap_clear;
    using ::cap_clear_flag;
    using ::cap_fill_flag;
    using ::cap_fill;

    using ::cap_compare;
    using ::cap_iab_compare;

    using ::cap_iab_get_vector;
    using ::cap_iab_set_vector;
    using ::cap_iab_fill;

    using ::cap_get_fd;
    using ::cap_get_file;
    using ::cap_get_nsowner;
    using ::cap_set_fd;
    using ::cap_set_file;
    using ::cap_set_nsowner;

    using ::cap_get_proc;
    using ::cap_get_pid;
    using ::cap_set_proc;

    using ::cap_get_bound;
    using ::cap_drop_bound;

    using ::cap_get_ambient;
    using ::cap_set_ambient;
    using ::cap_reset_ambient;

    using ::cap_size;
    using ::cap_copy_ext;
    using ::cap_copy_int;
    using ::cap_copy_int_check;

    using ::cap_from_text;
    using ::cap_to_text;
    using ::cap_from_name;
    using ::cap_to_name;

    using ::cap_iab_to_text;
    using ::cap_iab_from_text;

    using ::cap_set_syscall;

    using ::cap_set_mode;
    using ::cap_get_mode;
    using ::cap_mode_name;

    using ::cap_get_secbits;
    using ::cap_set_secbits;

    using ::cap_prctl;
    using ::cap_prctlw;
    using ::cap_setuid;
    using ::cap_setgroups;

    using ::cap_iab_get_proc;
    using ::cap_iab_get_pid;
    using ::cap_iab_set_proc;

    using ::cap_new_launcher;
    using ::cap_func_launcher;
    using ::cap_launcher_callback;
    using ::cap_launcher_setuid;
    using ::cap_launcher_setgroups;
    using ::cap_launcher_set_mode;
    using ::cap_launcher_set_iab;
    using ::cap_launcher_set_chroot;
    using ::cap_launch;

    using ::capget;
    using ::capset;

    using ::capgetp;
    using ::capsetp;
    #endif
}
