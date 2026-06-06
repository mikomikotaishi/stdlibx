import stdx;

using stdx::audio::AudioException;
using stdx::audio::sampled::AudioFormat;
using stdx::audio::sampled::AudioInputStream;
using stdx::audio::sampled::AudioSystem;
using stdx::audio::sampled::Clip;
using stdx::audio::sampled::SampleFormat;
using stdx::audio::sampled::UnsupportedAudioFileException;
using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::mem::UniquePointer;
using stdx::time::Duration;
using stdx::time::Instant;
using stdx::time::SteadyClock;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

struct TestContext {
    i32 passed = 0;
    i32 failed = 0;

    void check(StringView name, bool condition) {
        if (condition) {
            System::out.println("[PASS] {}", name);
            ++passed;
        } else {
            System::err.println("[FAIL] {}", name);
            ++failed;
        }
    }
};

// The samples directory ships next to the source. Tests may be run from
// either the project root, build/, or build/tests/, so probe a few common
// prefixes before giving up. The first match wins.
[[nodiscard]]
Optional<Path> resolve_default_sample() noexcept {
    static constexpr Array<StringView, 4> CANDIDATES = {
        "tests/Audio/audio/wav/th06/th06_01-SD90_440Hz.wav",
        "../tests/Audio/audio/wav/th06/th06_01-SD90_440Hz.wav",
        "../../tests/Audio/audio/wav/th06/th06_01-SD90_440Hz.wav",
        "../../../tests/Audio/audio/wav/th06/th06_01-SD90_440Hz.wav",
    };
    for (const StringView p: CANDIDATES) {
        Path path{p};
        if (stdx::fs::exists(path)) {
            return path;
        }
    }
    return nullopt;
}

void describe(TestContext& ctx, const Path& path) {
    UniquePointer<AudioInputStream> stream;
    try {
        stream = AudioSystem::open_audio_file(path);
    } catch (const UnsupportedAudioFileException& e) {
        ctx.check(stdx::fmt::format("decodes {}", path), false);
        System::err.println("       reason: {}", e.what());
        return;
    } catch (const AudioException& e) {
        ctx.check(stdx::fmt::format("opens {}", path), false);
        System::err.println("       reason: {}", e.what());
        return;
    }
    ctx.check(stdx::fmt::format("opens {}", path), true);

    const AudioFormat fmt = stream->format();
    const u64 total = stream->total_frames();
    System::out.println(
        "       format: {} Hz, {} ch, {} frames ({:.3f} s)",
        fmt.sample_rate, fmt.channels, total,
        static_cast<f64>(total) / static_cast<f64>(fmt.sample_rate)
    );

    // The decoder normalizes everything to SampleFormat::FLOAT - confirm.
    ctx.check("decoded format is f32", fmt.format == SampleFormat::FLOAT);
    ctx.check("non-zero sample rate", fmt.sample_rate > 0);
    ctx.check("non-zero channel count", fmt.channels > 0);
    ctx.check("non-zero total frames", total > 0);

    // Pull the whole stream through read() in chunks; verify it reports a
    // consistent total and position-after-EOF matches frame_count.
    constexpr usize CHUNK_FRAMES = 4096;
    Vector<f32> buf(CHUNK_FRAMES * fmt.channels);
    u64 read_total = 0;
    while (true) {
        const usize got = stream->read(Span<f32>{buf.data(), buf.size()});
        if (got == 0) {
            break;
        }
        read_total += got;
    }
    ctx.check("read sum equals total_frames", read_total == total);
    ctx.check(
        "position after EOF equals total_frames",
        stream->position_frames() == total
    );

    // After EOF, further reads return 0 forever.
    const usize after_eof = stream->read(Span<f32>{buf.data(), buf.size()});
    ctx.check("read past EOF returns 0", after_eof == 0);
}

// @p max_seconds caps playback at the first N seconds; 0 plays in full.
void play(const Path& path, i64 max_seconds) {
    UniquePointer<AudioInputStream> stream;
    try {
        stream = AudioSystem::open_audio_file(path);
    } catch (const AudioException& e) {
        System::err.println("Cannot decode {}: {}", path, e.what());
        return;
    }
    const u64 total = stream->total_frames();
    const u32 rate = stream->format().sample_rate;
    const f64 length = static_cast<f64>(total) / static_cast<f64>(rate);
    if (max_seconds > 0 && static_cast<f64>(max_seconds) < length) {
        System::out.println(
            "Playing {} (first {}s of {:.2f}s)…", path, max_seconds, length
        );
    } else {
        System::out.println("Playing {} ({:.2f}s)…", path, length);
    }
    try {
        Clip clip{Ops::move(stream)};
        clip.play();
        // Poll instead of wait_for_completion() so the wait ends at whichever
        // comes first: natural EOF (is_playing() flips) or the abridge cap.
        const Instant<SteadyClock> start = SteadyClock::now();
        while (clip.is_playing()) {
            const f64 elapsed = Duration<f64>(SteadyClock::now() - start).count();
            if (max_seconds > 0 && elapsed >= static_cast<f64>(max_seconds)) {
                break;
            }
            System::Thread::sleep_for(10ms);
        }
        clip.close();
        System::out.println("Playback finished.");
    } catch (const AudioException& e) {
        System::err.println("Playback failed: {}", e.what());
    }
}

int main(int argc, char* argv[]) {
    ArgumentParser parser("WavTest", "1.0", DefaultArguments::NONE);
    parser.add_argument("--file")
        .default_value("")
        .help("WAV file to test. If omitted, the bundled th06 sample is used.");
    parser.add_argument("--play")
        .flag()
        .help("After decoding, also play the file through the default output.");
    parser.add_argument("--abridge")
        .default_value(static_cast<i64>(0))
        .scan<'d', i64>()
        .help(
            "Limit playback to the first N seconds (e.g. --abridge 10). "
            "0 (the default) plays the file in full. Only affects --play."
        );

    parser.parse_args(argc, argv);

    Path path(parser.get("--file"));
    if (!path.string().empty()) {
        if (!stdx::fs::exists(path)) {
            System::err.println("File not found: {}", path);
            return 1;
        }
    } else {
        Optional<Path> resolved = resolve_default_sample();
        if (!resolved.has_value()) {
            System::err.println(
                "Could not locate the bundled sample. Pass --file <path> or "
                "run the test from the project root."
            );
            return 1;
        }
        path = *resolved;
    }

    TestContext ctx;
    describe(ctx, path);

    System::out.println(
        "\nResults: {} passed, {} failed", ctx.passed, ctx.failed
    );

    if (parser.get<bool>("--play")) {
        System::out.println("");
        play(path, parser.get<i64>("--abridge"));
    }

    return ctx.failed == 0 ? 0 : 1;
}
