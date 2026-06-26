#pragma once

using stdx::time::chrono::ChronoLocalDate;
using stdx::time::chrono::ChronologyLike;
using stdx::time::chrono::ChronologyTextLike;
using stdx::time::chrono::DateTextLength;

/**
 * @namespace stdx::time
 * @brief Pattern-based date/time formatting and parsing.
 *
 * Provides DateTimeFormatter, pattern engine that works against any 
 * chronology satisfying stdx::time::chrono::ChronologyLike.
 * Patterns containing textual fields (G, MMM+, EEE+) additionally
 * require ChronologyTextLike.
 *
 * Intentionally locale-independent: textual names come from each
 * Chronology's own canonical data. A future Locale provider can
 * shadow those defaults without changing DateTimeFormatter.
 */
export namespace stdx::time {
    /**
     * @brief English short/full/narrow name of a weekday.
     *
     * Weekdays are chronology-independent, so the names live with the
     * formatter rather than on each Chronology.
     */
    [[nodiscard]]
    constexpr StringView weekday_name(Weekday wd, DateTextLength style) noexcept {
        static constexpr Array<StringView, 7> FULL_NAMES = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday",
        };
        static constexpr Array<StringView, 7> SHORT_NAMES = {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
        };
        static constexpr Array<StringView, 7> NARROW_NAMES = {
            "S", "M", "T", "W", "T", "F", "S",
        };
        u32 idx = wd.c_encoding(); // 0 = Sunday .. 6 = Saturday
        switch (style) {
            case DateTextLength::FULL:
                return FULL_NAMES[idx];
            case DateTextLength::SHORT:
                return SHORT_NAMES[idx];
            case DateTextLength::NARROW:
                return NARROW_NAMES[idx];
        }
        Ops::unreachable();
    }

    /**
     * @class DateTimeFormatter
     * @brief Pattern-based formatter for dates in any chronology.
     *
     * Pattern letters:
     *   - `y`     proleptic year. Count is minimum digit width; `yy`
     *             takes the low two digits of the absolute year.
     *   - `M`     month. `M`/`MM` are numeric (padded to `count`);
     *             `MMM` short text, `MMMM` full text, `MMMMM` narrow.
     *             Text forms require ChronologyTextLike.
     *   - `d`     day-of-month. Numeric, padded to `count`.
     *   - `E`     day-of-week. `E`/`EE`/`EEE` short, `EEEE` full,
     *             `EEEEE` narrow.
     *   - `G`     era. `G`/`GG`/`GGG` short, `GGGG` full, `GGGGG`
     *             narrow. Requires ChronologyTextLike.
     *   - `'…'`   literal text. Use `''` for a single quote.
     *   - Any other punctuation is emitted verbatim.
     *
     * Time-of-day letters (`H`, `m`, `s`, `a`, ...) are reserved for
     * when a LocalTime / LocalDateTime is added.
     */
    class [[nodiscard]] DateTimeFormatter final {
    private:
        /**
         * @enum FieldKind
         * @brief Kind of date/time field encoded by a pattern token.
         */
        enum class FieldKind: u8 {
            LITERAL, ///< Verbatim text segment.
            YEAR, ///< Proleptic year ('y').
            MONTH, ///< Month ('M').
            DAY_OF_MONTH, ///< Day of month ('d').
            DAY_OF_WEEK, ///< Day of week ('E').
            ERA, ///< Era ('G').
        };

        /**
         * @struct Token
         * @brief A compiled pattern token.
         */
        struct Token final {
            FieldKind kind;
            u32 count; ///< Number of repetitions of the pattern letter.
            String literal; ///< Populated only when kind == LITERAL.
        };

        Vector<Token> tokens;
        String fmt_pattern;

        DateTimeFormatter(Vector<Token> tokens, String pattern) noexcept:
            tokens{Ops::move(tokens)}, fmt_pattern{Ops::move(pattern)} {}

        [[nodiscard]]
        THROWS(DateTimeException)
        static Vector<Token> parse_pattern(StringView p) {
            Vector<Token> tokens;
            String literal_buf;
            auto flush_literal = [&] -> void {
                if (!literal_buf.empty()) {
                    tokens.emplace_back(FieldKind::LITERAL, 0, Ops::move(literal_buf));
                    literal_buf.clear();
                }
            };
            const usize n = p.size();
            usize i = 0;
            while (i < n) {
                const char c = p[i];
                if (c == '\'') {
                    bool closed = false;
                    ++i;
                    while (i < n) {
                        if (p[i] == '\'') {
                            if (i + 1 < n && p[i + 1] == '\'') {
                                literal_buf.push_back('\'');
                                i += 2;
                            } else {
                                ++i;
                                closed = true;
                                break;
                            }
                        } else {
                            literal_buf.push_back(p[i]);
                            ++i;
                        }
                    }
                    if (!closed) {
                        throw DateTimeException("Unclosed quote in pattern");
                    }
                } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    flush_literal();
                    const char letter = c;
                    u32 count = 0;
                    while (i < n && p[i] == letter) {
                        ++count;
                        ++i;
                    }
                    FieldKind kind;
                    switch (letter) {
                        case 'y':
                            kind = FieldKind::YEAR;
                            break;
                        case 'M':
                            kind = FieldKind::MONTH;
                            break;
                        case 'd':
                            kind = FieldKind::DAY_OF_MONTH;
                            break;
                        case 'E':
                            kind = FieldKind::DAY_OF_WEEK;
                            break;
                        case 'G':
                            kind = FieldKind::ERA;
                            break;
                        default:
                            throw DateTimeException(stdx::fmt::format("Unsupported pattern letter: {}", letter));
                    }
                    tokens.emplace_back(kind, count, "");
                } else {
                    literal_buf.push_back(c);
                    ++i;
                }
            }
            flush_literal();
            return tokens;
        }

    public:
        /**
         * @brief Compile a pattern into a reusable formatter.
         * @param pattern The pattern string.
         * @returns The compiled formatter.
         * @throws DateTimeException on malformed patterns (unclosed
         * quote or unknown pattern letter).
         */
        [[nodiscard]]
        THROWS(DateTimeException)
        static DateTimeFormatter of_pattern(StringView pattern) {
            return DateTimeFormatter(parse_pattern(pattern), String(pattern));
        }

        /**
         * @brief Format a date using this pattern.
         * @tparam Chrono The chronology of the date. Patterns using
         * text fields (G, MMM+, E+) additionally require
         * ChronologyTextLike<Chrono>; a DateTimeException
         * is thrown if a text field is encountered for
         * a chronology lacking text data.
         * @param date The date to format.
         * @returns The formatted string.
         * @throws DateTimeException if the date contains out-of-range
         * values, or if a text field is encountered for a chronology
         * that does not satisfy ChronologyTextLike.
         */
        template <ChronologyLike Chrono>
        [[nodiscard]]
        THROWS(DateTimeException)
        String format(const ChronoLocalDate<Chrono>& date) const {
            String out;
            for (const Token& tok: tokens) {
                switch (tok.kind) {
                    case FieldKind::LITERAL:
                        out += tok.literal;
                        break;
                    case FieldKind::YEAR: {
                        const i32 y = date.year();
                        if (tok.count == 2) {
                            const u32 yy = static_cast<u32>(y < 0 ? -y : y) % 100u;
                            out += stdx::fmt::format("{:02d}", yy);
                        } else {
                            out += stdx::fmt::format("{:0{}d}", y, tok.count);
                        }
                        break;
                    }
                    case FieldKind::MONTH: {
                        const u32 m = date.month();
                        if (tok.count <= 2) {
                            out += stdx::fmt::format("{:0{}d}", m, tok.count);
                        } else if constexpr (ChronologyTextLike<Chrono>) {
                            const DateTextLength s = tok.count == 3 ? DateTextLength::SHORT
                                : tok.count == 4 ? DateTextLength::FULL
                                : DateTextLength::NARROW;
                            out += Chrono::month_name(date.year(), m, s);
                        } else {
                            throw DateTimeException("Chronology lacks textual month names (does not satisfy ChronologyTextLike)");
                        }
                        break;
                    }
                    case FieldKind::DAY_OF_MONTH:
                        out += stdx::fmt::format("{:0{}d}", date.day(), tok.count);
                        break;
                    case FieldKind::DAY_OF_WEEK: {
                        const DateTextLength s = tok.count <= 3 ? DateTextLength::SHORT
                            : tok.count == 4 ? DateTextLength::FULL
                            : DateTextLength::NARROW;
                        out += weekday_name(date.day_of_week(), s);
                        break;
                    }
                    case FieldKind::ERA: {
                        if constexpr (ChronologyTextLike<Chrono>) {
                            const auto era = Chrono::era_of(date.to_epoch_day());
                            const DateTextLength s = tok.count <= 3 ? DateTextLength::SHORT
                                : tok.count == 4 ? DateTextLength::FULL
                                : DateTextLength::NARROW;
                            out += Chrono::era_name(era, s);
                        } else {
                            throw DateTimeException("Chronology lacks textual era names (does not satisfy ChronologyTextLike)");
                        }
                        break;
                    }
                }
            }
            return out;
        }

        /**
         * @brief Parse a string into a date.
         * @tparam Chrono The chronology to interpret the parsed fields in.
         * @param text The input string.
         * @returns The parsed date.
         * @throws DateTimeException on parse failure or out-of-range values.
         *
         * @todo Parsing is not yet implemented. The signature is stable;
         *       the body currently throws unconditionally.
         */
        template <ChronologyLike Chrono>
        [[nodiscard]]
        THROWS(DateTimeException)
        ChronoLocalDate<Chrono> parse([[maybe_unused]] StringView text) const {
            throw DateTimeException("DateTimeFormatter::parse is not yet implemented");
        }

        /**
         * @brief Get the pattern this formatter was compiled from.
         */
        [[nodiscard]]
        StringView pattern() const noexcept {
            return fmt_pattern;
        }

        static const DateTimeFormatter ISO_LOCAL_DATE; ///< Predefined formatter for "yyyy-MM-dd".
        static const DateTimeFormatter BASIC_ISO_DATE; ///< Predefined formatter for "yyyyMMdd" (no separators).
    };

    inline const DateTimeFormatter DateTimeFormatter::ISO_LOCAL_DATE = DateTimeFormatter::of_pattern("yyyy-MM-dd");
    inline const DateTimeFormatter DateTimeFormatter::BASIC_ISO_DATE = DateTimeFormatter::of_pattern("yyyyMMdd");
}
