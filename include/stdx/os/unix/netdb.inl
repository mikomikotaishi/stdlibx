#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::captured
 * @brief The numeric values of the <netdb.h> macros, read where they exist.
 *
 * Worth deriving even though every name here exists on both platforms, because
 * several of the values do not agree and two of the disagreements are silent:
 *
 *   Linux AI_ADDRCONFIG  0x0020    Darwin AI_ADDRCONFIG  0x0400
 *   Linux AI_NUMERICSERV 0x0400    Darwin AI_NUMERICSERV 0x1000
 *
 * Linux's AI_NUMERICSERV is Darwin's AI_ADDRCONFIG, so a copied literal asks for
 * a real but different flag rather than failing. The EAI_* codes also change
 * sign - EAI_SERVICE is -8 on Linux and 9 on Darwin - so a resolver comparing
 * against the wrong set simply never matches.
 */
namespace stdx::os::unix::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netdb.h>)
    constexpr auto AI_PASSIVE_VALUE = AI_PASSIVE;
    constexpr auto AI_CANONNAME_VALUE = AI_CANONNAME;
    constexpr auto AI_NUMERICHOST_VALUE = AI_NUMERICHOST;
    constexpr auto AI_V4MAPPED_VALUE = AI_V4MAPPED;
    constexpr auto AI_ALL_VALUE = AI_ALL;
    constexpr auto AI_ADDRCONFIG_VALUE = AI_ADDRCONFIG;
    constexpr auto AI_NUMERICSERV_VALUE = AI_NUMERICSERV;
    constexpr auto NI_NUMERICHOST_VALUE = NI_NUMERICHOST;
    constexpr auto NI_NUMERICSERV_VALUE = NI_NUMERICSERV;
    constexpr auto NI_NOFQDN_VALUE = NI_NOFQDN;
    constexpr auto NI_NAMEREQD_VALUE = NI_NAMEREQD;
    constexpr auto NI_DGRAM_VALUE = NI_DGRAM;
    constexpr auto NI_MAXHOST_VALUE = NI_MAXHOST;
    constexpr auto NI_MAXSERV_VALUE = NI_MAXSERV;
    constexpr auto EAI_BADFLAGS_VALUE = EAI_BADFLAGS;
    constexpr auto EAI_NONAME_VALUE = EAI_NONAME;
    constexpr auto EAI_AGAIN_VALUE = EAI_AGAIN;
    constexpr auto EAI_FAIL_VALUE = EAI_FAIL;
    constexpr auto EAI_NODATA_VALUE = EAI_NODATA;
    constexpr auto EAI_FAMILY_VALUE = EAI_FAMILY;
    constexpr auto EAI_SOCKTYPE_VALUE = EAI_SOCKTYPE;
    constexpr auto EAI_SERVICE_VALUE = EAI_SERVICE;
    constexpr auto EAI_MEMORY_VALUE = EAI_MEMORY;
    constexpr auto EAI_SYSTEM_VALUE = EAI_SYSTEM;
    constexpr auto EAI_OVERFLOW_VALUE = EAI_OVERFLOW;
    #endif
}

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netdb.h>)
    using ::herror;
    using ::hstrerror;

    using HostEntry = ::hostent;

    using AddressInfo = ::addrinfo;

    using ::getaddrinfo;
    using ::freeaddrinfo;
    using ::getnameinfo;
    using ::gai_strerror;

    using ::sethostent;
    using ::endhostent;
    using ::gethostent;
    using ::gethostbyaddr;
    using ::gethostbyname;
    using ::gethostbyname2;

    /**
     * @brief glibc's reentrant host-lookup variants.
     *
     * GNU extensions rather than POSIX. Darwin makes the non-reentrant forms
     * thread-safe instead, so it never added these. Nothing in stdx::net uses
     * them - Resolver goes through getaddrinfo, which is reentrant everywhere.
     */
    #ifdef __linux__
    using ::gethostent_r;
    using ::gethostbyaddr_r;
    using ::gethostbyname_r;
    using ::gethostbyname2_r;
    #endif
    #endif
}
