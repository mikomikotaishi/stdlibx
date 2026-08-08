#pragma once

using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::fmt::FormatContext;
using stdx::fmt::Formatter;
using stdx::time::Duration;
using stdx::time::Instant;

using namespace stdx::os;

/**
 * @internal
 * @namespace stdx::core::arguments
 * @brief Where Environment::args gets argv, and the table it hands out.
 *
 * A library cannot read main's parameters directly, but it does not need main's
 * cooperation either - each platform offers a way in:
 *
 *   Linux, BSD   .init_array entries are called with (argc, argv, envp), which
 *                is the only moment the values are offered. Hence the
 *                constructor below; everywhere else can read on demand.
 *   Darwin       _NSGetArgc / _NSGetArgv, the documented dyld accessors.
 *   Windows      the CRT's __argc / __wargv, reached through win32::ArgCount
 *                and win32::WideArgv because they are macros.
 */
namespace stdx::core::arguments {
    /// Set by set_args, and preferred over any platform source when non-null.
    inline i32 override_argc = 0;
    inline char** override_argv = nullptr;

    #if (defined(__unix__) || defined(__linux__)) && !defined(__APPLE__) && defined(__GNUC__)
    inline i32 startup_argc = 0;
    inline char** startup_argv = nullptr;

    /**
     * @internal
     * @brief Records argv before main runs.
     *
     * Not inline and not static: the attribute is what causes this to be
     * emitted and referenced from .init_array, and it is defined in exactly one
     * translation unit, so neither is needed.
     */
    [[gnu::constructor]]
    void capture_startup_arguments(int argc, char* argv[], [[maybe_unused]] char* envp[]) noexcept {
        startup_argc = argc;
        startup_argv = argv;
    }
    #endif

    /**
     * @internal
     * @brief The views args() spans, plus whatever backs them.
     *
     * On Unix the views point straight into argv, which lives in the process's
     * initial stack region and is never reallocated by a library call - unlike
     * environ, which is why variables() has to copy. On Windows they point into
     * `owned`, because the faithful source there is UTF-16 and has to be
     * converted. Either way the span is valid for the life of the process.
     */
    struct Table {
        Vector<String> owned;
        Vector<StringView> views;
    };

    [[nodiscard]]
    inline Table build() {
        Table table;

        i32 argc = 0;
        char** argv = nullptr;
        if (override_argv != nullptr) {
            argc = override_argc;
            argv = override_argv;
        } else {
            #ifdef _WIN32
            const i32 wide_argc = static_cast<i32>(win32::ArgCount());
            wchar** wide_argv = win32::WideArgv();
            if (wide_argv != nullptr) {
                table.owned.reserve(static_cast<usize>(wide_argc));
                table.views.reserve(static_cast<usize>(wide_argc));
                for (i32 i = 0; i < wide_argc; ++i) {
                    table.owned.push_back(text::utf16_to_utf8(wide_argv[i]));
                }
                for (const String& argument: table.owned) {
                    table.views.emplace_back(argument);
                }
            }
            return table;
            #elifdef __APPLE__
            argc = static_cast<i32>(*darwin::_NSGetArgc());
            argv = *darwin::_NSGetArgv();
            #elif defined(__unix__) || defined(__linux__)
            argc = startup_argc;
            argv = startup_argv;
            #endif
        }

        if (argv == nullptr || argc <= 0) {
            return table;
        }
        table.views.reserve(static_cast<usize>(argc));
        for (i32 i = 0; i < argc; ++i) {
            if (argv[i] != nullptr) {
                table.views.emplace_back(argv[i]);
            }
        }
        return table;
    }

    /**
     * @internal
     * @brief The table, built once on first use.
     *
     * A function-local static rather than a OnceFlag: sync/mutex.inl is included
     * after this file, and the language already guarantees this initialization
     * happens exactly once even under contention.
     */
    [[nodiscard]]
    inline const Table& table() {
        static const Table built = build();
        return built;
    }
}

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

        Environment() = DELETE_METHOD("Environment is a static utility class and cannot be instantiated.");

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
            win32::DWord host_len = sizeof(host);
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
         * @brief The command line the process was started with.
         * @return The arguments, argv[0] first, valid for the life of the process.
         *
         * Matches Rust's std::env::args in including the program name, and needs no
         * cooperation from main: the values are taken from the platform (see
         * stdx::core::arguments) rather than passed in.
         *
         * These are the bytes the process was handed, not validated text. Nothing
         * guarantees they are well-formed UTF-8 on Unix - execve accepts arbitrary
         * bytes, and the locale decides how to read them - so this promises an
         * encoding no more than argv itself does. On Windows the wide argv is
         * converted, with U+FFFD for anything ill-formed.
         *
         * Empty if no platform source was available and set_args was not called. On
         * Linux that includes being called from a static initializer that runs
         * before this file's own, so treat args() as valid from main onward.
         */
        [[nodiscard]]
        static Span<const StringView> args() {
            const arguments::Table& table = arguments::table();
            return Span<const StringView>(table.views.data(), table.views.size());
        }

        /**
         * @brief The program name, as argv[0].
         * @return argv[0], or an empty view if no arguments were available.
         */
        [[nodiscard]]
        static StringView program_name() {
            const Span<const StringView> all = args();
            return all.empty() ? ""sv : all.front();
        }

        /**
         * @brief Supplies the command line explicitly.
         * @param argc The argument count.
         * @param argv The argument vector, which must outlive the process's use of
         * args() - it is borrowed, not copied.
         *
         * For hosts where no platform source applies: an embedded interpreter, a
         * plugin loaded into someone else's main, or a test that wants a specific
         * command line. Rust has no equivalent because it always owns main.
         *
         * @warning Must be called before the first args(), which builds its table
         * once and does not rebuild.
         */
        static void set_args(i32 argc, char* argv[]) noexcept {
            arguments::override_argc = argc;
            arguments::override_argv = argv;
        }

        /**
         * @brief All environment variables as a name -> value map.
         * @return A HashMap of the current environment at the moment of the call.
         */
        [[nodiscard]]
        static HashMap<String, String> variables() {
            HashMap<String, String> map;
            #if defined(__unix__) || defined(__APPLE__)
            char** env_list = unix::environ;
            #elifdef _WIN32
            char** env_list = win32::EnvironmentBlock();
            #endif
            if (env_list == nullptr) {
                return map;
            }
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
         * @return The value, or nullopt if the variable is unset, at the moment of the call.
         */
        [[nodiscard]]
        static Optional<String> get(StringView name) noexcept {
            const String name_str(name);
            const char* value = std::getenv(name_str.c_str());
            if (value == nullptr) {
                return nullopt;
            }
            return String(value);
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
struct stdx::fmt::formatter<Environment::OperatingSystem>: public Formatter<Environment::OperatingSystem> {};

template <>
struct stdx::fmt::formatter<Environment::SystemInfo>: public Formatter<Environment::SystemInfo> {};
