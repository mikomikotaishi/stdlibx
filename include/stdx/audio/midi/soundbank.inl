#pragma once

using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief Soundbank / instrument catalog used by Synthesizer.
 */
export namespace stdx::audio::midi {
    /**
     * @struct Patch
     * @brief Identifier for an instrument slot: bank-MSB/LSB + program.
     */
    struct Patch {
        u16 bank = 0; ///< 14-bit bank: bank-MSB << 7 | bank-LSB
        u8 program = 0; ///< 0-127 GM program number

        [[nodiscard]]
        friend constexpr bool operator==(const Patch&, const Patch&) = default;
    };

    /**
     * @class Instrument
     * @brief Metadata for a single instrument. Backends (e.g. SoftSynthesizer
     * with a wavetable) attach concrete sound data via subclassing.
     */
    class Instrument {
    protected:
        Patch inst_patch;
        String inst_name;
    public:
        Instrument() = default;
        Instrument(Patch patch, String name):
            inst_patch{patch}, inst_name{Ops::move(name)} {}
        virtual ~Instrument() = default;

        [[nodiscard]]
        const Patch& patch() const noexcept {
            return inst_patch;
        }

        [[nodiscard]]
        const String& name() const noexcept {
            return inst_name;
        }
    };

    /**
     * @class SoundBank
     * @brief Catalog of instruments. Multiple banks can be loaded into a
     * Synthesizer; programs select within the active bank.
     */
    class SoundBank {
    protected:
        String bank_name;
        Vector<UniquePointer<Instrument>> bank_instruments;
    public:
        SoundBank() = default;
        explicit SoundBank(String name):
            bank_name{Ops::move(name)} {}

        virtual ~SoundBank() = default;

        [[nodiscard]]
        const String& name() const noexcept {
            return bank_name;
        }

        [[nodiscard]]
        Span<const UniquePointer<Instrument>> instruments() const noexcept {
            return Span{bank_instruments.data(), bank_instruments.size()};
        }

        void add(UniquePointer<Instrument> i) {
            bank_instruments.push_back(Ops::move(i));
        }

        /**
         * @brief Find the first instrument that matches @p patch
         * @param patch The patch to search for
         * @return Pointer to the matching instrument, or nullptr if not found
         */
        [[nodiscard]]
        const Instrument* find(const Patch& patch) const noexcept {
            for (const UniquePointer<Instrument>& i: bank_instruments) {
                if (i->patch() == patch) {
                    return i.get();
                }
            }
            return nullptr;
        }
    };
}
