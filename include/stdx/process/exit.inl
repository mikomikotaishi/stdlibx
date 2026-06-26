#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::process
 * @brief Process creation, management, and I/O handling.
 */
export namespace stdx::process {
    /**
     * @enum Stdio
     * @brief Controls how a child process's standard stream is connected.
     */
    enum class Stdio: u8 {
        INHERIT, ///< Inherit the parent's stream unchanged.
        NULL_DEV, ///< Redirect to the null device (/dev/null or NUL).
        PIPED, ///< Connect via an anonymous pipe.
    };

    /**
     * @class ExitStatus
     * @brief The exit status of a finished child process.
     *
     * On Unix the raw value is the integer returned by waitpid(2).
     * On Windows it is the DWORD exit code from GetExitCodeProcess.
     */
    class ExitStatus {
    public:
        static constexpr i32 EXIT_SUCCESS = System::EXIT_SUCCESS;
        static constexpr i32 EXIT_FAILURE = System::EXIT_FAILURE;
    private:
        i32 raw_value; ///< The raw platform-specific exit status value.
    public:
        explicit ExitStatus(i32 raw) noexcept:
            raw_value{raw} {}

        /**
         * @brief True if the process exited normally with code 0.
         * @return true If the process exited with status 0 (success).
         */
        [[nodiscard]]
        bool success() const noexcept {
            #ifdef __unix__
            return unix::sys::WIFEXITED(raw_value) && unix::sys::WEXITSTATUS(raw_value) == 0;
            #else
            return raw_value == 0;
            #endif
        }

        /** 
         * @brief The numeric exit code if the process exited normally.
         * @return Optional<i32> The exit code if the process exited normally, otherwise nullopt.
         */
        [[nodiscard]]
        Optional<i32> code() const noexcept {
            #ifdef __unix__
            if (unix::sys::WIFEXITED(raw_value)) {
                return static_cast<i32>(unix::sys::WEXITSTATUS(raw_value));
            }
            return nullopt;
            #else
            return static_cast<i32>(raw_value);
            #endif
        }

        #ifdef __unix__
        /** 
         * @brief The signal number that killed the process, if it was signal-terminated.
         * @return Optional<i32> The signal number if the process was signal-terminated, otherwise nullopt.
         */
        [[nodiscard]]
        Optional<i32> signal() const noexcept {
            if (unix::sys::WIFSIGNALED(raw_value)) {
                return static_cast<i32>(unix::sys::WTERMSIG(raw_value));
            }
            return nullopt;
        }

        /** 
         * @brief True if the process was stopped (not terminated) by a signal.
         * @return bool
         */
        [[nodiscard]]
        bool stopped() const noexcept {
            return unix::sys::WIFSTOPPED(raw_value);
        }
        #endif

        /** 
         * @brief The raw platform-specific status value (waitpid status on Unix, exit code on Windows).
         * @return i32
         */
        [[nodiscard]]
        i32 raw() const noexcept {
            return raw_value;
        }

        [[nodiscard]]
        bool operator==(const ExitStatus&) const noexcept = default;

        [[nodiscard]]
        StrongOrdering operator<=>(const ExitStatus&) const noexcept = default;
    };

    /**
     * @struct Output
     * @brief The captured result of a completed process (stdout, stderr, status).
     */
    struct [[nodiscard]] Output {
        Vector<u8> stdout_data; ///< The data captured from the child's standard output.
        Vector<u8> stderr_data; ///< The data captured from the child's standard error.
        ExitStatus status; ///< The exit status of the child process.

        /**
         * @brief True if the process exited normally with code 0.
         * @return true If the process exited with status 0 (success).
         */
        [[nodiscard]]
        bool success() const noexcept {
            return status.success();
        }
    };
}

using stdx::process::ExitStatus;

namespace stdx::fmt {
    template <>
    struct Formatter<ExitStatus> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const ExitStatus& status, FormatContext& ctx) {
            return format_to(ctx.out(), "ExitStatus({})", status.raw());
        }
    };
}

template <>
struct stdx::fmt::formatter<ExitStatus> : public Formatter<ExitStatus> {};
