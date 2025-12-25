/**
 * @file thread.cppm
 * @module std:concurrent.thread
 * @brief Module file for standard library thread operations.
 *
 * This file contains the implementation of the thread operations in the standard library.
 */

module;

#include <thread>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:concurrent.thread;
#else
export module stdlib:concurrent.thread;
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
    using Thread = ::std::thread;
    using ThreadId = ::std::thread::id;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using JThread = ::std::jthread;
    #endif

    using JoiningThread = ::std::jthread;

    /**
     * @brief Namespace for operations on the current thread.
     */
    namespace this_thread {
        using ::std::this_thread::yield;
        using ::std::this_thread::get_id;
        using ::std::this_thread::sleep_for;
        using ::std::this_thread::sleep_until;
    }

    using ::std::formatter;
    using ::std::swap;
}
