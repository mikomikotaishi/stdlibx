#pragma once

/**
 * @namespace stdx::thread
 * @brief Wrapper namespace for standard library threading operations.
 */
export namespace stdx::thread {
    using Thread = std::thread;
    using ThreadId = std::thread::id;

    using JoiningThread = std::jthread;

    /**
     * @brief Namespace for operations on the current thread.
     */
    namespace this_thread {
        using std::this_thread::yield;
        using std::this_thread::get_id;
        using std::this_thread::sleep_for;
        using std::this_thread::sleep_until;
    }

    using std::formatter;
    using std::swap;
}
