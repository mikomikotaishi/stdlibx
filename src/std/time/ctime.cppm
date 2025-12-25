/**
 * @file ctime.cppm
 * @module std:time.ctime
 * @brief Module file for standard library C-style time operations.
 *
 * This file contains the implementation of the C-style time operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
#define time _get_time
#include <time.h>
#undef time
#else 
#include <ctime>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:time.ctime;
#else
export module stdlib:time.ctime;
#endif

#ifndef STDLIBX_NO_RESERVED_STD_MODULE
/**
 * @namespace std::time
 * @brief Wrapper namespace for standard library time operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::time {
#else 
export namespace stdlib::time {
#endif
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
    using Time = ::tm;
    using Time_t = ::time_t;
    using Clock_t = ::clock_t;
    using TimeSpecification = ::timespec;

    using ::clock;
    using ::difftime;
    #else
    using Time = std::tm;
    using Time_t = std::time_t;
    using Clock_t = std::clock_t;
    using TimeSpecification = std::timespec;

    using std::clock;
    using std::time;
    using std::difftime;
    #endif
    
    inline Time_t get_time(Time_t* timer) {
        #if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
        return ::_get_time(timer);
        #else
        return std::time(timer);
        #endif
    }

    /**
     * @brief fmt
     * @brief Wrapper namespace for standard library time formatting operations
     */
    namespace fmt {
        #if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
        using ::timespec_get;
        using ::ctime;
        using ::asctime;
        using ::strftime;
        using ::gmtime;
        using ::localtime;
        using ::mktime;
        #else
        using std::timespec_get;
        using std::ctime;
        using std::asctime;
        using std::strftime;
        using std::gmtime;
        using std::localtime;
        using std::mktime;
        #endif
    }
}
#endif
