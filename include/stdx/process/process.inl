#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::ranges::RangeValue;
using stdx::sys::Signal;
using stdx::thread::Thread;

using namespace stdx::os;
using namespace stdx::os::unix;
using namespace stdx::os::win32;

/**
 * @namespace stdx::process
 * @brief Process creation, management, and I/O handling.
 */
namespace stdx::process {
    #ifdef __unix__
    /**
     * @brief Read everything from fd into a buffer, then close it.
     *
     * @param fd File descriptor to read from. Must be open for reading.
     * @return Vector<u8>
     */
    [[nodiscard]]
    Vector<u8> drain_fd(i32 fd) noexcept {
        Vector<u8> buf;
        Array<u8, 4096> tmp;
        while (true) {
            isize n = unix::read(fd, tmp.data(), tmp.size());
            if (n <= 0) {
                break;
            }
            buf.insert(buf.end(), tmp.begin(), tmp.begin() + n);
        }
        unix::close(fd);
        return buf;
    }

    /**
     * @brief Read everything from fd into the given buffer, then close it.
     *
     * @param fd File descriptor to read from. Must be open for reading.
     * @param out Pointer to the output buffer.
     */
    void drain_to(i32 fd, Vector<u8>* out) noexcept {
        *out = drain_fd(fd);
    }
    #endif

    #ifdef _WIN32
    /**
     * @brief Read everything from a Windows HANDLE into a buffer, then close it.
     *
     * @param h HANDLE to read from. Must be open for reading.
     * @return Vector<u8>
     */
    [[nodiscard]]
    Vector<u8> drain_handle(Handle h) noexcept {
        Vector<u8> buf;
        Array<u8, 4096> tmp;
        DWord n = 0;g
        while (win32::ReadFile(h, tmp.data(), tmp.size(), &n, nullptr) && n > 0) {
            buf.insert(buf.end(), tmp.begin(), tmp.begin() + n);
        }
        win32::CloseHandle(h);
        return buf;
    }

    /**
     * @brief Read everything from a Windows HANDLE into the given buffer, then close it.
     *
     * @param h HANDLE to read from. Must be open for reading.
     * @param out Pointer to the output buffer.
     */
    void drain_handle_to(Handle h, Vector<u8>* out) noexcept {
        *out = drain_handle(h);
    }
    #endif
}

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
        [[nodiscard]] i32 raw() const noexcept {
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

    /**
     * @class Child
     * @brief Handle to a running child process.
     *
     * Move-only. Dropping a Child without calling wait() will NOT automatically
     * reap it - the caller must call wait() (or kill() then wait()) to avoid
     * zombie processes on Unix.
     */
    class Child {
    private:
        #ifdef __unix__
        i32 pid = -1; ///< The OS process ID (from fork).
        i32 stdin_wr = -1; ///< The write end of the child's stdin pipe (or -1 if not piped).
        i32 stdout_rd = -1; ///< The read end of the child's stdout pipe (or -1 if not piped).
        i32 stderr_rd = -1; ///< The read end of the child's stderr pipe (or -1 if not piped).
        #elifdef _WIN32
        Handle process = nullptr; ///< The child process HANDLE.
        Handle thread = nullptr; ///< The child thread HANDLE (for proper cleanup).
        Handle stdin_wr = nullptr; ///< The write end of the child's stdin pipe (or nullptr if not piped).
        Handle stdout_rd = nullptr; ///< The read end of the child's stdout pipe (or nullptr if not piped).
        Handle stderr_rd = nullptr; ///< The read end of the child's stderr pipe (or nullptr if not piped).
        #endif

        Child() = default;

        #ifdef __unix__
        Child(i32 pid, i32 stdin_wr, i32 stdout_rd, i32 stderr_rd) noexcept:
            pid{pid}, stdin_wr{stdin_wr}, stdout_rd{stdout_rd}, stderr_rd{stderr_rd} {}
        #elifdef _WIN32
        Child(Handle process, Handle thread, Handle stdin_wr, Handle stdout_rd, Handle stderr_rd) noexcept:
            process{process}, thread{thread}, stdin_wr{stdin_wr}, stdout_rd{stdout_rd}, stderr_rd{stderr_rd} {}
        #endif

        friend class Command;

        /**
         * @brief Close any open handles/fds. Called by the destructor and move assignment operator.
         */
        void close_handles() noexcept {
            #ifdef __unix__
            auto close_fd = [](i32& fd) noexcept -> void {
                if (fd != -1) {
                    unix::close(fd); fd = -1;
                }
            };
            close_fd(stdin_wr);
            close_fd(stdout_rd);
            close_fd(stderr_rd);
            #elifdef _WIN32
            auto close_h = [](Handle& h) noexcept -> void {
                if (h) {
                    win32::CloseHandle(static_cast<Handle>(h)); h = nullptr;
                }
            };
            close_h(stdin_wr);
            close_h(stdout_rd);
            close_h(stderr_rd);
            close_h(thread);
            close_h(process);
            #endif
        }

    public:
        Child(const Child&) = delete;
        Child& operator=(const Child&) = delete;

        Child(Child&& o) noexcept:
        #ifdef __unix__
            pid{Ops::exchange(o.pid, -1)},
            stdin_wr{Ops::exchange(o.stdin_wr, -1)},
            stdout_rd{Ops::exchange(o.stdout_rd, -1)},
            stderr_rd{Ops::exchange(o.stderr_rd, -1)}
        #elifdef _WIN32
            process{Ops::exchange(o.process, nullptr)},
            thread{Ops::exchange(o.thread, nullptr)},
            stdin_wr{Ops::exchange(o.stdin_wr, nullptr)},
            stdout_rd{Ops::exchange(o.stdout_rd, nullptr)},
            stderr_rd{Ops::exchange(o.stderr_rd, nullptr)}
        #endif
            {}

        Child& operator=(Child&& o) noexcept {
            if (this != &o) {
                close_handles();
                new (this) Child(Ops::move(o));
            }
            return *this;
        }

        ~Child() {
            close_handles();
        }

        /**
         * @brief The OS process ID.
         * @return u32 The process ID.
         */
        [[nodiscard]]
        u32 id() const noexcept {
            #ifdef __unix__
            return static_cast<u32>(pid);
            #elifdef _WIN32
            return win32::GetProcessId(static_cast<Handle>(process));
            #else
            return 0;
            #endif
        }

        [[nodiscard]]
        bool has_stdin()  const noexcept {
            #ifdef __unix__
            return stdin_wr != -1;
            #elifdef _WIN32
            return stdin_wr != nullptr;
            #else
            return false;
            #endif
        }

        [[nodiscard]]
        bool has_stdout() const noexcept {
            #ifdef __unix__
            return stdout_rd != -1;
            #elifdef _WIN32
            return stdout_rd != nullptr;
            #else
            return false;
            #endif
        }

        [[nodiscard]]
        bool has_stderr() const noexcept {
            #ifdef __unix__
            return stderr_rd != -1;
            #elifdef _WIN32
            return stderr_rd != nullptr;
            #else
            return false;
            #endif
        }

        #ifdef __unix__
        [[nodiscard]]
        i32 stdin_fd() const noexcept {
            return stdin_wr;
        }

        [[nodiscard]]
        i32 stdout_fd() const noexcept {
            return stdout_rd;
        }

        [[nodiscard]]
        i32 stderr_fd() const noexcept {
            return stderr_rd;
        }
        #endif

        /**
         * @brief Block until the child exits and return its exit status.
         * @return Expected<ExitStatus, ErrorCode> The exit status or an error.
         */
        [[nodiscard]]
        Expected<ExitStatus, ErrorCode> wait() {
            #ifdef __unix__
            if (pid == -1) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            i32 status = 0;
            ProcessId ret;
            do {
                ret = unix::sys::waitpid(pid, &status, 0);
            } while (ret == -1 && unix::errnov() == unix::EINTR);
            if (ret == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            pid = -1;
            return ExitStatus(status);
            #elifdef _WIN32
            if (!process) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            if (win32::WaitForSingleObject(static_cast<Handle>(process), INFINITE) == WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            DWord code = 0;
            win32::GetExitCodeProcess(static_cast<Handle>(process), &code);
            return win32::ExitStatus(static_cast<i32>(code));
            #else
            return Unexpected(Ops::make_error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Non-blocking poll: returns the exit status if the child has already
         * finished, or an empty Optional if it is still running.
         * @return Expected<Optional<ExitStatus>, ErrorCode> The exit status or an error.
         */
        [[nodiscard]]
        Expected<Optional<ExitStatus>, ErrorCode> try_wait() {
            #ifdef __unix__
            if (pid == -1) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            i32 status = 0;
            ProcessId ret = unix::sys::waitpid(pid, &status, unix::sys::WNOHANG);
            if (ret == 0) {
                return Optional<ExitStatus>{};
            }
            if (ret == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            pid = -1;
            return Optional<ExitStatus>{ExitStatus(status)};
            #elifdef _WIN32
            if (!process) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            DWord res = win32::WaitForSingleObject(static_cast<Handle>(process), 0);
            if (res == WAIT_TIMEOUT) {
                return Optional<ExitStatus>{};
            }
            if (res == WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            DWord code = 0;
            win32::GetExitCodeProcess(static_cast<Handle>(process), &code);
            return Optional<ExitStatus>{ExitStatus(static_cast<i32>(code))};
            #else
            return Optional<ExitStatus>{};
            #endif
        }

        /**
         * @brief Send SIGKILL / TerminateProcess to the child.
         * @return Expected<void, ErrorCode> An error if the operation failed.
         */
        [[nodiscard]]
        Expected<void, ErrorCode> kill() {
            #ifdef __unix__
            if (pid == -1) {
                return {};
            }
            if (unix::kill(pid, Signal::KILL) == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            return {};
            #elifdef _WIN32
            if (!process) {
                return {};
            }
            if (!win32::TerminateProcess(static_cast<Handle>(process), 1)) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            return {};
            #else
            return Unexpected(Ops::error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Consume the Child: drain all piped output concurrently (to prevent
         * pipe-buffer deadlocks), wait for exit, and return an Output.
         * @return Expected<Output, ErrorCode> The output or an error.
         */
        [[nodiscard]]
        Expected<Output, ErrorCode> wait_with_output() {
            Vector<u8> out_data;
            Vector<u8> err_data;
            Thread out_thread;
            Thread err_thread;

            #ifdef __unix__
            i32 out_fd = Ops::exchange(stdout_rd, -1);
            i32 err_fd = Ops::exchange(stderr_rd, -1);
            if (out_fd != -1) {
                out_thread = Thread(drain_to, out_fd, &out_data);
            }
            if (err_fd != -1) {
                err_thread = Thread(drain_to, err_fd, &err_data);
            }
            #elifdef _WIN32
            Handle out_h = Ops::exchange(stdout_rd, nullptr);
            Handle err_h = Ops::exchange(stderr_rd, nullptr);
            if (out_h) {
                out_thread = Thread(drain_handle_to, out_h, &out_data);
            }
            if (err_h) {
                err_thread = Thread(drain_handle_to, err_h, &err_data);
            }
            #endif

            if (out_thread.joinable()) {
                out_thread.join();
            }
            if (err_thread.joinable()) {
                err_thread.join();
            }

            Expected<ExitStatus, ErrorCode> result = wait();
            if (!result) {
                return Unexpected(result.error());
            }
            return Output(Ops::move(out_data), Ops::move(err_data), *result);
        }
    };

    #ifdef _WIN32
    /**
     * @brief A process-wide job object configured to kill its members when its
     * last handle closes.
     *
     * The handle is created once and held for the lifetime of this process (the
     * OS closes it on exit or TerminateProcess), so any child assigned to the job
     * is terminated when the parent dies - which a destructor cannot guarantee.
     * Returns nullptr if the job could not be created or configured.
     */
    [[nodiscard]]
    inline Handle parent_death_job() noexcept {
        static Handle job = [] noexcept -> Handle {
            Handle handle = win32::CreateJobObjectW(nullptr, nullptr);
            if (handle == nullptr) {
                return nullptr;
            }
            win32::JobObjectExtendedLimitInformation info{};
            info.BasicLimitInformation.LimitFlags = win32::JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE_FLAG;
            if (!win32::SetInformationJobObject(
                handle, win32::JobObjectExtendedLimitInfoClass, &info, sizeof(info)
            )) {
                win32::CloseHandle(handle);
                return nullptr;
            }
            return handle;
        }();
        return job;
    }
    #endif

    /**
     * @class Command
     * @brief Builder for configuring and spawning a child process.
     *
     * @code
     *   Expected<Output, ErrorCode> result = Command::from("ls")
     *       .arg("-la")
     *       .current_dir("/tmp")
     *       .stdout(Stdio::Piped)
     *       .output();
     * @endcode
     */
    class Command {
    private:
        Vector<String> prog_args; /// The program and its arguments (first element is the program).
        Vector<Pair<String, String>> env_set; ///< Environment variables to set (key-value pairs).
        Vector<String> env_rem; ///< Environment variable keys to remove.
        String prog; ///< The program to execute.
        Optional<Path> cwd; ///< Optional working directory for the child process.
        Stdio stdin_cfg = Stdio::INHERIT; ///< Configuration for the child's standard input stream.
        Stdio stdout_cfg = Stdio::INHERIT; ///< Configuration for the child's standard output stream.
        Stdio stderr_cfg = Stdio::INHERIT; ///< Configuration for the child's standard error stream.
        bool env_clr = false; ///< If true, start with an empty environment instead of inheriting the parent's.
        bool kill_with_parent = false; ///< If true, ask the OS to kill the child when this process dies (Linux: PR_SET_PDEATHSIG).

        /**
         * @brief Construct a Command for the given program name (searched via PATH).
         * @param program The name of the program to execute.
         */
        explicit Command(StringView program):
            prog{program} {}

        #ifdef __unix__
        [[nodiscard]]
        Expected<Child, ErrorCode> spawn_unix() const {
            i32 in_r = -1;
            i32 in_w = -1;
            i32 out_r = -1;
            i32 out_w = -1;
            i32 err_r = -1;
            i32 err_w = -1;
            i32 null = -1;

            auto cleanup = [&] noexcept -> void {
                for (i32 fd: {in_r, in_w, out_r, out_w, err_r, err_w, null}) {
                    if (fd != -1) {
                        unix::close(fd);
                    }
                }
            };
            auto make_pipe = [](i32& r, i32& w) noexcept -> bool {
                i32 fds[2];
                if (unix::pipe(fds) == -1) {
                    return false;
                }
                r = fds[0];
                w = fds[1];
                return true;
            };

            if (stdin_cfg == Stdio::PIPED && !make_pipe(in_r,  in_w)) {
                cleanup();
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            if (stdout_cfg == Stdio::PIPED && !make_pipe(out_r, out_w)) {
                cleanup();
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            if (stderr_cfg == Stdio::PIPED && !make_pipe(err_r, err_w)) {
                cleanup(); return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }

            if (stdin_cfg == Stdio::NULL_DEV || stdout_cfg == Stdio::NULL_DEV || stderr_cfg == Stdio::NULL_DEV) {
                null = static_cast<i32>(unix::open("/dev/null", unix::O_RDWR));
                if (null == -1) {
                    cleanup();
                    return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
                }
            }

            #ifdef __linux__
            const i32 parent_pid = unix::getpid();
            #endif

            i32 raw_pid = static_cast<i32>(unix::fork());
            if (raw_pid == -1) {
                i32 e = unix::errnov();
                cleanup();
                return Unexpected(ErrorCode(e, Ops::system_category()));
            }

            if (raw_pid == 0) {
                #ifdef __linux__
                if (kill_with_parent) {
                    // Ask the kernel to SIGKILL us when our parent dies - this
                    // survives the parent's own SIGKILL/crash, which no parent-side
                    // destructor can. Then close the fork->prctl race: if the parent
                    // already exited, the signal won't fire, so bail out now.
                    linux::sys::prctl(
                        linux::sys::PR_SET_PDEATHSIG_OPTION,
                        static_cast<unsigned long>(Signal::KILL)
                    );
                    if (unix::getppid() != parent_pid) {
                        unix::_exit(127);
                    }
                }
                #endif

                auto dup_or_die = [](i32 src, i32 dst) noexcept -> void {
                    if (src != -1 && unix::dup2(src, dst) == -1) {
                        unix::_exit(127);
                    }
                };

                switch (stdin_cfg) {
                    case Stdio::PIPED:
                        dup_or_die(in_r, unix::STDIN_FILENO);
                        break;
                    case Stdio::NULL_DEV:
                        dup_or_die(null, unix::STDIN_FILENO);
                        break;
                    case Stdio::INHERIT:
                        break;
                    default:
                        Ops::unreachable();
                }
                switch (stdout_cfg) {
                    case Stdio::PIPED:
                        dup_or_die(out_w, unix::STDOUT_FILENO);
                        break;
                    case Stdio::NULL_DEV:
                        dup_or_die(null,  unix::STDOUT_FILENO);
                        break;
                    case Stdio::INHERIT:
                        break;
                    default:
                        Ops::unreachable();
                }
                switch (stderr_cfg) {
                    case Stdio::PIPED:
                        dup_or_die(err_w, unix::STDERR_FILENO);
                        break;
                    case Stdio::NULL_DEV:
                        dup_or_die(null, unix::STDERR_FILENO);
                        break;
                    case Stdio::INHERIT:
                        break;
                    default:
                        Ops::unreachable();
                }
                for (i32 fd: {in_r, in_w, out_r, out_w, err_r, err_w, null}) {
                    if (fd != -1) {
                        unix::close(fd);
                    }
                }

                if (cwd && unix::chdir(cwd->c_str()) == -1) {
                    unix::_exit(127);
                }

                if (env_clr) {
                    unix::clearenv();
                }
                for (const String& key: env_rem) {
                    unix::unsetenv(key.c_str());
                }
                for (auto& [key, val]: env_set) {
                    unix::setenv(key.c_str(), val.c_str(), 1);
                }

                Vector<const char*> argv;
                argv.reserve(prog_args.size() + 2);
                argv.push_back(prog.c_str());
                for (const String& a: prog_args) {
                    argv.push_back(a.c_str());
                }
                argv.push_back(nullptr);

                unix::execvp(prog.c_str(), const_cast<char* const*>(argv.data()));
                unix::_exit(127);
            }

            for (i32 fd: {in_r, out_w, err_w, null}) {
                if (fd != -1) {
                    unix::close(fd);
                }
            }

            Child child(raw_pid, in_w, out_r, err_r);
            return child;
        }
        #endif

        #ifdef _WIN32
        [[nodiscard]]
        Expected<Child, ErrorCode> spawn_win32() const {
            SecurityAttributes sa(sizeof(sa), nullptr, TRUE);
            Handle in_r = INVALID_HANDLE_VALUE;
            Handle in_w = INVALID_HANDLE_VALUE;
            Handle out_r = INVALID_HANDLE_VALUE;
            Handle out_w = INVALID_HANDLE_VALUE;
            Handle err_r = INVALID_HANDLE_VALUE;
            Handle err_w = INVALID_HANDLE_VALUE;
            Handle null = INVALID_HANDLE_VALUE;

            auto close_h = [](Handle& h) noexcept -> void {
                if (h != INVALID_HANDLE_VALUE) {
                    win32::CloseHandle(h); h = INVALID_HANDLE_VALUE;
                }
            };
            auto cleanup = [&] noexcept -> void {
                for (Handle* h: {&in_r, &in_w, &out_r, &out_w, &err_r, &err_w, &null}) {
                    close_h(*h);
                }
            };
            auto make_pipe = [&](Handle& r, Handle& w, bool parent_is_r) -> bool {
                if (!win32::CreatePipe(&r, &w, &sa, 0)) {
                    return false;
                }
                win32::SetHandleInformation(parent_is_r ? r : w, HANDLE_FLAG_INHERIT, 0);
                return true;
            };

            if (stdin_cfg == Stdio::PIPED && !make_pipe(in_r, in_w, false)) {
                cleanup();
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            if (stdout_cfg == Stdio::PIPED && !make_pipe(out_r, out_w, true)) {
                cleanup();
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            if (stderr_cfg == Stdio::PIPED && !make_pipe(err_r, err_w, true)) {
                cleanup();
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }

            if (stdin_cfg == Stdio::NULL_DEV || stdout_cfg == Stdio::NULL_DEV || stderr_cfg == Stdio::NULL_DEV) {
                null = win32::CreateFileW(L"NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
                if (null == INVALID_HANDLE_VALUE) {
                    cleanup();
                    return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
                }
            }

            StartupInfoW si{
                .cb = sizeof(StartupInfoW),
                .dwFlags = STARTF_USESTDHANDLES,
                .hStdInput = (stdin_cfg == Stdio::PIPED)
                    ? in_r : (stdin_cfg == Stdio::NULL_DEV)
                    ? null : win32::GetStdHandle(STD_INPUT_HANDLE),
                .hStdOutput = (stdout_cfg == Stdio::PIPED)
                    ? out_w : (stdout_cfg == Stdio::NULL_DEV)
                    ? null : win32::GetStdHandle(STD_OUTPUT_HANDLE),
                .hStdError = (stderr_cfg == Stdio::PIPED)
                    ? err_w : (stderr_cfg == Stdio::NULL_DEV)
                    ? null : win32::GetStdHandle(STD_ERROR_HANDLE)
            };

            WideString cmdline;
            cmdline += L'"';
            cmdline.append(prog.begin(), prog.end());
            cmdline += L'"';
            for (const String& a: prog_args) {
                cmdline += L" \"";
                cmdline.append(a.begin(), a.end());
                cmdline += L'"';
            }

            WideString wcwd;
            if (cwd) {
                wcwd = cwd->wstring();
            }

            ProcessInformation pi;
            // When linking the child to our lifetime, start it suspended so it can
            // be placed in the kill-on-close job before it (or any of its own
            // children) runs - closing the create/assign race.
            const unsigned long flags = kill_with_parent ? win32::CREATE_SUSPENDED_FLAG : 0ul;
            if (!win32::CreateProcessW(
                nullptr,
                cmdline.data(),
                nullptr,
                nullptr,
                true,
                flags,
                nullptr,
                cwd ? wcwd.c_str() : nullptr,
                &si,
                &pi
            )) {
                cleanup();
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }

            if (kill_with_parent) {
                if (Handle job = parent_death_job(); job != nullptr) {
                    win32::AssignProcessToJobObject(job, pi.hProcess);
                }
                win32::ResumeThread(pi.hThread);
            }

            for (Handle* h: {&in_r, &out_w, &err_w, &null}) {
                close_h(*h);
            }

            Child child(pi.hProcess, pi.hThread, in_w, out_r, err_r);
            return child;
        }
        #endif
    public:
        /**
         * @brief Create a Command with the given program. The program must be a valid executable name or path on the system, and must not be empty.
         * 
         * @param program The program to execute (executable name or path).
         * @return Command 
         */
        [[nodiscard]]
        static Command from(StringView program) {
            return Command(program);
        }

        Command& arg(StringView a) {
            prog_args.emplace_back(a);
            return *this;
        }

        Command& env(StringView k, StringView v) {
            env_set.emplace_back(k, v);
            return *this;
        }

        Command& env_remove(StringView k) {
            env_rem.emplace_back(k);
            return *this;
        }

        Command& env_clear() noexcept {
            env_clr = true;
            return *this;
        }

        /**
         * @brief Ask the OS to kill the spawned child if this (parent) process
         * dies - including on SIGKILL or a crash, which a Child destructor cannot
         * cover because it never runs in those cases.
         *
         * Linux: sets PR_SET_PDEATHSIG(SIGKILL) on the child, with a getppid()
         * race guard for a parent that exits during the fork. Note the "parent"
         * is the spawning thread, and the setting is cleared by a set-user-ID/
         * set-group-ID execve.
         *
         * Windows: assigns the child to a process-wide job object created with
         * JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE; the kernel kills the job's processes
         * when this process exits or is terminated.
         *
         * Other platforms (e.g. macOS/BSD): no effect - no equivalent primitive.
         */
        Command& terminate_on_parent_exit() noexcept {
            kill_with_parent = true;
            return *this;
        }

        Command& current_dir(const Path& path) noexcept {
            cwd = path;
            return *this;
        }

        Command& stdin(Stdio s) noexcept {
            stdin_cfg = s;
            return *this;
        }

        Command& stdout(Stdio s) noexcept {
            stdout_cfg = s;
            return *this;
        }

        Command& stderr(Stdio s) noexcept {
            stderr_cfg = s;
            return *this;
        }

        /**
         * @brief Append all arguments from any input range of string-like values.
         * @tparam R The type of the input range.
         * @param range The input range of string-like values.
         * @return Command& A reference to the modified Command object.
         */
        template <InputRange R>
            requires ConvertibleTo<RangeValue<R>, StringView>
        Command& args(R&& range) {
            for (auto&& a: range) {
                prog_args.emplace_back(StringView(a));
            }
            return *this;
        }

        [[nodiscard]]
        StringView program() const noexcept {
            return prog;
        }

        [[nodiscard]]
        const Vector<String>& args() const noexcept {
            return prog_args;
        }

        /**
         * @brief Spawn the child, returning a handle. Stdio streams follow their configured Stdio mode.
         * @return Expected<Child, ErrorCode> A handle to the spawned child process or an error.
         */
        [[nodiscard]]
        Expected<Child, ErrorCode> spawn() const {
            #ifdef __unix__
            return spawn_unix();
            #elifdef _WIN32
            return spawn_win32();
            #else
            return Unexpected(make_error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Spawn the child, wait for it to exit, and return its ExitStatus.
         * @return Expected<ExitStatus, ErrorCode> The exit status of the spawned child process or an error.
         * Stdout/stderr are inherited from the parent.
         */
        [[nodiscard]]
        Expected<ExitStatus, ErrorCode> status() const {
            Expected<Child, ErrorCode> child = spawn();
            if (!child) {
                return Unexpected(child.error());
            }
            return child->wait();
        }

        /**
         * @brief Spawn the child with stdout/stderr piped and stdin null, wait for exit,
         * and return the captured Output.
         * @return Expected<Output, ErrorCode> The captured output of the spawned child process or an error.
         */
        [[nodiscard]]
        Expected<Output, ErrorCode> output() const {
            Command temp(*this);
            temp.stdin(Stdio::NULL_DEV)
                .stdout(Stdio::PIPED)
                .stderr(Stdio::PIPED);
            Expected<Child, ErrorCode> child = temp.spawn();
            if (!child) {
                return Unexpected(child.error());
            }
            return child->wait_with_output();
        }
    };
}
