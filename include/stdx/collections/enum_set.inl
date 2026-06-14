#pragma once

#ifdef __cpp_lib_reflection
using stdx::meta::IsEnumValue;
using stdx::meta::IsSameValue;
using stdx::meta::UnderlyingTypeType;
using stdx::meta::reflect::Info;
#endif

namespace stdx::collections {
    #ifdef __cpp_lib_reflection
    template <typename E>
    [[nodiscard]]
    consteval usize count() {
        return stdx::meta::reflect::enumerators_of(^^E).size();
    }

    template <typename E>
    [[nodiscard]]
    consteval Array<E, count<E>()> values_impl() {
        Array<E, count<E>()> result;
        usize i = 0;
        for (Info e : stdx::meta::reflect::enumerators_of(^^E)) {
            result[i++] = stdx::meta::reflect::extract<E>(e);
        }
        return result;
    }

    template <typename E>
    inline constexpr Array<E, count<E>()> VALUES_PER = values_impl<E>();

    template <typename E>
    [[nodiscard]]
    consteval Array<StringView, count<E>()> names_impl() {
        Array<StringView, count<E>()> result;
        usize i = 0;
        for (Info e : stdx::meta::reflect::enumerators_of(^^E)) {
            result[i++] = stdx::meta::reflect::identifier_of(e);
        }
        return result;
    }

    template <typename E>
    inline constexpr Array<StringView, count<E>()> NAMES_PER = names_impl<E>();
    #endif
}

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_reflection
    /**
     * @brief A bitset-backed set of enumerators.
     *
     * Requires reflection so the enumerator set of E can be discovered at
     * compile time.
     * @tparam E An enumeration type.
     */
    template <typename E>
        requires (IsEnumValue<E>)
    class EnumSet {
    public:
        using ValueType = E;
        using UnderlyingType = UnderlyingTypeType<E>;
        using SizeType = usize;

        static constexpr const Array<E, count<E>()>& VALUES = VALUES_PER<E>; ///< All enumerator values of E, in declaration order.
        static constexpr usize CAPACITY = VALUES.size(); ///< Number of enumerators of E.
    private:
        BitSet<CAPACITY> bits; ///< Bitset storing the presence of each enumerator in VALUES.

        [[nodiscard]]
        static constexpr usize index_of(E v) noexcept {
            for (usize i = 0; i < CAPACITY; ++i) {
                if (VALUES[i] == v) {
                    return i;
                }
            }
            return CAPACITY;
        }

        constexpr explicit EnumSet(BitSet<CAPACITY> b) noexcept:
            bits{b} {}
    public:
        constexpr EnumSet() noexcept = default;

        constexpr EnumSet(InitializerList<E> values) noexcept {
            for (E v : values) {
                if (usize i = index_of(v); i < CAPACITY) {
                    bits.set(i);
                }
            }
        }

        [[nodiscard]]
        static constexpr EnumSet none_of() noexcept {
            return EnumSet();
        }

        [[nodiscard]]
        static constexpr EnumSet all_of() noexcept {
            EnumSet s;
            s.bits.set();
            return s;
        }

        [[nodiscard]]
        static constexpr EnumSet of(E v) noexcept {
            EnumSet s;
            s.add(v);
            return s;
        }

        template <typename... Es>
            requires (IsSameValue<Es, E> && ...)
        [[nodiscard]]
        static constexpr EnumSet of(E first, Es... rest) noexcept {
            EnumSet s;
            s.add(first);
            (s.add(rest), ...);
            return s;
        }

        [[nodiscard]]
        static constexpr EnumSet range(E from, E to) noexcept {
            EnumSet s;
            usize start = index_of(from);
            usize end = index_of(to);
            if (start == CAPACITY || end == CAPACITY) {
                return s;
            }
            if (start > end) {
                usize tmp = start;
                start = end;
                end = tmp;
            }
            for (usize i = start; i <= end; ++i) {
                s.bits.set(i);
            }
            return s;
        }

        [[nodiscard]]
        static constexpr EnumSet copy_of(const EnumSet& other) noexcept {
            return other;
        }

        [[nodiscard]]
        static constexpr EnumSet complement_of(const EnumSet& other) noexcept {
            EnumSet s = other;
            s.bits.flip();
            return s;
        }

        [[nodiscard]]
        constexpr bool contains(E v) const noexcept {
            usize i = index_of(v);
            return i < CAPACITY && bits.test(i);
        }

        [[nodiscard]]
        constexpr bool contains_all(const EnumSet& other) const noexcept {
            return (bits & other.bits) == other.bits;
        }

        [[nodiscard]]
        constexpr bool contains_any(const EnumSet& other) const noexcept {
            return (bits & other.bits).any();
        }

        [[nodiscard]]
        constexpr usize size() const noexcept {
            return bits.count();
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept {
            return bits.none();
        }

        [[nodiscard]]
        constexpr bool is_full() const noexcept {
            return bits.all();
        }

        [[nodiscard]]
        static constexpr usize capacity() noexcept {
            return CAPACITY;
        }

        constexpr bool add(E v) noexcept {
            usize i = index_of(v);
            if (i >= CAPACITY) {
                return false;
            }
            bool was_set = bits.test(i);
            bits.set(i);
            return !was_set;
        }

        constexpr bool remove(E v) noexcept {
            usize i = index_of(v);
            if (i >= CAPACITY) {
                return false;
            }
            bool was_set = bits.test(i);
            bits.reset(i);
            return was_set;
        }

        constexpr void clear() noexcept {
            bits.reset();
        }

        constexpr bool add_all(const EnumSet& other) noexcept {
            BitSet<CAPACITY> before = bits;
            bits |= other.bits;
            return bits != before;
        }

        constexpr bool remove_all(const EnumSet& other) noexcept {
            BitSet<CAPACITY> before = bits;
            bits &= ~other.bits;
            return bits != before;
        }

        constexpr bool retain_all(const EnumSet& other) noexcept {
            BitSet<CAPACITY> before = bits;
            bits &= other.bits;
            return bits != before;
        }

        constexpr EnumSet& operator|=(const EnumSet& o) noexcept {
            bits |= o.bits;
            return *this;
        }

        constexpr EnumSet& operator&=(const EnumSet& o) noexcept {
            bits &= o.bits;
            return *this;
        }

        constexpr EnumSet& operator^=(const EnumSet& o) noexcept {
            bits ^= o.bits;
            return *this;
        }

        constexpr EnumSet& operator-=(const EnumSet& o) noexcept {
            bits &= ~o.bits;
            return *this;
        }

        [[nodiscard]]
        friend constexpr EnumSet operator|(EnumSet a, const EnumSet& b) noexcept {
            a |= b;
            return a;
        }

        [[nodiscard]]
        friend constexpr EnumSet operator&(EnumSet a, const EnumSet& b) noexcept {
            a &= b;
            return a;
        }

        [[nodiscard]]
        friend constexpr EnumSet operator^(EnumSet a, const EnumSet& b) noexcept {
            a ^= b;
            return a;
        }

        [[nodiscard]]
        friend constexpr EnumSet operator-(EnumSet a, const EnumSet& b) noexcept {
            a -= b;
            return a;
        }

        [[nodiscard]]
        constexpr EnumSet operator~() const noexcept {
            return complement_of(*this);
        }

        [[nodiscard]]
        constexpr bool operator==(const EnumSet&) const noexcept = default;

        class Iterator {
        private:
            friend class EnumSet;
            const BitSet<CAPACITY>* bits{};
            usize idx = 0;

            constexpr Iterator(const BitSet<CAPACITY>* b, usize i) noexcept:
                bits{b}, idx{i} {
                advance();
            }

            constexpr void advance() noexcept {
                while (idx < CAPACITY && !bits->test(idx)) {
                    ++idx;
                }
            }
        public:
            using IteratorCategory = ForwardIteratorTag;
            using ValueType = E;
            using Reference = E;
            using Pointer = void;
            using DifferenceType = ptrdiff;

            constexpr Iterator() noexcept = default;

            [[nodiscard]]
            constexpr E operator*() const noexcept {
                return VALUES[idx];
            }

            constexpr Iterator& operator++() noexcept {
                ++idx;
                advance();
                return *this;
            }

            constexpr Iterator operator++([[maybe_unused]] int _) noexcept {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            [[nodiscard]]
            constexpr bool operator==(const Iterator&) const noexcept = default;

            using value_type = ValueType;
            using underlying_type = UnderlyingType;
            using size_type = SizeType;
            using iterator_category = IteratorCategory;
        };

        using ConstIterator = Iterator;

        [[nodiscard]]
        constexpr Iterator begin() const noexcept {
            return Iterator(&bits, 0);
        }
        [[nodiscard]]
        constexpr Iterator end() const noexcept {
            return Iterator(&bits, CAPACITY);
        }

        [[nodiscard]]
        constexpr Iterator cbegin() const noexcept {
            return begin();
        }

        [[nodiscard]]
        constexpr Iterator cend() const noexcept {
            return end();
        }

        using iterator = Iterator;
        using const_iterator = ConstIterator;
    };
    #endif
}

#ifdef __cpp_lib_reflection
using stdx::collections::EnumSet;

namespace stdx::fmt {
    template <typename T, typename Char>
    struct Formatter<EnumSet<T>, Char> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const EnumSet<T>& s, FormatContext& ctx) {
            auto out = ctx.out();
            out = format_to(out, "{{");
            bool first = true;
            for (usize i = 0; i < EnumSet<T>::CAPACITY; ++i) {
                if (s.contains(EnumSet<T>::VALUES[i])) {
                    if (!first) {
                        out = format_to(out, ", ");
                    }
                    out = format_to(out, "{}", stdx::collections::NAMES_PER<T>[i]);
                    first = false;
                }
            }
            return format_to(out, "}}");
        }
    };
}

template <typename T, typename Char>
struct stdx::fmt::formatter<EnumSet<T>, Char> : public Formatter<EnumSet<T>, Char> {};
#endif
