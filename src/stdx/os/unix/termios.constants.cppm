/**
 * @file termios.constants.cppm
 * @module stdx:os.unix.termios:constants
 * @brief Module file for Unix terminal I/O operations.
 *
 * This file contains the implementation of the constants used 
 * in <termios.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.termios.constants;
#else
export module stdlibx:os.unix.termios.constants;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
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
}
