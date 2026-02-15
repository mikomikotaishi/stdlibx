/**
 * @file format.cppm
 * @module stdx:fmt.format
 * @brief Module file for standard library format operations.
 *
 * This file contains the implementation of the format operations in the standard library.
 */

module;

#if __has_include(<format>)
#include <format>
#endif

export module stdx:fmt.format;

/**
 * @namespace stdx::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
export namespace stdx::fmt {
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

    
    using WideFormatParseContext = std::wformat_parse_context;

    template <typename Out, typename CharT>
    using BasicFormatContext = std::basic_format_context<Out, CharT>;

    using FormatContext = std::format_context;


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
