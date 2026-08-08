#pragma once

// Darwin's own C headers, as opposed to the Objective-C frameworks below. These
// need no Blocks support, so they are gated on the platform alone: a GCC or
// -fno-blocks build still wants kqueue.
//
// Every wrapper in stdx/os/darwin/ is a bare `using ::name;` over one of these,
// so a header missing here leaves the wrapper declaring names that do not exist
// - not an inert wrapper, a broken one. The list is unguarded for the same
// reason linux.includes.inl is: these all ship in the macOS SDK, and an
// __has_include per entry would only move a hard error to a silently empty
// namespace.
#if defined(__APPLE__)
#include <mach/clock.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_port.h>
#include <mach/mach_time.h>
#include <mach/mach_types.h>
#include <mach/mach_vm.h>
#include <mach/message.h>
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/task_info.h>
#include <mach/thread_act.h>
#include <mach/thread_info.h>
#include <mach/thread_policy.h>
#include <mach/vm_map.h>
#include <mach-o/dyld.h>
#include <mach-o/fat.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <malloc/malloc.h>
#include <net/bpf.h>
#include <net/ethernet.h>
#include <net/if_dl.h>
#include <net/if_media.h>
#include <net/ndrv.h>
#include <net/route.h>
#include <netinet/icmp6.h>
#include <netinet/if_ether.h>
#include <netinet/igmp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <os/lock.h>
#include <os/log.h>
#include <os/proc.h>
#include <os/signpost.h>
#include <pthread/qos.h>
#include <sys/attr.h>
#include <sys/clonefile.h>
#include <sys/event.h>
#include <sys/fsgetpath.h>
#include <sys/kern_control.h>
#include <sys/kern_event.h>
#include <sys/proc_info.h>
#include <sys/qos.h>
#include <sys/sysctl.h>
#include <sys/xattr.h>

#include <NSSystemDirectories.h>
#include <copyfile.h>
#include <crt_externs.h>
#include <gethostuuid.h>
#include <libproc.h>
#include <removefile.h>
#include <util.h>
#endif

// Apple's audio/MIDI framework headers are built on Clang Blocks (the `^`
// syntax) and Clang-only availability attributes, so they only parse when
// Blocks are enabled. Gate on __BLOCKS__ so GCC (and clang -fno-blocks) skip
// them rather than failing to compile.
#if defined(__APPLE__) && defined(__BLOCKS__)
#include <TargetConditionals.h>

// The Blocks runtime itself, and libdispatch, whose wrappers carry the same
// __BLOCKS__ guard: both are about handing blocks to the system, so neither has
// anything left to offer a build without them.
#include <Block.h>
#include <dispatch/dispatch.h>

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
