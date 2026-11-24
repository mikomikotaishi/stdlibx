#include <version>

import stdx;

#if defined(__linux__)
using stdx::collections::Vector;
using stdx::mem::UniquePointer;
using stdx::sync::LockGuard;
using stdx::sync::Mutex;
using stdx::thread::Thread;

using namespace stdx::test;
using namespace stdx::audio::midi;

namespace {
    /**
     * @brief What arrived at a receiver, captured on the device's input
     * thread (the message itself is a temporary there).
     */
    struct Seen {
        String kind;
        u8 group = 0;
        Vector<u8> bytes;
    };

    /**
     * @brief Thread-safe Receiver that records each message it is given.
     */
    class Recorder final: public Receiver {
    public:
        Mutex mutex;
        Vector<Seen> seen;

        void send(const MidiMessage& msg, [[maybe_unused]] u64 time_micros) override {
            Seen s;
            const Span<const u8> raw = msg.data();
            s.bytes.assign(raw.begin(), raw.end());
            if (const Sysex7Message* sysex = dynamic_cast<const Sysex7Message*>(&msg)) {
                s.kind = "sysex7";
                s.group = sysex->group();
                s.bytes = sysex->payload();
            } else if (const Midi2ChannelVoiceMessage* m2 = dynamic_cast<const Midi2ChannelVoiceMessage*>(&msg)) {
                s.kind = "midi2";
                s.group = m2->group();
            } else if (const Midi1ChannelVoiceMessage* m1 = dynamic_cast<const Midi1ChannelVoiceMessage*>(&msg)) {
                s.kind = "midi1-ump";
                s.group = m1->group();
            } else if (dynamic_cast<const UmpMessage*>(&msg) != nullptr) {
                s.kind = "ump";
            } else {
                s.kind = "bytes"; // a MIDI 1.0 byte message, from a legacy client
            }
            LockGuard<Mutex> guard{mutex};
            seen.push_back(Ops::move(s));
        }

        void close() noexcept override {}

        /// Wait up to two seconds for at least @p count messages.
        [[nodiscard]]
        bool wait_for(usize count) {
            for (int i = 0; i < 1000; ++i) {
                {
                    LockGuard<Mutex> guard{mutex};
                    if (seen.size() >= count) {
                        return true;
                    }
                }
                Thread::sleep_for(2ms);
            }
            return false;
        }

        [[nodiscard]]
        Seen at(usize i) {
            LockGuard<Mutex> guard{mutex};
            return seen.at(i);
        }
    };

    /**
     * @brief Create a virtual endpoint, or skip the test where the sequencer
     * is unavailable (no /dev/snd/seq, or no UMP support in the kernel).
     */
    [[nodiscard]]
    UniquePointer<MidiDevice> endpoint(const char* name, u8 groups = 1, UmpProtocol protocol = UmpProtocol::MIDI2) {
        try {
            return MidiSystem::create_virtual_endpoint({.name = name, .groups = groups, .protocol = protocol});
        } catch (...) {
            skip("no ALSA sequencer with UMP support here");
        }
    }

    /// The device info for port @p port of the client behind @p ep.
    [[nodiscard]]
    MidiDeviceInfo port_of(const MidiDevice& ep, i32 port, MidiBackend backend = MidiBackend::ALSA_SEQ_UMP) {
        const String& id = ep.info().id;
        return MidiDeviceInfo {
            .id = Ops::fmt("{}:{}", id.substr(0, id.find(':')), port),
            .name = "stdx test peer",
            .backend = backend,
            .is_input = true,
            .is_output = true,
        };
    }
}

void test_endpoint_is_listed() {
    UniquePointer<MidiDevice> ep = endpoint("stdx-test-listed");
    expect(ep->is_open(), "the endpoint comes back open");
    expect(ep->info().backend == MidiBackend::ALSA_SEQ_UMP, "as an ALSA sequencer UMP device");

    bool found = false;
    for (const MidiDeviceInfo& info: MidiSystem::devices()) {
        if (info.id == ep->info().id) {
            found = true;
            expect(info.backend == MidiBackend::ALSA_SEQ_UMP, "listed under the UMP backend");
            expect(info.name.find("stdx-test-listed") != String::npos, "under its name");
        }
    }
    expect(found, "MidiSystem::devices() lists the endpoint port");
}

void test_ump_round_trip() {
    UniquePointer<MidiDevice> a = endpoint("stdx-test-a");
    UniquePointer<MidiDevice> b = MidiSystem::open_device(port_of(*a, 0));
    Recorder at_a;
    Recorder at_b;
    a->transmitter()->set_receiver(&at_a);
    b->transmitter()->set_receiver(&at_b);

    b->receiver()->send(Midi2ChannelVoiceMessage::note_on(0, 3, 60, 0x8000), 0);
    require(at_a.wait_for(1), "a UMP note sent to the endpoint arrives");
    expect_eq(at_a.at(0).kind, "midi2"s, "as a MIDI 2.0 channel-voice message");

    a->receiver()->send(Midi2ChannelVoiceMessage::control_change(0, 1, 74, 0x40000000u), 0);
    require(at_b.wait_for(1), "the endpoint's own output reaches the connected device");
    expect_eq(at_b.at(0).kind, "midi2"s, "as a MIDI 2.0 channel-voice message too");

    a->transmitter()->set_receiver(nullptr);
    b->transmitter()->set_receiver(nullptr);
}

void test_midi1_and_sysex_through_ump() {
    UniquePointer<MidiDevice> a = endpoint("stdx-test-encode");
    UniquePointer<MidiDevice> b = MidiSystem::open_device(port_of(*a, 0));
    Recorder at_a;
    a->transmitter()->set_receiver(&at_a);

    b->receiver()->send(ShortMessage{MidiStatus::NOTE_ON, 2, 64, 127}, 0);
    Vector<u8> body;
    for (u8 i = 0; i < 20; ++i) {
        body.push_back(i);
    }
    b->receiver()->send(Sysex7Message::from_bytes(0, Span<const u8>{body.data(), body.size()}), 0);

    require(at_a.wait_for(2), "both messages arrive");
    expect_eq(at_a.at(0).kind, "midi2"s, "a MIDI 1.0 note is encoded for the MIDI 2.0 endpoint");
    const Seen sysex = at_a.at(1);
    expect_eq(sysex.kind, "sysex7"s, "a four-packet SysEx7 arrives as one message");
    expect_eq(sysex.bytes.size(), 20uz, "with its whole body");

    a->transmitter()->set_receiver(nullptr);
}

void test_legacy_client_interop() {
    UniquePointer<MidiDevice> a = endpoint("stdx-test-legacy");
    // The MIDI 1.0 sequencer backend, connected to the same endpoint port.
    UniquePointer<MidiDevice> legacy = MidiSystem::open_device(port_of(*a, 0, MidiBackend::ALSA_SEQ));
    Recorder at_a;
    Recorder at_legacy;
    a->transmitter()->set_receiver(&at_a);
    legacy->transmitter()->set_receiver(&at_legacy);

    legacy->receiver()->send(ShortMessage{MidiStatus::NOTE_ON, 0, 60, 100}, 0);
    require(at_a.wait_for(1), "a legacy client's note reaches the UMP endpoint");
    expect_eq(at_a.at(0).kind, "midi2"s, "converted by the sequencer to MIDI 2.0 UMP");

    a->receiver()->send(Midi2ChannelVoiceMessage::note_on(0, 5, 62, 0xFFFF), 0);
    require(at_legacy.wait_for(1), "the endpoint's UMP note reaches the legacy client");
    const Seen back = at_legacy.at(0);
    expect_eq(back.kind, "bytes"s, "as a MIDI 1.0 byte message");
    require(back.bytes.size() == 3, "a three-byte note on");
    expect(back.bytes[0] == 0x95 && back.bytes[1] == 62, "on channel 5, note 62");

    a->transmitter()->set_receiver(nullptr);
    legacy->transmitter()->set_receiver(nullptr);
}

void test_group_port_rewrites_group() {
    UniquePointer<MidiDevice> a = endpoint("stdx-test-groups", 2);
    // Port 2 is group 2 (group index 1).
    UniquePointer<MidiDevice> b = MidiSystem::open_device(port_of(*a, 2));
    Recorder at_a;
    a->transmitter()->set_receiver(&at_a);

    b->receiver()->send(Midi2ChannelVoiceMessage::note_on(0, 0, 60, 0x8000), 0);
    require(at_a.wait_for(1), "a note sent through a group port arrives");
    expect_eq(at_a.at(0).group, u8{1}, "with its group rewritten to the port's group");

    a->transmitter()->set_receiver(nullptr);
}
#endif

int main(int argc, char* argv[]) {
    #if defined(__linux__)
    return run(argc, argv, {
        {"MidiSeqUmp.endpoint_is_listed", test_endpoint_is_listed},
        {"MidiSeqUmp.ump_round_trip", test_ump_round_trip},
        {"MidiSeqUmp.midi1_and_sysex_through_ump", test_midi1_and_sysex_through_ump},
        {"MidiSeqUmp.legacy_client_interop", test_legacy_client_interop},
        {"MidiSeqUmp.group_port_rewrites_group", test_group_port_rewrites_group},
    });
    #else
    System::out.println("[test] Test disabled (needs Linux + ALSA).");
    return System::EXIT_SUCCESS;
    #endif
}
