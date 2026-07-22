#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
namespace stdx::core {
    using CharTypeBase = std::ctype_base;
    using CodeConverterBase = std::codecvt_base;
    using MessagesBase = std::messages_base;
    using TimeBase = std::time_base;
    using MoneyBase = std::money_base;

    template <typename Char>
    using CharType = std::ctype<Char>;

    template <typename Intern, typename Extern, typename State>
    using CodeConvert = std::codecvt<Intern, Extern, State>;

    template <typename Char>
    using Collate = std::collate<Char>;

    template <typename Char>
    using Messages = std::messages<Char>;

    template <typename Char, typename InputIt = InputStreamBufferIterator<Char>>
    using TimeGet = std::time_get<Char, InputIt>;

    template <typename Char, typename OutputIt = OutputStreamBufferIterator<Char>>
    using TimePut = std::time_put<Char, OutputIt>;

    template <typename Char, typename InputIt = InputStreamBufferIterator<Char>>
    using NumericGet = std::num_get<Char, InputIt>;

    template <typename Char, typename OutputIt = OutputStreamBufferIterator<Char>>
    using NumericPut = std::num_put<Char, OutputIt>;

    template <typename Char>
    using NumericPunctuation = std::numpunct<Char>;

    template <typename Char, typename InputIt = InputStreamBufferIterator<Char>>
    using MoneyGet = std::money_get<Char, InputIt>;

    template <typename Char, typename OutputIt = OutputStreamBufferIterator<Char>>
    using MoneyPut = std::money_put<Char, OutputIt>;

    template <typename Char, bool Intl = false>
    using MoneyPunctuation = std::moneypunct<Char, Intl>;

    template <typename Char>
    using CharTypeByName = std::ctype_byname<Char>;

    template <typename Intern, typename Extern, typename State>
    using CodeConvertByName = std::codecvt_byname<Intern, Extern, State>;

    template <typename Char>
    using MessagesByName = std::messages_byname<Char>;

    template <typename Char>
    using CollateByName = std::collate_byname<Char>;

    template <typename Char, typename InputIt = InputStreamBufferIterator<Char>>
    using TimeGetByName = std::time_get_byname<Char, InputIt>;

    template <typename Char, typename OutputIt = OutputStreamBufferIterator<Char>>
    using TimePutByName = std::time_put_byname<Char, OutputIt>;

    template <typename Char>
    using NumericPunctuationByName = std::numpunct_byname<Char>;

    template <typename Char, bool Intl = false>
    using MoneyPunctuationByName = std::moneypunct_byname<Char, Intl>;

    using std::use_facet;
    using std::has_facet;

    using std::isspace;
    using std::isprint;
    using std::iscntrl;
    using std::isupper;
    using std::islower;
    using std::isalpha;
    using std::isdigit;
    using std::ispunct;
    using std::isxdigit;
    using std::isalnum;
    using std::isgraph;
    using std::toupper;
    using std::tolower;
}
