#pragma once

using stdx::io::BasicOutputStream;

/**
 * @namespace stdx::time
 * @brief Standard library time operations.
 */
export namespace stdx::time {
    template <typename Char, typename Traits>
    BasicOutputStream<Char, Traits>& operator<<(BasicOutputStream<Char, Traits>& os, const Month& m) {
        return os << static_cast<Month::Self>(m);
    }

    template <typename Char, typename Traits>
    BasicOutputStream<Char, Traits>& operator<<(BasicOutputStream<Char, Traits>& os, const Weekday& w) {
        return os << static_cast<Weekday::Self>(w);
    }
}
