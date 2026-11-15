#pragma once

/**
 * @namespace stdx::meta
 * @brief Standard library metaprogramming operations.
 */
namespace stdx::meta {
    /**
     * @internal
     * @struct FixedString
     * @brief A compile-time string usable as a non-type template parameter.
     * @tparam N The length of the source literal, including its terminating NUL.
     *
     * Lets a string literal ride along as a template argument, which is how an
     * annotation carries text.
     *
     * The stored @ref data keeps the terminating NUL so the character array is a
     * valid structural NTTP; @ref view hands back the string without it.
     */
    template <usize N>
    struct FixedString {
        Array<char, N> data = {}; ///< The characters, including the terminating NUL.

        consteval FixedString(const char (&str)[N]) noexcept {
            for (usize i = 0; i < N; ++i) {
                data[i] = str[i];
            }
        }

        /**
         * @brief A view over the string, excluding its terminating NUL.
         * @return The string as a view.
         */
        [[nodiscard]]
        consteval StringView view() const noexcept {
            return StringView(data.data(), N - 1);
        }
    };
}
