/**
 * @file fstream.cppm
 * @module stdx:io.fstream
 * @brief Module file for standard library file stream operations.
 *
 * This file contains the implementation of the file stream operations in the standard library.
 */

module;

#include <fstream>

export module stdx:io.fstream;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    using FileBuffer = std::filebuf;
    using WideFileBuffer = std::wfilebuf;
    using InputFileStream = std::ifstream;
    using WideInputFileStream = std::wifstream;
    using OutputFileStream = std::ofstream;
    using WideOutputFileStream = std::wofstream;
    using FileStream = std::fstream;
    using WideFileStream = std::wfstream;

    using std::operator>>;
    using std::operator<<;
}
