#pragma once

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::arpa {
    inline constexpr Array<const char*, 21> TelCmds = {
        "EOF", "SUSP", "ABORT", "EOR",
        "SE", "NOP", "DMARK", "BRK", "IP", "AO", "AYT", "EC",
        "EL", "GA", "SB", "WILL", "WONT", "DO", "DONT", "IAC",
    };

    inline constexpr Array<const char*, NTELOPTS> TelOptNames = {
        "BINARY", "ECHO", "RCP", "SUPPRESS GO AHEAD", "NAME",
        "STATUS", "TIMING MARK", "RCTE", "NAOL", "NAOP",
        "NAOCRD", "NAOHTS", "NAOHTD", "NAOFFD", "NAOVTS",
        "NAOVTD", "NAOLFD", "EXTEND ASCII", "LOGOUT", "BYTE MACRO",
        "DATA ENTRY TERMINAL", "SUPDUP", "SUPDUP OUTPUT",
        "SEND LOCATION", "TERMINAL TYPE", "END OF RECORD",
        "TACACS UID", "OUTPUT MARKING", "TTYLOC",
        "3270 REGIME", "X.3 PAD", "NAWS", "TSPEED", "LFLOW",
        "LINEMODE", "XDISPLOC", "OLD-ENVIRON", "AUTHENTICATION",
        "ENCRYPT", "NEW-ENVIRON",
    };

    inline constexpr Array<const char*, 20> SlcNames = {
        "0", "SYNCH", "BRK", "IP", "AO", "AYT", "EOR",
        "ABORT", "EOF", "SUSP", "EC", "EL", "EW", "RP",
        "LNEXT", "XON", "XOFF", "FORW1", "FORW2",
    };

    inline constexpr Array<const char*, 5> AuthTypeNames = {
        "NULL", "KERBEROS_V4", "KERBEROS_V5", "SPX", "MINK"
    };

    inline constexpr Array<const char*, 9> EncryptNames = {
        "IS", "SUPPORT", "REPLY", "START", "END",
        "REQUEST-START", "REQUEST-END", "ENC-KEYID", "DEC-KEYID",
    };

    inline constexpr Array<const char*, 3> EnctypeNames = {
        "ANY", "DES_CFB64",  "DES_OFB64",
    };
}