#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    using ::core::math::Quaternion;
    using ::core::math::operator*;
    using ::core::math::slerp;
}

using stdx::math::Quaternion;

namespace stdx::fmt {
    template <typename T, typename Char>
    struct Formatter<Quaternion<T>, Char> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const Quaternion<T>& q, FormatContext& ctx) {
            return format_to(ctx.out(), "({} + {}i + {}j + {}k)", q.w(), q.x(), q.y(), q.z());
        }
    };
}

template <typename  T, typename Char>
struct stdx::fmt::formatter<Quaternion<T>, Char> : public Formatter<Quaternion<T>, Char> {};
