/**
 * @file process.cppm
 * @module stdx:process
 * @brief Import of process handling modules.
 *
 * This file imports the modules for the process handling system.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:process;
#else
export module stdlibx:process;
#endif

export import :process.Child;
export import :process.ChildStderr;
export import :process.ChildStdin;
export import :process.ChildStdout;
export import :process.Command;
export import :process.CommandArgs;
export import :process.CommandEnvs;
export import :process.ExitCode;
export import :process.ExitStatus;
export import :process.ExitStatusError;
export import :process.Output;
export import :process.Stdio;
