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
     *        using delimiters (whitespace by default).
     *
     * The Scanner class can read from an InputStream, a File, a filesystem Path,
     * or a String. It breaks its input into tokens using a delimiter pattern,
     * then parses those tokens into various types.
     *
     * @note This class is move-only.
     *
     * Example usage:
     * @code
     *     // Read from stdin
     *     Scanner sc(System::in);
     *     while (sc.has_next_int()) {
     *         i32 n = sc.next_int();
     *     }
     *
     *     // Read from a file
     *     Scanner file_sc("input.txt"_path);
     *     String line = file_sc.next_line();
     *
     *     // Read from a string
     *     Scanner str_sc("hello world 42"s);
     *     String word = str_sc.next();
     * @endcode
     */
    class Scanner {
    private:
        UniquePointer<InputFileStream> _owned_file_stream; ///< Owned file stream (when constructed from Path/File).
        InputStream* _source = nullptr; ///< Non-owning pointer to the input stream being scanned.
        String _delimiter = " \t\n\r\f\v"; ///< Current set of delimiter characters.
        String _buffered_line; ///< Internal line buffer for token parsing.
        InputStringStream _line_stream; ///< Stream over the current buffered line.
        bool _line_consumed = true; ///< Whether the current line buffer has been fully consumed.

        /**
         * @brief Ensures the line buffer has content to parse.
         * @return True if there is content available, false if the source is exhausted.
         */
        bool ensure_line() {
            if (!_line_consumed) {
                if (_line_stream.peek() != CharTraits<char>::eof()) {
                    InputStringStream test(_line_stream.str().substr(
                        static_cast<usize>(_line_stream.tellg())
                    ));
                    String token;
                    if (test >> token) {
                        return true;
                    }
                }
                _line_consumed = true;
            }
            while (_source && getline(*_source, _buffered_line)) {
                _line_stream = InputStringStream(_buffered_line);
                _line_consumed = false;
                return true;
            }
            return false;
        }
    public:
        /**
         * @brief Constructs a Scanner that reads from an InputStream.
         *
         * @param stream The input stream to read from (e.g., System.in / Cin).
         */
        explicit Scanner(InputStream& stream) noexcept:
            _source{&stream} {}

        /**
         * @brief Constructs a Scanner that reads from a filesystem Path.
         *
         * @param path The path to the file to read.
         * @throws IOException if the file cannot be opened.
         */
        THROWS(IOException)
        explicit Scanner(const Path& path):
            _owned_file_stream{Pointers::unique<InputFileStream>(path)} {
            if (!_owned_file_stream->is_open()) {
                throw IOException(Ops::fmt("Failed to open file: {}", path));
            }
            _source = _owned_file_stream.get();
        }

        /**
         * @brief Constructs a Scanner that reads from a File object.
         * The File must have been opened with a path.
         *
         * @param file The File to read from.
         * @throws IOException if the file has no associated path or cannot be opened.
         */
        THROWS(IOException)
        explicit Scanner(const File& file) {
            const Optional<Path>& path = file.path();
            if (!path.has_value()) {
                throw IOException("File has no associated path.");
            }
            _owned_file_stream = Pointers::unique<InputFileStream>(*path);
            if (!_owned_file_stream->is_open()) {
                throw IOException(Ops::fmt("Failed to open file: {}", path->string()));
            }
            _source = _owned_file_stream.get();
        }

        /**
         * @brief Constructs a Scanner that reads from a String.
         *
         * @param input The string to scan.
         */
        explicit Scanner(const String& input):
            _owned_file_stream{nullptr},
            _source{nullptr} {
            _buffered_line = input;
            _line_stream = InputStringStream(_buffered_line);
            _line_consumed = false;
        }

        ~Scanner() = default;
        Scanner(const Scanner&) = delete("Scanner is not copyable.");
        Scanner& operator=(const Scanner&) = delete("Scanner is not copyable.");
        Scanner(Scanner&&) = default;
        Scanner& operator=(Scanner&&) = default;

        /**
         * @brief Closes the scanner and releases any underlying resources.
         */
        void close() noexcept {
            _owned_file_stream.reset();
            _source = nullptr;
            _line_consumed = true;
        }

        /**
         * @brief Returns true if the scanner has another token in its input.
         *
         * @return True if another token is available.
         */
        [[nodiscard]]
        bool has_next() {
            return ensure_line();
        }

        /**
         * @brief Returns the next token from the scanner.
         *
         * @return The next token as an Optional<String>, or empty if no more tokens are available.
         */
        Optional<String> next() {
            if (!ensure_line()) {
                return nullopt;
            }
            String token;
            if (_line_stream >> token) {
                return token;
            }
            _line_consumed = true;
            if (!ensure_line()) {
                return nullopt;
            }
            if (_line_stream >> token) {
                return token;
            }
            return nullopt;
        }

        /**
         * @brief Returns true if the scanner has another line in its input.
         *
         * @return True if another line is available.
         */
        [[nodiscard]]
        bool has_next_line() {
            if (!_line_consumed) {
                return true;
            }
            return _source && _source->peek() != CharTraits<char>::eof();
        }

        /**
         * @brief Advances past the current line and returns the rest of it.
         *
         * @return The rest of the current line as an Optional<String>, or empty if no more lines are available.
         */
        Optional<String> next_line() {
            if (!_line_consumed) {
                _line_consumed = true;
                StreamPosition pos = _line_stream.tellg();
                if (pos >= 0 && static_cast<usize>(pos) < _buffered_line.size()) {
                    return _buffered_line.substr(static_cast<usize>(pos));
                }
                return ""s;
            }
            if (_source && getline(*_source, _buffered_line)) {
                _line_consumed = true;
                return _buffered_line;
            }
            return nullopt;
        }

    #ifdef __cpp_lib_generator
        /**
         * @brief Lazily yields each remaining token as a single-pass range.
         *
         * The generator form of the has_next()/next() loop: it drives next()
         * to exhaustion, yielding one delimited token at a time. The scanner is
         * consumed as the range is advanced, so traverse it once and do not
         * interleave with manual next()/has_next() calls.
         *
         * @return A Generator yielding each remaining token in turn.
         *
         * @note Synchronous: each step reads from the underlying stream as
         *       needed. This is lazy iteration, not asynchronous I/O.
         */
        [[nodiscard]]
        Generator<String> tokens() {
            while (Optional<String> token = next()) {
                co_yield *token;
            }
        }

        /**
         * @brief Lazily yields each remaining line as a single-pass range.
         *
         * The generator form of the has_next_line()/next_line() loop: it drives
         * next_line() to exhaustion, yielding one line at a time without ever
         * buffering the whole input. The scanner is consumed as the range is
         * advanced, so traverse it once and do not interleave with manual
         * next_line()/has_next_line() calls.
         *
         * @return A Generator yielding each remaining line in turn.
         *
         * @note Synchronous: each step reads from the underlying stream as
         *       needed. This is lazy iteration, not asynchronous I/O.
         */
        [[nodiscard]]
        Generator<String> lines() {
            while (Optional<String> line = next_line()) {
                co_yield *line;
            }
        }
    #endif

        /**
         * @brief Returns true if the next token can be parsed as an i32.
         *
         * @return True if the next token is a valid integer.
         */
        [[nodiscard]]
        bool has_next_int() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Integer::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an i32.
         *
         * @return The next token parsed as an Optional<i32>, or empty if no more tokens
         *         are available or the token cannot be parsed as an integer.
         */
        Optional<i32> next_int() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Integer::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as an i64.
         *
         * @return True if the next token is a valid long.
         */
        [[nodiscard]]
        bool has_next_long() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Long::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an i64.
         *
         * @return The next token parsed as an Optional<i64>, or empty if no more tokens
         *         are available or the token cannot be parsed as a long.
         */
        Optional<i64> next_long() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Long::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as an f32.
         *
         * @return True if the next token is a valid float.
         */
        [[nodiscard]]
        bool has_next_float() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Float::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an f32.
         *
         * @return The next token parsed as an Optional<f32>, or empty if no more tokens
         *         are available or the token cannot be parsed as a float.
         */
        Optional<f32> next_float() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Float::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as an f64.
         *
         * @return True if the next token is a valid double.
         */
        [[nodiscard]]
        bool has_next_double() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Double::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an f64.
         *
         * @return The next token parsed as an Optional<f64>, or empty if no more tokens
         *         are available or the token cannot be parsed as a double.
         */
        Optional<f64> next_double() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Double::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as a boolean.
         *
         * @return True if the next token is "true", "false", "1", or "0".
         */
        [[nodiscard]]
        bool has_next_boolean() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Boolean::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as a boolean.
         *
         * @return The next token parsed as an Optional<bool>, or empty if no more tokens
         *         are available or the token cannot be parsed as a boolean.
         */
        Optional<bool> next_boolean() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Boolean::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as an i8.
         *
         * @return True if the next token is a valid byte.
         */
        [[nodiscard]]
        bool has_next_byte() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && SignedByte::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an i8.
         *
         * @return The next token parsed as an Optional<i8>, or empty if no more tokens
         *         are available or the token cannot be parsed as a byte.
         */
        Optional<i8> next_byte()  {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return SignedByte::parse(*token);
        }

        /**
         * @brief Returns true if the next token can be parsed as an i16.
         *
         * @return True if the next token is a valid short.
         */
        [[nodiscard]]
        bool has_next_short() {
            if (!ensure_line()) {
                return false;
            }
            StreamPosition pos = _line_stream.tellg();
            String token;
            bool result = (_line_stream >> token) && Short::parse(token).has_value();
            _line_stream.seekg(pos);
            _line_stream.clear();
            return result;
        }

        /**
         * @brief Scans the next token as an i16.
         *
         * @return The next token parsed as an Optional<i16>, or empty if no more tokens
         *         are available or the token cannot be parsed as a short.
         */
        Optional<i16> next_short() {
            Optional<String> token = next();
            if (!token.has_value()) {
                return nullopt;
            }
            return Short::parse(*token);
        }
    };
}
