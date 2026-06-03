#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
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
        const Self value;
    public:
        constexpr OpenMode() noexcept = delete;

        constexpr OpenMode(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
        const Self value;
    public:
        constexpr FormatFlags() noexcept = delete;

        constexpr FormatFlags(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
        const Self value;
    public:
        constexpr IOState() noexcept = delete;

        constexpr IOState(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
        const Self value;
    public:
        constexpr SeekingDirection() noexcept = delete;

        constexpr SeekingDirection(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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
        const Self value;
    public:
        constexpr IOEvent() noexcept = delete;

        constexpr IOEvent(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
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

    using StreamOffset = std::streamoff;
    using StreamSize = std::streamsize;

    using IOException = std::ios_base::failure;

    using std::iostream_category;
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

    class ByteBuffer {
    private:
        Vector<u8> buffer;
        usize pos = 0;
        usize lim = 0;
    public:
        ByteBuffer() = default;

        explicit ByteBuffer(usize capacity):
            buffer(capacity), lim{capacity} {}

        ByteBuffer(usize capacity, u8 value):
            buffer(capacity, value), lim{capacity} {}

        explicit ByteBuffer(const Vector<u8>& data):
            buffer{data}, lim{data.size()} {}

        explicit ByteBuffer(Vector<u8>&& data):
            buffer{std::move(data)}, lim{buffer.size()} {}

        explicit ByteBuffer(Span<const u8> data):
            buffer(data.begin(), data.end()), lim{data.size()} {}

        template <typename It>
        ByteBuffer(It first, It last):
            buffer(first, last), lim{buffer.size()} {}

        ByteBuffer(InitializerList<u8> data):
            buffer{data}, lim{data.size()} {}

        [[nodiscard]]
        usize position() const noexcept {
            return pos;
        }

        [[nodiscard]]
        usize remaining() const noexcept {
            return pos < lim ? lim - pos : 0;
        }

        [[nodiscard]]
        usize capacity() const noexcept {
            return buffer.size();
        }

        [[nodiscard]]
        usize size() const noexcept {
            return lim;
        }

        [[nodiscard]]
        usize limit() const noexcept {
            return lim;
        }

        void flip() noexcept {
            if (pos > lim) {
                pos = lim;
            }
            lim = pos;
            pos = 0;
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return lim == 0;
        }

        [[nodiscard]]
        u8& operator[](usize index) noexcept {
            return buffer[index];
        }

        [[nodiscard]]
        const u8& operator[](usize index) const noexcept {
            return buffer[index];
        }

        [[nodiscard]]
        u8* data() noexcept {
            return buffer.data();
        }

        [[nodiscard]]
        const u8* data() const noexcept {
            return buffer.data();
        }

        void advance(usize count) throws (OutOfRangeException) {
            if (count > remaining()) {
                throw OutOfRangeException("ByteBuffer overflow");
            }
            pos += count;
        }

        [[nodiscard]]
        u8* begin() noexcept {
            return buffer.data();
        }

        [[nodiscard]]
        const u8* begin() const noexcept {
            return buffer.data();
        }

        [[nodiscard]]
        u8* end() noexcept {
            return buffer.data() + static_cast<ptrdiff>(lim);
        }

        [[nodiscard]]
        const u8* end() const noexcept {
            return buffer.data() + static_cast<ptrdiff>(lim);
        }

        void clear() noexcept {
            buffer.clear();
            pos = 0;
            lim = 0;
        }

        void resize(usize new_size) {
            buffer.resize(new_size);
            lim = new_size;
            if (pos > lim) {
                pos = lim;
            }
        }

        template <typename It>
        void assign(It first, It last) {
            buffer.assign(first, last);
            pos = 0;
            lim = buffer.size();
        }

        [[nodiscard]]
        u8 get() throws (OutOfRangeException) {
            if (pos >= lim) {
                throw OutOfRangeException("ByteBuffer underflow");
            }
            return buffer[pos++];
        }

        void put(u8 byte) throws (OutOfRangeException) {
            if (pos >= lim) {
                throw OutOfRangeException("ByteBuffer overflow");
            }
            buffer[pos++] = byte;
        }


        [[nodiscard]]
        Span<const u8> span() const noexcept {
            return Span<const u8>(buffer.data() + pos, remaining());
        }

        [[nodiscard]]
        Span<u8> mutable_span() noexcept {
            return Span<u8>(buffer.data() + pos, remaining());
        }
    };

    /**
     * @class InterruptedIOException
     * @brief Exception class for handling interrupted I/O operations.
     *
     * @extends IOException
     * 
     * The InterruptedIOException class is used to represent an exception that occurs when an I/O operation is interrupted.
     */
    class InterruptedIOException: public IOException {
    public:
        /**
         * @brief Constructor that initialises the exception with a message.
         * @param msg The exception message.
         */
        explicit InterruptedIOException(const String& msg = ""):
            IOException(msg) {}
    };
}

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    template <typename T>
    using IsErrorCodeEnum = std::is_error_code_enum<T>;
}
