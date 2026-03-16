#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
namespace stdx::core {
    using ::core::prelude::Locale;
    using ::core::prelude::LocaleId;
    using ::core::prelude::LocaleFacet;
    using ::core::prelude::LocaleCategory;
    using ::core::prelude::CharTypeBase;
    using ::core::prelude::CodeConverterBase;
    using ::core::prelude::MessagesBase;
    using ::core::prelude::TimeBase;
    using ::core::prelude::MoneyBase;
    using ::core::prelude::CharType;
    using ::core::prelude::CodeConvert;
    using ::core::prelude::Collate;
    using ::core::prelude::Messages;
    using ::core::prelude::TimeGet;
    using ::core::prelude::TimePut;
    using ::core::prelude::NumericGet;
    using ::core::prelude::NumericPut;
    using ::core::prelude::NumericPunctuation;
    using ::core::prelude::MoneyGet;
    using ::core::prelude::MoneyPut;
    using ::core::prelude::MoneyPunctuation;
    using ::core::prelude::CharTypeByName;
    using ::core::prelude::CodeConvertByName;
    using ::core::prelude::MessagesByName;
    using ::core::prelude::CollateByName;
    using ::core::prelude::TimeGetByName;
    using ::core::prelude::TimePutByName;
    using ::core::prelude::NumericPunctuationByName;
    using ::core::prelude::MoneyPunctuationByName;

    using ::core::prelude::use_facet;
    using ::core::prelude::has_facet;
    using ::core::prelude::isspace;
    using ::core::prelude::isprint;
    using ::core::prelude::iscntrl;
    using ::core::prelude::isupper;
    using ::core::prelude::islower;
    using ::core::prelude::isalpha;
    using ::core::prelude::isdigit;
    using ::core::prelude::ispunct;
    using ::core::prelude::isxdigit;
    using ::core::prelude::isalnum;
    using ::core::prelude::isgraph;
    using ::core::prelude::toupper;
    using ::core::prelude::tolower;
}
