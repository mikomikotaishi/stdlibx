#pragma once

using stdx::collections::HashMap;
using stdx::fmt::FormatContext;
using stdx::fmt::Formatter;
using stdx::time::Duration;
using stdx::time::Instant;

using namespace stdx::os;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @class Environment
     * @brief Cross-platform access to the process's environment information.
     */
    class Environment final {
    public:
        enum class OperatingSystem: u8 {
            UNKNOWN = 0, ///< An unknown or undetectable OS.
            UNIX, ///< Any Unix-like operating system (Linux, BSDs, macOS, etc.).
            WINDOWS, ///< Microsoft Windows.
            LINUX, ///< Linux specifically, as opposed to other Unix-like operating systems.
            FREEBSD, ///< FreeBSD specifically.
            DARWIN, ///< Apple operating systems (macOS, iOS, ...).
        };

        /**
         * @struct SystemInfo
         * @brief The running system's identity, as reported by the OS at runtime.
         */
        struct SystemInfo {
            String name; ///< OS name: "Linux", "Darwin", "Windows". Always present.
            String release; ///< Unix: kernel release. Windows: OS build (major.minor.build).
            String version; ///< Unix: kernel build string. Windows: display version (may be empty).
            String machine; ///< CPU architecture: "x86_64", "aarch64", ... Empty if unknown.
            String node; ///< Network host (node) name.
        };

        Environment() = delete("Environment is a static utility class and cannot be instantiated.");

        #if defined(__GNUC__) && !defined(__clang__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Winterference-size"
        #endif
        static constexpr usize HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE = std::hardware_constructive_interference_size;
        static constexpr usize HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE = std::hardware_destructive_interference_size;
        #if defined(__GNUC__) && !defined(__clang__)
        #pragma GCC diagnostic pop
        #endif

        /**
         * @brief The OS process ID of the current process.
         * @return u32 The OS process ID.
         */
        [[nodiscard]]
        static u32 pid() noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            return static_cast<u32>(unix::getpid());
            #elifdef _WIN32
            return static_cast<u32>(win32::GetCurrentProcessId());
            #else
            return 0;
            #endif
        }

        [[noreturn]]
        static void abort() noexcept {
            std::abort();
        }

        [[noreturn]]
        static void exit(i32 status) noexcept {
            std::exit(status);
        }

        [[noreturn]]
        static void quick_exit(i32 status) noexcept {
            std::quick_exit(status);
        }

        [[noreturn]]
        static void immediate_exit(i32 status) noexcept {
            std::exit(status);
        }

        static i32 exec(StringView command) noexcept {
            return std::system(command.data());
        }

        [[nodiscard]]
        static OperatingSystem operating_system() noexcept {
            #ifdef __linux__
            return OperatingSystem::LINUX;
            #elifdef __FreeBSD__
            return OperatingSystem::FREEBSD;
            #elifdef __APPLE__
            return OperatingSystem::DARWIN;
            #elifdef _WIN32
            return OperatingSystem::WINDOWS;
            #elifdef __unix__
            return OperatingSystem::UNIX;
            #else
            return OperatingSystem::UNKNOWN;
            #endif
        }

        /**
         * @brief The running system's identity (name, kernel/build version, arch, host).
         * @return The system info, or nullopt if the OS query itself failed.
         * Individual fields are empty where the platform cannot supply them.
         *
         * Reads the live system via uname(2) on Unix and RtlGetVersion +
         * GetNativeSystemInfo on Windows - information that, unlike @ref
         * operating_system, cannot be resolved at compile time.
         */
        [[nodiscard]]
        static Optional<SystemInfo> system_info() {
            #if defined(__unix__) || defined(__APPLE__)
            unix::sys::UnixTimeSystemName uts{};
            if (unix::sys::uname(&uts) != 0) {
                return nullopt;
            }
            return SystemInfo {
                .name = uts.sysname,
                .release = uts.release,
                .version = uts.version,
                .machine = uts.machine,
                .node = uts.nodename,
            };
            #elifdef _WIN32
            SystemInfo info;
            info.name = "Windows";

            // CPU architecture from the native system info.
            win32::SystemInformation sysinfo{};
            win32::GetNativeSystemInfo(&sysinfo);
            switch (sysinfo.wProcessorArchitecture) {
                case win32::ProcessorArchitecture::AMD64:
                    info.machine = "x86_64";
                    break;
                case win32::ProcessorArchitecture::ARM64:
                    info.machine = "aarch64";
                    break;
                case win32::ProcessorArchitecture::ARM:
                    info.machine = "arm";
                    break;
                case win32::ProcessorArchitecture::INTEL:
                    info.machine = "x86";
                    break;
                default:
                    break; // leave empty: unknown architecture
            }

            // Host name (physical DNS host name), ANSI to avoid a wide->UTF-8 hop.
            char host[256];
            win32::WinDWord host_len = sizeof(host);
            if (win32::GetComputerNameExA(win32::ComputerNamePhysicalDnsHostname, host, &host_len)) {
                info.node = StringView(host, host_len);
            }

            // True OS version via ntdll!RtlGetVersion: unlike GetVersionEx it is
            // not capped by the process's compatibility manifest.
            win32::OsVersionInfoW osvi{};
            if (win32::RtlGetVersion(osvi)) {
                info.release = std::format(
                    "{}.{}.{}",
                    osvi.dwMajorVersion,
                    osvi.dwMinorVersion,
                    osvi.dwBuildNumber
                );
            }
            // version left empty: the friendly display version ("23H2") lives in
            // the registry, not in RtlGetVersion - empty means "unavailable here".
            return info;
            #else
            return nullopt;
            #endif
        }

        /**
         * @brief All environment variables as a name -> value map.
         * @return A HashMap of the current environment.
         */
        [[nodiscard]]
        static HashMap<String, StringView> variables() {
            HashMap<String, StringView> map;
            #if defined(__unix__) || defined(__APPLE__)
            char** env_list = unix::environ;
            #elifdef _WIN32
            char** env_list = win32::_environ;
            #endif
            for (char** env = env_list; *env != nullptr; ++env) {
                StringView entry(*env);
                if (auto pos = entry.find('='); pos != StringView::npos) {
                    map.emplace(entry.substr(0, pos), entry.substr(pos + 1));
                }
            }
            return map;
        }

        /**
         * @brief The value of a single environment variable.
         * @param name The variable name.
         * @return The value, or nullopt if the variable is unset.
         */
        [[nodiscard]]
        static Optional<StringView> get(StringView name) noexcept {
            const String name_str(name);
            const char* value = std::getenv(name_str.c_str());
            if (value == nullptr) {
                return nullopt;
            }
            return StringView(value);
        }

        /**
         * @brief Sets an environment variable.
         * @param name The variable name.
         * @param value The value to assign.
         * @param overwrite If false and the variable already exists, leave it
         * unchanged. Defaults to true.
         * @return true on success, false on failure (e.g. an invalid name).
         *
         * @warning Mutating the environment is NOT thread-safe, and no lock can
         * make it so. The C library's environment store is shared and
         * unsynchronized, and a concurrent get() in another thread,
         * including calls the C library or third-party code make
         * internally (locale, time zone, ...), races with this write
         * and can read freed memory and crash. Only set or unset
         * environment variables while the program is single-threaded,
         * typically early in main() before any threads are spawned.
         */
        static bool set(StringView name, StringView value, bool overwrite = true) noexcept {
            const String name_str(name);
            const String value_str(value);
            #if defined(__unix__) || defined(__APPLE__)
            return unix::setenv(name_str.c_str(), value_str.c_str(), overwrite ? 1 : 0) == 0;
            #elifdef _WIN32
            // _putenv_s keeps the CRT environment (what get() reads) in
            // sync, unlike SetEnvironmentVariable. It always overwrites, so
            // emulate overwrite=false by checking for an existing value first.
            if (!overwrite && get(name_str.c_str()).has_value()) {
                return true;
            }
            return win32::_putenv_s(name_str.c_str(), value_str.c_str()) == 0;
            #else
            return false;
            #endif
        }

        /**
         * @brief Removes an environment variable. Succeeds if it is already unset.
         * @param name The variable name.
         * @return true on success, false on failure (e.g. an invalid name).
         *
         * @warning Not thread-safe. Mutate the environment only while single-threaded.
         */
        static bool unset(StringView name) noexcept {
            const String name_str(name);
            #if defined(__unix__) || defined(__APPLE__)
            return unix::unsetenv(name_str.c_str()) == 0;
            #elifdef _WIN32
            // On the MSVC CRT, assigning an empty value removes the variable.
            return win32::_putenv_s(name_str.c_str(), "") == 0;
            #else
            return false;
            #endif
        }
    };
}

namespace stdx::fmt {
    template <>
    struct Formatter<Environment::OperatingSystem> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(Environment::OperatingSystem os, FormatContext& ctx) const {
            StringView os_name;
            switch (os) {
                case Environment::OperatingSystem::UNKNOWN:
                    os_name = "unknown";
                    break;
                case Environment::OperatingSystem::UNIX:
                    os_name = "Unix-like";
                    break;
                case Environment::OperatingSystem::WINDOWS:
                    os_name = "Windows";
                    break;
                case Environment::OperatingSystem::LINUX:
                    os_name = "Linux";
                    break;
                case Environment::OperatingSystem::FREEBSD:
                    os_name = "FreeBSD";
                    break;
                case Environment::OperatingSystem::DARWIN:
                    os_name = "Darwin";
                    break;
            }
            return format_to(ctx.out(), "{}", os_name);
        }
    };

    template <>
    struct Formatter<Environment::SystemInfo> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const Environment::SystemInfo& info, FormatContext& ctx) const {
            auto out = format_to(ctx.out(), "{}", info.name);
            if (!info.release.empty()) {
                out = format_to(out, " {}", info.release);
            }
            if (!info.machine.empty()) {
                out = format_to(out, " ({})", info.machine);
            }
            if (!info.node.empty()) {
                out = format_to(out, " on {}", info.node);
            }
            return out;
        }
    };
}

template <>
struct stdx::fmt::formatter<Environment::OperatingSystem> : public Formatter<Environment::OperatingSystem> {};

template <>
struct stdx::fmt::formatter<Environment::SystemInfo> : public Formatter<Environment::SystemInfo> {};
