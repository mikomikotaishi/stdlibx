import stdx;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
using stdx::collections::Vector;
using stdx::io::ByteBuffer;
using stdx::zip::Adler32;
using stdx::zip::CompressionLevel;
using stdx::zip::CRC32;
using stdx::zip::DataFormatException;
using stdx::zip::Deflater;
using stdx::zip::GZIPDeflater;
using stdx::zip::GZIPInflater;
using stdx::zip::Inflater;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY

[[nodiscard]]
bool bytes_equal(Span<const u8> a, Span<const u8> b) noexcept {
    if (a.size() != b.size()) {
        return false;
    }
    for (usize i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Run input through a Deflater fully (FINISH), then through an Inflater, and
// return the round-tripped bytes. Throws if either step fails.
[[nodiscard]]
Vector<u8> round_trip(Span<const u8> input, bool nowrap) {
    Deflater deflater{CompressionLevel::DEFAULT_COMPRESSION, nowrap};
    deflater.set_input(input);
    ByteBuffer compressed = deflater.finish();

    Inflater inflater{nowrap};
    inflater.set_input(Span<const u8>{compressed.data(), compressed.size()});
    ByteBuffer expanded = inflater.inflate_all();

    return Vector<u8>{expanded.data(), expanded.data() + expanded.size()};
}

void test_crc32() {
    // ISO/IEC 13818-1 reference: CRC-32/ISO-HDLC of "123456789" = 0xCBF43926.
    static constexpr StringView VECTOR = "123456789";
    const Span<const u8> bytes{
        reinterpret_cast<const u8*>(VECTOR.data()), VECTOR.size()
    };
    CRC32 crc;
    crc.update(bytes);
    expect(
        crc.value() == 0xCBF43926u,
        "CRC32 of \"123456789\" equals 0xCBF43926"
    );

    // Incremental update must match single-shot.
    CRC32 crc_incremental;
    crc_incremental.update(bytes.subspan(0, 4));
    crc_incremental.update(bytes.subspan(4));
    expect(
        crc_incremental.value() == crc.value(),
        "CRC32 is associative across chunk boundaries"
    );

    // reset() returns to the initial state.
    crc.reset();
    expect(
        crc.value() == 0u,
        "CRC32::reset returns to 0"
    );
}

void test_adler32() {
    // Reference vector from the Adler-32 RFC 1950 appendix: "Wikipedia" → 0x11E60398.
    static constexpr StringView VECTOR = "Wikipedia";
    const Span<const u8> bytes{
        reinterpret_cast<const u8*>(VECTOR.data()), VECTOR.size()
    };
    Adler32 a;
    a.update(bytes);
    expect(
        a.value() == 0x11E60398u,
        "Adler32 of \"Wikipedia\" equals 0x11E60398"
    );

    // Adler-32 of the empty string is 1 (the algorithm's initial state).
    Adler32 empty;
    expect(
        empty.value() == 1u,
        "Adler32 of empty string is 1"
    );
}

void test_roundtrip_small() {
    static constexpr StringView TEXT = "The quick brown fox jumps over the lazy dog.";
    const Span<const u8> input{
        reinterpret_cast<const u8*>(TEXT.data()), TEXT.size()
    };

    const Vector<u8> back = round_trip(input, false);
    expect(
        back.size() == input.size(),
        "zlib-wrapped round-trip preserves length"
    );
    expect(
        bytes_equal(Span<const u8>{back.data(), back.size()}, input),
        "zlib-wrapped round-trip preserves bytes"
    );

    const Vector<u8> raw_back = round_trip(input, true);
    expect(
        bytes_equal(Span<const u8>{raw_back.data(), raw_back.size()}, input),
        "raw deflate round-trip preserves bytes"
    );
}

void test_roundtrip_large() {
    // Build ~256 KB of repeating-pattern data so the compressor actually has
    // something to chew on. Pattern: 0..255 cycled.
    static constexpr usize SIZE = 256 * 1024;
    Vector<u8> input(SIZE);
    for (usize i = 0; i < SIZE; ++i) {
        input[i] = static_cast<u8>(i & 0xFF);
    }

    Deflater d{CompressionLevel::BEST_COMPRESSION, false};
    d.set_input(Span<const u8>{input.data(), input.size()});
    ByteBuffer compressed = d.finish();

    // 256 KB of a perfectly repeating byte pattern should compress to <1 KB
    // with BEST_COMPRESSION. If it doesn't, something is wrong with the
    // compressor pipeline (FlushMode::FINISH not applied, output truncated, ...).
    expect(
        compressed.size() < 2 * 1024,
        "repeating pattern compresses to under 2 KB"
    );

    Inflater inf{};
    inf.set_input(Span<const u8>{compressed.data(), compressed.size()});
    ByteBuffer expanded = inf.inflate_all();

    expect(
        expanded.size() == input.size(),
        "256 KB round-trip preserves length"
    );
    expect(
        bytes_equal(
            Span<const u8>{expanded.data(), expanded.size()},
            Span<const u8>{input.data(), input.size()}
        ),
        "256 KB round-trip preserves bytes"
    );
}

void test_gzip_roundtrip() {
    static constexpr StringView TEXT =
        "GZIP wrapping adds a 10-byte header, an 8-byte trailer, and uses "
        "raw deflate underneath.";
    const Span<const u8> input{
        reinterpret_cast<const u8*>(TEXT.data()), TEXT.size()
    };

    GZIPDeflater d{CompressionLevel::DEFAULT_COMPRESSION};
    d.set_input(input);
    ByteBuffer compressed = d.finish();
    // GZIP framing must be present: first two bytes are the magic 0x1F 0x8B.
    expect(
        compressed.size() >= 2 &&
        compressed.data()[0] == 0x1F &&
        compressed.data()[1] == 0x8B,
        "GZIP magic 0x1F 0x8B at offset 0"
    );

    GZIPInflater inf{};
    inf.set_input(Span<const u8>{compressed.data(), compressed.size()});
    ByteBuffer expanded = inf.inflate_all();
    expect(
        expanded.size() == input.size(),
        "GZIP round-trip preserves length"
    );
    expect(
        bytes_equal(
            Span<const u8>{expanded.data(), expanded.size()},
            Span<const u8>{input.data(), input.size()}
        ),
        "GZIP round-trip preserves bytes"
    );
}

void test_invalid_input() {
    // Bytes that aren't a valid deflate stream - inflate() must throw
    // DataFormatException (the project's own subclass of ZipException).
    static constexpr Array<u8, 8> GARBAGE = {
        0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE,
    };
    Inflater inf{};
    inf.set_input(Span<const u8>(GARBAGE));
    bool threw = false;
    try {
        (void)inf.inflate_all();
    } catch (const DataFormatException& _) {
        threw = true;
    }
    expect(threw, "inflate of garbage throws DataFormatException");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
    return run(argc, argv, {
        {"zip.crc32", test_crc32},
        {"zip.adler32", test_adler32},
        {"zip.roundtrip_small", test_roundtrip_small},
        {"zip.roundtrip_large", test_roundtrip_large},
        {"zip.gzip_roundtrip", test_gzip_roundtrip},
        {"zip.invalid_input", test_invalid_input},
    });
    #else
    System::out.println("[test] Test disabled (enable with STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY).");
    return System::EXIT_SUCCESS;
    #endif
}
