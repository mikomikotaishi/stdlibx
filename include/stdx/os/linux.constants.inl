#pragma once

/**
 * @namespace stdx::os::linux
 * @brief Linux operations
 */
export namespace stdx::os::linux {
    /**
     * @namespace alsa
     * @brief ALSA operations.
     */
    namespace alsa {
        /**
         * <alsa/asoundef.h> constants
         */

        // IEC958 (S/PDIF) - AES0 status byte
        inline constexpr u8 IEC958_AES0_PROFESSIONAL = 1 << 0; ///< 0 = consumer, 1 = professional
        inline constexpr u8 IEC958_AES0_NONAUDIO =  1 << 1; ///< 0 = audio, 1 = non-audio
        inline constexpr u8 IEC958_AES0_PRO_EMPHASIS = 7 << 2; ///< mask - emphasis
        inline constexpr u8 IEC958_AES0_PRO_EMPHASIS_NOTID = 0 << 2; ///< emphasis not indicated
        inline constexpr u8 IEC958_AES0_PRO_EMPHASIS_NONE = 1 << 2; ///< no emphasis
        inline constexpr u8 IEC958_AES0_PRO_EMPHASIS_5015 = 3 << 2; ///< 50/15us emphasis
        inline constexpr u8 IEC958_AES0_PRO_EMPHASIS_CCITT = 7 << 2; ///< CCITT J.17 emphasis
        inline constexpr u8 IEC958_AES0_PRO_FREQ_UNLOCKED = 1 << 5; ///< source sample frequency: 0 = locked, 1 = unlocked
        inline constexpr u8 IEC958_AES0_PRO_FS = 3 << 6; ///< mask - sample frequency
        inline constexpr u8 IEC958_AES0_PRO_FS_NOTID = 0 << 6; ///< fs not indicated
        inline constexpr u8 IEC958_AES0_PRO_FS_44100 = 1 << 6; ///< 44.1kHz
        inline constexpr u8 IEC958_AES0_PRO_FS_48000 = 2 << 6; ///< 48kHz
        inline constexpr u8 IEC958_AES0_PRO_FS_32000 = 3 << 6; ///< 32kHz
        inline constexpr u8 IEC958_AES0_CON_NOT_COPYRIGHT = 1 << 2; ///< 0 = copyright, 1 = not copyright
        inline constexpr u8 IEC958_AES0_CON_EMPHASIS = 7 << 3; ///< mask - emphasis
        inline constexpr u8 IEC958_AES0_CON_EMPHASIS_NONE = 0 << 3; ///< no emphasis
        inline constexpr u8 IEC958_AES0_CON_EMPHASIS_5015 = 1 << 3; ///< 50/15us emphasis
        inline constexpr u8 IEC958_AES0_CON_MODE = 3 << 6; ///< mask - mode

        // IEC958 - AES1 status byte
        inline constexpr u8 IEC958_AES1_PRO_MODE = 15 << 0; ///< mask - channel mode
        inline constexpr u8 IEC958_AES1_PRO_MODE_NOTID = 0 << 0; ///< mode not indicated
        inline constexpr u8 IEC958_AES1_PRO_MODE_STEREOPHONIC = 2 << 0; ///< stereophonic - ch A is left
        inline constexpr u8 IEC958_AES1_PRO_MODE_SINGLE = 4 << 0; ///< single channel
        inline constexpr u8 IEC958_AES1_PRO_MODE_TWO = 8 << 0; ///< two channels
        inline constexpr u8 IEC958_AES1_PRO_MODE_PRIMARY = 12 << 0; ///< primary/secondary
        inline constexpr u8 IEC958_AES1_PRO_MODE_BYTE3 = 15 << 0; ///< vector to byte 3
        inline constexpr u8 IEC958_AES1_PRO_USERBITS = 15 << 4; ///< mask - user bits
        inline constexpr u8 IEC958_AES1_PRO_USERBITS_NOTID = 0 << 4; ///< user bits not indicated
        inline constexpr u8 IEC958_AES1_PRO_USERBITS_192 = 8 << 4; ///< 192-bit structure
        inline constexpr u8 IEC958_AES1_PRO_USERBITS_UDEF = 12 << 4; ///< user defined application
        inline constexpr u8 IEC958_AES1_CON_CATEGORY = 0x7f; ///< consumer category
        inline constexpr u8 IEC958_AES1_CON_GENERAL = 0x00; ///< general category
        inline constexpr u8 IEC958_AES1_CON_LASEROPT_MASK = 0x07; ///< Laser-optical mask
        inline constexpr u8 IEC958_AES1_CON_LASEROPT_ID = 0x01; ///< Laser-optical ID
        inline constexpr u8 IEC958_AES1_CON_IEC908_CD = IEC958_AES1_CON_LASEROPT_ID | 0x00; ///< IEC958 CD compatible device
        inline constexpr u8 IEC958_AES1_CON_NON_IEC908_CD = IEC958_AES1_CON_LASEROPT_ID | 0x08; ///< non-IEC958 CD compatible device
        inline constexpr u8 IEC958_AES1_CON_MINI_DISC = IEC958_AES1_CON_LASEROPT_ID | 0x48; ///< Mini-Disc device
        inline constexpr u8 IEC958_AES1_CON_DVD = IEC958_AES1_CON_LASEROPT_ID | 0x18; ///< DVD device
        inline constexpr u8 IEC958_AES1_CON_LASTEROPT_OTHER = IEC958_AES1_CON_LASEROPT_ID | 0x78; ///< Other laser-optical product
        inline constexpr u8 IEC958_AES1_CON_DIGDIGCONV_MASK = 0x07; ///< digital<->digital converter mask
        inline constexpr u8 IEC958_AES1_CON_DIGDIGCONV_ID = 0x02; ///< digital<->digital converter id
        inline constexpr u8 IEC958_AES1_CON_PCM_CODER = IEC958_AES1_CON_DIGDIGCONV_ID | 0x00; ///< PCM coder
        inline constexpr u8 IEC958_AES1_CON_MIXER = IEC958_AES1_CON_DIGDIGCONV_ID | 0x10; ///< Digital signal mixer
        inline constexpr u8 IEC958_AES1_CON_RATE_CONVERTER = IEC958_AES1_CON_DIGDIGCONV_ID | 0x18; ///< Rate converter
        inline constexpr u8 IEC958_AES1_CON_SAMPLER = IEC958_AES1_CON_DIGDIGCONV_ID | 0x20; ///< PCM sampler
        inline constexpr u8 IEC958_AES1_CON_DSP = IEC958_AES1_CON_DIGDIGCONV_ID | 0x28; ///< Digital sound processor
        inline constexpr u8 IEC958_AES1_CON_DIGDIGCONV_OTHER = IEC958_AES1_CON_DIGDIGCONV_ID | 0x78; ///< Other digital<->digital product
        inline constexpr u8 IEC958_AES1_CON_MAGNETIC_MASK = 0x07; ///< Magnetic device mask
        inline constexpr u8 IEC958_AES1_CON_MAGNETIC_ID = 0x03; ///< Magnetic device ID
        inline constexpr u8 IEC958_AES1_CON_DAT = IEC958_AES1_CON_MAGNETIC_ID | 0x00; ///< Digital Audio Tape
        inline constexpr u8 IEC958_AES1_CON_VCR = IEC958_AES1_CON_MAGNETIC_ID | 0x08; ///< Video recorder
        inline constexpr u8 IEC958_AES1_CON_DCC = IEC958_AES1_CON_MAGNETIC_ID | 0x40; ///< Digital compact cassette
        inline constexpr u8 IEC958_AES1_CON_MAGNETIC_DISC = IEC958_AES1_CON_MAGNETIC_ID | 0x18; ///< Magnetic disc digital audio device
        inline constexpr u8 IEC958_AES1_CON_MAGNETIC_OTHER = IEC958_AES1_CON_MAGNETIC_ID | 0x78; ///< Other magnetic device
        inline constexpr u8 IEC958_AES1_CON_BROADCAST1_MASK = 0x07; ///< Broadcast mask
        inline constexpr u8 IEC958_AES1_CON_BROADCAST1_ID = 0x04; ///< Broadcast ID
        inline constexpr u8 IEC958_AES1_CON_DAB_JAPAN = IEC958_AES1_CON_BROADCAST1_ID | 0x00; ///< Digital audio broadcast (Japan)
        inline constexpr u8 IEC958_AES1_CON_DAB_EUROPE = IEC958_AES1_CON_BROADCAST1_ID | 0x08; ///< Digital audio broadcast (Europe)
        inline constexpr u8 IEC958_AES1_CON_DAB_USA = IEC958_AES1_CON_BROADCAST1_ID | 0x60; ///< Digital audio broadcast (USA)
        inline constexpr u8 IEC958_AES1_CON_SOFTWARE = IEC958_AES1_CON_BROADCAST1_ID | 0x40; ///< Electronic software delivery
        inline constexpr u8 IEC958_AES1_CON_IEC62105 = IEC958_AES1_CON_BROADCAST1_ID | 0x20; ///< Used by another standard (IEC 62105)
        inline constexpr u8 IEC958_AES1_CON_BROADCAST1_OTHER = IEC958_AES1_CON_BROADCAST1_ID | 0x78; ///< Other broadcast product
        inline constexpr u8 IEC958_AES1_CON_BROADCAST2_MASK = 0x0f; ///< Broadcast alternative mask
        inline constexpr u8 IEC958_AES1_CON_BROADCAST2_ID = 0x0e; ///< Broadcast alternative ID
        inline constexpr u8 IEC958_AES1_CON_MUSICAL_MASK = 0x07; ///< Musical device mask
        inline constexpr u8 IEC958_AES1_CON_MUSICAL_ID = 0x05; ///< Musical device ID
        inline constexpr u8 IEC958_AES1_CON_SYNTHESIZER = IEC958_AES1_CON_MUSICAL_ID | 0x00; ///< Synthesizer
        inline constexpr u8 IEC958_AES1_CON_MICROPHONE = IEC958_AES1_CON_MUSICAL_ID | 0x08; ///< Microphone
        inline constexpr u8 IEC958_AES1_CON_MUSICAL_OTHER = IEC958_AES1_CON_MUSICAL_ID | 0x78; ///< Other musical device
        inline constexpr u8 IEC958_AES1_CON_ADC_MASK = 0x1f; ///< ADC Mask
        inline constexpr u8 IEC958_AES1_CON_ADC_ID = 0x06; ///< ADC ID
        inline constexpr u8 IEC958_AES1_CON_ADC = IEC958_AES1_CON_ADC_ID | 0x00; ///< ADC without copyright information
        inline constexpr u8 IEC958_AES1_CON_ADC_OTHER = IEC958_AES1_CON_ADC_ID | 0x60; ///< Other ADC product (no copyright)
        inline constexpr u8 IEC958_AES1_CON_ADC_COPYRIGHT_MASK = 0x1f; ///< ADC Copyright mask
        inline constexpr u8 IEC958_AES1_CON_ADC_COPYRIGHT_ID = 0x16; ///< ADC Copyright ID
        inline constexpr u8 IEC958_AES1_CON_ADC_COPYRIGHT = IEC958_AES1_CON_ADC_COPYRIGHT_ID | 0x00; ///< ADC with copyright information
        inline constexpr u8 IEC958_AES1_CON_ADC_COPYRIGHT_OTHER = IEC958_AES1_CON_ADC_COPYRIGHT_ID | 0x60; ///< Other ADC with copyright information product
        inline constexpr u8 IEC958_AES1_CON_SOLIDMEM_MASK = 0x0f; ///< Solid memory based products mask
        inline constexpr u8 IEC958_AES1_CON_SOLIDMEM_ID = 0x08; ///< Solid memory based products ID
        inline constexpr u8 IEC958_AES1_CON_SOLIDMEM_DIGITAL_RECORDER_PLAYER = IEC958_AES1_CON_SOLIDMEM_ID | 0x00; ///< Digital audio recorder/player using solid state memory
        inline constexpr u8 IEC958_AES1_CON_SOLIDMEM_OTHER = IEC958_AES1_CON_SOLIDMEM_ID | 0x70; ///< Other solid state memory based product
        inline constexpr u8 IEC958_AES1_CON_EXPERIMENTAL = 0x40; ///< experimental category
        inline constexpr u8 IEC958_AES1_CON_ORIGINAL = 1 << 7; ///< depends on the category code

        // IEC958 - AES2 status byte
        inline constexpr u8 IEC958_AES2_PRO_SBITS = 7 << 0; ///< mask - sample bits
        inline constexpr u8 IEC958_AES2_PRO_SBITS_20 = 2 << 0; ///< 20-bit - coordination
        inline constexpr u8 IEC958_AES2_PRO_SBITS_24 = 4 << 0; ///< 24-bit - main audio
        inline constexpr u8 IEC958_AES2_PRO_SBITS_UDEF = 6 << 0; ///< user defined application
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN = 7 << 3; ///< mask - source word length
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN_NOTID = 0 << 3; ///< source word length not indicated
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN_22_18 = 2 << 3; ///< 22-bit or 18-bit
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN_23_19 = 4 << 3; ///< 23-bit or 19-bit
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN_24_20 = 5 << 3; ///< 24-bit or 20-bit
        inline constexpr u8 IEC958_AES2_PRO_WORDLEN_20_16 = 6 << 3; ///< 20-bit or 16-bit
        inline constexpr u8 IEC958_AES2_CON_SOURCE = 15 << 0; ///< mask - source number
        inline constexpr u8 IEC958_AES2_CON_SOURCE_UNSPEC = 0 << 0; ///< source number unspecified
        inline constexpr u8 IEC958_AES2_CON_CHANNEL = 15 << 4; ///< mask - channel number
        inline constexpr u8 IEC958_AES2_CON_CHANNEL_UNSPEC = 0 << 4; ///< channel number unspecified

        // IEC958 - AES3 status byte
        inline constexpr u8 IEC958_AES3_CON_FS = 15 << 0; ///< mask - sample frequency
        inline constexpr u8 IEC958_AES3_CON_FS_44100 = 0 << 0; ///< 44.1kHz
        inline constexpr u8 IEC958_AES3_CON_FS_NOTID = 1 << 0; ///< sample frequency non indicated
        inline constexpr u8 IEC958_AES3_CON_FS_48000 = 2 << 0; ///< 48kHz
        inline constexpr u8 IEC958_AES3_CON_FS_32000 = 3 << 0; ///< 32kHz
        inline constexpr u8 IEC958_AES3_CON_FS_22050 = 4 << 0; ///< 22.05kHz
        inline constexpr u8 IEC958_AES3_CON_FS_24000 = 6 << 0; ///< 24kHz
        inline constexpr u8 IEC958_AES3_CON_FS_88200 = 8 << 0; ///< 88.2kHz
        inline constexpr u8 IEC958_AES3_CON_FS_768000 = 9 << 0; ///< 768kHz
        inline constexpr u8 IEC958_AES3_CON_FS_96000 = 10 << 0; ///< 96kHz
        inline constexpr u8 IEC958_AES3_CON_FS_176400 = 12 << 0; ///< 176.4kHz
        inline constexpr u8 IEC958_AES3_CON_FS_192000 = 14 << 0; ///< 192kHz
        inline constexpr u8 IEC958_AES3_CON_CLOCK = 3 << 4; ///< mask - clock accuracy
        inline constexpr u8 IEC958_AES3_CON_CLOCK_1000PPM = 0 << 4; ///< 1000 ppm
        inline constexpr u8 IEC958_AES3_CON_CLOCK_50PPM = 1 << 4; ///< 50 ppm
        inline constexpr u8 IEC958_AES3_CON_CLOCK_VARIABLE = 2 << 4; ///< variable pitch

        // IEC958 - AES4 status byte
        inline constexpr u8 IEC958_AES4_CON_MAX_WORDLEN_24 = 1 << 0; ///< 0 = 20-bit, 1 = 24-bit
        inline constexpr u8 IEC958_AES4_CON_WORDLEN = 7 << 1; ///< mask - sample word length
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_NOTID = 0 << 1; ///< not indicated
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_20_16 = 1 << 1; ///< 20-bit or 16-bit
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_22_18 = 2 << 1; ///< 22-bit or 18-bit
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_23_19 = 4 << 1; ///< 23-bit or 19-bit
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_24_20 = 5 << 1; ///< 24-bit or 20-bit
        inline constexpr u8 IEC958_AES4_CON_WORDLEN_21_17 = 6 << 1; ///< 21-bit or 17-bit
        inline constexpr u8 IEC958_AES4_CON_ORIGFS = 15 << 4; ///< mask - original sample frequency
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_NOTID = 0 << 4; ///< original sample frequency not indicated
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_192000 = 1 << 4; ///< 192kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_12000 = 2 << 4; ///< 12kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_176400 = 3 << 4; ///< 176.4kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_96000 = 5 << 4; ///< 96kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_8000 = 6 << 4; ///< 8kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_88200 = 7 << 4; ///< 88.2kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_16000 = 8 << 4; ///< 16kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_24000 = 9 << 4; ///< 24kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_11025 = 10 << 4; ///< 11.025kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_22050 = 11 << 4; ///< 22.05kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_32000 = 12 << 4; ///< 32kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_48000 = 13 << 4; ///< 48kHz
        inline constexpr u8 IEC958_AES4_CON_ORIGFS_44100 = 15 << 4; ///< 44.1kHz

        // IEC958 - AES5 status byte (CGMS-A)
        inline constexpr u8 IEC958_AES5_CON_CGMSA = 3 << 0; ///< mask - CGMS-A
        inline constexpr u8 IEC958_AES5_CON_CGMSA_COPYFREELY = 0 << 0; ///< copying is permitted without restriction
        inline constexpr u8 IEC958_AES5_CON_CGMSA_COPYONCE = 1 << 0; ///< one generation of copies may be made
        inline constexpr u8 IEC958_AES5_CON_CGMSA_COPYNOMORE = 2 << 0; ///< condition not be used
        inline constexpr u8 IEC958_AES5_CON_CGMSA_COPYNEVER = 3 << 0; ///< no copying is permitted

        // CEA-861 - Audio InfoFrame fields
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CC = 7 << 0; ///< mask - channel count
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT = 0xf << 4; ///< mask - coding type
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_FROM_STREAM = 0 << 4; ///< refer to stream
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_IEC60958 = 1 << 4; ///< IEC-60958 L-PCM
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_AC3 = 2 << 4; ///< AC-3
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_MPEG1 = 3 << 4; ///< MPEG1 Layers 1 & 2
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_MP3 = 4 << 4; ///< MPEG1 Layer 3
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_MPEG2_MULTICH = 5 << 4; ///< MPEG2 Multichannel
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_AAC = 6 << 4; ///< AAC
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_DTS = 7 << 4; ///< DTS
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_ATRAC = 8 << 4; ///< ATRAC
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_ONEBIT = 9 << 4; ///< One Bit Audio
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_DOLBY_DIG_PLUS = 10 << 4; ///< Dolby Digital +
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_DTS_HD = 11 << 4; ///< DTS-HD
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_MAT = 12 << 4; ///< MAT (MLP)
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_DST = 13 << 4; ///< DST
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB1CT_WMA_PRO = 14 << 4; ///< WMA Pro
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF = 7 << 2; ///< mask - sample frequency
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_FROM_STREAM = 0 << 2; ///< refer to stream
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_32000 = 1 << 2; ///< 32kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_44100 = 2 << 2; ///< 44.1kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_48000 = 3 << 2; ///< 48kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_88200 = 4 << 2; ///< 88.2kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_96000 = 5 << 2; ///< 96kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_176400 = 6 << 2; ///< 176.4kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SF_192000 = 7 << 2; ///< 192kHz
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SS = 3 << 0; ///< mask - sample size
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SS_FROM_STREAM = 0 << 0; ///< refer to stream
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SS_16BIT = 1 << 0; ///< 16 bits
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SS_20BIT = 2 << 0; ///< 20 bits
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB2SS_24BIT = 3 << 0; ///< 24 bits
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB5_DM_INH = 1 << 7; ///< mask - inhibit downmixing
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB5_DM_INH_PERMITTED = 0 << 7; ///< stereo downmix permitted
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB5_DM_INH_PROHIBITED = 1 << 7; ///< stereo downmix prohibited
        inline constexpr u8 CEA861_AUDIO_INFOFRAME_DB5_LSV = 0xf << 3; ///< mask - level-shift values

        // MIDI 1.0 command bytes and channel count
        inline constexpr i32 MIDI_CHANNELS = 16; ///< Number of channels per port/cable.
        inline constexpr i32 MIDI_GM_DRUM_CHANNEL = 10 - 1; ///< Channel number for GM drums.
        inline constexpr u8 MIDI_CMD_NOTE_OFF = 0x80; ///< note off
        inline constexpr u8 MIDI_CMD_NOTE_ON = 0x90; ///< note on
        inline constexpr u8 MIDI_CMD_NOTE_PRESSURE = 0xa0; ///< key pressure
        inline constexpr u8 MIDI_CMD_CONTROL = 0xb0; ///< control change
        inline constexpr u8 MIDI_CMD_PGM_CHANGE = 0xc0; ///< program change
        inline constexpr u8 MIDI_CMD_CHANNEL_PRESSURE = 0xd0; ///< channel pressure
        inline constexpr u8 MIDI_CMD_BENDER = 0xe0; ///< pitch bender
        inline constexpr u8 MIDI_CMD_COMMON_SYSEX = 0xf0; ///< sysex (system exclusive) begin
        inline constexpr u8 MIDI_CMD_COMMON_MTC_QUARTER = 0xf1; ///< MTC quarter frame
        inline constexpr u8 MIDI_CMD_COMMON_SONG_POS = 0xf2; ///< song position
        inline constexpr u8 MIDI_CMD_COMMON_SONG_SELECT = 0xf3; ///< song select
        inline constexpr u8 MIDI_CMD_COMMON_TUNE_REQUEST = 0xf6; ///< tune request
        inline constexpr u8 MIDI_CMD_COMMON_SYSEX_END = 0xf7; ///< end of sysex
        inline constexpr u8 MIDI_CMD_COMMON_CLOCK = 0xf8; ///< clock
        inline constexpr u8 MIDI_CMD_COMMON_START = 0xfa; ///< start
        inline constexpr u8 MIDI_CMD_COMMON_CONTINUE = 0xfb; ///< continue
        inline constexpr u8 MIDI_CMD_COMMON_STOP = 0xfc; ///< stop
        inline constexpr u8 MIDI_CMD_COMMON_SENSING = 0xfe; ///< active sensing
        inline constexpr u8 MIDI_CMD_COMMON_RESET = 0xff; ///< reset

        // MIDI controller IDs (MSB / fine-resolution pair via +0x20)
        inline constexpr u8 MIDI_CTL_MSB_BANK = 0x00; ///< Bank selection
        inline constexpr u8 MIDI_CTL_MSB_MODWHEEL = 0x01; ///< Modulation
        inline constexpr u8 MIDI_CTL_MSB_BREATH = 0x02; ///< Breath
        inline constexpr u8 MIDI_CTL_MSB_FOOT = 0x04; ///< Foot
        inline constexpr u8 MIDI_CTL_MSB_PORTAMENTO_TIME = 0x05; ///< Portamento time
        inline constexpr u8 MIDI_CTL_MSB_DATA_ENTRY = 0x06; ///< Data entry
        inline constexpr u8 MIDI_CTL_MSB_MAIN_VOLUME = 0x07; ///< Main volume
        inline constexpr u8 MIDI_CTL_MSB_BALANCE = 0x08; ///< Balance
        inline constexpr u8 MIDI_CTL_MSB_PAN = 0x0a; ///< Panpot
        inline constexpr u8 MIDI_CTL_MSB_EXPRESSION = 0x0b; ///< Expression
        inline constexpr u8 MIDI_CTL_MSB_EFFECT1 = 0x0c; ///< Effect1
        inline constexpr u8 MIDI_CTL_MSB_EFFECT2 = 0x0d; ///< Effect2
        inline constexpr u8 MIDI_CTL_MSB_GENERAL_PURPOSE1 = 0x10; ///< General purpose 1
        inline constexpr u8 MIDI_CTL_MSB_GENERAL_PURPOSE2 = 0x11; ///< General purpose 2
        inline constexpr u8 MIDI_CTL_MSB_GENERAL_PURPOSE3 = 0x12; ///< General purpose 3
        inline constexpr u8 MIDI_CTL_MSB_GENERAL_PURPOSE4 = 0x13; ///< General purpose 4
        inline constexpr u8 MIDI_CTL_LSB_BANK = 0x20; ///< Bank selection
        inline constexpr u8 MIDI_CTL_LSB_MODWHEEL = 0x21; ///< Modulation
        inline constexpr u8 MIDI_CTL_LSB_BREATH = 0x22; ///< Breath
        inline constexpr u8 MIDI_CTL_LSB_FOOT = 0x24; ///< Foot
        inline constexpr u8 MIDI_CTL_LSB_PORTAMENTO_TIME = 0x25; ///< Portamento time
        inline constexpr u8 MIDI_CTL_LSB_DATA_ENTRY = 0x26; ///< Data entry
        inline constexpr u8 MIDI_CTL_LSB_MAIN_VOLUME = 0x27; ///< Main volume
        inline constexpr u8 MIDI_CTL_LSB_BALANCE = 0x28; ///< Balance
        inline constexpr u8 MIDI_CTL_LSB_PAN = 0x2a; ///< Panpot
        inline constexpr u8 MIDI_CTL_LSB_EXPRESSION = 0x2b; ///< Expression
        inline constexpr u8 MIDI_CTL_LSB_EFFECT1 = 0x2c; ///< Effect1
        inline constexpr u8 MIDI_CTL_LSB_EFFECT2 = 0x2d; ///< Effect2
        inline constexpr u8 MIDI_CTL_LSB_GENERAL_PURPOSE1 = 0x30; ///< General purpose 1
        inline constexpr u8 MIDI_CTL_LSB_GENERAL_PURPOSE2 = 0x31; ///< General purpose 2
        inline constexpr u8 MIDI_CTL_LSB_GENERAL_PURPOSE3 = 0x32; ///< General purpose 3
        inline constexpr u8 MIDI_CTL_LSB_GENERAL_PURPOSE4 = 0x33; ///< General purpose 4
        inline constexpr u8 MIDI_CTL_SUSTAIN = 0x40; ///< Sustain pedal
        inline constexpr u8 MIDI_CTL_PORTAMENTO = 0x41; ///< Portamento
        inline constexpr u8 MIDI_CTL_SOSTENUTO = 0x42; ///< Sostenuto
        inline constexpr u8 MIDI_CTL_SOFT_PEDAL = 0x43; ///< Soft pedal
        inline constexpr u8 MIDI_CTL_LEGATO_FOOTSWITCH = 0x44; ///< Legato foot switch
        inline constexpr u8 MIDI_CTL_HOLD2 = 0x45; ///< Hold2
        inline constexpr u8 MIDI_CTL_SC1_SOUND_VARIATION = 0x46; ///< SC1 Sound Variation
        inline constexpr u8 MIDI_CTL_SC2_TIMBRE = 0x47; ///< SC2 Timbre
        inline constexpr u8 MIDI_CTL_SC3_RELEASE_TIME = 0x48; ///< SC3 Release Time
        inline constexpr u8 MIDI_CTL_SC4_ATTACK_TIME = 0x49; ///< SC4 Attack Time
        inline constexpr u8 MIDI_CTL_SC5_BRIGHTNESS = 0x4a; ///< SC5 Brightness
        inline constexpr u8 MIDI_CTL_SC6 = 0x4b; ///< SC6
        inline constexpr u8 MIDI_CTL_SC7 = 0x4c; ///< SC7
        inline constexpr u8 MIDI_CTL_SC8 = 0x4d; ///< SC8
        inline constexpr u8 MIDI_CTL_SC9 = 0x4e; ///< SC9
        inline constexpr u8 MIDI_CTL_SC10 = 0x4f; ///< SC10
        inline constexpr u8 MIDI_CTL_GENERAL_PURPOSE5 = 0x50; ///< General purpose 5
        inline constexpr u8 MIDI_CTL_GENERAL_PURPOSE6 = 0x51; ///< General purpose 6
        inline constexpr u8 MIDI_CTL_GENERAL_PURPOSE7 = 0x52; ///< General purpose 7
        inline constexpr u8 MIDI_CTL_GENERAL_PURPOSE8 = 0x53; ///< General purpose 8
        inline constexpr u8 MIDI_CTL_PORTAMENTO_CONTROL = 0x54; ///< Portamento control
        inline constexpr u8 MIDI_CTL_E1_REVERB_DEPTH = 0x5b; ///< E1 Reverb Depth
        inline constexpr u8 MIDI_CTL_E2_TREMOLO_DEPTH = 0x5c; ///< E2 Tremolo Depth
        inline constexpr u8 MIDI_CTL_E3_CHORUS_DEPTH = 0x5d; ///< E3 Chorus Depth
        inline constexpr u8 MIDI_CTL_E4_DETUNE_DEPTH = 0x5e; ///< E4 Detune Depth
        inline constexpr u8 MIDI_CTL_E5_PHASER_DEPTH = 0x5f; ///< E5 Phaser Depth
        inline constexpr u8 MIDI_CTL_DATA_INCREMENT = 0x60; ///< Data Increment
        inline constexpr u8 MIDI_CTL_DATA_DECREMENT = 0x61; ///< Data Decrement
        inline constexpr u8 MIDI_CTL_NRPN_LSB = 0x62; ///< Non-registered parameter number
        inline constexpr u8 MIDI_CTL_NRPN_MSB = 0x63; ///< Non-registered parameter number
        inline constexpr u8 MIDI_CTL_RPN_LSB = 0x64; ///< Registered parameter number
        inline constexpr u8 MIDI_CTL_RPN_MSB = 0x65; ///< Registered parameter number
        inline constexpr u8 MIDI_CTL_ALL_SOUNDS_OFF = 0x78; ///< All sounds off
        inline constexpr u8 MIDI_CTL_RESET_CONTROLLERS = 0x79; ///< Reset Controllers
        inline constexpr u8 MIDI_CTL_LOCAL_CONTROL_SWITCH = 0x7a; ///< Local Control Switch
        inline constexpr u8 MIDI_CTL_ALL_NOTES_OFF = 0x7b; ///< All notes off
        inline constexpr u8 MIDI_CTL_OMNI_OFF = 0x7c; ///< Omni off
        inline constexpr u8 MIDI_CTL_OMNI_ON = 0x7d; ///< Omni on
        inline constexpr u8 MIDI_CTL_MONO1 = 0x7e; ///< Mono1
        inline constexpr u8 MIDI_CTL_MONO2 = 0x7f; ///< Mono2

        /**
         * <alsa/pcm.h> constants
         */

        inline constexpr u32 SND_PCM_NONBLOCK = 0x00000001;
        inline constexpr u32 SND_PCM_ASYNC = 0x00000002;
        inline constexpr u32 SND_PCM_EINTR = 0x00000080;
        inline constexpr u32 SND_PCM_ABORT = 0x00008000;
        inline constexpr u32 SND_PCM_NO_AUTO_RESAMPLE = 0x00010000;
        inline constexpr u32 SND_PCM_NO_AUTO_CHANNELS = 0x00020000;
        inline constexpr u32 SND_PCM_NO_AUTO_FORMAT = 0x00040000;
        inline constexpr u32 SND_PCM_NO_SOFTVOL = 0x00080000;
        inline constexpr i32 SND_PCM_HW_PARAMS_SYNC_SIZE = 16;
        inline constexpr i32 SND_PCM_WAIT_INFINITE = -1;
        inline constexpr i32 SND_PCM_WAIT_IO = -10001;
        inline constexpr i32 SND_PCM_WAIT_DRAIN = -10002;

        inline constexpr u32 SND_CHMAP_POSITION_MASK = 0xffff;
        inline constexpr u32 SND_CHMAP_PHASE_INVERSE = (0x01 << 16);
        inline constexpr u32 SND_CHMAP_DRIVER_SPEC = (0x02 << 16);

        /**
         * <alsa/rawmidi.h> constants
         */

        inline constexpr u32 SND_RAWMIDI_APPEND = 0x0001;
        inline constexpr u32 SND_RAWMIDI_NONBLOCK = 0x0002;
        inline constexpr u32 SND_RAWMIDI_SYNC = 0x0004;
        inline constexpr u32 SND_RAWMIDI_INFO_UMP = 0x00000008; ///< rawmidi is UMP
        inline constexpr u32 SND_RAWMIDI_INFO_STREAM_INACTIVE = 0x00000010; ///< selected substream is inactive
        inline constexpr u32 SNDRV_RAWMIDI_INFO_STREAM_INACTIVE = SND_RAWMIDI_INFO_STREAM_INACTIVE; ///< compatibility alias

        /**
         * <alsa/seq.h> constants
         */

        inline constexpr i32 SND_SEQ_OPEN_OUTPUT = 1; ///< open for output (write)
        inline constexpr i32 SND_SEQ_OPEN_INPUT = 2; ///< open for input (read)
        inline constexpr i32 SND_SEQ_OPEN_DUPLEX = SND_SEQ_OPEN_OUTPUT | SND_SEQ_OPEN_INPUT; ///< open for both input and output (read/write)
        inline constexpr u32 SND_SEQ_NONBLOCK = 0x0001; ///< non-blocking mode (flag to open mode)

        // Sequencer addresses
        inline constexpr u8 SND_SEQ_ADDRESS_UNKNOWN = 253; ///< unknown source
        inline constexpr u8 SND_SEQ_ADDRESS_SUBSCRIBERS = 254; ///< send event to all subscribed ports
        inline constexpr u8 SND_SEQ_ADDRESS_BROADCAST = 255; ///< send event to all queues/clients/ports/channels

        // System client / ports
        inline constexpr u8 SND_SEQ_CLIENT_SYSTEM = 0; ///< system client
        inline constexpr u8 SND_SEQ_PORT_SYSTEM_TIMER = 0; ///< system timer port
        inline constexpr u8 SND_SEQ_PORT_SYSTEM_ANNOUNCE = 1; ///< system announce port

        // Port capabilities (bit flags)
        inline constexpr u32 SND_SEQ_PORT_CAP_READ = 1 << 0; ///< readable from this port
        inline constexpr u32 SND_SEQ_PORT_CAP_WRITE = 1 << 1; ///< writable to this port
        inline constexpr u32 SND_SEQ_PORT_CAP_SYNC_READ = 1 << 2; ///< allow read subscriptions
        inline constexpr u32 SND_SEQ_PORT_CAP_SYNC_WRITE = 1 << 3; ///< allow write subscriptions
        inline constexpr u32 SND_SEQ_PORT_CAP_DUPLEX = 1 << 4; ///< allow read/write duplex
        inline constexpr u32 SND_SEQ_PORT_CAP_SUBS_READ = 1 << 5; ///< allow read subscription
        inline constexpr u32 SND_SEQ_PORT_CAP_SUBS_WRITE = 1 << 6; ///< allow write subscription
        inline constexpr u32 SND_SEQ_PORT_CAP_NO_EXPORT = 1 << 7; ///< routing not allowed
        inline constexpr u32 SND_SEQ_PORT_CAP_INACTIVE = 1 << 8; ///< inactive port
        inline constexpr u32 SND_SEQ_PORT_CAP_UMP_ENDPOINT = 1 << 9; ///< UMP Endpoint port

        // Port direction
        inline constexpr u8 SND_SEQ_PORT_DIR_UNKNOWN = 0; ///< Unknown
        inline constexpr u8 SND_SEQ_PORT_DIR_INPUT = 1; ///< Input only; sink, receiver
        inline constexpr u8 SND_SEQ_PORT_DIR_OUTPUT = 2; ///< Output only; source, transmitter
        inline constexpr u8 SND_SEQ_PORT_DIR_BIDIRECTION = 3; ///< Input/output bidirectional

        // Port type (bit flags)
        inline constexpr u32 SND_SEQ_PORT_TYPE_SPECIFIC = 1 << 0;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_GENERIC = 1 << 1;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_GM = 1 << 2;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_GS = 1 << 3;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_XG = 1 << 4;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_MT32 = 1 << 5;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_GM2 = 1 << 6;
        inline constexpr u32 SND_SEQ_PORT_TYPE_MIDI_UMP = 1 << 7;
        inline constexpr u32 SND_SEQ_PORT_TYPE_SYNTH = 1 << 10;
        inline constexpr u32 SND_SEQ_PORT_TYPE_DIRECT_SAMPLE = 1 << 11;
        inline constexpr u32 SND_SEQ_PORT_TYPE_SAMPLE = 1 << 12;
        inline constexpr u32 SND_SEQ_PORT_TYPE_HARDWARE = 1 << 16;
        inline constexpr u32 SND_SEQ_PORT_TYPE_SOFTWARE = 1 << 17;
        inline constexpr u32 SND_SEQ_PORT_TYPE_SYNTHESIZER = 1 << 18;
        inline constexpr u32 SND_SEQ_PORT_TYPE_PORT = 1 << 19;
        inline constexpr u32 SND_SEQ_PORT_TYPE_APPLICATION = 1 << 20;

        // Queue
        inline constexpr u8 SND_SEQ_QUEUE_DIRECT = 253; ///< direct dispatch

        // remove_events flags
        inline constexpr u32 SND_SEQ_REMOVE_INPUT = 1 << 0; ///< Flush input queues
        inline constexpr u32 SND_SEQ_REMOVE_OUTPUT = 1 << 1; ///< Flush output queues
        inline constexpr u32 SND_SEQ_REMOVE_DEST = 1 << 2; ///< Restrict by destination q:client:port
        inline constexpr u32 SND_SEQ_REMOVE_DEST_CHANNEL = 1 << 3; ///< Restrict by channel
        inline constexpr u32 SND_SEQ_REMOVE_TIME_BEFORE = 1 << 4; ///< Restrict to before time
        inline constexpr u32 SND_SEQ_REMOVE_TIME_AFTER = 1 << 5; ///< Restrict to time or after
        inline constexpr u32 SND_SEQ_REMOVE_TIME_TICK = 1 << 6; ///< Time is in ticks
        inline constexpr u32 SND_SEQ_REMOVE_EVENT_TYPE = 1 << 7; ///< Restrict to event type
        inline constexpr u32 SND_SEQ_REMOVE_IGNORE_OFF = 1 << 8; ///< Do not flush off events
        inline constexpr u32 SND_SEQ_REMOVE_TAG_MATCH = 1 << 9; ///< Restrict to events with given tag

        /**
         * <alsa/seq_event.h> constants
         */

        // Timestamp / time-mode bits on snd_seq_event_t.flags
        inline constexpr u8 SND_SEQ_TIME_STAMP_TICK = 0 << 0; ///< timestamp in clock ticks
        inline constexpr u8 SND_SEQ_TIME_STAMP_REAL = 1 << 0; ///< timestamp in real time
        inline constexpr u8 SND_SEQ_TIME_STAMP_MASK = 1 << 0; ///< mask for timestamp bits
        inline constexpr u8 SND_SEQ_TIME_MODE_ABS = 0 << 1; ///< absolute timestamp
        inline constexpr u8 SND_SEQ_TIME_MODE_REL = 1 << 1; ///< relative to current time
        inline constexpr u8 SND_SEQ_TIME_MODE_MASK = 1 << 1; ///< mask for time mode bits
        inline constexpr u8 SND_SEQ_PRIORITY_NORMAL = 0 << 4; ///< normal priority
        inline constexpr u8 SND_SEQ_PRIORITY_HIGH = 1 << 4; ///< event should be processed before others
        inline constexpr u8 SND_SEQ_PRIORITY_MASK = 1 << 4; ///< mask for priority bits
        inline constexpr u8 SND_SEQ_EVENT_UMP = 1 << 5; ///< UMP packet event

        /**
         * <alsa/hwdep.h> constants
         */

        inline constexpr i32 SND_HWDEP_OPEN_READ = stdx::os::unix::O_RDONLY;
        inline constexpr i32 SND_HWDEP_OPEN_WRITE = stdx::os::unix::O_WRONLY;
        inline constexpr i32 SND_HWDEP_OPEN_DUPLEX = stdx::os::unix::O_RDWR;
        inline constexpr i32 SND_HWDEP_OPEN_NONBLOCK = stdx::os::unix::O_NONBLOCK;

        /**
         * <alsa/timer.h> constants
         */

        inline constexpr i32 SND_TIMER_GLOBAL_SYSTEM = 0;
        inline constexpr i32 SND_TIMER_GLOBAL_RTC = 1; ///< Obsoleted, due to enough legacy.
        inline constexpr i32 SND_TIMER_GLOBAL_HPET = 2;
        inline constexpr i32 SND_TIMER_GLOBAL_HRTIMER = 3;
        inline constexpr u32 SND_TIMER_OPEN_NONBLOCK = 1 << 0;
        inline constexpr u32 SND_TIMER_OPEN_TREAD = 1 << 1;

        /**
         * <alsa/error.h> constants
         */

        inline constexpr i32 SND_ERROR_BEGIN = 500000; ///< Lower boundary of sound error codes.
        inline constexpr i32 SND_ERROR_INCOMPATIBLE_VERSION = SND_ERROR_BEGIN + 0; ///< Kernel/library protocols are not compatible.

        // Log priority levels (snd_lib_log)
        inline constexpr i32 SND_LOG_ERROR = 1; ///< error priority level
        inline constexpr i32 SND_LOG_WARN = 2; ///< warning priority level
        inline constexpr i32 SND_LOG_INFO = 3; ///< info priority level
        inline constexpr i32 SND_LOG_DEBUG = 4; ///< debug priority level
        inline constexpr i32 SND_LOG_TRACE = 5; ///< trace priority level
        inline constexpr i32 SND_LOG_LAST = SND_LOG_TRACE; ///< last known value for priority level

        // Log interface identifiers
        inline constexpr i32 SND_ILOG_CORE = 1; ///< core library code
        inline constexpr i32 SND_ILOG_CONFIG = 2; ///< configuration parsing and operations
        inline constexpr i32 SND_ILOG_CONTROL = 3; ///< control API
        inline constexpr i32 SND_ILOG_HWDEP = 4; ///< hwdep API
        inline constexpr i32 SND_ILOG_TIMER = 5; ///< timer API
        inline constexpr i32 SND_ILOG_RAWMIDI = 6; ///< RawMidi API
        inline constexpr i32 SND_ILOG_PCM = 7; ///< PCM API
        inline constexpr i32 SND_ILOG_MIXER = 8; ///< mixer API
        inline constexpr i32 SND_ILOG_SEQUENCER = 9; ///< sequencer API
        inline constexpr i32 SND_ILOG_UCM = 10; ///< UCM API
        inline constexpr i32 SND_ILOG_TOPOLOGY = 11; ///< topology API
        inline constexpr i32 SND_ILOG_ASERVER = 12; ///< aserver
        inline constexpr i32 SND_ILOG_PCM_PARAMS = 13; ///< PCM hw_params operations
        inline constexpr i32 SND_ILOG_LAST = SND_ILOG_PCM_PARAMS; ///< last known value for interface

        /**
         * <alsa/ump.h> constants - Universal MIDI Packet (MIDI 2.0)
         */

        inline constexpr i32 SND_UMP_MAX_GROUPS = 16;
        inline constexpr i32 SND_UMP_MAX_BLOCKS = 32;
        inline constexpr u32 SND_UMP_EP_INFO_STATIC_BLOCKS = 0x01;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_MIDI_MASK = 0x0300;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_MIDI1 = 0x0100;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_MIDI2 = 0x0200;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_JRTS_MASK = 0x0003;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_JRTS_TX = 0x0001;
        inline constexpr u32 SND_UMP_EP_INFO_PROTO_JRTS_RX = 0x0002;
        inline constexpr u32 SND_UMP_EP_INFO_DEFAULT_VERSION = 0x0101;
        inline constexpr u32 SND_UMP_BLOCK_IS_MIDI1 = 1U << 0;
        inline constexpr u32 SND_UMP_BLOCK_INFO_DEFAULT_MIDI_CI_VERSION = 0x01;

        /**
         * <alsa/version.h> constants
         */

        inline constexpr i32 SND_LIB_MAJOR = 1; ///< major number of library version
        inline constexpr i32 SND_LIB_MINOR = 2; ///< minor number of library version
        inline constexpr i32 SND_LIB_SUBMINOR = 16; ///< subminor number of library version
        inline constexpr i32 SND_LIB_EXTRAVER = 1000000; ///< extra version number, used mainly for betas
        inline constexpr StringView SND_LIB_VERSION_STR = "1.2.16"; ///< library version string
    }

    /**
     * @namespace arpa
     * @brief ARPA operations.
     */
    namespace arpa {
        /**
         * <arpa/ftp.h> constants
         */

        // Reply codes
        inline constexpr i32 PRELIM = 1; ///< positive preliminary
        inline constexpr i32 COMPLETE = 2; ///< positive completion
        inline constexpr i32 CONTINUE = 3; ///< positive i32ermediate
        inline constexpr i32 TRANSIENT = 4; ///< transient negative completion
        inline constexpr i32 ERROR = 5; ///< permanent negative completion

        // Type codes
        inline constexpr i32 TYPE_A = 1; ///< ASCII
        inline constexpr i32 TYPE_E = 2; ///< EBCDIC
        inline constexpr i32 TYPE_I = 3; ///< image
        inline constexpr i32 TYPE_L = 4; ///< local byte size

        inline constexpr Array<StringView, 5> TypeNames = {"0", "ASCII", "EBCDIC", "Image", "Local" };

        // Form codes
        inline constexpr i32 FORM_N = 1; ///< non-pri32
        inline constexpr i32 FORM_T = 2; ///< telnet format effectors
        inline constexpr i32 FORM_C = 3; ///< carriage control (ASA)

        inline constexpr Array<StringView, 4> FormNames = {"0", "Nonpri32", "Telnet", "Carriage-control" };

        // Structure codes
        inline constexpr i32 STRU_F = 1; ///< file (no record structure)
        inline constexpr i32 STRU_R = 2; ///< record structure
        inline constexpr i32 STRU_P = 3; ///< page structure

        inline constexpr Array<StringView, 4> StruNames = {"0", "File", "Record", "Page" };

        // Mode types
        inline constexpr i32 MODE_S = 1; ///< stream
        inline constexpr i32 MODE_B = 2; ///< block
        inline constexpr i32 MODE_C = 3; ///< compressed 

        inline constexpr Array<StringView, 4> ModeNames = {"0", "Stream", "Block", "Compressed" };

        // Record Tokens
        inline constexpr char REC_ESC = '\377'; ///< Record-mode Escape
        inline constexpr char REC_EOR = '\001'; ///< Record-mode End-of-Record
        inline constexpr char REC_EOF = '\002'; ///< Record-mode End-of-File

        // Block Tokens
        inline constexpr u16 BLK_EOR = 0x80; ///< Block is End-of-Record
        inline constexpr u16 BLK_EOF = 0x40; ///< Block is End-of-File
        inline constexpr u16 BLK_ERRORS = 0x20; ///< Block is suspected of containing errors
        inline constexpr u16 BLK_RESTART = 0x10; ///< Block is Restart Marker
        inline constexpr u16 BLK_BYTECOUNT = 2; ///< Bytes in this block

        /**
         * <arpa/nameser.h> constants
         */

        /// Constants based on RFC 883, RFC 1034, RFC 1035
        inline constexpr i32 NS_PACKETSZ = 512; ///< default UDP packet size
        inline constexpr i32 NS_MAXDNAME = 1025; ///< maximum domain name
        inline constexpr i32 NS_MAXMSG = 65535; ///< maximum message size
        inline constexpr i32 NS_MAXCDNAME = 255; ///< maximum compressed domain name
        inline constexpr i32 NS_MAXLABEL = 63; ///< maximum length of domain label
        inline constexpr i32 NS_HFIXEDSZ = 12; ///< #/bytes of fixed data in header
        inline constexpr i32 NS_QFIXEDSZ = 4; ///< #/bytes of fixed data in query
        inline constexpr i32 NS_RRFIXEDSZ = 10; ///< #/bytes of fixed data in r record
        inline constexpr i32 NS_INT32SZ = 4; ///< #/bytes of data in a ui3232_t
        inline constexpr i32 NS_INT16SZ = 2; ///< #/bytes of data in a ui3216_t
        inline constexpr i32 NS_INT8SZ = 1; ///< #/bytes of data in a ui328_t
        inline constexpr i32 NS_INADDRSZ = 4; ///< IPv4 T_A
        inline constexpr i32 NS_IN6ADDRSZ = 16; ///< IPv6 T_AAAA
        inline constexpr i32 NS_CMPRSFLGS = 0xc0; ///< Flag bits indicating name compression
        inline constexpr i32 NS_DEFAULTPORT = 53; ///< For both TCP and UDP

        inline constexpr i32 NS_TSIG_FUDGE = 300;
        inline constexpr i32 NS_TSIG_TCP_COUNT = 100;
        inline constexpr StringView NS_TSIG_ALG_HMAC_MD5 = "HMAC-MD5.SIG-ALG.REG.INT";

        inline constexpr i32 NS_TSIG_ERROR_NO_TSIG = -10;
        inline constexpr i32 NS_TSIG_ERROR_NO_SPACE = -11;
        inline constexpr i32 NS_TSIG_ERROR_FORMERR = -12;

        /**
            * <arpa/telnet.h> constants
            */
        inline constexpr u8 IAC = 255; ///< i32erpret as command
        inline constexpr u8 DONT = 254; ///< you are not to use option
        inline constexpr u8 DO = 253; ///< please, you use option
        inline constexpr u8 WONT = 252; ///< I won't use option
        inline constexpr u8 WILL = 251; ///< I will use option
        inline constexpr u8 SB = 250; ///< i32erpret as subnegotiation
        inline constexpr u8 GA = 249; ///< you may reverse the line
        inline constexpr u8 EL = 248; ///< erase the current line
        inline constexpr u8 EC = 247; ///< erase the current character
        inline constexpr u8 AYT = 246; ///< are you there
        inline constexpr u8 AO = 245; ///< abort output--but let prog finish
        inline constexpr u8 IP = 244; ///< i32errupt process--permanently
        inline constexpr u8 BREAK = 243; ///< break
        inline constexpr u8 DM = 242; ///< data mark--for connect. cleaning
        inline constexpr u8 NOP = 241; ///< nop
        inline constexpr u8 SE = 240; ///< end sub negotiation
        inline constexpr u8 EOR = 239; ///< end of record (transparent mode)
        inline constexpr u8 ABORT = 238; ///< Abort process
        inline constexpr u8 SUSP = 237; ///< Suspend process
        inline constexpr u8 xEOF = 236; ///< End of file: EOF is already used...
        inline constexpr u8 SYNCH = 242; ///< for telfunc calls

        inline constexpr u8 TELCMD_FIRST = xEOF; ///< First telnet command
        inline constexpr u8 TELCMD_LAST = IAC; ///< Last telnet command

        [[nodiscard]]
        constexpr bool TELCMD_OK(u8 x) noexcept {
            return x <= TELCMD_LAST && x >= TELCMD_FIRST;
        }

        inline constexpr u8 TELOPT_BINARY = 0; ///< 8-bit data path
        inline constexpr u8 TELOPT_ECHO = 1; ///< echo
        inline constexpr u8 TELOPT_RCP = 2; ///< prepare to reconnect
        inline constexpr u8 TELOPT_SGA = 3; ///< suppress go ahead
        inline constexpr u8 TELOPT_NAMS = 4; ///< approximate message size
        inline constexpr u8 TELOPT_STATUS = 5; ///< give status
        inline constexpr u8 TELOPT_TM = 6; ///< timing mark
        inline constexpr u8 TELOPT_RCTE = 7; ///< remote controlled transmission and echo
        inline constexpr u8 TELOPT_NAOL = 8; ///< negotiate about output line width
        inline constexpr u8 TELOPT_NAOP = 9; ///< negotiate about output page size
        inline constexpr u8 TELOPT_NAOCRD = 10; ///< negotiate about CR disposition
        inline constexpr u8 TELOPT_NAOHTS = 11; ///< negotiate about horizontal tabstops
        inline constexpr u8 TELOPT_NAOHTD = 12; ///< negotiate about horizontal tab disposition
        inline constexpr u8 TELOPT_NAOFFD = 13; ///< negotiate about formfeed disposition
        inline constexpr u8 TELOPT_NAOVTS = 14; ///< negotiate about vertical tab stops
        inline constexpr u8 TELOPT_NAOVTD = 15; ///< negotiate about vertical tab disposition
        inline constexpr u8 TELOPT_NAOLFD = 16; ///< negotiate about output LF disposition
        inline constexpr u8 TELOPT_XASCII = 17; ///< extended ascii character set
        inline constexpr u8 TELOPT_LOGOUT = 18; ///< force logout
        inline constexpr u8 TELOPT_BM = 19; ///< byte macro
        inline constexpr u8 TELOPT_DET = 20; ///< data entry terminal
        inline constexpr u8 TELOPT_SUPDUP = 21; ///< supdup protocol
        inline constexpr u8 TELOPT_SUPDUPOUTPUT = 22; ///< supdup output
        inline constexpr u8 TELOPT_SNDLOC = 23; ///< send location
        inline constexpr u8 TELOPT_TTYPE = 24; ///< terminal type
        inline constexpr u8 TELOPT_EOR = 25; ///< end or record
        inline constexpr u8 TELOPT_TUID = 26; ///< TACACS user identification
        inline constexpr u8 TELOPT_OUTMRK = 27; ///< output marking
        inline constexpr u8 TELOPT_TTYLOC = 28; ///< terminal location number
        inline constexpr u8 TELOPT_3270REGIME = 29; ///< 3270 regime
        inline constexpr u8 TELOPT_X3PAD = 30; ///< X.3 PAD
        inline constexpr u8 TELOPT_NAWS = 31; ///< window size
        inline constexpr u8 TELOPT_TSPEED = 32; ///< terminal speed
        inline constexpr u8 TELOPT_LFLOW = 33; ///< remote flow control
        inline constexpr u8 TELOPT_LINEMODE = 34; ///< Linemode option
        inline constexpr u8 TELOPT_XDISPLOC = 35; ///< X Display Location
        inline constexpr u8 TELOPT_OLD_ENVIRON = 36; ///< Old - Environment variables
        inline constexpr u8 TELOPT_AUTHENTICATION = 37; ///< Authenticate
        inline constexpr u8 TELOPT_ENCRYPT = 38; ///< Encryption option
        inline constexpr u8 TELOPT_NEW_ENVIRON = 39; ///< New - Environment variables
        inline constexpr u8 TELOPT_EXOPL = 255; ///< extended-options-list

        inline constexpr i32 NTELOPTS = 1 + TELOPT_NEW_ENVIRON;
        inline constexpr u8 TELOPT_FIRST = TELOPT_BINARY;
        inline constexpr u8 TELOPT_LAST = TELOPT_NEW_ENVIRON;

        [[nodiscard]]
        constexpr bool TELOPT_OK(u8 x) noexcept {
            return x <= TELOPT_LAST;
        }

        // sub-option qualifiers
        inline constexpr u8 TELQUAL_IS = 0; ///< option is...
        inline constexpr u8 TELQUAL_SEND = 1; ///< send option
        inline constexpr u8 TELQUAL_INFO = 2; ///< ENVIRON: informational version of IS
        inline constexpr u8 TELQUAL_REPLY = 2; ///< AUTHENTICATION: client version of IS
        inline constexpr u8 TELQUAL_NAME = 3; ///< AUTHENTICATION: client version of IS

        inline constexpr u8 LFLOW_OFF = 0; ///< Disable remote flow control
        inline constexpr u8 LFLOW_ON = 1; ///< Enable remote flow control
        inline constexpr u8 LFLOW_RESTART_ANY = 2; ///< Restart output on any char
        inline constexpr u8 LFLOW_RESTART_XON = 3; ///< Restart output only on XON

        // LINEMODE suboptions
        inline constexpr u8 LM_MODE = 1;
        inline constexpr u8 LM_FORWARDMASK = 2;
        inline constexpr u8 LM_SLC = 3;

        inline constexpr u8 MODE_EDIT = 0x01;
        inline constexpr u8 MODE_TRAPSIG = 0x02;
        inline constexpr u8 MODE_ACK = 0x04;
        inline constexpr u8 MODE_SOFT_TAB = 0x08;
        inline constexpr u8 MODE_LIT_ECHO = 0x10;

        inline constexpr u8 MODE_MASK = 0x1f;

        // Not part of protocol, but needed to simplify things...
        inline constexpr u16 MODE_FLOW = 0x0100;
        inline constexpr u16 MODE_ECHO = 0x0200;
        inline constexpr u16 MODE_INBIN = 0x0400;
        inline constexpr u16 MODE_OUTBIN = 0x0800;
        inline constexpr u16 MODE_FORCE = 0x1000;

        inline constexpr u8 SLC_SYNCH = 1;
        inline constexpr u8 SLC_BRK = 2;
        inline constexpr u8 SLC_IP = 3;
        inline constexpr u8 SLC_AO = 4;
        inline constexpr u8 SLC_AYT = 5;
        inline constexpr u8 SLC_EOR = 6;
        inline constexpr u8 SLC_ABORT = 7;
        inline constexpr u8 SLC_EOF = 8;
        inline constexpr u8 SLC_SUSP = 9;
        inline constexpr u8 SLC_EC = 10;
        inline constexpr u8 SLC_EL = 11;
        inline constexpr u8 SLC_EW = 12;
        inline constexpr u8 SLC_RP = 13;
        inline constexpr u8 SLC_LNEXT = 14;
        inline constexpr u8 SLC_XON = 15;
        inline constexpr u8 SLC_XOFF = 16;
        inline constexpr u8 SLC_FORW1 = 17;
        inline constexpr u8 SLC_FORW2 = 18;

        inline constexpr i32 NSLC = 18;

        [[nodiscard]]
        constexpr bool SLC_NAME_OK(u8 x) noexcept {
            return x <= NSLC;
        }

        inline constexpr u8 SLC_NOSUPPORT = 0;
        inline constexpr u8 SLC_CANTCHANGE = 1;
        inline constexpr u8 SLC_VARIABLE = 2;
        inline constexpr u8 SLC_DEFAULT = 3;
        inline constexpr u8 SLC_LEVELBITS = 0x03;

        inline constexpr u8 SLC_FUNC = 0;
        inline constexpr u8 SLC_FLAGS = 1;
        inline constexpr u8 SLC_VALUE = 2;

        inline constexpr u8 SLC_ACK = 0x80;
        inline constexpr u8 SLC_FLUSHIN = 0x40;
        inline constexpr u8 SLC_FLUSHOUT = 0x20;

        inline constexpr u8 OLD_ENV_VAR = 1;
        inline constexpr u8 OLD_ENV_VALUE = 0;
        inline constexpr u8 NEW_ENV_VAR = 0;
        inline constexpr u8 NEW_ENV_VALUE = 1;
        inline constexpr u8 ENV_ESC = 2;
        inline constexpr u8 ENV_USERVAR = 3;

        // AUTHENTICATION suboptions

        // Who is authenticating who ...
        inline constexpr u8 AUTH_WHO_CLIENT = 0; ///< Client authenticating server
        inline constexpr u8 AUTH_WHO_SERVER = 1; ///< Server authenticating client
        inline constexpr u8 AUTH_WHO_MASK = 1;

        // amount of authentication done
        inline constexpr u8 AUTH_HOW_ONE_WAY = 0;
        inline constexpr u8 AUTH_HOW_MUTUAL = 2;
        inline constexpr u8 AUTH_HOW_MASK = 2;

        inline constexpr u8 AUTHTYPE_NULL = 0;
        inline constexpr u8 AUTHTYPE_KERBEROS_V4 = 1;
        inline constexpr u8 AUTHTYPE_KERBEROS_V5 = 2;
        inline constexpr u8 AUTHTYPE_SPX = 3;
        inline constexpr u8 AUTHTYPE_MINK = 4;
        inline constexpr u8 AUTHTYPE_CNT = 5;

        inline constexpr u8 AUTHTYPE_TEST = 99;

        constexpr bool AUTHTYPE_NAME_OK(u8 x) noexcept {
            return x < AUTHTYPE_CNT;
        }

        // encryption suboptions
        inline constexpr u8 ENCRYPT_IS = 0; ///< I pick encryption type ...
        inline constexpr u8 ENCRYPT_SUPPORT = 1; ///< I support encryption types ...
        inline constexpr u8 ENCRYPT_REPLY = 2; ///< Initial setup response
        inline constexpr u8 ENCRYPT_START = 3; ///< Am starting to send encrypted
        inline constexpr u8 ENCRYPT_END = 4; ///< Am ending encrypted
        inline constexpr u8 ENCRYPT_REQSTART = 5; ///< Request you start encrypting
        inline constexpr u8 ENCRYPT_REQEND = 6; ///< Request you send encrypting
        inline constexpr u8 ENCRYPT_ENC_KEYID = 7;
        inline constexpr u8 ENCRYPT_DEC_KEYID = 8;
        inline constexpr u8 ENCRYPT_CNT = 9;

        inline constexpr u8 ENCTYPE_ANY = 0;
        inline constexpr u8 ENCTYPE_DES_CFB64 = 1;
        inline constexpr u8 ENCTYPE_DES_OFB64 = 2;
        inline constexpr u8 ENCTYPE_CNT = 3;

        [[nodiscard]]
        constexpr bool ENCRYPT_NAME_OK(u8 x) noexcept {
            return x < ENCRYPT_CNT;
        }

        [[nodiscard]]
        constexpr bool ENCTYPE_NAME_OK(u8 x) noexcept {
            return x < ENCTYPE_CNT;
        }

        inline constexpr Array<StringView, 21> TelCmds = {
            "EOF", "SUSP", "ABORT", "EOR",
            "SE", "NOP", "DMARK", "BRK", "IP", "AO", "AYT", "EC",
            "EL", "GA", "SB", "WILL", "WONT", "DO", "DONT", "IAC",
        };

        inline constexpr Array<StringView, NTELOPTS> TelOptNames = {
            "BINARY", "ECHO", "RCP", "SUPPRESS GO AHEAD", "NAME",
            "STATUS", "TIMING MARK", "RCTE", "NAOL", "NAOP",
            "NAOCRD", "NAOHTS", "NAOHTD", "NAOFFD", "NAOVTS",
            "NAOVTD", "NAOLFD", "EXTEND ASCII", "LOGOUT", "BYTE MACRO",
            "DATA ENTRY TERMINAL", "SUPDUP", "SUPDUP OUTPUT",
            "SEND LOCATION", "TERMINAL TYPE", "END OF RECORD",
            "TACACS UID", "OUTPUT MARKING", "TTYLOC",
            "3270 REGIME", "X.3 PAD", "NAWS", "TSPEED", "LFLOW",
            "LINEMODE", "XDISPLOC", "OLD-ENVIRON", "AUTHENTICATION",
            "ENCRYPT", "NEW-ENVIRON",
        };

        inline constexpr Array<StringView, 20> SlcNames = {
            "0", "SYNCH", "BRK", "IP", "AO", "AYT", "EOR",
            "ABORT", "EOF", "SUSP", "EC", "EL", "EW", "RP",
            "LNEXT", "XON", "XOFF", "FORW1", "FORW2",
        };

        inline constexpr Array<StringView, 5> AuthTypeNames = {
            "NULL", "KERBEROS_V4", "KERBEROS_V5", "SPX", "MINK"
        };

        inline constexpr Array<StringView, 9> EncryptNames = {
            "IS", "SUPPORT", "REPLY", "START", "END",
            "REQUEST-START", "REQUEST-END", "ENC-KEYID", "DEC-KEYID",
        };

        inline constexpr Array<StringView, 3> EnctypeNames = {
            "ANY", "DES_CFB64",  "DES_OFB64",
        };
    }

    /**
     * @namespace asmgeneric
     * @brief Linux asm-generic error number operations.
     */
    namespace asmgeneric {
        /**
         * <asm-generic/errno.h> constants
         */

        inline constexpr i32 EPERM = 1; ///< Operation not permitted
        inline constexpr i32 ENOENT = 2; ///< No such file or directory
        inline constexpr i32 ESRCH = 3; ///< No such process
        inline constexpr i32 EINTR = 4; ///< Interrupted system call
        inline constexpr i32 EIO = 5; ///< I/O error
        inline constexpr i32 ENXIO = 6; ///< No such device or address
        inline constexpr i32 E2BIG = 7; ///< Argument list too i64
        inline constexpr i32 ENOEXEC = 8; ///< Exec format error
        inline constexpr i32 EBADF = 9; ///< Bad file number
        inline constexpr i32 ECHILD = 10; ///< No child processes
        inline constexpr i32 EAGAIN = 11; ///< Try again
        inline constexpr i32 ENOMEM = 12; ///< Out of memory
        inline constexpr i32 EACCES = 13; ///< Permission denied
        inline constexpr i32 EFAULT = 14; ///< Bad address
        inline constexpr i32 ENOTBLK = 15; ///< Block device required
        inline constexpr i32 EBUSY = 16; ///< Device or resource busy
        inline constexpr i32 EEXIST = 17; ///< File exists
        inline constexpr i32 EXDEV = 18; ///< Cross-device link
        inline constexpr i32 ENODEV = 19; ///< No such device
        inline constexpr i32 ENOTDIR = 20; ///< Not a directory
        inline constexpr i32 EISDIR = 21; ///< Is a directory
        inline constexpr i32 EINVAL = 22; ///< Invalid argument
        inline constexpr i32 ENFILE = 23; ///< File table overflow
        inline constexpr i32 EMFILE = 24; ///< Too many open files
        inline constexpr i32 ENOTTY = 25; ///< Not a typewriter
        inline constexpr i32 ETXTBSY = 26; ///< Text file busy
        inline constexpr i32 EFBIG = 27; ///< File too large
        inline constexpr i32 ENOSPC = 28; ///< No space left on device
        inline constexpr i32 ESPIPE = 29; ///< Illegal seek
        inline constexpr i32 EROFS = 30; ///< Read-only file system
        inline constexpr i32 EMLINK = 31; ///< Too many links
        inline constexpr i32 EPIPE = 32; ///< Broken pipe
        inline constexpr i32 EDOM = 33; ///< Math argument out of domain of func
        inline constexpr i32 ERANGE = 34; ///< Math result not representable
        inline constexpr i32 EDEADLK = 35; ///< Resource deadlock would occur
        inline constexpr i32 ENAMETOOLONG = 36; ///< File name too i64
        inline constexpr i32 ENOLCK = 37; ///< No record locks available
        inline constexpr i32 ENOSYS = 38; ///< Invalid system call number
        inline constexpr i32 ENOTEMPTY = 39; ///< Directory not empty
        inline constexpr i32 ELOOP = 40; ///< Too many symbolic links encountered
        inline constexpr i32 EWOULDBLOCK = EAGAIN; ///< Operation would block
        inline constexpr i32 ENOMSG = 42; ///< No message of desired type
        inline constexpr i32 EIDRM = 43; ///< Identifier removed
        inline constexpr i32 ECHRNG = 44; ///< Channel number out of range
        inline constexpr i32 EL2NSYNC = 45; ///< Level 2 not synchronized
        inline constexpr i32 EL3HLT = 46; ///< Level 3 halted
        inline constexpr i32 EL3RST = 47; ///< Level 3 reset
        inline constexpr i32 ELNRNG = 48; ///< Link number out of range
        inline constexpr i32 EUNATCH = 49; ///< Protocol driver not attached
        inline constexpr i32 ENOCSI = 50; ///< No CSI structure available
        inline constexpr i32 EL2HLT = 51; ///< Level 2 halted
        inline constexpr i32 EBADE = 52; ///< Invalid exchange
        inline constexpr i32 EBADR = 53; ///< Invalid request descriptor
        inline constexpr i32 EXFULL = 54; ///< Exchange full
        inline constexpr i32 ENOANO = 55; ///< No anode
        inline constexpr i32 EBADRQC = 56; ///< Invalid request code
        inline constexpr i32 EBADSLT = 57; ///< Invalid slot
        inline constexpr i32 EDEADLOCK = EDEADLK; ///< Alias for EDEADLK
        inline constexpr i32 EBFONT = 59; ///< Bad font file format
        inline constexpr i32 ENOSTR = 60; ///< Device not a stream
        inline constexpr i32 ENODATA = 61; ///< No data available
        inline constexpr i32 ETIME = 62; ///< Timer expired
        inline constexpr i32 ENOSR = 63; ///< Out of streams resources
        inline constexpr i32 ENONET = 64; ///< Machine is not on the network
        inline constexpr i32 ENOPKG = 65; ///< Package not installed
        inline constexpr i32 EREMOTE = 66; ///< Object is remote
        inline constexpr i32 ENOLINK = 67; ///< Link has been severed
        inline constexpr i32 EADV = 68; ///< Advertise error
        inline constexpr i32 ESRMNT = 69; ///< Srmount error
        inline constexpr i32 ECOMM = 70; ///< Communication error on send
        inline constexpr i32 EPROTO = 71; ///< Protocol error
        inline constexpr i32 EMULTIHOP = 72; ///< Multihop attempted
        inline constexpr i32 EDOTDOT = 73; ///< RFS specific error
        inline constexpr i32 EBADMSG = 74; ///< Not a data message
        inline constexpr i32 EOVERFLOW = 75; ///< Value too large for defined data type
        inline constexpr i32 ENOTUNIQ = 76; ///< Name not unique on network
        inline constexpr i32 EBADFD = 77; ///< File descriptor in bad state
        inline constexpr i32 EREMCHG = 78; ///< Remote address changed
        inline constexpr i32 ELIBACC = 79; ///< Cannot access a needed shared library
        inline constexpr i32 ELIBBAD = 80; ///< Accessing a corrupted shared library
        inline constexpr i32 ELIBSCN = 81; ///< .lib section in a.out corrupted
        inline constexpr i32 ELIBMAX = 82; ///< Too many shared libraries
        inline constexpr i32 ELIBEXEC = 83; ///< Cannot exec a shared library directly
        inline constexpr i32 EILSEQ = 84; ///< Illegal byte sequence
        inline constexpr i32 ERESTART = 85; ///< Interrupted system call should be restarted
        inline constexpr i32 ESTRPIPE = 86; ///< Streams pipe error
        inline constexpr i32 EUSERS = 87; ///< Too many users
        inline constexpr i32 ENOTSOCK = 88; ///< Socket operation on non-socket
        inline constexpr i32 EDESTADDRREQ = 89; ///< Destination address required
        inline constexpr i32 EMSGSIZE = 90; ///< Message too i64
        inline constexpr i32 EPROTOTYPE = 91; ///< Protocol wrong type for socket
        inline constexpr i32 ENOPROTOOPT = 92; ///< Protocol not available
        inline constexpr i32 EPROTONOSUPPORT = 93; ///< Protocol not supported
        inline constexpr i32 ESOCKTNOSUPPORT = 94; ///< Socket type not supported
        inline constexpr i32 EOPNOTSUPP = 95; ///< Operation not supported on transport endpoi32
        inline constexpr i32 EPFNOSUPPORT = 96; ///< Protocol family not supported
        inline constexpr i32 EAFNOSUPPORT = 97; ///< Address family not supported by protocol
        inline constexpr i32 EADDRINUSE = 98; ///< Address already in use
        inline constexpr i32 EADDRNOTAVAIL = 99; ///< Cannot assign requested address
        inline constexpr i32 ENETDOWN = 100; ///< Network is down
        inline constexpr i32 ENETUNREACH = 101; ///< Network is unreachable
        inline constexpr i32 ENETRESET = 102; ///< Network dropped connection because of reset
        inline constexpr i32 ECONNABORTED = 103; ///< Software caused connection abort
        inline constexpr i32 ECONNRESET = 104; ///< Connection reset by peer
        inline constexpr i32 ENOBUFS = 105; ///< No buffer space available
        inline constexpr i32 EISCONN = 106; ///< Transport endpoi32 is already connected
        inline constexpr i32 ENOTCONN = 107; ///< Transport endpoi32 is not connected
        inline constexpr i32 ESHUTDOWN = 108; ///< Cannot send after transport endpoi32 shutdown
        inline constexpr i32 ETOOMANYREFS = 109; ///< Too many references: cannot splice
        inline constexpr i32 ETIMEDOUT = 110; ///< Connection timed out
        inline constexpr i32 ECONNREFUSED = 111; ///< Connection refused
        inline constexpr i32 EHOSTDOWN = 112; ///< Host is down
        inline constexpr i32 EHOSTUNREACH = 113; ///< No route to host
        inline constexpr i32 EALREADY = 114; ///< Operation already in progress
        inline constexpr i32 EINPROGRESS = 115; ///< Operation now in progress
        inline constexpr i32 ESTALE = 116; ///< Stale file handle
        inline constexpr i32 EUCLEAN = 117; ///< Structure needs cleaning
        inline constexpr i32 ENOTNAM = 118; ///< Not a XENIX named type file
        inline constexpr i32 ENAVAIL = 119; ///< No XENIX semaphores available
        inline constexpr i32 EISNAM = 120; ///< Is a named type file
        inline constexpr i32 EREMOTEIO = 121; ///< Remote I/O error
        inline constexpr i32 EDQUOT = 122; ///< Quota exceeded
        inline constexpr i32 ENOMEDIUM = 123; ///< No medium found
        inline constexpr i32 EMEDIUMTYPE = 124; ///< Wrong medium type
        inline constexpr i32 ECANCELED = 125; ///< Operation Canceled
        inline constexpr i32 ENOKEY = 126; ///< Required key not available
        inline constexpr i32 EKEYEXPIRED = 127; ///< Key has expired
        inline constexpr i32 EKEYREVOKED = 128; ///< Key has been revoked
        inline constexpr i32 EKEYREJECTED = 129; ///< Key was rejected by service
        inline constexpr i32 EOWNERDEAD = 130; ///< Owner died
        inline constexpr i32 ENOTRECOVERABLE = 131; ///< State not recoverable
        inline constexpr i32 ERFKILL = 132; ///< Operation not possible due to RF-kill
        inline constexpr i32 EHWPOISON = 133; ///< Memory page has hardware error
    }

    /**
     * @namespace sys
     * @brief Linux system operations.
     */
    namespace sys {
        /**
         * <sys/capability.h> constants
         */

        enum class CapabilityFlag: u8 {
            EFFECTIVE = 0, ///< Specifies the effective flag
            PERMITTED = 1, ///< Specifies the permitted flag
            INHERITABLE = 2 ///< Specifies the inheritable flag
        };

        enum class CapabilityIABVector: u8 {
            INH = 2,
            AMB = 3,
            BOUND = 4
        };

        enum class CapFlagValue: u8 {
            CLEAR = 0, ///< The flag is cleared/disabled
            SET = 1 ///< The flag is set/enabled
        };

        /**
         * <sys/epoll.h> constants
         */

        enum class EpollEvents: u32 {
            IN = 0x001,
            PRI = 0x002,
            OUT = 0x004,
            RD_NORM = 0x040,
            RD_BAND = 0x080,
            WR_NORM = 0x100,
            WR_BAND = 0x200,
            MSG = 0x400,
            ERR = 0x008,
            HUP = 0x010,
            RD_HUP = 0x2000,
            EXCLUSIVE = 1u << 28,
            WAKEUP = 1u << 29,
            ONE_SHOT = 1u << 30,
            EDGE_TRIGGERED = 1u << 31
        };

        /**
         * <sys/inotify.h> constants
         */

        inline constexpr i32 IN_CLOEXEC = 02000000;
        inline constexpr i32 IN_NONBLOCK = 00004000;
    }
}
