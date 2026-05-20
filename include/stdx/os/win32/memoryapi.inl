#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Memory management (memoryapi.h + heapapi.h):
 *        VirtualAlloc/Free/Protect/Query, HeapAlloc, Global/Local heap.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    using MemoryBasicInformation = ::MEMORY_BASIC_INFORMATION;
    using MemoryBasicInformationPointer = ::PMEMORY_BASIC_INFORMATION;
    using ProcessHeapEntry = ::PROCESS_HEAP_ENTRY;
    using ProcessHeapEntryPointer = ::LPPROCESS_HEAP_ENTRY;
    using HeapSummary = ::HEAP_SUMMARY;
    using HeapSummaryPointer = ::PHEAP_SUMMARY;

    // Virtual memory
    using ::VirtualAlloc;
    using ::VirtualAllocEx;
    using ::VirtualFree;
    using ::VirtualFreeEx;
    using ::VirtualProtect;
    using ::VirtualProtectEx;
    using ::VirtualQuery;
    using ::VirtualQueryEx;
    using ::VirtualLock;
    using ::VirtualUnlock;

    // File mapping
    using ::CreateFileMappingA;
    using ::CreateFileMappingW;
    using ::OpenFileMappingA;
    using ::OpenFileMappingW;
    using ::MapViewOfFile;
    using ::MapViewOfFileEx;
    using ::UnmapViewOfFile;
    using ::FlushViewOfFile;

    // Process heap
    using ::GetProcessHeap;
    using ::GetProcessHeaps;

    // Heap
    using ::HeapCreate;
    using ::HeapDestroy;
    using ::HeapAlloc;
    using ::HeapReAlloc;
    using ::HeapFree;
    using ::HeapSize;
    using ::HeapValidate;
    using ::HeapCompact;
    using ::HeapLock;
    using ::HeapUnlock;
    using ::HeapWalk;
    using ::HeapQueryInformation;
    using ::HeapSetInformation;

    // Global heap (legacy but still used). Note: ::GlobalHandle (function)
    // is shadowed by the GlobalHandle type alias in windows_base.inl —
    // call it qualified as ::GlobalHandle(...).
    using ::GlobalAlloc;
    using ::GlobalReAlloc;
    using ::GlobalFree;
    using ::GlobalLock;
    using ::GlobalUnlock;
    using ::GlobalSize;
    using ::GlobalFlags;

    // Local heap (legacy). ::LocalHandle (function) likewise shadowed.
    using ::LocalAlloc;
    using ::LocalReAlloc;
    using ::LocalFree;
    using ::LocalLock;
    using ::LocalUnlock;
    using ::LocalSize;
    using ::LocalFlags;

    // Read/write across processes
    using ::ReadProcessMemory;
    using ::WriteProcessMemory;

    // Working set
    using ::SetProcessWorkingSetSize;
    using ::GetProcessWorkingSetSize;
    using ::EmptyWorkingSet;
    #endif
}
