#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Minguo (Republic of China) chronology.
 */
export namespace stdx::time::chrono {
    /**
     * @brief The Minguo (Republic of China) chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) with a year
     * offset. Minguo year 1 = 1912 CE. Proleptic Minguo year = ISO year - 1911.
     */
    class [[nodiscard]] MinguoChronology final {
    public:
        MinguoChronology() = delete("MinguoChronology is a static utility class and cannot be instantiated.");

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
            return "Minguo";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "roc"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "roc";
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
         * @param y The proleptic Minguo year.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<MinguoChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic Minguo year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<MinguoChronology> of(Year y, Month m, Day d) noexcept;

        /**
         * @brief Create a date from era, year-of-era, month, and day.
         * @param era The calendar era (BEFORE_ROC or ROC).
         * @param year_of_era The positive year-of-era value.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<MinguoChronology> of(Era era, i32 year_of_era, u32 m, u32 d) noexcept;

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

    using MinguoDate = ChronoLocalDate<MinguoChronology>;
    using MinguoEra = MinguoChronology::Era;

    constexpr MinguoDate MinguoChronology::of(i32 y, u32 m, u32 d) noexcept {
        return MinguoDate(y, m, d);
    }

    constexpr MinguoDate MinguoChronology::of(Year y, Month m, Day d) noexcept {
        return MinguoDate(y, m, d);
    }

    constexpr MinguoDate MinguoChronology::of(MinguoChronology::Era era, i32 yoe, u32 m, u32 d) noexcept {
        return MinguoDate(proleptic_year(era, yoe), m, d);
    }

    constexpr MinguoDate MinguoChronology::date_epoch_day(i64 e) noexcept {
        return MinguoDate::of_epoch_day(e);
    }

    inline MinguoDate MinguoChronology::date_now() noexcept {
        return MinguoDate::now();
    }
}
