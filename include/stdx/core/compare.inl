#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T>
    concept ThreeWayComparable = std::three_way_comparable<T>;

    template <typename T, typename U>
    concept ThreeWayComparableWith = std::three_way_comparable_with<T, U>;

    class Compare final {
    public:
        Compare() = delete;

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool equal(T t, U u) noexcept {
            return std::cmp_equal(t, u);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool not_equal(T t, U u) noexcept {
            return std::cmp_not_equal(t, u);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool less(T t, U u) noexcept {
            return std::cmp_less(t, u);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool greater(T t, U u) noexcept {
            return std::cmp_greater(t, u);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool less_equal(T t, U u) noexcept {
            return std::cmp_less_equal(t, u);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static constexpr bool greater_equal(T t, U u) noexcept {
            return std::cmp_greater_equal(t, u);
        }
    };

    class [[nodiscard]] PartialOrdering final {
    public:
        using Self = std::partial_ordering;

        static constexpr Self LESS = std::partial_ordering::less;
        static constexpr Self EQUIVALENT = std::partial_ordering::equivalent;
        static constexpr Self GREATER = std::partial_ordering::greater;
        static constexpr Self UNORDERED = std::partial_ordering::unordered;
    private:
        const Self value;
    public:
        constexpr PartialOrdering() noexcept = delete;

        constexpr PartialOrdering(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        /**
         * @brief Compares against the literal 0, as the standard comparison-category
         * types do, so this wrapper may be returned from a user-defined operator<=>.
         */
        [[nodiscard]]
        friend constexpr bool operator==(PartialOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value == 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<(PartialOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value < 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>(PartialOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value > 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<=(PartialOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value <= 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>=(PartialOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value >= 0;
        }
    };

    class [[nodiscard]] WeakOrdering final {
    public:
        using Self = std::weak_ordering;

        static constexpr Self LESS = std::weak_ordering::less;
        static constexpr Self EQUIVALENT = std::weak_ordering::equivalent;
        static constexpr Self GREATER = std::weak_ordering::greater;
    private:
        const Self value;
    public:
        constexpr WeakOrdering() noexcept = delete;

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

        /**
         * @brief Compares against the literal 0, as the standard comparison-category
         * types do, so this wrapper may be returned from a user-defined operator<=>.
         */
        [[nodiscard]]
        friend constexpr bool operator==(WeakOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value == 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<(WeakOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value < 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>(WeakOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value > 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<=(WeakOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value <= 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>=(WeakOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value >= 0;
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
        const Self value;
    public:
        constexpr StrongOrdering() noexcept = delete;

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

        /**
         * @brief Compares against the literal 0, as the standard comparison-category
         * types do, so this wrapper may be returned from a user-defined operator<=>.
         */
        [[nodiscard]]
        friend constexpr bool operator==(StrongOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value == 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<(StrongOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value < 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>(StrongOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value > 0;
        }

        [[nodiscard]]
        friend constexpr bool operator<=(StrongOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value <= 0;
        }

        [[nodiscard]]
        friend constexpr bool operator>=(StrongOrdering lhs, decltype(nullptr)) noexcept {
            return lhs.value >= 0;
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
