/**
 * @file syncstream.cppm
 * @module std:io.syncstream
 * @brief Module file for standard library syncstream operations.
 *
 * This file contains the implementation of the syncstream operations in the standard library.
 */

module;

#if __has_include(<syncstream>)
#include <syncstream>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.syncstream;
#else
export module stdlib:io.syncstream;
#endif

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library IO operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    #if __has_include(<syncstream>)
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename CharT>
    using BasicSyncBuf = std::basic_syncbuf<CharT>;

    template <typename CharT>
    using BasicOSyncStream = std::basic_osyncstream<CharT>;

    using SyncBuf = std::syncbuf;
    using WSyncBuf = std::wsyncbuf;
    using OSyncStream = std::osyncstream;
    using WOSyncStream = std::wosyncstream;
    #endif

    template <typename CharT>
    using BasicSyncBuffer = std::basic_syncbuf<CharT>;

    template <typename CharT>
    using BasicOutputSyncStream = std::basic_osyncstream<CharT>;

    using SyncBuffer = std::syncbuf;
    using WideSyncBuffer = std::wsyncbuf;
    using OutputSyncStream = std::osyncstream;
    using WideOutputSyncStream = std::wosyncstream;

    using std::swap;
    #endif
}
