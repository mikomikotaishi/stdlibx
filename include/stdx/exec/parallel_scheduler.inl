#pragma once

/**
 * @namespace std::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    #if defined(__cpp_lib_parallel_scheduler) && !defined(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    using ParallelScheduler = std::execution::parallel_scheduler;

    using std::execution::Async::parallel_scheduler;

    namespace replaceability {
        using ReceiverProxy = std::execution::system_context_replaceability::receiver_proxy;
        using BulkItemReceiverProxy = std::execution::system_context_replaceability::bulk_item_receiver_proxy;
        using ParallelSchedulerBackend = std::execution::system_context_replaceability::parallel_scheduler_backend;

        using std::execution::system_context_replaceability::query_parallel_scheduler_backend;
    }
    #endif
}
