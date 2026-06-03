#pragma once

/**
 * @namespace stdx::os::darwin::coreaudio
 * @brief CoreAudio HAL — <CoreAudio/CoreAudio.h>, <CoreAudio/CoreAudioTypes.h>,
 *        <CoreAudio/AudioHardware.h>, <CoreAudio/HostTime.h>.
 *
 * Low-level access to audio devices, streams, and the property-based
 * Hardware Abstraction Layer.
 */
export namespace stdx::os::darwin::coreaudio {
    #ifdef __APPLE__
    #if __has_include(<CoreAudio/CoreAudio.h>)
    // Core scalars / IDs
    using ObjectId = ::AudioObjectID;
    using ClassId = ::AudioClassID;
    using DeviceId = ::AudioDeviceID;
    using StreamId = ::AudioStreamID;
    using ObjectPropertySelector = ::AudioObjectPropertySelector;
    using ObjectPropertyScope = ::AudioObjectPropertyScope;
    using ObjectPropertyElement = ::AudioObjectPropertyElement;
    using ObjectPropertyAddress = ::AudioObjectPropertyAddress;

    // Time
    using TimeStamp = ::AudioTimeStamp;
    using TimeStampFlags = ::AudioTimeStampFlags;
    using SmpteTime = ::SMPTETime;

    // Stream description (format)
    using StreamBasicDescription = ::AudioStreamBasicDescription;
    using StreamRangedDescription = ::AudioStreamRangedDescription;
    using StreamPacketDescription = ::AudioStreamPacketDescription;
    using ChannelLayout = ::AudioChannelLayout;
    using ChannelDescription = ::AudioChannelDescription;
    using ChannelLabel = ::AudioChannelLabel;
    using ChannelBitmap = ::AudioChannelBitmap;
    using ChannelLayoutTag = ::AudioChannelLayoutTag;

    // Buffers
    using Buffer = ::AudioBuffer;
    using BufferList = ::AudioBufferList;

    // Value types
    using ValueRange = ::AudioValueRange;
    using ValueTranslation = ::AudioValueTranslation;
    using FormatId = ::AudioFormatID;
    using FormatFlags = ::AudioFormatFlags;

    // Listeners / callbacks
    using ObjectPropertyListenerProc = ::AudioObjectPropertyListenerProc;
    using DeviceIoProc = ::AudioDeviceIOProc;
    using DeviceIoProcId = ::AudioDeviceIOProcID;

    // HAL — property access
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

    // HAL — devices and streams
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
    #endif
}
