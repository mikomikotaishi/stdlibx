#pragma once

using stdx::fmt::FormatString;

namespace _detail {
    [[nodiscard]]
    bool should_use_color() noexcept {
        #if defined(_WIN32)
        if (!stdx::os::windows::_isatty(stdx::os::windows::_fileno(stdout))) {
            return false;
        }

        HANDLE h = stdx::os::windows::GetStdHandle(stdx::os::windows::STD_OUTPUT_HANDLE);
        if (h == stdx::os::windows::INVALID_HANDLE_VALUE) {
            return false;
        }

        DWORD mode = 0;
        if (!stdx::os::windows::GetConsoleMode(h, &mode)) {
            return false;
        }

        mode |= stdx::os::windows::ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!stdx::os::windows::SetConsoleMode(h, mode)) {
            return false;
        }

        return true;
        #else
        if (!stdx::os::unix::isatty(stdx::os::unix::STDOUT_FILENO)) {
            return false;
        }

        const char* term = core::sys::getenv("TERM");
        if (!term || stdx::core::StringView(term) == "dumb") {
            return false;
        }

        return true;
        #endif
    }
}

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    #ifdef __cpp_lib_print
    using std::print;
    using std::println;
    using std::vprint_unicode;
    using std::vprint_nonunicode;

    #if 0
    template <typename... Args>
    void printf(FormatString<Args...> fmt, Args&&... args) {
        
    }

    template <typename... Args>
    void printf(File* stream, FormatString<Args...> fmt, Args&&... args) {
        
    }
    #endif

    class TextStyle {
    public:
        enum class Color: u32 {
            ALICE_BLUE = 0xF0F8FF,               // rgb(240,248,255)
            ANTIQUE_WHITE = 0xFAEBD7,            // rgb(250,235,215)
            AQUA = 0x00FFFF,                     // rgb(0,255,255)
            AQUAMARINE = 0x7FFFD4,               // rgb(127,255,212)
            AZURE = 0xF0FFFF,                    // rgb(240,255,255)
            BEIGE = 0xF5F5DC,                    // rgb(245,245,220)
            BISQUE = 0xFFE4C4,                   // rgb(255,228,196)
            BLACK = 0x000000,                    // rgb(0,0,0)
            BLANCHED_ALMOND = 0xFFEBCD,          // rgb(255,235,205)
            BLUE = 0x0000FF,                     // rgb(0,0,255)
            BLUE_VIOLET = 0x8A2BE2,              // rgb(138,43,226)
            BROWN = 0xA52A2A,                    // rgb(165,42,42)
            BURLY_WOOD = 0xDEB887,               // rgb(222,184,135)
            CADET_BLUE = 0x5F9EA0,               // rgb(95,158,160)
            CHARTREUSE = 0x7FFF00,               // rgb(127,255,0)
            CHOCOLATE = 0xD2691E,                // rgb(210,105,30)
            CORAL = 0xFF7F50,                    // rgb(255,127,80)
            CORNFLOWER_BLUE = 0x6495ED,          // rgb(100,149,237)
            CORNSILK = 0xFFF8DC,                 // rgb(255,248,220)
            CRIMSON = 0xDC143C,                  // rgb(220,20,60)
            CYAN = 0x00FFFF,                     // rgb(0,255,255)
            DARK_BLUE = 0x00008B,                // rgb(0,0,139)
            DARK_CYAN = 0x008B8B,                // rgb(0,139,139)
            DARK_GOLDEN_ROD = 0xB8860B,          // rgb(184,134,11)
            DARK_GRAY = 0xA9A9A9,                // rgb(169,169,169)
            DARK_GREEN = 0x006400,               // rgb(0,100,0)
            DARK_KHAKI = 0xBDB76B,               // rgb(189,183,107)
            DARK_MAGENTA = 0x8B008B,             // rgb(139,0,139)
            DARK_OLIVE_GREEN = 0x556B2F,         // rgb(85,107,47)
            DARK_ORANGE = 0xFF8C00,              // rgb(255,140,0)
            DARK_ORCHID = 0x9932CC,              // rgb(153,50,204)
            DARK_RED = 0x8B0000,                 // rgb(139,0,0)
            DARK_SALMON = 0xE9967A,              // rgb(233,150,122)
            DARK_SEA_GREEN = 0x8FBC8F,           // rgb(143,188,143)
            DARK_SLATE_BLUE = 0x483D8B,          // rgb(72,61,139)
            DARK_SLATE_GRAY = 0x2F4F4F,          // rgb(47,79,79)
            DARK_TURQUOISE = 0x00CED1,           // rgb(0,206,209)
            DARK_VIOLET = 0x9400D3,              // rgb(148,0,211)
            DEEP_PINK = 0xFF1493,                // rgb(255,20,147)
            DEEP_SKY_BLUE = 0x00BFFF,            // rgb(0,191,255)
            DIM_GRAY = 0x696969,                 // rgb(105,105,105)
            DODGER_BLUE = 0x1E90FF,              // rgb(30,144,255)
            FIRE_BRICK = 0xB22222,               // rgb(178,34,34)
            FLORAL_WHITE = 0xFFFAF0,             // rgb(255,250,240)
            FOREST_GREEN = 0x228B22,             // rgb(34,139,34)
            FUCHSIA = 0xFF00FF,                  // rgb(255,0,255)
            GAINSBORO = 0xDCDCDC,                // rgb(220,220,220)
            GHOST_WHITE = 0xF8F8FF,              // rgb(248,248,255)
            GOLD = 0xFFD700,                     // rgb(255,215,0)
            GOLDEN_ROD = 0xDAA520,               // rgb(218,165,32)
            GRAY = 0x808080,                     // rgb(128,128,128)
            GREEN = 0x008000,                    // rgb(0,128,0)
            GREEN_YELLOW = 0xADFF2F,             // rgb(173,255,47)
            HONEY_DEW = 0xF0FFF0,                // rgb(240,255,240)
            HOT_PINK = 0xFF69B4,                 // rgb(255,105,180)
            INDIAN_RED = 0xCD5C5C,               // rgb(205,92,92)
            INDIGO = 0x4B0082,                   // rgb(75,0,130)
            IVORY = 0xFFFFF0,                    // rgb(255,255,240)
            KHAKI = 0xF0E68C,                    // rgb(240,230,140)
            LAVENDER = 0xE6E6FA,                 // rgb(230,230,250)
            LAVENDER_BLUSH = 0xFFF0F5,           // rgb(255,240,245)
            LAWN_GREEN = 0x7CFC00,               // rgb(124,252,0)
            LEMON_CHIFFON = 0xFFFACD,            // rgb(255,250,205)
            LIGHT_BLUE = 0xADD8E6,               // rgb(173,216,230)
            LIGHT_CORAL = 0xF08080,              // rgb(240,128,128)
            LIGHT_CYAN = 0xE0FFFF,               // rgb(224,255,255)
            LIGHT_GOLDEN_ROD_YELLOW = 0xFAFAD2,  // rgb(250,250,210)
            LIGHT_GRAY = 0xD3D3D3,               // rgb(211,211,211)
            LIGHT_GREEN = 0x90EE90,              // rgb(144,238,144)
            LIGHT_PINK = 0xFFB6C1,               // rgb(255,182,193)
            LIGHT_SALMON = 0xFFA07A,             // rgb(255,160,122)
            LIGHT_SEA_GREEN = 0x20B2AA,          // rgb(32,178,170)
            LIGHT_SKY_BLUE = 0x87CEFA,           // rgb(135,206,250)
            LIGHT_SLATE_GRAY = 0x778899,         // rgb(119,136,153)
            LIGHT_STEEL_BLUE = 0xB0C4DE,         // rgb(176,196,222)
            LIGHT_YELLOW = 0xFFFFE0,             // rgb(255,255,224)
            LIME = 0x00FF00,                     // rgb(0,255,0)
            LIME_GREEN = 0x32CD32,               // rgb(50,205,50)
            LINEN = 0xFAF0E6,                    // rgb(250,240,230)
            MAGENTA = 0xFF00FF,                  // rgb(255,0,255)
            MAROON = 0x800000,                   // rgb(128,0,0)
            MEDIUM_AQUAMARINE = 0x66CDAA,        // rgb(102,205,170)
            MEDIUM_BLUE = 0x0000CD,              // rgb(0,0,205)
            MEDIUM_ORCHID = 0xBA55D3,            // rgb(186,85,211)
            MEDIUM_PURPLE = 0x9370DB,            // rgb(147,112,219)
            MEDIUM_SEA_GREEN = 0x3CB371,         // rgb(60,179,113)
            MEDIUM_SLATE_BLUE = 0x7B68EE,        // rgb(123,104,238)
            MEDIUM_SPRING_GREEN = 0x00FA9A,      // rgb(0,250,154)
            MEDIUM_TURQUOISE = 0x48D1CC,         // rgb(72,209,204)
            MEDIUM_VIOLET_RED = 0xC71585,        // rgb(199,21,133)
            MIDNIGHT_BLUE = 0x191970,            // rgb(25,25,112)
            MINT_CREAM = 0xF5FFFA,               // rgb(245,255,250)
            MISTY_ROSE = 0xFFE4E1,               // rgb(255,228,225)
            MOCCASIN = 0xFFE4B5,                 // rgb(255,228,181)
            NAVAJO_WHITE = 0xFFDEAD,             // rgb(255,222,173)
            NAVY = 0x000080,                     // rgb(0,0,128)
            OLD_LACE = 0xFDF5E6,                 // rgb(253,245,230)
            OLIVE = 0x808000,                    // rgb(128,128,0)
            OLIVE_DRAB = 0x6B8E23,               // rgb(107,142,35)
            ORANGE = 0xFFA500,                   // rgb(255,165,0)
            ORANGE_RED = 0xFF4500,               // rgb(255,69,0)
            ORCHID = 0xDA70D6,                   // rgb(218,112,214)
            PALE_GOLDEN_ROD = 0xEEE8AA,          // rgb(238,232,170)
            PALE_GREEN = 0x98FB98,               // rgb(152,251,152)
            PALE_TURQUOISE = 0xAFEEEE,           // rgb(175,238,238)
            PALE_VIOLET_RED = 0xDB7093,          // rgb(219,112,147)
            PAPAYA_WHIP = 0xFFEFD5,              // rgb(255,239,213)
            PEACH_PUFF = 0xFFDAB9,               // rgb(255,218,185)
            PERU = 0xCD853F,                     // rgb(205,133,63)
            PINK = 0xFFC0CB,                     // rgb(255,192,203)
            PLUM = 0xDDA0DD,                     // rgb(221,160,221)
            POWDER_BLUE = 0xB0E0E6,              // rgb(176,224,230)
            PURPLE = 0x800080,                   // rgb(128,0,128)
            REBECCA_PURPLE = 0x663399,           // rgb(102,51,153)
            RED = 0xFF0000,                      // rgb(255,0,0)
            ROSY_BROWN = 0xBC8F8F,               // rgb(188,143,143)
            ROYAL_BLUE = 0x4169E1,               // rgb(65,105,225)
            SADDLE_BROWN = 0x8B4513,             // rgb(139,69,19)
            SALMON = 0xFA8072,                   // rgb(250,128,114)
            SANDY_BROWN = 0xF4A460,              // rgb(244,164,96)
            SEA_GREEN = 0x2E8B57,                // rgb(46,139,87)
            SEA_SHELL = 0xFFF5EE,                // rgb(255,245,238)
            SIENNA = 0xA0522D,                   // rgb(160,82,45)
            SILVER = 0xC0C0C0,                   // rgb(192,192,192)
            SKY_BLUE = 0x87CEEB,                 // rgb(135,206,235)
            SLATE_BLUE = 0x6A5ACD,               // rgb(106,90,205)
            SLATE_GRAY = 0x708090,               // rgb(112,128,144)
            SNOW = 0xFFFAFA,                     // rgb(255,250,250)
            SPRING_GREEN = 0x00FF7F,             // rgb(0,255,127)
            STEEL_BLUE = 0x4682B4,               // rgb(70,130,180)
            TAN = 0xD2B48C,                      // rgb(210,180,140)
            TEAL = 0x008080,                     // rgb(0,128,128)
            THISTLE = 0xD8BFD8,                  // rgb(216,191,216)
            TOMATO = 0xFF6347,                   // rgb(255,99,71)
            TURQUOISE = 0x40E0D0,                // rgb(64,224,208)
            VIOLET = 0xEE82EE,                   // rgb(238,130,238)
            WHEAT = 0xF5DEB3,                    // rgb(245,222,179)
            WHITE = 0xFFFFFF,                    // rgb(255,255,255)
            WHITE_SMOKE = 0xF5F5F5,              // rgb(245,245,245)
            YELLOW = 0xFFFF00,                   // rgb(255,255,0)
            YELLOW_GREEN = 0x9ACD32,             // rgb(154,205,50)
        };

        enum class TerminalColor: u8 {
            BLACK = 30,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            WHITE,
            BRIGHT_BLACK = 90,
            BRIGHT_RED,
            BRIGHT_GREEN,
            BRIGHT_YELLOW,
            BRIGHT_BLUE,
            BRIGHT_MAGENTA,
            BRIGHT_CYAN,
            BRIGHT_WHITE,
        };

        enum class Emphasis {
            BOLD = 1,
            FAINT = 1 << 1,
            ITALIC = 1 << 2,
            UNDERLINE = 1 << 3,
            BLINK = 1 << 4,
            REVERSE = 1 << 5,
            CONCEAL = 1 << 6,
            STRIKETHROUGH = 1 << 7,
        };

        struct RGB {
            u8 r;
            u8 g;
            u8 b;

            constexpr RGB(u8 r = 0, u8 g = 0, u8 b = 0) noexcept:
                r{r}, g{g}, b{b} {}
            constexpr RGB(u32 hex) noexcept:
                r{static_cast<u8>((hex >> 16) & 0xFF)},
                g{static_cast<u8>((hex >> 8) & 0xFF)},
                b{static_cast<u8>(hex & 0xFF)} {}
            constexpr RGB(Color color) noexcept:
                RGB(stdx::util::to_underlying(color)) {}
        };
    private:
        // 
    public:
        
    };

    #if 0

    // Implementations for text style will go here
    template <typename... Args>
    void print(TextStyle ts, FormatString<Args...> fmt, Args&&... args) {

    }

    template <typename... Args>
    void print(File* stream, TextStyle ts, FormatString<Args...> fmt, Args&&... args) {

    }

    template <typename... Args>
    void println(TextStyle ts, FormatString<Args...> fmt, Args&&... args) {

    }

    template <typename... Args>
    void println(File* stream, TextStyle ts, FormatString<Args...> fmt, Args&&... args) {

    }

    template <typename... Args>
    void printf(TextStyle ts, FormatString<Args...> fmt, Args&&... args) {
        
    }

    template <typename... Args>
    void printf(File* stream, TextStyle ts, FormatString<Args...> fmt, Args&&... args) {
        
    }
    #endif
    #endif
}
