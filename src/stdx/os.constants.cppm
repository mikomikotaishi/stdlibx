/**
 * @file os.constants.cppm
 * @module stdx:os.constants
 * @brief Module file for operating system constants.
 *
 * This file contains the imports for the operating system constants.
 */

module;

export module stdx:os.constants;

import core;

using namespace core;

/**
 * @namespace stdx::os
 * @brief Wrapper namespace for operating system operations.
 */
export namespace stdx::os {
    /**
     * @namespace linux
     * @brief Wrapper namespace for Linux operations.
     */
    namespace linux {
        /**
         * @namespace arpa
         * @brief Wrapper namespace for ARPA operations.
         */
        namespace arpa {
            /**
             * <arpa/ftp.h> constants
             */

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

            inline constexpr Array<const char*, 5> TypeNames = {"0", "ASCII", "EBCDIC", "Image", "Local" };

            // Form codes
            inline constexpr int FORM_N = 1; ///< non-print
            inline constexpr int FORM_T = 2; ///< telnet format effectors
            inline constexpr int FORM_C = 3; ///< carriage control (ASA)

            inline constexpr Array<const char*, 4> FormNames = {"0", "Nonprint", "Telnet", "Carriage-control" };

            // Structure codes
            inline constexpr int STRU_F = 1; ///< file (no record structure)
            inline constexpr int STRU_R = 2; ///< record structure
            inline constexpr int STRU_P = 3; ///< page structure

            inline constexpr Array<const char*, 4> StruNames = {"0", "File", "Record", "Page" };

            // Mode types
            inline constexpr int MODE_S = 1; ///< stream
            inline constexpr int MODE_B = 2; ///< block
            inline constexpr int MODE_C = 3; ///< compressed 

            inline constexpr Array<const char*, 4> ModeNames = {"0", "Stream", "Block", "Compressed" };

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

            /**
             * <arpa/nameser.h> constants
             */
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

            /**
             * <arpa/telnet.h> constants
             */
            inline constexpr unsigned char IAC = 255; ///< interpret as command
            inline constexpr unsigned char DONT = 254; ///< you are not to use option
            inline constexpr unsigned char DO = 253; ///< please, you use option
            inline constexpr unsigned char WONT = 252; ///< I won't use option
            inline constexpr unsigned char WILL = 251; ///< I will use option
            inline constexpr unsigned char SB = 250; ///< interpret as subnegotiation
            inline constexpr unsigned char GA = 249; ///< you may reverse the line
            inline constexpr unsigned char EL = 248; ///< erase the current line
            inline constexpr unsigned char EC = 247; ///< erase the current character
            inline constexpr unsigned char AYT = 246; ///< are you there
            inline constexpr unsigned char AO = 245; ///< abort output--but let prog finish
            inline constexpr unsigned char IP = 244; ///< interrupt process--permanently
            inline constexpr unsigned char BREAK = 243; ///< break
            inline constexpr unsigned char DM = 242; ///< data mark--for connect. cleaning
            inline constexpr unsigned char NOP = 241; ///< nop
            inline constexpr unsigned char SE = 240; ///< end sub negotiation
            inline constexpr unsigned char EOR = 239; ///< end of record (transparent mode)
            inline constexpr unsigned char ABORT = 238; ///< Abort process
            inline constexpr unsigned char SUSP = 237; ///< Suspend process
            inline constexpr unsigned char xEOF = 236; ///< End of file: EOF is already used...
            inline constexpr unsigned char SYNCH = 242; ///< for telfunc calls

            inline constexpr unsigned char TELCMD_FIRST = xEOF; ///< First telnet command
            inline constexpr unsigned char TELCMD_LAST = IAC; ///< Last telnet command

            constexpr bool TELCMD_OK(unsigned char x) noexcept {
                return x <= TELCMD_LAST && x >= TELCMD_FIRST;
            }

            inline constexpr unsigned char TELOPT_BINARY = 0; ///< 8-bit data path
            inline constexpr unsigned char TELOPT_ECHO = 1; ///< echo
            inline constexpr unsigned char TELOPT_RCP = 2; ///< prepare to reconnect
            inline constexpr unsigned char TELOPT_SGA = 3; ///< suppress go ahead
            inline constexpr unsigned char TELOPT_NAMS = 4; ///< approximate message size
            inline constexpr unsigned char TELOPT_STATUS = 5; ///< give status
            inline constexpr unsigned char TELOPT_TM = 6; ///< timing mark
            inline constexpr unsigned char TELOPT_RCTE = 7; ///< remote controlled transmission and echo
            inline constexpr unsigned char TELOPT_NAOL = 8; ///< negotiate about output line width
            inline constexpr unsigned char TELOPT_NAOP = 9; ///< negotiate about output page size
            inline constexpr unsigned char TELOPT_NAOCRD = 10; ///< negotiate about CR disposition
            inline constexpr unsigned char TELOPT_NAOHTS = 11; ///< negotiate about horizontal tabstops
            inline constexpr unsigned char TELOPT_NAOHTD = 12; ///< negotiate about horizontal tab disposition
            inline constexpr unsigned char TELOPT_NAOFFD = 13; ///< negotiate about formfeed disposition
            inline constexpr unsigned char TELOPT_NAOVTS = 14; ///< negotiate about vertical tab stops
            inline constexpr unsigned char TELOPT_NAOVTD = 15; ///< negotiate about vertical tab disposition
            inline constexpr unsigned char TELOPT_NAOLFD = 16; ///< negotiate about output LF disposition
            inline constexpr unsigned char TELOPT_XASCII = 17; ///< extended ascii character set
            inline constexpr unsigned char TELOPT_LOGOUT = 18; ///< force logout
            inline constexpr unsigned char TELOPT_BM = 19; ///< byte macro
            inline constexpr unsigned char TELOPT_DET = 20; ///< data entry terminal
            inline constexpr unsigned char TELOPT_SUPDUP = 21; ///< supdup protocol
            inline constexpr unsigned char TELOPT_SUPDUPOUTPUT = 22; ///< supdup output
            inline constexpr unsigned char TELOPT_SNDLOC = 23; ///< send location
            inline constexpr unsigned char TELOPT_TTYPE = 24; ///< terminal type
            inline constexpr unsigned char TELOPT_EOR = 25; ///< end or record
            inline constexpr unsigned char TELOPT_TUID = 26; ///< TACACS user identification
            inline constexpr unsigned char TELOPT_OUTMRK = 27; ///< output marking
            inline constexpr unsigned char TELOPT_TTYLOC = 28; ///< terminal location number
            inline constexpr unsigned char TELOPT_3270REGIME = 29; ///< 3270 regime
            inline constexpr unsigned char TELOPT_X3PAD = 30; ///< X.3 PAD
            inline constexpr unsigned char TELOPT_NAWS = 31; ///< window size
            inline constexpr unsigned char TELOPT_TSPEED = 32; ///< terminal speed
            inline constexpr unsigned char TELOPT_LFLOW = 33; ///< remote flow control
            inline constexpr unsigned char TELOPT_LINEMODE = 34; ///< Linemode option
            inline constexpr unsigned char TELOPT_XDISPLOC = 35; ///< X Display Location
            inline constexpr unsigned char TELOPT_OLD_ENVIRON = 36; ///< Old - Environment variables
            inline constexpr unsigned char TELOPT_AUTHENTICATION = 37; ///< Authenticate
            inline constexpr unsigned char TELOPT_ENCRYPT = 38; ///< Encryption option
            inline constexpr unsigned char TELOPT_NEW_ENVIRON = 39; ///< New - Environment variables
            inline constexpr unsigned char TELOPT_EXOPL = 255; ///< extended-options-list

            inline constexpr int NTELOPTS = 1 + TELOPT_NEW_ENVIRON;
            inline constexpr unsigned char TELOPT_FIRST = TELOPT_BINARY;
            inline constexpr unsigned char TELOPT_LAST = TELOPT_NEW_ENVIRON;

            constexpr bool TELOPT_OK(unsigned char x) noexcept {
                return x <= TELOPT_LAST;
            }

            // sub-option qualifiers
            inline constexpr unsigned char TELQUAL_IS = 0; ///< option is...
            inline constexpr unsigned char TELQUAL_SEND = 1; ///< send option
            inline constexpr unsigned char TELQUAL_INFO = 2; ///< ENVIRON: informational version of IS
            inline constexpr unsigned char TELQUAL_REPLY = 2; ///< AUTHENTICATION: client version of IS
            inline constexpr unsigned char TELQUAL_NAME = 3; ///< AUTHENTICATION: client version of IS

            inline constexpr unsigned char LFLOW_OFF = 0; ///< Disable remote flow control
            inline constexpr unsigned char LFLOW_ON = 1; ///< Enable remote flow control
            inline constexpr unsigned char LFLOW_RESTART_ANY = 2; ///< Restart output on any char
            inline constexpr unsigned char LFLOW_RESTART_XON = 3; ///< Restart output only on XON

            // LINEMODE suboptions
            inline constexpr unsigned char LM_MODE = 1;
            inline constexpr unsigned char LM_FORWARDMASK = 2;
            inline constexpr unsigned char LM_SLC = 3;

            inline constexpr unsigned char MODE_EDIT = 0x01;
            inline constexpr unsigned char MODE_TRAPSIG = 0x02;
            inline constexpr unsigned char MODE_ACK = 0x04;
            inline constexpr unsigned char MODE_SOFT_TAB = 0x08;
            inline constexpr unsigned char MODE_LIT_ECHO = 0x10;

            inline constexpr unsigned char MODE_MASK = 0x1f;

            // Not part of protocol, but needed to simplify things...
            inline constexpr unsigned short MODE_FLOW = 0x0100;
            inline constexpr unsigned short MODE_ECHO = 0x0200;
            inline constexpr unsigned short MODE_INBIN = 0x0400;
            inline constexpr unsigned short MODE_OUTBIN = 0x0800;
            inline constexpr unsigned short MODE_FORCE = 0x1000;

            inline constexpr unsigned char SLC_SYNCH = 1;
            inline constexpr unsigned char SLC_BRK = 2;
            inline constexpr unsigned char SLC_IP = 3;
            inline constexpr unsigned char SLC_AO = 4;
            inline constexpr unsigned char SLC_AYT = 5;
            inline constexpr unsigned char SLC_EOR = 6;
            inline constexpr unsigned char SLC_ABORT = 7;
            inline constexpr unsigned char SLC_EOF = 8;
            inline constexpr unsigned char SLC_SUSP = 9;
            inline constexpr unsigned char SLC_EC = 10;
            inline constexpr unsigned char SLC_EL = 11;
            inline constexpr unsigned char SLC_EW = 12;
            inline constexpr unsigned char SLC_RP = 13;
            inline constexpr unsigned char SLC_LNEXT = 14;
            inline constexpr unsigned char SLC_XON = 15;
            inline constexpr unsigned char SLC_XOFF = 16;
            inline constexpr unsigned char SLC_FORW1 = 17;
            inline constexpr unsigned char SLC_FORW2 = 18;

            inline constexpr int NSLC = 18;

            constexpr bool SLC_NAME_OK(unsigned char x) noexcept {
                return x <= NSLC;
            }

            inline constexpr unsigned char SLC_NOSUPPORT = 0;
            inline constexpr unsigned char SLC_CANTCHANGE = 1;
            inline constexpr unsigned char SLC_VARIABLE = 2;
            inline constexpr unsigned char SLC_DEFAULT = 3;
            inline constexpr unsigned char SLC_LEVELBITS = 0x03;

            inline constexpr unsigned char SLC_FUNC = 0;
            inline constexpr unsigned char SLC_FLAGS = 1;
            inline constexpr unsigned char SLC_VALUE = 2;

            inline constexpr unsigned char SLC_ACK = 0x80;
            inline constexpr unsigned char SLC_FLUSHIN = 0x40;
            inline constexpr unsigned char SLC_FLUSHOUT = 0x20;

            inline constexpr unsigned char OLD_ENV_VAR = 1;
            inline constexpr unsigned char OLD_ENV_VALUE = 0;
            inline constexpr unsigned char NEW_ENV_VAR = 0;
            inline constexpr unsigned char NEW_ENV_VALUE = 1;
            inline constexpr unsigned char ENV_ESC = 2;
            inline constexpr unsigned char ENV_USERVAR = 3;

            // AUTHENTICATION suboptions

            // Who is authenticating who ...
            inline constexpr unsigned char AUTH_WHO_CLIENT = 0; ///< Client authenticating server
            inline constexpr unsigned char AUTH_WHO_SERVER = 1; ///< Server authenticating client
            inline constexpr unsigned char AUTH_WHO_MASK = 1;

            // amount of authentication done
            inline constexpr unsigned char AUTH_HOW_ONE_WAY = 0;
            inline constexpr unsigned char AUTH_HOW_MUTUAL = 2;
            inline constexpr unsigned char AUTH_HOW_MASK = 2;

            inline constexpr unsigned char AUTHTYPE_NULL = 0;
            inline constexpr unsigned char AUTHTYPE_KERBEROS_V4 = 1;
            inline constexpr unsigned char AUTHTYPE_KERBEROS_V5 = 2;
            inline constexpr unsigned char AUTHTYPE_SPX = 3;
            inline constexpr unsigned char AUTHTYPE_MINK = 4;
            inline constexpr unsigned char AUTHTYPE_CNT = 5;

            inline constexpr unsigned char AUTHTYPE_TEST = 99;

            constexpr bool AUTHTYPE_NAME_OK(unsigned char x) noexcept {
                return x < AUTHTYPE_CNT;
            }

            // ENCRYPTion suboptions
            inline constexpr unsigned char ENCRYPT_IS = 0; ///< I pick encryption type ...
            inline constexpr unsigned char ENCRYPT_SUPPORT = 1; ///< I support encryption types ...
            inline constexpr unsigned char ENCRYPT_REPLY = 2; ///< Initial setup response
            inline constexpr unsigned char ENCRYPT_START = 3; ///< Am starting to send encrypted
            inline constexpr unsigned char ENCRYPT_END = 4; ///< Am ending encrypted
            inline constexpr unsigned char ENCRYPT_REQSTART = 5; ///< Request you start encrypting
            inline constexpr unsigned char ENCRYPT_REQEND = 6; ///< Request you send encrypting
            inline constexpr unsigned char ENCRYPT_ENC_KEYID = 7;
            inline constexpr unsigned char ENCRYPT_DEC_KEYID = 8;
            inline constexpr unsigned char ENCRYPT_CNT = 9;

            inline constexpr unsigned char ENCTYPE_ANY = 0;
            inline constexpr unsigned char ENCTYPE_DES_CFB64 = 1;
            inline constexpr unsigned char ENCTYPE_DES_OFB64 = 2;
            inline constexpr unsigned char ENCTYPE_CNT = 3;

            constexpr bool ENCRYPT_NAME_OK(unsigned char x) noexcept {
                return x < ENCRYPT_CNT;
            }

            constexpr bool ENCTYPE_NAME_OK(unsigned char x) noexcept {
                return x < ENCTYPE_CNT;
            }
        }

        /**
         * @namespace stdx::os::linux::asmgeneric
         * @brief Wrapper namespace for Linux asm-generic error number operations.
         */
        namespace asmgeneric {
            /**
             * <asm-generic/errno.h> constants
             */
            inline constexpr int EPERM = 1; ///< Operation not permitted
            inline constexpr int ENOENT = 2; ///< No such file or directory
            inline constexpr int ESRCH = 3; ///< No such process
            inline constexpr int EINTR = 4; ///< Interrupted system call
            inline constexpr int EIO = 5; ///< I/O error
            inline constexpr int ENXIO = 6; ///< No such device or address
            inline constexpr int E2BIG = 7; ///< Argument list too long
            inline constexpr int ENOEXEC = 8; ///< Exec format error
            inline constexpr int EBADF = 9; ///< Bad file number
            inline constexpr int ECHILD = 10; ///< No child processes
            inline constexpr int EAGAIN = 11; ///< Try again
            inline constexpr int ENOMEM = 12; ///< Out of memory
            inline constexpr int EACCES = 13; ///< Permission denied
            inline constexpr int EFAULT = 14; ///< Bad address
            inline constexpr int ENOTBLK = 15; ///< Block device required
            inline constexpr int EBUSY = 16; ///< Device or resource busy
            inline constexpr int EEXIST = 17; ///< File exists
            inline constexpr int EXDEV = 18; ///< Cross-device link
            inline constexpr int ENODEV = 19; ///< No such device
            inline constexpr int ENOTDIR = 20; ///< Not a directory
            inline constexpr int EISDIR = 21; ///< Is a directory
            inline constexpr int EINVAL = 22; ///< Invalid argument
            inline constexpr int ENFILE = 23; ///< File table overflow
            inline constexpr int EMFILE = 24; ///< Too many open files
            inline constexpr int ENOTTY = 25; ///< Not a typewriter
            inline constexpr int ETXTBSY = 26; ///< Text file busy
            inline constexpr int EFBIG = 27; ///< File too large
            inline constexpr int ENOSPC = 28; ///< No space left on device
            inline constexpr int ESPIPE = 29; ///< Illegal seek
            inline constexpr int EROFS = 30; ///< Read-only file system
            inline constexpr int EMLINK = 31; ///< Too many links
            inline constexpr int EPIPE = 32; ///< Broken pipe
            inline constexpr int EDOM = 33; ///< Math argument out of domain of func
            inline constexpr int ERANGE = 34; ///< Math result not representable
            inline constexpr int EDEADLK = 35; ///< Resource deadlock would occur
            inline constexpr int ENAMETOOLONG = 36; ///< File name too long
            inline constexpr int ENOLCK = 37; ///< No record locks available
            inline constexpr int ENOSYS = 38; ///< Invalid system call number
            inline constexpr int ENOTEMPTY = 39; ///< Directory not empty
            inline constexpr int ELOOP = 40; ///< Too many symbolic links encountered
            inline constexpr int EWOULDBLOCK = EAGAIN; ///< Operation would block
            inline constexpr int ENOMSG = 42; ///< No message of desired type
            inline constexpr int EIDRM = 43; ///< Identifier removed
            inline constexpr int ECHRNG = 44; ///< Channel number out of range
            inline constexpr int EL2NSYNC = 45; ///< Level 2 not synchronized
            inline constexpr int EL3HLT = 46; ///< Level 3 halted
            inline constexpr int EL3RST = 47; ///< Level 3 reset
            inline constexpr int ELNRNG = 48; ///< Link number out of range
            inline constexpr int EUNATCH = 49; ///< Protocol driver not attached
            inline constexpr int ENOCSI = 50; ///< No CSI structure available
            inline constexpr int EL2HLT = 51; ///< Level 2 halted
            inline constexpr int EBADE = 52; ///< Invalid exchange
            inline constexpr int EBADR = 53; ///< Invalid request descriptor
            inline constexpr int EXFULL = 54; ///< Exchange full
            inline constexpr int ENOANO = 55; ///< No anode
            inline constexpr int EBADRQC = 56; ///< Invalid request code
            inline constexpr int EBADSLT = 57; ///< Invalid slot
            inline constexpr int EDEADLOCK = EDEADLK; ///< Alias for EDEADLK
            inline constexpr int EBFONT = 59; ///< Bad font file format
            inline constexpr int ENOSTR = 60; ///< Device not a stream
            inline constexpr int ENODATA = 61; ///< No data available
            inline constexpr int ETIME = 62; ///< Timer expired
            inline constexpr int ENOSR = 63; ///< Out of streams resources
            inline constexpr int ENONET = 64; ///< Machine is not on the network
            inline constexpr int ENOPKG = 65; ///< Package not installed
            inline constexpr int EREMOTE = 66; ///< Object is remote
            inline constexpr int ENOLINK = 67; ///< Link has been severed
            inline constexpr int EADV = 68; ///< Advertise error
            inline constexpr int ESRMNT = 69; ///< Srmount error
            inline constexpr int ECOMM = 70; ///< Communication error on send
            inline constexpr int EPROTO = 71; ///< Protocol error
            inline constexpr int EMULTIHOP = 72; ///< Multihop attempted
            inline constexpr int EDOTDOT = 73; ///< RFS specific error
            inline constexpr int EBADMSG = 74; ///< Not a data message
            inline constexpr int EOVERFLOW = 75; ///< Value too large for defined data type
            inline constexpr int ENOTUNIQ = 76; ///< Name not unique on network
            inline constexpr int EBADFD = 77; ///< File descriptor in bad state
            inline constexpr int EREMCHG = 78; ///< Remote address changed
            inline constexpr int ELIBACC = 79; ///< Cannot access a needed shared library
            inline constexpr int ELIBBAD = 80; ///< Accessing a corrupted shared library
            inline constexpr int ELIBSCN = 81; ///< .lib section in a.out corrupted
            inline constexpr int ELIBMAX = 82; ///< Too many shared libraries
            inline constexpr int ELIBEXEC = 83; ///< Cannot exec a shared library directly
            inline constexpr int EILSEQ = 84; ///< Illegal byte sequence
            inline constexpr int ERESTART = 85; ///< Interrupted system call should be restarted
            inline constexpr int ESTRPIPE = 86; ///< Streams pipe error
            inline constexpr int EUSERS = 87; ///< Too many users
            inline constexpr int ENOTSOCK = 88; ///< Socket operation on non-socket
            inline constexpr int EDESTADDRREQ = 89; ///< Destination address required
            inline constexpr int EMSGSIZE = 90; ///< Message too long
            inline constexpr int EPROTOTYPE = 91; ///< Protocol wrong type for socket
            inline constexpr int ENOPROTOOPT = 92; ///< Protocol not available
            inline constexpr int EPROTONOSUPPORT = 93; ///< Protocol not supported
            inline constexpr int ESOCKTNOSUPPORT = 94; ///< Socket type not supported
            inline constexpr int EOPNOTSUPP = 95; ///< Operation not supported on transport endpoint
            inline constexpr int EPFNOSUPPORT = 96; ///< Protocol family not supported
            inline constexpr int EAFNOSUPPORT = 97; ///< Address family not supported by protocol
            inline constexpr int EADDRINUSE = 98; ///< Address already in use
            inline constexpr int EADDRNOTAVAIL = 99; ///< Cannot assign requested address
            inline constexpr int ENETDOWN = 100; ///< Network is down
            inline constexpr int ENETUNREACH = 101; ///< Network is unreachable
            inline constexpr int ENETRESET = 102; ///< Network dropped connection because of reset
            inline constexpr int ECONNABORTED = 103; ///< Software caused connection abort
            inline constexpr int ECONNRESET = 104; ///< Connection reset by peer
            inline constexpr int ENOBUFS = 105; ///< No buffer space available
            inline constexpr int EISCONN = 106; ///< Transport endpoint is already connected
            inline constexpr int ENOTCONN = 107; ///< Transport endpoint is not connected
            inline constexpr int ESHUTDOWN = 108; ///< Cannot send after transport endpoint shutdown
            inline constexpr int ETOOMANYREFS = 109; ///< Too many references: cannot splice
            inline constexpr int ETIMEDOUT = 110; ///< Connection timed out
            inline constexpr int ECONNREFUSED = 111; ///< Connection refused
            inline constexpr int EHOSTDOWN = 112; ///< Host is down
            inline constexpr int EHOSTUNREACH = 113; ///< No route to host
            inline constexpr int EALREADY = 114; ///< Operation already in progress
            inline constexpr int EINPROGRESS = 115; ///< Operation now in progress
            inline constexpr int ESTALE = 116; ///< Stale file handle
            inline constexpr int EUCLEAN = 117; ///< Structure needs cleaning
            inline constexpr int ENOTNAM = 118; ///< Not a XENIX named type file
            inline constexpr int ENAVAIL = 119; ///< No XENIX semaphores available
            inline constexpr int EISNAM = 120; ///< Is a named type file
            inline constexpr int EREMOTEIO = 121; ///< Remote I/O error
            inline constexpr int EDQUOT = 122; ///< Quota exceeded
            inline constexpr int ENOMEDIUM = 123; ///< No medium found
            inline constexpr int EMEDIUMTYPE = 124; ///< Wrong medium type
            inline constexpr int ECANCELED = 125; ///< Operation Canceled
            inline constexpr int ENOKEY = 126; ///< Required key not available
            inline constexpr int EKEYEXPIRED = 127; ///< Key has expired
            inline constexpr int EKEYREVOKED = 128; ///< Key has been revoked
            inline constexpr int EKEYREJECTED = 129; ///< Key was rejected by service
            inline constexpr int EOWNERDEAD = 130; ///< Owner died
            inline constexpr int ENOTRECOVERABLE = 131; ///< State not recoverable
            inline constexpr int ERFKILL = 132; ///< Operation not possible due to RF-kill
            inline constexpr int EHWPOISON = 133; ///< Memory page has hardware error
        }

        /**
         * @namespace sys
         * @brief Wrapper namespace for Linux system operations.
         */
        namespace sys {
            /**
             * <sys/inotify.h> constants
             */
            inline constexpr int IN_CLOEXEC = 02000000;
            inline constexpr int IN_NONBLOCK = 00004000;
        }
    }

    /**
     * @namespace unix
     * @brief Wrapper namespace for Unix operations.
     */
    namespace unix {
        /**
         * <glob.h> constants
         */
        inline constexpr int GLOB_ERR = (1 << 0); ///< Return on read errors.
        inline constexpr int GLOB_MARK = (1 << 1); ///< Append a slash to each name.
        inline constexpr int GLOB_NOSORT = (1 << 2); ///< Don't sort the names.
        inline constexpr int GLOB_DOOFFS = (1 << 3); ///< Insert PGLOB->gl_offs NULLs.
        inline constexpr int GLOB_NOCHECK = (1 << 4); ///< If nothing matches, return the pattern.
        inline constexpr int GLOB_APPEND = (1 << 5); ///< Append to results of a previous call.
        inline constexpr int GLOB_NOESCAPE = (1 << 6); ///< Backslashes don't quote metacharacters.
        inline constexpr int GLOB_PERIOD = (1 << 7); ///< Leading `.` can be matched by metachars.

        inline constexpr int GLOB_MAGCHAR = (1 << 8); ///< Set in gl_flags if any metachars seen.
        inline constexpr int GLOB_ALTDIRFUNC = (1 << 9); ///< Use gl_opendir et al functions.
        inline constexpr int GLOB_BRACE = (1 << 10); ///< Expand "{a,b}" to "a" "b".
        inline constexpr int GLOB_NOMAGIC = (1 << 11); ///< If no magic chars, return the pattern.
        inline constexpr int GLOB_TILDE = (1 << 12); ///< Expand ~user and ~ to home directories.
        inline constexpr int GLOB_ONLYDIR = (1 << 13); ///< Match only directories.
        inline constexpr int GLOB_TILDE_CHECK = (1 << 14); ///< Like GLOB_TILDE but return an error if user name is unavailable.

        inline constexpr int __GLOB_FLAGS =
            GLOB_ERR | GLOB_MARK | GLOB_NOSORT | GLOB_DOOFFS |
            GLOB_NOESCAPE | GLOB_NOCHECK | GLOB_APPEND | GLOB_PERIOD |
            GLOB_ALTDIRFUNC | GLOB_BRACE | GLOB_NOMAGIC |
            GLOB_TILDE | GLOB_ONLYDIR | GLOB_TILDE_CHECK;

        /// Error returns from glob
        inline constexpr int GLOB_NOSPACE = 1; ///< Ran out of memory.
        inline constexpr int GLOB_ABORTED = 2; ///< Read error.
        inline constexpr int GLOB_NOMATCH = 3; ///< No matches found.
        inline constexpr int GLOB_NOSYS = 4; ///< Not implemented.

        /**
         * <termios.h> constants
         */
        // Baud rates (c_cflag bit meanings)
        inline constexpr unsigned int B0 = 0000000; ///< Hang up
        inline constexpr unsigned int B50 = 0000001;
        inline constexpr unsigned int B75 = 0000002;
        inline constexpr unsigned int B110 = 0000003;
        inline constexpr unsigned int B134 = 0000004;
        inline constexpr unsigned int B150 = 0000005;
        inline constexpr unsigned int B200 = 0000006;
        inline constexpr unsigned int B300 = 0000007;
        inline constexpr unsigned int B600 = 0000010;
        inline constexpr unsigned int B1200 = 0000011;
        inline constexpr unsigned int B1800 = 0000012;
        inline constexpr unsigned int B2400 = 0000013;
        inline constexpr unsigned int B4800 = 0000014;
        inline constexpr unsigned int B9600 = 0000015;
        inline constexpr unsigned int B19200 = 0000016;
        inline constexpr unsigned int B38400 = 0000017;

        // Aliases (used with __USE_MISC)
        inline constexpr unsigned int EXTA = B19200;
        inline constexpr unsigned int EXTB = B38400;

        // TIOCSERGETLSR result bits (used with ioctl)
        inline constexpr unsigned int TIOCSER_TEMT = 0x01; ///< Transmitter physically empty

        // tcflow() / TCXONC options
        inline constexpr int TCOOFF = 0;
        inline constexpr int TCOON = 1;
        inline constexpr int TCIOFF = 2;
        inline constexpr int TCION = 3;

        // tcflush() / TCFLSH options
        inline constexpr int TCIFLUSH = 0;
        inline constexpr int TCOFLUSH = 1;
        inline constexpr int TCIOFLUSH = 2;

        inline constexpr int VINTR = 0;
        inline constexpr int VQUIT = 1;
        inline constexpr int VERASE = 2;
        inline constexpr int VKILL = 3;
        inline constexpr int VEOF = 4;
        inline constexpr int VTIME = 5;
        inline constexpr int VMIN = 6;
        inline constexpr int VSWTC = 7;
        inline constexpr int VSTART = 8;
        inline constexpr int VSTOP = 9;
        inline constexpr int VSUSP = 10;
        inline constexpr int VEOL = 11;
        inline constexpr int VREPRINT = 12;
        inline constexpr int VDISCARD = 13;
        inline constexpr int VWERASE = 14;
        inline constexpr int VLNEXT = 15;
        inline constexpr int VEOL2 = 16;

        inline constexpr unsigned int IGNBRK = 0000001;  ///< Ignore break condition
        inline constexpr unsigned int BRKINT = 0000002;  ///< Signal interrupt on break
        inline constexpr unsigned int IGNPAR = 0000004;  ///< Ignore characters with parity errors
        inline constexpr unsigned int PARMRK = 0000010;  ///< Mark parity and framing errors
        inline constexpr unsigned int INPCK = 0000020;  ///< Enable input parity check
        inline constexpr unsigned int ISTRIP = 0000040;  ///< Strip 8th bit off characters
        inline constexpr unsigned int INLCR = 0000100;  ///< Map NL to CR on input
        inline constexpr unsigned int IGNCR = 0000200;  ///< Ignore CR
        inline constexpr unsigned int ICRNL = 0000400;  ///< Map CR to NL on input
        inline constexpr unsigned int IUCLC = 0001000;  ///< Map uppercase to lowercase (non-POSIX)
        inline constexpr unsigned int IXON = 0002000;  ///< Enable start/stop output control
        inline constexpr unsigned int IXANY = 0004000;  ///< Any char restarts output
        inline constexpr unsigned int IXOFF = 0010000;  ///< Enable start/stop input control
        inline constexpr unsigned int IMAXBEL = 0020000;  ///< Bell on input queue full (non-POSIX)
        inline constexpr unsigned int IUTF8 = 0040000;  ///< Input is UTF-8 (non-POSIX)

        inline constexpr unsigned int OPOST = 0000001; ///< Post-process output
        inline constexpr unsigned int OLCUC = 0000002; ///< Map lowercase to uppercase (non-POSIX)
        inline constexpr unsigned int ONLCR = 0000004; ///< Map NL to CR-NL on output
        inline constexpr unsigned int OCRNL = 0000010; ///< Map CR to NL on output
        inline constexpr unsigned int ONOCR = 0000020; ///< No CR at column 0
        inline constexpr unsigned int ONLRET = 0000040; ///< NL performs CR function
        inline constexpr unsigned int OFILL = 0000100; ///< Use fill characters for delay
        inline constexpr unsigned int OFDEL = 0000200; ///< Fill is DEL

        // Newline delays
        inline constexpr unsigned int NLDLY = 0000400;
        inline constexpr unsigned int NL0 = 0000000;
        inline constexpr unsigned int NL1 = 0000400;

        // Carriage-return delays
        inline constexpr unsigned int CRDLY = 0003000;
        inline constexpr unsigned int CR0 = 0000000;
        inline constexpr unsigned int CR1 = 0001000;
        inline constexpr unsigned int CR2 = 0002000;
        inline constexpr unsigned int CR3 = 0003000;

        // Horizontal-tab delays
        inline constexpr unsigned int TABDLY = 0014000;
        inline constexpr unsigned int TAB0 = 0000000;
        inline constexpr unsigned int TAB1 = 0004000;
        inline constexpr unsigned int TAB2 = 0010000;
        inline constexpr unsigned int TAB3 = 0014000; ///< Expand tabs to spaces

        // Backspace delays
        inline constexpr unsigned int BSDLY = 0020000;
        inline constexpr unsigned int BS0 = 0000000;
        inline constexpr unsigned int BS1 = 0020000;

        // Vertical-tab delays
        inline constexpr unsigned int VTDLY = 0040000;
        inline constexpr unsigned int VT0 = 0000000;
        inline constexpr unsigned int VT1 = 0040000;

        // Form-feed delays
        inline constexpr unsigned int FFDLY = 0100000;
        inline constexpr unsigned int FF0 = 0000000;
        inline constexpr unsigned int FF1 = 0100000;

        // Non-POSIX alias
        inline constexpr unsigned int XTABS = 0014000;

        inline constexpr unsigned int CBAUD = 000000010017; ///< Baud speed mask (non-POSIX)
        inline constexpr unsigned int CBAUDEX = 000000010000; ///< Extra baud speed mask, part of CBAUD (non-POSIX)
        inline constexpr unsigned int CIBAUD = 002003600000; ///< Input baud rate mask (unused, non-POSIX)
        inline constexpr unsigned int CMSPAR = 010000000000; ///< Mark or space (stick) parity (non-POSIX)
        inline constexpr unsigned int CRTSCTS = 020000000000; ///< RTS/CTS flow control (non-POSIX)

        inline constexpr unsigned int B57600 = 0010001; ///< Baud rate: 57600
        inline constexpr unsigned int B115200 = 0010002; ///< Baud rate: 115200
        inline constexpr unsigned int B230400 = 0010003; ///< Baud rate: 230400
        inline constexpr unsigned int B460800 = 0010004; ///< Baud rate: 460800
        inline constexpr unsigned int B500000 = 0010005; ///< Baud rate: 500000
        inline constexpr unsigned int B576000 = 0010006; ///< Baud rate: 576000
        inline constexpr unsigned int B921600 = 0010007; ///< Baud rate: 921600
        inline constexpr unsigned int B1000000 = 0010010; ///< Baud rate: 1000000
        inline constexpr unsigned int B1152000 = 0010011; ///< Baud rate: 1152000
        inline constexpr unsigned int B1500000 = 0010012; ///< Baud rate: 1500000
        inline constexpr unsigned int B2000000 = 0010013; ///< Baud rate: 2000000
        inline constexpr unsigned int B2500000 = 0010014; ///< Baud rate: 2500000
        inline constexpr unsigned int B3000000 = 0010015; ///< Baud rate: 3000000
        inline constexpr unsigned int B3500000 = 0010016; ///< Baud rate: 3500000
        inline constexpr unsigned int B4000000 = 0010017; ///< Baud rate: 4000000

        inline constexpr unsigned int MAX_BAUD = B4000000; ///< Maximum supported baud rate

        inline constexpr unsigned int CSIZE = 0000060; ///< Character size mask
        inline constexpr unsigned int CS5 = 0000000; ///< 5 bits per character
        inline constexpr unsigned int CS6 = 0000020; ///< 6 bits per character
        inline constexpr unsigned int CS7 = 0000040; ///< 7 bits per character
        inline constexpr unsigned int CS8 = 0000060; ///< 8 bits per character

        inline constexpr unsigned int CSTOPB = 0000100; ///< 2 stop bits
        inline constexpr unsigned int CREAD = 0000200; ///< Enable receiver
        inline constexpr unsigned int PARENB = 0000400; ///< Enable parity
        inline constexpr unsigned int PARODD = 0001000; ///< Odd parity
        inline constexpr unsigned int HUPCL = 0002000; ///< Hang up on last close
        inline constexpr unsigned int CLOCAL = 0004000; ///< Local line (ignore modem control)

        inline constexpr unsigned int ADDRB = 04000000000; ///< Address of device (non-POSIX)

        inline constexpr unsigned int ISIG = 0000001;   ///< Enable signals
        inline constexpr unsigned int ICANON = 0000002;  ///< Canonical input (erase and kill processing)
        inline constexpr unsigned int XCASE = 0000004;   ///< Enable case mapping (non-POSIX)
        inline constexpr unsigned int ECHO = 0000010;    ///< Enable echo
        inline constexpr unsigned int ECHOE = 0000020;   ///< Echo erase character as error-correcting backspace
        inline constexpr unsigned int ECHOK = 0000040;   ///< Echo KILL
        inline constexpr unsigned int ECHONL = 0000100;  ///< Echo NL
        inline constexpr unsigned int NOFLSH = 0000200;  ///< Disable flush after interrupt or quit
        inline constexpr unsigned int TOSTOP = 0000400;  ///< Send SIGTTOU for background output
        inline constexpr unsigned int ECHOCTL = 0001000; ///< If ECHO is also set, terminal special characters echoed as ^X
        inline constexpr unsigned int ECHOPRT = 0002000; ///< If ICANON and ECHO are set, characters printed as they are erased
        inline constexpr unsigned int ECHOKE = 0004000;  ///< If ICANON is set, KILL echoed by erasing each character on the line
        inline constexpr unsigned int FLUSHO = 0010000;  ///< Output is being flushed (toggle by DISCARD character)
        inline constexpr unsigned int PENDIN = 0040000;  ///< All characters in the input queue are reprinted when next character is read
        inline constexpr unsigned int IEXTEN = 0100000;  ///< Enable implementation-defined input processing
        inline constexpr unsigned int EXTPROC = 0200000; ///< Enable external processing (non-POSIX)

        inline constexpr unsigned int TCSANOW = 0;
        inline constexpr unsigned int TCSADRAIN = 1;
        inline constexpr unsigned int TCSAFLUSH = 2;

        /**
         * <unistd.h> constants
         */
        inline constexpr int STDIN_FILENO = 0;
        inline constexpr int STDOUT_FILENO = 1;
        inline constexpr int STDERR_FILENO = 2;

        /**
         * @namespace sys
         * @brief Wrapper namespace for Unix system operations.
         */
        namespace sys {
            /**
             * <sys/ioctl.h> constants
             */
            inline constexpr int NCC = 8;

            // TIOCM flags for modem control lines
            inline constexpr int TIOCM_LE = 0x001; ///< Loopback control
            inline constexpr int TIOCM_DTR = 0x002; ///< Data Terminal Ready
            inline constexpr int TIOCM_RTS = 0x004; ///< Request to Send
            inline constexpr int TIOCM_ST = 0x008; ///< Secondary Transmit Data
            inline constexpr int TIOCM_SR = 0x010; ///< Secondary Receive Data
            inline constexpr int TIOCM_CTS = 0x020; ///< Clear to Send
            inline constexpr int TIOCM_CAR = 0x040; ///< Carrier Detect
            inline constexpr int TIOCM_RNG = 0x080; ///< Ring Indicator
            inline constexpr int TIOCM_DSR = 0x100; ///< Data Set Ready
            inline constexpr int TIOCM_CD = TIOCM_CAR; ///< Carrier Detect (alias)
            inline constexpr int TIOCM_RI = TIOCM_RNG; ///< Ring Indicator (alias)

            // Network device types for serial connections
            inline constexpr int N_TTY = 0; ///< TTY (teletype) serial device
            inline constexpr int N_SLIP = 1; ///< Serial Line Internet Protocol (SLIP)
            inline constexpr int N_MOUSE = 2; ///< Mouse device
            inline constexpr int N_PPP = 3; ///< Point-to-Point Protocol (PPP)
            inline constexpr int N_STRIP = 4; ///< Striping (frame mode)
            inline constexpr int N_AX25 = 5; ///< AX.25 protocol (Amateur Radio)
            inline constexpr int N_X25 = 6; ///< X.25 asynchronous protocol
            inline constexpr int N_6PACK = 7; ///< 6PACK protocol
            inline constexpr int N_MASC = 8; ///< Mobitex module (wireless)
            inline constexpr int N_R3964 = 9; ///< Simatic R3964 module (industrial control)
            inline constexpr int N_PROFIBUS_FDL = 10; ///< Profibus Fieldbus Protocol
            inline constexpr int N_IRDA = 11; ///< Infrared Data Association (IR)
            inline constexpr int N_SMSBLOCK = 12; ///< SMS block mode
            inline constexpr int N_HDLC = 13; ///< Synchronous HDLC (High-Level Data Link Control)
            inline constexpr int N_SYNC_PPP = 14; ///< Synchronous PPP protocol
            inline constexpr int N_HCI = 15; ///< Bluetooth Host Controller Interface (HCI)

            inline constexpr int TCGETS = 0x5401;
            inline constexpr int TCSETS = 0x5402;
            inline constexpr int TCSETSW = 0x5403;
            inline constexpr int TCSETSF = 0x5404;
            inline constexpr int TCGETA = 0x5405;
            inline constexpr int TCSETA = 0x5406;
            inline constexpr int TCSETAW = 0x5407;
            inline constexpr int TCSETAF = 0x5408;
            inline constexpr int TCSBRK = 0x5409;
            inline constexpr int TCXONC = 0x540A;
            inline constexpr int TCFLSH = 0x540B;
            inline constexpr int TIOCEXCL = 0x540C;
            inline constexpr int TIOCNXCL = 0x540D;
            inline constexpr int TIOCSCTTY = 0x540E;
            inline constexpr int TIOCGPGRP = 0x540F;
            inline constexpr int TIOCSPGRP = 0x5410;
            inline constexpr int TIOCOUTQ = 0x5411;
            inline constexpr int TIOCSTI = 0x5412;
            inline constexpr int TIOCGWINSZ = 0x5413;
            inline constexpr int TIOCSWINSZ = 0x5414;
            inline constexpr int TIOCMGET = 0x5415;
            inline constexpr int TIOCMBIS = 0x5416;
            inline constexpr int TIOCMBIC = 0x5417;
            inline constexpr int TIOCMSET = 0x5418;
            inline constexpr int TIOCGSOFTCAR = 0x5419;
            inline constexpr int TIOCSSOFTCAR = 0x541A;
            inline constexpr int FIONREAD = 0x541B;
            inline constexpr int TIOCINQ = FIONREAD;
            inline constexpr int TIOCLINUX = 0x541C;
            inline constexpr int TIOCCONS = 0x541D;
            inline constexpr int TIOCGSERIAL = 0x541E;
            inline constexpr int TIOCSSERIAL = 0x541F;
            inline constexpr int TIOCPKT = 0x5420;
            inline constexpr int FIONBIO = 0x5421;
            inline constexpr int TIOCNOTTY = 0x5422;
            inline constexpr int TIOCSETD = 0x5423;
            inline constexpr int TIOCGETD = 0x5424;
            inline constexpr int TCSBRKP = 0x5425;
            inline constexpr int TIOCSBRK = 0x5427;
            inline constexpr int TIOCCBRK = 0x5428;
            inline constexpr int TIOCGSID = 0x5429;
            inline constexpr int TIOCGRS485 = 0x542E;
            inline constexpr int TIOCSRS485 = 0x542F;
            inline constexpr int TCGETX = 0x5432;
            inline constexpr int TCSETX = 0x5433;
            inline constexpr int TCSETXF = 0x5434;
            inline constexpr int TCSETXW = 0x5435;
            inline constexpr int TIOCSIG = 0x5436;
            inline constexpr int TIOCVHANGUP = 0x5437;
            inline constexpr int TIOCGPKT = 0x5438;
            inline constexpr int TIOCGPTLCK = 0x5439;
            inline constexpr int TIOCGEXCL = 0x5440;
            inline constexpr int TIOCGPTPEER = 0x5441;
            inline constexpr int TIOCGISO7816 = 0x5442;
            inline constexpr int TIOCSISO7816 = 0x5443;
            inline constexpr int FIONCLEX = 0x5450;
            inline constexpr int FIOCLEX = 0x5451;
            inline constexpr int FIOASYNC = 0x5452;
            inline constexpr int TIOCSERCONFIG = 0x5453;
            inline constexpr int TIOCSERGWILD = 0x5454;
            inline constexpr int TIOCSERSWILD = 0x5455;
            inline constexpr int TIOCGLCKTRMIOS = 0x5456;
            inline constexpr int TIOCSLCKTRMIOS = 0x5457;
            inline constexpr int TIOCSERGSTRUCT = 0x5458;
            inline constexpr int TIOCSERGETLSR = 0x5459;
            inline constexpr int TIOCSERGETMULTI = 0x545A;
            inline constexpr int TIOCSERSETMULTI = 0x545B;
            inline constexpr int TIOCMIWAIT = 0x545C;
            inline constexpr int TIOCGICOUNT = 0x545D;
            inline constexpr int FIOQSIZE = 0x5460;

            inline constexpr int TIOCPKT_DATA = 0;
            inline constexpr int TIOCPKT_FLUSHREAD = 1;
            inline constexpr int TIOCPKT_FLUSHWRITE = 2;
            inline constexpr int TIOCPKT_STOP = 4;
            inline constexpr int TIOCPKT_START = 8;
            inline constexpr int TIOCPKT_NOSTOP = 16;
            inline constexpr int TIOCPKT_DOSTOP = 32;
            inline constexpr int TIOCPKT_IOCTL = 64;

            /**
             * <sys/mount.h> constants
             */
            // Block size constants
            inline constexpr int BLOCK_SIZE_BITS = 10;
            inline constexpr int BLOCK_SIZE = (1 << BLOCK_SIZE_BITS);

            // Filesystem-independent mount flags
            inline constexpr unsigned long MS_RDONLY = 1; ///< Mount read-only
            inline constexpr unsigned long MS_NOSUID = 2; ///< Ignore suid and sgid bits
            inline constexpr unsigned long MS_NODEV = 4; ///< Disallow access to device special files
            inline constexpr unsigned long MS_NOEXEC = 8; ///< Disallow program execution
            inline constexpr unsigned long MS_SYNCHRONOUS = 16; ///< Writes are synced at once
            inline constexpr unsigned long MS_REMOUNT = 32; ///< Alter flags of a mounted FS
            inline constexpr unsigned long MS_MANDLOCK = 64; ///< Allow mandatory locks on an FS
            inline constexpr unsigned long MS_DIRSYNC = 128; ///< Directory modifications are synchronous
            inline constexpr unsigned long MS_NOSYMFOLLOW = 256; ///< Do not follow symlinks
            inline constexpr unsigned long MS_NOATIME = 1024; ///< Do not update access times
            inline constexpr unsigned long MS_NODIRATIME = 2048; ///< Do not update directory access times
            inline constexpr unsigned long MS_BIND = 4096; ///< Bind directory at different place
            inline constexpr unsigned long MS_MOVE = 8192;
            inline constexpr unsigned long MS_REC = 16384;
            inline constexpr unsigned long MS_SILENT = 32768;
            inline constexpr unsigned long MS_POSIXACL = 1 << 16; ///< VFS does not apply the umask
            inline constexpr unsigned long MS_UNBINDABLE = 1 << 17; ///< Change to unbindable
            inline constexpr unsigned long MS_PRIVATE = 1 << 18; ///< Change to private
            inline constexpr unsigned long MS_SLAVE = 1 << 19; ///< Change to slave
            inline constexpr unsigned long MS_SHARED = 1 << 20; ///< Change to shared
            inline constexpr unsigned long MS_RELATIME = 1 << 21; ///< Update atime relative to mtime/ctime
            inline constexpr unsigned long MS_KERNMOUNT = 1 << 22; ///< This is a kern_mount call
            inline constexpr unsigned long MS_I_VERSION = 1 << 23; ///< Update inode I_version field
            inline constexpr unsigned long MS_STRICTATIME = 1 << 24; ///< Always perform atime updates
            inline constexpr unsigned long MS_LAZYTIME = 1 << 25; ///< Update the on-disk [acm]times lazily
            inline constexpr unsigned long MS_ACTIVE = 1 << 30;
            inline constexpr unsigned long MS_NOUSER = 1U << 31;

            // Flags that can be altered by MS_REMOUNT
            inline constexpr unsigned long MS_RMT_MASK = (MS_RDONLY | MS_SYNCHRONOUS | MS_MANDLOCK | MS_I_VERSION | MS_LAZYTIME);

            // Magic mount flag number
            inline constexpr unsigned long MS_MGC_VAL = 0xc0ed0000; ///< Magic flag number to indicate "new" flags
            inline constexpr unsigned long MS_MGC_MSK = 0xffff0000; ///< Magic flag number mask

            // Block device ioctl constants
            inline constexpr int BLKROSET = 0x1000 | (0x12 << 8) | 93; ///< Set device read-only (0 = read-write)
            inline constexpr int BLKROGET = 0x1000 | (0x12 << 8) | 94; ///< Get read-only status (0 = read_write)
            inline constexpr int BLKRRPART = 0x1000 | (0x12 << 8) | 95; ///< Re-read partition table
            inline constexpr int BLKGETSIZE = 0x1000 | (0x12 << 8) | 96; ///< Return device size
            inline constexpr int BLKFLSBUF = 0x1000 | (0x12 << 8) | 97; ///< Flush buffer cache
            inline constexpr int BLKRASET = 0x1000 | (0x12 << 8) | 98; ///< Set read ahead for block device
            inline constexpr int BLKRAGET = 0x1000 | (0x12 << 8) | 99; ///< Get current read ahead setting
            inline constexpr int BLKFRASET = 0x1000 | (0x12 << 8) | 100; ///< Set filesystem read-ahead
            inline constexpr int BLKFRAGET = 0x1000 | (0x12 << 8) | 101; ///< Get filesystem read-ahead
            inline constexpr int BLKSECTSET = 0x1000 | (0x12 << 8) | 102; ///< Set max sectors per request
            inline constexpr int BLKSECTGET = 0x1000 | (0x12 << 8) | 103; ///< Get max sectors per request
            inline constexpr int BLKSSZGET = 0x1000 | (0x12 << 8) | 104; ///< Get block device sector size

            // Unmount flags
            inline constexpr int MNT_FORCE = 1; ///< Force unmounting
            inline constexpr int MNT_DETACH = 2; ///< Just detach from the tree
            inline constexpr int MNT_EXPIRE = 4; ///< Mark for expiry
            inline constexpr int UMOUNT_NOFOLLOW = 8; ///< Don't follow symlink on umount

            // fsmount flags
            inline constexpr int FSMOUNT_CLOEXEC = 0x00000001;

            // Mount attributes
            inline constexpr int MOUNT_ATTR_RDONLY = 0x00000001; ///< Mount read-only
            inline constexpr int MOUNT_ATTR_NOSUID = 0x00000002; ///< Ignore suid and sgid bits
            inline constexpr int MOUNT_ATTR_NODEV = 0x00000004; ///< Disallow access to device special files
            inline constexpr int MOUNT_ATTR_NOEXEC = 0x00000008; ///< Disallow program execution
            inline constexpr int MOUNT_ATTR__ATIME = 0x00000070; ///< Setting on how atime should be updated
            inline constexpr int MOUNT_ATTR_RELATIME = 0x00000000; ///< Update atime relative to mtime/ctime
            inline constexpr int MOUNT_ATTR_NOATIME = 0x00000010; ///< Do not update access times
            inline constexpr int MOUNT_ATTR_STRICTATIME = 0x00000020; ///< Always perform atime updates
            inline constexpr int MOUNT_ATTR_NODIRATIME = 0x00000080; ///< Do not update directory access times
            inline constexpr int MOUNT_ATTR_IDMAP = 0x00100000; ///< Idmap mount to @userns_fd in struct mount_attr
            inline constexpr int MOUNT_ATTR_NOSYMFOLLOW = 0x00200000; ///< Do not follow symlinks

            inline constexpr int MOUNT_ATTR_SIZE_VER0 = 32; ///< sizeof first published struct

            // move_mount flags
            inline constexpr int MOVE_MOUNT_F_SYMLINKS = 0x00000001; ///< Follow symlinks on from path
            inline constexpr int MOVE_MOUNT_F_AUTOMOUNTS = 0x00000002; ///< Follow automounts on from path
            inline constexpr int MOVE_MOUNT_F_EMPTY_PATH = 0x00000004; ///< Empty from path permitted
            inline constexpr int MOVE_MOUNT_T_SYMLINKS = 0x00000010; ///< Follow symlinks on to path
            inline constexpr int MOVE_MOUNT_T_AUTOMOUNTS = 0x00000020; ///< Follow automounts on to path
            inline constexpr int MOVE_MOUNT_T_EMPTY_PATH = 0x00000040; ///< Empty to path permitted
            inline constexpr int MOVE_MOUNT_SET_GROUP = 0x00000100; ///< Set sharing group instead
            inline constexpr int MOVE_MOUNT_BENEATH = 0x00000200; ///< Mount beneath top mount

            // fspick flags
            inline constexpr int FSPICK_CLOEXEC = 0x00000001;
            inline constexpr int FSPICK_SYMLINK_NOFOLLOW = 0x00000002;
            inline constexpr int FSPICK_NO_AUTOMOUNT = 0x00000004;
            inline constexpr int FSPICK_EMPTY_PATH = 0x00000008;

            // fsconfig commands
            inline constexpr int FSCONFIG_SET_FLAG = 0; ///< Set parameter, supplying no value
            inline constexpr int FSCONFIG_SET_STRING = 1; ///< Set parameter, supplying a string value
            inline constexpr int FSCONFIG_SET_BINARY = 2; ///< Set parameter, supplying a binary blob value
            inline constexpr int FSCONFIG_SET_PATH = 3; ///< Set parameter, supplying an object by path
            inline constexpr int FSCONFIG_SET_PATH_EMPTY = 4; ///< Set parameter, supplying an object by (empty) path
            inline constexpr int FSCONFIG_SET_FD = 5; ///< Set parameter, supplying an object by fd
            inline constexpr int FSCONFIG_CMD_CREATE = 6; ///< Invoke superblock creation
            inline constexpr int FSCONFIG_CMD_RECONFIGURE = 7; ///< Invoke superblock reconfiguration
            inline constexpr int FSCONFIG_CMD_CREATE_EXCL = 8; ///< Create new superblock, fail if reusing existing superblock

            // fsopen flags
            inline constexpr int FSOPEN_CLOEXEC = 0x00000001;

            // open_tree flags
            inline constexpr int OPEN_TREE_CLONE = 1; ///< Clone the target tree and attach the clone
            inline constexpr int OPEN_TREE_CLOEXEC = 0x80000; ///< Close the file on execve() (O_CLOEXEC)

            /**
             * <sys/reboot.h> constants
             */
            inline constexpr unsigned int RB_AUTOBOOT = 0x01234567; ///< Perform a hard reset now.
            inline constexpr unsigned int RB_HALT_SYSTEM = 0xCDEF0123; ///< Halt the system.
            inline constexpr unsigned int RB_ENABLE_CAD = 0x89ABCDEF; ///< Enable reboot using Ctrl-Alt-Delete keystroke.
            inline constexpr unsigned int RB_DISABLE_CAD = 0x00000000; ///< Disable reboot using Ctrl-Alt-Delete keystroke.
            inline constexpr unsigned int RB_POWER_OFF = 0x4321FEDC; ///< Stop system and switch power off if possible.
            inline constexpr unsigned int RB_SW_SUSPEND = 0xD000FCE2; ///< Suspend system using software suspend.
            inline constexpr unsigned int RB_KEXEC = 0x45584543; ///< Reboot system into new kernel.

            /**
             * <sys/ttydefaults.h> constants
             */
            inline constexpr int TTYDEF_IFLAG = (BRKINT | ISTRIP | ICRNL | IMAXBEL | IXON | IXANY);
            inline constexpr int TTYDEF_OFLAG = (OPOST | ONLCR | XTABS);
            inline constexpr int TTYDEF_LFLAG = (ECHO | ICANON | ISIG | IEXTEN | ECHOE | ECHOKE | ECHOCTL);
            inline constexpr int TTYDEF_CFLAG = (CREAD | CS7 | PARENB | HUPCL);
            inline constexpr int TTYDEF_SPEED = B9600;
            
            [[nodiscard]]
            constexpr unsigned char CTRL(char x) noexcept {
                return x & 037; 
            }
            
            inline constexpr unsigned char CEOF = CTRL('d');
            inline constexpr unsigned char CEOL = '\0';
            inline constexpr unsigned char CERASE = 0177;
            inline constexpr unsigned char CINTR = CTRL('c');
            inline constexpr unsigned char CSTATUS = '\0';
            inline constexpr unsigned char CKILL = CTRL('u');
            inline constexpr unsigned char CMIN = 1;
            inline constexpr unsigned char CQUIT = 034;
            inline constexpr unsigned char CSUSP = CTRL('z');
            inline constexpr unsigned char CTIME = 0;
            inline constexpr unsigned char CDSUSP = CTRL('y');
            inline constexpr unsigned char CSTART = CTRL('q');
            inline constexpr unsigned char CSTOP = CTRL('s');
            inline constexpr unsigned char CLNEXT = CTRL('v');
            inline constexpr unsigned char CDISCARD = CTRL('o');
            inline constexpr unsigned char CWERASE = CTRL('w');
            inline constexpr unsigned char CREPRINT = CTRL('r');
            inline constexpr unsigned char CEOT = CEOF;
            inline constexpr unsigned char CBRK = CEOL;
            inline constexpr unsigned char CRPRNT = CREPRINT;
            inline constexpr unsigned char CFLUSH = CDISCARD;
            
            inline constexpr unsigned char ttydefchars[32] = {
                CEOF, CEOL, CEOL, CERASE, CWERASE, CKILL, CREPRINT,
                '\0', CINTR, CQUIT, CSUSP, CDSUSP, CSTART, CSTOP, CLNEXT,
                CDISCARD, CMIN, CTIME, CSTATUS, '\0'
            };
        }
    }

    /**
     * @namespace win32
     * @brief Wrapper namespace for Windows operations.
     */
    namespace win32 {
        constexpr usize PATH_MAX = 260;
        constexpr usize MAX_PATH = 260;

        constexpr unsigned char MINCHAR = 0x80;
        constexpr unsigned char MAXCHAR = 0x7f;
        constexpr unsigned short MINSHORT = 0x8000;
        constexpr unsigned short MAXSHORT = 0x7fff;
        constexpr unsigned int MINLONG = 0x80000000;
        constexpr unsigned int MAXLONG = 0x7fffffff;
        constexpr unsigned char MAXBYTE = 0xff;
        constexpr unsigned short MAXWORD = 0xffff;
        constexpr unsigned long MAXDWORD = 0xffffffff;

        constexpr long ERROR_SUCCESS = 0L;
        constexpr long ERROR_FILE_NOT_FOUND = 2L;
        constexpr long ERROR_PATH_NOT_FOUND = 3L;
        constexpr long ERROR_TOO_MANY_OPEN_FILES = 4L;
        constexpr long ERROR_ACCESS_DENIED = 5L;
        constexpr long ERROR_NO_MORE_FILES = 18L;
        constexpr long ERROR_SHARING_VIOLATION = 32L;
        constexpr long ERROR_FILE_EXISTS = 80L;
        constexpr long ERROR_INSUFFICIENT_BUFFER = 122L;
        constexpr long ERROR_ALREADY_EXISTS = 183L;
        constexpr long ERROR_MORE_DATA = 234L;

        constexpr unsigned short DLL_PROCESS_ATTACH = 1;
        constexpr unsigned short DLL_PROCESS_DETACH = 0;
        constexpr unsigned short DLL_THREAD_ATTACH = 2;
        constexpr unsigned short DLL_THREAD_DETACH = 3;

        constexpr long EXCEPTION_MAXIMUM_PARAMETERS = 15;
        constexpr long EXCEPTION_EXECUTE_HANDLER = 0x1;
        constexpr long EXCEPTION_CONTINUE_EXECUTION = 0xFFFFFFFF;
        constexpr long EXCEPTION_CONTINUE_SEARCH = 0x0;

        constexpr unsigned long EXCEPTION_ACCESS_VIOLATION = 0xC0000005UL;
        constexpr unsigned long EXCEPTION_DATATYPE_MISALIGNMENT = 0x80000002UL;
        constexpr unsigned long EXCEPTION_BREAKPOINT = 0x80000003UL;
        constexpr unsigned long EXCEPTION_SINGLE_STEP = 0x80000004UL;
        constexpr unsigned long EXCEPTION_ARRAY_BOUNDS_EXCEEDED = 0xC000008CUL;
        constexpr unsigned long EXCEPTION_FLT_DENORMAL_OPERAND = 0xC000008DUL;
        constexpr unsigned long EXCEPTION_FLT_DIVIDE_BY_ZERO = 0xC000008EUL;
        constexpr unsigned long EXCEPTION_FLT_INEXACT_RESULT = 0xC000008FUL;
        constexpr unsigned long EXCEPTION_FLT_INVALID_OPERATION = 0xC0000090UL;
        constexpr unsigned long EXCEPTION_FLT_OVERFLOW = 0xC0000091UL;
        constexpr unsigned long EXCEPTION_FLT_STACK_CHECK = 0xC0000092UL;
        constexpr unsigned long EXCEPTION_FLT_UNDERFLOW = 0xC0000093UL;
        constexpr unsigned long EXCEPTION_INT_DIVIDE_BY_ZERO = 0xC0000094UL;
        constexpr unsigned long EXCEPTION_INT_OVERFLOW = 0xC0000095UL;
        constexpr unsigned long EXCEPTION_PRIV_INSTRUCTION = 0xC0000096UL;
        constexpr unsigned long EXCEPTION_IN_PAGE_ERROR = 0xC0000006UL;
        constexpr unsigned long EXCEPTION_STACK_OVERFLOW = 0xC00000FDUL;
        constexpr unsigned long EXCEPTION_ILLEGAL_INSTRUCTION = 0xC000001DUL;
        constexpr unsigned long EXCEPTION_NONCONTINUABLE_EXCEPTION = 0xC0000025UL;
        constexpr unsigned long EXCEPTION_INVALID_DISPOSITION = 0xC0000026UL;
        constexpr unsigned long EXCEPTION_GUARD_PAGE = 0x80000001UL;
        constexpr unsigned long EXCEPTION_INVALID_HANDLE = 0xC0000008UL;
        constexpr unsigned long EXCEPTION_POSSIBLE_DEADLOCK = 0xC0000194UL;
        constexpr unsigned long CONTROL_C_EXIT = 0xC000013AUL;
        
        constexpr unsigned long EXCEPTION_ASSERTION = 0xEF000001UL;

        constexpr long TH32CS_SNAPTHREAD = 0x00000004;
        constexpr long IMAGE_FILE_MACHINE_I386 = 0x014c;
        constexpr long IMAGE_FILE_MACHINE_AMD64 = 0x8664;
        constexpr long CONTEXT_AMD64 = 0x100000;
        constexpr long CONTEXT_CONTROL = (CONTEXT_AMD64 | 0x1L);
        constexpr long CONTEXT_INTEGER = (CONTEXT_AMD64 | 0x2L);
        constexpr long CONTEXT_SEGMENTS = (CONTEXT_AMD64 | 0x4L);
        constexpr long CONTEXT_FLOATING_POINT = (CONTEXT_AMD64 | 0x8L);
        constexpr long CONTEXT_DEBUG_REGISTERS = (CONTEXT_AMD64 | 0x10L);
        constexpr long CONTEXT_FULL = (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT);
        constexpr long CONTEXT_ALL = (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS);

        constexpr unsigned long SYMOPT_CASE_INSENSITIVE = 0x00000001;
        constexpr unsigned long SYMOPT_UNDNAME = 0x00000002;
        constexpr unsigned long SYMOPT_DEFERRED_LOADS = 0x00000004;
        constexpr unsigned long SYMOPT_NO_CPP = 0x00000008;
        constexpr unsigned long SYMOPT_LOAD_LINES = 0x00000010;
        constexpr unsigned long SYMOPT_OMAP_FIND_NEAREST = 0x00000020;
        constexpr unsigned long SYMOPT_LOAD_ANYTHING = 0x00000040;
        constexpr unsigned long SYMOPT_IGNORE_CVREC = 0x00000080;
        constexpr unsigned long SYMOPT_NO_UNQUALIFIED_LOADS = 0x00000100;
        constexpr unsigned long SYMOPT_FAIL_CRITICAL_ERRORS = 0x00000200;
        constexpr unsigned long SYMOPT_EXACT_SYMBOLS = 0x00000400;
        constexpr unsigned long SYMOPT_ALLOW_ABSOLUTE_SYMBOLS = 0x00000800;
        constexpr unsigned long SYMOPT_IGNORE_NT_SYMPATH = 0x00001000;
        constexpr unsigned long SYMOPT_INCLUDE_32BIT_MODULES = 0x00002000;
        constexpr unsigned long SYMOPT_PUBLICS_ONLY = 0x00004000;
        constexpr unsigned long SYMOPT_NO_PUBLICS = 0x00008000;
        constexpr unsigned long SYMOPT_AUTO_PUBLICS = 0x00010000;
        constexpr unsigned long SYMOPT_NO_IMAGE_SEARCH = 0x00020000;
        constexpr unsigned long SYMOPT_SECURE = 0x00040000;
        constexpr unsigned long SYMOPT_NO_PROMPTS = 0x00080000;
        constexpr unsigned long SYMOPT_OVERWRITE = 0x00100000;
        constexpr unsigned long SYMOPT_IGNORE_IMAGEDIR = 0x00200000;
        constexpr unsigned long SYMOPT_FLAT_DIRECTORY = 0x00400000;
        constexpr unsigned long SYMOPT_FAVOR_COMPRESSED = 0x00800000;
        constexpr unsigned long SYMOPT_ALLOW_ZERO_ADDRESS = 0x01000000;
        constexpr unsigned long SYMOPT_DISABLE_SYMSRV_AUTODETECT = 0x02000000;
        constexpr unsigned long SYMOPT_DEBUG = 0x80000000;

        constexpr int MAXIMUM_SUPPORTED_EXTENSION = 512;
    }
}   
