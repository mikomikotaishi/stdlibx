/**
 * @file ttydefaults.cppm
 * @module stdx:os.unix.sys.ttydefaults
 * @brief Module file for ttydefaults constants.
 *
 * This file contains the implementation of constants,
 * located in <sys/ttydefaults.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.ttydefaults;
#else
export module stdlibx:os.unix.sys.ttydefaults;
#endif

import :os.unix.termios;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    inline constexpr int TTYDEF_IFLAG = (BRKINT | ISTRIP | ICRNL | IMAXBEL | IXON | IXANY);
    inline constexpr int TTYDEF_OFLAG = (OPOST | ONLCR | XTABS);
    inline constexpr int TTYDEF_LFLAG = (ECHO | ICANON | ISIG | IEXTEN | ECHOE | ECHOKE | ECHOCTL);
    inline constexpr int TTYDEF_CFLAG = (CREAD | CS7 | PARENB | HUPCL);
    inline constexpr int TTYDEF_SPEED = B9600;
    
    inline constexpr unsigned char CTRL(char x) { 
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
