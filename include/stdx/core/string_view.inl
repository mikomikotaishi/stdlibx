#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::BasicStringView;
    using ::core::prelude::StringView;
    using ::core::prelude::Utf8StringView;
    using ::core::prelude::Utf16StringView;
    using ::core::prelude::Utf32StringView;
    using ::core::prelude::WideStringView;

    using ::core::prelude::operator==;
    using ::core::prelude::operator<=>;
    using ::core::prelude::operator<<;

    using ::core::prelude::hash;
    using ::core::prelude::swap;
    
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
}
