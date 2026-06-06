#pragma once

/**
 * @namespace stdx::os::darwin::audiotoolbox
 * @brief AudioToolbox - <AudioToolbox/AudioToolbox.h>.
 *        Higher-level audio: file I/O, format conversion, audio queues,
 *        extended file I/O, MusicSequence.
 */
export namespace stdx::os::darwin::audiotoolbox {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<AudioToolbox/AudioToolbox.h>)
    // AudioFile (decode/encode at the packet level)
    using AudioFileId = ::AudioFileID;
    using AudioFileTypeId = ::AudioFileTypeID;
    using AudioFilePropertyId = ::AudioFilePropertyID;
    using AudioFilePermissions = ::AudioFilePermissions;
    using AudioFilePacketTableInfo = ::AudioFilePacketTableInfo;

    // AudioConverter (PCM/compressed transcoding)
    using AudioConverterRef = ::AudioConverterRef;
    using AudioConverterPropertyId = ::AudioConverterPropertyID;
    using AudioConverterInputDataProc = ::AudioConverterInputDataProc;
    using AudioConverterComplexInputDataProc = ::AudioConverterComplexInputDataProc;

    // AudioQueue (callback-driven device I/O)
    using AudioQueueRef = ::AudioQueueRef;
    using AudioQueueBuffer = ::AudioQueueBuffer;
    using AudioQueueBufferRef = ::AudioQueueBufferRef;
    using AudioQueueOutputCallback = ::AudioQueueOutputCallback;
    using AudioQueueInputCallback = ::AudioQueueInputCallback;
    using AudioQueuePropertyId = ::AudioQueuePropertyID;
    using AudioQueueTimelineRef = ::AudioQueueTimelineRef;
    using AudioQueueLevelMeterState = ::AudioQueueLevelMeterState;

    // ExtAudioFile (file I/O with auto-format conversion)
    using AudioExtFileRef = ::ExtAudioFileRef;
    using AudioExtFilePropertyId = ::ExtAudioFilePropertyID;

    // MusicSequence / MusicPlayer (MIDI playback)
    using AudioMusicSequence = ::MusicSequence;
    using AudioMusicTrack = ::MusicTrack;
    using AudioMusicPlayer = ::MusicPlayer;
    using AudioMusicEventIterator = ::MusicEventIterator;
    using AudioMusicSequenceLoadFlags = ::MusicSequenceLoadFlags;
    using AudioMusicSequenceType = ::MusicSequenceType;
    using AudioMusicTimeStamp = ::MusicTimeStamp;

    // AudioFile lifecycle
    using ::AudioFileOpenURL;
    using ::AudioFileCreateWithURL;
    using ::AudioFileClose;
    using ::AudioFileReadBytes;
    using ::AudioFileWriteBytes;
    using ::AudioFileReadPacketData;
    using ::AudioFileWritePackets;
    using ::AudioFileGetPropertyInfo;
    using ::AudioFileGetProperty;
    using ::AudioFileSetProperty;

    // AudioConverter
    using ::AudioConverterNew;
    using ::AudioConverterNewSpecific;
    using ::AudioConverterDispose;
    using ::AudioConverterReset;
    using ::AudioConverterConvertBuffer;
    using ::AudioConverterFillComplexBuffer;
    using ::AudioConverterConvertComplexBuffer;
    using ::AudioConverterGetPropertyInfo;
    using ::AudioConverterGetProperty;
    using ::AudioConverterSetProperty;

    // AudioQueue
    using ::AudioQueueNewOutput;
    using ::AudioQueueNewInput;
    using ::AudioQueueDispose;
    using ::AudioQueueAllocateBuffer;
    using ::AudioQueueAllocateBufferWithPacketDescriptions;
    using ::AudioQueueFreeBuffer;
    using ::AudioQueueEnqueueBuffer;
    using ::AudioQueueEnqueueBufferWithParameters;
    using ::AudioQueueStart;
    using ::AudioQueuePause;
    using ::AudioQueueStop;
    using ::AudioQueueReset;
    using ::AudioQueueFlush;
    using ::AudioQueueGetParameter;
    using ::AudioQueueSetParameter;
    using ::AudioQueueGetPropertyInfo;
    using ::AudioQueueGetProperty;
    using ::AudioQueueSetProperty;
    using ::AudioQueueGetCurrentTime;

    // ExtAudioFile
    using ::ExtAudioFileOpenURL;
    using ::ExtAudioFileCreateWithURL;
    using ::ExtAudioFileDispose;
    using ::ExtAudioFileRead;
    using ::ExtAudioFileWrite;
    using ::ExtAudioFileSeek;
    using ::ExtAudioFileTell;
    using ::ExtAudioFileGetPropertyInfo;
    using ::ExtAudioFileGetProperty;
    using ::ExtAudioFileSetProperty;

    // MusicPlayer / sequence
    using ::NewMusicPlayer;
    using ::DisposeMusicPlayer;
    using ::MusicPlayerSetSequence;
    using ::MusicPlayerStart;
    using ::MusicPlayerStop;
    using ::NewMusicSequence;
    using ::DisposeMusicSequence;
    using ::MusicSequenceNewTrack;
    using ::MusicSequenceDisposeTrack;
    using ::MusicSequenceGetTrackCount;
    using ::MusicSequenceGetIndTrack;
    using ::MusicTrackNewMIDINoteEvent;
    using ::MusicTrackNewMIDIChannelEvent;
    using ::MusicTrackNewMIDIRawDataEvent;
    using ::MusicTrackNewExtendedTempoEvent;
    #endif
}
