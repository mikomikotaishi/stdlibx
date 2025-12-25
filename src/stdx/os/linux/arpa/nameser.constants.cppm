/**
 * @file nameser.constants.cppm
 * @module stdx:os.linux.arpa.nameser.constants
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the constants
 * located in <arpa/nameser.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa.nameser.constants;
#else
export module stdlibx:os.linux.arpa.nameser.constants;
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
    /// Constants based on RFC 883, RFC 1034, RFC 1035
    inline constexpr int NS_PACKETSZ = 512; ///< default UDP packet size
    inline constexpr int NS_MAXDNAME = 1025; ///< maximum domain name
    inline constexpr int NS_MAXMSG = 65535; ///< maximum message size
    inline constexpr int NS_MAXCDNAME = 255; ///< maximum compressed domain name
    inline constexpr int NS_MAXLABEL = 63; ///< maximum length of domain label
    inline constexpr int NS_HFIXEDSZ = 12; ///< #/bytes of fixed data in header
    inline constexpr int NS_QFIXEDSZ = 4; ///< #/bytes of fixed data in query
    inline constexpr int NS_RRFIXEDSZ = 10; ///< #/bytes of fixed data in r record
    inline constexpr int NS_INT32SZ = 4; ///< #/bytes of data in a uint32_t
    inline constexpr int NS_INT16SZ = 2; ///< #/bytes of data in a uint16_t
    inline constexpr int NS_INT8SZ = 1; ///< #/bytes of data in a uint8_t
    inline constexpr int NS_INADDRSZ = 4; ///< IPv4 T_A
    inline constexpr int NS_IN6ADDRSZ = 16; ///< IPv6 T_AAAA
    inline constexpr int NS_CMPRSFLGS = 0xc0; ///< Flag bits indicating name compression
    inline constexpr int NS_DEFAULTPORT = 53; ///< For both TCP and UDP

    inline constexpr int NS_TSIG_FUDGE = 300;
    inline constexpr int NS_TSIG_TCP_COUNT = 100;
    inline constexpr const char* NS_TSIG_ALG_HMAC_MD5 = "HMAC-MD5.SIG-ALG.REG.INT";

    inline constexpr int NS_TSIG_ERROR_NO_TSIG = -10;
    inline constexpr int NS_TSIG_ERROR_NO_SPACE = -11;
    inline constexpr int NS_TSIG_ERROR_FORMERR = -12;
}
