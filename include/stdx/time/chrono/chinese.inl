#pragma once

/**
 * @brief Chinese lunisolar calendar implementation.
 *
 * A faithful port of ICU's ChineseCalendar astronomical algorithm.
 * All astronomical computations use UTC+8 (Asia/Shanghai) and a longitude
 * of 120°E, matching the modern convention used by ICU.
 *
 * Solar longitude uses Jean Meeus' "Astronomical Algorithms" (Ch. 25).
 * New moon times use Meeus Ch. 49 with full correction terms.
 *
 * @see ICU4J com.ibm.icu.util.ChineseCalendar
 * @see Jean Meeus, "Astronomical Algorithms", 2nd ed.
 */
namespace stdx::time::chrono {
    inline constexpr f64 SYNODIC_MONTH = 29.530588853; ///< Mean synodic month in days.
    inline constexpr f64 EPOCH_JD = 2440587.5; ///< Julian Day of 1970-01-01 00:00 UTC.
    inline constexpr i32 SYNODIC_GAP = 25; ///< Safe step size (days) that won't cross a new moon.
    inline constexpr i32 CHINESE_EPOCH_YEAR = 1; ///< Gregorian year corresponding to Chinese year 1.
    inline constexpr i32 CYCLE_EPOCH_YEAR = -2636; ///< Gregorian year of cycle 1, year 1 (61st year of Huang Di).

    // "Local days" count whole days from 1970-01-01 in the UTC+8 time zone.
    // Because the offset is only +8 h and we work exclusively with whole-day
    // boundaries, local_days == epoch_day for integer epoch days.  The JD
    // conversions, however, must account for the 8-hour shift so that
    // astronomical events near local midnight round to the correct date.

    /**
     * @internal
     * @brief Convert local days (UTC+8) to Julian Day Number at local midnight.
     * @returns JD corresponding to local midnight of the given local days.
     */
    [[nodiscard]]
    inline f64 local_days_to_jd(i32 days) noexcept {
        // Local midnight in UTC = days * 86400 − 8 * 3600 seconds after epoch.
        // JD = utc_seconds / 86400 + 2440587.5
        //    = days − 8/24 + 2440587.5
        return static_cast<f64>(days) - 8.0 / 24.0 + EPOCH_JD;
    }

    /**
     * @internal
     * @brief Convert a Julian Day Number to local days (UTC+8).
     * @returns Local days (UTC+8) corresponding to the given JD, rounded down to the nearest whole day.
     */
    [[nodiscard]]
    inline i32 jd_to_local_days(f64 jd) noexcept {
        // Shift JD to local time, then subtract epoch JD and floor.
        return static_cast<i32>(
            Math::floor(jd + 8.0 / 24.0 - EPOCH_JD)
        );
    }

    /**
     * @internal
     * @brief Compute the sun's apparent ecliptic longitude in degrees [0, 360).
     * @param jd Julian Day Number (dynamical time ≈ UTC for calendar purposes).
     * @returns Solar longitude in degrees.
     *
     * Accuracy is approximately 0.01°, sufficient for calendar solar-term
     * determination.
     * @see Meeus, Ch. 25
     */
    [[nodiscard]]
    inline f64 solar_longitude_deg(f64 jd) noexcept {
        f64 T = (jd - 2451545.0) / 36525.0; // Julian centuries from J2000.0
        f64 T2 = T * T;

        // Geometric mean longitude of the sun (degrees)
        f64 L0 = Math::fmod(280.46646 + 36000.76983 * T + 0.0003032 * T2, 360.0);
        if (L0 < 0.0) {
            L0 += 360.0;
        }

        // Mean anomaly of the sun (radians)
        f64 M = Math::to_radians(Math::fmod(357.52911 + 35999.05029 * T - 0.0001537 * T2, 360.0));

        // Equation of the centre
        f64 C = (1.914602 - 0.004817 * T - 0.000014 * T2) * Math::sin(M)
            + (0.019993 - 0.000101 * T) * Math::sin(2.0 * M)
            + 0.000289 * Math::sin(3.0 * M);

        // Sun's true longitude
        f64 theta = L0 + C;

        // Apparent longitude (nutation + aberration correction)
        f64 omega = Math::to_radians(125.04 - 1934.136 * T);
        f64 lambda = theta - 0.00569 - 0.00478 * Math::sin(omega);

        lambda = Math::fmod(lambda, 360.0);
        if (lambda < 0.0) {
            lambda += 360.0;
        }
        return lambda;
    }

    /**
     * @internal
     * @brief Compute the Julian Ephemeris Day of the new moon for lunation number @p k.
     * @param k Lunation number (k = 0 corresponds to the new moon of 2000-01-06).
     *          Must be an integer for new-moon phase.
     * @returns JDE of the new moon.
     * @see Meeus, Ch. 49
     */
    [[nodiscard]]
    inline f64 new_moon_jde(f64 k) noexcept {
        f64 T = k / 1236.85;
        f64 T2 = T * T;
        f64 T3 = T2 * T;
        f64 T4 = T3 * T;

        // Mean phase JDE
        f64 jde = 2451550.09766
            + 29.530588861 * k
            + 0.00015437 * T2
            - 0.000000150 * T3
            + 0.00000000073 * T4;

        // Eccentricity of Earth's orbit
        f64 E = 1.0 - 0.002516 * T - 0.0000074 * T2;
        f64 E2 = E * E;

        // Sun's mean anomaly
        f64 M = Math::to_radians(
            Math::fmod(
                2.5534 + 29.10535670 * k
                - 0.0000014 * T2
                - 0.00000011 * T3,
                360.0
            )
        );

        // Moon's mean anomaly
        f64 Mp = Math::to_radians(
            Math::fmod(
                201.5643 + 385.81693528 * k
                + 0.0107582 * T2
                + 0.00001238 * T3
                - 0.000000058 * T4,
                360.0
            )
        );

        // Moon's argument of latitude
        f64 F = Math::to_radians(
            Math::fmod(
                160.7108 + 390.67050284 * k
                - 0.0016118 * T2
                - 0.00000227 * T3
                + 0.000000011 * T4,
                360.0
            )   
        );

        // Longitude of ascending node of lunar orbit
        f64 O = Math::to_radians(
            Math::fmod(
                124.7746 - 1.56375588 * k
                + 0.0020672 * T2
                + 0.00000215 * T3,
                360.0
            )
        );

        // New-moon correction
        f64 corr = -0.40720 * Math::sin(Mp)
            + 0.17241 * E * Math::sin(M)
            + 0.01608 * Math::sin(2.0 * Mp)
            + 0.01039 * Math::sin(2.0 * F)
            + 0.00739 * E * Math::sin(Mp - M)
            - 0.00514 * E * Math::sin(Mp + M)
            + 0.00208 * E2 * Math::sin(2.0 * M)
            - 0.00111 * Math::sin(Mp - 2.0 * F)
            - 0.00057 * Math::sin(Mp + 2.0 * F)
            + 0.00056 * E * Math::sin(2.0 * Mp + M)
            - 0.00042 * Math::sin(3.0 * Mp)
            + 0.00042 * E * Math::sin(M + 2.0 * F)
            + 0.00038 * E * Math::sin(M - 2.0 * F)
            - 0.00024 * E * Math::sin(2.0 * Mp - M)
            - 0.00017 * Math::sin(O)
            - 0.00007 * Math::sin(Mp + 2.0 * M)
            + 0.00004 * Math::sin(2.0 * Mp - 2.0 * F)
            + 0.00004 * Math::sin(3.0 * M)
            + 0.00003 * Math::sin(Mp + M - 2.0 * F)
            + 0.00003 * Math::sin(2.0 * Mp + 2.0 * F)
            - 0.00003 * Math::sin(Mp + M + 2.0 * F)
            + 0.00003 * Math::sin(Mp - M + 2.0 * F)
            - 0.00002 * Math::sin(Mp - M - 2.0 * F)
            - 0.00002 * Math::sin(3.0 * Mp + M)
            + 0.00002 * Math::sin(4.0 * Mp);

        jde += corr;

        // Planetary arguments (additional corrections)
        f64 A1 = Math::to_radians(Math::fmod(299.77 + 132.8475848 * k - 0.009173 * T2, 360.0));
        f64 A2 = Math::to_radians(Math::fmod(251.88 + 0.016321 * k, 360.0));
        f64 A3 = Math::to_radians(Math::fmod(251.83 + 26.651886 * k, 360.0));
        f64 A4 = Math::to_radians(Math::fmod(349.42 + 36.412478 * k, 360.0));
        f64 A5 = Math::to_radians(Math::fmod(84.66 + 18.206239 * k, 360.0));
        f64 A6 = Math::to_radians(Math::fmod(141.74 + 53.303771 * k, 360.0));
        f64 A7 = Math::to_radians(Math::fmod(207.14 + 2.453732 * k, 360.0));
        f64 A8 = Math::to_radians(Math::fmod(154.84 + 7.306860 * k, 360.0));
        f64 A9 = Math::to_radians(Math::fmod(34.52 + 27.261239 * k, 360.0));
        f64 A10 = Math::to_radians(Math::fmod(207.19 + 0.121824 * k, 360.0));
        f64 A11 = Math::to_radians(Math::fmod(291.34 + 1.844379 * k, 360.0));
        f64 A12 = Math::to_radians(Math::fmod(161.72 + 24.198154 * k, 360.0));
        f64 A13 = Math::to_radians(Math::fmod(239.56 + 25.513099 * k, 360.0));
        f64 A14 = Math::to_radians(Math::fmod(331.55 + 3.592518 * k, 360.0));

        jde += 0.000325 * Math::sin(A1)
            + 0.000165 * Math::sin(A2)
            + 0.000164 * Math::sin(A3)
            + 0.000126 * Math::sin(A4)
            + 0.000110 * Math::sin(A5)
            + 0.000062 * Math::sin(A6)
            + 0.000060 * Math::sin(A7)
            + 0.000056 * Math::sin(A8)
            + 0.000047 * Math::sin(A9)
            + 0.000042 * Math::sin(A10)
            + 0.000040 * Math::sin(A11)
            + 0.000037 * Math::sin(A12)
            + 0.000035 * Math::sin(A13)
            + 0.000023 * Math::sin(A14);

        return jde;
    }

    /**
     * @internal
     * @brief Find the new moon nearest to @p days.
     * @param days  Local days (UTC+8) since epoch.
     * @param after If true, returns the new moon on or after @p days;
     *              otherwise, returns the new moon strictly before @p days.
     * @returns Local days (UTC+8) of the nearest qualifying new moon.
     */
    [[nodiscard]]
    inline i32 find_new_moon_near(i32 days, bool after) noexcept {
        f64 jd = local_days_to_jd(days);
        f64 k = Math::round((jd - 2451550.09766) / 29.530588861);

        f64 nm_jd = new_moon_jde(k);
        i32 nm_days = jd_to_local_days(nm_jd);

        if (after) {
            while (nm_days < days) {
                k += 1.0;
                nm_jd = new_moon_jde(k);
                nm_days = jd_to_local_days(nm_jd);
            }
        } else {
            while (nm_days >= days) {
                k -= 1.0;
                nm_jd = new_moon_jde(k);
                nm_days = jd_to_local_days(nm_jd);
            }
        }

        return nm_days;
    }

    /**
     * @internal
     * @brief Return the nearest integer number of synodic months between two dates.
     * @param day1 Local days (UTC+8).
     * @param day2 Local days (UTC+8).
     * @returns Approximate integer month count.
     */
    [[nodiscard]]
    inline i32 synodic_months_between(i32 day1, i32 day2) noexcept {
        return static_cast<i32>(
            Math::round(static_cast<f64>(day2 - day1) / SYNODIC_MONTH)
        );
    }

    /**
     * @internal
     * @brief Return the winter solstice (solar longitude = 270°) for the given
     *        Gregorian year, as local days (UTC+8).
     * @param gyear Gregorian year.
     * @returns Local days of the winter solstice (Dōngzhì, 冬至).
     *
     * Starts the search from December 15 and iterates until the solar longitude
     * converges to 270° within 0.0001° (~0.4 arc-seconds).
     */
    [[nodiscard]]
    inline i32 winter_solstice(i32 gyear) noexcept {
        // Approximate: December 15 of the given year
        f64 jd = local_days_to_jd(
            static_cast<i32>(gregorian_to_epoch_day(gyear, 12, 15))
        );

        // Iterate to find solar longitude = 270° (Dōngzhì)
        for (i32 i = 0; i < 50; ++i) {
            f64 lon = solar_longitude_deg(jd);
            f64 diff = 270.0 - lon;

            // Normalise to [−180, 180]
            if (diff > 180.0) {
                diff -= 360.0;
            } else if (diff < -180.0) {
                diff += 360.0;
            }

            if (Math::abs(diff) < 0.0001) {
                break;
            }

            // The sun moves ~0.9856°/day ≈ 360°/365.25
            jd += diff * 365.25 / 360.0;
        }

        return jd_to_local_days(jd);
    }

    /**
     * @internal
     * @brief Return the major solar term number (1–12) for a given local day.
     *
     * The numbering follows ICU convention:
     * - Term 1 corresponds to 330° (Dàhán, 大寒)
     * - Term 2 corresponds to 0° (Yǔshuǐ, 雨水)
     * - Term 3 corresponds to 30° (Chūnfēn, 春分)
     * - ...
     * - Term 11 corresponds to 270° (Dōngzhì, 冬至)
     * - Term 12 corresponds to 300° (Dàxuě, 大雪)
     *
     * @param days Local days (UTC+8).
     * @returns Major solar term number (1–12).
     */
    [[nodiscard]]
    inline i32 major_solar_term(i32 days) noexcept {
        f64 jd = local_days_to_jd(days);
        f64 lon = solar_longitude_deg(jd);
        // Compute (floor(lon / 30) + 2) % 12, mapped to 1–12
        i32 term = (static_cast<i32>(Math::floor(lon / 30.0)) + 2) % 12;
        if (term < 1) {
            term += 12;
        }
        return term;
    }

    /**
     * @internal
     * @brief Return true if the lunar month starting at @p new_moon contains
     *        no major solar term (a necessary condition for a leap month).
     * @param new_moon Local days (UTC+8) of a new moon.
     */
    [[nodiscard]]
    inline bool has_no_major_solar_term(i32 new_moon) noexcept {
        i32 next_moon = find_new_moon_near(new_moon + SYNODIC_GAP, true);
        return major_solar_term(new_moon) == major_solar_term(next_moon);
    }

    /**
     * @internal
     * @brief Return true if there is a leap month on or after the new moon at
     *        @p new_moon1 and at or before the new moon at @p new_moon2.
     * @param new_moon1 Local days (UTC+8) of a new moon.
     * @param new_moon2 Local days (UTC+8) of a new moon (>= @p new_moon1).
     *
     * This is a recursive walk backwards from @p new_moon2 to @p new_moon1.
     * Recursion depth is bounded by the number of months between the two
     * moons (at most ~13).
     *
     * @see ICU4J ChineseCalendar.isLeapMonthBetween
     */
    [[nodiscard]]
    inline bool is_leap_month_between(i32 new_moon1, i32 new_moon2) noexcept {
        if (new_moon2 < new_moon1) {
            return false;
        }
        // Sanity guard: the two moons should never be more than ~50 months apart
        // in any valid call path. Mirrors ICU's assertion.
        if (synodic_months_between(new_moon1, new_moon2) >= 50) {
            return false;
        }
        return is_leap_month_between(new_moon1, find_new_moon_near(new_moon2 - SYNODIC_GAP, false))
            || has_no_major_solar_term(new_moon2);
    }

    /**
     * @internal
     * @struct ChineseMonthInfo
     * @brief Decomposed month information for a given local day within the
     *        Chinese calendar.
     */
    struct ChineseMonthInfo final {
        i32 month; ///< 0-based month number (0–11).
        i32 ordinal_month; ///< Ordinal month offset from new year (0-based).
        i32 this_moon; ///< Local days of this month's new moon.
        bool is_leap_month; ///< Whether this month is a leap (intercalary) month.
        bool has_leap_month_between_solstices; ///< Whether a leap month exists between the bracketing winter solstices.
    };

    /**
     * @internal
     * @brief Compute the Chinese month information for a given local day.
     * @param days Local days (UTC+8).
     * @param gyear Gregorian year of @p days.
     * @returns Month information including month number, leap status, and new moon.
     *
     * @see ICU4J ChineseCalendar.computeMonthInfo
     */
    [[nodiscard]]
    inline ChineseMonthInfo compute_chinese_month_info(i32 days, i32 gyear) noexcept;

    /**
     * @internal
     * @brief Return the Chinese New Year for a given Gregorian year.
     * @param gyear Gregorian year.
     * @returns Local days (UTC+8) of the Chinese New Year (a new moon in
     *          late January or February).
     *
     * The Chinese New Year is the second new moon after the winter solstice
     * of the preceding year, unless there is a leap month between the
     * solstice new moon and the second new moon — in which case it is the
     * third new moon.
     *
     * @see ICU4J ChineseCalendar.newYear
     */
    [[nodiscard]]
    inline i32 chinese_new_year(i32 gyear) noexcept {
        i32 solstice_before = winter_solstice(gyear - 1);
        i32 solstice_after = winter_solstice(gyear);

        i32 new_moon1 = find_new_moon_near(solstice_before + 1, true);
        i32 new_moon2 = find_new_moon_near(new_moon1 + SYNODIC_GAP, true);
        i32 new_moon11 = find_new_moon_near(solstice_after + 1, false);

        if (synodic_months_between(new_moon1, new_moon11) == 12
            && (has_no_major_solar_term(new_moon1) || has_no_major_solar_term(new_moon2))) {
            return find_new_moon_near(new_moon2 + SYNODIC_GAP, true);
        }
        return new_moon2;
    }

    // --- Deferred definition of compute_chinese_month_info ---
    // (needs chinese_new_year, which in turn needs has_no_major_solar_term)

    inline ChineseMonthInfo compute_chinese_month_info(i32 days, i32 gyear) noexcept {
        // Find winter solstices bracketing this date.
        // These define the boundaries of the Chinese year: month 11 always
        // contains the winter solstice.
        i32 solstice_after = winter_solstice(gyear);
        i32 solstice_before;
        if (days < solstice_after) {
            solstice_before = winter_solstice(gyear - 1);
        } else {
            solstice_before = solstice_after;
            solstice_after = winter_solstice(gyear + 1);
        }

        // New moon starting month 11 (or leap 11) after each solstice.
        i32 first_moon = find_new_moon_near(solstice_before + 1, true);
        i32 last_moon = find_new_moon_near(solstice_after + 1, false);
        i32 this_moon = find_new_moon_near(days + 1, false);

        // 12 synodic intervals between the two solstice-adjacent moons means
        // 13 months, so a leap month exists somewhere in the span.
        bool has_leap = synodic_months_between(first_moon, last_moon) == 12;

        i32 month = synodic_months_between(first_moon, this_moon);

        if (has_leap && is_leap_month_between(first_moon, this_moon)) {
            --month;
        }
        if (month < 1) {
            month += 12;
        }

        // Ordinal month offset from new year.
        i32 the_new_year = chinese_new_year(gyear);
        if (days < the_new_year) {
            the_new_year = chinese_new_year(gyear - 1);
        }
        i32 ordinal_month = synodic_months_between(the_new_year, this_moon);
        if (ordinal_month < 0) {
            ordinal_month += 12;
        }

        // A month is a leap month if:
        //   (a) there IS a leap month between the solstices, AND
        //   (b) this month has no major solar term, AND
        //   (c) no earlier month in the span is already the leap month.
        bool is_leap_month = has_leap
            && has_no_major_solar_term(this_moon)
            && !is_leap_month_between(first_moon, find_new_moon_near(this_moon - SYNODIC_GAP, false));

        return {
            .month = month - 1, // convert to 0-based
            .ordinal_month = ordinal_month,
            .this_moon = this_moon,
            .is_leap_month = is_leap_month,
            .has_leap_month_between_solstices = has_leap,
        };
    }
}

/**
 * @namespace stdx::time::chrono
 * @brief Calendar system support.
 */
export namespace stdx::time::chrono {
    /**
     * @struct ChineseDateComponents
     * @brief Extended date components for the Chinese calendar, including
     * sexagenary cycle and leap-month information.
     */
    struct [[nodiscard]] ChineseDateComponents final {
        i32 proleptic_year; ///< Continuous year count (year 1 ≈ 1 CE).
        i32 cycle;///< 60-year sexagenary cycle number (1-based).
        i32 year_of_cycle;  ///< Year within the cycle (1–60).
        u32 month; ///< Month (1–12).
        u32 day; ///< Day of the month (1–29 or 1–30).
        bool is_leap_month; ///< Whether this month is a leap (intercalary) month.
    };

    /**
     * @class ChineseChronology
     * @brief The Chinese lunisolar chronology.
     *
     * A faithful port of ICU's ChineseCalendar. All astronomical computations
     * are performed relative to UTC+8 (Asia/Shanghai, 120°E).
     *
     * Years are counted continuously from a conventional epoch where year 1
     * corresponds to approximately 1 CE. The 60-year sexagenary cycle is
     * available through @ref cycle_of and @ref year_of_cycle.
     *
     * Because the Chinese calendar is lunisolar and month boundaries depend
     * on astronomical new-moon computation, the methods in this class are
     * **not** @c constexpr.
     *
     * The basic @ref ChronologyLike interface (@ref to_epoch_day / @ref
     * from_epoch_day with three-parameter signatures) treats the month
     * parameter as a non-leap month. For full leap-month control use the
     * extended overloads @ref to_epoch_day(i32,u32,u32,bool) and
     * @ref from_epoch_day_full.
     *
     * @see ICU4J com.ibm.icu.util.ChineseCalendar
     */
    class [[nodiscard]] ChineseChronology final {
    public:
        ChineseChronology() = delete;

        /**
         * @enum Era
         * @brief Chinese calendar era, relative to the Yellow Emperor (黃帝).
         *
         * The conventional epoch is the 61st year of Huang Di's reign
         * (2637 BCE), which marks cycle 1, year 1. Proleptic years >= 1
         * fall in the HUANGDI era; years <= 0 are BEFORE_HUANGDI.
         */
        enum class Era : i32 {
            BEFORE_HUANGDI = 0, ///< Before the Yellow Emperor epoch (proleptic years <= 0)
            HUANGDI = 1, ///< Yellow Emperor epoch onwards (proleptic years >= 1)
        };

        /**
         * @brief Determine the era for a given proleptic year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns Era::HUANGDI if proleptic_year >= 1, Era::BEFORE_HUANGDI otherwise.
         */
        [[nodiscard]]
        static constexpr Era era_of(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? Era::HUANGDI : Era::BEFORE_HUANGDI;
        }

        /**
         * @brief Get the year-of-era from a proleptic year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns The positive year-of-era value.
         */
        [[nodiscard]]
        static constexpr i32 year_of_era(i32 proleptic_year) noexcept {
            return proleptic_year >= 1 ? proleptic_year : 1 - proleptic_year;
        }

        /**
         * @brief Returns the chronology identifier.
         * @returns "Chinese"
         */
        [[nodiscard]]
        static constexpr StringView id() noexcept {
            return "Chinese"sv;
        }

        /**
         * @brief Returns the calendar type.
         * @returns "chinese"
         */
        [[nodiscard]]
        static constexpr StringView calendar_type() noexcept {
            return "chinese"sv;
        }

        /**
         * @brief Check if a Chinese year is a leap year (has 13 months).
         * @param proleptic_year The proleptic Chinese year.
         * @returns true if the year contains a leap (intercalary) month.
         */
        [[nodiscard]]
        static bool is_leap_year(i32 proleptic_year) noexcept {
            i32 ny1 = chinese_new_year(proleptic_year);
            i32 ny2 = chinese_new_year(proleptic_year + 1);
            return synodic_months_between(ny1, ny2) == 13;
        }

        /**
         * @brief Get the number of days in a month (non-leap variant).
         * @param proleptic_year The proleptic Chinese year.
         * @param m The month (1–12). Refers to the non-leap month.
         * @returns 29 or 30.
         */
        [[nodiscard]]
        static u32 days_in_month(i32 proleptic_year, u32 m) noexcept {
            i64 start = to_epoch_day(proleptic_year, m, 1);
            i64 end = (m == 12)
                ? to_epoch_day(proleptic_year + 1, 1, 1)
                : to_epoch_day(proleptic_year, m + 1, 1);
            return static_cast<u32>(end - start);
        }

        /**
         * @brief Get the number of days in a Chinese year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns Total days from this New Year to the next (353–385).
         */
        [[nodiscard]]
        static i32 days_in_year(i32 proleptic_year) noexcept {
            i32 ny1 = chinese_new_year(proleptic_year);
            i32 ny2 = chinese_new_year(proleptic_year + 1);
            return ny2 - ny1;
        }

        /**
         * @brief Get the number of months in a Chinese year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns 12 in a normal year, 13 in a leap year.
         */
        [[nodiscard]]
        static u32 months_in_year(i32 proleptic_year) noexcept {
            return is_leap_year(proleptic_year) ? 13 : 12;
        }

        /**
         * @brief Convert a Chinese date to an epoch day count.
         *
         * The month is treated as a non-leap month. For leap months, use
         * the four-parameter overload.
         *
         * @param proleptic_year The proleptic Chinese year.
         * @param m The month (1–12).
         * @param d The day of the month.
         * @returns Days since 1970-01-01 (Gregorian).
         */
        [[nodiscard]]
        static i64 to_epoch_day(i32 proleptic_year, u32 m, u32 d) noexcept {
            return to_epoch_day(proleptic_year, m, d, false);
        }

        /**
         * @brief Convert a Chinese date to an epoch day count, with
         *        explicit leap-month flag.
         * @param proleptic_year The proleptic Chinese year.
         * @param m The month (1–12).
         * @param d The day of the month.
         * @param is_leap_month true if @p m refers to the leap variant.
         * @returns Days since 1970-01-01 (Gregorian).
         *
         * @see ICU4J ChineseCalendar.handleComputeMonthStart
         */
        [[nodiscard]]
        static i64 to_epoch_day(i32 proleptic_year, u32 m, u32 d, bool is_leap_month) noexcept {
            // The Chinese proleptic year roughly corresponds to the Gregorian year.
            i32 gyear = proleptic_year;
            i32 ny = chinese_new_year(gyear);

            // Month is 1-based; approximate the target new moon.
            i32 new_moon = find_new_moon_near(
                ny + static_cast<i32>(static_cast<f64>(m - 1) * SYNODIC_MONTH), true
            );

            // Verify this is the correct month by decomposing it.
            DateComponents greg = epoch_day_to_gregorian(static_cast<i64>(new_moon));
            ChineseMonthInfo info = compute_chinese_month_info(new_moon, greg.proleptic_year);

            // If the month number or leap status doesn't match, step one moon forward.
            if (static_cast<i32>(m) - 1 != info.month || is_leap_month != info.is_leap_month) {
                new_moon = find_new_moon_near(new_moon + SYNODIC_GAP, true);
            }

            return static_cast<i64>(new_moon) + static_cast<i64>(d) - 1;
        }

        /**
         * @brief Convert an epoch day count to basic date components.
         *
         * Returns a @ref DateComponents with month (1–12) and day, but no
         * leap-month information. For full decomposition use @ref
         * from_epoch_day_full.
         *
         * @param e Days since 1970-01-01 (Gregorian).
         * @returns Decomposed date components (proleptic_year, month, day).
         */
        [[nodiscard]]
        static DateComponents from_epoch_day(i64 e) noexcept {
            auto full = from_epoch_day_full(e);
            return {full.proleptic_year, full.month, full.day};
        }

        /**
         * @brief Convert an epoch day count to full Chinese date components,
         *        including sexagenary cycle and leap-month information.
         * @param e Days since 1970-01-01 (Gregorian).
         * @returns Full decomposed Chinese date.
         *
         * @see ICU4J ChineseCalendar.handleComputeFields
         */
        [[nodiscard]]
        static ChineseDateComponents from_epoch_day_full(i64 e) noexcept {
            i32 days = static_cast<i32>(e); // local_days == epoch_day for integers

            DateComponents greg = epoch_day_to_gregorian(e);
            i32 gyear = greg.proleptic_year;

            ChineseMonthInfo info = compute_chinese_month_info(days, gyear);

            // Extended year and cycle year are based on the epoch year.
            i32 extended_year = gyear - CHINESE_EPOCH_YEAR;
            i32 cycle_year = gyear - CYCLE_EPOCH_YEAR;

            // The Chinese year increments at Chinese New Year, which falls
            // in Gregorian January or February. For months before month 10
            // (0-based), or any date from Gregorian July onwards, we are in
            // the "next" Chinese year relative to the raw Gregorian year.
            if (info.month < 10 || greg.month >= 7) {
                ++extended_year;
                ++cycle_year;
            }

            i32 day_of_month = days - info.this_moon + 1;

            // 60-year cycle: cycle_year 1 = year 1 of cycle 1.
            i32 yoc_minus_1 = (cycle_year - 1) % 60;
            if (yoc_minus_1 < 0) {
                yoc_minus_1 += 60;
            }
            i32 cycle = (cycle_year - 1 - yoc_minus_1) / 60 + 1;
            i32 year_of_cycle = yoc_minus_1 + 1;

            return {
                .proleptic_year = extended_year,
                .cycle = cycle,
                .year_of_cycle = year_of_cycle,
                .month = static_cast<u32>(info.month + 1), // 1-based
                .day = static_cast<u32>(day_of_month),
                .is_leap_month = info.is_leap_month,
            };
        }

        /**
         * @brief Get the 60-year cycle number for a proleptic year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns Cycle number (1-based).
         */
        [[nodiscard]]
        static constexpr i32 cycle_of(i32 proleptic_year) noexcept {
            i32 cy = proleptic_year - CYCLE_EPOCH_YEAR + CHINESE_EPOCH_YEAR;
            i32 yoc = (cy - 1) % 60;
            if (yoc < 0) {
                yoc += 60;
            }
            return (cy - 1 - yoc) / 60 + 1;
        }

        /**
         * @brief Get the year within the 60-year cycle for a proleptic year.
         * @param proleptic_year The proleptic Chinese year.
         * @returns Year of cycle (1–60).
         */
        [[nodiscard]]
        static constexpr i32 year_of_cycle(i32 proleptic_year) noexcept {
            i32 cy = proleptic_year - CYCLE_EPOCH_YEAR + CHINESE_EPOCH_YEAR;
            i32 yoc = (cy - 1) % 60;
            if (yoc < 0) {
                yoc += 60;
            }
            return yoc + 1;
        }

        /**
         * @brief Convert a cycle and year-of-cycle to a proleptic year.
         * @param cycle The cycle number (1-based).
         * @param year_of_cycle The year within the cycle (1–60).
         * @returns The proleptic Chinese year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(i32 cycle, i32 year_of_cycle) noexcept {
            return (cycle - 1) * 60 + year_of_cycle + CYCLE_EPOCH_YEAR - CHINESE_EPOCH_YEAR;
        }

        /**
         * @brief Convert an era and year-of-era to a proleptic year.
         * @param era The calendar era (BEFORE_HUANGDI or HUANGDI).
         * @param year_of_era The positive year-of-era value.
         * @returns The proleptic Chinese year.
         */
        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, i32 year_of_era) noexcept {
            return era == Era::HUANGDI ? year_of_era : 1 - year_of_era;
        }

        [[nodiscard]]
        static bool is_leap_year(Year y) noexcept {
            return is_leap_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static u32 days_in_month(Year y, Month m) noexcept {
            return days_in_month(
                static_cast<i32>(y),
                static_cast<u32>(m)
            );
        }

        [[nodiscard]]
        static i32 days_in_year(Year y) noexcept {
            return days_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static u32 months_in_year(Year y) noexcept {
            return months_in_year(static_cast<i32>(y));
        }

        [[nodiscard]]
        static i64 to_epoch_day(Year y, Month m, Day d) noexcept {
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
        static constexpr i32 cycle_of(Year y) noexcept {
            return cycle_of(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 year_of_cycle(Year y) noexcept {
            return year_of_cycle(static_cast<i32>(y));
        }

        [[nodiscard]]
        static constexpr i32 proleptic_year(Era era, Year year_of_era) noexcept {
            return proleptic_year(era, static_cast<i32>(year_of_era));
        }

        /**
         * @brief Create a date from proleptic year, month, and day.
         * @param y The proleptic Chinese year.
         * @param m The month (1–12, non-leap).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ChineseChronology> of(i32 y, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from typed year, month, and day.
         * @param y The proleptic Chinese year.
         * @param m The month (1–12, non-leap).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ChineseChronology> of(Year y, Month m, Day d) noexcept;

        /**
         * @brief Create a date from era, year-of-era, month, and day.
         * @param era The calendar era (BEFORE_HUANGDI or HUANGDI).
         * @param year_of_era The positive year-of-era value.
         * @param m The month (1–12, non-leap).
         * @param d The day of the month.
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ChineseChronology> of(Era era, i32 year_of_era, u32 m, u32 d) noexcept;

        /**
         * @brief Create a date from an epoch day count.
         * @param e Days since 1970-01-01 (Gregorian).
         * @returns The date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ChineseChronology> date_epoch_day(i64 e) noexcept;

        /**
         * @brief Create a date for today according to the system clock.
         * @returns Today's date in this chronology.
         */
        [[nodiscard]]
        static ChronoLocalDate<ChineseChronology> date_now() noexcept;
    };

    using ChineseDate = ChronoLocalDate<ChineseChronology>;
    using ChineseEra = ChineseChronology::Era;

    inline ChineseDate ChineseChronology::of(i32 y, u32 m, u32 d) noexcept {
        return ChineseDate(y, m, d);
    }

    inline ChineseDate ChineseChronology::of(Year y, Month m, Day d) noexcept {
        return ChineseDate(y, m, d);
    }

    inline ChineseDate ChineseChronology::of(ChineseChronology::Era era, i32 yoe, u32 m, u32 d) noexcept {
        return ChineseDate(proleptic_year(era, yoe), m, d);
    }

    inline ChineseDate ChineseChronology::date_epoch_day(i64 e) noexcept {
        return ChineseDate::of_epoch_day(e);
    }

    inline ChineseDate ChineseChronology::date_now() noexcept {
        return ChineseDate::now();
    }
}
