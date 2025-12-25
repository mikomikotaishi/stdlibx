/**
 * @file fstream.cppm
 * @module std:io.fstream
 * @brief Module file for standard library file stream operations.
 *
 * This file contains the implementation of the file stream operations in the standard library.
 */

module;

#include <fstream>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.fstream;
#else
export module stdlib:io.fstream;
#endif

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using FileBuf = std::filebuf;
    using WFileBuf = std::wfilebuf;
    using IFStream = std::ifstream;
    using WIFStream = std::wifstream;
    using OFStream = std::ofstream;
    using WOFStream = std::wofstream;
    using FStream = std::fstream;
    using WFStream = std::wfstream;
    #endif

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
