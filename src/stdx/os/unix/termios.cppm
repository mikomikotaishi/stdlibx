/**
 * @file termios.cppm
 * @module stdx:os.unix.termios
 * @brief Module file for Unix terminal I/O operations.
 *
 * This file contains the implementation of the Unix terminal I/O operations from the POSIX libraries,
 * located in <termios.h>.
 */

module;

#ifdef __unix__
#include <termios.h>
#endif

export module stdx:os.unix.termios;

export import :os.unix.termios.constants;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using TermIOSettings = ::termios;

    using ProcessId = ::pid_t;
    using ControlCharacter = ::cc_t;
    using Speed = ::speed_t;
    using TerminalControlFlag = ::tcflag_t;

    using ::cfgetospeed;
    using ::cfgetispeed;
    using ::cfsetospeed;
    using ::cfsetispeed;
    using ::cfsetspeed;
    using ::tcgetattr;
    using ::tcsetattr;
    using ::cfmakeraw;
    using ::tcsendbreak;
    using ::tcdrain;
    using ::tcflush;
    using ::tcflow;
    using ::tcgetsid;
    #endif
}
