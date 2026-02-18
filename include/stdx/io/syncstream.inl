#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    #if __has_include(<syncstream>)
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
