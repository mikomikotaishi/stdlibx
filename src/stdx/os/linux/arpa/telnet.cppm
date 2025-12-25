/**
 * @file telnet.cppm
 * @module stdx:os.linux.arpa.telnet
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in <arpa/telnet.h>.
 */

module;

#ifdef __linux__
#define TELOPTS

#include <arpa/telnet.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa.telnet;
#else
export module stdlibx:os.linux.arpa.telnet;
#endif

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::arpa {
#else 
export namespace stdlibx::os::linux::arpa {
#endif
    #ifdef __linux__
    using ::telcmds;
    using ::telopts;
    using ::slc_names;
    using ::authtype_names;
    #endif

    const char* AuthTypeNames[] = {
        "NULL", "KERBEROS_V4", "KERBEROS_V5", "SPX", "MINK", 0,
    };

    const char* EncryptNames[] = {
        "IS", "SUPPORT", "REPLY", "START", "END",
        "REQUEST-START", "REQUEST-END", "ENC-KEYID", "DEC-KEYID",
        0,
    };

    const char* EnctypeNames[] = {
        "ANY", "DES_CFB64",  "DES_OFB64",  0,
    };
}
