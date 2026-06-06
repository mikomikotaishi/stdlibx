#pragma once

using stdx::alloc::Allocator;
using stdx::text::CharTraits;

/**
 * @namespace stdx::io
 * @brief Standard library IO operations.
 */
export namespace stdx::io {
    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicSyncBuffer = std::basic_syncbuf<Char, Traits, Alloc>;

    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicOutputSyncStream = std::basic_osyncstream<Char, Traits, Alloc>;

    using SyncBuffer = std::syncbuf;
    using WideSyncBuffer = std::wsyncbuf;
    using OutputSyncStream = std::osyncstream;
    using WideOutputSyncStream = std::wosyncstream;

    using std::swap;
}
