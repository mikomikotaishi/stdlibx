#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Process / thread management (processthreadsapi.h + tlhelp32.h).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<processthreadsapi.h>)
    // Process / thread startup
    using StartupInfo = ::STARTUPINFOA;
    using StartupInfoW = ::STARTUPINFOW;
    using StartupInfoEx = ::STARTUPINFOEXA;
    using StartupInfoExW = ::STARTUPINFOEXW;
    using StartupInfoPointer = ::LPSTARTUPINFOA;
    using StartupInfoWPointer = ::LPSTARTUPINFOW;
    using ProcessInformation = ::PROCESS_INFORMATION;
    using ProcessInformationPointer = ::LPPROCESS_INFORMATION;
    using ThreadStartRoutine = ::LPTHREAD_START_ROUTINE;

    // Tool Help snapshot entries
    using ProcessEntry32 = ::PROCESSENTRY32;
    using ProcessEntry32W = ::PROCESSENTRY32W;
    using ProcessEntry32Pointer = ::LPPROCESSENTRY32;
    using ProcessEntry32WPointer = ::LPPROCESSENTRY32W;
    using ThreadEntry32 = ::THREADENTRY32;
    using ThreadEntry32Pointer = ::LPTHREADENTRY32;
    using ModuleEntry32 = ::MODULEENTRY32;
    using ModuleEntry32W = ::MODULEENTRY32W;
    using ModuleEntry32Pointer = ::LPMODULEENTRY32;
    using ModuleEntry32WPointer = ::LPMODULEENTRY32W;
    using HeapList32 = ::HEAPLIST32;
    using HeapEntry32 = ::HEAPENTRY32;

    // Process creation / termination
    using ::CreateProcessA;
    using ::CreateProcessW;
    using ::CreateProcessAsUserA;
    using ::CreateProcessAsUserW;
    using ::OpenProcess;
    using ::TerminateProcess;
    using ::ExitProcess;
    using ::GetExitCodeProcess;
    using ::GetCurrentProcess;
    using ::GetCurrentProcessId;
    using ::GetProcessId;
    using ::GetProcessTimes;
    using ::GetProcessHandleCount;
    using ::FlushInstructionCache;

    // Job objects (parent-death linkage via kill-on-close)
    using ::CreateJobObjectW;
    using ::AssignProcessToJobObject;
    using ::SetInformationJobObject;

    using JobObjectBasicLimitInformation = ::JOBOBJECT_BASIC_LIMIT_INFORMATION;
    using JobObjectExtendedLimitInformation = ::JOBOBJECT_EXTENDED_LIMIT_INFORMATION;
    using JobObjectInfoClass = ::JOBOBJECTINFOCLASS;

    /// JOBOBJECTINFOCLASS value that selects JOBOBJECT_EXTENDED_LIMIT_INFORMATION.
    inline constexpr JobObjectInfoClass JobObjectExtendedLimitInfoClass = ::JobObjectExtendedLimitInformation;
    /// Limit flag (a <winnt.h> macro): kill every process in the job when its last handle closes.
    inline constexpr WinDWord JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE_FLAG = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    /// CreateProcess flag (a <winbase.h> macro): start the primary thread suspended.
    inline constexpr WinDWord CREATE_SUSPENDED_FLAG = CREATE_SUSPENDED;

    // Thread creation / control
    using ::CreateThread;
    using ::CreateRemoteThread;
    using ::ExitThread;
    using ::TerminateThread;
    using ::GetExitCodeThread;
    using ::GetCurrentThread;
    using ::GetCurrentThreadId;
    using ::GetThreadId;
    using ::SuspendThread;
    using ::ResumeThread;
    using ::SwitchToThread;
    using ::GetThreadTimes;
    using ::SetThreadPriority;
    using ::GetThreadPriority;
    using ::SetThreadPriorityBoost;
    using ::GetThreadPriorityBoost;
    using ::SetThreadAffinityMask;
    using ::SetThreadDescription;
    using ::GetThreadDescription;
    using ::Sleep;
    using ::SleepEx;
    using ::QueueUserAPC;

    // Tool Help snapshots
    using ::CreateToolhelp32Snapshot;
    using ::Process32First;
    using ::Process32Next;
    using ::Process32FirstW;
    using ::Process32NextW;
    using ::Thread32First;
    using ::Thread32Next;
    using ::Module32First;
    using ::Module32Next;
    using ::Module32FirstW;
    using ::Module32NextW;
    using ::Heap32ListFirst;
    using ::Heap32ListNext;
    using ::Heap32First;
    using ::Heap32Next;
    #endif
}
