#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::collections::Vector;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Info;

namespace reflect = stdx::meta::reflect;

using namespace stdx::test;

/**
 * @class RingBuffer
 * @brief The code under test: a fixed-capacity queue that overwrites its oldest
 * entry once full.
 *
 * Its interesting states are the wrap-around boundaries, and none of the indices
 * that describe them are observable through the public interface - which is what
 * the tests below reach for.
 */
class RingBuffer {
private:
    static constexpr usize CAPACITY = 4; ///< How many entries fit before wrapping.

    Array<i32, CAPACITY> _slots = {}; ///< The storage, used as a circle.
    usize _head = 0; ///< Where the next push writes.
    usize _count = 0; ///< How many entries are live.

    /**
     * @brief The index that follows another, wrapping at the capacity.
     * @param index The index to advance.
     * @return The next index.
     */
    [[nodiscard]]
    static usize next_index(usize index) noexcept {
        return (index + 1) % CAPACITY;
    }

    /**
     * @brief The index of the oldest live entry.
     * @return The tail index.
     */
    [[nodiscard]]
    usize tail() const noexcept {
        return (_head + CAPACITY - _count) % CAPACITY;
    }
public:
    /**
     * @brief Appends a value, dropping the oldest entry when the buffer is full.
     * @param value The value to append.
     */
    void push(i32 value) noexcept {
        _slots[_head] = value;
        _head = next_index(_head);
        if (_count < CAPACITY) {
            ++_count;
        }
    }

    /**
     * @brief Removes and returns the oldest live entry.
     * @return The entry, or nullopt when the buffer is empty.
     */
    [[nodiscard]]
    Optional<i32> pop() noexcept {
        if (_count == 0) {
            return nullopt;
        }
        const usize index = tail();
        --_count;
        return _slots[index];
    }

    /**
     * @brief How many entries are live.
     * @return The entry count.
     */
    [[nodiscard]]
    usize size() const noexcept {
        return _count;
    }
};

/**
 * @brief The member of a class with the given name, whatever its access.
 * @param clazz The reflection of the class to search.
 * @param name The member name to look for.
 * @return The member's reflection, or a null reflection when there is no such member.
 *
 * Members are collected in an unchecked access context, so private members come back
 * alongside public ones.
 */
[[nodiscard]]
consteval Info member_named(Info clazz, StringView name) {
    for (const Info member: reflect::members_of(clazz, AccessContext::unchecked())) {
        if (reflect::has_identifier(member) && reflect::identifier_of(member) == name) {
            return member;
        }
    }
    return Info{};
}

/**
 * @brief Whether a class has a member of the given name in a given access context.
 * @param clazz The reflection of the class to search.
 * @param name The member name to look for.
 * @param ctx The access context to search in.
 * @return True if the member is visible in that context.
 */
[[nodiscard]]
consteval bool has_member(Info clazz, StringView name, AccessContext ctx) {
    for (const Info member: reflect::members_of(clazz, ctx)) {
        if (reflect::has_identifier(member) && reflect::identifier_of(member) == name) {
            return true;
        }
    }
    return false;
}

using TailFn = usize (RingBuffer::*)() const; ///< The type of the private tail() member.
using NextIndexFn = usize (*)(usize) noexcept; ///< The type of the private next_index().

// Reflections of the private internals. A private *data* member can be spliced
// directly; a private *function* cannot - GCC rejects `(buffer.[:TAIL:])()` with "is
// private within this context" - so its address is pulled out with extract(), which
// is not access-checked.
constexpr Info HEAD = member_named(^^RingBuffer, "_head");
constexpr Info COUNT = member_named(^^RingBuffer, "_count");
constexpr Info SLOTS = member_named(^^RingBuffer, "_slots");
constexpr TailFn TAIL = reflect::extract<TailFn>(member_named(^^RingBuffer, "tail"));
constexpr NextIndexFn NEXT_INDEX = reflect::extract<NextIndexFn>(
    member_named(^^RingBuffer, "next_index")
);

namespace mmt::internals {
    void test_reads_the_private_indices() {
        RingBuffer buffer;
        expect_eq(buffer.[:HEAD:], 0uz, "a fresh buffer writes at index 0");
        buffer.push(1);
        buffer.push(2);
        expect_eq(buffer.[:HEAD:], 2uz, "the write index advanced with the pushes");
        expect_eq(buffer.[:COUNT:], 2uz, "and so did the live count");
    }

    void test_observes_the_wrap_around() {
        RingBuffer buffer;
        for (i32 value = 1; value <= 5; ++value) {
            buffer.push(value);
        }
        // size() alone cannot tell a full buffer from a wrapped one; _head can.
        expect_eq(buffer.size(), 4uz, "the buffer is saturated at its capacity");
        expect_eq(buffer.[:HEAD:], 1uz, "the fifth push wrapped the write index");
        expect_eq(buffer.[:SLOTS:][0], 5, "and overwrote the oldest slot in place");
    }

    void test_writes_a_state_that_is_awkward_to_reach() {
        // Rather than pushing three values to walk the write index up to the
        // boundary, put it there and test only the transition that matters.
        RingBuffer buffer;
        buffer.[:HEAD:] = 3uz;
        buffer.push(10);
        buffer.push(20);
        expect_eq(buffer.[:HEAD:], 1uz, "the push at the last slot wrapped");
        expect_eq(*buffer.pop(), 10, "the oldest entry still comes out first");
        expect_eq(*buffer.pop(), 20, "then the one written after the wrap");
    }

    void test_calls_the_private_helpers() {
        RingBuffer buffer;
        buffer.push(1);
        buffer.push(2);
        expect_eq((buffer.*TAIL)(), 0uz, "the private tail() reached through a pointer");
        expect_eq(NEXT_INDEX(3uz), 0uz, "the private static wraps at the capacity");
        expect_eq(NEXT_INDEX(0uz), 1uz);
    }

    void test_the_access_context_decides_what_is_visible() {
        constexpr bool head_when_checked = has_member(^^RingBuffer, "_head", AccessContext::current());
        constexpr bool head_when_unchecked = has_member(^^RingBuffer, "_head", AccessContext::unchecked());
        constexpr bool size_when_checked = has_member(^^RingBuffer, "size", AccessContext::current());
        expect(!head_when_checked, "a private member is hidden from a checked context");
        expect(head_when_unchecked, "and revealed by an unchecked one");
        expect(size_when_checked, "public members show up either way");
        expect(reflect::is_private(HEAD), "_head reports its own access");
    }
}

/**
 * @class BufferFixture
 * @brief A discovery target that is a class rather than a namespace.
 *
 * Its static member functions are scanned exactly like namespace members, and since
 * discovery collects them in an unchecked access context, a private one is a test
 * too. Non-static members are skipped: a Test holds a plain function pointer, which
 * has no object to call them on.
 */
class BufferFixture {
private:
    static void test_a_private_static_is_discovered() {
        RingBuffer buffer;
        buffer.push(7);
        expect_eq(buffer.[:COUNT:], 1uz, "and it can reach the internals as well");
    }
public:
    static void test_a_public_static_is_discovered() {
        expect(true, "static member functions become tests");
    }

    void test_a_non_static_member_is_not() {
        expect(false, "never discovered, so never run");
    }
};
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    // Reflection is an access-control bypass by design: no friend declaration, no
    // change to RingBuffer, and nothing here is a macro. Reach for it when a state
    // is genuinely unreachable through the public interface - the tests bind to
    // member names, so a rename of a private member breaks them.
    return run_suites(argc, argv, {Suite::of<^^mmt::internals>(), Suite::of<^^BufferFixture>()});
    #else
    System::out.println("[example] Example disabled (compiler does not support reflection).");
    return System::EXIT_SUCCESS;
    #endif
}
