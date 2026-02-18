#pragma once

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
