/**
 * @file expected.cppm
 * @module core:expected
 * @brief Module file for importing the Expected class from the standard library.
 * 
 * This file exports the Expected class operations in the standard library.
 */

module;

#if __has_include(<span>)
#include <expected>
#endif

#include "Macros.hpp"

export module core:expected;

import :meta;
import :string_view;
import :util;
import :variant;

using core::meta::DecayType;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #if __has_include(<expected>)
    template <typename T, typename E>
    using Expected = std::expected<T, E>;

    template <typename E>
    using Unexpected = std::unexpected<E>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using Unexpect_t = std::unexpect_t;
    #endif

    using UnexpectTag = std::unexpect_t;
    inline constexpr UnexpectTag Unexpect = std::unexpect;
    
    template <typename E>
    using BadExpectedAccessException = std::bad_expected_access<E>;

    template <typename E>
    class [[nodiscard]] Error {
    private:
        E err;
        StringView msg;
    public:
        constexpr Error(E err, StringView msg = ""):
            err{err}, msg{msg} {}
        constexpr ~Error() = default;
        constexpr Error(const Error& other) = default;
        constexpr Error(Error&& other) = default;
        constexpr Error& operator=(const Error& other) = default;
        constexpr Error& operator=(Error&& other) = default;

        [[nodiscard]]
        constexpr E error() const noexcept {
            return err;
        }

        [[nodiscard]]
        constexpr StringView message() const noexcept {
            return msg;
        }
    };

    using std::swap;

    /**
     * @brief Explicitly constructs a successful Expected value.
     *
     * @tparam T The success value type.
     * @tparam Args The types of arguments to construct the value.
     * @param args Arguments forwarded to construct the value.
     * @return The constructed value (relies on implicit conversion to Expected).
     */
    template <typename T, typename... Args>
    inline constexpr T Ok(Args&&... args) {
        return T(std::forward<Args>(args)...);
    }

    /**
     * @brief Explicitly constructs a successful Expected value (deduced type).
     *
     * @tparam T The type of the success value.
     * @param x The success value.
     * @return The value (relies on implicit conversion to Expected).
     */
    template <typename T>
    inline constexpr DecayType<T> Ok(T&& x) {
        return std::forward<T>(x);
    }

    /**
     * @brief Constructs an error Expected value.
     *
     * @tparam E The error type.
     * @tparam Args The types of arguments to construct the error.
     * @param args Arguments forwarded to construct the error.
     * @return An Unexpected<E> containing the error.
     */
    template <typename E, typename... Args>
    inline constexpr Unexpected<E> Err(Args&&... args) {
        return Unexpected<E>(E(std::forward<Args>(args)...));
    }

    /**
     * @brief Constructs an error Expected value with single argument (type deduced).
     *
     * @tparam E The type of the error argument.
     * @param err The error value.
     * @return An Unexpected containing the error.
     */
    template <typename E>
    inline constexpr Unexpected<DecayType<E>> Err(E&& err) {
        return Unexpected<DecayType<E>>(std::forward<E>(err));
    }
    #endif
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
