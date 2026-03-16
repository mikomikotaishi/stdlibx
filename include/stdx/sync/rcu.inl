#pragma once

using stdx::alloc::DefaultDelete;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    #ifdef __cpp_lib_rcu
    template <typename T, typename Del = DefaultDelete<T>>
    using RcuObject = std::rcu_object_base;

    using RcuDomain = std::rcu_domain;

    using std::rcu_default_domain;
    using std::rcu_synchronize;
    using std::rcu_barrier;
    using std::rcu_retire;
    #endif
}
