/**
 * @file stop_token.cppm
 * @module std:concurrent.stop_token
 * @brief Module file for standard library stop token operations.
 *
 * This file contains the implementation of the stop token operations in the standard library.
 */

module;

#if __has_include(<stop_token>)
#include <stop_token>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:concurrent.stop_token;
#else
export module stdlib:concurrent.stop_token;
#endif

/**
 * @namespace std::concurrent
 * @brief Wrapper namespace for standard library concurrency operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::concurrent {
#else 
export namespace stdlib::concurrent {
#endif
    #if __has_include(<stop_token>)
    using StopToken = ::std::stop_token;
    using StopSource = ::std::stop_source;

    template <typename Callback>
    using StopCallback = ::std::stop_callback<Callback>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using NoStopState_t = ::std::nostopstate_t;
    #endif

    using NoStopStateTag = ::std::nostopstate_t;
    inline constexpr NoStopStateTag NoStopState = ::std::nostopstate;

    using ::std::swap;
    #endif
}
