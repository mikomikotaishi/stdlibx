#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    template <typename Char>
    using BasicSyncBuffer = std::basic_syncbuf<Char>;

    template <typename Char>
    using BasicOutputSyncStream = std::basic_osyncstream<Char>;

    using SyncBuffer = std::syncbuf;
    using WideSyncBuffer = std::wsyncbuf;
    using OutputSyncStream = std::osyncstream;
    using WideOutputSyncStream = std::wosyncstream;

    using std::swap;
}
