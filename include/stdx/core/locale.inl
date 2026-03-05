#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
namespace stdx::core {
    using ::core::Locale;
    using ::core::LocaleId;
    using ::core::LocaleFacet;
    using ::core::LocaleCategory;
    using ::core::CharTypeBase;
    using ::core::CodeConverterBase;
    using ::core::MessagesBase;
    using ::core::TimeBase;
    using ::core::MoneyBase;
    using ::core::CharType;
    using ::core::CodeConvert;
    using ::core::Collate;
    using ::core::Messages;
    using ::core::TimeGet;
    using ::core::TimePut;
    using ::core::NumericGet;
    using ::core::NumericPut;
    using ::core::NumericPunctuation;
    using ::core::MoneyGet;
    using ::core::MoneyPut;
    using ::core::MoneyPunctuation;
    using ::core::CharTypeByName;
    using ::core::CodeConvertByName;
    using ::core::MessagesByName;
    using ::core::CollateByName;
    using ::core::TimeGetByName;
    using ::core::TimePutByName;
    using ::core::NumericPunctuationByName;
    using ::core::MoneyPunctuationByName;

    using ::core::use_facet;
    using ::core::has_facet;
    using ::core::isspace;
    using ::core::isprint;
    using ::core::iscntrl;
    using ::core::isupper;
    using ::core::islower;
    using ::core::isalpha;
    using ::core::isdigit;
    using ::core::ispunct;
    using ::core::isxdigit;
    using ::core::isalnum;
    using ::core::isgraph;
    using ::core::toupper;
    using ::core::tolower;
}
