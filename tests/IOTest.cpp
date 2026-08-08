import stdx;

using stdx::collections::Vector;
using stdx::io::ByteBuffer;
using stdx::io::TextStyle;

using namespace stdx::test;

/**
 * @brief Tests the initial state, which decides what every other operation means.
 */
void test_buffer_construction() {
    ByteBuffer sized(4);
    expect_eq(sized.capacity(), 4uz, "the capacity is what was asked for");
    expect_eq(sized.limit(), 4uz, "a fresh buffer is limited by its capacity");
    expect_eq(sized.position(), 0uz, "and positioned at the start");
    expect_eq(sized.remaining(), 4uz, "so all of it remains");

    ByteBuffer filled(3, 0xEE);
    expect(filled[0] == 0xEE && filled[2] == 0xEE, "a fill value reaches every byte");

    ByteBuffer from_vector(Vector<u8>{1, 2, 3});
    expect_eq(from_vector.limit(), 3uz, "a buffer built from data is limited to that data");

    ByteBuffer listed{1, 2, 3};
    expect_eq(listed.capacity(), 3uz, "a braced list is data, not a capacity");

    // The trap that follows from having both constructors: braces prefer the
    // initializer-list overload, so these two spellings mean opposite things.
    ByteBuffer parens(4);
    ByteBuffer braces{4};
    expect_eq(parens.capacity(), 4uz, "ByteBuffer(4) is four bytes of room");
    expect_eq(braces.capacity(), 1uz, "ByteBuffer{4} is one byte holding 4");
}

/**
 * @brief Tests the write-flip-read cycle the position/limit pair exists for.
 */
void test_buffer_round_trip() {
    ByteBuffer buffer(4);
    buffer.put(0xAA);
    buffer.put(0xBB);

    expect_eq(buffer.position(), 2uz, "each put advances the position");
    expect_eq(buffer.remaining(), 2uz, "and consumes the room that was left");

    buffer.flip();
    expect_eq(buffer.position(), 0uz, "flip rewinds to the start");
    expect_eq(buffer.limit(), 2uz, "and limits reading to what was written");
    expect_eq(buffer.capacity(), 4uz, "without touching the capacity");
    expect_eq(buffer.remaining(), 2uz, "so exactly the written bytes remain");

    expect_eq(buffer.get(), u8{0xAA}, "the first byte read is the first written");
    expect_eq(buffer.get(), u8{0xBB}, "and then the second");
    expect_eq(buffer.remaining(), 0uz, "after which nothing remains");
}

/**
 * @brief Tests that the bounds are enforced rather than trusted.
 */
void test_buffer_bounds() {
    ByteBuffer buffer(2);
    buffer.put(1);
    buffer.put(2);
    expect_throws<OutOfRangeException>(
        [&] -> void { buffer.put(3); },
        "writing past the limit overflows"
    );

    buffer.flip();
    static_cast<void>(buffer.get());
    static_cast<void>(buffer.get());
    expect_throws<OutOfRangeException>(
        [&] -> void { static_cast<void>(buffer.get()); },
        "reading past the limit underflows"
    );

    ByteBuffer other(4);
    other.advance(4);
    expect_eq(other.position(), 4uz, "advancing to exactly the limit is allowed");
    expect_throws<OutOfRangeException>(
        [&] -> void { other.advance(1); },
        "advancing past it is not"
    );
}

/**
 * @brief Tests that span() is the remaining bytes, and that iteration is not.
 *
 * These two disagree on purpose - span() is what a reader still has to consume,
 * begin()/end() is everything up to the limit - but the difference is invisible
 * at the call site, so it is pinned here.
 */
void test_buffer_views() {
    ByteBuffer buffer{10, 20, 30};
    buffer.advance(1);

    expect_eq(buffer.span().size(), 2uz, "span() covers only what is left to read");
    expect_eq(buffer.span()[0], u8{20}, "starting at the current position");

    usize iterated = 0;
    for (const u8 _: buffer) {
        ++iterated;
    }
    expect_eq(iterated, 3uz, "iteration covers everything up to the limit, ignoring the position");

    buffer.mutable_span()[0] = 99;
    expect_eq(buffer[1], u8{99}, "mutable_span writes through to the buffer");
}

/**
 * @brief Tests resize and assign, which both have to restore the invariant.
 */
void test_buffer_resize_and_assign() {
    ByteBuffer buffer(8);
    buffer.advance(6);
    buffer.resize(3);
    expect_eq(buffer.limit(), 3uz, "resize moves the limit");
    expect_eq(buffer.position(), 3uz, "and drags a position that was past it back");

    const Vector<u8> data{7, 8};
    buffer.assign(data.begin(), data.end());
    expect_eq(buffer.position(), 0uz, "assign rewinds");
    expect_eq(buffer.limit(), 2uz, "and relimits to the new data");
    expect_eq(buffer[0], u8{7}, "which is the data given");
}

/**
 * @brief Tests what clear() does, which is not what the rest of the API implies.
 */
void test_buffer_clear_releases_storage() {
    ByteBuffer buffer(4);
    buffer.put(1);
    buffer.clear();

    expect_eq(buffer.capacity(), 0uz, "clear() releases the storage");
    expect_eq(buffer.limit(), 0uz, "leaving nothing to write into");
    expect(buffer.empty(), "and reporting itself empty");
    expect_throws<OutOfRangeException>(
        [&] -> void { buffer.put(1); },
        "so the nio habit of clear-then-write overflows"
    );

    // resize is what actually returns it to a writable state.
    buffer.resize(2);
    expect_no_throw([&] -> void { buffer.put(1); }, "resize is how a cleared buffer is reused");
}

/**
 * @brief Tests that rewind re-reads without changing what is readable.
 */
void test_buffer_rewind() {
    ByteBuffer buffer{1, 2, 3};
    expect_eq(buffer.get(), u8{1}, "the first read consumes the first byte");
    expect_eq(buffer.get(), u8{2}, "and the second the second");

    buffer.rewind();
    expect_eq(buffer.position(), 0uz, "rewind returns to the start");
    expect_eq(buffer.limit(), 3uz, "and leaves the limit alone, unlike flip");
    expect_eq(buffer.get(), u8{1}, "so the same bytes read again");
}

/**
 * @brief Tests mark and reset, the bounded form of rewind.
 */
void test_buffer_mark_and_reset() {
    ByteBuffer buffer{1, 2, 3, 4};
    expect(!buffer.has_mark(), "a fresh buffer has no mark");
    expect_throws<IllegalStateException>(
        [&] -> void { buffer.reset(); },
        "resetting without a mark is a programmer error, not a rewind"
    );

    static_cast<void>(buffer.get());
    buffer.mark();
    expect(buffer.has_mark(), "mark records the position");

    static_cast<void>(buffer.get());
    static_cast<void>(buffer.get());
    expect_eq(buffer.position(), 3uz, "reads move on from it");

    buffer.reset();
    expect_eq(buffer.position(), 1uz, "reset returns to the mark, not to the start");
    expect_eq(buffer.get(), u8{2}, "so reading resumes where it was marked");

    buffer.rewind();
    expect(!buffer.has_mark(), "rewind discards the mark it invalidates");
}

/**
 * @brief Tests the read-flip-parse-compact-refill cycle a reactor runs on.
 *
 * This is the operation the class was missing. A codec that consumed part of
 * what arrived has to keep the tail, move it to the front, and go on filling
 * behind it; without compact each round either reallocates or hand-rolls the
 * same move and gets the position arithmetic wrong.
 */
void test_buffer_compact() {
    ByteBuffer buffer(8);

    // A first read arrives and is partly consumed.
    buffer.put(0xA1);
    buffer.put(0xA2);
    buffer.put(0xA3);
    buffer.flip();
    expect_eq(buffer.get(), u8{0xA1}, "the parser consumes what it understands");

    buffer.compact();
    expect_eq(buffer.position(), 2uz, "compact leaves the position past the bytes it kept");
    expect_eq(buffer.limit(), 8uz, "and reopens the rest of the capacity for writing");
    expect_eq(buffer.remaining(), 6uz, "so there is room for the next read");

    // The next read appends behind what was kept.
    buffer.put(0xA4);
    buffer.flip();
    expect_eq(buffer.get(), u8{0xA2}, "the unconsumed bytes survived the move");
    expect_eq(buffer.get(), u8{0xA3}, "in order");
    expect_eq(buffer.get(), u8{0xA4}, "followed by what arrived afterwards");
    expect_eq(buffer.remaining(), 0uz, "and nothing else");
}

/**
 * @brief Tests compact in its two degenerate cases.
 */
void test_buffer_compact_edges() {
    ByteBuffer drained(4);
    drained.put(1);
    drained.flip();
    static_cast<void>(drained.get());
    drained.compact();
    expect_eq(drained.position(), 0uz, "compacting a fully-read buffer starts over at zero");
    expect_eq(drained.limit(), 4uz, "with all of the capacity available");

    ByteBuffer untouched{7, 8, 9};
    untouched.compact();
    expect_eq(untouched.position(), 3uz, "compacting an unread buffer moves nothing");
    expect_eq(untouched[0], u8{7}, "and leaves the bytes where they were");
}

/**
 * @brief Tests the multi-byte accessors, which default to network order.
 *
 * Every wire format ahead of this - TLS records, HTTP/2 frame headers,
 * WebSocket lengths - is big-endian, so a default that matched the host would
 * be right on nothing and silently wrong on x86.
 */
void test_buffer_integers_default_to_network_order() {
    ByteBuffer buffer(14);
    buffer.put_u16(0x0102);
    buffer.put_u32(0x03040506);
    buffer.put_u64(0x0708090A0B0C0D0Eull);
    buffer.flip();

    expect_eq(buffer.remaining(), 14uz, "the three writes occupy 2 + 4 + 8 bytes");
    expect_eq(buffer[0], u8{0x01}, "the most significant byte is written first");
    expect_eq(buffer[1], u8{0x02}, "and the least significant last");

    expect_eq(buffer.get_u16(), u16{0x0102}, "a 16-bit value round-trips");
    expect_eq(buffer.get_u32(), u32{0x03040506}, "so does a 32-bit one");
    expect_eq(buffer.get_u64(), u64{0x0708090A0B0C0D0Eull}, "and a 64-bit one");
}

/**
 * @brief Tests that the byte order is per call, and independent of the host's.
 */
void test_buffer_integer_byte_order() {
    ByteBuffer buffer(4);
    buffer.put_u32(0x01020304, Endian::LITTLE);
    buffer.flip();

    expect_eq(buffer[0], u8{0x04}, "little-endian writes the least significant byte first");
    expect_eq(buffer[3], u8{0x01}, "and the most significant last");

    expect_eq(
        buffer.get_u32(Endian::LITTLE),
        u32{0x01020304},
        "reading back in the same order recovers the value"
    );

    buffer.rewind();
    expect_eq(
        buffer.get_u32(Endian::BIG),
        u32{0x04030201},
        "reading in the other order reverses it, which is the whole point of naming it"
    );

    // Mixed orders in one buffer, which is what a stateful order() setting
    // would make dependent on call history.
    ByteBuffer mixed(6);
    mixed.put_u16(0xABCD);
    mixed.put_u32(0x11223344, Endian::LITTLE);
    mixed.flip();
    expect_eq(mixed.get_u16(), u16{0xABCD}, "the big-endian field reads correctly");
    expect_eq(mixed.get_u32(Endian::LITTLE), u32{0x11223344}, "and so does the little-endian one");
}

/**
 * @brief Tests that a partial multi-byte value is refused rather than assembled from junk.
 */
void test_buffer_integer_bounds() {
    ByteBuffer buffer{1, 2, 3};
    expect_throws<OutOfRangeException>(
        [&] -> void { static_cast<void>(buffer.get_u32()); },
        "reading four bytes from three underflows"
    );
    expect_eq(buffer.position(), 0uz, "and consumes nothing when it fails");

    expect_eq(buffer.get_u16(), u16{0x0102}, "the bytes that are there still read");
    expect_throws<OutOfRangeException>(
        [&] -> void { static_cast<void>(buffer.get_u16()); },
        "and the trailing single byte is not padded into a pair"
    );

    ByteBuffer small(3);
    expect_throws<OutOfRangeException>(
        [&] -> void { small.put_u32(0); },
        "writing four bytes into three overflows"
    );
    expect_eq(small.position(), 0uz, "leaving the position untouched");
}

/**
 * @brief Tests that a style with nothing set emits nothing.
 *
 * Emitting a bare "\033[m" for an empty style would reset the terminal on every
 * unstyled print, so the empty case has to be an empty string.
 */
void test_style_empty() {
    expect_eq(TextStyle().ansi_open(), "", "an unset style emits no escape at all");
    expect_eq(
        TextStyle()
        .fg(TextStyle::Color::BLACK).ansi_open(),
        "\033[38;2;0;0;0m",
        "but a color of zero is still a color, and must not be mistaken for unset"
    );
}

/**
 * @brief Tests the emphasis bit-to-SGR-code mapping.
 *
 * The codes are not the bit positions: reverse is 7 and conceal is 8, because
 * 6 is a rapid-blink nobody implements. An off-by-one in that table is silent
 * until a terminal renders the wrong attribute.
 */
void test_style_emphasis() {
    expect_eq(
        TextStyle()
            .bold()
            .ansi_open(),
        "\033[1m",
        "bold is 1"
    );
    expect_eq(
        TextStyle()
            .faint()
            .ansi_open(),
        "\033[2m",
        "faint is 2"
    );
    expect_eq(
        TextStyle()
            .italic()
            .ansi_open(),
        "\033[3m",
        "italic is 3"
    );
    expect_eq(
        TextStyle()
            .underline()
            .ansi_open(),
        "\033[4m",
        "underline is 4"
    );
    expect_eq(
        TextStyle()
            .with(TextStyle::Emphasis::REVERSE)
            .ansi_open(),
        "\033[7m",
        "reverse is 7, skipping the unimplemented 6"
    );
    expect_eq(
        TextStyle()
            .with(TextStyle::Emphasis::STRIKETHROUGH)
            .ansi_open(),
        "\033[9m",
        "and strikethrough is 9"
    );

    expect_eq(
        TextStyle()
            .bold()
            .italic()
            .underline()
            .ansi_open(),
        "\033[1;3;4m",
        "several attributes are separated, in bit order"
    );
    expect_eq(
        TextStyle()
            .bold()
            .bold()
            .ansi_open(),
        "\033[1m",
        "setting one twice does not repeat it"
    );
}

/**
 * @brief Tests true-color foreground and background sequences.
 */
void test_style_rgb() {
    expect_eq(
        TextStyle()
            .fg(TextStyle::Color::RED)
            .ansi_open(),
        "\033[38;2;255;0;0m",
        "a named color becomes a 24-bit foreground"
    );
    expect_eq(
        TextStyle()
            .bg(TextStyle::Color::BLUE)
            .ansi_open(),
        "\033[48;2;0;0;255m",
        "and 48 rather than 38 for a background"
    );
    expect_eq(
        TextStyle()
            .fg(TextStyle::Color::RED)
            .ansi_open(),
        "\033[38;2;255;0;0m",
        "an explicit RGB triple is emitted verbatim"
    );
    expect_eq(
        TextStyle()
            .fg(TextStyle::Color::WHITE)
            .ansi_open(),
        "\033[38;2;255;255;255m",
        "the hex value is unpacked most-significant byte first"
    );
}

/**
 * @brief Tests the 16-color codes, where a background is the foreground plus ten.
 */
void test_style_terminal_colors() {
    expect_eq(
        TextStyle()
            .fg(TextStyle::TerminalColor::RED)
            .ansi_open(),
        "\033[31m",
        "red is 31"
    );
    expect_eq(
        TextStyle()
            .bg(TextStyle::TerminalColor::RED)
            .ansi_open(),
        "\033[41m",
        "as a background, 41"
    );
    expect_eq(
        TextStyle()
            .fg(TextStyle::TerminalColor::BRIGHT_CYAN)
            .ansi_open(),
        "\033[96m",
        "the bright range starts at 90"
    );
    expect_eq(
        TextStyle()
            .bg(TextStyle::TerminalColor::BRIGHT_CYAN)
            .ansi_open(),
        "\033[106m",
        "and its backgrounds at 100"
    );
}

/**
 * @brief Tests that attributes and both colors compose into one escape.
 *
 * The separator is written between elements and not after them, which is the
 * part that breaks first: a trailing ';' makes the whole sequence invalid.
 */
void test_style_composition() {
    expect_eq(
        TextStyle()
            .bold()
            .fg(TextStyle::TerminalColor::GREEN)
            .bg(TextStyle::TerminalColor::BLACK)
            .ansi_open(),
        "\033[1;32;40m",
        "emphasis, then foreground, then background, separated once each"
    );
    expect_eq(
        TextStyle()
            .fg(TextStyle::Color::RED)
            .bg(TextStyle::Color::BLUE)
            .ansi_open(),
        "\033[38;2;255;0;0;48;2;0;0;255m",
        "two true-color halves join without a stray separator"
    );

    const String composed = TextStyle()
        .bold()
        .underline()
        .fg(TextStyle::Color::YELLOW)
        .ansi_open();
    expect(!composed.ends_with(";m"), "no sequence ends with a dangling separator");
    expect(composed.starts_with("\033["), "and every sequence opens the same way");
}

/**
 * @brief Tests that the pipe spelling builds the same style as the fluent one.
 */
void test_style_pipe_composition() {
    using stdx::io::bg;
    using stdx::io::emphasis;
    using stdx::io::fg;

    expect_eq(
        (fg(TextStyle::Color::RED) | emphasis(TextStyle::Emphasis::BOLD)).ansi_open(),
        TextStyle().fg(TextStyle::Color::RED).bold().ansi_open(),
        "the pipe form composes to the same style as the fluent form"
    );
    expect_eq(
        (fg(TextStyle::TerminalColor::GREEN) | bg(TextStyle::TerminalColor::BLACK) | emphasis(TextStyle::Emphasis::ITALIC)).ansi_open(),
        TextStyle().fg(TextStyle::TerminalColor::GREEN).bg(TextStyle::TerminalColor::BLACK).italic().ansi_open(),
        "and keeps doing so as the chain grows"
    );
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Buffer.construction", test_buffer_construction},
        {"Buffer.round_trip", test_buffer_round_trip},
        {"Buffer.bounds", test_buffer_bounds},
        {"Buffer.views", test_buffer_views},
        {"Buffer.resize_and_assign", test_buffer_resize_and_assign},
        {"Buffer.clear_releases_storage", test_buffer_clear_releases_storage},
        {"Buffer.rewind", test_buffer_rewind},
        {"Buffer.mark_and_reset", test_buffer_mark_and_reset},
        {"Buffer.compact", test_buffer_compact},
        {"Buffer.compact_edges", test_buffer_compact_edges},
        {"Buffer.integers_default_to_network_order", test_buffer_integers_default_to_network_order},
        {"Buffer.integer_byte_order", test_buffer_integer_byte_order},
        {"Buffer.integer_bounds", test_buffer_integer_bounds},
        {"TextStyle.empty", test_style_empty},
        {"TextStyle.emphasis", test_style_emphasis},
        {"TextStyle.rgb", test_style_rgb},
        {"TextStyle.terminal_colors", test_style_terminal_colors},
        {"TextStyle.composition", test_style_composition},
        {"TextStyle.pipe_composition", test_style_pipe_composition},
    });
}
