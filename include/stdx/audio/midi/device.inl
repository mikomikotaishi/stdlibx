#pragma once

/**
 * @namespace stdx::audio::midi
 * @brief MIDI device abstraction
 */
export namespace stdx::audio::midi {
    /**
     * @enum MidiBackend
     * @brief Backend identifier - which OS MIDI system a device comes from.
     */
    enum class MidiBackend: u8 {
        NONE, ///< No MIDI support; placeholder for error cases.
        ALSA_SEQ, ///< Linux: ALSA sequencer API (virtual ports, synths, etc.)
        ALSA_RAW_MIDI, ///< Linux: ALSA raw MIDI API (physical ports, no synths)
        WINMM, ///< Windows: legacy Multimedia API
        CORE_MIDI, ///< macOS/iOS: Core MIDI
        SOFT, ///< Built-in soft-synth / virtual device
    };

    /**
     * @struct MidiDeviceInfo
     * @brief Description of one MIDI endpoint reported by the host OS.
     *
     * Holds the data needed to identify a device. Concrete handles for
     * opening ports live behind the MidiSystem factory methods.
     */
    struct MidiDeviceInfo {
        String id;
        String name;
        String vendor;
        String description;
        MidiBackend backend = MidiBackend::NONE;
        bool is_input = false; ///< Provides messages (Transmitter)
        bool is_output = false; ///< Accepts messages (Receiver)
        bool is_default = false;
    };

    class MidiDevice;

    /**
     * @interface Receiver
     * @brief Sink for outgoing MIDI messages.
     *
     * @c send() is callable from any thread; implementations serialize
     * internally where needed.
     */
    class Receiver {
    public:
        Receiver() = default;
        Receiver(const Receiver&) = delete;
        Receiver& operator=(const Receiver&) = delete;
        virtual ~Receiver() = default;

        /**
         * @brief Send a MIDI message to this receiver, optionally with a delivery deadline.
         * @param msg Message to deliver.
         * @param time_micros Monotonic-time delivery deadline in microseconds.
         * 0 (or any value already in the past) means "deliver as soon as possible".
         */
        virtual void send(const MidiMessage& msg, u64 time_micros = 0) = 0;

        virtual void close() noexcept = 0;
    };

    /**
     * @interface Transmitter
     * @brief Source for incoming MIDI messages.
     */
    class Transmitter {
    public:
        Transmitter() = default;
        Transmitter(const Transmitter&) = delete;
        Transmitter& operator=(const Transmitter&) = delete;
        virtual ~Transmitter() = default;

        /**
         * @brief Set the destination that will be called for each arriving message.
         * @param r Pointer to the receiver, or nullptr to clear.
         */
        virtual void set_receiver(Receiver* r) noexcept = 0;

        [[nodiscard]]
        virtual Receiver* receiver() const noexcept = 0;

        virtual void close() noexcept = 0;
    };

    /**
     * @interface MidiDevice
     * @brief Abstract MIDI endpoint.
     */
    class MidiDevice {
    public:
        MidiDevice() = default;
        MidiDevice(const MidiDevice&) = delete;
        MidiDevice& operator=(const MidiDevice&) = delete;
        virtual ~MidiDevice() = default;

        [[nodiscard]]
        virtual const MidiDeviceInfo& info() const noexcept = 0;

        virtual void open() = 0;

        virtual void close() noexcept = 0;

        [[nodiscard]]
        virtual bool is_open() const noexcept = 0;

        /**
         * @brief Open or borrow a Receiver if this device accepts output (is_output==true).
         * @return Pointer to the receiver, or nullptr otherwise.
         */
        [[nodiscard]]
        virtual Receiver* receiver() = 0;

        /**
         * @brief Open or borrow a Transmitter if this device accepts input (is_input==true).
         * @return Pointer to the transmitter, or nullptr otherwise.
         */
        [[nodiscard]]
        virtual Transmitter* transmitter() = 0;

        [[nodiscard]]
        virtual i64 microsecond_position() const noexcept {
            return -1;
        }
    };
}
