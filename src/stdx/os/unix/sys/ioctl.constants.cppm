/**
 * @file ioctl.constants.cppm
 * @module stdx:os.unix.sys.ioctl.constants
 * @brief Module file for I/O control operations.
 *
 * This file contains the implementation of the constants used 
 * in <sys/ioctl.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.ioctl.constants;
#else
export module stdlibx:os.unix.sys.ioctl.constants;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
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

    inline constexpr int TIOCSER_TEMT = 0x01;
}
