#pragma once

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
