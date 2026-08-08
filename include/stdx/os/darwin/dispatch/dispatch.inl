#pragma once

/**
 * @namespace stdx::os::darwin::dispatch
 * @brief Grand Central Dispatch - <dispatch/dispatch.h>.
 *
 * dispatch_get_main_queue and the DISPATCH_TIME_ constants are macros, so they
 * live in stdx::os::darwin::dispatch in darwin.constants.inl instead.
 * dispatch_once and dispatch_once_f are likewise macros, over a header-inline
 * fast path whose internal linkage a module may not re-export; reach for
 * std::call_once or a function-local static in preference to either.
 */
export namespace stdx::os::darwin::dispatch {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<dispatch/dispatch.h>)
    using Object = ::dispatch_object_t;
    using Queue = ::dispatch_queue_t;
    using QueueAttributes = ::dispatch_queue_attr_t;
    using Group = ::dispatch_group_t;
    using Semaphore = ::dispatch_semaphore_t;
    using Source = ::dispatch_source_t;
    using SourceType = ::dispatch_source_type_t;
    using Data = ::dispatch_data_t;
    using IoChannel = ::dispatch_io_t;
    using Time = ::dispatch_time_t;
    using Once = ::dispatch_once_t;
    using Function = ::dispatch_function_t;
    using Block = ::dispatch_block_t;

    using ::dispatch_queue_create;
    using ::dispatch_get_global_queue;
    using ::dispatch_queue_get_label;
    using ::dispatch_main;
    using ::dispatch_async;
    using ::dispatch_async_f;
    using ::dispatch_sync;
    using ::dispatch_sync_f;
    using ::dispatch_after;
    using ::dispatch_after_f;
    using ::dispatch_apply;
    using ::dispatch_apply_f;
    using ::dispatch_time;
    using ::dispatch_walltime;
    using ::dispatch_retain;
    using ::dispatch_release;
    using ::dispatch_suspend;
    using ::dispatch_resume;
    using ::dispatch_set_target_queue;
    using ::dispatch_group_create;
    using ::dispatch_group_async;
    using ::dispatch_group_enter;
    using ::dispatch_group_leave;
    using ::dispatch_group_wait;
    using ::dispatch_group_notify;
    using ::dispatch_semaphore_create;
    using ::dispatch_semaphore_signal;
    using ::dispatch_semaphore_wait;
    using ::dispatch_source_create;
    using ::dispatch_source_set_event_handler;
    using ::dispatch_source_set_cancel_handler;
    using ::dispatch_source_set_timer;
    using ::dispatch_source_cancel;
    using ::dispatch_source_get_data;
    using ::dispatch_source_get_handle;
    using ::dispatch_source_merge_data;
    #endif
}
