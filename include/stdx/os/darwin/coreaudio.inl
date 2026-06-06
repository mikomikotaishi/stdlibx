#pragma once

/**
 * @namespace stdx::os::darwin::coreaudio
 * @brief CoreAudio HAL - <CoreAudio/CoreAudio.h>, <CoreAudio/CoreAudioTypes.h>,
 *        <CoreAudio/AudioHardware.h>, <CoreAudio/HostTime.h>.
 *
 * Low-level access to audio devices, streams, and the property-based
 * Hardware Abstraction Layer.
 */
export namespace stdx::os::darwin::coreaudio {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<CoreAudio/CoreAudio.h>)
    // Core scalars / IDs
    using AudioObjectId = ::AudioObjectID;
    using AudioClassId = ::AudioClassID;
    using AudioDeviceId = ::AudioDeviceID;
    using AudioStreamId = ::AudioStreamID;
    using AudioObjectPropertySelector = ::AudioObjectPropertySelector;
    using AudioObjectPropertyScope = ::AudioObjectPropertyScope;
    using AudioObjectPropertyElement = ::AudioObjectPropertyElement;
    using AudioObjectPropertyAddress = ::AudioObjectPropertyAddress;

    // Time
    using AudioTimeStamp = ::AudioTimeStamp;
    using AudioTimeStampFlags = ::AudioTimeStampFlags;
    using SmpteTime = ::SMPTETime;

    // Stream description (format)
    using AudioStreamBasicDescription = ::AudioStreamBasicDescription;
    using AudioStreamRangedDescription = ::AudioStreamRangedDescription;
    using AudioStreamPacketDescription = ::AudioStreamPacketDescription;
    using AudioChannelLayout = ::AudioChannelLayout;
    using AudioChannelDescription = ::AudioChannelDescription;
    using AudioChannelLabel = ::AudioChannelLabel;
    using AudioChannelBitmap = ::AudioChannelBitmap;
    using AudioChannelLayoutTag = ::AudioChannelLayoutTag;

    // Buffers
    using AudioBuffer = ::AudioBuffer;
    using AudioBufferList = ::AudioBufferList;

    // Value types
    using AudioValueRange = ::AudioValueRange;
    using AudioValueTranslation = ::AudioValueTranslation;
    using AudioFormatID = ::AudioFormatID;
    using AudioFormatFlags = ::AudioFormatFlags;

    // Listeners / callbacks
    using AudioObjectPropertyListenerProc = ::AudioObjectPropertyListenerProc;
    using AudioDeviceIOProc = ::AudioDeviceIOProc;
    using AudioDeviceIOProcId = ::AudioDeviceIOProcID;

    // HAL - property access
    using ::AudioObjectShow;
    using ::AudioObjectHasProperty;
    using ::AudioObjectIsPropertySettable;
    using ::AudioObjectGetPropertyDataSize;
    using ::AudioObjectGetPropertyData;
    using ::AudioObjectSetPropertyData;
    using ::AudioObjectAddPropertyListener;
    using ::AudioObjectRemovePropertyListener;
    using ::AudioObjectAddPropertyListenerBlock;
    using ::AudioObjectRemovePropertyListenerBlock;

    // HAL - devices and streams
    using ::AudioDeviceCreateIOProcID;
    using ::AudioDeviceCreateIOProcIDWithBlock;
    using ::AudioDeviceDestroyIOProcID;
    using ::AudioDeviceStart;
    using ::AudioDeviceStartAtTime;
    using ::AudioDeviceStop;
    using ::AudioDeviceGetCurrentTime;
    using ::AudioDeviceTranslateTime;
    using ::AudioDeviceGetNearestStartTime;

    // Host time / clock
    using ::AudioGetCurrentHostTime;
    using ::AudioGetHostClockFrequency;
    using ::AudioGetHostClockMinimumTimeDelta;
    using ::AudioConvertHostTimeToNanos;
    using ::AudioConvertNanosToHostTime;
    #endif
}
