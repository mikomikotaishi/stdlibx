#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Hebrew (Jewish) lunisolar chronology.
 */
export namespace stdx::time::chrono {
    /**
     * @class HebrewChronology
     * @brief The Hebrew (Jewish) lunisolar chronology.
     *
     * Uses the modern (post-Hillel II) fixed arithmetic Hebrew calendar with
     * a 19-year Metonic cycle and the four dehiyyot postponement rules.
     * Year 1 AM corresponds to 1 Tishri of year 1 ≈ 3761 BCE (proleptic Julian).
     *
     * Civil month numbering is used: 1 = Tishri (start of the civil year),
     * with Adar I and Adar II inserted as months 6 and 7 in leap years.
     * Year length is 353, 354, or 355 days in regular years and 383, 384,
     * or 385 days in leap years.
     */
    class [[nodiscard]] HebrewChronology final {
    private:
        static inline constexpr i64 HEBREW_EPOCH_OFFSET = 2092591; ///< Offset such that epoch_day = hebrew_elapsed_days(y) - HEBREW_EPOCH_OFFSET gives 1 Tishri of year y.

        /**
         * @internal
         * @brief Check if a Hebrew year is a leap year (i.e., contains 13 months).
         * @param y The proleptic Hebrew year.
         * @returns true if the year is a leap year, false otherwise.
         *
         * Leap years in the Metonic 19-year cycle occupy positions
         * 3, 6, 8, 11, 14, 17, and 19.
         */
        [[nodiscard]]
        static constexpr bool is_hebrew_leap(i32 y) noexcept {
            return ((7 * static_cast<i64>(y) + 1) % 19) < 7;
        }

        /**
         * @internal
         * @brief Compute elapsed days from the calendar's reference origin to
         * the start of Rosh Hashanah of Hebrew year @p y.
         * @param y The proleptic Hebrew year.
         * @returns Days since the molad-tohu reference (1-based).
         *
         * Applies the four dehiyyot (postponement) rules:
         *  - Molad Zaken: postpone if the molad falls at or after noon.
         *  - GaTaRaD: postpone Tuesday morning molad in a non-leap year.
         *  - BeTUTeKaPoT: postpone Monday morning molad in a year following a leap year.
         *  - Lo ADU Rosh: never start the year on Sunday, Wednesday, or Friday.
         */
        [[nodiscard]]
        static constexpr i64 hebrew_elapsed_days(i32 y) noexcept {
            i64 year = static_cast<i64>(y);
            i64 months_elapsed = 235 * ((year - 1) / 19)
                + 12 * ((year - 1) % 19)
                + (7 * ((year - 1) % 19) + 1) / 19;
            i64 parts_elapsed = 204 + 793 * (months_elapsed % 1080);
            i64 hours_elapsed = 5 + 12 * months_elapsed
                + 793 * (months_elapsed / 1080)
                + parts_elapsed / 1080;
            i64 day = 1 + 29 * months_elapsed + hours_elapsed / 24;
            i64 parts = (hours_elapsed % 24) * 1080 + (parts_elapsed % 1080);

            bool postpone = parts >= 19440
                || (day % 7 == 2 && parts >= 9924 && !is_hebrew_leap(y))
                || (day % 7 == 1 && parts >= 16789 && is_hebrew_leap(y - 1));
            i64 alt_day = day + (postpone ? 1 : 0);
            i64 wd = alt_day % 7;
            if (wd == 0 || wd == 3 || wd == 5) {
                return alt_day + 1;
            }
            return alt_day;
        }

        /**
         * @internal
         * @brief Compute the epoch day of 1 Tishri of Hebrew year @p y.
         * @param y The proleptic Hebrew year.
         * @returns Days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static constexpr i64 hebrew_new_year_epoch_day(i32 y) noexcept {
            return hebrew_elapsed_days(y) - HEBREW_EPOCH_OFFSET;
        }

        /**
         * @internal
         * @brief Get the length of a Hebrew year in days.
         * @param y The proleptic Hebrew year.
         * @returns 353, 354, or 355 in a regular year; 383, 384, or 385 in a leap year.
         */
        [[nodiscard]]
        static constexpr i32 hebrew_year_length(i32 y) noexcept {
            return static_cast<i32>(hebrew_elapsed_days(y + 1) - hebrew_elapsed_days(y));
        }

        /**
         * @internal
         * @brief Get the number of days in a Hebrew month.
         * @param y The proleptic Hebrew year.
         * @param m The month (1-12 in regular years, 1-13 in leap years).
         * @returns 29 or 30.
         *
         * Civil month numbering: 1=Tishri, 2=Cheshvan, 3=Kislev, 4=Tevet,
         * 5=Shevat, 6=Adar (or Adar I in leap years), 7=Adar II (leap only)
         * or Nisan, ..., 12/13=Elul.
         */
        [[nodiscard]]
        static constexpr u32 hebrew_days_in_month(i32 y, u32 m) noexcept {
            bool leap = is_hebrew_leap(y);
            i32 yl = hebrew_year_length(y);
            switch (m) {
                case 1:
                    return 30; // Tishri
                case 2:
                    return (yl == 355 || yl == 385) ? 30 : 29; // Cheshvan
                case 3:
                    return (yl == 353 || yl == 383) ? 29 : 30; // Kislev
                case 4:
                    return 29; // Tevet
                case 5:
                    return 30; // Shevat
                case 6:
                    return leap ? 30 : 29; // Adar I (leap) or Adar (regular)
                case 7:
                    return leap ? 29 : 30; // Adar II (leap) or Nisan (regular)
                case 8:
                    return leap ? 30 : 29; // Nisan (leap) or Iyyar (regular)
                case 9:
                    return leap ? 29 : 30; // Iyyar (leap) or Sivan (regular)
                case 10:
                    return leap ? 30 : 29; // Sivan (leap) or Tammuz (regular)
                case 11:
                    return leap ? 29 : 30; // Tammuz (leap) or Av (regular)
                case 12:
                    return leap ? 30 : 29; // Av (leap) or Elul (regular)
                case 13:
                    return 29; // Elul (leap only)
                default:
                    Ops::unreachable();
            }
        }

        /**
         * @internal
         * @brief Convert a Hebrew date to epoch day.
         * @param y The proleptic Hebrew year.
         * @param m The month (civil numbering, 1-12 or 1-13).
         * @param d The day of the month.
         * @returns Days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static constexpr i64 hebrew_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
            i64 result = hebrew_new_year_epoch_day(y);
            for (u32 mm = 1; mm < m; ++mm) {
                result += hebrew_days_in_month(y, mm);
            }
            return result + static_cast<i64>(d) - 1;
        }

        /**
         * @internal
         * @brief Convert an epoch day to Hebrew date components.
         * @param e Days since 1970-01-01 (Gregorian).
         * @returns The decomposed Hebrew date components.
         */
        [[nodiscard]]
        static constexpr DateComponents epoch_day_to_hebrew(i64 e) noexcept {
            i32 y = static_cast<i32>((e + HEBREW_EPOCH_OFFSET) / 366 + 1);
            while (hebrew_new_year_epoch_day(y + 1) <= e) {
                ++y;
            }
            while (hebrew_new_year_epoch_day(y) > e) {
                --y;
            }
            i64 day_in_year = e - hebrew_new_year_epoch_day(y);
            u32 m = 1;
            u32 max_m = is_hebrew_leap(y) ? 13 : 12;
            while (m <= max_m) {
                i64 dm = static_cast<i64>(hebrew_days_in_month(y, m));
                if (day_in_year < dm) {
                    break;
                }
                day_in_year -= dm;
                ++m;
            }
            return {y, m, static_cast<u32>(day_in_year + 1)};
        }
    public:
        HebrewChronology() = delete;

        /**
         * @enum Era
         * @brief Hebrew calendar era.
         */
        enum class Era: i32 {
            AM = 1, ///< Anno Mundi (in the year of the world, proleptic years >= 1)
        };

        /**
         * @brief Returns the chronology identifier.
         * @returns "Hebrew"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Hebrew";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "hebrew"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "hebrew";
        }

        /**
         * @brief Check if a Hebrew year is a leap year.
         * @param y The proleptic Hebrew year.
         * @returns true if the year contains 13 months.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr bool is_leap_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return is_hebrew_leap(y);
        }

        /**
         * @brief Get the number of days in a month.
         * @param y The proleptic Hebrew year.
         * @param m The month (1-12 regular, 1-13 leap).
         * @returns The number of days in the month (29 or 30).
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr u32 days_in_month(i32 y, u32 m) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return hebrew_days_in_month(y, m);
        }

        /**
         * @brief Get the number of days in a year.
         * @param y The proleptic Hebrew year.
         * @returns 353/354/355 for regular years, 383/384/385 for leap years.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr i32 days_in_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return hebrew_year_length(y);
        }

        /**
         * @brief Get the number of months in a year.
         * @param y The proleptic Hebrew year.
         * @returns 12 in a regular year, 13 in a leap year.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr u32 months_in_year(i32 y) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return is_hebrew_leap(y) ? 13 : 12;
        }

        /**
         * @brief Convert a Hebrew date to an epoch day count.
         * @param y The proleptic Hebrew year.
         * @param m The month (1-12 regular, 1-13 leap).
         * @param d The day of the month.
         * @returns The number of days since 1970-01-01 (Gregorian).
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) throws (DateTimeException) {
            if (y < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return hebrew_to_epoch_day(y, m, d);
        }

        /**
         * @brief Convert an epoch day count to Hebrew date components.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The decomposed Hebrew date components.
         * @throws DateTimeException if e is before 1 Tishri 1 AM.
         */
        [[nodiscard]]
        static constexpr DateComponents from_epoch_day(i64 e) throws (DateTimeException) {
            if (e < hebrew_new_year_epoch_day(1)) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return epoch_day_to_hebrew(e);
        }

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic Hebrew year.
         * @returns Era::AM.
         * @throws DateTimeException if proleptic_year < 1.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) throws (DateTimeException) {
            if (proleptic_year < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return Era::AM;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic Hebrew year.
         * @returns The year-of-era (equal to the proleptic year for Hebrew).
         * @throws DateTimeException if proleptic_year < 1.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) throws (DateTimeException) {
            if (proleptic_year < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return proleptic_year;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era (always AM).
         * @param year_of_era The year within the era.
         * @returns The proleptic year (equal to year_of_era for Hebrew).
         * @throws DateTimeException if year_of_era < 1.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year([[maybe_unused]] Era era, i32 year_of_era) throws (DateTimeException) {
            if (year_of_era < 1) {
                throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
            }
            return year_of_era;
        }

        [[nodiscard]]
        static constexpr bool is_leap_year(Year y) throws (DateTimeException) {
            return is_leap_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr u32 days_in_month(Year y, Month m) throws (DateTimeException) {
            return days_in_month(
                static_cast<i32>(y),
                static_cast<u32>(m)
            );
        }

        [[nodiscard]]
        static constexpr i32 days_in_year(Year y) throws (DateTimeException) {
            return days_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr u32 months_in_year(Year y) throws (DateTimeException) {
            return months_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i64 to_epoch_day(Year y, Month m, Day d) throws (DateTimeException) {
            return to_epoch_day(
                static_cast<i32>(y),
                static_cast<u32>(static_cast<Month>(m)),
                static_cast<u32>(d)
            );
        }

        [[nodiscard]]
        static constexpr Era era_of(Year y) throws (DateTimeException) {
            return era_of(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 year_of_era(Year y) throws (DateTimeException) {
            return year_of_era(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 proleptic_year([[maybe_unused]] Era era, Year year_of_era) throws (DateTimeException) {
            return proleptic_year(era, static_cast<i32>(year_of_era));
        }

        /**
         * @brief Create a date from year, month, and day components.
         * @param y The proleptic Hebrew year.
         * @param m The month (1-12 regular, 1-13 leap).
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<HebrewChronology> of(i32 y, u32 m, u32 d) throws (DateTimeException);

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic Hebrew year.
         * @param m The month (1-12 regular, 1-13 leap). Note: month 13 is
         *          outside std::chrono::month's ok() range.
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<HebrewChronology> of(Year y, Month m, Day d) throws (DateTimeException);

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         * @throws DateTimeException if e is before 1 Tishri 1 AM.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<HebrewChronology> date_epoch_day(i64 e) throws (DateTimeException);

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<HebrewChronology> date_now() throws (DateTimeException);
    };

    using HebrewDate = ChronoLocalDate<HebrewChronology>;
    using HebrewEra = HebrewChronology::Era;

    constexpr HebrewDate HebrewChronology::of(i32 y, u32 m, u32 d) throws (DateTimeException) {
        if (y < 1) {
            throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
        }
        return HebrewDate(y, m, d);
    }

    constexpr HebrewDate HebrewChronology::of(Year y, Month m, Day d) throws (DateTimeException) {
        if (static_cast<i32>(y) < 1) {
            throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
        }
        return HebrewDate(y, m, d);
    }

    constexpr HebrewDate HebrewChronology::date_epoch_day(i64 e) throws (DateTimeException) {
        if (e < hebrew_new_year_epoch_day(1)) {
            throw DateTimeException("HebrewChronology does not support dates before year 1 AM");
        }
        return HebrewDate::of_epoch_day(e);
    }

    inline HebrewDate HebrewChronology::date_now() throws (DateTimeException) {
        return HebrewDate::now();
    }
}
