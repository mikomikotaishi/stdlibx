/**
 * @file stop_token.cppm
 * @module stdx:thread.stop_token
 * @brief Module file for standard library stop token operations.
 *
 * This file contains the implementation of the stop token operations in the standard library.
 */

module;

#if __has_include(<stop_token>)
#include <stop_token>
#endif

export module stdx:thread.stop_token;

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
