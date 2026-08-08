#pragma once

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @struct ZipEntry
     * @brief Metadata for a single entry within a ZIP archive.
     *
     * An entry represents either a file or a directory. Directory entries
     * conventionally have names ending in {@code '/'}.
     */
    struct ZipEntry {
        String name; ///< Entry path within the archive (e.g. {@code "dir/file.txt"}).
        String comment; ///< Optional per-entry comment.
        CompressionMethod method = CompressionMethod::DEFLATED; ///< Compression method.
        i64 size = -1; ///< Uncompressed size in bytes, or {@code -1} if unknown.
        i64 compressed_size = -1; ///< Compressed size in bytes, or {@code -1} if unknown.
        u32 crc = 0; ///< CRC-32 checksum of the uncompressed data.
        i64 time = -1; ///< Last-modification time as a DOS-format timestamp, or {@code -1}.

        /**
         * @param name The entry path within the archive.
         */
        explicit ZipEntry(StringView name):
            name{name} {}

        /**
         * @brief Returns {@code true} if this entry represents a directory.
         * @return {@code true} if the name ends with {@code '/'}.
         */
        [[nodiscard]]
        bool is_directory() const noexcept {
            return !name.empty() && name.back() == '/';
        }
    };
}
#endif
