#pragma once

/**
 * @namespace stdx::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace stdx::thread {
    using StopToken = std::stop_token;
    using StopSource = std::stop_source;

    template <typename Fn>
    using StopCallback = std::stop_callback<Fn>;

    using NoStopStateTag = std::nostopstate_t;
    inline constexpr NoStopStateTag NoStopState = std::nostopstate;

    using std::swap;
}
