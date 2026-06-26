#pragma once

using stdx::meta::DecayType;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_expected
    template <typename E>
    using BadExpectedAccessException = std::bad_expected_access<E>;

    template <typename E>
    class [[nodiscard]] ErrorDescription {
    private:
        E err;
        StringView msg;
    public:
        constexpr ErrorDescription(E err, StringView msg = ""):
            err{err}, msg{msg} {}
        constexpr ~ErrorDescription() = default;
        constexpr ErrorDescription(const ErrorDescription& other) = default;
        constexpr ErrorDescription(ErrorDescription&& other) = default;
        constexpr ErrorDescription& operator=(const ErrorDescription& other) = default;
        constexpr ErrorDescription& operator=(ErrorDescription&& other) = default;

        [[nodiscard]]
        constexpr E error() const noexcept {
            return err;
        }

        [[nodiscard]]
        constexpr StringView message() const noexcept {
            return msg;
        }
    };

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
        return Unexpected<stdx::meta::DecayType<E>>(std::forward<E>(err));
    }
    #endif
}
