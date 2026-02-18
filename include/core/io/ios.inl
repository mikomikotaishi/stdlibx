#pragma once

/**
 * @namespace core::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace core::io {
    using IOSBase = std::ios_base;
    using IOS = std::ios;

    class [[nodiscard]] OpenMode final {
    public:
        using Self = std::ios::openmode;

        static constexpr Self APPEND = std::ios::app;
        static constexpr Self BINARY = std::ios::binary;
        static constexpr Self IN = std::ios::in;
        static constexpr Self READ = std::ios::in;
        static constexpr Self OUT = std::ios::out;
        static constexpr Self WRITE = std::ios::out;
        static constexpr Self TRUNCATE = std::ios::trunc;
        static constexpr Self AT_END = std::ios::ate;
        static constexpr Self NO_REPLACE = std::ios::noreplace;
    private:
        Self value;
    public:
        constexpr OpenMode(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] FormatFlags final {
    public:
        using Self = std::ios::fmtflags;

        static constexpr Self DECIMAL = std::ios::dec;
        static constexpr Self OCTAL = std::ios::oct;
        static constexpr Self HEXADECIMAL = std::ios::hex;
        static constexpr Self BASE_FIELD = std::ios::basefield;
        static constexpr Self LEFT = std::ios::left;
        static constexpr Self RIGHT = std::ios::right;
        static constexpr Self INTERNAL = std::ios::internal;
        static constexpr Self ADJUST_FIELD = std::ios::adjustfield;
        static constexpr Self SCIENTIFIC = std::ios::scientific;
        static constexpr Self FIXED = std::ios::fixed;
        static constexpr Self FLOAT_FIELD = std::ios::floatfield;
        static constexpr Self BOOL_ALPHA = std::ios::boolalpha;
        static constexpr Self SHOW_BASE = std::ios::showbase;
        static constexpr Self SHOW_DECIMAL = std::ios::showpoint;
        static constexpr Self SHOW_POLARITY = std::ios::showpos;
        static constexpr Self SKIP_WHITESPACE = std::ios::skipws;
        static constexpr Self UNIT_BUFFER = std::ios::unitbuf;
        static constexpr Self UPPERCASE = std::ios::uppercase;
    private:
        Self value;
    public:
        constexpr FormatFlags(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] IOState final {
    public:
        using Self = std::ios::iostate;

        static constexpr Self GOOD_BIT = std::ios::goodbit;
        static constexpr Self BAD_BIT = std::ios::badbit;
        static constexpr Self FAIL_BIT = std::ios::failbit;
        static constexpr Self EOF_BIT = std::ios::eofbit;
        static constexpr Self NO_ERROR = std::ios::goodbit;
        static constexpr Self STREAM_ERROR = std::ios::badbit;
        static constexpr Self IO_OPERATION_FAIL = std::ios::failbit;
        static constexpr Self END_OF_FILE = std::ios::eofbit;
    private:
        Self value;
    public:
        constexpr IOState(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] SeekingDirection final {
    public:
        using Self = std::ios::seekdir;

        static constexpr Self BEGINNING = std::ios::beg;
        static constexpr Self ENDING = std::ios::end;
        static constexpr Self CURRENT = std::ios::cur;
    private:
        Self value;
    public:
        constexpr SeekingDirection(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] IOEvent final {
    public:
        using Self = std::ios::event;

        static constexpr Self ERASE_EVENT = std::ios::event::erase_event;
        static constexpr Self IMBUE_EVENT = std::ios::event::imbue_event;
        static constexpr Self COPY_FORMAT_EVENT = std::ios::event::copyfmt_event;
    private:
        Self value;
    public:
        constexpr IOEvent(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };
    
    using IOEventCallback = std::ios::event_callback;

    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator~;
    using std::operator|=;
    using std::operator&=;
    using std::operator^=;

    using WideIOS = std::wios;

    template <typename StateT>
    using FilePosition = std::fpos<StateT>;

    class [[nodiscard]] IOErrc final {
    public:
        using Self = std::io_errc;

        static constexpr Self SUCCESS = std::io_errc();
        static constexpr Self STREAM = std::io_errc::stream;
    private:
        Self value;
    public:
        constexpr IOErrc(Self value = SUCCESS) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    template <typename T>
    using IsErrorCodeEnum = std::is_error_code_enum<T>;

    using StreamOffset = std::streamoff;
    using StreamSize = std::streamsize;

    using IOException = std::ios_base::failure;

    using std::iostream_category;
    using std::make_error_code;
    using std::make_error_condition;
    using std::boolalpha;
    using std::noboolalpha;
    using std::showbase;
    using std::noshowbase;
    using std::showpoint;
    using std::noshowpoint;
    using std::showpos;
    using std::noshowpos;
    using std::skipws;
    using std::noskipws;
    using std::uppercase;
    using std::nouppercase;
    using std::unitbuf;
    using std::nounitbuf;
    using std::internal;
    using std::left;
    using std::right;
    using std::dec;
    using std::hex;
    using std::oct;
    using std::fixed;
    using std::scientific;
    using std::hexfloat;
    using std::defaultfloat;
}

/**
 * @namespace core
 * @brief Wrapper namespace for standard library core operations.
 */
export namespace core {
    using core::io::IOErrc;
}
