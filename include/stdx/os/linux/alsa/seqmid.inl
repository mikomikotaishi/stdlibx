#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA sequencer middle-level helpers — alsa/seqmid.h.
 *        Convenience functions over the raw seq.h API.
 */
export namespace stdx::os::linux::alsa {
    #ifdef __linux__
    #if __has_include(<alsa/asoundlib.h>)
    // Port creation / connection helpers
    using ::snd_seq_create_simple_port;
    using ::snd_seq_delete_simple_port;
    using ::snd_seq_connect_from;
    using ::snd_seq_connect_to;
    using ::snd_seq_disconnect_from;
    using ::snd_seq_disconnect_to;

    // Queue control
    using ::snd_seq_control_queue;
    using ::snd_seq_start_queue;
    using ::snd_seq_stop_queue;
    using ::snd_seq_continue_queue;
    using ::snd_seq_change_queue_tempo;
    using ::snd_seq_sync_output_queue;

    // Naming
    using ::snd_seq_set_client_name;
    using ::snd_seq_set_client_event_filter;
    using ::snd_seq_set_client_pool_output;
    using ::snd_seq_set_client_pool_output_room;
    using ::snd_seq_set_client_pool_input;

    // Event composition
    using ::snd_seq_parse_address;
    using ::snd_seq_reset_pool_output;
    using ::snd_seq_reset_pool_input;
    #endif
    #endif
}
