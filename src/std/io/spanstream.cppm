/**
 * @file spanstream.cppm
 * @module std:io.spanstream
 * @brief Module file for standard library spanstream operations.
 *
 * This file contains the implementation of the spanstream operations in the standard library.
 */

module;

#if __has_include(<spanstream>)
#include <spanstream>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.spanstream;
#else
export module stdlib:io.spanstream;
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
    #if __has_include(<spanstream>)
    template <typename CharT>
    using BasicSpanBuf = std::basic_spanbuf<CharT>;


    template <typename CharT>
    using BasicInputSpanStream = std::basic_ispanstream<CharT>;

    template <typename CharT>
    using BasicOutputSpanStream = std::basic_ospanstream<CharT>;

    template <typename CharT>
    using BasicSpanStream = std::basic_spanstream<CharT>;


    using SpanBuffer = std::spanbuf;
    using WideSpanBuffer = std::wspanbuf;
    using InputSpanStream = std::ispanstream;
    using WideInputSpanstream = std::wispanstream;
    using OutputSpanStream = std::ospanstream;
    using WideOutputSpanStream = std::wospanstream;

    using SpanStream = std::spanstream;


    using WideSpanStream = std::wspanstream;

    using std::swap;
    #endif
}
