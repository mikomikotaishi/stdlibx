#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Calendar system support inspired by java.time.chrono and HowardHinnant/date.h.
 *
 * Provides multiple chronologies (ISO, Julian, Hijrah, Hebrew, Japanese, Minguo,
 * Thai Buddhist, Chinese) with constexpr calendar arithmetic and
 * epoch-day-based interconversion.
 */
export namespace stdx::time::chrono {
    /**
     * @struct DateComponents
     * @brief Decomposed date components in a chronology.
     */
    struct [[nodiscard]] DateComponents final {
        i32 proleptic_year; ///< The proleptic year. For ISO, this is the same as the calendar year. For other chronologies, this may differ from the calendar year.
        u32 month; ///< The month of the date.
        u32 day; ///< The day of the month.
    };

    /**
     * @concept ChronologyLike
     * @brief Concept constraining types usable as calendar chronologies.
     */
    template <typename T>
    concept ChronologyLike = requires (i32 y, u32 m, u32 d, i64 e) {
        { T::id() } noexcept -> ConvertibleTo<StringView>;
        { T::calendar_type() } noexcept -> ConvertibleTo<StringView>;
        { T::is_leap_year(y) } -> SameAs<bool>;
        { T::days_in_month(y, m) } -> ConvertibleTo<u32>;
        { T::days_in_year(y) } -> ConvertibleTo<i32>;
        { T::to_epoch_day(y, m, d) } -> SameAs<i64>;
        { T::from_epoch_day(e) } -> SameAs<DateComponents>;
    };

    // Forward declaration
    template <ChronologyLike Chrono>
    class ChronoLocalDate;
}

namespace stdx::time::chrono {
    /**
     * @internal
     * @brief Converts a Gregorian date to the number of days since the epoch.
     * @param y The year.
     * @param m The month.
     * @param d The day.
     * @return i64 The number of days since the epoch.
     */
    [[nodiscard]]
    constexpr i64 gregorian_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
        i64 yi = static_cast<i64>(y) - (m <= 2);
        i64 era = (yi >= 0 ? yi : yi - 399) / 400;
        u32 yoe = static_cast<u32>(yi - era * 400);
        u32 doy = (153u * (m > 2 ? m - 3 : m + 9) + 2u) / 5u + d - 1u;
        u32 doe = yoe * 365u + yoe / 4u - yoe / 100u + doy;
        return era * 146097 + static_cast<i64>(doe) - 719468;
    }

    /**
     * @internal
     * @brief Converts the number of days since the epoch to a Gregorian date.
     * @param z The number of days since the epoch.
     * @return DateComponents The corresponding Gregorian date components.
     */
    [[nodiscard]]
    constexpr DateComponents epoch_day_to_gregorian(i64 z) noexcept {
        z += 719468;
        i64 era = (z >= 0 ? z : z - 146096) / 146097;
        u32 doe = static_cast<u32>(z - era * 146097);
        u32 yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        i64 y = static_cast<i64>(yoe) + era * 400;
        u32 doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        u32 mp = (5 * doy + 2) / 153;
        u32 d = doy - (153 * mp + 2) / 5 + 1;
        u32 m = mp < 10 ? mp + 3 : mp - 9;
        return {static_cast<i32>(y + (m <= 2)), m, d};
    }

    /**
     * @internal
     * @brief Check if a Gregorian year is a leap year.
     * @param y The proleptic year.
     * @returns true if the year is a leap year, false otherwise.
     */
    [[nodiscard]]
    constexpr bool is_gregorian_leap(i32 y) noexcept {
        return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
    }
}

export namespace stdx::time::chrono {
    /**
     * @class ChronoLocalDate
     * @brief A date in a specific chronology.
     * @tparam Chrono The chronology type (must satisfy ChronologyLike).
     *
     * Stores decomposed year/month/day components alongside an epoch-day
     * count (days since 1970-01-01 ISO) for efficient cross-chronology
     * conversions and comparisons.
     */
    template <ChronologyLike Chrono>
    class [[nodiscard]] ChronoLocalDate final {
    private:
        i64 epoch_day; ///< The number of days since 1970-01-01 (Gregorian).
        i32 proleptic_year; ///< The proleptic year in the chronology (e.g., 2024 for ISO, 1445 for Hijrah).
        u32 mth; ///< The month of the date.
        u32 dy; ///< The day of the month.
    public:
        using Chronology = Chrono;

        /**
         * @brief Construct a date from year, month, and day components.
         * @param year The proleptic year in the chronology.
         * @param month The month (1-12).
         * @param day The day of the month.
         */
        constexpr ChronoLocalDate(i32 year, u32 month, u32 day):
            epoch_day{Chrono::to_epoch_day(year, month, day)},
            proleptic_year{year}, mth{month}, dy{day} {}

        /**
         * @brief Construct a date from typed year, month, and day components.
         * @param year The proleptic year in the chronology.
         * @param month The month.
         * @param day The day of the month.
         *
         * Convenience overload taking std::chrono-style typed components.
         * Internally converts to integers and delegates to the primary
         * constructor.
         */
        constexpr ChronoLocalDate(Year year, Month month, Day day):
            ChronoLocalDate(
                static_cast<i32>(year),
                static_cast<u32>(static_cast<Month>(month)),
                static_cast<u32>(day)
            ) {}

        /**
         * @brief Named constructor from components.
         * @param year The proleptic year in the chronology.
         * @param month The month (1-12).
         * @param day The day of the month.
         * @returns The constructed date.
         */
        static constexpr ChronoLocalDate of(i32 year, u32 month, u32 day) {
            return ChronoLocalDate{year, month, day};
        }

        /**
         * @brief Named constructor from typed components.
         * @param year The proleptic year in the chronology.
         * @param month The month.
         * @param day The day of the month.
         * @returns The constructed date.
         */
        static constexpr ChronoLocalDate of(Year year, Month month, Day day) {
            return ChronoLocalDate{year, month, day};
        }

        /**
         * @brief Named constructor from an epoch day count.
         * @param epoch_day The number of days since 1970-01-01 (Gregorian).
         * @returns The constructed date.
         */
        static constexpr ChronoLocalDate of_epoch_day(i64 epoch_day) {
            auto [y, m, d] = Chrono::from_epoch_day(epoch_day);
            ChronoLocalDate result{y, m, d};
            result.epoch_day = epoch_day;
            return result;
        }

        /**
         * @brief Construct today's date according to the system clock.
         * @returns Today's date in this chronology.
         */
        static ChronoLocalDate now() {
            SystemDays tp = floor<Days>(SystemClock::now());
            return of_epoch_day(tp.time_since_epoch().count());
        }

        /**
         * @brief Get the proleptic year.
         * @returns The proleptic year in the chronology.
         */
        [[nodiscard]]
        constexpr i32 year() const noexcept {
            return proleptic_year;
        }

        /**
         * @brief Get the month.
         * @returns The month (1-12).
         */
        [[nodiscard]]
        constexpr u32 month() const noexcept {
            return mth;
        }

        /**
         * @brief Get the day of the month.
         * @returns The day of the month.
         */
        [[nodiscard]]
        constexpr u32 day() const noexcept {
            return dy;
        }

        /**
         * @brief Get the epoch day count.
         * @returns The number of days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        constexpr i64 to_epoch_day() const noexcept {
            return epoch_day;
        }

        /**
         * @brief Check if the year of this date is a leap year.
         * @returns true if the year is a leap year in this chronology.
         */
        [[nodiscard]]
        constexpr bool is_leap_year() const {
            return Chrono::is_leap_year(proleptic_year);
        }

        /**
         * @brief Get the number of days in this date's month.
         * @returns The length of the month in days.
         */
        [[nodiscard]]
        constexpr i32 length_of_month() const {
            return static_cast<i32>(Chrono::days_in_month(proleptic_year, mth));
        }

        /**
         * @brief Get the number of days in this date's year.
         * @returns The length of the year in days.
         */
        [[nodiscard]]
        constexpr i32 length_of_year() const {
            return Chrono::days_in_year(proleptic_year);
        }

        /**
         * @brief Convert this date to a system clock time point at midnight.
         * @returns The corresponding SystemDays value.
         */
        [[nodiscard]]
        constexpr SystemDays to_sys_days() const noexcept {
            return SystemDays(Days(epoch_day));
        }

        /**
         * @brief Pack the chronology's (year, month, day) triple into a YearMonthDay.
         * @returns A YearMonthDay carrying this date's components.
         *
         * Provided for interop with std::chrono code that expects a
         * year_month_day. For Gregorian-shaped chronologies (ISO, Julian,
         * Japanese, Minguo, Thai Buddhist) the result is also valid in the
         * std::chrono sense (year_month_day::ok() is true). For lunisolar
         * chronologies (Hebrew, Chinese) the month or day may fall outside
         * std::chrono's valid range (e.g., Hebrew Adar II is month 13) and
         * year_month_day::ok() will return false — the wrapper is still
         * useful as a transport struct but should not be fed back to
         * std::chrono arithmetic.
         */
        [[nodiscard]]
        constexpr YearMonthDay to_ymd() const noexcept {
            return YearMonthDay(Year(proleptic_year), Month(mth), Day(dy));
        }

        /**
         * @brief Get the day of the week for this date.
         * @returns The Weekday corresponding to this date.
         *
         * Chronology-independent: two dates with the same epoch day return
         * the same weekday regardless of their calendar systems.
         */
        [[nodiscard]]
        constexpr Weekday day_of_week() const noexcept {
            return Weekday(to_sys_days());
        }

        /**
         * @brief Explicit conversion to SystemDays.
         * @returns The corresponding SystemDays value.
         */
        [[nodiscard]]
        constexpr explicit operator SystemDays() const noexcept {
            return to_sys_days();
        }

        /**
         * @brief Create a date from a SystemDays time point.
         * @param sd The system clock time point.
         * @returns The corresponding date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate from_sys_days(SystemDays sd) {
            return of_epoch_day(sd.time_since_epoch().count());
        }

        /**
         * @brief Convert this date to the ISO (Gregorian) chronology.
         * @returns The equivalent date in IsoChronology.
         */
        [[nodiscard]]
        constexpr auto to_iso() const noexcept;

        /**
         * @brief Convert this date to another chronology.
         * @tparam Other The target chronology type.
         * @returns The equivalent date in the target chronology.
         */
        template <ChronologyLike Other>
        [[nodiscard]]
        constexpr ChronoLocalDate<Other> to_chronology() const {
            return ChronoLocalDate<Other>::of_epoch_day(epoch_day);
        }

        /**
         * @brief Add days to this date.
         * @param n The number of days to add.
         * @returns A new date offset by n days.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate plus_days(i64 n) const {
            return of_epoch_day(epoch_day + n);
        }

        /**
         * @brief Add a Days duration to this date.
         * @param days The number of days to add.
         * @returns A new date offset by the given duration.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate plus_days(Days days) const {
            return of_epoch_day(epoch_day + days.count());
        }

        /**
         * @brief Add a Months duration to this date.
         * @param months The number of months to add.
         * @returns A new date with the month advanced by @p months.
         *
         * If the resulting day exceeds the length of the target month, the
         * day is clamped to the last valid day of that month (mirrors
         * java.time semantics). Negative values move backwards.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate plus_months(Months months) const {
            i64 n = months.count();
            i32 y = proleptic_year;
            i64 mo = static_cast<i64>(mth);
            while (n > 0) {
                i64 mly = static_cast<i64>(Chrono::months_in_year(y));
                i64 room = mly - mo;
                if (n <= room) {
                    mo += n;
                    n = 0;
                } else {
                    n -= room + 1;
                    mo = 1;
                    ++y;
                }
            }
            while (n < 0) {
                i64 used = mo - 1;
                if (-n <= used) {
                    mo += n;
                    n = 0;
                } else {
                    n += used + 1;
                    --y;
                    mo = static_cast<i64>(Chrono::months_in_year(y));
                }
            }
            u32 new_dm = Chrono::days_in_month(y, static_cast<u32>(mo));
            u32 new_d = dy > new_dm ? new_dm : dy;
            return ChronoLocalDate(y, static_cast<u32>(mo), new_d);
        }

        /**
         * @brief Add a Years duration to this date.
         * @param years The number of years to add.
         * @returns A new date with the year advanced by @p years.
         *
         * If the original day does not exist in the target year's month
         * (e.g., 2024-02-29 + 1 year), the day is clamped to the last
         * valid day of that month.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate plus_years(Years years) const {
            i32 new_y = proleptic_year + static_cast<i32>(years.count());
            u32 new_dm = Chrono::days_in_month(new_y, mth);
            u32 new_d = dy > new_dm ? new_dm : dy;
            return ChronoLocalDate(new_y, mth, new_d);
        }

        /**
         * @brief Subtract days from this date.
         * @param n The number of days to subtract.
         * @returns A new date offset by -n days.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate minus_days(i64 n) const {
            return of_epoch_day(epoch_day - n);
        }

        /**
         * @brief Subtract a Days duration from this date.
         * @param days The number of days to subtract.
         * @returns A new date offset by -@p days.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate minus_days(Days days) const {
            return of_epoch_day(epoch_day - days.count());
        }

        /**
         * @brief Subtract a Months duration from this date.
         * @param months The number of months to subtract.
         * @returns A new date with the month moved back by @p months.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate minus_months(Months months) const {
            return plus_months(-months);
        }

        /**
         * @brief Subtract a Years duration from this date.
         * @param years The number of years to subtract.
         * @returns A new date with the year moved back by @p years.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate minus_years(Years years) const {
            return plus_years(-years);
        }

        constexpr ChronoLocalDate& operator+=(Days days) {
            *this = plus_days(days);
            return *this;
        }

        constexpr ChronoLocalDate& operator+=(Months months) {
            *this = plus_months(months);
            return *this;
        }

        constexpr ChronoLocalDate& operator+=(Years years) {
            *this = plus_years(years);
            return *this;
        }

        constexpr ChronoLocalDate& operator-=(Days days) {
            *this = minus_days(days);
            return *this;
        }

        constexpr ChronoLocalDate& operator-=(Months months) {
            *this = minus_months(months);
            return *this;
        }

        constexpr ChronoLocalDate& operator-=(Years years) {
            *this = minus_years(years);
            return *this;
        }

        constexpr ChronoLocalDate& operator++() {
            *this = of_epoch_day(epoch_day + 1);
            return *this;
        }

        constexpr ChronoLocalDate operator++(i32) {
            ChronoLocalDate tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr ChronoLocalDate& operator--() {
            *this = of_epoch_day(epoch_day - 1);
            return *this;
        }

        constexpr ChronoLocalDate operator--(i32) {
            ChronoLocalDate tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief Three-way comparison with another date of the same chronology.
         * @param other The other date.
         * @returns The ordering between the two dates.
         */
        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const ChronoLocalDate& other) const noexcept = default;

        /**
         * @brief Three-way comparison with a date of a different chronology.
         * @tparam Other The other chronology type.
         * @param other The other date.
         * @returns The ordering based on epoch day comparison.
         */
        template <ChronologyLike Other>
        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const ChronoLocalDate<Other>& other) const noexcept {
            return epoch_day <=> other.to_epoch_day();
        }

        /**
         * @brief Equality comparison with a date of a different chronology.
         * @tparam Other The other chronology type.
         * @param other The other date.
         * @returns true if both dates represent the same epoch day.
         */
        template <ChronologyLike Other>
        [[nodiscard]]
        constexpr bool operator==(const ChronoLocalDate<Other>& other) const noexcept {
            return epoch_day == other.to_epoch_day();
        }
    };

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(const ChronoLocalDate<Chrono>& d, Days days) {
        return d.plus_days(days);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(Days days, const ChronoLocalDate<Chrono>& d) {
        return d.plus_days(days);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(const ChronoLocalDate<Chrono>& d, Months months) {
        return d.plus_months(months);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(Months months, const ChronoLocalDate<Chrono>& d) {
        return d.plus_months(months);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(const ChronoLocalDate<Chrono>& d, Years years) {
        return d.plus_years(years);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator+(Years years, const ChronoLocalDate<Chrono>& d) {
        return d.plus_years(years);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator-(const ChronoLocalDate<Chrono>& d, Days days) {
        return d.minus_days(days);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator-(const ChronoLocalDate<Chrono>& d, Months months) {
        return d.minus_months(months);
    }

    template <ChronologyLike Chrono>
    [[nodiscard]]
    constexpr ChronoLocalDate<Chrono> operator-(const ChronoLocalDate<Chrono>& d, Years years) {
        return d.minus_years(years);
    }

    /**
     * @brief Difference between two dates as a Days duration.
     * @returns The signed number of days from @p b to @p a.
     *
     * Works across chronologies via the shared epoch_day representation.
     */
    template <ChronologyLike A, ChronologyLike B>
    [[nodiscard]]
    constexpr Days operator-(const ChronoLocalDate<A>& a, const ChronoLocalDate<B>& b) noexcept {
        return Days(a.to_epoch_day() - b.to_epoch_day());
    }
}

#include "stdx/time/chrono/iso.inl"
#include "stdx/time/chrono/julian.inl"
#include "stdx/time/chrono/hijrah.inl"
#include "stdx/time/chrono/hebrew.inl"
#include "stdx/time/chrono/japanese.inl"
#include "stdx/time/chrono/minguo.inl"
#include "stdx/time/chrono/thai_buddhist.inl"
#include "stdx/time/chrono/chinese.inl"

namespace stdx::time::chrono {
    template <ChronologyLike Chrono>
    constexpr auto ChronoLocalDate<Chrono>::to_iso() const noexcept {
        return ChronoLocalDate<IsoChronology>::of_epoch_day(epoch_day);
    }
}

using stdx::time::chrono::ChronoLocalDate;
using stdx::time::chrono::ChronologyLike;

namespace stdx::fmt {
    template <ChronologyLike Chrono, typename Char>
    struct Formatter<ChronoLocalDate<Chrono>, Char> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const ChronoLocalDate<Chrono>& d, FormatContext& ctx) {
            return format_to(ctx.out(), "{}-{:02d}-{:02d}", d.year(), d.month(), d.day());
        }
    };
}

template <ChronologyLike Chrono, typename Char>
struct stdx::fmt::formatter<ChronoLocalDate<Chrono>, Char> : public stdx::fmt::Formatter<ChronoLocalDate<Chrono>, Char> {};
