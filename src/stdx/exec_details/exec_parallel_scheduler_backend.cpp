/**
 * @file exec_parallel_scheduler_backend.cpp
 * @brief Single translation unit providing NVIDIA stdexec's default
 *        parallel-scheduler backend.
 *
 * This lives in its own plain (non-module) TU on purpose: the alternative,
 * STDEXEC_PARALLEL_SCHEDULER_HEADER_ONLY in exec.cppm, bakes the same backend
 * into the stdx:exec module BMI, which crashes clang's module AST reader and
 * forces every consumer to emit the backend inline. Isolating it here keeps
 * the heavy static_thread_pool machinery out of the module entirely.
 */
#include <stdexec/execution.hpp>

#define STDEXEC_PARALLEL_SCHEDULER_INLINE
#include <stdexec/__detail/__parallel_scheduler_default_impl_entry.hpp>
