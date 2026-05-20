#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Synchronization primitives (synchapi.h): mutexes, events, semaphores,
 *        critical sections, slim reader/writer locks, condition variables,
 *        wait functions, init-once.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // Lock and CV primitives
    using CriticalSection = ::CRITICAL_SECTION;
    using CriticalSectionPointer = ::LPCRITICAL_SECTION;
    using CriticalSectionDebug = ::CRITICAL_SECTION_DEBUG;
    using SrwLock = ::SRWLOCK;
    using SrwLockPointer = ::PSRWLOCK;
    using ConditionVariable = ::CONDITION_VARIABLE;
    using ConditionVariablePointer = ::PCONDITION_VARIABLE;
    using InitOnce = ::INIT_ONCE;
    using InitOncePointer = ::PINIT_ONCE;
    using InitOnceFn = ::PINIT_ONCE_FN;

    // Wait callback / TP
    using WaitCallback = ::WAITORTIMERCALLBACK;
    using WaitCallbackRoutine = ::WAITORTIMERCALLBACKFUNC;

    // Mutex / event / semaphore
    using ::CreateMutexA;
    using ::CreateMutexW;
    using ::CreateMutexExA;
    using ::CreateMutexExW;
    using ::OpenMutexA;
    using ::OpenMutexW;
    using ::ReleaseMutex;

    using ::CreateEventA;
    using ::CreateEventW;
    using ::CreateEventExA;
    using ::CreateEventExW;
    using ::OpenEventA;
    using ::OpenEventW;
    using ::SetEvent;
    using ::ResetEvent;
    using ::PulseEvent;

    using ::CreateSemaphoreA;
    using ::CreateSemaphoreW;
    using ::CreateSemaphoreExA;
    using ::CreateSemaphoreExW;
    using ::OpenSemaphoreA;
    using ::OpenSemaphoreW;
    using ::ReleaseSemaphore;

    using ::CreateWaitableTimerA;
    using ::CreateWaitableTimerW;
    using ::CreateWaitableTimerExA;
    using ::CreateWaitableTimerExW;
    using ::OpenWaitableTimerA;
    using ::OpenWaitableTimerW;
    using ::SetWaitableTimer;
    using ::SetWaitableTimerEx;
    using ::CancelWaitableTimer;

    // Wait functions
    using ::WaitForSingleObject;
    using ::WaitForSingleObjectEx;
    using ::WaitForMultipleObjects;
    using ::WaitForMultipleObjectsEx;
    using ::SignalObjectAndWait;
    using ::MsgWaitForMultipleObjects;
    using ::MsgWaitForMultipleObjectsEx;

    // Critical sections
    using ::InitializeCriticalSection;
    using ::InitializeCriticalSectionAndSpinCount;
    using ::InitializeCriticalSectionEx;
    using ::EnterCriticalSection;
    using ::TryEnterCriticalSection;
    using ::LeaveCriticalSection;
    using ::DeleteCriticalSection;
    using ::SetCriticalSectionSpinCount;

    // SRW locks
    using ::InitializeSRWLock;
    using ::AcquireSRWLockExclusive;
    using ::AcquireSRWLockShared;
    using ::ReleaseSRWLockExclusive;
    using ::ReleaseSRWLockShared;
    using ::TryAcquireSRWLockExclusive;
    using ::TryAcquireSRWLockShared;

    // Condition variables
    using ::InitializeConditionVariable;
    using ::WakeConditionVariable;
    using ::WakeAllConditionVariable;
    using ::SleepConditionVariableCS;
    using ::SleepConditionVariableSRW;

    // One-time init
    using ::InitOnceInitialize;
    using ::InitOnceExecuteOnce;
    using ::InitOnceBeginInitialize;
    using ::InitOnceComplete;
    #endif
}
