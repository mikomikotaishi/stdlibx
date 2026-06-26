#pragma once

#ifdef __cpp_lib_reflection
using stdx::meta::IsEnumValue;
using stdx::meta::UnderlyingTypeType;
using stdx::meta::reflect::NamesPer;
using stdx::meta::reflect::ValuesPer;
#endif

/**
 * @namespace stdx::collections
 * @brief Standard library collection operations.
 */
export namespace stdx::collections {
    #ifdef __cpp_lib_reflection
    /**
     * @class EnumMap
     * @brief An array-backed map keyed by the enumerators of E.
     * Each slot is an Optional<V>; an empty slot denotes an absent key, so V need
     * not be default-constructible (only operator[] requires that, on use).
     * @tparam K An enumeration type used as the key.
     * @tparam V The mapped value type.
     */
    template <typename K, typename V>
        requires (IsEnumValue<K>)
    class EnumMap {
    public:
        using KeyType = K;
        using MappedType = V;
        using ValueType = Pair<const K, V>;
        using SizeType = usize;
        using UnderlyingType = UnderlyingTypeType<K>;

        static constexpr const Array<K, stdx::meta::reflect::count<K>()>& KEYS = ValuesPer<K>; ///< All enumerator values of K, in declaration order.
        static constexpr usize CAPACITY = KEYS.size(); ///< Number of enumerators of K (the maximum number of entries).
    private:
        Array<Optional<V>, CAPACITY> slots{}; ///< Per-enumerator value storage; an empty Optional means the key is absent.
        usize occupied = 0; ///< Number of present entries.

        [[nodiscard]]
        static constexpr usize index_of(K k) noexcept {
            for (usize i = 0; i < CAPACITY; ++i) {
                if (KEYS[i] == k) {
                    return i;
                }
            }
            return CAPACITY;
        }
    public:
        constexpr EnumMap() noexcept = default;

        constexpr EnumMap(InitializerList<Pair<K, V>> entries) {
            for (const auto& [key, value]: entries) {
                insert_or_assign(key, value);
            }
        }

        [[nodiscard]]
        constexpr usize size() const noexcept {
            return occupied;
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept {
            return occupied == 0;
        }

        [[nodiscard]]
        static constexpr usize capacity() noexcept {
            return CAPACITY;
        }

        [[nodiscard]]
        constexpr bool contains(K key) const noexcept {
            usize i = index_of(key);
            return i < CAPACITY && slots[i].has_value();
        }

        [[nodiscard]]
        constexpr bool contains_value(const V& value) const {
            for (usize i = 0; i < CAPACITY; ++i) {
                if (slots[i].has_value() && *slots[i] == value) {
                    return true;
                }
            }
            return false;
        }

        /**
         * @brief Returns a pointer to the value mapped to @p key, or nullptr if absent.
         * @param key The key to look up.
         * @return A pointer to the mapped value, valid until the entry is removed; nullptr if @p key is absent.
         */
        [[nodiscard]]
        constexpr V* find(K key) noexcept {
            usize i = index_of(key);
            if (i >= CAPACITY || !slots[i].has_value()) {
                return nullptr;
            }
            return &*slots[i];
        }

        [[nodiscard]]
        constexpr const V* find(K key) const noexcept {
            usize i = index_of(key);
            if (i >= CAPACITY || !slots[i].has_value()) {
                return nullptr;
            }
            return &*slots[i];
        }

        /**
         * @brief Returns a reference to the value mapped to @p key.
         * @param key The key to look up.
         * @return A reference to the mapped value.
         * @throws OutOfRangeException if @p key is absent.
         */
        [[nodiscard]]
        constexpr V& at(K key) {
            usize i = index_of(key);
            if (i >= CAPACITY || !slots[i].has_value()) {
                throw OutOfRangeException("no mapping for key");
            }
            return *slots[i];
        }

        [[nodiscard]]
        [[=Throws<OutOfRangeException>()]]
        constexpr const V& at(K key) const {
            usize i = index_of(key);
            if (i >= CAPACITY || !slots[i].has_value()) {
                throw OutOfRangeException("no mapping for key");
            }
            return *slots[i];
        }

        /**
         * @brief Associates @p value with @p key, replacing any existing mapping.
         * @param key The key to associate.
         * @param value The value to store.
         * @return The previous value mapped to @p key, or an empty Optional if there was none.
         */
        constexpr Optional<V> insert_or_assign(K key, V value) {
            usize i = index_of(key);
            if (i >= CAPACITY) {
                return nullopt;
            }
            Optional<V> previous = std::move(slots[i]);
            if (!previous.has_value()) {
                ++occupied;
            }
            slots[i] = std::move(value);
            return previous;
        }

        /**
         * @brief Removes the mapping for @p key, if present.
         * @param key The key whose mapping is to be removed.
         * @return The value that was mapped to @p key, or an empty Optional if there was none.
         */
        constexpr Optional<V> erase(K key) {
            usize i = index_of(key);
            if (i >= CAPACITY || !slots[i].has_value()) {
                return nullopt;
            }
            Optional<V> previous = std::move(slots[i]);
            slots[i].reset();
            --occupied;
            return previous;
        }

        /**
         * @brief Copies every entry of @p other into this map, overwriting on conflict.
         * @param other The map whose entries are to be inserted.
         */
        constexpr void put_all(const EnumMap& other) {
            for (usize i = 0; i < CAPACITY; ++i) {
                if (other.slots[i].has_value()) {
                    if (!slots[i].has_value()) {
                        ++occupied;
                    }
                    slots[i] = other.slots[i];
                }
            }
        }

        constexpr void clear() noexcept {
            for (Optional<V>& slot: slots) {
                slot.reset();
            }
            occupied = 0;
        }

        /**
         * @brief Returns a reference to the value mapped to @p key, inserting a
         * default-constructed value first if @p key is absent.
         * @note @p key must be a declared enumerator of K, and V must be
         * default-constructible.
         * @param key The key to access.
         * @return A reference to the mapped value.
         */
        [[nodiscard]]
        constexpr V& operator[](K key) {
            usize i = index_of(key);
            if (!slots[i].has_value()) {
                slots[i].emplace();
                ++occupied;
            }
            return *slots[i];
        }

        /**
         * @brief Returns the set of keys that currently have a mapping.
         * @return An EnumSet of the present keys.
         */
        [[nodiscard]]
        constexpr EnumSet<K> key_set() const noexcept {
            EnumSet<K> keys;
            for (usize i = 0; i < CAPACITY; ++i) {
                if (slots[i].has_value()) {
                    keys.insert(KEYS[i]);
                }
            }
            return keys;
        }

        /**
         * @brief Returns the present values, in key declaration order.
         * @return A Vector of the mapped values.
         */
        [[nodiscard]]
        constexpr Vector<V> values() const {
            Vector<V> result;
            for (usize i = 0; i < CAPACITY; ++i) {
                if (slots[i].has_value()) {
                    result.push_back(*slots[i]);
                }
            }
            return result;
        }

        [[nodiscard]]
        constexpr bool operator==(const EnumMap&) const = default;

        class Iterator {
        private:
            friend class EnumMap;
            const EnumMap* map{};
            usize idx = 0;

            constexpr Iterator(const EnumMap* m, usize i) noexcept:
                map{m}, idx{i} {
                advance();
            }

            constexpr void advance() noexcept {
                while (idx < CAPACITY && !map->slots[idx].has_value()) {
                    ++idx;
                }
            }
        public:
            using IteratorCategory = ForwardIteratorTag;
            using ValueType = Pair<K, const V&>;
            using Reference = Pair<K, const V&>;
            using Pointer = void;
            using DifferenceType = ptrdiff;

            constexpr Iterator() noexcept = default;

            [[nodiscard]]
            constexpr Pair<K, const V&> operator*() const noexcept {
                return Pair<K, const V&>(KEYS[idx], *map->slots[idx]);
            }

            constexpr Iterator& operator++() noexcept {
                ++idx;
                advance();
                return *this;
            }

            constexpr Iterator operator++(int _) noexcept {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            [[nodiscard]]
            constexpr bool operator==(const Iterator& other) const noexcept {
                return map == other.map && idx == other.idx;
            }

            using value_type = ValueType;
            using reference = Reference;
            using difference_type = DifferenceType;
            using iterator_category = IteratorCategory;
        };

        using ConstIterator = Iterator;

        [[nodiscard]]
        constexpr Iterator begin() const noexcept {
            return Iterator(this, 0);
        }

        [[nodiscard]]
        constexpr Iterator end() const noexcept {
            return Iterator(this, CAPACITY);
        }

        [[nodiscard]]
        constexpr Iterator cbegin() const noexcept {
            return begin();
        }

        [[nodiscard]]
        constexpr Iterator cend() const noexcept {
            return end();
        }

        using key_type = KeyType;
        using mapped_type = MappedType;
        using value_type = ValueType;
        using size_type = SizeType;
        using iterator = Iterator;
        using const_iterator = ConstIterator;
    };
    #endif
}

#ifdef __cpp_lib_reflection
using stdx::collections::EnumMap;

namespace stdx::fmt {
    template <typename K, typename V, typename Char>
    struct Formatter<EnumMap<K, V>, Char> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const EnumMap<K, V>& m, FormatContext& ctx) {
            auto out = ctx.out();
            out = format_to(out, "{{");
            bool first = true;
            for (usize i = 0; i < EnumMap<K, V>::CAPACITY; ++i) {
                if (const V* v = m.find(EnumMap<K, V>::KEYS[i])) {
                    if (!first) {
                        out = format_to(out, ", ");
                    }
                    out = format_to(out, "{}={}", NamesPer<K>[i], *v);
                    first = false;
                }
            }
            return format_to(out, "}}");
        }
    };
}

template <typename K, typename V, typename Char>
struct stdx::fmt::formatter<EnumMap<K, V>, Char> : public Formatter<EnumMap<K, V>, Char> {};
#endif
