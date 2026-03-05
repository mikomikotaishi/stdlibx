#pragma once

/**
 * @namespace stdx::os::linux
 * @brief Wrapper namespace for Linux operations.
 */
export namespace stdx::os::linux {
    #ifdef __linux__
    using KernelFileDescriptorSet = ::__kernel_fd_set;
    using KernelSignalHandler = ::__kernel_sighandler_t;
    using KernelKey = ::__kernel_key_t;
    using KernelMqd = ::__kernel_mqd_t;
    #endif
}
