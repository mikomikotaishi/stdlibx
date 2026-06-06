#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief ISO-8601 (proleptic Gregorian) chronology.
 */
export namespace stdx::time::chrono {
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
            return "ISO";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "iso8601"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "iso8601";
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
         * @brief Determine the era for a given epoch day.
         * @param epoch_day The number of days since 1970-01-01.
         * @returns Era::CE if the corresponding proleptic year is >= 1, Era::BCE otherwise.
         *
         * Overload required by ChronologyTextLike so DateTimeFormatter
         * can determine the era from a date's epoch_day uniformly across
         * chronologies.
         */
        [[nodiscard]]
        static constexpr Era era_of(i64 epoch_day) noexcept {
            return epoch_day_to_gregorian(epoch_day).proleptic_year >= 1 ? Era::CE: Era::BCE;
        }

        /**
         * @brief Get the year-of-era from a proleptic year and epoch day.
         * @param proleptic_year The proleptic year.
         * @returns The positive year-of-era value.
         *
         * Overload required by ChronologyTextLike. The epoch_day argument
         * is unused for ISO since the era is fully determined by the
         * proleptic year.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year, [[maybe_unused]] i64 epoch_day) noexcept {
            return year_of_era(proleptic_year);
        }

        /**
         * @brief English name of an era.
         * @param era The era.
         * @param style The desired textual style.
         * @returns "CE"/"Common Era"/"C" or "BCE"/"Before Common Era"/"B".
         */
        [[nodiscard]]
        static constexpr StringView era_name(Era era, DateTextLength style) noexcept {
            switch (style) {
                case DateTextLength::FULL:
                    return era == Era::CE ? "Common Era" : "Before Common Era";
                case DateTextLength::SHORT:
                    return era == Era::CE ? "CE" : "BCE";
                case DateTextLength::NARROW:
                    return era == Era::CE ? "C" : "B";
                default:
                    Ops::unreachable();
            }
        }

        /**
         * @brief English name of a month.
         * @param year Unused for ISO (no leap-year-dependent month names).
         * @param month The month (1-12).
         * @param style The desired textual style.
         * @returns The month name in the requested style.
         * @throws DateTimeException if @p month is not in [1, 12].
         */
        [[nodiscard]]
        static constexpr StringView month_name(
            [[maybe_unused]] i32 year,
            u32 month,
            DateTextLength style
        ) throws (DateTimeException) {
            static constexpr Array<StringView, 12> LONG_NAMES = {
                "January", "February", "March", "April",
                "May", "June", "July", "August",
                "September", "October", "November", "December",
            };
            static constexpr Array<StringView, 12> SHORT_NAMES = {
                "Jan", "Feb", "Mar", "Apr",
                "May", "Jun", "Jul", "Aug",
                "Sep", "Oct", "Nov", "Dec",
            };
            static constexpr Array<StringView, 12> NARROW_NAMES = {
                "J", "F", "M", "A",
                "M", "J", "J", "A",
                "S", "O", "N", "D",
            };
            if (month < 1 || month > 12) {
                throw DateTimeException("ISO month_name: month out of range [1, 12]");
            }
            const u32 idx = month - 1;
            switch (style) {
                case DateTextLength::FULL:
                    return LONG_NAMES[idx];
                case DateTextLength::SHORT:
                    return SHORT_NAMES[idx];
                case DateTextLength::NARROW:
                    return NARROW_NAMES[idx];
                default:
                    Ops::unreachable();
            }
        }

        /**
         * @brief Parse an era name back into an Era value.
         * @param text The text to match.
         * @param style The style the text is expected to be in.
         * @returns The matched era, or empty Optional if @p text does
         * not match any era name in the requested style.
         */
        [[nodiscard]]
        static constexpr Optional<Era> parse_era(StringView text, DateTextLength style) noexcept {
            if (text == era_name(Era::CE, style)) {
                return Era::CE;
            }
            if (text == era_name(Era::BCE, style)) {
                return Era::BCE;
            }
            return nullopt;
        }

        /**
         * @brief Parse a month name back into a 1-based month index.
         * @param text The text to match.
         * @param year Unused for ISO.
         * @param style The style the text is expected to be in.
         * @returns The 1-based month, or empty Optional if no match.
         *
         * Narrow style is ambiguous (e.g. "J" matches January / June /
         * July) and currently returns the first match.
         */
        [[nodiscard]]
        static constexpr Optional<u32> parse_month(
            StringView text,
            [[maybe_unused]] i32 year,
            DateTextLength style
        ) noexcept {
            for (u32 m = 1; m <= 12; ++m) {
                if (text == month_name(0, m, style)) {
                    return m;
                }
            }
            return nullopt;
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
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<IsoChronology> of(Year y, Month m, Day d) noexcept;

        /**
         * @brief Create a date from era, year-of-era, month, and day.
         * @param era The calendar era (BCE or CE).
         * @param year_of_era The positive year-of-era value.
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static constexpr ChronoLocalDate<IsoChronology> of(Era era, i32 year_of_era, u32 m, u32 d) noexcept;

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

    using IsoDate = ChronoLocalDate<IsoChronology>;
    using IsoEra = IsoChronology::Era;

    constexpr IsoDate IsoChronology::of(i32 y, u32 m, u32 d) noexcept {
        return IsoDate(y, m, d);
    }

    constexpr IsoDate IsoChronology::of(Year y, Month m, Day d) noexcept {
        return IsoDate(y, m, d);
    }

    constexpr IsoDate IsoChronology::of(IsoChronology::Era era, i32 yoe, u32 m, u32 d) noexcept {
        return IsoDate(proleptic_year(era, yoe), m, d);
    }

    constexpr IsoDate IsoChronology::date_epoch_day(i64 e) noexcept {
        return IsoDate::of_epoch_day(e);
    }

    inline IsoDate IsoChronology::date_now() noexcept {
        return IsoDate::now();
    }
}
