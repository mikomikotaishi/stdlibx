/**
 * @file chrono.detail.cppm
 * @module stdx:time.chrono.detail
 * @brief Module file for standard library time operations.
 *
 * This file contains the implementation of the time operations in the standard library.
 */

module;

#include <chrono>

export module stdx:time.chrono.detail;

import :math.ratio;

using stdx::math::ratio::Ratio;

/**
 * @internal
 * @namespace _detail
 * @brief Wrapper namespace for standard library time operations.
 * Detail namespace - not to be exported for external use
 */
export namespace _detail {
    template <typename Rep, typename Period = Ratio<1>>
    using Duration = ::std::chrono::duration<Rep, Period>;

    template <typename Rep>
    using TreatAsFloatingPoint = ::std::chrono::treat_as_floating_point<Rep>;

    template <typename Rep>
    using DurationValues = ::std::chrono::duration_values<Rep>;

    template <typename Clock, typename Duration = typename Clock::duration>
    using TimePoint = ::std::chrono::time_point<Clock, Duration>;

    template <typename Duration>
    using LocalTime = ::std::chrono::local_time<Duration>;

    template <typename Dest, typename Source>
    using ClockTimeConversion = ::std::chrono::clock_time_conversion<Dest, Source>;

    template <typename T>
    using IsClock = ::std::chrono::is_clock<T>;

    using SystemClock = ::std::chrono::system_clock;
    using SteadyClock = ::std::chrono::steady_clock;
    using HighResolutionClock = ::std::chrono::high_resolution_clock;
    using UTCClock = ::std::chrono::utc_clock;
    using TAIClock = ::std::chrono::tai_clock;
    using GPSClock = ::std::chrono::gps_clock;
    using FileClock = ::std::chrono::file_clock;
    using Local = ::std::chrono::local_t;

    using Day = ::std::chrono::day;
    using Month = ::std::chrono::month;
    using Year = ::std::chrono::year;
    using Weekday = ::std::chrono::weekday;
    using MonthDay = ::std::chrono::month_day;
    using YearMonth = ::std::chrono::year_month;
    using YearMonthDay = ::std::chrono::year_month_day;

    using ::std::chrono::January;
    using ::std::chrono::February;
    using ::std::chrono::March;
    using ::std::chrono::April;
    using ::std::chrono::May;
    using ::std::chrono::June;
    using ::std::chrono::July;
    using ::std::chrono::August;
    using ::std::chrono::September;
    using ::std::chrono::October;
    using ::std::chrono::November;
    using ::std::chrono::December;

    using ::std::chrono::Sunday;
    using ::std::chrono::Monday;
    using ::std::chrono::Tuesday;
    using ::std::chrono::Wednesday;
    using ::std::chrono::Thursday;
    using ::std::chrono::Friday;
    using ::std::chrono::Saturday;

    inline constexpr Month JANUARY = ::std::chrono::January;
    inline constexpr Month FEBRUARY = ::std::chrono::February;
    inline constexpr Month MARCH = ::std::chrono::March;
    inline constexpr Month APRIL = ::std::chrono::April;
    inline constexpr Month MAY = ::std::chrono::May;
    inline constexpr Month JUNE = ::std::chrono::June;
    inline constexpr Month JULY = ::std::chrono::July;
    inline constexpr Month AUGUST = ::std::chrono::August;
    inline constexpr Month SEPTEMBER = ::std::chrono::September;
    inline constexpr Month OCTOBER = ::std::chrono::October;
    inline constexpr Month NOVEMBER = ::std::chrono::November;
    inline constexpr Month DECEMBER = ::std::chrono::December;

    inline constexpr Weekday SUNDAY = ::std::chrono::Sunday;
    inline constexpr Weekday MONDAY = ::std::chrono::Monday;
    inline constexpr Weekday TUESDAY = ::std::chrono::Tuesday;
    inline constexpr Weekday WEDNESDAY = ::std::chrono::Wednesday;
    inline constexpr Weekday THURSDAY = ::std::chrono::Thursday;
    inline constexpr Weekday FRIDAY = ::std::chrono::Friday;
    inline constexpr Weekday SATURDAY = ::std::chrono::Saturday;

    using TimeZoneDatabase = ::std::chrono::tzdb;
    using TimeZoneDatabaseList = ::std::chrono::tzdb_list;
    using TimeZone = ::std::chrono::time_zone;
    using SystemInfo = ::std::chrono::sys_info;
    using LocalInfo = ::std::chrono::local_info;

    using LeapSecond = ::std::chrono::leap_second;
    using LeapSecondInfo = ::std::chrono::leap_second_info;

    using AmbiguousLocalTimeException = ::std::chrono::ambiguous_local_time;
    using NonexistentLocalTimeException = ::std::chrono::nonexistent_local_time;

    using ::std::chrono::duration_cast;
    using ::std::chrono::time_point_cast;
    using ::std::chrono::floor;

    using ::std::chrono::get_tzdb;
    using ::std::chrono::get_tzdb_list;
    using ::std::chrono::reload_tzdb;
    using ::std::chrono::remote_version;
    using ::std::chrono::locate_zone;
    using ::std::chrono::current_zone;

    using ::std::chrono::get_leap_second_info;

    using ::std::hash;
    using ::std::formatter;

    using LastSpecifier = ::std::chrono::last_spec;
    inline constexpr LastSpecifier Last = ::std::chrono::last;
    using WeekdayIndexed = ::std::chrono::weekday_indexed;
    using WeekdayLast = ::std::chrono::weekday_last;
    using MonthDayLast = ::std::chrono::month_day_last;
    using MonthWeekday = ::std::chrono::month_weekday;
    using MonthWeekdayLast = ::std::chrono::month_weekday_last;
    using YearMonthDayLast = ::std::chrono::year_month_day_last;
    using YearMonthWeekday = ::std::chrono::year_month_weekday;
    using YearMonthWeekdayLast = ::std::chrono::year_month_weekday_last;
    
    using Choose = ::std::chrono::choose;

    using ::std::chrono::is_am;
    using ::std::chrono::is_pm;
    using ::std::chrono::make12;
    using ::std::chrono::make24;

    template <typename _Duration>
    using HHMMSS = ::std::chrono::hh_mm_ss<_Duration>;

    using ::std::chrono::from_stream;
    using ::std::chrono::parse;

    using Nanoseconds = ::std::chrono::nanoseconds;
    using Microseconds = ::std::chrono::microseconds;
    using Milliseconds = ::std::chrono::milliseconds;
    using Seconds = ::std::chrono::seconds;
    using Minutes = ::std::chrono::minutes;
    using Hours = ::std::chrono::hours;
    using Days = ::std::chrono::days;
    using Weeks = ::std::chrono::weeks;
    using Months = ::std::chrono::months;
    using Years = ::std::chrono::years;

    template <typename _Duration>
    using ZonedTraits = ::std::chrono::zoned_traits<_Duration>;

    template <typename _Duration>
    using ZonedTime = ::std::chrono::zoned_time<_Duration>;

    using TimeZoneLink = ::std::chrono::time_zone_link;
}

/**
 * @internal
 * @namespace _detail
 * @brief Chrono literals namespace
 */
export namespace _detail {
    using ::std::literals::chrono_literals::operator""d;
    using ::std::literals::chrono_literals::operator""h;
    using ::std::literals::chrono_literals::operator""min;
    using ::std::literals::chrono_literals::operator""ms;
    using ::std::literals::chrono_literals::operator""s;
    using ::std::literals::chrono_literals::operator""us;
    using ::std::literals::chrono_literals::operator""y; 
}
