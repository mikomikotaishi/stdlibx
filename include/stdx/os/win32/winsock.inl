#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Winsock2 + WS2_32 + IP6 type and function wrappers.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<winsock2.h>)
    // Core socket types
    using WinSocket = ::SOCKET;
    using WinSocketPointer = ::LPSOCKET;
    using WinFdSet = ::fd_set;
    using WinTimeval = ::TIMEVAL;
    using WinTimevalPointer = ::PTIMEVAL;

    // Address structures
    using WinSockAddr = ::SOCKADDR;
    using WinSockAddrPointer = ::PSOCKADDR;
    using WinSockAddrIn = ::SOCKADDR_IN;
    using WinSockAddrInPointer = ::PSOCKADDR_IN;
    using WinSockAddrIn6 = ::SOCKADDR_IN6;
    using WinSockAddrIn6Pointer = ::PSOCKADDR_IN6;
    using WinSockAddrStorage = ::SOCKADDR_STORAGE;
    using WinSockAddrStoragePointer = ::PSOCKADDR_STORAGE;
    using WinInAddr = ::IN_ADDR;
    using WinInAddrPointer = ::PIN_ADDR;
    using WinIn6Addr = ::IN6_ADDR;
    using WinIn6AddrPointer = ::PIN6_ADDR;

    // Lookup records
    using WinHostEnt = ::HOSTENT;
    using WinHostEntPointer = ::PHOSTENT;
    using WinServEnt = ::SERVENT;
    using WinServEntPointer = ::PSERVENT;
    using WinProtoEnt = ::PROTOENT;
    using WinProtoEntPointer = ::PPROTOENT;
    using WinAddrInfo = ::ADDRINFOA;
    using WinAddrInfoPointer = ::PADDRINFOA;
    using WinAddrInfoW = ::ADDRINFOW;
    using WinAddrInfoWPointer = ::PADDRINFOW;
    using WinSocketLength = ::socklen_t;

    // Winsock-level types
    using WsaData = ::WSADATA;
    using WsaDataPointer = ::LPWSADATA;
    using WsaBuf = ::WSABUF;
    using WsaBufPointer = ::LPWSABUF;
    using WsaEvent = ::WSAEVENT;
    using WsaOverlapped = ::WSAOVERLAPPED;
    using WsaOverlappedPointer = ::LPWSAOVERLAPPED;
    using WsaOverlappedCompletionRoutine = ::LPWSAOVERLAPPED_COMPLETION_ROUTINE;
    using WsaProtocolInfo = ::WSAPROTOCOL_INFOA;
    using WsaProtocolInfoW = ::WSAPROTOCOL_INFOW;
    using WsaNetworkEvents = ::WSANETWORKEVENTS;
    using WsaCompletion = ::WSACOMPLETION;
    using Group = ::GROUP;

    // Core BSD-style socket API
    using ::socket;
    using ::accept;
    using ::bind;
    using ::listen;
    using ::connect;
    using ::shutdown;
    using ::closesocket;
    using ::send;
    using ::sendto;
    using ::recv;
    using ::recvfrom;
    using ::select;
    using ::getpeername;
    using ::getsockname;
    using ::getsockopt;
    using ::setsockopt;
    using ::ioctlsocket;

    // Address conversion
    using ::htons;
    using ::htonl;
    using ::ntohs;
    using ::ntohl;
    using ::inet_addr;
    using ::inet_ntoa;
    using ::inet_ntop;
    using ::inet_pton;

    // Name resolution
    using ::gethostname;
    using ::gethostbyname;
    using ::gethostbyaddr;
    using ::getservbyname;
    using ::getservbyport;
    using ::getprotobyname;
    using ::getprotobynumber;
    using ::getaddrinfo;
    using ::freeaddrinfo;
    using ::getnameinfo;

    // Winsock lifecycle and events
    using ::WSAStartup;
    using ::WSACleanup;
    using ::WSAGetLastError;
    using ::WSASetLastError;
    using ::WSACreateEvent;
    using ::WSACloseEvent;
    using ::WSAResetEvent;
    using ::WSASetEvent;
    using ::WSAWaitForMultipleEvents;
    using ::WSAEventSelect;
    using ::WSAEnumNetworkEvents;
    using ::WSASend;
    using ::WSASendTo;
    using ::WSARecv;
    using ::WSARecvFrom;
    using ::WSAConnect;
    using ::WSAAccept;
    using ::WSAIoctl;
    using ::WSASocketA;
    using ::WSASocketW;
    using ::WSAPoll;
    #endif
}
