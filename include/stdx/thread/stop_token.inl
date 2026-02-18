#pragma once

/**
 * @namespace stdx::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace stdx::thread {
    #if __has_include(<stop_token>)
    using StopToken = std::stop_token;
    using StopSource = std::stop_source;

    template <typename Callback>
    using StopCallback = std::stop_callback<Callback>;

    using NoStopStateTag = std::nostopstate_t;
    inline constexpr NoStopStateTag NoStopState = std::nostopstate;

    using std::swap;
    #endif
}
