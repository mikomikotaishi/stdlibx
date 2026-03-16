#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::BasicString;
    using ::core::prelude::String;
    using ::core::prelude::Utf8String;
    using ::core::prelude::Utf16String;
    using ::core::prelude::Utf32String;
    using ::core::prelude::WideString;

    using ::core::prelude::stoi;
    using ::core::prelude::stol;
    using ::core::prelude::stoll;
    using ::core::prelude::stoul;
    using ::core::prelude::stoull;
    using ::core::prelude::stof;
    using ::core::prelude::stod;
    using ::core::prelude::stold;
    using ::core::prelude::to_string;
    using ::core::prelude::to_wstring;

    using ::core::prelude::operator+;
    using ::core::prelude::operator==;
    using ::core::prelude::operator<=>;

    using ::core::prelude::erase;
    using ::core::prelude::erase_if;

    using ::core::prelude::begin;
    using ::core::prelude::cbegin;
    using ::core::prelude::end;
    using ::core::prelude::cend;
    using ::core::prelude::rbegin;
    using ::core::prelude::crbegin;
    using ::core::prelude::rend;
    using ::core::prelude::crend;
    using ::core::prelude::size;
    using ::core::prelude::ssize;
    using ::core::prelude::empty;
    using ::core::prelude::data;

    using ::core::prelude::hash;
    using ::core::prelude::swap;
}
