#pragma once

#undef stdin
#undef stdout
#undef stderr

using stdx::fs::Path;
using stdx::io::IOException;
using stdx::mem::UniquePointer;

using FILE = std::FILE;

extern "C" FILE* stdin;
extern "C" FILE* stdout;
extern "C" FILE* stderr;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    
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

    /**
     * @class File
     * @brief A wrapper class for C-style file handling using std::FILE*.
     */
    class [[nodiscard]] File {
    public:
        using Handle = std::FILE;

        /**
         * @struct FileDeleter
         * @brief Custom deleter for std::FILE* that ensures proper resource management.
         */
        struct FileDeleter {
            void operator()(Handle* file) const noexcept {
                if (file && file != stdin() && file != stdout() && file != stderr()) {
                    fclose(file);
                }
            }
        };
    private:
        UniquePointer<Handle, FileDeleter> handle; ///< Unique pointer managing the file handle with a custom deleter.
        Optional<Path> file_path; ///< Path of the file, if not constructed from FILE*.
    public:
        /**
         * @brief Constructs a File object from a FILE* handle.
         * The File object takes ownership of the handle and will close it when destroyed.
         * 
         * @param file 
         */
        explicit File(Handle* file) noexcept:
            handle{file} {}

        /**
         * @brief Constructs a File object by opening a file with the given name and mode.
         * The File object takes ownership of the handle and will close it when destroyed.
         * 
         * @param name 
         * @param mode 
         */
        File(StringView name, StringView mode):
            handle{fopen(name.data(), mode.data())}, file_path{Path(name)} {}

        File(const Path& path, StringView mode):
            handle{fopen(path.c_str(), mode.data())}, file_path{path} {}

        ~File() = default;
        File(const File&) = delete;
        File& operator=(const File&) = delete;
        File(File&&) = default;
        File& operator=(File&&) = default;

        /**
         * @brief Returns a reference to the standard input File object.
         * 
         * @return The standard input File object.
         */
        [[nodiscard]]
        static File& stdin() noexcept {
            static File stdin_file(::stdin);
            return stdin_file;
        }

        /**
         * @brief Returns a reference to the standard output File object.
         * 
         * @return The standard output File object.
         */
        [[nodiscard]]
        static File& stdout() noexcept {
            static File stdout_file(::stdout);
            return stdout_file;
        }

        /**
         * @brief Returns a reference to the standard error File object.
         * 
         * @return The standard error File object.
         */
        [[nodiscard]]
        static File& stderr() noexcept {
            static File stderr_file(::stderr);
            return stderr_file;
        }

        /**
         * @brief Returns the underlying FILE* handle.
         * 
         * @return Handle* 
         */
        operator Handle*() const noexcept {
            return handle.get();
        }

        /**
         * @brief Returns the underlying FILE* handle.
         * 
         * @return Handle* 
         */
        [[nodiscard]]
        Handle* get() const noexcept {
            return handle.get();
        }

        /**
         * @brief Checks if the File object has an associated file handle.
         * 
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
         * 
         * @return The released file handle.
         */
        [[nodiscard]]
        Handle* release() noexcept {
            return handle.release();
        }

        /**
         * @brief Checks if the File object has an open file handle.
         * 
         * @return True if the File object has an open file handle, false otherwise.
         */
        [[nodiscard]]
        bool is_open() const noexcept {
            return handle != nullptr;
        }

        /**
         * @brief Returns the path of the file, if available.
         * 
         * @return Optional containing the path of the file, or empty if not available.
         */
        [[nodiscard]]
        const Optional<Path>& path() const noexcept {
            return file_path;
        }

        /**
         * @brief Returns the size of the file in bytes, if available.
         * 
         * @return Optional containing the size of the file in bytes, or empty if not available.
         */
        [[nodiscard]]
        Optional<u64> size() const noexcept {
            if (!handle) {
                return {};
            }
            i64 current_pos = stdx::io::ftell(handle.get());
            if (current_pos < 0) {
                return {};
            }
            if (stdx::io::fseek(handle.get(), 0, SEEK_END) != 0) {
                return {};
            }
            i64 end_pos = stdx::io::ftell(handle.get());
            if (end_pos < 0) {
                return {};
            }
            if (stdx::io::fseek(handle.get(), current_pos, SEEK_SET) != 0) {
                return {};
            }
            return static_cast<u64>(end_pos);
        }

        /**
         * @brief Returns the current position of the file pointer in bytes, if available.
         * 
         * @return Optional containing the current position of the file pointer in bytes, or empty if not available.
         */
        [[nodiscard]]
        Optional<u64> position() const noexcept {
            if (!handle) {
                return {};
            }
            i64 pos = stdx::io::ftell(handle.get());
            if (pos < 0) {
                return {};
            }
            return static_cast<u64>(pos);
        }

        /**
         * @brief Seeks to a specific position in the file.
         * 
         * @param offset The offset in bytes to seek to.
         * @param whence The reference point for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
         * @throws IOException if the seek operation fails.
         */
        void seek(i64 offset, i32 whence = SEEK_SET) throws (IOException) {
            if (!handle || stdx::io::fseek(handle.get(), offset, whence) != 0) {
                throw IOException("Failed to seek in file.");
            }
        }

        /**
         * @brief Seeks to a specific position in the file.
         * 
         * @param offset The offset in bytes to seek to.
         * @param whence The reference point for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
         * @return True if the seek operation was successful, false otherwise.
         */
        bool try_seek(i64 offset, i32 whence = SEEK_SET) noexcept {
            return handle && stdx::io::fseek(handle.get(), offset, whence) == 0;
        }

        /**
         * @brief Rewinds the file pointer to the beginning of the file.
         */
        void rewind() noexcept {
            if (handle) {
                stdx::io::rewind(handle.get());
            }
        }

        /**
         * @brief Reopens the file with a new name and mode.
         * If the File object was constructed from a FILE* handle, this function will attempt to reopen the same file with the new mode.
         * 
         * @param mode The new mode to open the file with.
         * @throws IOException if the file could not be reopened.
         */
        void reopen(StringView mode) throws (IOException) {
            if (!file_path || !handle) {
                throw IOException("File is not open or has no path.");
            }
            Handle* new_handle = stdx::io::freopen(file_path->c_str(), mode.data(), handle.get());
            if (!new_handle) {
                handle.release();
                handle.reset(new_handle);
                throw IOException("Failed to reopen file.");
            }
        }

        /**
         * @brief Reopens the file with a new name and mode.
         * If the File object was constructed from a FILE* handle, this function will attempt to reopen the same file with the new mode.
         * 
         * @param mode The new mode to open the file with.
         * @return True if the file was successfully reopened, false otherwise.
         */
        bool try_reopen(StringView mode) noexcept {
            if (!file_path || !handle) {
                return false;
            }
            Handle* new_handle = stdx::io::freopen(file_path->c_str(), mode.data(), handle.get());
            if (!new_handle) {
                handle.release();
                handle.reset(new_handle);
                return true;
            }
            return false;
        }

        /**
         * @brief Flushes the file buffer, ensuring that all buffered output is written to the file.
         * 
         * @throws IOException if the flush operation fails.
         */
        void flush() throws (IOException) {
            if (!handle || stdx::io::fflush(handle.get()) != 0) {
                throw IOException("Failed to flush file.");
            }
        }

        /**
         * @brief Flushes the file buffer, ensuring that all buffered output is written to the file.
         * 
         * @return True if the flush operation was successful, false otherwise.
         */
        bool try_flush() noexcept {
            return handle && stdx::io::fflush(handle.get()) == 0;
        }

        /**
         * @brief Checks if the file has an error indicator set.
         * 
         * @return True if the file has an error indicator set, false otherwise.
         */
        [[nodiscard]]
        bool has_error() const noexcept {
            return handle && stdx::io::ferror(handle.get()) != 0;
        }
        
        /**
         * @brief Checks if the end-of-file indicator is set for the file.
         * 
         * @return True if the end-of-file indicator is set, false otherwise.
         */
        [[nodiscard]]
        bool eof() const noexcept {
            return handle && stdx::io::feof(handle.get()) != 0;
        }

        /**
         * @brief Clears the error and end-of-file indicators for the file.
         */
        void clear_error() noexcept {
            if (handle) {
                stdx::io::clearerr(handle.get());
            }
        }

        [[nodiscard]]
        bool exists() const noexcept {
            return file_path.has_value() && stdx::fs::exists(*file_path);
        }

        /**
         * @brief Returns the size of the file in bytes.
         * 
         * @return The size of the file in bytes.
         * @throws IOException if the file does not exist or has no path.
         */
        [[nodiscard]]
        uintmax file_size() const throws (IOException) {
            if (!exists()) {
                throw IOException("File has no path.");
            }
            return stdx::fs::file_size(*file_path);
        }

        /**
         * @brief Returns the size of the file in bytes, or an empty Optional if the file does not exist or has no path.
         * 
         * @return Optional containing the size of the file in bytes, or empty if the file does not exist or has no path.
         */
        [[nodiscard]]
        Optional<uintmax> try_file_size() const noexcept {
            if (!exists()) {
                return {};
            }
            return stdx::fs::file_size(*file_path);
        }

        /**
         * @brief Creates a new file with the specified path and mode.
         * 
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created File object.
         * @throws IOException if the file could not be created.
         */
        [[nodiscard]]
        static File open(const Path& path, StringView mode) throws (IOException) {
            File file(path, mode);
            if (!file) {
                throw IOException(stdx::fmt::format("Failed to open file: {}", path.string()));
            }
            return file;
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode.
         * 
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created File object, or empty if the file could not be created.
         */
        [[nodiscard]]
        static Optional<File> try_open(const Path& path, StringView mode) noexcept {
            File file(path, mode);
            if (!file) {
                return {};
            }
            return file;
        }

        /**
         * @brief Creates a new file with the specified path and mode, but only if it does not already exist.
         * 
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created File object.
         * @throws IOException if the file already exists or could not be created.
         */
        [[nodiscard]]
        static File create(const Path& path) throws (IOException) {
            if (stdx::fs::exists(path)) {
                throw IOException(stdx::fmt::format("File already exists: {}", path.string()));
            }
            return open(path, "w");
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode, but only if it does not already exist.
         * 
         * @param path The path of the file to create.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created File object, or empty if the file already exists or could not be created.
         */
        [[nodiscard]]
        static Optional<File> try_create(const Path& path) noexcept {
            if (stdx::fs::exists(path)) {
                return {};
            }
            return try_open(path, "w");
        }

        /**
         * @brief Creates a new file with the specified path and mode, or opens it if it already exists.
         * 
         * @param path The path of the file to create or open.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return The created or opened File object.
         * @throws IOException if the file could not be created or opened.
         */
        [[nodiscard]]
        static File append(const Path& path) throws (IOException) {
            return open(path, "a");
        }

        /**
         * @brief Attempts to create a new file with the specified path and mode, or opens it if it already exists.
         * 
         * @param path The path of the file to create or open.
         * @param mode The mode to open the file with (e.g., "w" for write, "a" for append).
         * @return Optional containing the created or opened File object, or empty if the file could not be created or opened.
         */
        [[nodiscard]]
        static Optional<File> try_append(const Path& path) noexcept {
            return try_open(path, "a");
        }
    };

    File::Handle* const Stdin = ::stdin;
    File::Handle* const Stdout = ::stdout;
    File::Handle* const Stderr = ::stderr;

    [[nodiscard]]
    File::Handle* stdin() noexcept {
        return File::stdin();
    }

    [[nodiscard]]
    File::Handle* stdout() noexcept {
        return File::stdout();
    }

    [[nodiscard]]
    File::Handle* stderr() noexcept {
        return File::stderr();
    }
}
