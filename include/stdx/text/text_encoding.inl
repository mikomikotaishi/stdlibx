#pragma once

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    #ifdef __cpp_lib_text_encoding
    using ::core::text::TextEncoding;
    using ::core::text::TextEncodingAliasesView;
    using ::core::text::TextEncodingId;
    using ::core::text::hash;
    #endif
}
