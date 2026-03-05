#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    using ::core::io::IOSBase;
    using ::core::io::IOS;

    using ::core::io::OpenMode;
    using ::core::io::FormatFlags;
    using ::core::io::IOState;
    using ::core::io::SeekingDirection;
    using ::core::io::IOEvent;
    using ::core::io::IOEventCallback;

    using ::core::io::operator&;
    using ::core::io::operator|;
    using ::core::io::operator^;
    using ::core::io::operator~;
    using ::core::io::operator|=;
    using ::core::io::operator&=;
    using ::core::io::operator^=;

    using ::core::io::WideIOS;
    using ::core::io::FilePosition;
    using ::core::io::IOErrc;
    using ::core::io::IsErrorCodeEnum;

    using ::core::io::StreamOffset;
    using ::core::io::StreamSize;

    using ::core::io::IOException;

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
            buffer{stdx::util::move(data)}, lim{buffer.size()} {}

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

    using ::core::io::iostream_category;
    using ::core::io::make_error_code;
    using ::core::io::make_error_condition;
    using ::core::io::boolalpha;
    using ::core::io::noboolalpha;
    using ::core::io::showbase;
    using ::core::io::noshowbase;
    using ::core::io::showpoint;
    using ::core::io::noshowpoint;
    using ::core::io::showpos;
    using ::core::io::noshowpos;
    using ::core::io::skipws;
    using ::core::io::noskipws;
    using ::core::io::uppercase;
    using ::core::io::nouppercase;
    using ::core::io::unitbuf;
    using ::core::io::nounitbuf;
    using ::core::io::internal;
    using ::core::io::left;
    using ::core::io::right;
    using ::core::io::dec;
    using ::core::io::hex;
    using ::core::io::oct;
    using ::core::io::fixed;
    using ::core::io::scientific;
    using ::core::io::hexfloat;
    using ::core::io::defaultfloat;
}

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for standard library core operations.
 */
export namespace stdx::core {
    using stdx::io::IOErrc;
    
    using ::core::make_error_code;
    using ::core::make_error_condition;
}
