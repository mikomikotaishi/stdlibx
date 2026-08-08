#pragma once

using stdx::sync::Atomic;
using stdx::sync::Mutex;
using stdx::sync::LockGuard;
using stdx::thread::Thread;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

using namespace stdx::os;

namespace stdx::audio::midi {
    [[nodiscard]]
    inline linux::alsa::SoundSequencer* alsa_seq_client() noexcept {
        static linux::alsa::SoundSequencer* client = [] -> linux::alsa::SoundSequencer* {
            linux::alsa::SoundSequencer* c = nullptr;
            if (
                linux::alsa::snd_seq_open(
                    &c, "default",
                    linux::alsa::SND_SEQ_OPEN_DUPLEX,
                    linux::alsa::SND_SEQ_NONBLOCK
                ) < 0
            ) {
                return nullptr;
            }
            linux::alsa::snd_seq_set_client_name(c, "stdx::audio");
            return c;
        }();
        return client;
    }

    /**
     * @class AlsaMidiParser
     * @brief RAII wrapper for snd_midi_event_t MIDI parser/encoder.
     */
    class AlsaMidiParser final {
    private:
        linux::alsa::SoundMidiEventParser* _parser = nullptr;
    public:
        explicit AlsaMidiParser(usize bufsize = 256) noexcept {
            linux::alsa::snd_midi_event_new(bufsize, &_parser);
            if (_parser != nullptr) {
                linux::alsa::snd_midi_event_init(_parser);
                // Disable running-status suppression so each ShortMessage
                // becomes a self-contained snd_seq_event_t.
                linux::alsa::snd_midi_event_no_status(_parser, 1);
            }
        }

        ~AlsaMidiParser() noexcept {
            if (_parser != nullptr) {
                linux::alsa::snd_midi_event_free(_parser);
            }
        }

        AlsaMidiParser(const AlsaMidiParser&) = DELETE_METHOD("AlsaMidiParser is not copyable.");
        AlsaMidiParser& operator=(const AlsaMidiParser&) = DELETE_METHOD("AlsaMidiParser is not copyable.");

        [[nodiscard]]
        linux::alsa::SoundMidiEventParser* get() const noexcept {
            return _parser;
        }

        [[nodiscard]]
        bool valid() const noexcept {
            return _parser != nullptr;
        }
    };

    /**
     * @class AlsaSeqReceiver
     * @brief Receiver implementation that sends MIDI messages to an ALSA sequencer port.
     * @extends Receiver
     */
    class AlsaSeqReceiver final: public Receiver {
    private:
        i32 _my_port;
        i32 _dest_client;
        i32 _dest_port;
        AlsaMidiParser _encoder;
        Mutex _send_mutex;
    public:
        AlsaSeqReceiver(i32 my, i32 dc, i32 dp) noexcept:
            _my_port{my}, _dest_client{dc}, _dest_port{dp} {}

        void send(const MidiMessage& msg, [[maybe_unused]] u64 time_micros) override {
            linux::alsa::SoundSequencer* client = alsa_seq_client();
            if (client == nullptr || !_encoder.valid() || msg.length() == 0) {
                return;
            }

            LockGuard<Mutex> guard{_send_mutex};

            linux::alsa::SoundSequencerEvent ev{};

            linux::alsa::snd_midi_event_reset_encode(_encoder.get());
            const Span<const u8> bytes = msg.data();
            i64 n = linux::alsa::snd_midi_event_encode(
                _encoder.get(),
                bytes.data(),
                static_cast<i64>(bytes.size()),
                &ev
            );
            if (n <= 0) {
                return;
            }

            ev.source.port = static_cast<u8>(_my_port);
            ev.dest.client = static_cast<u8>(_dest_client);
            ev.dest.port = static_cast<u8>(_dest_port);
            ev.queue = linux::alsa::SND_SEQ_QUEUE_DIRECT;

            linux::alsa::snd_seq_event_output_direct(client, &ev);
        }

        void close() noexcept override {
            // The port is owned by the AlsaSeqDevice; nothing to release
            // here beyond what the parser destructor does.
        }
    };

    class AlsaSeqTransmitter final: public Transmitter {
    private:
        i32 _my_port;
        Atomic<Receiver*> _rx{nullptr};
        Atomic<bool> _running{false};
        Thread _worker;
        AlsaMidiParser _decoder;
        Mutex _decode_mutex;

        void input_loop() noexcept {
            linux::alsa::SoundSequencer* client = alsa_seq_client();
            if (client == nullptr) {
                return;
            }

            const i32 npfd = linux::alsa::snd_seq_poll_descriptors_count(client, unix::POLLIN);
            if (npfd <= 0) {
                return;
            }
            Vector<linux::sys::PollFileDescriptor> pfds(static_cast<usize>(npfd));
            linux::alsa::snd_seq_poll_descriptors(
                client, pfds.data(), static_cast<u32>(npfd), unix::POLLIN
            );

            u8 decode_buf[512];
            while (_running.load()) {
                i32 r = linux::sys::poll(pfds.data(), static_cast<linux::sys::FileDescriptorCount>(npfd), 100);
                if (r < 0) {
                    if (unix::errnov() == unix::EINTR) {
                        continue;
                    }
                    break;
                }
                if (r == 0) continue;

                // Drain every pending event for our port
                while (true) {
                    linux::alsa::SoundSequencerEvent* ev = nullptr;
                    i32 err = linux::alsa::snd_seq_event_input(client, &ev);
                    if (err < 0 || !ev) {
                        break;
                    }

                    // Filter: only events whose destination port matches us
                    if (ev->dest.port != static_cast<u8>(_my_port)) {
                        continue;
                    }

                    i64 n;
                    {
                        LockGuard<Mutex> guard{_decode_mutex};
                        linux::alsa::snd_midi_event_reset_decode(_decoder.get());
                        n = linux::alsa::snd_midi_event_decode(
                            _decoder.get(),
                            decode_buf, sizeof(decode_buf),
                            ev
                        );
                    }
                    if (n <= 0) {
                        continue;
                    }

                    Receiver* r = _rx.load();
                    if (r == nullptr) {
                        continue;
                    }

                    Vector<u8> bytes(decode_buf, decode_buf + n);
                    MidiMessage out_msg{Ops::move(bytes)};
                    r->send(out_msg, 0);
                }
            }
        }
    public:
        explicit AlsaSeqTransmitter(i32 my):
            _my_port{my} {
            if (_decoder.valid()) {
                _running.store(true);
                _worker = Thread([this] -> void {
                    input_loop();
                });
            }
        }

        ~AlsaSeqTransmitter() override {
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
     * @class AlsaSeqDevice
     * @brief MidiDevice implementation for ALSA sequencer ports.
     * @extends MidiDevice
     */
    class AlsaSeqDevice final: public MidiDevice {
        MidiDeviceInfo _dev_info;
        i32 _target_client;
        i32 _target_port;
        i32 _my_write_port = -1;
        i32 _my_read_port = -1;
        UniquePointer<AlsaSeqReceiver> _rx;
        UniquePointer<AlsaSeqTransmitter> _tx;
        bool _open_flag = false;
    public:
        AlsaSeqDevice(MidiDeviceInfo info, i32 tc, i32 tp) noexcept:
            _dev_info{Ops::move(info)},
            _target_client{tc},
            _target_port{tp} {}

        ~AlsaSeqDevice() override {
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
            linux::alsa::SoundSequencer* client = alsa_seq_client();
            if (client == nullptr) {
                throw MidiException("ALSA seq client unavailable");
            }

            if (_dev_info.is_output) {
                _my_write_port = linux::alsa::snd_seq_create_simple_port(
                    client,
                    _dev_info.name.c_str(),
                    linux::alsa::SND_SEQ_PORT_CAP_READ |
                    linux::alsa::SND_SEQ_PORT_CAP_SUBS_READ,
                    linux::alsa::SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                    linux::alsa::SND_SEQ_PORT_TYPE_APPLICATION
                );
                if (_my_write_port < 0) {
                    throw MidiException("create_simple_port (write) failed");
                }
                // Best-effort subscription - if the target rejects, the
                // user can still attempt a direct send.
                linux::alsa::snd_seq_connect_to(
                    client, _my_write_port, _target_client, _target_port
                );
                _rx = Pointers::unique<AlsaSeqReceiver>(
                    _my_write_port, _target_client, _target_port
                );
            }

            if (_dev_info.is_input) {
                _my_read_port = linux::alsa::snd_seq_create_simple_port(
                    client,
                    _dev_info.name.c_str(),
                    linux::alsa::SND_SEQ_PORT_CAP_WRITE |
                    linux::alsa::SND_SEQ_PORT_CAP_SUBS_WRITE,
                    linux::alsa::SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                    linux::alsa::SND_SEQ_PORT_TYPE_APPLICATION
                );
                if (_my_read_port < 0) {
                    if (_my_write_port >= 0) {
                        linux::alsa::snd_seq_delete_simple_port(
                            client, _my_write_port
                        );
                        _my_write_port = -1;
                    }
                    throw MidiException("create_simple_port (read) failed");
                }
                linux::alsa::snd_seq_connect_from(
                    client, _my_read_port, _target_client, _target_port
                );
                _tx = Pointers::unique<AlsaSeqTransmitter>(_my_read_port);
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

            linux::alsa::SoundSequencer* client = alsa_seq_client();
            if (client != nullptr) {
                if (_my_write_port >= 0) {
                    linux::alsa::snd_seq_delete_simple_port(client, _my_write_port);
                }
                if (_my_read_port >= 0) {
                    linux::alsa::snd_seq_delete_simple_port(client, _my_read_port);
                }
            }
            _my_write_port = -1;
            _my_read_port = -1;
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

    [[nodiscard]]
    inline String make_alsa_id(i32 c, i32 p) {
        return Ops::fmt("{}:{}", c, p);
    }

    inline bool parse_alsa_id(const String& id, i32& c, i32& p) noexcept {
        return stdx::io::cstdio::sscanf(id.c_str(), "%d:%d", &c, &p) == 2;
    }
}

export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() {
        Vector<MidiDeviceInfo> out;
        linux::alsa::SoundSequencer* client = alsa_seq_client();
        if (client == nullptr) {
            return out;
        }

        linux::alsa::SoundSequencerClientInfo* cinfo = nullptr;
        linux::alsa::SoundSequencerPortInfo* pinfo = nullptr;
        linux::alsa::snd_seq_client_info_malloc(&cinfo);
        linux::alsa::snd_seq_port_info_malloc(&pinfo);
        if (cinfo == nullptr || pinfo == nullptr) {
            if (cinfo != nullptr) {
                linux::alsa::snd_seq_client_info_free(cinfo);
            }
            if (pinfo != nullptr) {
                linux::alsa::snd_seq_port_info_free(pinfo);
            }
            throw MidiException("snd_seq_*_info allocation failed");
        }

        constexpr u32 read_caps = linux::alsa::SND_SEQ_PORT_CAP_READ
            | linux::alsa::SND_SEQ_PORT_CAP_SUBS_READ;
        constexpr u32 write_caps = linux::alsa::SND_SEQ_PORT_CAP_WRITE
            | linux::alsa::SND_SEQ_PORT_CAP_SUBS_WRITE;

        linux::alsa::snd_seq_client_info_set_client(cinfo, -1);
        while (linux::alsa::snd_seq_query_next_client(client, cinfo) >= 0) {
            const i32 c = linux::alsa::snd_seq_client_info_get_client(cinfo);
            // Skip our own client to avoid loopback "devices"
            if (c == linux::alsa::snd_seq_client_id(client)) {
                continue;
            }

            const char* client_name = linux::alsa::snd_seq_client_info_get_name(cinfo);

            linux::alsa::snd_seq_port_info_set_client(pinfo, c);
            linux::alsa::snd_seq_port_info_set_port(pinfo, -1);
            while (linux::alsa::snd_seq_query_next_port(client, pinfo) >= 0) {
                const u32 caps = linux::alsa::snd_seq_port_info_get_capability(pinfo);

                const bool can_read = (caps & read_caps)  == read_caps; // source: device -> us
                const bool can_write = (caps & write_caps) == write_caps; // sink: us -> device
                if (!can_read && !can_write) {
                    continue;
                }

                const i32 p = linux::alsa::snd_seq_port_info_get_port(pinfo);
                const char* port_name = linux::alsa::snd_seq_port_info_get_name(pinfo);

                out.push_back(Ops::move(MidiDeviceInfo {
                    .id = make_alsa_id(c, p),
                    .name = Ops::fmt("{} - {}", client_name, port_name),
                    .backend = MidiBackend::ALSA_SEQ,
                    .is_input = can_read,
                    .is_output = can_write,
                }));
            }
        }

        linux::alsa::snd_seq_port_info_free(pinfo);
        linux::alsa::snd_seq_client_info_free(cinfo);
        return out;
    }

    UniquePointer<MidiDevice> MidiSystem::open_device(const MidiDeviceInfo& info) {
        if (info.backend != MidiBackend::ALSA_SEQ) {
            throw MidiException("not an ALSA-seq device");
        }
        i32 c = -1;
        i32 p = -1;
        if (!parse_alsa_id(info.id, c, p)) {
            throw MidiException("malformed ALSA-seq device id");
        }
        UniquePointer<AlsaSeqDevice> dev = Pointers::unique<AlsaSeqDevice>(info, c, p);
        dev->open();
        return dev;
    }

    Synthesizer& MidiSystem::default_synthesizer() {
        throw MidiException("SoftSynthesizer not yet implemented");
    }
}
