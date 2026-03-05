#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    concept ThreeWayComparable = std::three_way_comparable<T>;

    template <typename T, typename U>
    concept ThreeWayComparableWith = std::three_way_comparable_with<T, U>;

    class [[nodiscard]] PartialOrdering final {
    public:
        using Self = std::partial_ordering;

        static constexpr Self LESS = std::partial_ordering::less;
        static constexpr Self EQUIVALENT = std::partial_ordering::equivalent;
        static constexpr Self GREATER = std::partial_ordering::greater;
        static constexpr Self UNORDERED = std::partial_ordering::unordered;
    private:
        Self value;
    public:
        constexpr PartialOrdering(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] WeakOrdering final {
    public:
        using Self = std::weak_ordering;

        static constexpr Self LESS = std::weak_ordering::less;
        static constexpr Self EQUIVALENT = std::weak_ordering::equivalent;
        static constexpr Self GREATER = std::weak_ordering::greater;
    private:
        Self value;
    public:
        constexpr WeakOrdering(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        constexpr operator PartialOrdering() const noexcept {
            return *this;
        }

        constexpr operator PartialOrdering::Self() const noexcept {
            return static_cast<PartialOrdering::Self>(value);
        }
    };

    class [[nodiscard]] StrongOrdering final {
    public:
        using Self = std::strong_ordering;

        static constexpr Self LESS = std::strong_ordering::less;
        static constexpr Self EQUIVALENT = std::strong_ordering::equivalent;
        static constexpr Self EQUAL = std::strong_ordering::equal;
        static constexpr Self GREATER = std::strong_ordering::greater;
    private:
        Self value;
    public:
        constexpr StrongOrdering(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        constexpr operator WeakOrdering() const noexcept {
            return *this;
        }

        constexpr operator WeakOrdering::Self() const noexcept {
            return static_cast<WeakOrdering::Self>(value);
        }

        constexpr operator PartialOrdering() const noexcept {
            return *this;
        }

        constexpr operator PartialOrdering::Self() const noexcept {
            return static_cast<PartialOrdering::Self>(value);
        }
    };

    template <typename... Ts>
    using CommonComparisonCategory = std::common_comparison_category<Ts...>;

    template <typename T>
    using CompareThreeWayResult = std::compare_three_way_result<T>;

    using CompareThreeWay = std::compare_three_way;
    
    using StrongFallback = decltype(std::compare_strong_order_fallback);
    using WeakFallback = decltype(std::compare_weak_order_fallback);
    using PartialFallback = decltype(std::compare_partial_order_fallback);

    inline namespace comparison {
        inline constexpr decltype(std::strong_order) StrongOrder = std::strong_order;
        inline constexpr decltype(std::weak_order) WeakOrder = std::weak_order;
        inline constexpr decltype(std::partial_order) PartialOrder = std::partial_order;

        inline constexpr StrongFallback CompareStrongOrderFallback = std::compare_strong_order_fallback;
        inline constexpr WeakFallback CompareWeakOrderFallback = std::compare_weak_order_fallback;
        inline constexpr PartialFallback ComparePartialOrderFallback = std::compare_partial_order_fallback;
    }

    using std::is_eq;
    using std::is_neq;
    using std::is_lt;
    using std::is_lteq;
    using std::is_gt;
    using std::is_gteq;

    using std::operator==;
    using std::operator<;
    using std::operator>;
    using std::operator<=;
    using std::operator>=;
    using std::operator<=>;
}
