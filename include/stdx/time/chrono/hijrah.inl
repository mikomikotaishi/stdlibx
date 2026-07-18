#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Hijrah (Islamic tabular) chronology.
 */
export namespace stdx::time::chrono {
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
    private:
        static constexpr i64 HIJRAH_EPOCH = gregorian_to_epoch_day(622, 7, 19); ///< The epoch day of the Hijrah calendar (corresponding to 622-07-19 in the Gregorian calendar).

        /**
         * @internal
         * @enum HijrahMonth
         * @brief The twelve months of the Islamic (Hijrah) calendar.
         */
        enum class HijrahMonth: u8 {
            MUHARRAM = 1, ///< Muharram, the first month of the year on the Islamic calendar
            SAFAR = 2, ///< Safar, the second month of the year on the Islamic calendar
            RABI_AL_AWWAL = 3, ///< Rabi' al-awwal, the third month of the year on the Islamic calendar
            RABI_AL_THANI = 4, ///< Rabi' al-thani, the fourth month of the year on the Islamic calendar
            JUMADA_AL_AWWAL = 5, ///< Jumada al-awwal, the fifth month of the year on the Islamic calendar
            JUMADA_AL_THANI = 6, ///< Jumada al-thani, the sixth month of the year on the Islamic calendar
            RAJAB = 7, ///< Rajab, the seventh month of the year on the Islamic calendar
            SHABAN = 8, ///< Sha'ban, the eighth month of the year on the Islamic calendar
            RAMADAN = 9, ///< Ramadan, the ninth month of the year on the Islamic calendar
            SHAWWAL = 10, ///< Shawwal, the tenth month of the year on the Islamic calendar
            DHU_AL_QIDAH = 11, ///< Dhu al-Qi'dah, the eleventh month of the year on the Islamic calendar
            DHU_AL_HIJJAH = 12, ///< Dhu al-Hijjah, the twelfth month of the year on the Islamic calendar
        };

        /**
         * @internal
         * @brief Check if a Hijrah year is a leap year.
         * @param y The proleptic year.
         * @returns true if the year is a leap year, false otherwise.
         */
        [[nodiscard]]
        static constexpr bool is_hijrah_leap(i32 y) noexcept {
            return (14 + 11 * static_cast<i64>(y)) % 30 < 11;
        }

        /**
         * @internal
         * @brief Get the number of days in a month for a given Hijrah year and month.
         * @param y The proleptic year.
         * @param m The month (1-12).
         * @returns The number of days in the month.
         */
        [[nodiscard]]
        static constexpr u32 hijrah_days_in_month(i32 y, u32 m) noexcept {
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
        static constexpr i32 hijrah_days_in_year(i32 y) noexcept {
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
        static constexpr i64 hijrah_to_epoch_day(i32 y, u32 m, u32 d) noexcept {
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
        static constexpr DateComponents epoch_day_to_hijrah(i64 epoch_day) noexcept {
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
    public:
        HijrahChronology() = delete("HijrahChronology is a static utility class and cannot be instantiated.");

        /**
         * @enum Era
         * @brief Hijrah (Islamic) calendar era.
         */
        enum class Era: i32 {
            AH = 1, ///< Anno Hegirae (in the year of the Hijrah, proleptic years >= 1)
        };

        using enum HijrahMonth;

        /**
         * @brief Returns the chronology identifier.
         * @returns "Hijrah-umalqura"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Hijrah-umalqura";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "islamic-umalqura"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "islamic-umalqura";
        }

        /**
         * @brief Check if a year is a leap year in the Hijrah calendar.
         * @param y The proleptic Hijrah year.
         * @returns true if the year is a leap year.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr bool is_leap_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr u32 days_in_month(i32 y, u32 m) {
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
        THROWS(DateTimeException)
        static constexpr i32 days_in_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr u32 months_in_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) {
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
        THROWS(DateTimeException)
        static constexpr DateComponents from_epoch_day(i64 e) {
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
        THROWS(DateTimeException)
        static constexpr Era era_of(i32 proleptic_year) {
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
        THROWS(DateTimeException)
        static constexpr i32 year_of_era(i32 proleptic_year) {
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
        THROWS(DateTimeException)
        static constexpr i32 proleptic_year([[maybe_unused]] Era era, i32 year_of_era) {
            if (year_of_era < 1) {
                throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
            }
            return year_of_era;
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr bool is_leap_year(Year y) {
            return is_leap_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr u32 days_in_month(Year y, Month m) {
            return days_in_month(
                static_cast<i32>(y),
                static_cast<u32>(m)
            );
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i32 days_in_year(Year y) {
            return days_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr u32 months_in_year(Year y) {
            return months_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i64 to_epoch_day(Year y, Month m, Day d) {
            return to_epoch_day(
                static_cast<i32>(y),
                static_cast<u32>(m),
                static_cast<u32>(d)
            );
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr Era era_of(Year y) {
            return era_of(static_cast<i32>(y));
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i32 year_of_era(Year y) {
            return year_of_era(static_cast<i32>(y));
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i32 proleptic_year([[maybe_unused]] Era era, Year year_of_era) {
            return proleptic_year(era, static_cast<i32>(year_of_era));
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
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<HijrahChronology> of(i32 y, u32 m, u32 d);

        /**
         * @brief Create a date from a named Islamic month.
         * @param y The proleptic Hijrah year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<HijrahChronology> of(i32 y, HijrahMonth m, u32 d);

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic Hijrah year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if y < 1.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<HijrahChronology> of(Year y, Month m, Day d);

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         * @throws DateTimeException if e < HIJRAH_EPOCH.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<HijrahChronology> date_epoch_day(i64 e);

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static ChronoLocalDate<HijrahChronology> date_now();
    };

    using HijrahDate = ChronoLocalDate<HijrahChronology>;
    using HijrahEra = HijrahChronology::Era;

    constexpr HijrahDate HijrahChronology::of(i32 y, u32 m, u32 d) {
        if (y < 1) {
            throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
        }
        return HijrahDate(y, m, d);
    }

    constexpr HijrahDate HijrahChronology::of(i32 y, HijrahMonth m, u32 d) {
        return of(y, Ops::to_underlying(m), d);
    }

    constexpr HijrahDate HijrahChronology::of(Year y, Month m, Day d) {
        if (static_cast<i32>(y) < 1) {
            throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
        }
        return HijrahDate(y, m, d);
    }

    constexpr HijrahDate HijrahChronology::date_epoch_day(i64 e) {
        if (e < HIJRAH_EPOCH) {
            throw DateTimeException("HijrahChronology does not support dates before year 1 AH");
        }
        return HijrahDate::of_epoch_day(e);
    }

    inline HijrahDate HijrahChronology::date_now() {
        return HijrahDate::now();
    }
}
