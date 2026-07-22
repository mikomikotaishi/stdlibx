import stdx;

using stdx::time::DateTimeException;
using stdx::time::Weekday;
using stdx::time::chrono::ChineseChronology;
using stdx::time::chrono::ChineseDate;
using stdx::time::chrono::ChineseDateComponents;
using stdx::time::chrono::HebrewChronology;
using stdx::time::chrono::HebrewDate;
using stdx::time::chrono::HijrahChronology;
using stdx::time::chrono::HijrahDate;
using stdx::time::chrono::IsoChronology;
using stdx::time::chrono::IsoDate;
using stdx::time::chrono::IsoEra;
using stdx::time::chrono::JapaneseChronology;
using stdx::time::chrono::JapaneseDate;
using stdx::time::chrono::JapaneseEra;
using stdx::time::chrono::JulianChronology;
using stdx::time::chrono::JulianDate;
using stdx::time::chrono::JulianEra;
using stdx::time::chrono::MinguoChronology;
using stdx::time::chrono::MinguoDate;
using stdx::time::chrono::MinguoEra;
using stdx::time::chrono::ThaiBuddhistChronology;
using stdx::time::chrono::ThaiBuddhistDate;
using stdx::time::chrono::ThaiBuddhistEra;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

/**
 * @struct JapaneseEraCase
 * @brief Test case for Japanese chronology era calculations.
 */
struct JapaneseEraCase {
    StringView label;
    JapaneseEra era;
    i32 yoe;
    u32 m;
    u32 d;
};

void test_iso_chronology() {
    // "Today" is non-deterministic, so only its invariants are checked.
    IsoDate today = IsoChronology::date_now();
    expect(today.length_of_month() >= 28 && today.length_of_month() <= 31, "ISO month length in [28, 31]");
    expect(today.length_of_year() == 365 || today.length_of_year() == 366, "ISO year length is 365 or 366");

    IsoDate y2k = IsoChronology::of(2000, 2, 29);
    expect(y2k.is_leap_year(), "2000 is a leap year");

    IsoDate epoch = IsoChronology::date_epoch_day(0);
    expect_eq(epoch, IsoChronology::of(1970, 1, 1), "epoch day 0 is 1970-01-01");
    expect(epoch.to_epoch_day() == 0, "epoch round-trips to day 0");

    expect(IsoChronology::era_of(-44) == IsoEra::BCE, "proleptic year -44 is BCE");
    expect_eq(IsoChronology::year_of_era(-44), 45, "proleptic year -44 is year-of-era 45 (45 BCE)");

    IsoDate bce_via_era = IsoChronology::of(IsoEra::BCE, 45, 3, 15);
    IsoDate bce_via_proleptic = IsoChronology::of(-44, 3, 15);
    expect_eq(bce_via_era, bce_via_proleptic, "BCE 45-03-15 == proleptic -44-03-15");

    IsoDate bce_roundtrip = IsoChronology::date_epoch_day(bce_via_era.to_epoch_day());
    expect_eq(bce_roundtrip, bce_via_era, "BCE date round-trips through epoch day");

    // ISO is proleptic: year 0 and negative proleptic years are valid.
    expect(IsoChronology::of(0, 1, 1).year() == 0, "proleptic year 0 is a valid ISO date");
    expect(IsoChronology::of(-1, 6, 15).year() == -1, "negative proleptic year is a valid ISO date");

    // Era-based construction reckons year-of-era from 1; there is no year 0.
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(IsoEra::BCE, 0, 1, 1); }, "BCE year-of-era 0 is rejected"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(IsoEra::CE, 0, 1, 1); }, "CE year-of-era 0 is rejected"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(IsoEra::BCE, -5, 1, 1); }, "negative year-of-era is rejected"
    );

    // Out-of-range months and days are rejected regardless of the year.
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(2024, 0, 1); }, "month 0 is rejected"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(2024, 13, 1); }, "month 13 is rejected"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(2024, 1, 0); }, "day 0 is rejected"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(2023, 2, 29); }, "2023-02-29 is rejected (2023 is not a leap year)"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)IsoChronology::of(2024, 4, 31); }, "April 31 is rejected (April has 30 days)"
    );
    // The leap-day boundary the rejection above hinges on is still accepted.
    expect(IsoChronology::of(2024, 2, 29).day() == 29, "2024-02-29 is accepted (2024 is a leap year)");

    // Named Gregorian months construct the same date as their 1-based index.
    expect_eq(IsoChronology::of(2024, IsoChronology::MARCH, 15), IsoChronology::of(2024, 3, 15), "IsoChronology::MARCH == month 3");
    expect(Ops::to_underlying(IsoChronology::DECEMBER) == 12, "IsoChronology::DECEMBER is the shared GregorianMonth value 12");
}

void test_julian_chronology() {
    JulianDate d = JulianChronology::of(1900, 2, 29);
    expect(d.is_leap_year(), "Julian 1900 is a leap year (divisible by 4)");
    expect(d.to_iso().to_epoch_day() == d.to_epoch_day(), "Julian->ISO preserves the day");

    // The Gregorian reform skipped ten days: Gregorian 1582-10-15 was Julian
    // 1582-10-05.
    JulianDate g_reform = JulianDate::from_sys_days(
        IsoChronology::of(1582, 10, 15).to_sys_days()
    );
    expect_eq(g_reform, JulianChronology::of(1582, 10, 5), "Gregorian 1582-10-15 is Julian 1582-10-05");

    JulianDate ides = JulianChronology::of(JulianEra::BC, 44, 3, 15);
    JulianDate caesar_calendar = JulianChronology::of(JulianEra::BC, 45, 1, 1);
    expect(caesar_calendar.to_epoch_day() < ides.to_epoch_day(), "BC 45 precedes BC 44");
    expect(ides.to_iso().to_epoch_day() == ides.to_epoch_day(), "Julian BC date->ISO preserves the day");

    expect_eq(JulianChronology::of(1900, JulianChronology::FEBRUARY, 29), JulianChronology::of(1900, 2, 29), "JulianChronology::FEBRUARY == month 2");
}

void test_hijrah_chronology() {
    HijrahDate today = HijrahChronology::date_now();
    expect(today.length_of_month() == 29 || today.length_of_month() == 30, "Hijrah month length is 29 or 30");
    expect(today.length_of_year() == 354 || today.length_of_year() == 355, "Hijrah year length is 354 or 355");

    HijrahDate hijrah_epoch = HijrahChronology::of(1, 1, 1);
    expect(
        hijrah_epoch.to_iso().to_epoch_day() == hijrah_epoch.to_epoch_day(),
        "1 Muharram 1 AH->ISO preserves the day"
    );

    expect_throws<DateTimeException>(
        [] -> void { (void)HijrahChronology::of(0, 1, 1); }, "Hijrah year 0 is rejected"
    );

    expect_eq(HijrahChronology::of(1445, HijrahChronology::RAMADAN, 1), HijrahChronology::of(1445, 9, 1), "HijrahChronology::RAMADAN == month 9");
}

void test_japanese_chronology() {
    JapaneseDate reiwa1 = JapaneseChronology::of(2019, 5, 1);
    expect(JapaneseChronology::era_of(reiwa1.to_epoch_day()) == JapaneseEra::REIWA, "2019-05-01 is in the Reiwa era");
    expect_eq(JapaneseChronology::year_of_era(reiwa1.year(), reiwa1.to_epoch_day()), 1, "2019-05-01 is Reiwa year 1");

    JapaneseDate showa64 = JapaneseChronology::of(JapaneseEra::SHOWA, 64, 1, 7);
    expect_eq(showa64.to_epoch_day(), IsoChronology::of(1989, 1, 7).to_epoch_day(), "Showa 64-01-07 is 1989-01-07");

    expect_throws<DateTimeException>(
        [] -> void { (void)JapaneseChronology::of(1867, 12, 31); }, "pre-Meiji dates are rejected"
    );

    Array<JapaneseEraCase, 3> boundary_cases = {{
        {"Reiwa 1-04-30 (really the last day of Heisei)", JapaneseEra::REIWA, 1, 4, 30},
        {"Showa 64-01-08 (really the first day of Heisei)", JapaneseEra::SHOWA, 64, 1, 8},
        {"Showa 65-01-01 (Showa 65 never existed)", JapaneseEra::SHOWA, 65, 1, 1},
    }};
    for (const JapaneseEraCase& c: boundary_cases) {
        expect_throws<DateTimeException>(
            [&] -> void { (void)JapaneseChronology::of(c.era, c.yoe, c.m, c.d); }, c.label
        );
    }

    // Traditional wafū month names map 1:1 onto the Gregorian months.
    expect_eq(JapaneseChronology::of(2019, JapaneseChronology::SHIWASU, 25), JapaneseChronology::of(2019, 12, 25), "JapaneseChronology::SHIWASU == December");
    expect(Ops::to_underlying(JapaneseChronology::MUTSUKI) == 1, "JapaneseChronology::MUTSUKI is January (month 1)");

    // Gregorian month names are also accepted, and agree with the wafū ones.
    expect_eq(JapaneseChronology::of(2019, JapaneseChronology::DECEMBER, 25), JapaneseChronology::of(2019, JapaneseChronology::SHIWASU, 25), "JapaneseChronology::DECEMBER == SHIWASU");
    expect_eq(JapaneseChronology::of(2019, JapaneseChronology::MAY, 1), JapaneseChronology::of(2019, 5, 1), "JapaneseChronology::MAY == month 5");
}

void test_hebrew_chronology() {
    HebrewDate today = HebrewChronology::date_now();
    expect(today.length_of_month() == 29 || today.length_of_month() == 30, "Hebrew month length is 29 or 30");
    const i32 year_length = today.length_of_year();
    expect(
        year_length == 353 || year_length == 354 || year_length == 355
        || year_length == 383 || year_length == 384 || year_length == 385,
        "Hebrew year length is one of the six valid lengths"
    );

    HebrewDate rh5784 = HebrewChronology::of(5784, 1, 1);
    expect(rh5784.to_iso().to_epoch_day() == rh5784.to_epoch_day(), "1 Tishri 5784->ISO preserves the day");

    expect(HebrewChronology::is_leap_year(5784), "Hebrew year 5784 is a leap year");
    expect(!HebrewChronology::is_leap_year(5785), "Hebrew year 5785 is not a leap year");
    expect_eq(HebrewChronology::months_in_year(5784), 13, "leap year 5784 has 13 months");
    expect_eq(HebrewChronology::months_in_year(5785), 12, "common year 5785 has 12 months");

    expect_throws<DateTimeException>(
        [] -> void { (void)HebrewChronology::of(0, 1, 1); }, "Hebrew year 0 is rejected"
    );

    // Named Hebrew months resolve to a civil index that shifts with the leap
    // year: Nisan is month 7 in a common year but month 8 in a leap year.
    expect_eq(HebrewChronology::of(5785, HebrewChronology::NISAN, 1), HebrewChronology::of(5785, 7, 1), "Nisan is month 7 in common year 5785");
    expect_eq(HebrewChronology::of(5784, HebrewChronology::NISAN, 1), HebrewChronology::of(5784, 8, 1), "Nisan is month 8 in leap year 5784");
    expect_eq(HebrewChronology::of(5784, HebrewChronology::ADAR_I, 1), HebrewChronology::of(5784, 6, 1), "Adar I is month 6 in leap year 5784");
    expect_eq(HebrewChronology::of(5784, HebrewChronology::ADAR_II, 1), HebrewChronology::of(5784, 7, 1), "Adar II is month 7 in leap year 5784");
    expect_eq(HebrewChronology::of(5785, HebrewChronology::ADAR, 1), HebrewChronology::of(5785, 6, 1), "Adar is month 6 in common year 5785");
    expect_throws<DateTimeException>(
        [] -> void { (void)HebrewChronology::of(5784, HebrewChronology::ADAR, 1); }, "bare Adar is rejected in leap year 5784 (ambiguous)"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)HebrewChronology::of(5785, HebrewChronology::ADAR_II, 1); }, "Adar II is rejected in common year 5785"
    );
    expect_throws<DateTimeException>(
        [] -> void { (void)HebrewChronology::of(5785, HebrewChronology::ADAR_I, 1); }, "Adar I is rejected in common year 5785"
    );
}

void test_minguo_chronology() {
    MinguoDate today = MinguoChronology::date_now();
    expect(today.to_iso().to_epoch_day() == today.to_epoch_day(), "Minguo today->ISO preserves the day");

    MinguoDate roc1 = MinguoChronology::of(1, 1, 1);
    expect_eq(roc1.to_iso(), IsoChronology::of(1912, 1, 1), "ROC year 1 is 1912-01-01");

    MinguoDate xinhai = MinguoChronology::of(MinguoEra::BEFORE_ROC, 1, 10, 10);
    expect_eq(xinhai.to_iso(), IsoChronology::of(1911, 10, 10), "BEFORE_ROC year 1-10-10 is 1911-10-10 (Xinhai Revolution)");

    expect_eq(MinguoChronology::of(1, MinguoChronology::JANUARY, 1), MinguoChronology::of(1, 1, 1), "MinguoChronology::JANUARY == month 1");
}

void test_thai_buddhist_chronology() {
    ThaiBuddhistDate today = ThaiBuddhistChronology::date_now();
    expect(today.to_iso().to_epoch_day() == today.to_epoch_day(), "Thai Buddhist today->ISO preserves the day");

    ThaiBuddhistDate be2500 = ThaiBuddhistChronology::of(ThaiBuddhistEra::BE, 2500, 1, 1);
    expect_eq(be2500.to_iso(), IsoChronology::of(1957, 1, 1), "BE 2500 is 1957 CE (BE = CE + 543)");

    expect_eq(ThaiBuddhistChronology::of(2500, ThaiBuddhistChronology::APRIL, 13), ThaiBuddhistChronology::of(2500, 4, 13), "ThaiBuddhistChronology::APRIL == month 4");
}

void test_chinese_chronology() {
    ChineseDate today = ChineseChronology::date_now();
    ChineseDateComponents full = ChineseChronology::from_epoch_day_full(today.to_epoch_day());
    expect(full.month >= 1 && full.month <= 12, "Chinese month in [1, 12]");
    expect(full.day >= 1 && full.day <= 30, "Chinese day in [1, 30]");
    expect(full.year_of_cycle >= 1 && full.year_of_cycle <= 60, "Chinese year-of-cycle in [1, 60]");

    IsoDate cny_2024_iso = IsoChronology::of(2024, 2, 10);
    ChineseDate cny_2024 = cny_2024_iso.to_chronology<ChineseChronology>();
    expect(cny_2024.to_epoch_day() == cny_2024_iso.to_epoch_day(), "ISO->Chinese preserves the day");
    ChineseDateComponents cny = ChineseChronology::from_epoch_day_full(cny_2024_iso.to_epoch_day());
    expect(cny.month == 1 && cny.day == 1 && !cny.is_leap_month, "2024-02-10 is Chinese New Year (month 1, day 1)");

    expect(ChineseChronology::is_leap_year(2023), "Chinese year 2023 is a leap year");
    expect(!ChineseChronology::is_leap_year(2024), "Chinese year 2024 is not a leap year");
    expect_eq(ChineseChronology::months_in_year(2023), 13, "leap year 2023 has 13 months");
    expect_eq(ChineseChronology::months_in_year(2024), 12, "common year 2024 has 12 months");

    // Earthly-branch month names: YIN (寅) is the first month of the year.
    expect_eq(ChineseChronology::of(2024, ChineseChronology::YIN, 1), ChineseChronology::of(2024, 1, 1), "ChineseChronology::YIN is the first month");

    // Leap-month construction: 2023 has a leap 2nd month (閏二月); leap_month
    // builds it, and only the leap-month path can.
    ChineseDate leap_second = ChineseChronology::of(2023, leap_month(ChineseChronology::MAO), 1);
    ChineseDateComponents lc = ChineseChronology::from_epoch_day_full(leap_second.to_epoch_day());
    expect(lc.month == 2 && lc.is_leap_month, "leap_month(MAO) builds the leap 2nd month of 2023");

    // Solar-term query: late December falls in the winter-solstice term, and
    // mid-April in Qingming - a sectional term only the full 24 include.
    ChineseDate around_solstice = IsoChronology::of(2024, 12, 25).to_chronology<ChineseChronology>();
    expect(
        ChineseChronology::solar_term_of(around_solstice) == ChineseChronology::DONGZHI,
        "2024-12-25 falls in the Dongzhi (winter solstice) solar term"
    );
    ChineseDate qingming = IsoChronology::of(2024, 4, 10).to_chronology<ChineseChronology>();
    expect(
        ChineseChronology::solar_term_of(qingming) == ChineseChronology::QINGMING,
        "2024-04-10 falls in the Qingming solar term"
    );
}

void test_cross_chronology_conversions() {
    // Converting between calendars never changes the underlying instant, so the
    // epoch day is preserved across every chronology.
    IsoDate iso = IsoChronology::of(2024, 7, 4);
    const i64 epoch = iso.to_epoch_day();

    expect(iso.to_chronology<JulianChronology>().to_epoch_day() == epoch, "ISO->Julian preserves the epoch day");
    expect(iso.to_chronology<HijrahChronology>().to_epoch_day() == epoch, "ISO->Hijrah preserves the epoch day");
    expect(iso.to_chronology<HebrewChronology>().to_epoch_day() == epoch, "ISO->Hebrew preserves the epoch day");
    expect(iso.to_chronology<JapaneseChronology>().to_epoch_day() == epoch, "ISO->Japanese preserves the epoch day");
    expect(iso.to_chronology<MinguoChronology>().to_epoch_day() == epoch, "ISO->Minguo preserves the epoch day");
    expect(iso.to_chronology<ThaiBuddhistChronology>().to_epoch_day() == epoch, "ISO->Thai Buddhist preserves the epoch day");
    expect(iso.to_chronology<ChineseChronology>().to_epoch_day() == epoch, "ISO->Chinese preserves the epoch day");
}

// Weekday implicitly converts to its underlying std::chrono::weekday, which
// makes a direct Weekday == Weekday::THURSDAY comparison ambiguous; settle both
// operands on the underlying type.
[[nodiscard]]
bool same_weekday(Weekday actual, Weekday::Self expected) noexcept {
    return static_cast<Weekday::Self>(actual) == expected;
}

void test_day_of_week() {
    expect(same_weekday(IsoChronology::date_epoch_day(0).day_of_week(), Weekday::THURSDAY), "1970-01-01 is a Thursday");
    expect(same_weekday(IsoChronology::of(1789, 7, 14).day_of_week(), Weekday::TUESDAY), "Bastille Day 1789-07-14 is a Tuesday");
    expect(same_weekday(IsoChronology::of(1969, 7, 20).day_of_week(), Weekday::SUNDAY), "Apollo 11 landing 1969-07-20 is a Sunday");

    HebrewDate rh = HebrewChronology::of(5784, 1, 1);
    expect(
        same_weekday(rh.day_of_week(), static_cast<Weekday::Self>(rh.to_iso().day_of_week())),
        "Hebrew date and its ISO equivalent agree on the weekday"
    );

    JulianDate caesar = JulianChronology::of(JulianEra::BC, 44, 3, 15);
    expect(same_weekday(caesar.day_of_week(), Weekday::WEDNESDAY), "Ides of March BC 44 is a Wednesday (Julian)");
}

void test_date_arithmetic() {
    IsoDate start = IsoChronology::of(2024, 1, 1);
    IsoDate later = start.plus_days(100);
    IsoDate earlier = start.minus_days(30);

    expect_eq(later, IsoChronology::of(2024, 4, 10), "2024-01-01 + 100 days is 2024-04-10");
    expect_eq(earlier, IsoChronology::of(2023, 12, 2), "2024-01-01 - 30 days is 2023-12-02");
    expect(later.to_epoch_day() - earlier.to_epoch_day() == 130, "later - earlier is 130 days");

    IsoDate a = IsoChronology::of(2024, 6, 15);
    HijrahDate b = a.to_chronology<HijrahChronology>();
    expect(a == b, "an ISO date equals its Hijrah conversion (same instant)");
    expect(a.to_epoch_day() < b.plus_days(1).to_epoch_day(), "ISO date precedes its Hijrah conversion + 1 day");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Chrono.iso", test_iso_chronology},
        {"Chrono.julian", test_julian_chronology},
        {"Chrono.hijrah", test_hijrah_chronology},
        {"Chrono.japanese", test_japanese_chronology},
        {"Chrono.hebrew", test_hebrew_chronology},
        {"Chrono.minguo", test_minguo_chronology},
        {"Chrono.thai_buddhist", test_thai_buddhist_chronology},
        {"Chrono.chinese", test_chinese_chronology},
        {"Chrono.cross_chronology_conversions", test_cross_chronology_conversions},
        {"Chrono.day_of_week", test_day_of_week},
        {"Chrono.date_arithmetic", test_date_arithmetic},
    });
}
