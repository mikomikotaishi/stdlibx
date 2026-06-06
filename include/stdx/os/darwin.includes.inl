#pragma once

// Apple's audio/MIDI framework headers are built on Clang Blocks (the `^`
// syntax) and Clang-only availability attributes, so they only parse when
// Blocks are enabled. Gate on __BLOCKS__ so GCC (and clang -fno-blocks) skip
// them rather than failing to compile.
#if defined(__APPLE__) && defined(__BLOCKS__)
#include <TargetConditionals.h>

// CoreFoundation underpins all Apple frameworks
#include <CoreFoundation/CoreFoundation.h>

// CoreAudio - HAL + types
#include <CoreAudio/CoreAudio.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreAudio/HostTime.h>

#if TARGET_OS_OSX
// HAL - macOS only; iOS/iPadOS/watchOS/tvOS route through AVAudioSession
#include <CoreAudio/AudioHardware.h>
#endif

// AudioUnit - render/effect plugin API
#include <AudioUnit/AudioUnit.h>
#include <AudioUnit/AUComponent.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioOutputUnit.h>
#include <AudioUnit/AUAudioUnit.h>

// AudioToolbox - higher-level audio (file I/O, queues, format conversion)
#include <AudioToolbox/AudioToolbox.h>

// CoreMIDI
#include <CoreMIDI/CoreMIDI.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreMIDI/MIDISetup.h>
#include <CoreMIDI/MIDIThruConnection.h>
#endif
