#pragma once

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/buffer>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
    using ConstBuffer = std::experimental::net::const_buffer;
    using MutableBuffer = std::experimental::net::mutable_buffer;

    template <typename T, typename Allocator>
    using DynamicVectorBuffer = std::experimental::net::dynamic_vector_buffer<T, Allocator>;

    template <typename CharT, typename Traits, typename Allocator>
    using DynamicStringBuffer = std::experimental::net::dynamic_string_buffer<CharT, Traits, Allocator>;

    using TransferAll = std::experimental::net::transfer_all;
    using TransferAtLeast = std::experimental::net::transfer_at_least;
    using TransferExactly = std::experimental::net::transfer_exactly;

    class [[nodiscard]] StreamErrc {
    public:
        using Self = std::experimental::net::stream_errc;

        static constexpr Self SUCCESS = std::experimental::net::stream_errc();
        static constexpr Self EOF = std::experimental::net::stream_errc::eof;
        static constexpr Self NOT_FOUND = std::experimental::net::stream_errc::not_found;
    private:
        Self value;
    public:
        constexpr StreamErrc(Self value = SUCCESS) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using std::experimental::net::stream_category;
    using std::experimental::net::make_error_code;
    using std::experimental::net::make_error_condition;
    #endif
}
