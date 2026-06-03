#pragma once

using stdx::fmt::FormatContext;
using stdx::fmt::FormatParseContext;
using stdx::fmt::Formatter;

using stdx::io::BasicOutputStream;
using stdx::math::ratio::Ratio;

/**
 * @namespace stdx::time
 * @brief Wrapper namespace for standard library time operations.
 */
export namespace stdx::time {
    template <typename Rep, typename Period = Ratio<1>>
    using Duration = ::std::chrono::duration<Rep, Period>;

    template <typename Rep>
    using TreatAsFloatingPoint = ::std::chrono::treat_as_floating_point<Rep>;

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

    using SystemDays = ::std::chrono::sys_days;
    using LocalDays = ::std::chrono::local_days;

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

    using Day = ::std::chrono::day;
    using Year = ::std::chrono::year;
    using MonthDay = ::std::chrono::month_day;
    using YearMonth = ::std::chrono::year_month;
    using YearMonthDay = ::std::chrono::year_month_day;

    class [[nodiscard]] Month final {
    public:
        using Self = std::chrono::month;

        static constexpr Self JANUARY = std::chrono::January;
        static constexpr Self FEBRUARY = std::chrono::February;
        static constexpr Self MARCH = std::chrono::March;
        static constexpr Self APRIL = std::chrono::April;
        static constexpr Self MAY = std::chrono::May;
        static constexpr Self JUNE = std::chrono::June;
        static constexpr Self JULY = std::chrono::July;
        static constexpr Self AUGUST = std::chrono::August;
        static constexpr Self SEPTEMBER = std::chrono::September;
        static constexpr Self OCTOBER = std::chrono::October;
        static constexpr Self NOVEMBER = std::chrono::November;
        static constexpr Self DECEMBER = std::chrono::December;
    private:
        Self value;
    public:
        constexpr Month() noexcept = default;

        constexpr Month(Self value) noexcept:
            value{value} {}

        constexpr explicit Month(unsigned int value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        constexpr Month& operator++() noexcept {
            ++value;
            return *this;
        }

        constexpr Month operator++(int) noexcept {
            Month temp = *this;
            ++value;
            return temp;
        }

        constexpr Month& operator--() noexcept {
            --value;
            return *this;
        }

        constexpr Month operator--(int) noexcept {
            Month temp = *this;
            --value;
            return temp;
        }

        constexpr Month& operator+=(const Months& m) noexcept {
            value += m;
            return *this;
        }

        constexpr Month& operator-=(const Months& m) noexcept {
            value -= m;
            return *this;
        }

        constexpr explicit operator unsigned int() const noexcept {
            return static_cast<unsigned int>(value);
        }

        [[nodiscard]]
        constexpr bool ok() const noexcept {
            return value.ok();
        }
    };

    class [[nodiscard]] Weekday final {
    public:
        using Self = std::chrono::weekday;

        static constexpr Self SUNDAY = std::chrono::Sunday;
        static constexpr Self MONDAY = std::chrono::Monday;
        static constexpr Self TUESDAY = std::chrono::Tuesday;
        static constexpr Self WEDNESDAY = std::chrono::Wednesday;
        static constexpr Self THURSDAY = std::chrono::Thursday;
        static constexpr Self FRIDAY = std::chrono::Friday;
        static constexpr Self SATURDAY = std::chrono::Saturday;
    private:
        Self value;
    public:
        constexpr Weekday() noexcept = default;

        constexpr Weekday(Self value) noexcept:
            value{value} {}

        constexpr explicit Weekday(unsigned int value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        constexpr Weekday& operator++() noexcept {
            ++value;
            return *this;
        }

        constexpr Weekday operator++(int) noexcept {
            Weekday temp = *this;
            ++value;
            return temp;
        }

        constexpr Weekday& operator--() noexcept {
            --value;
            return *this;
        }

        constexpr Weekday operator--(int) noexcept {
            Weekday temp = *this;
            --value;
            return temp;
        }

        constexpr Weekday& operator+=(const Days& d) noexcept {
            value += d;
            return *this;
        }

        constexpr Weekday& operator-=(const Days& d) noexcept {
            value -= d;
            return *this;
        }

        [[nodiscard]]
        constexpr bool ok() const noexcept {
            return value.ok();
        }

        [[nodiscard]]
        constexpr unsigned int c_encoding() const noexcept {
            return value.c_encoding();
        }

        [[nodiscard]]
        constexpr unsigned int iso_encoding() const noexcept {
            return value.iso_encoding();
        }

        constexpr WeekdayIndexed operator[](unsigned int index) const noexcept {
            return WeekdayIndexed(value, index);
        }

        constexpr WeekdayLast operator[](LastSpecifier) const noexcept {
            return WeekdayLast(value);
        }
    };

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
    
    class [[nodiscard]] Choose final {
    public:
        using Self = std::chrono::choose;

        static constexpr Self EARLIEST = std::chrono::choose::earliest;
        static constexpr Self LATEST = std::chrono::choose::latest;
    private:
        const Self value;
    public:
        constexpr Choose() noexcept = delete;

        constexpr Choose(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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

    [[nodiscard]]
    constexpr bool operator==(const Month& x, const Month& y) noexcept {
        return static_cast<unsigned int>(x) == static_cast<unsigned int>(y);
    }

    [[nodiscard]]
    constexpr bool operator==(const Weekday& x, const Weekday& y) noexcept {
        return x.c_encoding() == y.c_encoding();
    }

    [[nodiscard]]
    constexpr StrongOrdering operator<=>(const Month& x, const Month& y) noexcept {
        return static_cast<unsigned int>(x) <=> static_cast<unsigned int>(y);
    }

    [[nodiscard]]
    constexpr Month operator+(const Month& m, const Months& ms) noexcept {
        return static_cast<Month::Self>(m) + static_cast<Months>(ms);
    }

    [[nodiscard]]
    constexpr Month operator+(const Months& ms, const Month& m) noexcept {
        return static_cast<Month::Self>(m) + static_cast<Months>(ms);
    }

    [[nodiscard]]
    constexpr Weekday operator+(const Weekday& w, const Days& d) noexcept {
        return static_cast<Weekday::Self>(w) + static_cast<Days>(d);
    }

    [[nodiscard]]
    constexpr Weekday operator+(const Days& d, const Weekday& w) noexcept {
        return static_cast<Weekday::Self>(w) + static_cast<Days>(d);
    }

    [[nodiscard]]
    constexpr Month operator-(const Month& m, const Months& ms) noexcept {
        return static_cast<Month::Self>(m) - static_cast<Months>(ms);
    }

    [[nodiscard]]
    constexpr Month operator-(const Months& ms, const Month& m) noexcept {
        return static_cast<Month::Self>(m) - static_cast<Months>(ms);
    }

    [[nodiscard]]
    constexpr Weekday operator-(const Weekday& w, const Days& d) noexcept {
        return static_cast<Weekday::Self>(w) - static_cast<Days>(d);
    }

    [[nodiscard]]
    constexpr Days operator-(const Weekday& w1, const Weekday& w2) noexcept {
        return static_cast<Weekday::Self>(w1) - static_cast<Weekday::Self>(w2);
    }

    template <typename Char, typename Traits>
    BasicOutputStream<Char, Traits>& operator<<(BasicOutputStream<Char, Traits>& os, const Month& m) {
        return os << static_cast<Month::Self>(m);
    }

    template <typename Char, typename Traits>
    BasicOutputStream<Char, Traits>& operator<<(BasicOutputStream<Char, Traits>& os, const Weekday& w) {
        return os << static_cast<Weekday::Self>(w);
    }

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
    using ::std::chrono::operator<<;

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

using stdx::time::Month;
using stdx::time::Weekday;

namespace stdx::fmt {
    template <>
    struct Formatter<Month> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const Month& m, FormatContext& ctx) {
            return format_to(ctx.out(), "{}", static_cast<Month::Self>(m));
        }
    };

    template <>
    struct Formatter<Weekday> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const Weekday& w, FormatContext& ctx) {
            return format_to(ctx.out(), "{}", static_cast<Weekday::Self>(w));
        }
    };
}

template <>
struct stdx::fmt::formatter<Month> : public stdx::fmt::Formatter<Month> {};

template <>
struct stdx::fmt::formatter<Weekday> : public stdx::fmt::Formatter<Weekday> {};
