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

export module stdx:os.linux.arpa.telnet;

import :core;

using namespace stdx::core;

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::arpa {
    #ifdef __linux__
    using ::telcmds;
    using ::telopts;
    using ::slc_names;
    using ::authtype_names;
    #endif

    inline constexpr Array<const char*, 5> AuthTypeNames = {
        "NULL", "KERBEROS_V4", "KERBEROS_V5", "SPX", "MINK"
    };

    inline constexpr Array<const char*, 9> EncryptNames = {
        "IS", "SUPPORT", "REPLY", "START", "END",
        "REQUEST-START", "REQUEST-END", "ENC-KEYID", "DEC-KEYID",
    };

    inline constexpr Array<const char*, 3> EnctypeNames = {
        "ANY", "DES_CFB64",  "DES_OFB64"
    };
}
