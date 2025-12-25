/**
 * @file pthread.cppm
 * @module stdx:os.unix.pthread
 * @brief Module file for Unix POSIX thread manipulation operations.
 *
 * This file contains the implementation of the Unix POSIX thread manipulation operations from the POSIX libraries,
 * located in <pthread.h>.
 */

module;

#ifdef __unix__
#include <pthread.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.pthread;
#else
export module stdlibx:os.unix.pthread;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::pthread_create;
    using ::pthread_exit;
    using ::pthread_join;
    using ::pthread_tryjoin_np;
    using ::pthread_timedjoin_np;
    using ::pthread_clockjoin_np;
    using ::pthread_detach;
    using ::pthread_self;

    // Thread attribute handling
    using ::pthread_attr_init;
    using ::pthread_attr_destroy;
    using ::pthread_attr_getdetachstate;
    using ::pthread_attr_setdetachstate;
    using ::pthread_attr_getguardsize;
    using ::pthread_attr_setguardsize;
    using ::pthread_attr_getschedparam;
    using ::pthread_attr_setschedparam;
    using ::pthread_attr_getschedpolicy;
    using ::pthread_attr_setschedpolicy;
    using ::pthread_attr_getinheritsched;
    using ::pthread_attr_setinheritsched;
    using ::pthread_attr_getscope;
    using ::pthread_attr_setscope;
    using ::pthread_attr_getstackaddr;
    using ::pthread_attr_setstackaddr;
    using ::pthread_attr_getstacksize;
    using ::pthread_attr_setstacksize;
    using ::pthread_attr_getstack;
    using ::pthread_attr_setstack;
    using ::pthread_attr_setaffinity_np;
    using ::pthread_attr_getaffinity_np;
    using ::pthread_getattr_default_np;
    using ::pthread_attr_setsigmask_np;
    using ::pthread_attr_getsigmask_np;
    using ::pthread_setattr_default_np;
    using ::pthread_getattr_np;

    // Functions for scheduling control
    using ::pthread_setschedparam;
    using ::pthread_getschedparam;
    using ::pthread_setschedprio;
    using ::pthread_getname_np;
    using ::pthread_setname_np;
    using ::pthread_getconcurrency;
    using ::pthread_setconcurrency;
    using ::pthread_yield;
    using ::pthread_setaffinity_np;
    using ::pthread_getaffinity_np;
    using ::pthread_once;

    // Functions for handling initialisation
    using ::pthread_setcancelstate;

    // Functions for handling cancellation
    using ::pthread_setcanceltype;
    using ::pthread_cancel;
    using ::pthread_testcancel;

    // Mutex handling
    using ::pthread_mutex_init;
    using ::pthread_mutex_destroy;
    using ::pthread_mutex_trylock;
    using ::pthread_mutex_lock;
    using ::pthread_mutex_timedlock;
    using ::pthread_mutex_clocklock;
    using ::pthread_mutex_unlock;
    using ::pthread_mutex_getprioceiling;
    using ::pthread_mutex_setprioceiling;
    using ::pthread_mutex_consistent;

    // Functions for handling mutex attributes
    using ::pthread_mutexattr_init;
    using ::pthread_mutexattr_destroy;
    using ::pthread_mutexattr_getpshared;
    using ::pthread_mutexattr_setpshared;
    using ::pthread_mutexattr_getprotocol;
    using ::pthread_mutexattr_setprotocol;
    using ::pthread_mutexattr_getprioceiling;
    using ::pthread_mutexattr_setprioceiling;
    using ::pthread_mutexattr_getrobust;
    using ::pthread_mutexattr_setrobust;

    // Functions for handling read-write locks
    using ::pthread_rwlock_init;
    using ::pthread_rwlock_destroy;
    using ::pthread_rwlock_rdlock;
    using ::pthread_rwlock_tryrdlock;
    using ::pthread_rwlock_timedrdlock;
    using ::pthread_rwlock_clockrdlock;
    using ::pthread_rwlock_wrlock;
    using ::pthread_rwlock_trywrlock;
    using ::pthread_rwlock_timedwrlock;
    using ::pthread_rwlock_clockwrlock;
    using ::pthread_rwlock_unlock;

    // Functions for handling read-write lock attributes
    using ::pthread_rwlockattr_init;
    using ::pthread_rwlockattr_destroy;
    using ::pthread_rwlockattr_getpshared;
    using ::pthread_rwlockattr_setpshared;
    using ::pthread_rwlockattr_getkind_np;
    using ::pthread_rwlockattr_setkind_np;

    // Functions for handling conditional variables
    using ::pthread_cond_init;
    using ::pthread_cond_destroy;
    using ::pthread_cond_signal;
    using ::pthread_cond_broadcast;
    using ::pthread_cond_wait;
    using ::pthread_cond_timedwait;
    using ::pthread_cond_clockwait;

    // Functions for handling condition variable attributes
    using ::pthread_condattr_init;
    using ::pthread_condattr_destroy;
    using ::pthread_condattr_getpshared;
    using ::pthread_condattr_setpshared;
    using ::pthread_condattr_getclock;
    using ::pthread_condattr_setclock;

    // Functions to handle spinlocks
    using ::pthread_spin_init;
    using ::pthread_spin_destroy;
    using ::pthread_spin_lock;
    using ::pthread_spin_trylock;
    using ::pthread_spin_unlock;

    // Functions to handle barriers
    using ::pthread_barrier_init;
    using ::pthread_barrier_destroy;
    using ::pthread_barrier_wait;
    using ::pthread_barrierattr_init;
    using ::pthread_barrierattr_destroy;
    using ::pthread_barrierattr_getpshared;
    using ::pthread_barrierattr_setpshared;

    // Functions for handling thread-specific data
    using ::pthread_key_create;
    using ::pthread_key_delete;
    using ::pthread_getspecific;
    using ::pthread_getcpuclockid;
    using ::pthread_atfork;
    #endif
}
