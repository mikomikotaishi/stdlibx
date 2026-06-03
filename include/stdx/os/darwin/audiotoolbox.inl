#pragma once

/**
 * @namespace stdx::os::darwin::audiotoolbox
 * @brief AudioToolbox — <AudioToolbox/AudioToolbox.h>.
 *        Higher-level audio: file I/O, format conversion, audio queues,
 *        extended file I/O, MusicSequence.
 */
export namespace stdx::os::darwin::audiotoolbox {
    #ifdef __APPLE__
    #if __has_include(<AudioToolbox/AudioToolbox.h>)
    // AudioFile (decode/encode at the packet level)
    using FileId = ::AudioFileID;
    using FileTypeId = ::AudioFileTypeID;
    using FilePropertyId = ::AudioFilePropertyID;
    using FilePermissions = ::AudioFilePermissions;
    using FilePacketTableInfo = ::AudioFilePacketTableInfo;

    // AudioConverter (PCM/compressed transcoding)
    using ConverterRef = ::AudioConverterRef;
    using ConverterPropertyId = ::AudioConverterPropertyID;
    using ConverterInputDataProc = ::AudioConverterInputDataProc;
    using ConverterComplexInputDataProc = ::AudioConverterComplexInputDataProc;

    // AudioQueue (callback-driven device I/O)
    using QueueRef = ::AudioQueueRef;
    using QueueBuffer = ::AudioQueueBuffer;
    using QueueBufferRef = ::AudioQueueBufferRef;
    using QueueOutputCallback = ::AudioQueueOutputCallback;
    using QueueInputCallback = ::AudioQueueInputCallback;
    using QueuePropertyId = ::AudioQueuePropertyID;
    using QueueTimelineRef = ::AudioQueueTimelineRef;
    using QueueLevelMeterState = ::AudioQueueLevelMeterState;

    // ExtAudioFile (file I/O with auto-format conversion)
    using ExtFileRef = ::ExtAudioFileRef;
    using ExtFilePropertyId = ::ExtAudioFilePropertyID;

    // MusicSequence / MusicPlayer (MIDI playback)
    using MusicSequence = ::MusicSequence;
    using MusicTrack = ::MusicTrack;
    using MusicPlayer = ::MusicPlayer;
    using MusicEventIterator = ::MusicEventIterator;
    using MusicSequenceLoadFlags = ::MusicSequenceLoadFlags;
    using MusicSequenceType = ::MusicSequenceType;
    using MusicTimeStamp = ::MusicTimeStamp;

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
    #endif
}
