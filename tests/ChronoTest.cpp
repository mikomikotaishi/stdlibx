import stdx;

using stdx::time::DateTimeException;
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

#ifdef __GNUC__
using namespace stdx::core;
#endif

struct JapaneseEraCase {
    StringView label;
    JapaneseEra era;
    i32 yoe;
    u32 m;
    u32 d;
};

void test_iso_chronology() {
    System::out.println("ISO chronology:");

    IsoDate today = IsoChronology::date_now();
    System::out.println("today (ISO): {}", today);
    System::out.println("epoch day: {}", today.to_epoch_day());
    System::out.println("length of month: {} days", today.length_of_month());
    System::out.println("length of year: {} days", today.length_of_year());
    System::out.println("is leap year: {}", today.is_leap_year());

    IsoDate y2k = IsoChronology::of(2000, 2, 29);
    System::out.println("Y2K leap day: {} (leap: {})", y2k, y2k.is_leap_year());

    IsoDate epoch = IsoChronology::date_epoch_day(0);
    System::out.println("epoch day 0: {}", epoch);

    IsoEra era = IsoChronology::era_of(-44);
    i32 yoe = IsoChronology::year_of_era(-44);
    System::out.println(
        "proleptic year -44 -> era: {}, year-of-era: {}",
        era == IsoEra::BCE ? "BCE" : "CE",
        yoe
    );

    IsoDate bce_via_era = IsoChronology::of(IsoEra::BCE, 45, 3, 15);
    IsoDate bce_via_proleptic = IsoChronology::of(-44, 3, 15);
    System::out.println(
        "BCE 45-03-15 via era factory: {} (matches proleptic ctor: {})",
        bce_via_era,
        bce_via_era == bce_via_proleptic
    );
    IsoDate bce_roundtrip = IsoChronology::date_epoch_day(bce_via_era.to_epoch_day());
    System::out.println(
        "round-trip via epoch day {}: {} (match: {})",
        bce_via_era.to_epoch_day(),
        bce_roundtrip,
        bce_via_era == bce_roundtrip
    );

    System::out.println();
}

void test_julian_chronology() {
    System::out.println("Julian chronology:");

    JulianDate d = JulianChronology::of(1900, 2, 29);
    System::out.println("1900-02-29: {} (Julian leap: {})", d, d.is_leap_year());

    IsoDate iso_equiv = d.to_iso();
    System::out.println("same instant in ISO: {}", iso_equiv);

    JulianDate g_reform = JulianDate::from_sys_days(
        IsoChronology::of(1582, 10, 15).to_sys_days()
    );
    System::out.println("Julian equivalent: {} (ISO 1582-10-15 Gregorian reform)", g_reform);

    JulianDate ides = JulianChronology::of(JulianEra::BC, 44, 3, 15);
    System::out.println(
        "Ides of March, BC 44 (Caesar assassinated): {} (ISO {})",
        ides,
        ides.to_iso()
    );

    JulianDate caesar_calendar = JulianChronology::of(JulianEra::BC, 45, 1, 1);
    System::out.println(
        "BC 45-01-01 (Julian calendar instituted): {} (ISO {})",
        caesar_calendar,
        caesar_calendar.to_iso()
    );

    System::out.println();
}

void test_hijrah_chronology() {
    System::out.println("Hijrah chronology:");

    HijrahDate today = HijrahChronology::date_now();
    System::out.println("today (AH): {}", today);
    System::out.println("month length: {} days", today.length_of_month());
    System::out.println("year length: {} days", today.length_of_year());
    System::out.println("is leap year: {}", today.is_leap_year());

    HijrahDate hijrah_epoch = HijrahChronology::of(1, 1, 1);
    System::out.println(
        "1 Muharram 1 AH: {} (ISO equivalent: {})",
        hijrah_epoch,
        hijrah_epoch.to_iso()
    );

    try {
        HijrahDate bad = HijrahChronology::of(0, 1, 1);
        System::err.println("ERROR: HijrahChronology should reject year 0");
    } catch (const DateTimeException& e) {
        System::out.println("year 0 rejected: {}", e.what());
    }

    System::out.println();
}

void test_japanese_chronology() {
    System::out.println("Japanese chronology:");

    JapaneseDate reiwa1 = JapaneseChronology::of(2019, 5, 1);
    JapaneseEra era = JapaneseChronology::era_of(reiwa1.to_epoch_day());
    i32 yoe = JapaneseChronology::year_of_era(reiwa1.year(), reiwa1.to_epoch_day());
    System::out.println("2019-05-01: {} -> Reiwa, year {}", reiwa1, yoe);
    System::out.println(
        "era enum value: {}",
        static_cast<i32>(era)
    );

    JapaneseDate showa64 = JapaneseChronology::of(JapaneseEra::SHOWA, 64, 1, 7);
    System::out.println("Showa 64-01-07: {} (last day of Showa)", showa64);

    try {
        JapaneseDate bad = JapaneseChronology::of(1867, 12, 31);
        System::err.println("ERROR: should reject pre-Meiji dates");
    } catch (const DateTimeException& e) {
        System::out.println("pre-Meiji rejected: {}", e.what());
    }

    Array<JapaneseEraCase, 3> boundary_cases = {{
        {"Reiwa 1-04-30 (actually last day of Heisei)", JapaneseEra::REIWA, 1, 4, 30},
        {"Showa 64-01-08 (actually first day of Heisei)", JapaneseEra::SHOWA, 64, 1, 8},
        {"Showa 65-01-01 (Showa 65 never existed)", JapaneseEra::SHOWA, 65, 1, 1},
    }};
    for (const JapaneseEraCase& c: boundary_cases) {
        try {
            JapaneseDate bad = JapaneseChronology::of(c.era, c.yoe, c.m, c.d);
            System::err.println("ERROR: should reject {}", c.label);
        } catch (const DateTimeException& e) {
            System::out.println("{} rejected: {}", c.label, e.what());
        }
    }

    System::out.println();
}

void test_hebrew_chronology() {
    System::out.println("Hebrew chronology:");

    HebrewDate today = HebrewChronology::date_now();
    System::out.println("today (AM): {}", today);
    System::out.println("month length: {} days", today.length_of_month());
    System::out.println("year length: {} days", today.length_of_year());
    System::out.println("is leap year: {}", today.is_leap_year());

    HebrewDate rh5784 = HebrewChronology::of(5784, 1, 1);
    System::out.println(
        "1 Tishri 5784: {}  (ISO equivalent: {})",
        rh5784,
        rh5784.to_iso()
    );

    System::out.println("year 5784 leap (true): {}", HebrewChronology::is_leap_year(5784));
    System::out.println("year 5785 leap (false): {}", HebrewChronology::is_leap_year(5785));
    System::out.println("year 5784 months: {}", HebrewChronology::months_in_year(5784));
    System::out.println("year 5785 months: {}", HebrewChronology::months_in_year(5785));

    try {
        HebrewDate bad = HebrewChronology::of(0, 1, 1);
        System::err.println("ERROR: HebrewChronology should reject year 0");
    } catch (const DateTimeException& e) {
        System::out.println("year 0 rejected: {}", e.what());
    }

    System::out.println();
}

void test_minguo_chronology() {
    System::out.println("Minguo (ROC) chronology:");

    MinguoDate today = MinguoChronology::date_now();
    System::out.println("today (ROC): {}", today);

    MinguoDate roc1 = MinguoChronology::of(1, 1, 1);
    System::out.println("ROC year 1: {} (ISO {})", roc1, roc1.to_iso());

    MinguoDate xinhai = MinguoChronology::of(MinguoEra::BEFORE_ROC, 1, 10, 10);
    System::out.println(
        "BEFORE_ROC year 1 (Xinhai Revolution, 1911-10-10): {} (ISO {})",
        xinhai,
        xinhai.to_iso()
    );

    System::out.println();
}

void test_thai_buddhist_chronology() {
    System::out.println("Thai Buddhist chronology:");

    ThaiBuddhistDate today = ThaiBuddhistChronology::date_now();
    System::out.println("today (BE): {}", today);

    ThaiBuddhistDate be2500 = ThaiBuddhistChronology::of(ThaiBuddhistEra::BE, 2500, 1, 1);
    System::out.println("BE 2500: {} (ISO {})", be2500, be2500.to_iso());

    System::out.println();
}

void test_chinese_chronology() {
    System::out.println("Chinese (lunisolar) chronology:");

    ChineseDate today = ChineseChronology::date_now();
    System::out.println("today (Chinese): {}", today);

    ChineseDateComponents full = ChineseChronology::from_epoch_day_full(today.to_epoch_day());
    System::out.println(
        "cycle {}, year {} of cycle, month {}{}, day {}",
        full.cycle,
        full.year_of_cycle,
        full.month,
        full.is_leap_month ? " (leap)" : "",
        full.day
    );

    IsoDate cny_2024_iso = IsoChronology::of(2024, 2, 10);
    ChineseDate cny_2024 = cny_2024_iso.to_chronology<ChineseChronology>();
    System::out.println(
        "ISO {} -> Chinese {}  (Chinese New Year 2024)",
        cny_2024_iso,
        cny_2024
    );

    bool leap_2023 = ChineseChronology::is_leap_year(2023);
    bool leap_2024 = ChineseChronology::is_leap_year(2024);
    System::out.println("Chinese year 2023 leap: {}", leap_2023);
    System::out.println("Chinese year 2024 leap: {}", leap_2024);
    System::out.println("year 2024 has {} months", ChineseChronology::months_in_year(2024));
    System::out.println("year 2024 has {} days", ChineseChronology::days_in_year(2024));

    System::out.println();
}

void test_cross_chronology_conversions() {
    System::out.println("Cross-chronology conversions:");

    IsoDate iso = IsoChronology::of(2024, 7, 4);
    System::out.println("anchor: ISO {}", iso);

    JulianDate julian = iso.to_chronology<JulianChronology>();
    HijrahDate hijrah = iso.to_chronology<HijrahChronology>();
    HebrewDate hebrew = iso.to_chronology<HebrewChronology>();
    JapaneseDate japanese = iso.to_chronology<JapaneseChronology>();
    MinguoDate minguo = iso.to_chronology<MinguoChronology>();
    ThaiBuddhistDate thai = iso.to_chronology<ThaiBuddhistChronology>();
    ChineseDate chinese = iso.to_chronology<ChineseChronology>();

    System::out.println("  Julian: {}", julian);
    System::out.println("  Hijrah: {}", hijrah);
    System::out.println("  Hebrew: {}", hebrew);
    System::out.println("  Japanese: {}", japanese);
    System::out.println("  Minguo: {}", minguo);
    System::out.println("  Thai BE: {}", thai);
    System::out.println("  Chinese: {}", chinese);

    System::out.println("all same epoch day: {}",
        iso.to_epoch_day() == julian.to_epoch_day()
        && iso.to_epoch_day() == hijrah.to_epoch_day()
        && iso.to_epoch_day() == hebrew.to_epoch_day()
        && iso.to_epoch_day() == chinese.to_epoch_day()
    );

    System::out.println();
}

void test_day_of_week() {
    System::out.println("Day of week:");

    IsoDate epoch = IsoChronology::date_epoch_day(0);
    System::out.println("1970-01-01: {} (Thursday expected)", epoch.day_of_week());

    IsoDate bastille = IsoChronology::of(1789, 7, 14);
    System::out.println("1789-07-14 (Bastille Day): {} (Tuesday expected)", bastille.day_of_week());

    IsoDate apollo = IsoChronology::of(1969, 7, 20);
    System::out.println("1969-07-20 (Apollo 11 landing): {} (Sunday expected)", apollo.day_of_week());

    HebrewDate rh = HebrewChronology::of(5784, 1, 1);
    IsoDate rh_iso = rh.to_iso();
    System::out.println(
        "Hebrew 5784-01-01 ({}) <=> ISO {} ({}): match {}",
        rh.day_of_week(),
        rh_iso,
        rh_iso.day_of_week(),
        rh.day_of_week() == rh_iso.day_of_week()
    );

    JulianDate caesar = JulianChronology::of(JulianEra::BC, 44, 3, 15);
    System::out.println(
        "Ides of March BC 44 (Julian {}): {} (Wednesday expected)",
        caesar,
        caesar.day_of_week()
    );

    System::out.println();
}

void test_date_arithmetic() {
    System::out.println("Date arithmetic:");

    IsoDate start = IsoChronology::of(2024, 1, 1);
    IsoDate later = start.plus_days(100);
    IsoDate earlier = start.minus_days(30);

    System::out.println("start: {}", start);
    System::out.println("later: {} (+100 days)", later);
    System::out.println("earlier: {} (-30 days)", earlier);

    i64 delta = later.to_epoch_day() - earlier.to_epoch_day();
    System::out.println("later - earlier = {} days", delta);

    IsoDate a = IsoChronology::of(2024, 6, 15);
    HijrahDate b = a.to_chronology<HijrahChronology>();
    System::out.println("ISO == Hijrah-converted: {}", a == b);
    System::out.println(
        "ISO < (Hijrah + 1 day): {}",
        a.to_epoch_day() < b.plus_days(1).to_epoch_day()
    );

    System::out.println();
}

int main() {
    System::out.println("Current wall clock: {}", System::current_time_as_string());
    System::out.println();

    test_iso_chronology();
    test_julian_chronology();
    test_hijrah_chronology();
    test_hebrew_chronology();
    test_japanese_chronology();
    test_minguo_chronology();
    test_thai_buddhist_chronology();
    test_chinese_chronology();
    test_cross_chronology_conversions();
    test_day_of_week();
    test_date_arithmetic();
}
