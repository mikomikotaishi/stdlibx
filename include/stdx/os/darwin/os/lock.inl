#pragma once

/**
 * @namespace stdx::os::darwin::os
 * @brief os_unfair_lock, the supported replacement for the deprecated OSSpinLock
 *        - <os/lock.h>.
 *
 * Unlike a spin lock this hands priority to the lock owner, so it does not
 * invert priority when a real-time thread contends with a background one.
 */
export namespace stdx::os::darwin::os {
    #if defined(__APPLE__) && __has_include(<os/lock.h>)
    using UnfairLock = ::os_unfair_lock;
    using UnfairLockPointer = ::os_unfair_lock_t;

    using ::os_unfair_lock_lock;
    using ::os_unfair_lock_trylock;
    using ::os_unfair_lock_unlock;
    using ::os_unfair_lock_lock_with_flags;
    using ::os_unfair_lock_assert_owner;
    using ::os_unfair_lock_assert_not_owner;
    #endif
}
