#pragma once

using stdx::math::ratio::Ratio;

/**
 * @namespace stdx::time
 * @brief Wrapper namespace for standard library time operations.
 */
export namespace stdx::time {
    template <typename Rep, typename Period = Ratio<1>>
    using Duration = ::std::chrono::duration<Rep, Period>;

    template <typename Rep>
    using TreatAsFloatingPoi32 = ::std::chrono::treat_as_floating_point<Rep>;

    template <typename Rep>
    using DurationValues = ::std::chrono::duration_values<Rep>;

    template <typename Clock, typename Duration = typename Clock::duration>
    using Instant = ::std::chrono::time_point<Clock, Duration>;

    template <typename Duration>
    using LocalTime = ::std::chrono::local_time<Duration>;

    template <typename Dest, typename Source>
    using ClockTimeConversion = ::std::chrono::clock_time_conversion<Dest, Source>;

    template <typename T>
    using IsClock = ::std::chrono::is_clock<T>;

    using SystemClock = ::std::chrono::system_clock;
    using SteadyClock = ::std::chrono::steady_clock;
    using HighResolutionClock = ::std::chrono::high_resolution_clock;
    using UtcClock = ::std::chrono::utc_clock;
    using TaiClock = ::std::chrono::tai_clock;
    using GpsClock = ::std::chrono::gps_clock;
    using FileClock = ::std::chrono::file_clock;
    using Local = ::std::chrono::local_t;

    template <typename Dur>
    using SystemTime = ::std::chrono::sys_time<Dur>;

    template <typename Dur>
    using UtcTime = ::std::chrono::utc_time<Dur>;

    template <typename Dur>
    using TaiTime = ::std::chrono::tai_time<Dur>;

    template <typename Dur>
    using GpsTime = ::std::chrono::gps_time<Dur>;

    template <typename Dur>
    using FileTime = ::std::chrono::file_time<Dur>;

    template <typename Dur>
    using LocalTime = ::std::chrono::local_time<Dur>;

    using SystemSeconds = ::std::chrono::sys_seconds;
    using UtcSeconds = ::std::chrono::utc_seconds;
    using TaiSeconds = ::std::chrono::tai_seconds;
    using GpsSeconds = ::std::chrono::gps_seconds;
    using LocalSeconds = ::std::chrono::local_seconds;

    using SystemDays = ::std::chrono::sys_days;
    using LocalDays = ::std::chrono::local_days;

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

    using TimeZoneDatabase = ::std::chrono::tzdb;
    using TimeZoneDatabaseList = ::std::chrono::tzdb_list;
    using TimeZone = ::std::chrono::time_zone;
    using TimeZoneLink = ::std::chrono::time_zone_link;

    template <typename Dur>
    using ZonedTraits = ::std::chrono::zoned_traits<Dur>;

    template <typename Dur>
    using ZonedTime = ::std::chrono::zoned_time<Dur>;

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
    using SystemInfo = ::std::chrono::sys_info;
    using LocalInfo = ::std::chrono::local_info;

    using LeapSecond = ::std::chrono::leap_second;
    using LeapSecondInfo = ::std::chrono::leap_second_info;

    using AmbiguousLocalTimeException = ::std::chrono::ambiguous_local_time;
    using NonexistentLocalTimeException = ::std::chrono::nonexistent_local_time;

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
    
    class [[nodiscard]] Choose final {
    public:
        using Self = std::chrono::choose;

        static constexpr Self EARLIEST = std::chrono::choose::earliest;
        static constexpr Self LATEST = std::chrono::choose::latest;
    private:
        Self value;
    public:
        constexpr Choose(Self value = {}) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    /**
     * @class DateTimeException
     * @brief Exception thrown upon a problem with creating, querying or manipulating date-time objects.
     *
     * @extends RuntimeException
     */
    class DateTimeException: public RuntimeException {
    public:
        explicit DateTimeException(const String& msg = ""):
            RuntimeException(msg) {}
    };

    template <typename Duration>
    using TimeOfDay = ::std::chrono::hh_mm_ss<Duration>;

    using ::std::chrono::duration_cast;
    using ::std::chrono::time_point_cast;
    using ::std::chrono::floor;
    using ::std::chrono::ceil;
    using ::std::chrono::round;
    using ::std::chrono::abs;
    using ::std::chrono::from_stream;
    using ::std::chrono::clock_cast;

    using ::std::chrono::get_tzdb;
    using ::std::chrono::get_tzdb_list;
    using ::std::chrono::reload_tzdb;
    using ::std::chrono::remote_version;
    using ::std::chrono::locate_zone;
    using ::std::chrono::current_zone;

    using ::std::chrono::get_leap_second_info;

    using ::std::chrono::is_am;
    using ::std::chrono::is_pm;
    using ::std::chrono::make12;
    using ::std::chrono::make24;

    using ::std::chrono::from_stream;
    using ::std::chrono::parse;

    using ::std::hash;
    using ::std::formatter;

    using ::std::chrono::operator+;
    using ::std::chrono::operator-;
    using ::std::chrono::operator*;
    using ::std::chrono::operator/;
    using ::std::chrono::operator%;
    using ::std::chrono::operator==;
    using ::std::chrono::operator<;
    using ::std::chrono::operator<=;
    using ::std::chrono::operator>;
    using ::std::chrono::operator>=;
    using ::std::chrono::operator<=>;

    /**
     * @brief Get the current time as a string.
     * @return The current time formatted as a string.
     */
    [[nodiscard]]
    String current_time_as_string() {
        Instant<SystemClock> now = SystemClock::now();
        LocalTime<Seconds> currentTime = stdx::time::current_zone()->to_local(stdx::time::floor<Seconds>(now));
        return stdx::fmt::format("{:%Y-%m-%d %H:%M:%S}", currentTime);
    }
}

/**
 * @namespace stdx::literals::chrono_literals
 * @brief Chrono literals namespace
 */
export namespace stdx::inline literals::inline chrono_literals  {
    using ::std::literals::chrono_literals::operator""d;
    using ::std::literals::chrono_literals::operator""h;
    using ::std::literals::chrono_literals::operator""min;
    using ::std::literals::chrono_literals::operator""ms;
    using ::std::literals::chrono_literals::operator""s;
    using ::std::literals::chrono_literals::operator""us;
    using ::std::literals::chrono_literals::operator""y; 
}
