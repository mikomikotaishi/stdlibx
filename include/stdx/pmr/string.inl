#pragma once

/**
 * @namespace aloc::pmr::text::string
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::text::string {
    using ::alloc::pmr::text::string::BasicString;
    using ::alloc::pmr::text::string::String;
    using ::alloc::pmr::text::string::Utf8String;
    using ::alloc::pmr::text::string::Utf16String;
    using ::alloc::pmr::text::string::Utf32String;
    using ::alloc::pmr::text::string::WideString;
}

/**
 * @namespace stdx::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr {
    using ::alloc::pmr::BasicString;
    using ::alloc::pmr::String;
    using ::alloc::pmr::Utf8String;
    using ::alloc::pmr::Utf16String;
    using ::alloc::pmr::Utf32String;
    using ::alloc::pmr::WideString;
}
