#pragma once

/**
 * @namespace stdx::os::darwin::mach
 * @brief The task, thread, and host ports of the calling process - <mach/mach_init.h>.
 */
export namespace stdx::os::darwin::mach {
    #if defined(__APPLE__) && __has_include(<mach/mach_init.h>)
    // mach_task_self() and current_task() are macros over the mach_task_self_
    // global, so the global is what a using-declaration can name.
    using ::mach_task_self_;

    using ::mach_host_self;
    using ::mach_thread_self;
    using ::mach_task_is_self;
    using ::host_page_size;
    #endif
}
