#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief Mach semaphores - <mach/semaphore.h>.
 *
 * These are the primitive dispatch_semaphore and pthread condition variables
 * are built on, and the only ones safe to signal from a real-time thread.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/semaphore.h>)
    using ::semaphore_create;
    using ::semaphore_destroy;
    using ::semaphore_signal;
    using ::semaphore_signal_all;
    using ::semaphore_signal_thread;
    using ::semaphore_wait;
    using ::semaphore_timedwait;
    using ::semaphore_timedwait_signal;
    using ::semaphore_wait_signal;
    #endif
}
