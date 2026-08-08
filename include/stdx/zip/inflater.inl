#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @class Inflater
     * @brief Decompresses DEFLATE-compressed data.
     *
     * Call {@code set_input()} then {@code inflate()} in a loop until
     * {@code finished()} is {@code true} or {@code needs_input()} is {@code true}.
     *
     * @code{.cpp}
     * Inflater inf;
     * inf.set_input(compressed_bytes);
     * ByteBuffer plain = inf.inflate_all();
     * @endcode
     */
    class Inflater {
    public:
        static inline constexpr usize CHUNK_SIZE = 16 * 1024; ///< Internal output chunk size.
    private:
        z_stream _stream = { .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
        ByteBuffer _input; ///< Buffered input set by set_input().
        usize _input_offset; ///< Bytes of input already consumed.
        bool _opened; ///< Whether the zlib stream is live.
        bool _finished_stream; ///< Whether Z_STREAM_END has been seen.

        THROWS(ZipException)
        void init(i32 window_bits) {
            const i32 rc = inflateInit2(&_stream, window_bits);
            if (rc != Z_OK) {
                throw ZipException(build_error_message("Failed to initialize inflater", _stream, rc));
            }
            _opened = true;
        }
    public:
        /**
         * @brief Creates a new Inflater for decompressing a DEFLATE stream.
         * @param nowrap If {@code true}, expect raw deflate data (no zlib header/trailer).
         * @throws ZipException if zlib initialization fails.
         */
        THROWS(ZipException)
        explicit Inflater(bool nowrap = false):
            _opened{false}, _finished_stream{false}, _input{}, _input_offset{0} {
            init(nowrap ? -MAX_WBITS : MAX_WBITS);
        }

        /**
         * @brief Creates a new Inflater for decompressing a DEFLATE stream.
         * @param format Which framing to expect around the deflate stream.
         * @throws ZipException if zlib initialization fails.
         */
        THROWS(ZipException)
        explicit Inflater(WrapperFormat format):
            _opened{false}, _finished_stream{false}, _input{}, _input_offset{0} {
            i32 window_bits = MAX_WBITS;
            switch (format) {
                case WrapperFormat::ZLIB:
                    window_bits = MAX_WBITS;
                    break;
                case WrapperFormat::RAW:
                    window_bits = -MAX_WBITS;
                    break;
                case WrapperFormat::GZIP:
                    window_bits = 16 + MAX_WBITS;
                    break;
            }
            init(window_bits);
        }

        Inflater(const Inflater&) = DELETE_METHOD("Inflater is not copyable.");
        Inflater& operator=(const Inflater&) = DELETE_METHOD("Inflater is not copyable.");

        ~Inflater() {
            if (_opened) {
                ::inflateEnd(&_stream);
            }
        }

        /**
         * @brief Releases the native zlib stream eagerly.
         *
         * After calling {@code end()}, no other methods may be used.
         * The destructor calls this automatically if not already called.
         *
         * @throws ZipException if the stream was already ended.
         */
        THROWS(ZipException)
        void end() {
            if (!_opened) {
                throw ZipException("Inflater already ended");
            }
            ::inflateEnd(&_stream);
            _opened = false;
        }

        /**
         * @brief Sets the input data to be decompressed on the next {@code inflate()} call.
         * @param data The compressed bytes.
         */
        void set_input(Span<const u8> data) {
            _input.assign(data.begin(), data.end());
            _input_offset = 0;
        }

        /**
         * @brief Returns {@code true} if all current input has been consumed.
         * @return {@code true} when more input is needed before the next {@code inflate()}.
         */
        [[nodiscard]]
        bool needs_input() const noexcept {
            return _input_offset >= _input.size();
        }

        /**
         * @brief Returns {@code true} if the end of the compressed stream has been reached.
         * @return {@code true} once Z_STREAM_END is received.
         */
        [[nodiscard]]
        bool finished() const noexcept {
            return _finished_stream;
        }

        /**
         * @brief Resets the decompressor so it can be reused for a new stream.
         * @throws ZipException if the underlying reset fails.
         */
        THROWS(ZipException)
        void reset() {
            const i32 rc = ::inflateReset(&_stream);
            if (rc != Z_OK) {
                throw ZipException(build_error_message("Failed to reset inflater", _stream, rc));
            }
            _finished_stream = false;
            _input.clear();
            _input_offset = 0;
        }

        /**
         * @brief Decompresses pending input and returns the produced output.
         *
         * Returns an empty buffer when no output was produced (e.g. more input is needed).
         *
         * @return The decompressed bytes produced in this call.
         * @throws DataFormatException if the compressed data is corrupt.
         * @throws ZipException if the decompressor is not open or zlib reports a stream error.
         */
        [[nodiscard]]
        THROWS(DataFormatException, ZipException)
        ByteBuffer inflate() {
            if (!_opened) {
                throw ZipException("Inflater is not initialized");
            }

            if (_finished_stream) {
                return {};
            }

            Array<u8, CHUNK_SIZE> out_chunk = {};
            ByteBuffer out;

            const bool have_input = _input_offset < _input.size();
            const u32 initial_avail = have_input
                ? static_cast<u32>(_input.size() - _input_offset)
                : 0;

            _stream.next_in = have_input
                ? reinterpret_cast<u8*>(_input.data() + _input_offset)
                : nullptr;
            _stream.avail_in = initial_avail;

            i32 rc = Z_OK;
            do {
                _stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
                _stream.avail_out = static_cast<u32>(out_chunk.size());

                rc = ::inflate(&_stream, Z_NO_FLUSH);
                if (rc == Z_NEED_DICT || rc == Z_DATA_ERROR || rc == Z_MEM_ERROR) {
                    throw DataFormatException(build_error_message("Inflate failed", _stream, rc));
                }
                if (rc == Z_STREAM_ERROR) {
                    throw ZipException(build_error_message("Inflate stream error", _stream, rc));
                }

                const usize produced = out_chunk.size() - _stream.avail_out;
                if (produced > 0) {
                    const usize out_old_size = out.size();
                    out.resize(out_old_size + produced);
                    std::copy(
                        out_chunk.begin(),
                        out_chunk.begin() + static_cast<ptrdiff>(produced),
                        out.data() + static_cast<ptrdiff>(out_old_size)
                    );
                }

                if (rc == Z_STREAM_END) {
                    _finished_stream = true;
                    break;
                }

                if (rc == Z_BUF_ERROR && _stream.avail_in == 0) {
                    break;
                }
            } while (_stream.avail_out == 0 || _stream.avail_in > 0);

            if (have_input) {
                const u32 consumed = initial_avail - _stream.avail_in;
                _input_offset += consumed;
                if (_input_offset >= _input.size()) {
                    _input.clear();
                    _input_offset = 0;
                }
            }

            return out;
        }

        /**
         * @brief Decompresses all remaining input and returns it as a single buffer.
         *
         * Calls {@code inflate()} repeatedly until {@code finished()} or
         * {@code needs_input()} is {@code true}.
         *
         * @return The complete decompressed output.
         * @throws DataFormatException if the compressed data is corrupt.
         * @throws ZipException if a stream error occurs.
         */
        [[nodiscard]]
        THROWS(DataFormatException, ZipException)
        ByteBuffer inflate_all() {
            ByteBuffer out;
            while (true) {
                ByteBuffer chunk = inflate();
                if (!chunk.empty()) {
                    const usize out_old_size = out.size();
                    out.resize(out_old_size + chunk.size());
                    stdx::ranges::copy(chunk, out.data() + static_cast<ptrdiff>(out_old_size));
                }
                if (_finished_stream || (needs_input() && chunk.empty())) {
                    break;
                }
            }
            return out;
        }

        /**
         * @brief Returns the total number of compressed bytes consumed from the stream.
         * @return Total input bytes.
         */
        [[nodiscard]]
        u64 total_in() const noexcept {
            return _stream.total_in;
        }

        /**
         * @brief Returns the total number of decompressed bytes produced by the stream.
         * @return Total output bytes.
         */
        [[nodiscard]]
        u64 total_out() const noexcept {
            return _stream.total_out;
        }
    };
}
#endif
