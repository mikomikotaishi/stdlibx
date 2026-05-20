#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Proleptic Julian chronology.
 */
export namespace stdx::time::chrono {
    /**
     * @class JulianChronology
     * @brief The proleptic Julian chronology.
     *
     * Uses the Julian calendar rules: every 4th year is a leap year,
     * with no century correction. This is the calendar used in Europe
     * before the Gregorian reform of 1582-10-15.
     */
    class [[nodiscard]] JulianChronology final {
    private:
        /**
         * @internal
         * @brief Check if a Julian year is a leap year.
         * @param y The proleptic year.
         * @returns true if the year is a leap year, false otherwise.
         */
        [[nodiscard]]
        static constexpr bool is_julian_leap(i32 y) noexcept {
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
        static constexpr i64 julian_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
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
        static constexpr DateComponents epoch_day_to_julian(i64 z) noexcept {
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
            return "Julian";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "julian"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "julian";
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

        [[nodiscard]]
        static constexpr bool is_leap_year(Year y) noexcept {
            return is_leap_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr u32 days_in_month(Year y, Month m) noexcept {
            return days_in_month(
                static_cast<i32>(y),
                static_cast<u32>(m)
            );
        }

        [[nodiscard]]
        static constexpr i32 days_in_year(Year y) noexcept {
            return days_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr u32 months_in_year(Year y) noexcept {
            return months_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i64 to_epoch_day(Year y, Month m, Day d) noexcept {
            return to_epoch_day(
                static_cast<i32>(y),
                static_cast<u32>(m),
                static_cast<u32>(d)
            );
        }

        [[nodiscard]]
        static constexpr Era era_of(Year y) noexcept {
            return era_of(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 year_of_era(Year y) noexcept {
            return year_of_era(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, Year year_of_era) noexcept {
            return proleptic_year(era, static_cast<i32>(year_of_era));
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
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JulianChronology> of(Year y, Month m, Day d) noexcept;

        /**
         * @brief Create a date from era, year-of-era, month, and day.
         * @param era The calendar era (BC or AD).
         * @param year_of_era The positive year-of-era value.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<JulianChronology> of(Era era, i32 year_of_era, u32 m, u32 d) noexcept;

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

    using JulianDate = ChronoLocalDate<JulianChronology>;
    using JulianEra = JulianChronology::Era;

    constexpr JulianDate JulianChronology::of(i32 y, u32 m, u32 d) noexcept {
        return JulianDate(y, m, d);
    }

    constexpr JulianDate JulianChronology::of(Year y, Month m, Day d) noexcept {
        return JulianDate(y, m, d);
    }

    constexpr JulianDate JulianChronology::of(JulianChronology::Era era, i32 yoe, u32 m, u32 d) noexcept {
        return JulianDate(proleptic_year(era, yoe), m, d);
    }

    constexpr JulianDate JulianChronology::date_epoch_day(i64 e) noexcept {
        return JulianDate::of_epoch_day(e);
    }

    inline JulianDate JulianChronology::date_now() noexcept {
        return JulianDate::now();
    }
}
