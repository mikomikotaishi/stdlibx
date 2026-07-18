#pragma once

/**
 * @namespace stdx::time::chrono
 * @brief Japanese Imperial chronology.
 */
export namespace stdx::time::chrono {
    /**
     * @class JapaneseChronology
     * @brief The Japanese Imperial chronology.
     *
     * Uses the same calendar rules as ISO (Gregorian) but with
     * Japanese Imperial era-based year numbering. Supports eras
     * from Meiji (1868) through Reiwa (2019-).
     */
    class [[nodiscard]] JapaneseChronology final {
    private:
        static constexpr i64 MEIJI_START = gregorian_to_epoch_day(1868, 1, 25); ///< The epoch day of the Meiji era (corresponding to 1868-01-25 in the Gregorian calendar).
        static constexpr i64 TAISHO_START = gregorian_to_epoch_day(1912, 7, 30); ///< The epoch day of the Taisho era (corresponding to 1912-07-30 in the Gregorian calendar).
        static constexpr i64 SHOWA_START = gregorian_to_epoch_day(1926, 12, 25); ///< The epoch day of the Showa era (corresponding to 1926-12-25 in the Gregorian calendar).
        static constexpr i64 HEISEI_START = gregorian_to_epoch_day(1989, 1, 8); ///< The epoch day of the Heisei era (corresponding to 1989-01-08 in the Gregorian calendar).
        static constexpr i64 REIWA_START = gregorian_to_epoch_day(2019, 5, 1); ///< The epoch day of the Reiwa era (corresponding to 2019-05-01 in the Gregorian calendar).

        /**
         * @internal
         * @enum JapaneseMonth
         * @brief Traditional Japanese month names (和風月名, wafū getsumei).
         */
        enum class JapaneseMonth: u8 {
            MUTSUKI = 1, ///< Mutsuki, the first month of the year (January)
            KISARAGI = 2, ///< Kisaragi, the second month of the year (February)
            YAYOI = 3, ///< Yayoi, the third month of the year (March)
            UZUKI = 4, ///< Uzuki, the fourth month of the year (April)
            SATSUKI = 5, ///< Satsuki, the fifth month of the year (May)
            MINAZUKI = 6, ///< Minazuki, the sixth month of the year (June)
            FUMIZUKI = 7, ///< Fumizuki, the seventh month of the year (July)
            HAZUKI = 8, ///< Hazuki, the eighth month of the year (August)
            NAGATSUKI = 9, ///< Nagatsuki, the ninth month of the year (September)
            KANNAZUKI = 10, ///< Kannazuki, the tenth month of the year (October)
            SHIMOTSUKI = 11, ///< Shimotsuki, the eleventh month of the year (November)
            SHIWASU = 12, ///< Shiwasu, the twelfth month of the year (December)
        };
    public:
        JapaneseChronology() = delete("JapaneseChronology is a static utility class and cannot be instantiated.");

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

        using enum JapaneseMonth;

        /**
         * @brief Returns the chronology identifier.
         * @returns "Japanese"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Japanese";
        }

        /**
         * @brief Returns the calendar type.
         * @returns "japanese"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "japanese";
        }

        /**
         * @brief Check if a year is a leap year (uses Gregorian rules).
         * @param y The ISO proleptic year.
         * @returns true if the year is a leap year.
         * @throws DateTimeException if y < 1868.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr bool is_leap_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr u32 days_in_month(i32 y, u32 m) {
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
        THROWS(DateTimeException)
        static constexpr i32 days_in_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr u32 months_in_year(i32 y) {
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
        THROWS(DateTimeException)
        static constexpr i64 to_epoch_day(i32 y, u32 m, u32 d) {
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
        THROWS(DateTimeException)
        static constexpr DateComponents from_epoch_day(i64 e) {
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
        THROWS(DateTimeException)
        static constexpr Era era_of(i64 epoch_day) {
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
            }
            Ops::unreachable();
        }

        /**
         * @brief Get the year within the current era.
         * @param proleptic_year The ISO proleptic year.
         * @param epoch_day The epoch day for era determination.
         * @returns The year-of-era (1-based).
         * @throws DateTimeException if epoch_day < MEIJI_START.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i32 year_of_era(i32 proleptic_year, i64 epoch_day) {
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
        THROWS(DateTimeException)
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) {
            if (year_of_era < 1) {
                throw DateTimeException("JapaneseChronology does not support year-of-era less than 1");
            }
            return era_start_year(era) + year_of_era - 1;
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
        static constexpr i32 year_of_era(Year y, i64 epoch_day) {
            return year_of_era(static_cast<i32>(y), epoch_day);
        }

        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr i32 proleptic_year(Era era, Year year_of_era) {
            return proleptic_year(era, static_cast<i32>(year_of_era));
        }

        /**
         * @brief English/native name of a Japanese era.
         * @param era The era.
         * @param style The desired textual style.
         * @returns The era name.
         * @details
         * NARROW returns the native kanji (e.g. "令和");
         * SHORT returns the single-letter Latin abbreviation (e.g. "R");
         * FULL returns the romaji name (e.g. "Reiwa").
         */
        [[nodiscard]]
        static constexpr StringView era_name(Era era, DateTextLength style) noexcept {
            switch (era) {
                case Era::MEIJI:
                    switch (style) {
                        case DateTextLength::FULL:
                            return "Meiji";
                        case DateTextLength::SHORT:
                            return "M";
                        case DateTextLength::NARROW:
                            return "明治";
                    }
                    Ops::unreachable();
                case Era::TAISHO:
                    switch (style) {
                        case DateTextLength::FULL:
                            return "Taisho";
                        case DateTextLength::SHORT:
                            return "T";
                        case DateTextLength::NARROW:
                            return "大正";
                    }
                    Ops::unreachable();
                case Era::SHOWA:
                    switch (style) {
                        case DateTextLength::FULL:
                            return "Showa";
                        case DateTextLength::SHORT:
                            return "S";
                        case DateTextLength::NARROW:
                            return "昭和";
                    }
                    Ops::unreachable();
                case Era::HEISEI:
                    switch (style) {
                        case DateTextLength::FULL:
                            return "Heisei";
                        case DateTextLength::SHORT:
                            return "H";
                        case DateTextLength::NARROW:
                            return "平成";
                    }
                    Ops::unreachable();
                case Era::REIWA:
                    switch (style) {
                        case DateTextLength::FULL:
                            return "Reiwa";
                        case DateTextLength::SHORT:
                            return "R";
                        case DateTextLength::NARROW:
                            return "令和";
                    }
                    Ops::unreachable();
            }
            Ops::unreachable();
        }

        /**
         * @brief English name of a month.
         * @param year Unused (Japanese uses Gregorian month structure).
         * @param month The month (1-12).
         * @param style The desired textual style.
         * @returns The month name in the requested style.
         * @throws DateTimeException if @p month is not in [1, 12].
         *
         * Japanese formal documents typically use numeric months (一月,
         * 二月, ...).
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr StringView month_name(i32 year, u32 month, DateTextLength style) {
            return IsoChronology::month_name(year, month, style);
        }

        /**
         * @brief Parse an era name back into an Era value.
         * @param text The text to match.
         * @param style The style the text is expected to be in.
         * @returns The matched era, or empty Optional on no match.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr Optional<Era> parse_era(StringView text, DateTextLength style) noexcept {
            static constexpr Array<Era, 5> ERAS = {
                Era::MEIJI, Era::TAISHO, Era::SHOWA, Era::HEISEI, Era::REIWA,
            };
            for (Era e: ERAS) {
                if (text == era_name(e, style)) {
                    return e;
                }
            }
            return nullopt;
        }

        /**
         * @brief Parse a month name back into a 1-based month index.
         * @param text The text to match.
         * @param year Unused.
         * @param style The style the text is expected to be in.
         * @returns The 1-based month, or empty Optional on no match.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr Optional<u32> parse_month(StringView text, i32 year, DateTextLength style) noexcept {
            return IsoChronology::parse_month(text, year, style);
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
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<JapaneseChronology> of(i32 y, u32 m, u32 d);

        /**
         * @brief Create a date from a traditional Japanese month name.
         * @param y The ISO proleptic year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if the date is before Meiji (1868-01-25).
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<JapaneseChronology> of(i32 y, JapaneseMonth m, u32 d);

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The ISO proleptic year.
         * @param m The month.
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if the date is before Meiji (1868-01-25).
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<JapaneseChronology> of(Year y, Month m, Day d);

        /**
         * @brief Create a date from an epoch day count.
         * @param e The number of days since 1970-01-01.
         * @returns The date in this chronology.
         * @throws DateTimeException if e < MEIJI_START.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<JapaneseChronology> date_epoch_day(i64 e);

        /**
         * @brief Create a date from an era, year-of-era, month, and day.
         * @param era The Japanese era.
         * @param year_of_era The year within the era (1-based).
         * @param m The month (1-12).
         * @param d The day of the month.
         * @returns The date in this chronology.
         * @throws DateTimeException if year_of_era < 1, if the date is before Meiji
         *   (1868-01-25), or if the resulting date does not fall within the requested
         *   era (e.g. Showa 64-01-08, which is the first day of Heisei).
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static constexpr ChronoLocalDate<JapaneseChronology> of(Era era, i32 year_of_era, u32 m, u32 d);

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static ChronoLocalDate<JapaneseChronology> date_now();
    };

    using JapaneseDate = ChronoLocalDate<JapaneseChronology>;
    using JapaneseEra = JapaneseChronology::Era;

    constexpr JapaneseDate JapaneseChronology::of(i32 y, u32 m, u32 d) {
        if (gregorian_to_epoch_day(y, m, d) < MEIJI_START) {
            throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
        }
        return JapaneseDate(y, m, d);
    }

    constexpr JapaneseDate JapaneseChronology::of(i32 y, JapaneseMonth m, u32 d) {
        return of(y, static_cast<u32>(m), d);
    }

    constexpr JapaneseDate JapaneseChronology::of(Year y, Month m, Day d) {
        return of(
            static_cast<i32>(y),
            static_cast<u32>(m),
            static_cast<u32>(d)
        );
    }

    constexpr JapaneseDate JapaneseChronology::date_epoch_day(i64 e) {
        if (e < MEIJI_START) {
            throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
        }
        return JapaneseDate::of_epoch_day(e);
    }

    constexpr JapaneseDate JapaneseChronology::of(Era era, i32 year_of_era, u32 m, u32 d) {
        i32 py = proleptic_year(era, year_of_era);
        i64 e = gregorian_to_epoch_day(py, m, d);
        if (e < MEIJI_START) {
            throw DateTimeException("JapaneseChronology does not support dates before Meiji (1868-01-25)");
        }
        if (era_of(e) != era) {
            throw DateTimeException("Date does not fall within the specified Japanese era");
        }
        return JapaneseDate(py, m, d);
    }

    inline JapaneseDate JapaneseChronology::date_now() {
        return JapaneseDate::now();
    }
}
