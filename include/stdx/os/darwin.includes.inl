#pragma once

#ifdef __APPLE__
#include <TargetConditionals.h>

// CoreFoundation underpins all Apple frameworks
#include <CoreFoundation/CoreFoundation.h>

// CoreAudio — HAL + types
#include <CoreAudio/CoreAudio.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreAudio/HostTime.h>

#if TARGET_OS_OSX
// HAL — macOS only; iOS/iPadOS/watchOS/tvOS route through AVAudioSession
#include <CoreAudio/AudioHardware.h>
#endif

// AudioUnit — render/effect plugin API
#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AUComponent.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioOutputUnit.h>
#include <AudioUnit/AUAudioUnit.h>

// AudioToolbox — higher-level audio (file I/O, queues, format conversion)
#include <AudioToolbox/AudioToolbox.h>

// CoreMIDI
#include <CoreMIDI/CoreMIDI.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreMIDI/MIDISetup.h>
#include <CoreMIDI/MIDIThruConnection.h>
#endif
