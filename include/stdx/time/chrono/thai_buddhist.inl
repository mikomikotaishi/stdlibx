#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Thai Buddhist chronology.
 */
export namespace stdx::time::chrono {
    /**
     * @brief The Thai Buddhist chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) with a year
     * offset. Thai Buddhist year 1 = 543 BCE. Thai year = ISO year + 543.
     */
    class ThaiBuddhistChronology final {
    public:
        ThaiBuddhistChronology() = delete("ThaiBuddhistChronology is a static utility class and cannot be instantiated.");

        /**
         * @enum Era
         * @brief Thai Buddhist calendar era.
         */
        enum class Era: i32 {
            BEFORE_BE = 0, ///< Before Buddhist Era (proleptic years <= 0)
            BE = 1, ///< Buddhist Era (proleptic years >= 1)
        };

        using enum GregorianMonth;

        static constexpr i32 YEAR_OFFSET = 543;

        /**
         * @brief Returns the chronology identifier.
         * @returns "ThaiBuddhist"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "ThaiBuddhist";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "buddhist"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "buddhist";
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
         * @param y The proleptic Thai Buddhist year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from a named Gregorian month.
         * @param y The proleptic Thai Buddhist year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> of(i32 y, GregorianMonth m, u32 d) noexcept;

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic Thai Buddhist year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> of(Year y, Month m, Day d) noexcept;

        /**
         * @brief Create a date from era, year-of-era, month, and day.
         * @param era The calendar era (BEFORE_BE or BE).
         * @param year_of_era The positive year-of-era value.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<ThaiBuddhistChronology> of(Era era, i32 year_of_era, u32 m, u32 d) noexcept;

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

    using ThaiBuddhistDate = ChronoLocalDate<ThaiBuddhistChronology>;
    using ThaiBuddhistEra = ThaiBuddhistChronology::Era;

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::of(i32 y, u32 m, u32 d) noexcept {
        return ThaiBuddhistDate(y, m, d);
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::of(i32 y, GregorianMonth m, u32 d) noexcept {
        return of(y, static_cast<u32>(m), d);
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::of(Year y, Month m, Day d) noexcept {
        return ThaiBuddhistDate(y, m, d);
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::of(ThaiBuddhistChronology::Era era, i32 yoe, u32 m, u32 d) noexcept {
        return ThaiBuddhistDate(proleptic_year(era, yoe), m, d);
    }

    constexpr ThaiBuddhistDate ThaiBuddhistChronology::date_epoch_day(i64 e) noexcept {
        return ThaiBuddhistDate::of_epoch_day(e);
    }

    inline ThaiBuddhistDate ThaiBuddhistChronology::date_now() noexcept {
        return ThaiBuddhistDate::now();
    }
}
