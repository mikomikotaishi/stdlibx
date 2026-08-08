#pragma once

using stdx::io::IOException;

/**
 * @namespace stdx::zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */
export namespace stdx::zip {
    /**
     * @class ZipException
     * @brief Base exception for ZIP and compression errors.
     * @extends IOException
     */
    class ZipException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @class DataFormatException
     * @brief Thrown when compressed data is corrupt or in an unrecognized format.
     * @extends ZipException
     */
    class DataFormatException: public ZipException {
    public:
        using ZipException::ZipException;
    };
}
