#pragma once

/**
 * @namespace stdx::os::linux::alsa
 * @brief ALSA MIDI sequencer (snd_seq) - alsa/seq.h wrappers.
 */
export namespace stdx::os::linux::alsa {
    #if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
    using SoundSequencer = ::snd_seq_t;
    using SoundSequencerSystemInfo = ::snd_seq_system_info_t;
    using SoundSequencerClientInfo = ::snd_seq_client_info_t;
    using SoundSequencerClientPool = ::snd_seq_client_pool_t;
    using SoundSequencerPortInfo = ::snd_seq_port_info_t;
    using SoundSequencerPortSubscribe = ::snd_seq_port_subscribe_t;
    using SoundSequencerQuerySubscribe = ::snd_seq_query_subscribe_t;
    using SoundSequencerQueueInfo = ::snd_seq_queue_info_t;
    using SoundSequencerQueueStatus = ::snd_seq_queue_status_t;
    using SoundSequencerQueueTempo = ::snd_seq_queue_tempo_t;
    using SoundSequencerQueueTimer = ::snd_seq_queue_timer_t;
    using SoundSequencerRemoveEvents = ::snd_seq_remove_events_t;
    using SoundSequencerType = ::snd_seq_type_t;
    using SoundSequencerClientType = ::snd_seq_client_type_t;

    // Lifecycle
    using ::snd_seq_open;
    using ::snd_seq_open_lconf;
    using ::snd_seq_type;
    using ::snd_seq_close;
    using ::snd_seq_name;
    using ::snd_seq_nonblock;
    using ::snd_seq_client_id;
    using ::snd_seq_get_input_buffer_size;
    using ::snd_seq_set_input_buffer_size;
    using ::snd_seq_get_output_buffer_size;
    using ::snd_seq_set_output_buffer_size;

    // Poll
    using ::snd_seq_poll_descriptors;
    using ::snd_seq_poll_descriptors_count;
    using ::snd_seq_poll_descriptors_revents;

    // System info
    using ::snd_seq_system_info;
    using ::snd_seq_system_info_malloc;
    using ::snd_seq_system_info_free;
    using ::snd_seq_system_info_get_queues;
    using ::snd_seq_system_info_get_clients;
    using ::snd_seq_system_info_get_ports;
    using ::snd_seq_system_info_get_channels;

    // Client info
    using ::snd_seq_client_info_malloc;
    using ::snd_seq_client_info_free;
    using ::snd_seq_get_client_info;
    using ::snd_seq_set_client_info;
    using ::snd_seq_get_any_client_info;
    using ::snd_seq_query_next_client;
    using ::snd_seq_set_client_name;
    using ::snd_seq_client_info_get_client;
    using ::snd_seq_client_info_get_name;
    using ::snd_seq_client_info_get_broadcast_filter;
    using ::snd_seq_client_info_get_error_bounce;
    using ::snd_seq_client_info_get_num_ports;
    using ::snd_seq_client_info_set_client;
    using ::snd_seq_client_info_set_name;

    // Port info
    using ::snd_seq_port_info_malloc;
    using ::snd_seq_port_info_free;
    using ::snd_seq_create_port;
    using ::snd_seq_delete_port;
    using ::snd_seq_get_port_info;
    using ::snd_seq_set_port_info;
    using ::snd_seq_get_any_port_info;
    using ::snd_seq_query_next_port;
    using ::snd_seq_port_info_get_client;
    using ::snd_seq_port_info_get_port;
    using ::snd_seq_port_info_get_name;
    using ::snd_seq_port_info_get_capability;
    using ::snd_seq_port_info_get_type;
    using ::snd_seq_port_info_get_midi_channels;
    using ::snd_seq_port_info_get_direction;
    using ::snd_seq_port_info_set_client;
    using ::snd_seq_port_info_set_port;
    using ::snd_seq_port_info_set_name;
    using ::snd_seq_port_info_set_capability;
    using ::snd_seq_port_info_set_type;

    // Subscriptions
    using ::snd_seq_port_subscribe_malloc;
    using ::snd_seq_port_subscribe_free;
    using ::snd_seq_get_port_subscription;
    using ::snd_seq_subscribe_port;
    using ::snd_seq_unsubscribe_port;
    using ::snd_seq_query_port_subscribers;

    // Queue management
    using ::snd_seq_alloc_queue;
    using ::snd_seq_alloc_named_queue;
    using ::snd_seq_free_queue;
    using ::snd_seq_get_queue_info;
    using ::snd_seq_set_queue_info;
    using ::snd_seq_query_named_queue;
    using ::snd_seq_get_queue_status;
    using ::snd_seq_get_queue_tempo;
    using ::snd_seq_set_queue_tempo;
    using ::snd_seq_get_queue_timer;
    using ::snd_seq_set_queue_timer;
    using ::snd_seq_drain_output;

    // Event I/O
    using ::snd_seq_event_input;
    using ::snd_seq_event_input_pending;
    using ::snd_seq_event_output;
    using ::snd_seq_event_output_buffer;
    using ::snd_seq_event_output_direct;
    using ::snd_seq_event_output_pending;
    using ::snd_seq_drop_output;
    using ::snd_seq_drop_output_buffer;
    using ::snd_seq_drop_input;
    using ::snd_seq_drop_input_buffer;
    using ::snd_seq_remove_events;
    using ::snd_seq_extract_output;
    #endif
}
