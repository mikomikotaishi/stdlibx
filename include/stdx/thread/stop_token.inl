#pragma once

/**
 * @namespace stdx::thread
 * @brief Standard library threading operations.
 */
export namespace stdx::thread {
    using StopToken = std::stop_token;
    using StopSource = std::stop_source;

    template <typename Fn>
    using StopCallback = std::stop_callback<Fn>;

    #if defined(__cpp_lib_senders) && !defined(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    using NeverStopToken = std::never_stop_token;
    using InPlaceStopToken = std::inplace_stop_token;
    using InPlaceStopSource = std::inplace_stop_source;

    template <typename Fn>
    using InPlaceStopCallback = std::inplace_stop_callback<Fn>;

    template <typename T, typename Fn>
    using StopCallbackFor = std::stop_callback_for_t<T, Fn>;

    template <typename Tok>
    concept StoppableToken = std::stoppable_token<Tok>;

    template <typename Tok>
    concept UnstoppableToken = std::unstoppable_token<Tok>;
    #endif
}
