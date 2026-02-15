/**
 * @file ctime.cppm
 * @module stdx:time.ctime
 * @brief Module file for standard library C-style time operations.
 *
 * This file contains the implementation of the C-style time operations in the standard library.
 */

module;

#include <ctime>

export module stdx:time.ctime;

/**
 * @namespace stdx::time
 * @brief Wrapper namespace for standard library time operations.
 */
export namespace stdx::time {
    using Time = std::tm;
    using TimeType = std::time_t;
    using ClockType = std::clock_t;
    using TimeSpecification = std::timespec;

    using std::clock;
    using std::time;
    using std::difftime;

    /**
     * @brief fmt
     * @brief Wrapper namespace for standard library time formatting operations
     */
    namespace fmt {
        using std::timespec_get;
        using std::ctime;
        using std::asctime;
        using std::strftime;
        using std::gmtime;
        using std::localtime;
        using std::mktime;
    }
}
