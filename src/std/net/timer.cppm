/**
 * @file timer.cppm
 * @module std:net.timer
 * @brief Module file for (experimental) standard library networking timer operations.
 *
 * This file contains the implementation of the (experimental) networking timer operations in the standard library.
 */


module;

#if __has_include(<experimental/timer>)
#include <experimental/timer>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:net.timer;
#else
export module stdlib:net.timer;
#endif

/**
 * @namespace std::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::net {
#else 
export namespace stdlib::net {
#endif
    #if __has_include(<experimental/timer>)
    template <typename Clock>
    using WaitTraits = std::experimental::net::wait_traits<Clock>;

    template <typename Clock, typename _WaitTraits>
    using BasicWaitableTimer = std::experimental::net::basic_waitable_timer<Clock, _WaitTraits>;

    using SystemTimer = std::experimental::net::system_timer;
    using SteadyTimer = std::experimental::net::steady_timer;
    using HighResolutionTimer = std::experimental::net::high_resolution_timer;
    #endif
}
