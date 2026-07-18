#pragma once

/**
 * @namespace std::exec
 * @brief Standard library execution operations.
 */
export namespace stdx::exec {
    #if defined(__cpp_lib_task) && !defined(STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)
    template <typename T, typename Envir = Env<>>
    using Task = std::execution::task<T, Envir>;

    using TaskScheduler = std::execution::task_scheduler;
    #endif
}
