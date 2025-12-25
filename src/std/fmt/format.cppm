/**
 * @file format.cppm
 * @module std:fmt.format
 * @brief Module file for standard library format operations.
 *
 * This file contains the implementation of the format operations in the standard library.
 */

module;

#if __has_include(<format>)
#include <format>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fmt.format;
#else
export module stdlib:fmt.format;
#endif

/**
 * @namespace std::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::fmt {
#else 
export namespace stdlib::fmt {
#endif
    #if __has_include(<format>)
    using std::formatter;

    template <typename T, typename CharT>
    concept Formattable = std::formattable<T, CharT>;

    using FormatException = std::format_error;

    template <typename T, typename CharT = char>
    struct Formatter: std::formatter<T, CharT> {};

    template <typename CharT>
    using BasicFormatParseContext = std::basic_format_parse_context<CharT>;

    using FormatParseContext = std::format_parse_context;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WFormatParseContext = std::wformat_parse_context;
    #endif
    
    using WideFormatParseContext = std::wformat_parse_context;

    template <typename Out, typename CharT>
    using BasicFormatContext = std::basic_format_context<Out, CharT>;

    using FormatContext = std::format_context;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WFormatContext = std::wformat_context;
    #endif

    using WideFormatContext = std::wformat_context;

    template <typename Context>
    using BasicFormatArg = std::basic_format_arg<Context>;

    template <typename Context>
    using BasicFormatArgs = std::basic_format_args<Context>;

    using FormatArgs = std::format_args;

    #ifdef NDEBUG
    using WFormatArgs = std::wformat_args;
    #endif

    using WideFormatArgs = std::wformat_args;

    template <typename CharT, typename... Args>
    using BasicFormatString = std::basic_format_string<CharT, Args...>;

    template <typename... Args>
    using FormatString = std::format_string<Args...>;

    #ifdef NDEBUG
    template <typename... Args>
    using WFormatString = std::wformat_string<Args...>;
    #endif

    template <typename... Args>
    using WideFormatString = std::wformat_string<Args...>;

    using std::format;
    using std::format_to;
    using std::format_to_n;
    using std::formatted_size;
    using std::vformat;
    using std::vformat_to;
    using std::make_format_args;
    using std::make_wformat_args;
    #endif
}
