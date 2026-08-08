#pragma once

using stdx::fs::Path;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::text::CharTraits;

#ifdef __cpp_lib_generator
using stdx::ranges::Generator;
#endif

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    /**
     * @class Scanner
     * @brief A Java-style text scanner that parses primitive types and strings
     * using delimiters (whitespace by default).
     *
     * The Scanner class can read from an InputStream, a File, a filesystem Path,
     * or a String. It breaks its input into tokens using a set of delimiter
     * characters, then parses those tokens into various types.
     *
     * @note The delimiter is a set of characters rather than a pattern, and
     * input is read a line at a time, so a token never spans a line break.
     * See use_delimiter() for what that means in practice.
     *
     * @note This class is move-only.
     *
     * Example usage:
     * @code
     *     // Read from stdin
     *     Scanner sc(System::in);
     *     while (sc.has_next_int()) {
     *         i32 n = *sc.next_int();
     *     }
     *
     *     // Read from a file, distinguishing why a read stopped
     *     Scanner file_sc("input.txt"_path);
     *     Expected<String, Scanner::Error> line = file_sc.next_line();
     *     if (!line && line.error() == Scanner::Error::IO_ERROR) {
     *         // the source failed, rather than simply running out
     *     }
     *
     *     // Read comma-separated values from a string
     *     Scanner str_sc("hello,world,42"s);
     *     str_sc.use_delimiter(",");
     *     Expected<String, Scanner::Error> word = str_sc.next();
     * @endcode
     */
    class Scanner {
    public:
        /**
         * @enum Error
         * @brief Why a scan did not produce a value.
         */
        enum class Error: u8 {
            END_OF_INPUT, ///< The input holds no further token or line.
            MISMATCH, ///< A token was available but did not parse as the requested type.
            IO_ERROR, ///< The underlying stream failed while being read.
        };
    private:
        UniquePointer<InputStream> _owned_source; ///< Owned stream (when constructed from a Path, File, or String).
        InputStream* _source = nullptr; ///< Non-owning pointer to the input stream being scanned.
        String _delimiter = " \t\n\r\f\v"; ///< Current set of delimiter characters.
        String _buffered_line; ///< Internal line buffer for token parsing.
        usize _line_position = 0; ///< Read position within the line buffer.
        usize _token_end = 0; ///< End of the token last reported by peek_line_token().
        bool _line_consumed = true; ///< Whether the current line buffer has been fully consumed.
        bool _line_terminated = false; ///< Whether the buffered line ended with a line terminator.
        bool _io_failed = false; ///< Whether the source reported a read failure.
        bool _closed = false; ///< Whether close() has been called.

        /**
         * @brief Throws if the scanner has been closed.
         * @throws IllegalStateException if close() has been called.
         */
        THROWS(IllegalStateException)
        void ensure_open() const {
            if (_closed) {
                throw IllegalStateException("Scanner is closed");
            }
        }

        /**
         * @brief The reason a scan found nothing left to read.
         * @return IO_ERROR once the source has reported a read failure, END_OF_INPUT otherwise.
         */
        [[nodiscard]]
        Error exhausted_reason() const noexcept {
            return _io_failed ? Error::IO_ERROR : Error::END_OF_INPUT;
        }

        /**
         * @brief Reads the next line from the source into the line buffer.
         *
         * Recognises "\n", "\r\n" and a lone "\r" as line terminators, and
         * records whether the line ended with one so that has_next_line() can
         * tell a spent final line from one that still has an empty remainder.
         * A read failure on the source is latched, so it can be reported apart
         * from a clean end of input.
         *
         * @note The Unicode separators U+2028, U+2029 and U+0085 are not
         * treated as terminators. Tokens are split on a set of bytes, so
         * decoding line breaks as code points would be inconsistent with how
         * the rest of the scanner reads its input.
         *
         * @return True if a line was read, false if the source is exhausted or failed.
         */
        bool read_line() {
            if (_source == nullptr) {
                return false;
            }
            _buffered_line.clear();
            _line_terminated = false;

            int ch = _source->get();
            if (ch == CharTraits<char>::eof()) {
                _io_failed = _io_failed || _source->bad();
                return false;
            }
            while (ch != CharTraits<char>::eof()) {
                if (ch == '\n') {
                    _line_terminated = true;
                    break;
                }
                if (ch == '\r') {
                    if (_source->peek() == '\n') {
                        _source->get();
                    }
                    _line_terminated = true;
                    break;
                }
                _buffered_line.push_back(static_cast<char>(ch));
                ch = _source->get();
            }
            if (_source->bad()) {
                _io_failed = true;
                return false;
            }
            _line_position = 0;
            _line_consumed = false;
            return true;
        }

        /**
         * @brief Reads the next token from the current line without consuming it.
         * @param token Receives the token when one is available.
         * @return True if a token remains on the current line.
         *
         * Leading delimiter characters are skipped; the token runs to the next
         * delimiter or to the end of the line. The read position is left alone,
         * so a caller that rejects the token leaves the scanner exactly where it
         * found it. The token's end is recorded for consume_token().
         */
        bool peek_line_token(String& token) {
            usize start = _buffered_line.find_first_not_of(_delimiter, _line_position);
            if (start == String::npos) {
                return false;
            }
            usize stop = _buffered_line.find_first_of(_delimiter, start);
            _token_end = (stop == String::npos) ? _buffered_line.size() : stop;
            token = _buffered_line.substr(start, _token_end - start);
            return true;
        }

        /**
         * @brief Advances the current line past the token reported by peek_next().
         */
        void consume_token() noexcept {
            _line_position = _token_end;
        }

        /**
         * @brief Ensures the line buffer is positioned on a line that still holds a token.
         * @return True if a token is available, false if the source is exhausted.
         *
         * Blank and fully consumed lines are skipped, so a run of empty lines
         * does not hide the tokens that follow it.
         */
        bool ensure_line() {
            String token;
            while (true) {
                if (!_line_consumed && peek_line_token(token)) {
                    return true;
                }
                _line_consumed = true;
                if (!read_line()) {
                    return false;
                }
            }
        }

        /**
         * @brief Returns the next token without consuming it, advancing lines as needed.
         * @param token Receives the token when one is available.
         * @return True if a token is available.
         */
        bool peek_next(String& token) {
            return ensure_line() && peek_line_token(token);
        }
    public:
        /**
         * @brief Constructs a Scanner that reads from an InputStream.
         * @param stream The input stream to read from (e.g., System.in / Cin).
         */
        explicit Scanner(InputStream& stream) noexcept:
            _source{&stream} {}

        /**
         * @brief Constructs a Scanner that reads from a filesystem Path.
         * @param path The path to the file to read.
         * @throws IOException if the file cannot be opened.
         */
        THROWS(IOException)
        explicit Scanner(const Path& path) {
            UniquePointer<InputFileStream> stream = Pointers::unique<InputFileStream>(path);
            if (!stream->is_open()) {
                throw IOException(Ops::fmt("Failed to open file: {}", path));
            }
            _owned_source = Ops::move(stream);
            _source = _owned_source.get();
        }

        /**
         * @brief Constructs a Scanner that reads from a File object.
         * The File must have been opened with a path.
         * @param file The File to read from.
         * @throws IOException if the file has no associated path or cannot be opened.
         */
        THROWS(IOException)
        explicit Scanner(const File& file) {
            const Optional<Path>& path = file.path();
            if (!path.has_value()) {
                throw IOException("File has no associated path.");
            }
            UniquePointer<InputFileStream> stream = Pointers::unique<InputFileStream>(*path);
            if (!stream->is_open()) {
                throw IOException(Ops::fmt("Failed to open file: {}", path->string()));
            }
            _owned_source = Ops::move(stream);
            _source = _owned_source.get();
        }

        /**
         * @brief Constructs a Scanner that reads from a String.
         * @param input The string to scan.
         *
         * The string is scanned as a stream, so multi-line input is split into
         * lines exactly as it would be when read from a file.
         */
        explicit Scanner(const String& input):
            _owned_source{Pointers::unique<InputStringStream>(input)} {
            _source = _owned_source.get();
        }

        ~Scanner() = default;
        Scanner(const Scanner&) = DELETE_METHOD("Scanner is not copyable.");
        Scanner& operator=(const Scanner&) = DELETE_METHOD("Scanner is not copyable.");
        Scanner(Scanner&&) = default;
        Scanner& operator=(Scanner&&) = default;

        /**
         * @brief Closes the scanner and releases any underlying resources.
         *
         * Every scanning operation throws IllegalStateException afterwards.
         * Closing an already closed scanner has no effect.
         */
        void close() noexcept {
            _owned_source.reset();
            _source = nullptr;
            _line_consumed = true;
            _closed = true;
        }

        /**
         * @brief Returns the characters that currently separate tokens.
         * @return The current delimiter set.
         */
        [[nodiscard]]
        StringView delimiter() const noexcept {
            return _delimiter;
        }

        /**
         * @brief Sets the characters that separate tokens.
         * @param delimiters The characters that separate tokens.
         * @return This scanner, so calls can be chained.
         *
         * The delimiter is a set of characters rather than a pattern: every
         * character in @p delimiters ends a token, and runs of them between
         * tokens are skipped. The new set applies from the next token scanned,
         * so it can be changed part-way through the input.
         *
         * @note Input is read a line at a time, so a token never spans a line
         * break however the delimiter set is chosen. Scanning "a,b\nc" with a
         * "," delimiter yields "a", "b", then "c" - never "b\nc". Keep "\n" in
         * the set only to describe intent; it is never seen in a line buffer.
         */
        Scanner& use_delimiter(StringView delimiters) {
            _delimiter = delimiters;
            return *this;
        }

        /**
         * @brief Returns true if the scanner has another token in its input.
         * @return True if another token is available.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next() {
            ensure_open();
            return ensure_line();
        }

        /**
         * @brief Returns the next token from the scanner.
         * @return The next token, Error::END_OF_INPUT if none remains, or
         * Error::IO_ERROR if the source failed.
         * @throws IllegalStateException if the scanner is closed.
         */
        THROWS(IllegalStateException)
        Expected<String, Error> next() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            consume_token();
            return token;
        }

        /**
         * @brief Returns true if the scanner has another line in its input.
         * @return True if another line is available.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_line() {
            ensure_open();
            if (!_line_consumed
                && (_line_position < _buffered_line.size() || _line_terminated)) {
                return true;
            }
            return _source != nullptr && _source->peek() != CharTraits<char>::eof();
        }

        /**
         * @brief Advances past the current line and returns the rest of it.
         * @return The rest of the current line, Error::END_OF_INPUT if no line
         * remains, or Error::IO_ERROR if the source failed.
         * @throws IllegalStateException if the scanner is closed.
         */
        THROWS(IllegalStateException)
        Expected<String, Error> next_line() {
            ensure_open();
            if (!_line_consumed) {
                _line_consumed = true;
                if (_line_position < _buffered_line.size()) {
                    return _buffered_line.substr(_line_position);
                }
                if (_line_terminated) {
                    return ""s;
                }
            }
            if (read_line()) {
                _line_consumed = true;
                return _buffered_line;
            }
            return Unexpected(exhausted_reason());
        }

    #ifdef __cpp_lib_generator
        /**
         * @brief Lazily yields each remaining token as a single-pass range.
         * @return A Generator yielding each remaining token in turn.
         * @throws IllegalStateException if the scanner is closed, and IOException
         * if the source fails mid-scan - both on the step that reads from it
         * rather than when the generator is created.
         *
         * The generator form of the has_next()/next() loop: it drives next()
         * to exhaustion, yielding one delimited token at a time. The scanner is
         * consumed as the range is advanced, so traverse it once and do not
         * interleave with manual next()/has_next() calls.
         *
         * @note Synchronous: each step reads from the underlying stream as
         * needed. This is lazy iteration, not asynchronous I/O.
         */
        [[nodiscard]]
        THROWS(IllegalStateException, IOException)
        Generator<String> tokens() {
            while (true) {
                Expected<String, Error> token = next();
                if (token.has_value()) {
                    co_yield *token;
                    continue;
                }
                if (token.error() == Error::IO_ERROR) {
                    throw IOException("Scanner source failed while reading");
                }
                co_return;
            }
        }

        /**
         * @brief Lazily yields each remaining line as a single-pass range.
         * @return A Generator yielding each remaining line in turn.
         * @throws IllegalStateException if the scanner is closed, and IOException
         * if the source fails mid-scan - both on the step that reads from it
         * rather than when the generator is created.
         *
         * The generator form of the has_next_line()/next_line() loop: it drives
         * next_line() to exhaustion, yielding one line at a time without ever
         * buffering the whole input. The scanner is consumed as the range is
         * advanced, so traverse it once and do not interleave with manual
         * next_line()/has_next_line() calls.
         *
         * @note Synchronous: each step reads from the underlying stream as
         * needed. This is lazy iteration, not asynchronous I/O.
         */
        [[nodiscard]]
        THROWS(IllegalStateException, IOException)
        Generator<String> lines() {
            while (true) {
                Expected<String, Error> line = next_line();
                if (line.has_value()) {
                    co_yield *line;
                    continue;
                }
                if (line.error() == Error::IO_ERROR) {
                    throw IOException("Scanner source failed while reading");
                }
                co_return;
            }
        }
    #endif

        /**
         * @brief Returns true if the next token can be parsed as an i32.
         * @return True if the next token is a valid integer.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_int() {
            ensure_open();
            String token;
            return peek_next(token) && Integer::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an i32.
         * @return The parsed value, Error::MISMATCH if the token is not an integer,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<i32, Error> next_int() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<i32> value = Integer::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as an i64.
         * @return True if the next token is a valid long.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_long() {
            ensure_open();
            String token;
            return peek_next(token) && Long::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an i64.
         * @return The parsed value, Error::MISMATCH if the token is not a long,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<i64, Error> next_long() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<i64> value = Long::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as an f32.
         * @return True if the next token is a valid float.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_float() {
            ensure_open();
            String token;
            return peek_next(token) && Float::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an f32.
         * @return The parsed value, Error::MISMATCH if the token is not a float,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<f32, Error> next_float() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<f32> value = Float::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as an f64.
         * @return True if the next token is a valid double.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_double() {
            ensure_open();
            String token;
            return peek_next(token) && Double::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an f64.
         * @return The parsed value, Error::MISMATCH if the token is not a double,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<f64, Error> next_double() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<f64> value = Double::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as a boolean.
         * @return True if the next token is "true", "false", "1", or "0".
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_boolean() {
            ensure_open();
            String token;
            return peek_next(token) && Boolean::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as a boolean.
         * @return The parsed value, Error::MISMATCH if the token is not a boolean,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<bool, Error> next_boolean() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<bool> value = Boolean::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as an i8.
         * @return True if the next token is a valid byte.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_byte() {
            ensure_open();
            String token;
            return peek_next(token) && SignedByte::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an i8.
         * @return The parsed value, Error::MISMATCH if the token is not a byte,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<i8, Error> next_byte() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<i8> value = SignedByte::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }

        /**
         * @brief Returns true if the next token can be parsed as an i16.
         * @return True if the next token is a valid short.
         * @throws IllegalStateException if the scanner is closed.
         */
        [[nodiscard]]
        THROWS(IllegalStateException)
        bool has_next_short() {
            ensure_open();
            String token;
            return peek_next(token) && Short::parse(token).has_value();
        }

        /**
         * @brief Scans the next token as an i16.
         * @return The parsed value, Error::MISMATCH if the token is not a short,
         * Error::END_OF_INPUT if no token remains, or Error::IO_ERROR if the
         * source failed.
         * @throws IllegalStateException if the scanner is closed.
         *
         * The token is consumed only when it parses; on a mismatch the scanner
         * does not advance, so the token stays retrievable via next().
         */
        THROWS(IllegalStateException)
        Expected<i16, Error> next_short() {
            ensure_open();
            String token;
            if (!peek_next(token)) {
                return Unexpected(exhausted_reason());
            }
            Optional<i16> value = Short::parse(token);
            if (!value.has_value()) {
                return Unexpected(Error::MISMATCH);
            }
            consume_token();
            return *value;
        }
    };
}

using stdx::io::Scanner;

namespace stdx::fmt {
    template <>
    struct Formatter<Scanner::Error> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(Scanner::Error err, FormatContext& ctx) const {
            StringView name;
            switch (err) {
                case Scanner::Error::MISMATCH:
                    name = "Mismatch";
                    break;
                case Scanner::Error::END_OF_INPUT:
                    name = "End of input";
                    break;
                case Scanner::Error::IO_ERROR:
                    name = "I/O error";
                    break;
            }
            return format_to(ctx.out(), "{:11}", name);
        }
    };
}

template <>
struct stdx::fmt::formatter<Scanner::Error>: public Formatter<Scanner::Error> {};

