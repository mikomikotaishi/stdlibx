#pragma once

using stdx::sync::Atomic;
using stdx::sync::Mutex;
using stdx::sync::LockGuard;
using stdx::thread::Thread;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

using namespace stdx::os;

/**
 * @namespace stdx::audio::midi
 * @brief ALSA UMP (Universal MIDI Packet) backend - MIDI 2.0 endpoint I/O.
 *
 * Unlike the sequencer backend (alsa_seq.inl), which speaks byte-stream
 * MIDI 1.0 through snd_midi_event, this backend talks to a raw UMP endpoint
 * (snd_ump_open / snd_ump_write / snd_ump_read) and moves Universal MIDI
 * Packets directly. It is the transport for the MIDI 2.0 message classes in
 * ump.inl.
 *
 * The ALSA raw UMP API exchanges 32-bit UMP words in the host's native byte
 * order, which is exactly what UniversalMidiPacket stores, so no byte
 * swapping happens here: outgoing messages go through a UmpEncoder (which
 * also converts MIDI 1.0 messages), incoming words through a UmpParser.
 */
namespace stdx::audio::midi {
    /**
     * @brief Ask an open UMP endpoint which protocol it is currently using.
     *
     * Falls back to MIDI 1.0 if the endpoint cannot be queried: 32-bit MIDI
     * 1.0 channel-voice packets are the conservative choice, understood by any
     * endpoint that still speaks the MIDI 1.0 protocol.
     */
    [[nodiscard]]
    inline UmpProtocol query_ump_protocol(linux::alsa::SoundUmp* ump) noexcept {
        linux::alsa::SoundUmpEndpointInfo* info = nullptr;
        if (linux::alsa::snd_ump_endpoint_info_malloc(&info) < 0) {
            return UmpProtocol::MIDI1;
        }
        UmpProtocol protocol = UmpProtocol::MIDI1;
        if (
            linux::alsa::snd_ump_endpoint_info(ump, info) >= 0
            && (linux::alsa::snd_ump_endpoint_info_get_protocol(info) & linux::alsa::UMP_EP_INFO_PROTO_MIDI2) != 0
        ) {
            protocol = UmpProtocol::MIDI2;
        }
        linux::alsa::snd_ump_endpoint_info_free(info);
        return protocol;
    }

    /**
     * @brief The UMP group to put MIDI 1.0 messages on when sending to @p ump:
     * the first group of the first active function block that receives.
     *
     * Block direction is from the device's side, as in the UMP spec and as
     * ALSA reports it: SND_UMP_DIR_INPUT is a block that takes messages in
     * (the host writes to it), SND_UMP_DIR_OUTPUT one that sends them out.
     * Falls back to group 0 when the endpoint has no readable block info,
     * which is also where a single-group MIDI 1.0 device listens.
     */
    [[nodiscard]]
    inline u8 query_ump_send_group(linux::alsa::SoundUmp* ump) noexcept {
        linux::alsa::SoundUmpEndpointInfo* ep = nullptr;
        linux::alsa::SoundUmpBlockInfo* blk = nullptr;
        u8 group = 0;
        if (
            linux::alsa::snd_ump_endpoint_info_malloc(&ep) >= 0
            && linux::alsa::snd_ump_block_info_malloc(&blk) >= 0
            && linux::alsa::snd_ump_endpoint_info(ump, ep) >= 0
        ) {
            const u32 num_blocks = linux::alsa::snd_ump_endpoint_info_get_num_blocks(ep);
            for (u32 b = 0; b < num_blocks; ++b) {
                linux::alsa::snd_ump_block_info_set_block_id(blk, b);
                if (
                    linux::alsa::snd_ump_block_info(ump, blk) < 0
                    || linux::alsa::snd_ump_block_info_get_active(blk) == 0
                    || (linux::alsa::snd_ump_block_info_get_direction(blk) & linux::alsa::UMP_DIR_INPUT) == 0
                ) {
                    continue;
                }
                const u32 first = linux::alsa::snd_ump_block_info_get_first_group(blk);
                if (first < 16) {
                    group = static_cast<u8>(first);
                    break;
                }
            }
        }
        if (ep != nullptr) {
            linux::alsa::snd_ump_endpoint_info_free(ep);
        }
        if (blk != nullptr) {
            linux::alsa::snd_ump_block_info_free(blk);
        }
        return group;
    }

    /**
     * @class AlsaUmpReceiver
     * @brief Receiver that writes Universal MIDI Packets to an ALSA UMP output
     * endpoint.
     * @extends Receiver
     *
     * Accepts every kind of MidiMessage. UMP-family messages are written as
     * their own packets; MIDI 1.0 messages are converted by a UmpEncoder into
     * the endpoint's protocol, on the group its first receiving function
     * block starts at.
     */
    class AlsaUmpReceiver final: public Receiver {
    private:
        linux::alsa::SoundUmp* _out;
        UmpEncoder _encoder; ///< Stateful under MIDI 2.0; guarded by _send_mutex.
        Mutex _send_mutex;
    public:
        AlsaUmpReceiver(linux::alsa::SoundUmp* out, UmpProtocol protocol, u8 group):
            _out{out}, _encoder{protocol, group} {}

        THROWS(MidiException)
        void send(const MidiMessage& msg, [[maybe_unused]] u64 time_micros) override {
            if (_out == nullptr) {
                return;
            }
            LockGuard<Mutex> guard{_send_mutex};
            const Vector<UniversalMidiPacket> packets = _encoder.encode(msg);
            if (packets.empty()) {
                return;
            }
            Vector<u32> words;
            words.reserve(packets.size() * 4);
            for (const UniversalMidiPacket& packet: packets) {
                for (u8 i = 0; i < packet.word_count(); ++i) {
                    words.push_back(packet.word(i));
                }
            }
            const i64 written = linux::alsa::snd_ump_write(
                _out, words.data(), words.size() * sizeof(u32)
            );
            if (written < 0) {
                throw MidiException("snd_ump_write failed");
            }
        }

        void close() noexcept override {
            // The endpoint is owned by the AlsaUmpDevice.
        }
    };

    /**
     * @class AlsaUmpTransmitter
     * @brief Transmitter that reads Universal MIDI Packets from an ALSA UMP
     * input endpoint and forwards each as a typed message.
     * @extends Transmitter
     *
     * Words go through a UmpParser, so the receiver sees decoded message
     * classes, multi-packet SysEx arrives whole, and a packet split across two
     * reads is not lost.
     */
    class AlsaUmpTransmitter final: public Transmitter {
    private:
        linux::alsa::SoundUmp* _in;
        Atomic<Receiver*> _rx = nullptr;
        Atomic<bool> _running = false;
        UmpParser _parser; ///< Touched only by the worker thread.
        Thread _worker;

        void input_loop() noexcept {
            if (_in == nullptr) {
                return;
            }
            const i32 npfd = linux::alsa::snd_ump_poll_descriptors_count(_in);
            if (npfd <= 0) {
                return;
            }
            Vector<linux::sys::PollFileDescriptor> pfds(static_cast<usize>(npfd));
            linux::alsa::snd_ump_poll_descriptors(_in, pfds.data(), static_cast<u32>(npfd));

            // Read in whole-word units. A 128-bit packet is four words; a small
            // buffer keeps latency low while still batching bursts.
            u32 word_buf[64];
            while (_running.load()) {
                i32 r = linux::sys::poll(
                    pfds.data(), static_cast<linux::sys::FileDescriptorCount>(npfd), 100
                );
                if (r < 0) {
                    if (unix::errnov() == unix::EINTR) {
                        continue;
                    }
                    break;
                }
                if (r == 0) {
                    continue;
                }

                const i64 n = linux::alsa::snd_ump_read(_in, word_buf, sizeof(word_buf));
                if (n <= 0) {
                    continue;
                }
                const usize words_read = static_cast<usize>(n) / sizeof(u32);

                // Always feed the parser, receiver or not, so a receiver set
                // mid-stream starts on a packet boundary.
                try {
                    _parser.feed(
                        Span<const u32>{word_buf, words_read},
                        [this](const MidiMessage& msg) -> void {
                            if (Receiver* rx = _rx.load(); rx != nullptr) {
                                rx->send(msg, 0);
                            }
                        }
                    );
                } catch (...) {
                    // A throwing receiver must not kill the input thread; the
                    // parser is left mid-stream, so start clean.
                    _parser.reset();
                }
            }
        }
    public:
        explicit AlsaUmpTransmitter(linux::alsa::SoundUmp* in):
            _in{in} {
            if (_in != nullptr) {
                _running.store(true);
                _worker = Thread([this] -> void {
                    input_loop();
                });
            }
        }

        ~AlsaUmpTransmitter() override {
            close();
        }

        void set_receiver(Receiver* r) noexcept override {
            _rx.store(r);
        }

        [[nodiscard]]
        Receiver* receiver() const noexcept override {
            return _rx.load();
        }

        void close() noexcept override {
            if (!_running.exchange(false)) {
                return;
            }
            if (_worker.joinable()) {
                _worker.join();
            }
        }
    };

    /**
     * @class AlsaUmpDevice
     * @brief MidiDevice for an ALSA UMP endpoint. Opens the underlying
     * snd_ump_t on open() and tears it down on close().
     * @extends MidiDevice
     */
    class AlsaUmpDevice final: public MidiDevice {
        MidiDeviceInfo _dev_info;
        linux::alsa::SoundUmp* _in = nullptr;
        linux::alsa::SoundUmp* _out = nullptr;
        UniquePointer<AlsaUmpReceiver> _rx;
        UniquePointer<AlsaUmpTransmitter> _tx;
        bool _open_flag = false;
    public:
        explicit AlsaUmpDevice(MidiDeviceInfo info) noexcept:
            _dev_info{Ops::move(info)} {}

        ~AlsaUmpDevice() override {
            close();
        }

        [[nodiscard]]
        const MidiDeviceInfo& info() const noexcept override {
            return _dev_info;
        }

        THROWS(MidiException)
        void open() override {
            if (_open_flag) {
                return;
            }
            // id holds the ALSA UMP device name, e.g. "hw:1,0".
            linux::alsa::SoundUmp** inp = _dev_info.is_input ? &_in : nullptr;
            linux::alsa::SoundUmp** outp = _dev_info.is_output ? &_out : nullptr;
            if (inp == nullptr && outp == nullptr) {
                throw MidiException("UMP device is neither input nor output");
            }
            // Open blocking, then switch the input side to non-blocking so the
            // poll loop never stalls inside snd_ump_read. (SND_RAWMIDI_NONBLOCK
            // is a macro the FFI layer does not re-export, so we use the
            // explicit snd_ump_nonblock call instead of the open-mode flag.)
            const i32 rc = linux::alsa::snd_ump_open(inp, outp, _dev_info.id.c_str(), 0);
            if (rc < 0) {
                throw MidiException("snd_ump_open failed");
            }
            if (_in != nullptr) {
                linux::alsa::snd_ump_nonblock(_in, 1);
            }
            if (_out != nullptr) {
                // Encode for whatever protocol the endpoint is running now,
                // onto the group of the block that will receive it.
                _rx = Pointers::unique<AlsaUmpReceiver>(
                    _out, query_ump_protocol(_out), query_ump_send_group(_out)
                );
            }
            if (_in != nullptr) {
                _tx = Pointers::unique<AlsaUmpTransmitter>(_in);
            }
            _open_flag = true;
        }

        void close() noexcept override {
            if (!_open_flag) {
                return;
            }
            if (_rx) {
                _rx->close();
            }
            if (_tx) {
                _tx->close();
            }
            _rx.reset();
            _tx.reset();
            if (_in != nullptr) {
                linux::alsa::snd_ump_close(_in);
                _in = nullptr;
            }
            if (_out != nullptr) {
                linux::alsa::snd_ump_close(_out);
                _out = nullptr;
            }
            _open_flag = false;
        }

        [[nodiscard]]
        bool is_open() const noexcept override {
            return _open_flag;
        }

        [[nodiscard]]
        Receiver* receiver() override {
            return _rx.get();
        }

        [[nodiscard]]
        Transmitter* transmitter() override {
            return _tx.get();
        }
    };

    /**
     * @brief Enumerate the UMP (MIDI 2.0) endpoints exposed by every sound
     * card on the system.
     *
     * Walks each card's control interface for UMP devices, reading endpoint and
     * block info to decide direction. Each endpoint becomes one MidiDeviceInfo
     * with an id of the form "hw:<card>,<device>" that open_ump_endpoint can
     * open directly. Cards or devices that cannot be queried are skipped
     * silently so one bad card never hides the rest.
     */
    [[nodiscard]]
    inline Vector<MidiDeviceInfo> enumerate_ump_endpoints() {
        Vector<MidiDeviceInfo> out;

        linux::alsa::SoundUmpEndpointInfo* ep = nullptr;
        linux::alsa::SoundUmpBlockInfo* blk = nullptr;
        if (
            linux::alsa::snd_ump_endpoint_info_malloc(&ep) < 0
            || linux::alsa::snd_ump_block_info_malloc(&blk) < 0
        ) {
            if (ep != nullptr) linux::alsa::snd_ump_endpoint_info_free(ep);
            if (blk != nullptr) linux::alsa::snd_ump_block_info_free(blk);
            return out;
        }

        i32 card = -1;
        while (linux::alsa::snd_card_next(&card) >= 0 && card >= 0) {
            const String ctl_name = Ops::fmt("hw:{}", card);
            linux::alsa::SoundControl* ctl = nullptr;
            if (linux::alsa::snd_ctl_open(&ctl, ctl_name.c_str(), 0) < 0) {
                continue;
            }

            i32 device = -1;
            while (
                linux::alsa::snd_ctl_ump_next_device(ctl, &device) >= 0
                && device >= 0
            ) {
                linux::alsa::snd_ump_endpoint_info_set_device(ep, static_cast<u32>(device));
                if (linux::alsa::snd_ctl_ump_endpoint_info(ctl, ep) < 0) {
                    continue;
                }

                // Aggregate direction over the endpoint's function blocks.
                const u32 num_blocks = linux::alsa::snd_ump_endpoint_info_get_num_blocks(ep);
                bool any_input = false;
                bool any_output = false;
                for (u32 b = 0; b < num_blocks; ++b) {
                    linux::alsa::snd_ump_block_info_set_block_id(blk, b);
                    if (linux::alsa::snd_ctl_ump_block_info(ctl, blk) < 0) {
                        continue;
                    }
                    if (linux::alsa::snd_ump_block_info_get_active(blk) == 0) {
                        continue;
                    }
                    // Block direction is the device's: a block that sends
                    // (OUTPUT) is one the host reads, so it makes the endpoint
                    // an input here, and a block that receives (INPUT) makes
                    // it an output.
                    const u32 dir = linux::alsa::snd_ump_block_info_get_direction(blk);
                    any_input = any_input || (dir & linux::alsa::UMP_DIR_OUTPUT) != 0;
                    any_output = any_output || (dir & linux::alsa::UMP_DIR_INPUT) != 0;
                }
                // A UMP endpoint is at least bidirectional even if blocks were
                // unreadable; default to both so it stays openable.
                if (!any_input && !any_output) {
                    any_input = true;
                    any_output = true;
                }

                const char* ep_name = linux::alsa::snd_ump_endpoint_info_get_name(ep);
                const u32 proto = linux::alsa::snd_ump_endpoint_info_get_protocol_caps(ep);
                const bool midi2 = (proto & linux::alsa::UMP_EP_INFO_PROTO_MIDI2) != 0;

                out.push_back(Ops::move(MidiDeviceInfo {
                    .id = Ops::fmt("hw:{},{}", card, device),
                    .name = (ep_name != nullptr && ep_name[0] != '\0')
                        ? String(ep_name)
                        : Ops::fmt("UMP Endpoint hw:{},{}", card, device),
                    .description = midi2 ? "MIDI 2.0 capable"s : "MIDI 1.0 (UMP)"s,
                    .backend = MidiBackend::ALSA_UMP,
                    .is_input = any_input,
                    .is_output = any_output,
                }));
            }

            linux::alsa::snd_ctl_close(ctl);
        }

        linux::alsa::snd_ump_endpoint_info_free(ep);
        linux::alsa::snd_ump_block_info_free(blk);
        return out;
    }

    /**
     * @brief Open an ALSA UMP endpoint by device name.
     * @param name ALSA device string, e.g. "hw:1,0" (card, device).
     * @param input Whether to open the input (read) side.
     * @param output Whether to open the output (write) side.
     * @throws MidiException if the endpoint cannot be opened.
     */
    [[nodiscard]]
    THROWS(MidiException)
    inline UniquePointer<MidiDevice> open_ump_endpoint(const String& name, bool input, bool output) {
        MidiDeviceInfo info {
            .id = name,
            .name = name,
            .backend = MidiBackend::ALSA_UMP,
            .is_input = input,
            .is_output = output,
        };
        UniquePointer<AlsaUmpDevice> dev = Pointers::unique<AlsaUmpDevice>(Ops::move(info));
        dev->open();
        return dev;
    }
}
