#pragma once

/**
 * @namespace stdx::os::darwin::coremidi
 * @brief CoreMIDI — <CoreMIDI/CoreMIDI.h>, MIDIServices.h, MIDISetup.h,
 *        MIDIThruConnection.h.
 */
export namespace stdx::os::darwin::coremidi {
    #ifdef __APPLE__
    #if __has_include(<CoreMIDI/CoreMIDI.h>)
    // Object reference handles (all UInt32-backed opaque refs)
    using ObjectRef = ::MIDIObjectRef;
    using ClientRef = ::MIDIClientRef;
    using PortRef = ::MIDIPortRef;
    using DeviceRef = ::MIDIDeviceRef;
    using EntityRef = ::MIDIEntityRef;
    using EndpointRef = ::MIDIEndpointRef;
    using ExternalDeviceRef = ::MIDIDeviceRef;
    using ObjectType = ::MIDIObjectType;
    using TimeStamp = ::MIDITimeStamp;
    using UniqueId = ::MIDIUniqueID;

    // Packet / message types (MIDI 1.0)
    using Packet = ::MIDIPacket;
    using PacketList = ::MIDIPacketList;
    using SysexSendRequest = ::MIDISysexSendRequest;
    using NotificationMessage = ::MIDINotification;
    using NotificationType = ::MIDINotificationMessageID;

    // MIDI 2.0 / Universal MIDI Packet types (macOS 11+)
    #if defined(__MAC_11_0)
    using EventList = ::MIDIEventList;
    using EventPacket = ::MIDIEventPacket;
    using Protocol = ::MIDIProtocolID;
    using ReceiveBlock = ::MIDIReceiveBlock;
    #endif

    // Callbacks
    using NotifyProc = ::MIDINotifyProc;
    using ReadProc = ::MIDIReadProc;
    using ReadBlock = ::MIDIReadBlock;
    using CompletionProc = ::MIDICompletionProc;

    // Thru connection
    using ThruConnectionRef = ::MIDIThruConnectionRef;
    using ThruConnectionParams = ::MIDIThruConnectionParams;
    using ThruConnectionEndpoint = ::MIDIThruConnectionEndpoint;

    // Client / port lifecycle
    using ::MIDIClientCreate;
    using ::MIDIClientCreateWithBlock;
    using ::MIDIClientDispose;
    using ::MIDIInputPortCreate;
    using ::MIDIInputPortCreateWithBlock;
    using ::MIDIOutputPortCreate;
    using ::MIDIPortDispose;
    using ::MIDIPortConnectSource;
    using ::MIDIPortDisconnectSource;

    // Endpoint creation (virtual)
    using ::MIDISourceCreate;
    using ::MIDIDestinationCreate;
    using ::MIDIDestinationCreateWithBlock;
    using ::MIDIEndpointDispose;

    // Sending
    using ::MIDISend;
    using ::MIDISendSysex;
    using ::MIDIReceived;
    using ::MIDIFlushOutput;
    using ::MIDIRestart;

    // Packet list helpers
    using ::MIDIPacketListInit;
    using ::MIDIPacketListAdd;
    using ::MIDIPacketNext;

    // Device / entity / endpoint enumeration
    using ::MIDIGetNumberOfDevices;
    using ::MIDIGetDevice;
    using ::MIDIGetNumberOfExternalDevices;
    using ::MIDIGetExternalDevice;
    using ::MIDIDeviceGetNumberOfEntities;
    using ::MIDIDeviceGetEntity;
    using ::MIDIEntityGetDevice;
    using ::MIDIEntityGetNumberOfSources;
    using ::MIDIEntityGetSource;
    using ::MIDIEntityGetNumberOfDestinations;
    using ::MIDIEntityGetDestination;
    using ::MIDIGetNumberOfSources;
    using ::MIDIGetSource;
    using ::MIDIGetNumberOfDestinations;
    using ::MIDIGetDestination;

    // Property access
    using ::MIDIObjectGetIntegerProperty;
    using ::MIDIObjectSetIntegerProperty;
    using ::MIDIObjectGetStringProperty;
    using ::MIDIObjectSetStringProperty;
    using ::MIDIObjectGetDataProperty;
    using ::MIDIObjectSetDataProperty;
    using ::MIDIObjectGetDictionaryProperty;
    using ::MIDIObjectSetDictionaryProperty;
    using ::MIDIObjectGetProperties;
    using ::MIDIObjectRemoveProperty;
    using ::MIDIObjectFindByUniqueID;

    // Thru connections
    using ::MIDIThruConnectionCreate;
    using ::MIDIThruConnectionDispose;
    using ::MIDIThruConnectionGetParams;
    using ::MIDIThruConnectionSetParams;
    using ::MIDIThruConnectionFind;
    #endif
    #endif
}
