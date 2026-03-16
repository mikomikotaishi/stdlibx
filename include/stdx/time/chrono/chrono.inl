#pragma once

using namespace stdx::core;

/**
 * @namespace stdx::time::chrono
 * @brief Calendar system support inspired by java.time.chrono and HowardHinnant/date.h.
 *
 * Provides multiple chronologies (ISO, Hijrah, Japanese, Minguo, Thai Buddhist)
 * with constexpr calendar arithmetic and epoch-day-based interconversion.
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

    inline constexpr i64 HIJRAH_EPOCH = gregorian_to_epoch_day(622, 7, 19); ///< The epoch day of the Hijrah calendar (corresponding to 622-07-19 in the Gregorian calendar).

    /**
     * @internal
     * @brief Check if a Hijrah year is a leap year.
     * @param y The proleptic year.
     * @returns true if the year is a leap year, false otherwise.
     */
    [[nodiscard]]
    constexpr bool is_hijrah_leap(i32 y) noexcept {
        return (14 + 11 * y) % 30 < 11;
    }

    /**
     * @internal
     * @brief Get the number of days in a month for a given Hijrah year and month.
     * @param y The proleptic year.
     * @param m The month (1-12).
     * @returns The number of days in the month.
     */
    [[nodiscard]]
    constexpr u32 hijrah_days_in_month(i32 y, u32 m) noexcept {
        if (m % 2 == 1) {
            return 30;
        } else if (m == 12 && is_hijrah_leap(y)) {
            return 30;
        }
        return 29;
    }

    /**
     * @internal
     * @brief Get the number of days in a given Hijrah year.
     * @param y The proleptic year.
     * @returns The number of days in the year.
     */
    [[nodiscard]]
    constexpr i32 hijrah_days_in_year(i32 y) noexcept {
        return is_hijrah_leap(y) ? 355 : 354;
    }

    /**
     * @internal
     * @brief Converts a Hijrah date to the number of days since the epoch.
     * @param y The year.
     * @param m The month.
     * @param d The day.
     * @return i64 The number of days since the epoch.
     */
    [[nodiscard]]
    constexpr i64 hijrah_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
        i64 years_elapsed = static_cast<i64>(y - 1);
        i64 leap_days = (3 + 11 * years_elapsed) / 30;
        i64 month_days = 29 * static_cast<i64>(m - 1) + static_cast<i64>(m / 2);
        return HIJRAH_EPOCH + years_elapsed * 354 + leap_days + month_days + static_cast<i64>(d) - 1;
    }

    /**
     * @internal
     * @brief Converts the number of days since the epoch to a Hijrah date.
     * @param epoch_day The number of days since the epoch.
     * @return DateComponents The corresponding Hijrah date components.
     */
    [[nodiscard]]
    constexpr DateComponents epoch_day_to_hijrah(i64 epoch_day) noexcept {
        i64 days = epoch_day - HIJRAH_EPOCH;
        i64 cycles = days / 10631;
        i32 rem = days % 10631;
        if (rem < 0) {
            --cycles;
            rem += 10631;
        }

        i32 y = static_cast<i32>(cycles * 30 + 1);
        for (i32 i = 0; i < 30; ++i) {
            i32 dy = hijrah_days_in_year(y);
            if (rem < dy) {
                break;
            }
            rem -= dy;
            ++y;
        }

        u32 m = 1;
        for (u32 i = 0; i < 12; ++i) {
            i64 dm = static_cast<i64>(hijrah_days_in_month(y, m));
            if (rem < dm) {
                break;
            }
            rem -= dm;
            ++m;
        }

        return {y, m, static_cast<u32>(rem + 1)};
    }

    /**
     * @internal
     * @brief Check if a Julian year is a leap year.
     * @param y The proleptic year.
     * @returns true if the year is a leap year, false otherwise.
     */
    [[nodiscard]]
    constexpr bool is_julian_leap(i32 y) noexcept {
        return y % 4 == 0;
    }

    /**
     * @internal
     * @brief Converts a Julian date to the number of days since the epoch.
     * @param y The year.
     * @param m The month.
     * @param d The day.
     * @return i64 The number of days since the epoch.
     */
    [[nodiscard]]
    constexpr i64 julian_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
        i64 yi = static_cast<i64>(y) - (m <= 2);
        i64 era = (yi >= 0 ? yi : yi - 3) / 4;
        u32 yoe = static_cast<u32>(yi - era * 4);
        u32 doy = (153u * (m > 2 ? m - 3 : m + 9) + 2u) / 5u + d - 1u;
        u32 doe = yoe * 365u + doy;
        return era * 1461 + static_cast<i64>(doe) - 719470;
    }

    /**
     * @internal
     * @brief Converts the number of days since the epoch to a Julian date.
     * @param z The number of days since the epoch.
     * @return DateComponents The corresponding Julian date components.
     */
    [[nodiscard]]
    constexpr DateComponents epoch_day_to_julian(i64 z) noexcept {
        z += 719470;
        i64 era = (z >= 0 ? z : z - 1460) / 1461;
        u32 doe = static_cast<u32>(z - era * 1461);
        u32 yoe = (doe - doe / 1460) / 365;
        i64 y = static_cast<i64>(yoe) + era * 4;
        u32 doy = doe - 365 * yoe;
        u32 mp = (5 * doy + 2) / 153;
        u32 d = doy - (153 * mp + 2) / 5 + 1;
        u32 m = mp < 10 ? mp + 3 : mp - 9;
        return {static_cast<i32>(y + (m <= 2)), m, d};
    }

    inline constexpr i64 MEIJI_START = gregorian_to_epoch_day(1868, 1, 25); ///< The epoch day of the Meiji era (corresponding to 1868-01-25 in the Gregorian calendar).
    inline constexpr i64 TAISHO_START = gregorian_to_epoch_day(1912, 7, 30); ///< The epoch day of the Taisho era (corresponding to 1912-07-30 in the Gregorian calendar).
    inline constexpr i64 SHOWA_START = gregorian_to_epoch_day(1926, 12, 25); ///< The epoch day of the Showa era (corresponding to 1926-12-25 in the Gregorian calendar).
    inline constexpr i64 HEISEI_START = gregorian_to_epoch_day(1989, 1, 8); ///< The epoch day of the Heisei era (corresponding to 1989-01-08 in the Gregorian calendar).
    inline constexpr i64 REIWA_START = gregorian_to_epoch_day(2019, 5, 1); ///< The epoch day of the Reiwa era (corresponding to 2019-05-01 in the Gregorian calendar).
}

/**
 * @namespace stdx::time::chrono
 * @brief Calendar system support.
 *
 * Provides multiple chronologies (ISO, Hijrah, Japanese, Minguo, Thai Buddhist)
 * with constexpr calendar arithmetic and epoch-day-based interconversion.
 */
export namespace stdx::time::chrono {
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

    /**
     * @class IsoChronology
     * @brief The ISO-8601 (proleptic Gregorian) chronology.
     *
     * This is the default calendar system used by std::chrono.
     */
    class [[nodiscard]] IsoChronology final {
    public:
        IsoChronology() = delete;

        /**
         * @enum Era
         * @brief ISO calendar era (BCE / CE).
         */
        enum class Era: i32 {
            BCE = 0, ///< Before Common Era (proleptic years <= 0)
            CE = 1, ///< Common Era (proleptic years >= 1)
        };

        /**
         * @brief Returns the chronology identifier.
         * @returns "ISO"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "ISO"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "iso8601"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "iso8601"sv;
        }

        /**
         * @brief Check if a year is a leap year.
         * @param y The proleptic year.
         * @returns true if the year is a leap year.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 y) noexcept {
            return is_gregorian_leap(y);
        }

        /**
         * @brief Get the number of days in a month.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 y, u32 m) noexcept {
            static constexpr Array<u32, 12> TABLE{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (m == 2 && is_leap_year(y)) {
                return 29;
            }
            return TABLE[m - 1];
        }

        /**
         * @brief Get the number of days in a year.
         * @param y The proleptic year.
         * @returns 366 if the year is a leap year, 365 otherwise.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 y) noexcept {
            return is_leap_year(y) ? 366 : 365;
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic year.
         * @returns 12
         */
        [[nodiscard]]
        static constexpr u32 months_in_year([[maybe_unused]] i32 y) noexcept {
            return 12;
        }

        /**
         * @brief Convert a date to an epoch day count.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01.
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) noexcept {
            return gregorian_to_epoch_day(y, m, d);
        }

        /**
         * @brief Convert an epoch day count to date components.
         * @param e The number of days since 1970-01-01.
         * @returns The decomposed date components.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) noexcept {
            return epoch_day_to_gregorian(e);
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic year.
         * @returns Era::CE if proleptic_year >= 1, Era::BCE otherwise.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? Era::CE : Era::BCE;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic year.
         * @returns The positive year-of-era value.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? proleptic_year : 1 - proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era.
         * @param year_of_era The year within the era.
         * @returns The proleptic year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) noexcept {
            return era == Era::CE ? year_of_era : 1 - year_of_era;
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<IsoChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<IsoChronology> date_epoch_day(i64 e) noexcept;

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<IsoChronology> date_now() noexcept;
    };

    /**
     * @class JulianChronology
     * @brief The proleptic Julian chronology.
     *
     * Uses the Julian calendar rules: every 4th year is a leap year,
     * with no century correction. This is the calendar used in Europe
     * before the Gregorian reform of 1582-10-15.
     */
    class [[nodiscard]] JulianChronology final {
    public:
        JulianChronology() = delete;

        /**
         * @enum Era
         * @brief Julian calendar era (BC / AD).
         */
        enum class Era: i32 {
            BC = 0, ///< Before Christ (proleptic years <= 0)
            AD = 1, ///< Anno Domini (proleptic years >= 1)
        };

        /**
         * @brief Returns the chronology identifier.
         * @returns "Julian"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Julian"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "julian"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "julian"sv;
        }

        /**
         * @brief Check if a year is a leap year.
         * @param y The proleptic year.
         * @returns true if the year is divisible by 4.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 y) noexcept {
            return is_julian_leap(y);
        }

        /**
         * @brief Get the number of days in a month.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 y, u32 m) noexcept {
            static constexpr Array<u32, 12> TABLE{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (m == 2 && is_leap_year(y)) {
                return 29;
            }
            return TABLE[m - 1];
        }

        /**
         * @brief Get the number of days in a year.
         * @param y The proleptic year.
         * @returns 366 if the year is a leap year, 365 otherwise.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 y) noexcept {
            return is_leap_year(y) ? 366 : 365;
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic year.
         * @returns 12
         */
        [[nodiscard]]
        static constexpr u32 months_in_year([[maybe_unused]] i32 y) noexcept {
            return 12;
        }

        /**
         * @brief Convert a date to an epoch day count.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) noexcept {
            return julian_to_epoch_day(y, m, d);
        }

        /**
         * @brief Convert an epoch day count to date components.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The decomposed Julian date components.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) noexcept {
            return epoch_day_to_julian(e);
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic year.
         * @returns Era::AD if proleptic_year >= 1, Era::BC otherwise.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? Era::AD : Era::BC;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic year.
         * @returns The positive year-of-era value.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? proleptic_year : 1 - proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era.
         * @param year_of_era The year within the era.
         * @returns The proleptic year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) noexcept {
            return era == Era::AD ? year_of_era : 1 - year_of_era;
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JulianChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JulianChronology> date_epoch_day(i64 e) noexcept;

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<JulianChronology> date_now() noexcept;
    };

    /**
     * @class HijrahChronology
     * @brief The Hijrah (Islamic tabular) chronology.
     *
     * Uses the standard tabular Islamic calendar with a 30-year cycle
     * containing 11 leap years. The epoch is 1 Muharram 1 AH
     * (16 July 622 CE Gregorian). Months alternate between 30 and 29 days;
     * in leap years the 12th month (Dhu al-Hijjah) has 30 days.
     */
    class [[nodiscard]] HijrahChronology final {
    public:
        HijrahChronology() = delete;

        /**
         * @enum Era
         * @brief Hijrah (Islamic) calendar era.
         */
        enum class Era: i32 {
            AH = 1, ///< Anno Hegirae (in the year of the Hijrah, proleptic years >= 1)
        };

        /**
         * @brief Returns the chronology identifier.
         * @returns "Hijrah-umalqura"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Hijrah-umalqura"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "islamic-umalqura"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "islamic-umalqura"sv;
        }

        /**
         * @brief Check if a year is a leap year in the Hijrah calendar.
         * @param y The proleptic Hijrah year.
         * @returns true if the year is a leap year.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return is_hijrah_leap(y);
        }

        /**
         * @brief Get the number of days in a month.
         * @param y The proleptic Hijrah year.
         * @param m The month (1-12).
         * @returns The number of days in the month (29 or 30).
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 y, u32 m) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return hijrah_days_in_month(y, m);
        }

        /**
         * @brief Get the number of days in a year.
         * @param y The proleptic Hijrah year.
         * @returns 355 if the year is a leap year, 354 otherwise.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return hijrah_days_in_year(y);
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic Hijrah year.
         * @returns 12
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr u32 months_in_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return 12;
        }

        /**
         * @brief Convert a Hijrah date to an epoch day count.
         * @param y The proleptic Hijrah year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01 (Gregorian).
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return hijrah_to_epoch_day(y, m, d);
        }

        /**
         * @brief Convert an epoch day count to Hijrah date components.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The decomposed Hijrah date components.
         * @throws DateTimeException if e < HIJRAH_EPOCH.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) throws (DateTimeException) {
            if (e < HIJRAH_EPOCH) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return epoch_day_to_hijrah(e);
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic Hijrah year.
         * @returns Era::AH
         * @throws DateTimeException if proleptic_year < 1.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) throws (DateTimeException) {
            if (proleptic_year < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return Era::AH;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic Hijrah year.
         * @returns The year-of-era (equal to the proleptic year for Hijrah).
         * @throws DateTimeException if proleptic_year < 1.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) throws (DateTimeException) {
            if (proleptic_year < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era (always AH).
         * @param year_of_era The year within the era.
         * @returns The proleptic year (equal to year_of_era for Hijrah).
         * @throws DateTimeException if year_of_era < 1.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year([[maybe_unused]] Era era, i32 year_of_era) throws (DateTimeException) {
            if (year_of_era < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return year_of_era;
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic Hijrah year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<HijrahChronology> of(i32 y, u32 m, u32 d) throws (DateTimeException);

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         * @throws DateTimeException if e < HIJRAH_EPOCH.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<HijrahChronology> date_epoch_day(i64 e) throws (DateTimeException);

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<HijrahChronology> date_now() throws (DateTimeException);
    };

    /**
     * @class JapaneseChronology
     * @brief The Japanese Imperial chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) but with
     * Japanese Imperial era-based year numbering. Supports eras
     * from Meiji (1868) through Reiwa (2019-).
     */
    class [[nodiscard]] JapaneseChronology final {
    public:
        JapaneseChronology() = delete;

        /** 
         * @enum Era
         * @brief Japanese Imperial calendar era.
         */
        enum class Era: i32 {
            MEIJI = 0, ///< Meiji era (1868-01-25 to 1912-07-29)
            TAISHO = 1, ///< Taisho era (1912-07-30 to 1926-12-24)
            SHOWA = 2, ///< Showa era (1926-12-25 to 1989-01-07)
            HEISEI = 3, ///< Heisei era (1989-01-08 to 2019-04-30)
            REIWA = 4, ///< Reiwa era (2019-05-01 to present)
        };

        /**
         * @brief Returns the chronology identifier.
         * @returns "Japanese"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Japanese"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "japanese"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "japanese"sv;
        }

        /**
         * @brief Check if a year is a leap year (uses Gregorian rules).
         * @param y The ISO proleptic year.
         * @returns true if the year is a leap year.
         * @throws DateTimeException if y < 1868.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 y) throws (DateTimeException) {
            if (y < 1868) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return is_gregorian_leap(y);
        }

        /**
         * @brief Get the number of days in a month.
         * @param y The ISO proleptic year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         * @throws DateTimeException if y < 1868.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 y, u32 m) throws (DateTimeException) {
            if (y < 1868) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return IsoChronology::days_in_month(y, m);
        }

        /**
         * @brief Get the number of days in a year.
         * @param y The ISO proleptic year.
         * @returns 366 if the year is a leap year, 365 otherwise.
         * @throws DateTimeException if y < 1868.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 y) throws (DateTimeException) {
            if (y < 1868) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return IsoChronology::days_in_year(y);
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The ISO proleptic year.
         * @returns 12
         * @throws DateTimeException if y < 1868.
         */
        [[nodiscard]]
        static constexpr u32 months_in_year(i32 y) throws (DateTimeException) {
            if (y < 1868) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return 12;
        }

        /**
         * @brief Convert a date to an epoch day count.
         * @param y The ISO proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01.
         * @throws DateTimeException if the date is before Meiji (1868-01-25).
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) throws (DateTimeException) {
            i64 e = gregorian_to_epoch_day(y, m, d);
            if (e < MEIJI_START) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return e;
        }

        /**
         * @brief Convert an epoch day count to date components.
         * @param e The number of days since 1970-01-01.
         * @returns The decomposed date components.
         * @throws DateTimeException if e < MEIJI_START.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) throws (DateTimeException) {
            if (e < MEIJI_START) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            }
            return epoch_day_to_gregorian(e);
        }

        /**
         * @brief Determine the Japanese era for a given epoch day.
         * @param epoch_day The number of days since 1970-01-01.
         * @returns The Japanese era (MEIJI, TAISHO, SHOWA, HEISEI, or REIWA).
         * @throws DateTimeException if epoch_day < MEIJI_START.
         */
        [[nodiscard]]
        static constexpr Era era_of(i64 epoch_day) throws (DateTimeException) {
            if (epoch_day < MEIJI_START) {
                throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
            } else if (epoch_day < TAISHO_START) {
                return Era::MEIJI;
            } else if (epoch_day < SHOWA_START) {
                return Era::TAISHO;
            } else if (epoch_day < HEISEI_START) {
                return Era::SHOWA;
            } else if (epoch_day < REIWA_START) {
                return Era::HEISEI;
            }
            return Era::REIWA;
        }

        /**
         * @brief Get the Gregorian start year of a Japanese era.
         * @param era The Japanese era.
         * @returns The ISO proleptic year in which the era begins.
         */
        [[nodiscard]]
        static constexpr i32 era_start_year(Era era) noexcept {
            switch (era) {
                case Era::MEIJI:
                    return 1868;
                case Era::TAISHO:
                    return 1912;
                case Era::SHOWA:
                    return 1926;
                case Era::HEISEI:
                    return 1989;
                case Era::REIWA:
                    return 2019;
                default:
                    System::unreachable();
            }
        }

        /**
         * @brief Get the year within the current era.
         * @param proleptic_year The ISO proleptic year.
         * @param epoch_day The epoch day for era determination.
         * @returns The year-of-era (1-based).
         * @throws DateTimeException if epoch_day < MEIJI_START.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year, i64 epoch_day) throws (DateTimeException) {
            return proleptic_year - era_start_year(era_of(epoch_day)) + 1;
        }

        /**
         * @brief Convert an era and year-of-era to an ISO proleptic year.
         * @param era The Japanese era.
         * @param year_of_era The year within the era (1-based).
         * @returns The ISO proleptic year.
         * @throws DateTimeException if year_of_era < 1.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) throws (DateTimeException) {
            if (year_of_era < 1) {
                throw DateTimeException("JapaneseChronology does not support year-of-era less than 1");
            }
            return era_start_year(era) + year_of_era - 1;
        }

        /**
         * @brief Create a date from ISO proleptic year, month, and day.
         * @param y The ISO proleptic year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if the date is before Meiji (1868-01-25).
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JapaneseChronology> of(i32 y, u32 m, u32 d) throws (DateTimeException);

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01.
         * @returns The date in this chronology.
         * @throws DateTimeException if e < MEIJI_START.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JapaneseChronology> date_epoch_day(i64 e) throws (DateTimeException);

        /**
         * @brief Create a date from an era, year-of-era, month, and day.
         * @param era The Japanese era.
         * @param year_of_era The year within the era (1-based).
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if the date is before Meiji (1868-01-25) or year_of_era < 1.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JapaneseChronology> of(Era era, i32 year_of_era, u32 m, u32 d) throws (DateTimeException);

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<JapaneseChronology> date_now() throws (DateTimeException);
    };

    /**
     * @brief The Minguo (Republic of China) chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) with a year
     * offset. Minguo year 1 = 1912 CE. Proleptic Minguo year = ISO year - 1911.
     */
    class [[nodiscard]] MinguoChronology final {
    public:
        MinguoChronology() = delete;

        /**
         * @enum Era
         * @brief Minguo (Republic of China) calendar era.
         */
        enum class Era: i32 {
            BEFORE_ROC = 0, ///< Before Republic of China (proleptic years <= 0)
            ROC = 1, ///< Republic of China (proleptic years >= 1)
        };

        static constexpr i32 YEAR_OFFSET = 1911;

        /**
         * @brief Returns the chronology identifier.
         * @returns "Minguo"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Minguo"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "roc"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "roc"sv;
        }

        /**
         * @brief Check if a year is a leap year.
         * @param proleptic_year The proleptic Minguo year.
         * @returns true if the corresponding Gregorian year is a leap year.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 proleptic_year) noexcept {
            return is_gregorian_leap(proleptic_year + YEAR_OFFSET);
        }

        /**
         * @brief Get the number of days in a month.
         * @param proleptic_year The proleptic Minguo year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 proleptic_year, u32 m) noexcept {
            return IsoChronology::days_in_month(proleptic_year + YEAR_OFFSET, m);
        }

        /**
         * @brief Get the number of days in a year.
         * @param proleptic_year The proleptic Minguo year.
         * @returns 366 if the year is a leap year, 365 otherwise.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 proleptic_year) noexcept {
            return IsoChronology::days_in_year(proleptic_year + YEAR_OFFSET);
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic Minguo year.
         * @returns 12
         */
        [[nodiscard]]
        static constexpr u32 months_in_year([[maybe_unused]] i32 y) noexcept {
            return 12;
        }

        /**
         * @brief Convert a Minguo date to an epoch day count.
         * @param proleptic_year The proleptic Minguo year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 proleptic_year, u32 m, u32 d) noexcept {
            return gregorian_to_epoch_day(proleptic_year + YEAR_OFFSET, m, d);
        }

        /**
         * @brief Convert an epoch day count to Minguo date components.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The decomposed Minguo date components.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) noexcept {
            auto [y, m, d] = epoch_day_to_gregorian(e);
            return {y - YEAR_OFFSET, m, d};
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic Minguo year.
         * @returns Era::ROC if proleptic_year >= 1, Era::BEFORE_ROC otherwise.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? Era::ROC : Era::BEFORE_ROC;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic Minguo year.
         * @returns The positive year-of-era value.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? proleptic_year : 1 - proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era.
         * @param year_of_era The year within the era.
         * @returns The proleptic Minguo year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) noexcept {
            return era == Era::ROC ? year_of_era : 1 - year_of_era;
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic Minguo year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<MinguoChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<MinguoChronology> date_epoch_day(i64 e) noexcept;

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<MinguoChronology> date_now() noexcept;
    };

    /**
     * @brief The Thai Buddhist chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) with a year
     * offset. Thai Buddhist year 1 = 543 BCE. Thai year = ISO year + 543.
     */
    class [[nodiscard]] ThaiBuddhistChronology final {
    public:
        ThaiBuddhistChronology() = delete;

        /**
         * @enum Era
         * @brief Thai Buddhist calendar era.
         */
        enum class Era: i32 {
            BEFORE_BE = 0, ///< Before Buddhist Era (proleptic years <= 0)
            BE = 1, ///< Buddhist Era (proleptic years >= 1)
        };

        static constexpr i32 YEAR_OFFSET = 543;

        /**
         * @brief Returns the chronology identifier.
         * @returns "ThaiBuddhist"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "ThaiBuddhist"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "buddhist"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "buddhist"sv;
        }

        /**
         * @brief Check if a year is a leap year.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @returns true if the corresponding Gregorian year is a leap year.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 proleptic_year) noexcept {
            return is_gregorian_leap(proleptic_year - YEAR_OFFSET);
        }

        /**
         * @brief Get the number of days in a month.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 proleptic_year, u32 m) noexcept {
            return IsoChronology::days_in_month(proleptic_year - YEAR_OFFSET, m);
        }

        /**
         * @brief Get the number of days in a year.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @returns 366 if the year is a leap year, 365 otherwise.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 proleptic_year) noexcept {
            return IsoChronology::days_in_year(proleptic_year - YEAR_OFFSET);
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic Thai Buddhist year.
         * @returns 12
         */
        [[nodiscard]]
        static constexpr u32 months_in_year([[maybe_unused]] i32 y) noexcept {
            return 12;
        }

        /**
         * @brief Convert a Thai Buddhist date to an epoch day count.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 proleptic_year, u32 m, u32 d) noexcept {
            return gregorian_to_epoch_day(proleptic_year - YEAR_OFFSET, m, d);
        }

        /**
         * @brief Convert an epoch day count to Thai Buddhist date components.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The decomposed Thai Buddhist date components.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) noexcept {
            auto [y, m, d] = epoch_day_to_gregorian(e);
            return {y + YEAR_OFFSET, m, d};
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @returns Era::BE if proleptic_year >= 1, Era::BEFORE_BE otherwise.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? Era::BE : Era::BEFORE_BE;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic Thai Buddhist year.
         * @returns The positive year-of-era value.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? proleptic_year : 1 - proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era.
         * @param year_of_era The year within the era.
         * @returns The proleptic Thai Buddhist year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) noexcept {
            return era == Era::BE ? year_of_era : 1 - year_of_era;
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic Thai Buddhist year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> date_epoch_day(i64 e) noexcept;

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ThaiBuddhistChronology> date_now() noexcept;
    };

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
        i32 proleptic_year; ///< The proleptic year in the chronology (e.g., 2024 for ISO, 1445 for Hijrah).
        u32 mth; ///< The month of the date.
        u32 dy; ///< The day of the month.
        i64 epoch_day; ///< The number of days since 1970-01-01 (Gregorian).
    public:
        using Chronology = Chrono;

        /**
         * @brief Construct a date from year, month, and day components.
         * @param year The proleptic year in the chronology.
         * @param month The month (1-12).
         * @param day The day of the month.
         */
        constexpr ChronoLocalDate(i32 year, u32 month, u32 day):
            proleptic_year{year}, mth{month}, dy{day},
            epoch_day{Chrono::to_epoch_day(year, month, day)} {}

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
            Instant<SystemClock> tp = floor<Days>(SystemClock::now());
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
            return SystemDays{Days{epoch_day}};
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
        constexpr ChronoLocalDate<IsoChronology> to_iso() const noexcept {
            return ChronoLocalDate<IsoChronology>::of_epoch_day(epoch_day);
        }

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
         * @brief Subtract days from this date.
         * @param n The number of days to subtract.
         * @returns A new date offset by -n days.
         */
        [[nodiscard]]
        constexpr ChronoLocalDate minus_days(i64 n) const {
            return of_epoch_day(epoch_day - n);
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
        constexpr StrongOrdering operator==(const ChronoLocalDate<Other>& other) const noexcept {
            return epoch_day == other.to_epoch_day();
        }
    };

    using IsoDate = ChronoLocalDate<IsoChronology>;
    using JulianDate = ChronoLocalDate<JulianChronology>;
    using HijrahDate = ChronoLocalDate<HijrahChronology>;
    using JapaneseDate = ChronoLocalDate<JapaneseChronology>;
    using MinguoDate = ChronoLocalDate<MinguoChronology>;
    using ThaiBuddhistDate = ChronoLocalDate<ThaiBuddhistChronology>;

    using IsoEra = IsoChronology::Era;
    using JulianEra = JulianChronology::Era;
    using HijrahEra = HijrahChronology::Era;
    using JapaneseEra = JapaneseChronology::Era;
    using MinguoEra = MinguoChronology::Era;
    using ThaiBuddhistEra = ThaiBuddhistChronology::Era;

    constexpr IsoDate IsoChronology::of(i32 y, u32 m, u32 d) noexcept {
        return IsoDate(y, m, d);
    }

    constexpr IsoDate IsoChronology::date_epoch_day(i64 e) noexcept {
        return IsoDate::of_epoch_day(e);
    }

    inline IsoDate IsoChronology::date_now() noexcept {
        return IsoDate::now();
    }

    constexpr JulianDate JulianChronology::of(i32 y, u32 m, u32 d) noexcept {
        return JulianDate(y, m, d);
    }
    constexpr JulianDate JulianChronology::date_epoch_day(i64 e) noexcept {
        return JulianDate::of_epoch_day(e);
    }
    inline JulianDate JulianChronology::date_now() noexcept {
        return JulianDate::now();
    }

    constexpr HijrahDate HijrahChronology::of(i32 y, u32 m, u32 d) throws (DateTimeException) {
        if (y < 1) {
            throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
        }
        return HijrahDate(y, m, d);
    }

    constexpr HijrahDate HijrahChronology::date_epoch_day(i64 e) throws (DateTimeException) {
        if (e < HIJRAH_EPOCH) {
            throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
        }
        return HijrahDate::of_epoch_day(e);
    }

    inline HijrahDate HijrahChronology::date_now() throws (DateTimeException) {
        return HijrahDate::now();
    }

    constexpr JapaneseDate JapaneseChronology::of(i32 y, u32 m, u32 d) throws (DateTimeException) {
        if (gregorian_to_epoch_day(y, m, d) < MEIJI_START) {
            throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
        }
        return JapaneseDate(y, m, d);
    }

    constexpr JapaneseDate JapaneseChronology::date_epoch_day(i64 e) throws (DateTimeException) {
        if (e < MEIJI_START) {
            throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
        }
        return JapaneseDate::of_epoch_day(e);
    }

    constexpr JapaneseDate JapaneseChronology::of(Era era, i32 year_of_era, u32 m, u32 d) throws (DateTimeException) {
        return of(proleptic_year(era, year_of_era), m, d);
    }

    inline JapaneseDate JapaneseChronology::date_now() throws (DateTimeException) {
        return JapaneseDate::now();
    }

    constexpr MinguoDate MinguoChronology::of(i32 y, u32 m, u32 d) noexcept {
        return MinguoDate(y, m, d);
    }

    constexpr MinguoDate MinguoChronology::date_epoch_day(i64 e) noexcept {
        return MinguoDate::of_epoch_day(e);
    }

    inline MinguoDate MinguoChronology::date_now() noexcept {
        return MinguoDate::now();
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::of(i32 y, u32 m, u32 d) noexcept {
        return ThaiBuddhistDate(y, m, d);
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::date_epoch_day(i64 e) noexcept {
        return ThaiBuddhistDate::of_epoch_day(e);
    }

    inline ThaiBuddhistDate ThaiBuddhistChronology::date_now() noexcept {
        return ThaiBuddhistDate::now();
    }
}