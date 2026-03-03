#pragma once

/**
 * @namespace core::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace core::collections {
    template <usize N>
    using BitSet = std::bitset<N>;

    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator<<;
    using std::operator>>;

    using std::hash;
    using std::swap;
}
