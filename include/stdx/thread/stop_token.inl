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
}
