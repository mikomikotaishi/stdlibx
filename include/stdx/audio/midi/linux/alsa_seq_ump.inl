#pragma once

using stdx::sync::Atomic;
using stdx::sync::LockGuard;
using stdx::sync::Mutex;
using stdx::thread::Thread;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

using namespace stdx::os;

/**
 * @namespace stdx::audio::midi
 * @brief ALSA sequencer in UMP mode - MIDI 2.0 routing between applications.
 *
 * The sequencer backend in alsa_seq.inl is a legacy (MIDI 1.0 event) client.
 * This one opens sequencer clients in UMP mode instead, so Universal MIDI
 * Packets travel through the sequencer as they are, and adds virtual UMP
 * endpoints: a client of our own, announced as a MIDI 2.0 endpoint with one
 * function block, that other applications can connect to.
 *
 * Routing facts this relies on (from alsa-lib's seqmid.c and the kernel's
 * seq_clientmgr.c / seq_ump_convert.c):
 *
 * - snd_seq_create_ump_endpoint makes port 0 the endpoint port and ports
 *   1..N the group ports (port N carries group N-1). A group port only gets
 *   read/write capabilities if a function block covering it has a name, so
 *   blocks here are always named.
 * - An event sent from the endpoint port to its subscribers also reaches the
 *   subscribers of the group port matching the packet's group, so an
 *   endpoint sends everything from port 0.
 * - An event delivered to a group port has its group rewritten to that
 *   port's group.
 * - Legacy <-> UMP conversion is on by default, so legacy MIDI 1.0 clients
 *   still hear a UMP client and are heard by it.
 *
 * Each device owns its own sequencer client rather than sharing one, so one
 * device's input thread can never consume events meant for another.
 */
namespace stdx::audio::midi {
    /**
     * @class AlsaUmpSeqClient
     * @brief One sequencer client opened in UMP mode.
     *
     * alsa-lib sequencer handles are not thread-safe, and a device both sends
     * from the caller's thread and reads on its input thread, so every call
     * on the handle goes through one mutex.
     */
    class AlsaUmpSeqClient final {
    private:
        linux::alsa::SoundSequencer* _seq = nullptr;
        mutable Mutex _mutex;
    public:
        /**
         * @brief Open a duplex, non-blocking sequencer client named @p name.
         * @param protocol The protocol the client declares (MIDI 1.0 or 2.0 UMP).
         * @param declare_version Whether to set the client's MIDI version here.
         * snd_seq_create_ump_endpoint sets it itself, so an endpoint passes false.
         * @throws MidiException if the sequencer cannot be opened.
         */
        THROWS(MidiException)
        AlsaUmpSeqClient(const String& name, UmpProtocol protocol, bool declare_version = true) {
            if (
                linux::alsa::snd_seq_open(
                    &_seq, "default",
                    linux::alsa::SND_SEQ_OPEN_DUPLEX,
                    linux::alsa::SND_SEQ_NONBLOCK
                ) < 0
            ) {
                _seq = nullptr;
                throw MidiException("cannot open the ALSA sequencer");
            }
            linux::alsa::snd_seq_set_client_name(_seq, name.c_str());
            if (declare_version) {
                const i32 version = protocol == UmpProtocol::MIDI2
                    ? linux::alsa::SND_SEQ_CLIENT_UMP_MIDI_2_0
                    : linux::alsa::SND_SEQ_CLIENT_UMP_MIDI_1_0;
                if (linux::alsa::snd_seq_set_client_midi_version(_seq, version) < 0) {
                    linux::alsa::snd_seq_close(_seq);
                    _seq = nullptr;
                    throw MidiException("this ALSA sequencer has no UMP support");
                }
            }
        }

        ~AlsaUmpSeqClient() {
            if (_seq != nullptr) {
                linux::alsa::snd_seq_close(_seq);
            }
        }

        AlsaUmpSeqClient(const AlsaUmpSeqClient&) = DELETE_METHOD("AlsaUmpSeqClient is not copyable.");
        AlsaUmpSeqClient& operator=(const AlsaUmpSeqClient&) = DELETE_METHOD("AlsaUmpSeqClient is not copyable.");

        /// The client's sequencer id.
        [[nodiscard]]
        i32 id() const {
            LockGuard<Mutex> guard{_mutex};
            return linux::alsa::snd_seq_client_id(_seq);
        }

        /// Run @p body with the raw handle, holding the client's lock.
        template <typename F>
        decltype(auto) locked(F&& body) const {
            LockGuard<Mutex> guard{_mutex};
            return body(_seq);
        }

        /**
         * @brief Send one UMP packet from @p source_port.
         * @param dest_client The destination client, or SND_SEQ_ADDRESS_SUBSCRIBERS
         * to send to everything subscribed to @p source_port.
         * @param dest_port The destination port (ignored for subscribers).
         * @return Whether the sequencer accepted it.
         */
        bool send(u8 source_port, u8 dest_client, u8 dest_port, const UniversalMidiPacket& packet) const {
            linux::alsa::SoundSequencerUmpEvent ev;
            ev.type = 0; // unused for UMP events
            ev.flags = linux::alsa::SND_SEQ_EVENT_UMP;
            ev.tag = 0;
            ev.queue = linux::alsa::SND_SEQ_QUEUE_DIRECT;
            ev.time.tick = 0;
            ev.source.client = 0; // filled in by the kernel
            ev.source.port = source_port;
            ev.dest.client = dest_client;
            ev.dest.port = dest_client == linux::alsa::SND_SEQ_ADDRESS_SUBSCRIBERS
                ? linux::alsa::SND_SEQ_ADDRESS_UNKNOWN
                : dest_port;
            for (u8 i = 0; i < 4; ++i) {
                ev.ump[i] = packet.word(i); // unused words are zero
            }
            LockGuard<Mutex> guard{_mutex};
            return linux::alsa::snd_seq_ump_event_output_direct(_seq, &ev) >= 0;
        }

        /**
         * @internal
         * @brief One packet taken off the input queue.
         */
        struct Incoming {
            u8 dest_port; ///< Which of our ports it was delivered to.
            u32 words[4]; ///< The raw UMP words; how many count is set by the Message Type.
        };

        /**
         * @brief Take every pending UMP event off the input queue.
         *
         * Copies them out under the lock so the caller can dispatch without
         * holding it (a receiver may well send from inside its callback).
         * Non-UMP events (sequencer announcements, say) are skipped.
         */
        void drain(Vector<Incoming>& out) const {
            LockGuard<Mutex> guard{_mutex};
            while (true) {
                linux::alsa::SoundSequencerUmpEvent* ev = nullptr;
                if (linux::alsa::snd_seq_ump_event_input(_seq, &ev) < 0 || ev == nullptr) {
                    break;
                }
                if ((ev->flags & linux::alsa::SND_SEQ_EVENT_UMP) == 0) {
                    continue;
                }
                Incoming in{.dest_port = ev->dest.port, .words = {ev->ump[0], ev->ump[1], ev->ump[2], ev->ump[3]}};
                out.push_back(in);
            }
        }

        /// The descriptors to poll for input.
        [[nodiscard]]
        Vector<linux::sys::PollFileDescriptor> poll_descriptors() const {
            LockGuard<Mutex> guard{_mutex};
            const i32 count = linux::alsa::snd_seq_poll_descriptors_count(_seq, unix::POLLIN);
            Vector<linux::sys::PollFileDescriptor> fds(count > 0 ? static_cast<usize>(count) : 0uz);
            if (count > 0) {
                linux::alsa::snd_seq_poll_descriptors(_seq, fds.data(), static_cast<u32>(count), unix::POLLIN);
            }
            return fds;
        }
    };

    /**
     * @class AlsaSeqUmpReceiver
     * @brief Receiver that sends through a UMP-mode sequencer client.
     * @extends Receiver
     *
     * Accepts every kind of MidiMessage: UMP-family messages go as their own
     * packets, MIDI 1.0 messages are converted by a UmpEncoder.
     */
    class AlsaSeqUmpReceiver final: public Receiver {
    private:
        const AlsaUmpSeqClient* _client;
        u8 _port;
        u8 _dest_client;
        u8 _dest_port;
        UmpEncoder _encoder; ///< Stateful under MIDI 2.0; guarded by _encode_mutex.
        Mutex _encode_mutex;
    public:
        AlsaSeqUmpReceiver(
            const AlsaUmpSeqClient& client,
            u8 port,
            u8 dest_client,
            u8 dest_port,
            UmpProtocol protocol,
            u8 group
        ):
            _client{&client},
            _port{port},
            _dest_client{dest_client},
            _dest_port{dest_port},
            _encoder{protocol, group} {}

        THROWS(MidiException)
        void send(const MidiMessage& msg, [[maybe_unused]] u64 time_micros) override {
            Vector<UniversalMidiPacket> packets;
            {
                LockGuard<Mutex> guard{_encode_mutex};
                packets = _encoder.encode(msg);
            }
            for (const UniversalMidiPacket& packet: packets) {
                if (!_client->send(_port, _dest_client, _dest_port, packet)) {
                    throw MidiException("snd_seq_ump_event_output_direct failed");
                }
            }
        }

        void close() noexcept override {
            // The client and its ports are owned by the device.
        }
    };

    /**
     * @class AlsaSeqUmpTransmitter
     * @brief Transmitter that reads UMP events off a UMP-mode sequencer client
     * and forwards them, decoded, to its Receiver.
     * @extends Transmitter
     *
     * Each sequencer event carries exactly one packet, so the words go straight
     * through a UmpParser, which decodes the message class and reassembles
     * multi-packet SysEx.
     */
    class AlsaSeqUmpTransmitter final: public Transmitter {
    private:
        const AlsaUmpSeqClient* _client;
        i32 _accept_port; ///< Only events delivered to this port, or -1 for any.
        Atomic<Receiver*> _rx = nullptr;
        Atomic<bool> _running = false;
        UmpParser _parser; ///< Touched only by the worker thread.
        Thread _worker;

        void input_loop() noexcept {
            Vector<linux::sys::PollFileDescriptor> fds = _client->poll_descriptors();
            if (fds.empty()) {
                return;
            }
            Vector<AlsaUmpSeqClient::Incoming> pending;
            while (_running.load()) {
                const i32 r = linux::sys::poll(
                    fds.data(), static_cast<linux::sys::FileDescriptorCount>(fds.size()), 100
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
                pending.clear();
                _client->drain(pending);
                for (const AlsaUmpSeqClient::Incoming& in: pending) {
                    if (_accept_port >= 0 && in.dest_port != static_cast<u8>(_accept_port)) {
                        continue;
                    }
                    const u8 count = UniversalMidiPacket::words_for(
                        static_cast<MessageType>((in.words[0] >> 28) & 0x0F)
                    );
                    try {
                        _parser.feed(
                            Span<const u32>{in.words, count},
                            [this](const MidiMessage& msg) -> void {
                                if (Receiver* rx = _rx.load(); rx != nullptr) {
                                    rx->send(msg, 0);
                                }
                            }
                        );
                    } catch (...) {
                        // A throwing receiver must not kill the input thread.
                        _parser.reset();
                    }
                }
            }
        }
    public:
        AlsaSeqUmpTransmitter(const AlsaUmpSeqClient& client, i32 accept_port):
            _client{&client}, _accept_port{accept_port} {
            _running.store(true);
            _worker = Thread([this] -> void {
                input_loop();
            });
        }

        ~AlsaSeqUmpTransmitter() override {
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
     * @class AlsaSeqUmpDevice
     * @brief MidiDevice for another client's sequencer port, spoken to in UMP.
     * @extends MidiDevice
     *
     * Opens a UMP-mode client of its own with one port per direction,
     * subscribed to the target. Outgoing MIDI 1.0 messages are encoded for the
     * target's protocol (MIDI 2.0 if it is a MIDI 2.0 UMP client, else MIDI
     * 1.0) on the target port's group; the kernel converts for legacy targets.
     */
    class AlsaSeqUmpDevice final: public MidiDevice {
    private:
        MidiDeviceInfo _dev_info;
        i32 _target_client;
        i32 _target_port;
        UniquePointer<AlsaUmpSeqClient> _client;
        UniquePointer<AlsaSeqUmpReceiver> _rx;
        UniquePointer<AlsaSeqUmpTransmitter> _tx;
        bool _open_flag = false;

        /**
         * @internal
         * @brief The target's protocol and the group its port carries.
         */
        void query_target(UmpProtocol& protocol, u8& group) const {
            protocol = UmpProtocol::MIDI1;
            group = 0;
            _client->locked([&](linux::alsa::SoundSequencer* seq) -> void {
                linux::alsa::SoundSequencerClientInfo* cinfo = nullptr;
                linux::alsa::SoundSequencerPortInfo* pinfo = nullptr;
                linux::alsa::snd_seq_client_info_malloc(&cinfo);
                linux::alsa::snd_seq_port_info_malloc(&pinfo);
                if (cinfo != nullptr && linux::alsa::snd_seq_get_any_client_info(seq, _target_client, cinfo) >= 0) {
                    if (linux::alsa::snd_seq_client_info_get_midi_version(cinfo) == linux::alsa::SND_SEQ_CLIENT_UMP_MIDI_2_0) {
                        protocol = UmpProtocol::MIDI2;
                    }
                }
                if (pinfo != nullptr && linux::alsa::snd_seq_get_any_port_info(seq, _target_client, _target_port, pinfo) >= 0) {
                    // ump_group is 1-based; 0 means the endpoint port (all groups).
                    const i32 port_group = linux::alsa::snd_seq_port_info_get_ump_group(pinfo);
                    if (port_group >= 1 && port_group <= 16) {
                        group = static_cast<u8>(port_group - 1);
                    }
                }
                if (cinfo != nullptr) {
                    linux::alsa::snd_seq_client_info_free(cinfo);
                }
                if (pinfo != nullptr) {
                    linux::alsa::snd_seq_port_info_free(pinfo);
                }
            });
        }
    public:
        AlsaSeqUmpDevice(MidiDeviceInfo info, i32 target_client, i32 target_port) noexcept:
            _dev_info{Ops::move(info)},
            _target_client{target_client},
            _target_port{target_port} {}

        ~AlsaSeqUmpDevice() override {
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
            _client = Pointers::unique<AlsaUmpSeqClient>("stdx::audio (UMP)"s, UmpProtocol::MIDI2);
            UmpProtocol protocol;
            u8 group;
            query_target(protocol, group);

            const u32 type = linux::alsa::SND_SEQ_PORT_TYPE_MIDI_GENERIC
                | linux::alsa::SND_SEQ_PORT_TYPE_MIDI_UMP
                | linux::alsa::SND_SEQ_PORT_TYPE_APPLICATION;
            if (_dev_info.is_output) {
                const i32 port = _client->locked([&](linux::alsa::SoundSequencer* seq) -> i32 {
                    const i32 p = linux::alsa::snd_seq_create_simple_port(
                        seq, _dev_info.name.c_str(),
                        linux::alsa::SND_SEQ_PORT_CAP_READ | linux::alsa::SND_SEQ_PORT_CAP_SUBS_READ,
                        type
                    );
                    if (p >= 0 && linux::alsa::snd_seq_connect_to(seq, p, _target_client, _target_port) < 0) {
                        linux::alsa::snd_seq_delete_simple_port(seq, p);
                        return -1;
                    }
                    return p;
                });
                if (port < 0) {
                    _client.reset();
                    throw MidiException("cannot connect to the sequencer port");
                }
                _rx = Pointers::unique<AlsaSeqUmpReceiver>(
                    *_client, static_cast<u8>(port),
                    linux::alsa::SND_SEQ_ADDRESS_SUBSCRIBERS, u8{0},
                    protocol, group
                );
            }
            if (_dev_info.is_input) {
                const i32 port = _client->locked([&](linux::alsa::SoundSequencer* seq) -> i32 {
                    const i32 p = linux::alsa::snd_seq_create_simple_port(
                        seq, _dev_info.name.c_str(),
                        linux::alsa::SND_SEQ_PORT_CAP_WRITE | linux::alsa::SND_SEQ_PORT_CAP_SUBS_WRITE,
                        type
                    );
                    if (p >= 0 && linux::alsa::snd_seq_connect_from(seq, p, _target_client, _target_port) < 0) {
                        linux::alsa::snd_seq_delete_simple_port(seq, p);
                        return -1;
                    }
                    return p;
                });
                if (port < 0) {
                    _rx.reset();
                    _client.reset();
                    throw MidiException("cannot subscribe to the sequencer port");
                }
                _tx = Pointers::unique<AlsaSeqUmpTransmitter>(*_client, port);
            }
            _open_flag = true;
        }

        void close() noexcept override {
            if (!_open_flag) {
                return;
            }
            // Stop the input thread before the client it reads goes away.
            _tx.reset();
            _rx.reset();
            _client.reset(); // closing the client drops its ports and subscriptions
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
     * @class AlsaVirtualUmpEndpoint
     * @brief A MIDI 2.0 endpoint of our own, announced through the sequencer.
     * @extends MidiDevice
     *
     * A UMP-mode client set up with snd_seq_create_ump_endpoint and one
     * bidirectional, named function block spanning all its groups. Other
     * applications connect to its port 0 (the whole endpoint) or to a group
     * port. Its receiver sends to everything subscribed; its transmitter hands
     * on whatever is sent to any of its ports.
     */
    class AlsaVirtualUmpEndpoint final: public MidiDevice {
    private:
        MidiDeviceInfo _dev_info;
        VirtualEndpointOptions _options;
        UniquePointer<AlsaUmpSeqClient> _client;
        UniquePointer<AlsaSeqUmpReceiver> _rx;
        UniquePointer<AlsaSeqUmpTransmitter> _tx;
        bool _open_flag = false;

        /**
         * @internal
         * @brief Declare the client as a UMP endpoint with one function block.
         * @return Whether alsa-lib accepted both.
         */
        bool declare_endpoint() {
            return _client->locked([&](linux::alsa::SoundSequencer* seq) -> bool {
                linux::alsa::SoundUmpEndpointInfo* ep = nullptr;
                linux::alsa::SoundUmpBlockInfo* blk = nullptr;
                bool ok = linux::alsa::snd_ump_endpoint_info_malloc(&ep) >= 0
                    && linux::alsa::snd_ump_block_info_malloc(&blk) >= 0;
                if (ok) {
                    const u32 groups = _options.groups;
                    linux::alsa::snd_ump_endpoint_info_set_name(ep, _options.name.c_str());
                    linux::alsa::snd_ump_endpoint_info_set_protocol_caps(
                        ep, linux::alsa::SND_UMP_EP_INFO_PROTO_MIDI1 | linux::alsa::SND_UMP_EP_INFO_PROTO_MIDI2
                    );
                    linux::alsa::snd_ump_endpoint_info_set_protocol(
                        ep,
                        _options.protocol == UmpProtocol::MIDI2
                            ? linux::alsa::SND_UMP_EP_INFO_PROTO_MIDI2
                            : linux::alsa::SND_UMP_EP_INFO_PROTO_MIDI1
                    );
                    linux::alsa::snd_ump_endpoint_info_set_num_blocks(ep, 1);
                    linux::alsa::snd_ump_endpoint_info_set_version(ep, linux::alsa::SND_UMP_EP_INFO_DEFAULT_VERSION);
                    ok = linux::alsa::snd_seq_create_ump_endpoint(seq, ep, groups) >= 0;

                    if (ok) {
                        // The block must be named: a group port gets its
                        // read/write capabilities only from a named block.
                        linux::alsa::snd_ump_block_info_set_block_id(blk, 0);
                        linux::alsa::snd_ump_block_info_set_active(blk, 1);
                        linux::alsa::snd_ump_block_info_set_direction(blk, linux::alsa::SND_UMP_DIR_BIDIRECTION);
                        linux::alsa::snd_ump_block_info_set_first_group(blk, 0);
                        linux::alsa::snd_ump_block_info_set_num_groups(blk, groups);
                        linux::alsa::snd_ump_block_info_set_ui_hint(blk, linux::alsa::SND_UMP_BLOCK_UI_HINT_BOTH);
                        linux::alsa::snd_ump_block_info_set_name(blk, _options.name.c_str());
                        ok = linux::alsa::snd_seq_create_ump_block(seq, 0, blk) >= 0;
                    }
                }
                if (ep != nullptr) {
                    linux::alsa::snd_ump_endpoint_info_free(ep);
                }
                if (blk != nullptr) {
                    linux::alsa::snd_ump_block_info_free(blk);
                }
                return ok;
            });
        }
    public:
        explicit AlsaVirtualUmpEndpoint(VirtualEndpointOptions options):
            _options{Ops::move(options)} {}

        ~AlsaVirtualUmpEndpoint() override {
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
            if (_options.groups < 1 || _options.groups > 16) {
                throw MidiException("a UMP endpoint has 1 to 16 groups");
            }
            if (_options.name.empty()) {
                throw MidiException("a virtual endpoint needs a name");
            }
            _client = Pointers::unique<AlsaUmpSeqClient>(_options.name, _options.protocol, false);
            if (!declare_endpoint()) {
                _client.reset();
                throw MidiException("cannot create the UMP endpoint");
            }
            // Port 0 is the endpoint port: sending from it reaches its
            // subscribers and those of the packet's group port.
            _rx = Pointers::unique<AlsaSeqUmpReceiver>(
                *_client, u8{0},
                linux::alsa::SND_SEQ_ADDRESS_SUBSCRIBERS, u8{0},
                _options.protocol, u8{0}
            );
            _tx = Pointers::unique<AlsaSeqUmpTransmitter>(*_client, -1);

            const i32 id = _client->id();
            _dev_info = MidiDeviceInfo {
                .id = Ops::fmt("{}:0", id),
                .name = _options.name,
                .description = _options.protocol == UmpProtocol::MIDI2
                    ? "virtual MIDI 2.0 endpoint"s
                    : "virtual MIDI 1.0 (UMP) endpoint"s,
                .backend = MidiBackend::ALSA_SEQ_UMP,
                .is_input = true,
                .is_output = true,
            };
            _open_flag = true;
        }

        void close() noexcept override {
            if (!_open_flag) {
                return;
            }
            _tx.reset();
            _rx.reset();
            _client.reset();
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
}

export namespace stdx::audio::midi {
    UniquePointer<MidiDevice> MidiSystem::create_virtual_endpoint(const VirtualEndpointOptions& options) {
        UniquePointer<AlsaVirtualUmpEndpoint> endpoint = Pointers::unique<AlsaVirtualUmpEndpoint>(options);
        endpoint->open();
        return endpoint;
    }
}
