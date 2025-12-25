/**
 * @file chrono.cppm
 * @module std:time.chrono
 * @brief Module file for standard library time operations.
 *
 * This file contains the implementation of the time operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:time.chrono;
#else
export module stdlib:time.chrono;
#endif

import :time.chrono.detail;

/**
 * @namespace std::time
 * @brief Wrapper namespace for standard library time operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::time {
#else 
export namespace stdlib::time {
#endif
    using _detail::Duration;
    using _detail::TreatAsFloatingPoint;
    using _detail::DurationValues;
    using _detail::TimePoint;
    using _detail::LocalTime;
    using _detail::ClockTimeConversion;
    using _detail::IsClock;
    using _detail::SystemClock;
    using _detail::SteadyClock;
    using _detail::HighResolutionClock;
    using _detail::UTCClock;
    using _detail::TAIClock;
    using _detail::GPSClock;
    using _detail::FileClock;
    using _detail::Local;

    using _detail::Day;
    using _detail::Month;
    using _detail::Year;
    using _detail::Weekday;
    using _detail::MonthDay;
    using _detail::YearMonth;
    using _detail::YearMonthDay;

    using _detail::January;
    using _detail::February;
    using _detail::March;
    using _detail::April;
    using _detail::May;
    using _detail::June;
    using _detail::July;
    using _detail::August;
    using _detail::September;
    using _detail::October;
    using _detail::November;
    using _detail::December;

    using _detail::Sunday;
    using _detail::Monday;
    using _detail::Tuesday;
    using _detail::Wednesday;
    using _detail::Thursday;
    using _detail::Friday;
    using _detail::Saturday;

    using _detail::JANUARY;
    using _detail::FEBRUARY;
    using _detail::MARCH;
    using _detail::APRIL;
    using _detail::MAY;
    using _detail::JUNE;
    using _detail::JULY;
    using _detail::AUGUST;
    using _detail::SEPTEMBER;
    using _detail::OCTOBER;
    using _detail::NOVEMBER;
    using _detail::DECEMBER;
    
    using _detail::SUNDAY;
    using _detail::MONDAY;
    using _detail::TUESDAY;
    using _detail::WEDNESDAY;
    using _detail::THURSDAY;
    using _detail::FRIDAY;
    using _detail::SATURDAY;

    using _detail::TimeZoneDatabase;
    using _detail::TimeZoneDatabaseList;
    using _detail::TimeZone;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using _detail::SysInfo;
    #endif
    using _detail::SystemInfo;
    using _detail::LocalInfo;

    using _detail::LeapSecond;
    using _detail::LeapSecondInfo;

    using _detail::AmbiguousLocalTimeException;
    using _detail::NonexistentLocalTimeException;

    using _detail::duration_cast;
    using _detail::time_point_cast;
    using _detail::floor;

    using _detail::get_tzdb;
    using _detail::get_tzdb_list;
    using _detail::reload_tzdb;
    using _detail::remote_version;
    using _detail::locate_zone;
    using _detail::current_zone;

    using _detail::get_leap_second_info;

    using _detail::hash;
    using _detail::formatter;

    /**
     * @namespace chrono
     * @brief Wrapper namespace for standard library chronological operations
     */
    namespace chrono {
        using _detail::LastSpecifier;
        using _detail::Last;
        using _detail::WeekdayIndexed;
        using _detail::WeekdayLast;
        using _detail::MonthDayLast;
        using _detail::MonthWeekday;
        using _detail::MonthWeekdayLast;
        using _detail::YearMonthDayLast;
        using _detail::YearMonthWeekday;
        using _detail::YearMonthWeekdayLast;
        
        using _detail::Choose;
    }

    /**
     * @namespace fmt
     * @brief Wrapper namespace for standard library time formatting operations
     */
    namespace fmt {
        using _detail::is_am;
        using _detail::is_pm;
        using _detail::make12;
        using _detail::make24;

        using _detail::HHMMSS;

        using _detail::from_stream;
        using _detail::parse;
    }

    /**
     * @namespace temporal
     * @brief Wrapper namespace for standard library time temporal unit operations
     */
    namespace temporal {
        using _detail::Nanoseconds;
        using _detail::Microseconds;
        using _detail::Milliseconds;
        using _detail::Seconds;
        using _detail::Minutes;
        using _detail::Hours;
        using _detail::Days;
        using _detail::Weeks;
        using _detail::Months;
        using _detail::Years;
    }

    /**
     * @namespace zone
     * @brief Wrapper namespace for standard library time zone operations
     */
    namespace zone {
        using _detail::ZonedTraits;
        using _detail::ZonedTime;
        using _detail::TimeZoneLink;
    }
}

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::inline literals::inline chrono_literals  {
#else 
export namespace stdlib::inline literals::inline chrono_literals  {
#endif
    using _detail::operator""d;
    using _detail::operator""h;
    using _detail::operator""min;
    using _detail::operator""ms;
    using _detail::operator""s;
    using _detail::operator""us;
    using _detail::operator""y; 
}
