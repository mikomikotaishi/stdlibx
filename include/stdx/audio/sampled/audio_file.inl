#pragma once

using stdx::fs::Path;
using stdx::io::OpenMode;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::sampled
 * @brief Audio-file open dispatcher - sniffs container magic and constructs
 * the right concrete AudioInputStream.
 */
namespace stdx::audio::sampled {
    [[nodiscard]]
    inline AudioFileType sniff_audio_file_type(const Path& path) noexcept {
        InputFileStream f{path, OpenMode::BINARY};
        if (!f) {
            return AudioFileType::UNKNOWN;
        }
        u8 magic[12]{};
        f.read(reinterpret_cast<char*>(magic), sizeof(magic));
        if (f.gcount() < 12) {
            return AudioFileType::UNKNOWN;
        }
        // RIFF .... WAVE
        if (tag_equals(magic, "RIFF") && tag_equals(magic + 8, "WAVE")) {
            return AudioFileType::WAV;
        }
        // FORM .... AIFF / AIFC - placeholder for future decoder
        if (tag_equals(magic, "FORM") &&
            (tag_equals(magic + 8, "AIFF") || tag_equals(magic + 8, "AIFC"))) {
            return AudioFileType::AIFF;
        }
        // "fLaC" stream marker
        if (tag_equals(magic, "fLaC")) {
            return AudioFileType::FLAC;
        }
        // OggS Ogg page header
        if (tag_equals(magic, "OggS")) {
            return AudioFileType::OGG;
        }
        // "ID3" v2 tag or MPEG sync (0xFF 0xFB / 0xFF 0xFA / 0xFF 0xF3 / 0xFF 0xF2)
        if (magic[0] == 'I' && magic[1] == 'D' && magic[2] == '3') {
            return AudioFileType::MP3;
        }
        if (magic[0] == 0xFF && (magic[1] & 0xE0) == 0xE0) {
            return AudioFileType::MP3;
        }
        return AudioFileType::UNKNOWN;
    }
}

export namespace stdx::audio::sampled {
    UniquePointer<AudioInputStream> AudioSystem::open_audio_file(
        const Path& path
    ) throws (UnsupportedAudioFileException) {
        const AudioFileType t = sniff_audio_file_type(path);
        switch (t) {
            case AudioFileType::WAV:
                return Pointers::unique<WavAudioInputStream>(path);
            case AudioFileType::AIFF:
                return Pointers::unique<AiffAudioInputStream>(path);
            case AudioFileType::FLAC:
                throw UnsupportedAudioFileException(
                    "FLAC decoder requires the libFLAC extension"
                );
            case AudioFileType::OGG:
                throw UnsupportedAudioFileException(
                    "Ogg/Vorbis decoder requires the libvorbis extension"
                );
            case AudioFileType::MP3:
                throw UnsupportedAudioFileException(
                    "MP3 decoder requires the libmpg123 extension"
                );
            case AudioFileType::UNKNOWN:
            default:
                throw UnsupportedAudioFileException(
                    "unrecognized audio file container"
                );
        }
        Ops::unreachable();
    }
}
