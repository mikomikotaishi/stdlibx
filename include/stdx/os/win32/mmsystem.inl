#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Multimedia subsystem (mmsystem.h) wrappers — waveform audio, MIDI,
 *        mixer, multimedia timer, and MMIO.
 *
 * These are the legacy WinMM APIs (winmm.lib). Modern audio/MIDI work on
 * Win32 prefers WASAPI / WinRT MIDI; WinMM remains useful for simple I/O
 * and broad compatibility.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // Result + version
    using MmResult = ::MMRESULT;
    using MmVersion = ::MMVERSION;
    using MmTime = ::MMTIME;
    using MmTimePointer = ::LPMMTIME;
    using MmCallback = ::LPDRVCALLBACK;

    // Wave (PCM) audio
    using WaveOutHandle = ::HWAVEOUT;
    using WaveOutHandlePointer = ::LPHWAVEOUT;
    using WaveInHandle = ::HWAVEIN;
    using WaveInHandlePointer = ::LPHWAVEIN;
    using WaveHeader = ::WAVEHDR;
    using WaveHeaderPointer = ::LPWAVEHDR;
    using WaveFormat = ::WAVEFORMAT;
    using WaveFormatEx = ::WAVEFORMATEX;
    using WaveFormatExPointer = ::LPWAVEFORMATEX;
    using WaveOutCaps = ::WAVEOUTCAPSA;
    using WaveOutCapsW = ::WAVEOUTCAPSW;
    using WaveInCaps = ::WAVEINCAPSA;
    using WaveInCapsW = ::WAVEINCAPSW;
    using PcmWaveFormat = ::PCMWAVEFORMAT;

    // MIDI
    using MidiOutHandle = ::HMIDIOUT;
    using MidiOutHandlePointer = ::LPHMIDIOUT;
    using MidiInHandle = ::HMIDIIN;
    using MidiInHandlePointer = ::LPHMIDIIN;
    using MidiStreamHandle = ::HMIDISTRM;
    using MidiStreamHandlePointer = ::LPHMIDISTRM;
    using MidiHeader = ::MIDIHDR;
    using MidiHeaderPointer = ::LPMIDIHDR;
    using MidiEvent = ::MIDIEVENT;
    using MidiOutCaps = ::MIDIOUTCAPSA;
    using MidiOutCapsW = ::MIDIOUTCAPSW;
    using MidiInCaps = ::MIDIINCAPSA;
    using MidiInCapsW = ::MIDIINCAPSW;
    using MidiPropTimeDiv = ::MIDIPROPTIMEDIV;
    using MidiPropTempo = ::MIDIPROPTEMPO;

    // Mixer
    using MixerHandle = ::HMIXER;
    using MixerHandlePointer = ::LPHMIXER;
    using MixerCaps = ::MIXERCAPSA;
    using MixerCapsW = ::MIXERCAPSW;
    using MixerLine = ::MIXERLINEA;
    using MixerLineW = ::MIXERLINEW;
    using MixerLineControls = ::MIXERLINECONTROLSA;
    using MixerLineControlsW = ::MIXERLINECONTROLSW;
    using MixerControl = ::MIXERCONTROLA;
    using MixerControlW = ::MIXERCONTROLW;
    using MixerControlDetails = ::MIXERCONTROLDETAILS;

    // MMIO (multimedia file I/O)
    using MmioHandle = ::HMMIO;
    using MmioInfo = ::MMIOINFO;
    using MmioInfoPointer = ::LPMMIOINFO;
    using MmCkInfo = ::MMCKINFO;
    using MmCkInfoPointer = ::LPMMCKINFO;

    // Wave output
    using ::waveOutOpen;
    using ::waveOutClose;
    using ::waveOutPrepareHeader;
    using ::waveOutUnprepareHeader;
    using ::waveOutWrite;
    using ::waveOutPause;
    using ::waveOutRestart;
    using ::waveOutReset;
    using ::waveOutGetDevCapsA;
    using ::waveOutGetDevCapsW;
    using ::waveOutGetNumDevs;
    using ::waveOutGetVolume;
    using ::waveOutSetVolume;
    using ::waveOutGetPosition;
    using ::waveOutGetErrorTextA;
    using ::waveOutGetErrorTextW;

    // Wave input
    using ::waveInOpen;
    using ::waveInClose;
    using ::waveInPrepareHeader;
    using ::waveInUnprepareHeader;
    using ::waveInAddBuffer;
    using ::waveInStart;
    using ::waveInStop;
    using ::waveInReset;
    using ::waveInGetDevCapsA;
    using ::waveInGetDevCapsW;
    using ::waveInGetNumDevs;
    using ::waveInGetPosition;

    // MIDI output
    using ::midiOutOpen;
    using ::midiOutClose;
    using ::midiOutShortMsg;
    using ::midiOutLongMsg;
    using ::midiOutPrepareHeader;
    using ::midiOutUnprepareHeader;
    using ::midiOutReset;
    using ::midiOutGetDevCapsA;
    using ::midiOutGetDevCapsW;
    using ::midiOutGetNumDevs;
    using ::midiOutGetVolume;
    using ::midiOutSetVolume;

    // MIDI input
    using ::midiInOpen;
    using ::midiInClose;
    using ::midiInStart;
    using ::midiInStop;
    using ::midiInReset;
    using ::midiInAddBuffer;
    using ::midiInPrepareHeader;
    using ::midiInUnprepareHeader;
    using ::midiInGetDevCapsA;
    using ::midiInGetDevCapsW;
    using ::midiInGetNumDevs;

    // MIDI streaming
    using ::midiStreamOpen;
    using ::midiStreamClose;
    using ::midiStreamOut;
    using ::midiStreamPause;
    using ::midiStreamRestart;
    using ::midiStreamStop;
    using ::midiStreamProperty;
    using ::midiStreamPosition;

    // Mixer
    using ::mixerOpen;
    using ::mixerClose;
    using ::mixerGetNumDevs;
    using ::mixerGetDevCapsA;
    using ::mixerGetDevCapsW;
    using ::mixerGetLineInfoA;
    using ::mixerGetLineInfoW;
    using ::mixerGetLineControlsA;
    using ::mixerGetLineControlsW;
    using ::mixerGetControlDetailsA;
    using ::mixerGetControlDetailsW;
    using ::mixerSetControlDetails;

    // MMIO
    using ::mmioOpenA;
    using ::mmioOpenW;
    using ::mmioClose;
    using ::mmioRead;
    using ::mmioWrite;
    using ::mmioSeek;
    using ::mmioDescend;
    using ::mmioAscend;
    using ::mmioCreateChunk;

    // Timer
    using ::timeBeginPeriod;
    using ::timeEndPeriod;
    using ::timeGetTime;
    using ::timeGetDevCaps;
    using ::timeSetEvent;
    using ::timeKillEvent;
    #endif
}
