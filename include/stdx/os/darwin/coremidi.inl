#pragma once

/**
 * @namespace stdx::os::darwin::coremidi
 * @brief CoreMIDI - <CoreMIDI/CoreMIDI.h>, MIDIServices.h, MIDISetup.h,
 *        MIDIThruConnection.h.
 */
export namespace stdx::os::darwin::coremidi {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<CoreMIDI/CoreMIDI.h>)
    // Object reference handles (all UInt32-backed opaque refs)
    using MidiObjectRef = ::MIDIObjectRef;
    using MidiClientRef = ::MIDIClientRef;
    using MidiPortRef = ::MIDIPortRef;
    using MidiDeviceRef = ::MIDIDeviceRef;
    using MidiEntityRef = ::MIDIEntityRef;
    using MidiEndpointRef = ::MIDIEndpointRef;
    using MidiExternalDeviceRef = ::MIDIDeviceRef;
    using MidiObjectType = ::MIDIObjectType;
    using MidiTimeStamp = ::MIDITimeStamp;
    using MidiUniqueId = ::MIDIUniqueID;

    // Packet / message types (MIDI 1.0)
    using MidiPacket = ::MIDIPacket;
    using MidiPacketList = ::MIDIPacketList;
    using MidiSysexSendRequest = ::MIDISysexSendRequest;
    using MidiNotificationMessage = ::MIDINotification;
    using MidiNotificationType = ::MIDINotificationMessageID;

    // MIDI 2.0 / Universal MIDI Packet types (macOS 11+)
    #ifdef __MAC_11_0
    using MidiEventList = ::MIDIEventList;
    using MidiEventPacket = ::MIDIEventPacket;
    using MidiProtocol = ::MIDIProtocolID;
    using MidiReceiveBlock = ::MIDIReceiveBlock;
    #endif

    // Callbacks
    using MidiNotifyProc = ::MIDINotifyProc;
    using MidiReadProc = ::MIDIReadProc;
    using MidiReadBlock = ::MIDIReadBlock;
    using MidiCompletionProc = ::MIDICompletionProc;

    // Thru connection
    using MidiThruConnectionRef = ::MIDIThruConnectionRef;
    using MidiThruConnectionParams = ::MIDIThruConnectionParams;
    using MidiThruConnectionEndpoint = ::MIDIThruConnectionEndpoint;

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
}
