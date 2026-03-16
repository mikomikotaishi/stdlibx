#pragma once

using core::meta::DecayType;

using stdx::collections::BitSet;

namespace stdx::fmt {
    #if defined(_WIN32)
    inline constexpr StringView LINE_SEPARATOR = "\r\n"sv;
    #else
    inline constexpr StringView LINE_SEPARATOR = "\n"sv;
    #endif

    // Convert a Java-style printf format string to a std::format "{}" format string.
    // Handles: %d %i %f %e %E %g %G %a %A %s %c %b %B %x %X %o %h %H %n %%
    // Flags:   - (left-align) + (force sign) ' ' (space) 0 (zero-pad) # (alt) , ( (ignored)
    // Supports positional args: %1$d -> {0:d}

    [[nodiscard]]
    inline String printf_to_fmt(StringView fmt) {
        String out;
        out.reserve(fmt.size() * 2);

        const char* p = fmt.data();
        const char* end = p + fmt.size();

        while (p < end) {
            if (*p != '%') {
                if (*p == '{' || *p == '}') {
                    out += *p;
                }
                out += *p++;
                continue;
            }
            ++p;
            if (p >= end) {
                out += '%';
                break;
            }
            if (*p == '%') {
                out += '%';
                ++p;
                continue;
            }
            if (*p == 'n') {
                out += LINE_SEPARATOR;
                ++p;
                continue;
            }

            out += '{';

            const char* q = p;
            while (q < end && *q >= '0' && *q <= '9') ++q;
            if (q < end && *q == '$' && q > p) {
                usize idx = 0;
                for (const char* r = p; r < q; ++r) {
                    idx = idx * 10 + static_cast<usize>(*r - '0');
                }
                out += UnsignedSize::to_string(idx - 1); // 0-based
                p = q + 1;
            }

            // [0] = left-align, [1] = force-sign, [2] = space-sign, [3] = zero-pad, [4] = alt-form
            BitSet<5> flags;
            for (; p < end; ++p) {
                switch (*p) {
                    case '-':
                        flags.set(0);
                        continue;
                    case '+':
                        flags.set(1);
                        continue;
                    case ' ':
                        flags.set(2);
                        continue;
                    case '0':
                        flags.set(3);
                        continue;
                    case '#':
                        flags.set(4);
                        continue;
                    case ',':
                    case '(':
                        continue; // no fmtlib equivalent
                }
                break;
            }

            const char* wstart = p;
            while (p < end && *p >= '0' && *p <= '9') {
                ++p;
            }
            const char* wend = p;

            const char* prstart = nullptr;
            const char* prend = nullptr;
            if (p < end && *p == '.') {
                ++p;
                prstart = p;
                while (p < end && *p >= '0' && *p <= '9') {
                    ++p;
                }
                prend = p;
            }

            if (p >= end) {
                out += '}';
                break;
            }
            char conv = *p++;

            out += ':';
            if (flags[0]) {
                out += '<';
            }
            if (flags[1]) {
                out += '+';
            } else if (flags[2]) {
                out += ' ';
            }
            if (flags[4] && (conv == 'x' || conv == 'X' || conv == 'o' || conv == 'a' || conv == 'A')) {
                out += '#';
            }
            if (flags[3] && !flags[0] && wstart < wend) {
                out += '0';
            }
            out.append(wstart, static_cast<usize>(wend - wstart));
            if (prstart) {
                out += '.';
                out.append(prstart, static_cast<usize>(prend - prstart));
            }

            switch (conv) {
                case 'd':
                case 'i':
                    out += 'd';
                    break;
                case 'f':
                    out += 'f';
                    break;
                case 'e':
                    out += 'e';
                    break;
                case 'E':
                    out += 'E';
                    break;
                case 'g':
                    out += 'g';
                    break;
                case 'G':
                    out += 'G';
                    break;
                case 'a':
                    out += 'a';
                    break;
                case 'A':
                    out += 'A';
                    break;
                case 'c':
                    out += 'c';
                    break;
                case 'x':
                    out += 'x';
                    break;
                case 'X':
                    out += 'X';
                    break;
                case 'o':
                    out += 'o';
                    break;
                case 'h':
                    out += 'x';
                    break; // hash -> lowercase hex
                case 'H':
                    out += 'X';
                    break; // hash -> uppercase hex
                case 's':
                case 'b':
                case 'B':
                default:
                    break; // no type letter needed
            }
            out += '}';
        }
        return out;
    }
}

/**
 * @namespace stdx::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
export namespace stdx::fmt {
    //
    // Supported specifiers:
    //   %d / %i  — integer (decimal)
    //   %f       — floating-point (fixed)
    //   %e / %E  — scientific notation
    //   %g / %G  — general float
    //   %a / %A  — hex float
    //   %s       — string
    //   %c       — character
    //   %b / %B  — boolean / any (no type letter)
    //   %x / %X  — hex integer
    //   %o       — octal
    //   %h / %H  — hash (-> hex)
    //   %n       — newline (literal)
    //   %%       — literal percent
    //
    // Flags:
    //   - (left-align)
    //   + (force sign)
    //  ' ' (space sign)
    //   0 (zero-pad)
    //   # (alt form)
    //   , ( (accepted, ignored)
    //
    // Positional args: %1$d, %2$s, ...  (1-based, like Java)
    //
    // Compile-time validation:
    //   - All conversion characters must be from the supported set.
    //   - Sequential usage: specifier count must equal sizeof...(Args).
    //   - Positional/sequential mixing is rejected.
    //   - Integer specifiers require Integral argument types.
    //   - Floating-point specifiers require FloatingPoint argument types.

    template <typename... Args>
    class PrintfString {
    private:
        StringView fmt;

        [[nodiscard]]
        static consteval bool is_valid_specifier(char c) noexcept {
            switch (c) {
                case 'd':
                case 'i':
                case 'f':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A':
                case 's':
                case 'c':
                case 'b':
                case 'B':
                case 'x':
                case 'X':
                case 'o':
                case 'h':
                case 'H':
                    return true;
                default:
                    return false;
            }
        }
    
        // Per-argument type check (called for each sequential/positional specifier).
        template <usize I>
        static consteval void check_type(char conv) throws (FormatException) {
            using T = DecayType<TupleElementType<I, Tuple<Args...>>>;
            switch (conv) {
                case 'd':
                case 'i':
                case 'x':
                case 'X':
                case 'o':
                case 'h':
                case 'H':
                    if constexpr (!Integral<T>) {
                        throw FormatException("Integer specifier (%d/%i/%x/%o/...) requires an integral argument");
                    }
                    break;
                case 'f':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A':
                    if constexpr (!FloatingPoint<T>) {
                        throw FormatException("Floating-point specifier (%f/%e/%g/...) requires a floating-point argument");
                    }
                    break;
                case 'c':
                    if constexpr (!Integral<T>) {
                        throw FormatException("Character specifier (%c) requires an integral (character) argument");
                    }
                    break;
                default:
                    break; // %s %b %B: no type restriction
            }
        }

        // Dispatch check_type<I> for each index in Idxss... at position idx.
        template <usize... Idxs>
        static consteval void dispatch_check(usize idx, char conv, IndexSequence<Idxs...>) {
            (void)((Idxs == idx ? (check_type<Idxs>(conv), true) : false) || ...);
        }

        static consteval void validate(const char fmt[], usize len) throws (FormatException) {
            const char* p = fmt;
            const char* end = p + len;
            usize seq_count = 0;
            bool uses_positional = false;
            bool uses_sequential = false;

            while (p < end) {
                if (*p++ != '%') {
                    continue;
                }
                if (p >= end) {
                    throw FormatException("Trailing '%' at end of format string");
                }
                if (*p == '%' || *p == 'n') {
                    ++p;
                    continue;
                }

                // Detect positional arg index (e.g. "1$")
                const char* q = p;
                while (q < end && *q >= '0' && *q <= '9') {
                    ++q;
                }
                usize pos_idx = 0;
                bool positional = (q < end && *q == '$' && q > p);
                if (positional) {
                    for (const char* r = p; r < q; ++r) {
                        pos_idx = pos_idx * 10 + static_cast<usize>(*r - '0');
                    }
                    if (pos_idx == 0) {
                        throw FormatException("Positional argument index must be >= 1");
                    }
                    if (pos_idx > sizeof...(Args)) {
                        throw FormatException("Positional argument index out of range");
                    }
                    uses_positional = true;
                    p = q + 1;
                } else {
                    uses_sequential = true;
                }

                if (uses_positional && uses_sequential) {
                    throw FormatException("Cannot mix positional (%n$...) and sequential (%...) format arguments");
                }

                while (p < end && 
                    (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#' || *p == ',' || *p == '(')
                ) {
                    ++p;
                }
                while (p < end && *p >= '0' && *p <= '9') {
                    ++p;
                }
                if (p < end && *p == '.') {
                    ++p;
                    while (p < end && *p >= '0' && *p <= '9') ++p;
                }

                if (p >= end) {
                    throw FormatException("Missing conversion character after format flags");
                }
                char conv = *p++;

                if (!is_valid_specifier(conv)) {
                    throw FormatException("Unsupported conversion specifier");
                }

                usize check_idx = positional ? (pos_idx - 1) : seq_count;
                dispatch_check(check_idx, conv, IndexSequenceOf<sizeof...(Args)>{});

                if (!positional) {
                    ++seq_count;
                }
            }

            if (uses_sequential && seq_count != sizeof...(Args)) {
                throw FormatException("Number of format specifiers does not match number of arguments");
            }
        }
    public:
        template <usize N>
        consteval PrintfString(const char (&s)[N]):
            fmt(s, N - 1) {
            validate(s, N - 1);
        }

        [[nodiscard]]
        constexpr StringView get() const noexcept {
            return fmt;
        }
    };
}
