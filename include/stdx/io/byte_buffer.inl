#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::io
 * @brief Standard library IO operations.
 */
export namespace stdx::io {
    class ByteBuffer {
    private:
        Vector<u8> buffer;
        usize pos = 0;
        usize lim = 0;
        Optional<usize> marked = nullopt; ///< The position @ref reset returns to, if one was set.

        /**
         * @internal
         * @brief Reads @p T from the current position in @p order.
         * @tparam T The unsigned type to assemble.
         * @param order The byte order the bytes are stored in.
         * @return The assembled value.
         * @throws OutOfRangeException if fewer than sizeof(T) bytes remain.
         *
         * Assembled most-significant byte first and byte-swapped afterwards for
         * the little-endian case, so the result does not depend on the host's
         * own order.
         */
        template <UnsignedIntegral T>
        THROWS(OutOfRangeException)
        T read_integer(Endian order) {
            static constexpr usize WIDTH = sizeof(T);
            if (remaining() < WIDTH) {
                throw OutOfRangeException("ByteBuffer underflow");
            }

            T value = 0;
            for (usize index = 0; index < WIDTH; ++index) {
                value = static_cast<T>((value << 8) | buffer[pos + index]);
            }
            pos += WIDTH;
            return (order == Endian::BIG)
                ? value
                : Math::byteswap(value);
        }

        /**
         * @internal
         * @brief Writes @p value at the current position in @p order.
         * @tparam T The unsigned type to lay down.
         * @param value The value to write.
         * @param order The byte order to write it in.
         * @throws OutOfRangeException if fewer than sizeof(T) bytes remain.
         */
        template <UnsignedIntegral T>
        THROWS(OutOfRangeException)
        void write_integer(T value, Endian order) {
            static constexpr usize WIDTH = sizeof(T);
            if (remaining() < WIDTH) {
                throw OutOfRangeException("ByteBuffer overflow");
            }

            const T ordered = (order == Endian::BIG)
                ? value
                : Math::byteswap(value);
            for (usize index = 0; index < WIDTH; ++index) {
                buffer[pos + index] = static_cast<u8>(ordered >> (8 * (WIDTH - 1 - index)));
            }
            pos += WIDTH;
        }
    public:
        ByteBuffer() = default;

        explicit ByteBuffer(usize capacity):
            buffer(capacity), lim{capacity} {}

        ByteBuffer(usize capacity, u8 value):
            buffer(capacity, value), lim{capacity} {}

        explicit ByteBuffer(const Vector<u8>& data):
            buffer{data}, lim{data.size()} {}

        explicit ByteBuffer(Vector<u8>&& data):
            buffer{Ops::move(data)}, lim{buffer.size()} {}

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

        /**
         * @brief Turns a written buffer into a readable one.
         *
         * Limits reading to what was written and rewinds to the start. Discards
         * any mark, which no longer refers to anything meaningful.
         */
        void flip() noexcept {
            if (pos > lim) {
                pos = lim;
            }
            lim = pos;
            pos = 0;
            marked = nullopt;
        }

        /**
         * @brief Returns to the start of the readable region without changing it.
         *
         * For reading the same bytes again - a parser that consumed a header,
         * found it wanted a different shape, and needs the bytes back. Unlike
         * @ref flip it leaves the limit alone. Discards any mark.
         */
        void rewind() noexcept {
            pos = 0;
            marked = nullopt;
        }

        /**
         * @brief Moves the unread bytes to the front and reopens the rest for writing.
         *
         * The operation a reactor loop turns on: read, @ref flip, parse as far as
         * the data allows, compact, then read more into the room that frees up.
         * Afterwards the position sits just past the bytes that were kept, so the
         * next write appends to them rather than overwriting them, and the limit
         * is the capacity again. Discards any mark.
         */
        void compact() noexcept {
            const usize kept = remaining();
            if (kept > 0 && pos > 0) {
                // Forwards, because the destination is strictly below the source.
                for (usize index = 0; index < kept; ++index) {
                    buffer[index] = buffer[pos + index];
                }
            }
            pos = kept;
            lim = buffer.size();
            marked = nullopt;
        }

        /**
         * @brief Remembers the current position for a later @ref reset.
         */
        void mark() noexcept {
            marked = pos;
        }

        /**
         * @brief Returns to the position @ref mark recorded.
         * @throws IllegalStateException if no mark is set.
         *
         * The bounded form of @ref rewind: a codec that speculatively reads a
         * field and then finds the frame incomplete puts back exactly what it
         * took, not everything since the start.
         */
        THROWS(IllegalStateException)
        void reset() {
            if (!marked) {
                throw IllegalStateException("ByteBuffer has no mark to reset to");
            }
            pos = *marked;
        }

        /**
         * @brief Whether a mark is set.
         */
        [[nodiscard]]
        bool has_mark() const noexcept {
            return marked.has_value();
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return lim == 0;
        }

        [[nodiscard]]
        auto& operator[](this auto&& self, usize index) noexcept {
            return self.buffer[index];
        }

        [[nodiscard]]
        u8* data() noexcept {
            return buffer.data();
        }

        [[nodiscard]]
        const u8* data() const noexcept {
            return buffer.data();
        }

        THROWS(OutOfRangeException)
        void advance(usize count) {
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

        /**
         * @brief Releases the storage.
         *
         * The bytes are freed and the capacity drops to zero,
         * so a write afterwards overflows. @ref compact is what
         * prepares a buffer to be filled again, and @ref resize
         * is what gives a cleared one room.
         */
        void clear() noexcept {
            buffer.clear();
            pos = 0;
            lim = 0;
            marked = nullopt;
        }

        void resize(usize new_size) {
            buffer.resize(new_size);
            lim = new_size;
            if (pos > lim) {
                pos = lim;
            }
            if (marked && *marked > lim) {
                marked = nullopt;
            }
        }

        template <typename It>
        void assign(It first, It last) {
            buffer.assign(first, last);
            pos = 0;
            lim = buffer.size();
            marked = nullopt;
        }

        [[nodiscard]]
        THROWS(OutOfRangeException)
        u8 get() {
            if (pos >= lim) {
                throw OutOfRangeException("ByteBuffer underflow");
            }
            return buffer[pos++];
        }

        THROWS(OutOfRangeException)
        void put(u8 byte) {
            if (pos >= lim) {
                throw OutOfRangeException("ByteBuffer overflow");
            }
            buffer[pos++] = byte;
        }

        /**
         * @brief Reads two bytes as an unsigned 16-bit value.
         * @param order The byte order they are stored in; big-endian by default,
         * which is what every wire format in this library uses.
         * @return The value read.
         * @throws OutOfRangeException if fewer than two bytes remain.
         *
         * The order is named per call rather than held as buffer state.
         * A parser that reads a big-endian header and a little-endian
         * payload would otherwise depend on when order() was last set, and the
         * failure would be a plausible wrong number rather than an error.
         */
        [[nodiscard]]
        THROWS(OutOfRangeException)
        u16 get_u16(Endian order = Endian::BIG) {
            return read_integer<u16>(order);
        }

        /**
         * @brief Reads four bytes as an unsigned 32-bit value.
         * @param order The byte order they are stored in.
         * @return The value read.
         * @throws OutOfRangeException if fewer than four bytes remain.
         */
        [[nodiscard]]
        THROWS(OutOfRangeException)
        u32 get_u32(Endian order = Endian::BIG) {
            return read_integer<u32>(order);
        }

        /**
         * @brief Reads eight bytes as an unsigned 64-bit value.
         * @param order The byte order they are stored in.
         * @return The value read.
         * @throws OutOfRangeException if fewer than eight bytes remain.
         */
        [[nodiscard]]
        THROWS(OutOfRangeException)
        u64 get_u64(Endian order = Endian::BIG) {
            return read_integer<u64>(order);
        }

        /**
         * @brief Writes @p value as two bytes.
         * @param value The value to write.
         * @param order The byte order to write it in.
         * @throws OutOfRangeException if fewer than two bytes remain.
         */
        THROWS(OutOfRangeException)
        void put_u16(u16 value, Endian order = Endian::BIG) {
            write_integer<u16>(value, order);
        }

        /**
         * @brief Writes @p value as four bytes.
         * @param value The value to write.
         * @param order The byte order to write it in.
         * @throws OutOfRangeException if fewer than four bytes remain.
         */
        THROWS(OutOfRangeException)
        void put_u32(u32 value, Endian order = Endian::BIG) {
            write_integer<u32>(value, order);
        }

        /**
         * @brief Writes @p value as eight bytes.
         * @param value The value to write.
         * @param order The byte order to write it in.
         * @throws OutOfRangeException if fewer than eight bytes remain.
         */
        THROWS(OutOfRangeException)
        void put_u64(u64 value, Endian order = Endian::BIG) {
            write_integer<u64>(value, order);
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
}
