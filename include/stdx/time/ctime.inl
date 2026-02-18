#pragma once

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
