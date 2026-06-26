#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::ranges::RangeValue;
using stdx::sys::Signal;
using stdx::thread::Thread;

using namespace stdx::os;

/**
 * @namespace stdx::process
 * @brief Process creation, management, and I/O handling.
 */
namespace stdx::process {
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
    inline win32::Handle parent_death_job() noexcept {
        static win32::Handle job = [] noexcept -> win32::Handle {
            win32::Handle handle = win32::CreateJobObjectW(nullptr, nullptr);
            if (handle == nullptr) {
                return nullptr;
            }
            JobObjectExtendedLimitInformation info{};
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
}


/**
 * @namespace stdx::process
 * @brief Process creation, management, and I/O handling.
 */
export namespace stdx::process {
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
            win32::Handle in_r = INVALID_HANDLE_VALUE;
            win32::Handle in_w = INVALID_HANDLE_VALUE;
            win32::Handle out_r = INVALID_HANDLE_VALUE;
            win32::Handle out_w = INVALID_HANDLE_VALUE;
            win32::Handle err_r = INVALID_HANDLE_VALUE;
            win32::Handle err_w = INVALID_HANDLE_VALUE;
            win32::Handle null = INVALID_HANDLE_VALUE;

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
                if (win32::Handle job = parent_death_job(); job != nullptr) {
                    win32::AssignProcessToJobObject(job, pi.hProcess);
                }
                win32::ResumeThread(pi.hThread);
            }

            for (win32::Handle* h: {&in_r, &out_w, &err_w, &null}) {
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
