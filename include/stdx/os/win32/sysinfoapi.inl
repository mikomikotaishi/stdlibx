#pragma once

/**
 * @namespace stdx::os::win32
 * @brief System information, computer name, and OS version (sysinfoapi.h).
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<sysinfoapi.h>)
    // System information
    using SystemInformation = ::SYSTEM_INFO;
    using SystemInformationPointer = ::LPSYSTEM_INFO;

    using ::GetSystemInfo;
    using ::GetNativeSystemInfo;

    /**
     * @struct ProcessorArchitecture
     * @brief The <winnt.h> PROCESSOR_ARCHITECTURE_* values, renamed from their
     * macros so they can be named through the win32 namespace at the use site
     * (referencing the macro name there would expand to its literal value).
     */
    class ProcessorArchitecture final {
    public:
        static constexpr WinWord AMD64 = PROCESSOR_ARCHITECTURE_AMD64;
        static constexpr WinWord ARM = PROCESSOR_ARCHITECTURE_ARM;
        static constexpr WinWord ARM64 = PROCESSOR_ARCHITECTURE_ARM64;
        static constexpr WinWord INTEL = PROCESSOR_ARCHITECTURE_INTEL;
        static constexpr WinWord UNKNOWN = PROCESSOR_ARCHITECTURE_UNKNOWN;

        ProcessorArchitecture() noexcept = delete("ProcessorArchitecture is a static utility class and cannot be instantiated.");
    };

    // Computer name
    using ComputerNameFormat = ::COMPUTER_NAME_FORMAT;
    inline constexpr ComputerNameFormat ComputerNamePhysicalDnsHostname = ::ComputerNamePhysicalDnsHostname;

    using ::GetComputerNameExA;
    using ::GetComputerNameExW;

    // OS version
    using OsVersionInfoW = ::OSVERSIONINFOW;

    /**
     * @brief The true OS version, via ntdll!RtlGetVersion.
     *
     * RtlGetVersion has no public SDK header, so it is resolved from ntdll at
     * runtime. Unlike GetVersionExW it is not capped by the process's
     * compatibility manifest, so it reports the real running version.
     * @param info The structure to fill; its dwOSVersionInfoSize is set here.
     * @return true on success (STATUS_SUCCESS); false if ntdll or the entry
     * point could not be resolved, or the call failed.
     */
    [[nodiscard]]
    inline bool RtlGetVersion(OsVersionInfoW& info) noexcept {
        using Fn = long(__stdcall*)(OsVersionInfoW*);
        if (ModuleHandle ntdll = GetModuleHandleW(L"ntdll.dll")) {
            if (Fn rtl_get_version = reinterpret_cast<Fn>(GetProcAddress(ntdll, "RtlGetVersion"))) {
                info = OsVersionInfoW{};
                info.dwOSVersionInfoSize = sizeof(info);
                return rtl_get_version(&info) == 0; // STATUS_SUCCESS
            }
        }
        return false;
    }
    #endif
}
