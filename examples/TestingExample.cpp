#include <version>

import stdx;

using stdx::collections::Vector;

using namespace stdx::test;

/**
 * @class Cart
 * @brief The code under test: a shopping cart of priced lines.
 */
class Cart {
private:
    /**
     * @struct Line
     * @brief One item and how many of it the cart holds.
     */
    struct Line {
        String item; ///< The item name.
        u32 quantity; ///< How many were added.
        f64 unitPrice; ///< The price of a single unit.
    };

    Vector<Line> _lines; ///< The lines, in the order they were added.
public:
    /**
     * @brief Adds a line to the cart.
     * @param item The item name.
     * @param quantity How many to add; must be positive.
     * @param unitPrice The price of a single unit; must not be negative.
     * @throws InvalidArgumentException if the quantity is zero or the price is negative.
     */
    void add(StringView item, u32 quantity, f64 unitPrice) {
        if (quantity == 0) {
            throw InvalidArgumentException("quantity must be positive");
        }
        if (unitPrice < 0.0) {
            throw InvalidArgumentException("price must not be negative");
        }
        _lines.emplace_back(Line{String(item), quantity, unitPrice});
    }

    /**
     * @brief Removes every line from the cart.
     */
    void clear() noexcept {
        _lines.clear();
    }

    /**
     * @brief The number of lines in the cart.
     * @return The line count.
     */
    [[nodiscard]]
    usize size() const noexcept {
        return _lines.size();
    }

    /**
     * @brief Whether the cart holds no lines.
     * @return True if the cart is empty.
     */
    [[nodiscard]]
    bool empty() const noexcept {
        return _lines.empty();
    }

    /**
     * @brief The cart total, discounted by the given percentage.
     * @param discount_percent How much to take off, from 0 to 100.
     * @return The discounted total.
     * @throws InvalidRangeException if the discount is outside 0 to 100.
     */
    [[nodiscard]]
    f64 total(f64 discount_percent = 0.0) const {
        if (discount_percent < 0.0 || discount_percent > 100.0) {
            throw InvalidRangeException("discount must be between 0 and 100");
        }
        f64 sum = 0.0;
        for (const Line& line: _lines) {
            sum += static_cast<f64>(line.quantity) * line.unitPrice;
        }
        return sum * (1.0 - discount_percent / 100.0);
    }
};

// The explicit style: plain functions collected into a Suite by hand. This is all
// the framework needs, and it works on every compiler.
namespace mmt::by_hand {
    void empty_cart_costs_nothing() {
        const Cart cart;
        expect(cart.empty(), "a fresh cart holds no lines");
        expect_eq(cart.size(), 0uz);
        expect_near(cart.total(), 0.0, 1e-9);
    }

    void totals_add_up() {
        Cart cart;
        cart.add("apple", 3, 0.50);
        cart.add("bread", 1, 2.25);
        expect_eq(cart.size(), 2uz, "one line per add");
        expect_near(cart.total(), 3.75, 1e-9, "quantities multiply their unit price");
        expect_near(cart.total(20.0), 3.00, 1e-9, "a 20% discount comes off the total");
    }

    void bad_input_is_rejected() {
        Cart cart;
        // A failing expect_* prints "file:line: expected ..." and marks the test
        // failed, but lets it run on; a failing require_* aborts the test there.
        expect_throws<InvalidArgumentException>([&] -> void { cart.add("apple", 0, 0.50); });
        expect_throws<InvalidArgumentException>([&] -> void { cart.add("apple", 1, -1.0); });
        expect_throws<InvalidRangeException>([&] -> void { static_cast<void>(cart.total(101.0)); });
        expect_no_throw([&] -> void { cart.add("apple", 1, 0.0); }, "a free item is fine");
        expect_eq(cart.size(), 1uz, "only the free item was added");
    }

    void currency_rounding() {
        // Not written yet; the entry below marks it disabled, so this body is
        // never reached.
        expect(false, "not implemented");
    }

    /**
     * @brief The hand-written suite.
     * @return The suite, ready to be handed to run().
     */
    [[nodiscard]]
    Suite suite() {
        return Suite {
            .name = "by_hand",
            .tests = {
                {"by_hand.empty_cart_costs_nothing", empty_cart_costs_nothing},
                {"by_hand.totals_add_up", totals_add_up},
                {"by_hand.bad_input_is_rejected", bad_input_is_rejected, {"validation"}},
                // Registered and listed, but reported as SKIP instead of running.
                {"by_hand.currency_rounding", currency_rounding, {}, "waiting on a Money type"},
            }
        };
    }
}

// The reflected style: nothing is registered by hand. Every nullary void function
// named test_* becomes a test, reported as "discovered.<name without test_>", and
// before_each/after_each/before_all/after_all are picked up as the suite's hooks.
#ifdef __cpp_impl_reflection
namespace mmt::discovered {
    Cart cart;
    usize carts_prepared = 0;

    void before_each() {
        cart.clear();
        cart.add("apple", 2, 1.50);
        ++carts_prepared;
    }

    // A shared helper, not a test: it looks exactly like a test body, but it has no
    // test_ prefix and no TestCase annotation, so discovery leaves it alone.
    void check_the_cart_is_stocked() {
        expect(!cart.empty(), "the fixture stocked the cart");
    }

    void test_the_fixture_is_fresh_every_time() {
        check_the_cart_is_stocked();
        expect_eq(cart.size(), 1uz, "before_each rebuilt the cart");
        expect_near(cart.total(), 3.00, 1e-9);
    }

    void test_clearing_empties_the_cart() {
        cart.clear();
        expect(cart.empty(), "clear() removes every line");
        expect_gt(carts_prepared, 0uz, "the hook ran before this test");
    }

    // Any name works with an explicit TestCase annotation.
    [[=TestCase()]]
    void a_discount_is_applied_to_the_whole_cart() {
        cart.add("bread", 1, 2.00);
        expect_near(cart.total(50.0), 2.50, 1e-9);
    }

    // Tags feed --tag; a DisplayName replaces the derived name entirely.
    [[=Tag("validation")]]
    [[=DisplayName("discovered.a negative price is rejected")]]
    void test_negative_price() {
        expect_throws<InvalidArgumentException>([] -> void {
            Cart local;
            local.add("apple", 1, -0.01);
        });
    }

    // Disabled keeps the test listed and reports it as SKIP without running it,
    // so it stays visible instead of quietly disappearing.
    [[=Disabled("no Money type to round against yet")]]
    void test_currency_rounding() {
        expect(false, "this body never runs");
    }

    // A test may also decide to skip itself once it sees its surroundings; this is
    // how the library's own tests handle an optional service being absent.
    void test_skips_without_a_price_feed() {
        const bool price_feed_available = false;
        if (!price_feed_available) {
            skip("no price feed configured");
        }
        expect(false, "unreachable while the feed is missing");
    }

    // Not a test: no test_ prefix and no TestCase annotation.
    [[nodiscard]]
    f64 twice(f64 value) {
        return value * 2.0;
    }
}
#endif

int main(int argc, char* argv[]) {
    // Try `--verbose` to see the passing tests too, `--list` to list the names,
    // `--filter 'discovered.*'` to select a subset, `--tag validation` to run only
    // the tagged tests, or `--no-color` for plain output.
    #ifdef __cpp_impl_reflection
    return run_suites(argc, argv, {mmt::by_hand::suite(), Suite::of<^^mmt::discovered>()});
    #else
    System::out.println("[example] Test discovery disabled (compiler does not support reflection).");
    return run(argc, argv, mmt::by_hand::suite());
    #endif
}
