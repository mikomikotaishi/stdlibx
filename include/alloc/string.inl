#pragma once

/**
 * @namespace core::prelude
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core::prelude {
    using ::alloc::text::string::String;
    using ::alloc::text::string::BasicString;
    using ::alloc::text::string::Utf8String;
    using ::alloc::text::string::Utf16String;
    using ::alloc::text::string::Utf32String;
    using ::alloc::text::string::WideString;

    using ::alloc::text::string::stoi;
    using ::alloc::text::string::stol;
    using ::alloc::text::string::stoll;
    using ::alloc::text::string::stoul;
    using ::alloc::text::string::stoull;
    using ::alloc::text::string::stof;
    using ::alloc::text::string::stod;
    using ::alloc::text::string::stold;
    using ::alloc::text::string::to_string;
    using ::alloc::text::string::to_wstring;
    using ::alloc::text::string::operator+;
    using ::alloc::text::string::operator==;
    using ::alloc::text::string::operator<=>;

    using ::alloc::text::string::erase;
    using ::alloc::text::string::erase_if;
    using ::alloc::text::string::begin;
    using ::alloc::text::string::cbegin;
    using ::alloc::text::string::end;
    using ::alloc::text::string::cend;
    using ::alloc::text::string::rbegin;
    using ::alloc::text::string::crbegin;
    using ::alloc::text::string::rend;
    using ::alloc::text::string::crend;
    using ::alloc::text::string::size;
    using ::alloc::text::string::ssize;
    using ::alloc::text::string::empty;
    using ::alloc::text::string::data;

    using ::alloc::text::string::hash;
    using ::alloc::text::string::swap;
}
