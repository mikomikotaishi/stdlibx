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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.termios;
#else
export module stdlibx:os.unix.termios;
#endif

export import :os.unix.termios.constants;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using TermIOSettings = ::termios;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using PID_t = ::pid_t;
    using CC_t = ::cc_t;
    using Speed_t = ::speed_t;
    using TCFlag_t = ::tcflag_t;
    #endif

    using ProcessID = ::pid_t;
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
