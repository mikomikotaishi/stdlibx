/**
 * @file timer.cppm
 * @module stdx:net.timer
 * @brief Module file for (experimental) standard library networking timer operations.
 *
 * This file contains the implementation of the (experimental) networking timer operations in the standard library.
 */


module;

#if __has_include(<experimental/timer>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/timer>
#endif

export module stdx:net.timer;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/timer>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
    template <typename Clock>
    using WaitTraits = std::experimental::net::wait_traits<Clock>;

    template <typename Clock, typename WaitTraits>
    using BasicWaitableTimer = std::experimental::net::basic_waitable_timer<Clock, WaitTraits>;

    using SystemTimer = std::experimental::net::system_timer;
    using SteadyTimer = std::experimental::net::steady_timer;
    using HighResolutionTimer = std::experimental::net::high_resolution_timer;
    #endif
}
