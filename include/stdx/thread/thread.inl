#pragma once

/**
 * @namespace stdx::thread
 * @brief Standard library threading operations.
 */
export namespace stdx::thread {
    using ManualThread = std::thread;
    using Thread = std::jthread;
    using ThreadId = std::thread::id;
}
