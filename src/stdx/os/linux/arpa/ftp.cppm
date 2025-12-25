/**
 * @file ftp.cppm
 * @module stdx:os.linux.arpa.ftp
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in <arpa/ftp.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa.ftp;
#else
export module stdlibx:os.linux.arpa.ftp;
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
    // Reply codes
    inline constexpr int PRELIM = 1; ///< positive preliminary
    inline constexpr int COMPLETE = 2; ///< positive completion
    inline constexpr int CONTINUE = 3; ///< positive intermediate
    inline constexpr int TRANSIENT = 4; ///< transient negative completion
    inline constexpr int ERROR = 5; ///< permanent negative completion

    // Type codes
    inline constexpr int TYPE_A = 1; ///< ASCII
    inline constexpr int TYPE_E = 2; ///< EBCDIC
    inline constexpr int TYPE_I = 3; ///< image
    inline constexpr int TYPE_L = 4; ///< local byte size

    const char* TypeNames[] = {"0", "ASCII", "EBCDIC", "Image", "Local" };

    // Form codes
    inline constexpr int FORM_N = 1; ///< non-print
    inline constexpr int FORM_T = 2; ///< telnet format effectors
    inline constexpr int FORM_C = 3; ///< carriage control (ASA)

    const char* FormNames[] =  {"0", "Nonprint", "Telnet", "Carriage-control" };

    // Structure codes
    inline constexpr int STRU_F = 1; ///< file (no record structure)
    inline constexpr int STRU_R = 2; ///< record structure
    inline constexpr int STRU_P = 3; ///< page structure

    const char* StruNames[] =  {"0", "File", "Record", "Page" };

    // Mode types
    inline constexpr int MODE_S = 1; ///< stream
    inline constexpr int MODE_B = 2; ///< block
    inline constexpr int MODE_C = 3; ///< compressed 

    const char* ModeNames[] =  {"0", "Stream", "Block", "Compressed" };

    // Record Tokens
    inline constexpr char REC_ESC = '\377'; ///< Record-mode Escape
    inline constexpr char REC_EOR = '\001'; ///< Record-mode End-of-Record
    inline constexpr char REC_EOF = '\002'; ///< Record-mode End-of-File

    // Block Tokens
    inline constexpr unsigned short BLK_EOR = 0x80; ///< Block is End-of-Record
    inline constexpr unsigned short BLK_EOF = 0x40; ///< Block is End-of-File
    inline constexpr unsigned short BLK_ERRORS = 0x20; ///< Block is suspected of containing errors
    inline constexpr unsigned short BLK_RESTART = 0x10; ///< Block is Restart Marker
    inline constexpr unsigned short BLK_BYTECOUNT = 2; ///< Bytes in this block
}
