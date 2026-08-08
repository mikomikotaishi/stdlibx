#pragma once

#undef stdin
#undef stdout
#undef stderr

using stdx::fs::Path;
using stdx::io::IOException;
using stdx::mem::UniquePointer;

#ifdef __APPLE__
extern "C" std::FILE* __stdinp;
extern "C" std::FILE* __stdoutp;
extern "C" std::FILE* __stderrp;
#elifndef _WIN32
extern "C" std::FILE* stdin;
extern "C" std::FILE* stdout;
extern "C" std::FILE* stderr;
#endif

namespace stdx::io {
    /**
     * @internal
     * @brief The process's standard input stream, as a FILE*.
     * @return The standard input stream.
     */
    [[nodiscard]]
    inline std::FILE* stdin_stream() noexcept {
        #ifdef __APPLE__
        return ::__stdinp;
        #elifdef _WIN32
        return stdx::os::win32::__acrt_iob_func(0);
        #else
        return ::stdin;
        #endif
    }

    /**
     * @internal
     * @brief The process's standard output stream, as a FILE*.
     * @return The standard output stream.
     */
    [[nodiscard]]
    inline std::FILE* stdout_stream() noexcept {
        #ifdef __APPLE__
        return ::__stdoutp;
        #elifdef _WIN32
        return stdx::os::win32::__acrt_iob_func(1);
        #else
        return ::stdout;
        #endif
    }

    /**
     * @internal
     * @brief The process's standard error stream, as a FILE*.
     * @return The standard error stream.
     */
    [[nodiscard]]
    inline std::FILE* stderr_stream() noexcept {
        #ifdef __APPLE__
        return ::__stderrp;
        #elifdef _WIN32
        return stdx::os::win32::__acrt_iob_func(2);
        #else
        return ::stderr;
        #endif
    }
}

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    
    namespace cstdio {
        // File access
        using std::fopen;
        using std::freopen;
        using std::fclose;
        using std::fflush;
        using std::setbuf;
        using std::setvbuf;

        // Direct I/O
        using std::fread;
        using std::fwrite;

        // Byte/multibyte character unformatted I/O
        using std::fgetc;
        using std::getc;
        using std::fgets;
        using std::fputc;
        using std::putc;
        using std::getchar;
        using std::putchar;
        using std::puts;
        using std::ungetc;

        // Byte/multibyte formatted I/O
        using std::scanf;
        using std::fscanf;
        using std::sscanf;
        using std::vscanf;
        using std::vfscanf;
        using std::vsscanf;
        using std::printf;
        using std::fprintf;
        using std::sprintf;
        using std::snprintf;
        using std::vprintf;
        using std::vfprintf;
        using std::vsprintf;
        using std::vsnprintf;

        // File positioning
        using std::ftell;
        using std::fgetpos;
        using std::fseek;
        using std::fsetpos;
        using std::rewind;

        // Error handling
        using std::clearerr;
        using std::feof;
        using std::ferror;
        using std::perror;

        // Operations
        using std::remove;
        using std::rename;
        using std::tmpfile;
        using std::tmpnam;
    }

    /**
     * @class File
     * @brief A wrapper class for C-style file handling using std::FILE*.
     */
    class [[nodiscard]] File {
    public:
        using Handle = std::FILE;
    private:
        /**
         * @struct FileDeleter
         * @brief Custom deleter for std::FILE* that ensures proper resource management.
         */
        struct FileDeleter {
            void operator()(Handle* file) const noexcept {
                if (file != nullptr && file != stdin() && file != stdout() && file != stderr()) {
                    cstdio::fclose(file);
                }
            }
        };

        UniquePointer<Handle, FileDeleter> handle; ///< Unique pointer managing the file handle with a custom deleter.
        Optional<Path> file_path; ///< Path of the file, if not constructed from FILE*.

        /**
         * @brief Opens @p path in @p mode, wide on Windows and narrow elsewhere.
         * @param path The path to the file to open.
         * @param mode The mode string for opening the file.
         * @return A pointer to the opened file handle, or nullptr if the operation fails.
         *
         * Path::c_str() is const wchar_t* on Windows, where the narrow fopen
         * would also read the name in the ACP rather than as Unicode - so the
         * wide opener is the correct one there, not just the one that compiles.
         * The mode string is ASCII by contract and is widened char-for-char.
         */
        [[nodiscard]]
        static Handle* open_path(const Path& path, StringView mode) noexcept {
            #ifdef _WIN32
            const WideString wide_mode(mode.begin(), mode.end());
            return stdx::os::win32::_wfopen(path.c_str(), wide_mode.c_str());
            #else
            return cstdio::fopen(path.c_str(), mode.data());
            #endif
        }

        /**
         * @brief The freopen counterpart of open_path, with the same encoding rules.
         * @param path The path to the file to reopen.
         * @param mode The mode string for reopening the file.
         * @param stream The existing file stream to reopen.
         * @return A pointer to the reopened file handle, or nullptr if the operation fails.
         */
        [[nodiscard]]
        static Handle* reopen_path(const Path& path, StringView mode, Handle* stream) noexcept {
            #ifdef _WIN32
            const WideString wide_mode(mode.begin(), mode.end());
            return stdx::os::win32::_wfreopen(path.c_str(), wide_mode.c_str(), stream);
            #else
            return cstdio::freopen(path.c_str(), mode.data(), stream);
            #endif
        }
    public:
        /**
         * @brief Constructs a File object from a FILE* handle.
         * The File object takes ownership of the handle and will close it when destroyed.
         * @param file The FILE* handle to wrap.
         */
        explicit File(Handle* file) noexcept:
            handle{file} {}

        /**
         * @brief Constructs a File object by opening a file with the given name and mode.
         * The File object takes ownership of the handle and will close it when destroyed.
         * @param name The name of the file to open.
         * @param mode The mode string for opening the file.
         */
        File(StringView name, StringView mode):
            handle{cstdio::fopen(name.data(), mode.data())}, file_path{Path(name)} {}

        File(const Path& path, StringView mode):
            handle{open_path(path, mode)}, file_path{path} {}

        ~File() = default;
        File(const File&) = DELETE_METHOD("File is not copyable.");
        File& operator=(const File&) = DELETE_METHOD("File is not copyable.");
        File(File&&) = default;
        File& operator=(File&&) = default;

        /**
         * @brief Returns a reference to the standard input File object.
         * @return The standard input File object.
         */
        [[nodiscard]]
        static File& stdin() noexcept {
            static File stdin_file(stdin_stream());
            return stdin_file;
        }

        /**
         * @brief Returns a reference to the standard output File object.
         * @return The standard output File object.
         */
        [[nodiscard]]
        static File& stdout() noexcept {
            static File stdout_file(stdout_stream());
            return stdout_file;
        }

        /**
         * @brief Returns a reference to the standard error File object.
         * @return The standard error File object.
         */
        [[nodiscard]]
        static File& stderr() noexcept {
            static File stderr_file(stderr_stream());
            return stderr_file;
        }

        /**
         * @brief Returns the underlying FILE* handle.
         * @return The underlying FILE* handle.
         */
        operator Handle*() const noexcept {
            return handle.get();
        }

        /**
         * @brief Returns the underlying FILE* handle.
         * @return The underlying FILE* handle.
         */
        [[nodiscard]]
        Handle* get() const noexcept {
            return handle.get();
        }

        /**
         * @brief Checks if the File object has an associated file handle.
         * @return True if the File object has a valid file handle, false otherwise.
         */
        explicit operator bool() const noexcept {
            return handle != nullptr;
        }

        /**
         * @brief Closes the file handle associated with this File object.
         */
        void close() noexcept {
            handle.reset();
        }

        /**
         * @brief Releases ownership of the file handle and returns it.
         * After this call, the File object will no longer manage the file handle and will not close it when destroyed.
         * @return The released file handle.
         */
        [[nodiscard]]
        Handle* release() noexcept {
            return handle.release();
        }

        /**
         * @brief Checks if the File object has an open file handle.
         * @return True if the File object has an open file handle, false otherwise.
         */
        [[nodiscard]]
        bool is_open() const noexcept {
            return handle != nullptr;
        }

        /**
         * @brief Returns the path of the file, if available.
         * @return Optional containing the path of the file, or empty if not available.
         */
        [[nodiscard]]
        const Optional<Path>& path() const noexcept {
            return file_path;
        }

        /**
         * @brief Returns the size of the file in bytes, if available.
         * @return Optional containing the size of the file in bytes, or empty if not available.
         */
        [[nodiscard]]
        Optional<u64> size() const noexcept {
            if (!handle) {
                return nullopt;
            }
            i64 current_pos = cstdio::ftell(handle.get());
            if (current_pos < 0) {
                return nullopt;
            }
            if (cstdio::fseek(handle.get(), 0, SEEK_END) != 0) {
                return nullopt;
            }
            i64 end_pos = cstdio::ftell(handle.get());
            if (end_pos < 0) {
                return nullopt;
            }
            if (cstdio::fseek(handle.get(), current_pos, SEEK_SET) != 0) {
                return nullopt;
            }
            return static_cast<u64>(end_pos);
        }

        /**
         * @brief Returns the current position of the file pointer in bytes, if available.
         * @return Optional containing the current position of the file pointer in bytes, or empty if not available.
         */
        [[nodiscard]]
        Optional<u64> position() const noexcept {
            if (!handle) {
                return nullopt;
            }
            i64 pos = cstdio::ftell(handle.get());
            if (pos < 0) {
                return nullopt;
            }
            return static_cast<u64>(pos);
        }

        /**
         * @brief Seeks to a specific position in the file.
         * @param offset The offset in bytes to seek to.
         * @param whence The reference point for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
         * @throws IOException if the seek operation fails.
         */
        THROWS(IOException)
        void seek(i64 offset, i32 whence = SEEK_SET) {
            if (!handle || cstdio::fseek(handle.get(), offset, whence) != 0) {
                throw IOException("Failed to seek in file.");
            }
        }

        /**
         * @brief Seeks to a specific position in the file.
         * @param offset The offset in bytes to seek to.
         * @param whence The reference point for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
         * @return True if the seek operation was successful, false otherwise.
         */
        bool try_seek(i64 offset, i32 whence = SEEK_SET) noexcept {
            return handle && cstdio::fseek(handle.get(), offset, whence) == 0;
        }

        /**
         * @brief Rewinds the file pointer to the beginning of the file.
         */
        void rewind() noexcept {
            if (handle) {
                cstdio::rewind(handle.get());
            }
        }

        /**
         * @brief Reopens the file with a new name and mode.
         * @param mode The new mode to open the file with.
         * @throws IOException if the file could not be reopened.
         *
         * If the File object was constructed from a FILE* handle, this function will attempt to reopen the same file with the new mode.
         */
        THROWS(IOException)
        void reopen(StringView mode) {
            if (!file_path.has_value() || !handle) {
                throw IOException("File is not open or has no path.");
            }
            Handle* new_handle = reopen_path(*file_path, mode, handle.get());
            if (new_handle == nullptr) {
                handle.release();
                handle.reset(new_handle);
                throw IOException("Failed to reopen file.");
            }
        }

        /**
         * @brief Reopens the file with a new name and mode.
         * @param mode The new mode to open the file with.
         * @return True if the file was successfully reopened, false otherwise.
         *
         * If the File object was constructed from a FILE* handle, this function will attempt to reopen the same file with the new mode.
         */
        bool try_reopen(StringView mode) noexcept {
            if (!file_path.has_value() || !handle) {
                return false;
            }
            Handle* new_handle = reopen_path(*file_path, mode, handle.get());
            if (new_handle == nullptr) {
                handle.release();
                handle.reset(new_handle);
                return true;
            }
            return false;
        }

        /**
         * @brief Flushes the file buffer, ensuring that all buffered output is written to the file.
         * @throws IOException if the flush operation fails.
         */
        THROWS(IOException)
        void flush() {
            if (!handle || cstdio::fflush(handle.get()) != 0) {
                throw IOException("Failed to flush file.");
            }
        }

        /**
         * @brief Flushes the file buffer, ensuring that all buffered output is written to the file.
         * @return True if the flush operation was successful, false otherwise.
         */
        bool try_flush() noexcept {
            return handle && cstdio::fflush(handle.get()) == 0;
        }

        /**
         * @brief Checks if the file has an error indicator set.
         * @return True if the file has an error indicator set, false otherwise.
         */
        [[nodiscard]]
        bool has_error() const noexcept {
            return handle && cstdio::ferror(handle.get()) != 0;
        }
        
        /**
         * @brief Checks if the end-of-file indicator is set for the file.
         * @return True if the end-of-file indicator is set, false otherwise.
         */
        [[nodiscard]]
        bool eof() const noexcept {
            return handle && cstdio::feof(handle.get()) != 0;
        }

        /**
         * @brief Clears the error and end-of-file indicators for the file.
         */
        void clear_error() noexcept {
            if (handle) {
                cstdio::clearerr(handle.get());
            }
        }

        /**
         * @brief Checks if the file exists and has a valid path.
         * @return True if the file exists and has a valid path, false otherwise. 
         */
        [[nodiscard]]
        bool exists() const noexcept {
            return file_path.has_value() && stdx::fs::exists(*file_path);
        }

        /**
         * @brief Returns the size of the file in bytes.
         * @return The size of the file in bytes.
         * @throws IOException if the file does not exist or has no path.
         */
        [[nodiscard]]
        THROWS(IOException)
        uintmax file_size() const {
            if (!exists()) {
                throw IOException("File has no path.");
            }
            return stdx::fs::file_size(*file_path);
        }

        /**
         * @brief Returns the size of the file in bytes, or an empty Optional if the file does not exist or has no path.
         * @return Optional containing the size of the file in bytes, or empty if the file does not exist or has no path.
         */
        [[nodiscard]]
        Optional<uintmax> try_file_size() const noexcept {
            if (!exists()) {
                return nullopt;
            }
            return stdx::fs::file_size(*file_path);
        }

        /**
         * @brief Creates a new file with the specified path and mode.
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created File object.
         * @throws IOException if the file could not be created.
         */
        [[nodiscard]]
        THROWS(IOException)
        static File open(const Path& path, StringView mode) {
            File file(path, mode);
            if (!file) {
                throw IOException(Ops::fmt("Failed to open file: {}", path));
            }
            return file;
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode.
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created File object, or empty if the file could not be created.
         */
        [[nodiscard]]
        static Optional<File> try_open(const Path& path, StringView mode) noexcept {
            File file(path, mode);
            if (!file) {
                return nullopt;
            }
            return file;
        }

        /**
         * @brief Creates a new file with the specified path and mode, but only if it does not already exist.
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created File object.
         * @throws IOException if the file already exists or could not be created.
         */
        [[nodiscard]]
        THROWS(IOException)
        static File create(const Path& path) {
            if (stdx::fs::exists(path)) {
                throw IOException(Ops::fmt("File already exists: {}", path));
            }
            return open(path, "w");
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode, but only if it does not already exist.
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created File object, or empty if the file already exists or could not be created.
         */
        [[nodiscard]]
        static Optional<File> try_create(const Path& path) noexcept {
            if (stdx::fs::exists(path)) {
                return nullopt;
            }
            return try_open(path, "w");
        }

        /**
         * @brief Creates a new file with the specified path and mode, or opens it if it already exists.
         * @param path The path of the file to create or open.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created or opened File object.
         * @throws IOException if the file could not be created or opened.
         */
        [[nodiscard]]
        THROWS(IOException)
        static File append(const Path& path) {
            return open(path, "a");
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode, or opens it if it already exists.
         * @param path The path of the file to create or open.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created or opened File object, or empty if the file could not be created or opened.
         */
        [[nodiscard]]
        static Optional<File> try_append(const Path& path) noexcept {
            return try_open(path, "a");
        }
    };
}


#ifdef __cpp_lib_reflection
using stdx::io::File;

namespace stdx::fmt {
    template <>
    struct Formatter<File> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const File& f, FormatContext& ctx) const {
            auto out = ctx.out();
            if (f.path()) {
                out = format_to(out, "{}", *f.path());
            } else {
                out = format_to(out, "File({})", static_cast<const void*>(f.get()));
            }
            return out;
        }
    };
}

template <>
struct stdx::fmt::formatter<File>: public Formatter<File> {};
#endif

