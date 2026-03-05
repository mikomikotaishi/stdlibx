#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using TerminalIOSettings = ::termios;

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
