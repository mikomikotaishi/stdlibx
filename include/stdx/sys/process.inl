#pragma once

using stdx::sys::Signal;
using stdx::thread::Thread;
using stdx::time::Instant;
using stdx::time::Milliseconds;
using stdx::time::SteadyClock;

using namespace stdx::os;

#ifdef STDLIBX_EXECUTION_AVAILABLE
// Declared in net/reactor.task.inl, which is included ahead of this header.
using stdx::net::Reactor;
using stdx::net::ReactorTask;
#endif

/**
 * @namespace stdx::sys
 * @brief Standard library system operations.
 */
namespace stdx::sys {
    #if defined(__unix__) || defined(__APPLE__)
    /**
     * @brief Read everything from fd into a buffer, then close it.
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
     * @param h HANDLE to read from. Must be open for reading.
     * @return Vector<u8>
     */
    [[nodiscard]]
    Vector<u8> drain_handle(win32::Handle h) noexcept {
        Vector<u8> buf;
        Array<u8, 4096> tmp;
        win32::DWord n = 0;
        while (win32::ReadFile(h, tmp.data(), tmp.size(), &n, nullptr) && n > 0) {
            buf.insert(buf.end(), tmp.begin(), tmp.begin() + n);
        }
        win32::CloseHandle(h);
        return buf;
    }

    /**
     * @brief Read everything from a Windows HANDLE into the given buffer, then close it.
     * @param h HANDLE to read from. Must be open for reading.
     * @param out Pointer to the output buffer.
     */
    void drain_handle_to(win32::Handle h, Vector<u8>* out) noexcept {
        *out = drain_handle(h);
    }
    #endif
}

/**
 * @namespace stdx::sys
 * @brief Standard library system operations.
 */
export namespace stdx::sys {
    /**
     * @class Process
     * @brief Handle to a running child process.
     *
     * Move-only. Dropping a Process without calling wait() will NOT automatically
     * reap it - the caller must call wait() (or kill() then wait()) to avoid
     * zombie processes on Unix.
     */
    class [[nodiscard]] Process {
    public:
        class Builder;
    private:
        #if defined(__unix__) || defined(__APPLE__)
        i32 _pid = -1; ///< The OS process ID (from fork).
        i32 _stdin_wr = -1; ///< The write end of the child's stdin pipe (or -1 if not piped).
        i32 _stdout_rd = -1; ///< The read end of the child's stdout pipe (or -1 if not piped).
        i32 _stderr_rd = -1; ///< The read end of the child's stderr pipe (or -1 if not piped).
        #ifdef __linux__
        i32 _exit_pidfd = -1; ///< A pidfd for the child, readable on exit; -1 unless the Process was bound to a Reactor. @see Reactor::spawn
        #endif
        #elifdef _WIN32
        win32::Handle _process = nullptr; ///< The child process HANDLE.
        win32::Handle _thread = nullptr; ///< The child thread HANDLE (for proper cleanup).
        win32::Handle _stdin_wr = nullptr; ///< The write end of the child's stdin pipe (or nullptr if not piped).
        win32::Handle _stdout_rd = nullptr; ///< The read end of the child's stdout pipe (or nullptr if not piped).
        win32::Handle _stderr_rd = nullptr; ///< The read end of the child's stderr pipe (or nullptr if not piped).
        #endif

        Process() = default;

        #if defined(__unix__) || defined(__APPLE__)
        Process(i32 pid, i32 stdin_wr, i32 stdout_rd, i32 stderr_rd) noexcept:
            _pid{pid}, _stdin_wr{stdin_wr}, _stdout_rd{stdout_rd}, _stderr_rd{stderr_rd} {}
        #elifdef _WIN32
        Process(win32::Handle process, win32::Handle thread, win32::Handle stdin_wr, win32::Handle stdout_rd, win32::Handle stderr_rd) noexcept:
            _process{process}, _thread{thread}, _stdin_wr{stdin_wr}, _stdout_rd{stdout_rd}, _stderr_rd{stderr_rd} {}
        #endif

        /**
         * @brief Close any open handles/fds. Called by the destructor and move assignment operator.
         */
        void close_handles() noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            auto close_fd = [](i32& fd) noexcept -> void {
                if (fd != -1) {
                    unix::close(fd); fd = -1;
                }
            };

            close_fd(_stdin_wr);
            close_fd(_stdout_rd);
            close_fd(_stderr_rd);
            #ifdef __linux__
            close_fd(_exit_pidfd);
            #endif
            #elifdef _WIN32
            auto close_h = [](win32::Handle& h) noexcept -> void {
                if (h) {
                    win32::CloseHandle(static_cast<win32::Handle>(h)); h = nullptr;
                }
            };

            close_h(_stdin_wr);
            close_h(_stdout_rd);
            close_h(_stderr_rd);
            close_h(_thread);
            close_h(_process);
            #endif
        }

    public:
        Process(const Process&) = DELETE_METHOD("Process is not copyable.");
        Process& operator=(const Process&) = DELETE_METHOD("Process is not copyable.");

        Process(Process&& o) noexcept:
        #if defined(__unix__) || defined(__APPLE__)
            _pid{Ops::exchange(o._pid, -1)},
            _stdin_wr{Ops::exchange(o._stdin_wr, -1)},
            _stdout_rd{Ops::exchange(o._stdout_rd, -1)},
            _stderr_rd{Ops::exchange(o._stderr_rd, -1)}
            #ifdef __linux__
            , _exit_pidfd{Ops::exchange(o._exit_pidfd, -1)}
            #endif
        #elifdef _WIN32
            _process{Ops::exchange(o._process, nullptr)},
            _thread{Ops::exchange(o._thread, nullptr)},
            _stdin_wr{Ops::exchange(o._stdin_wr, nullptr)},
            _stdout_rd{Ops::exchange(o._stdout_rd, nullptr)},
            _stderr_rd{Ops::exchange(o._stderr_rd, nullptr)}
        #endif
            {}

        Process& operator=(Process&& o) noexcept {
            if (this != &o) {
                close_handles();
                new (this) Process(Ops::move(o));
            }
            return *this;
        }

        ~Process() {
            close_handles();
        }

        /**
         * @brief The OS process ID.
         * @return u32 The process ID.
         */
        [[nodiscard]]
        u32 id() const noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            return static_cast<u32>(_pid);
            #elifdef _WIN32
            return win32::GetProcessId(static_cast<win32::Handle>(_process));
            #else
            return 0;
            #endif
        }

        [[nodiscard]]
        bool has_stdin() const noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            return _stdin_wr != -1;
            #elifdef _WIN32
            return _stdin_wr != nullptr;
            #else
            return false;
            #endif
        }

        [[nodiscard]]
        bool has_stdout() const noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            return _stdout_rd != -1;
            #elifdef _WIN32
            return _stdout_rd != nullptr;
            #else
            return false;
            #endif
        }

        [[nodiscard]]
        bool has_stderr() const noexcept {
            #if defined(__unix__) || defined(__APPLE__)
            return _stderr_rd != -1;
            #elifdef _WIN32
            return _stderr_rd != nullptr;
            #else
            return false;
            #endif
        }

        #if defined(__unix__) || defined(__APPLE__)
        [[nodiscard]]
        i32 stdin_fd() const noexcept {
            return _stdin_wr;
        }

        [[nodiscard]]
        i32 stdout_fd() const noexcept {
            return _stdout_rd;
        }

        [[nodiscard]]
        i32 stderr_fd() const noexcept {
            return _stderr_rd;
        }
        #endif

        #if defined(__linux__) && defined(STDLIBX_EXECUTION_AVAILABLE)
        /**
         * @brief The pidfd watching this child for exit, or -1 if this Process
         * was not bound to a Reactor via @ref stdx::net::Reactor::spawn.
         * @return i32 The pidfd, or -1.
         */
        [[nodiscard]]
        i32 exit_pidfd() const noexcept {
            return _exit_pidfd;
        }

        /**
         * @internal
         * @brief Attach a pidfd that a Reactor opened for this child. Takes
         * ownership: the fd is closed with the other handles. Only
         * @ref stdx::net::Reactor::spawn calls this.
         * @param pidfd A pidfd referring to this child, from pidfd_open.
         */
        void set_exit_pidfd(i32 pidfd) noexcept {
            if (_exit_pidfd != -1) {
                unix::close(_exit_pidfd);
            }
            _exit_pidfd = pidfd;
        }

        /**
         * @brief Asynchronously wait for the child to exit, suspending the
         * calling coroutine instead of blocking its thread.
         *
         * Requires this Process to have been spawned through
         * @ref stdx::net::Reactor::spawn, which opens the pidfd this awaits on.
         * Awaiting a Process that was not reactor-bound is a precondition
         * violation. The @p reactor must be the same loop the Process was
         * spawned on, and must outlive the returned Task.
         *
         * Defined out of line in net/async.inl, where Reactor and Readiness are
         * complete types.
         *
         * @param reactor The loop driving the wait; the one this was spawned on.
         * @return A reactor task completing with the child's exit status.
         */
        [[nodiscard]]
        ReactorTask<ExitStatus> wait_async(Reactor& reactor);
        #endif

        /**
         * @brief Block until the child exits and return its exit status.
         * @return Expected<ExitStatus, ErrorCode> The exit status or an error.
         */
        [[nodiscard]]
        Expected<ExitStatus, ErrorCode> wait() {
            #if defined(__unix__) || defined(__APPLE__)
            if (_pid == -1) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            i32 status = 0;
            unix::ProcessId ret;
            do {
                ret = unix::sys::waitpid(_pid, &status, 0);
            } while (ret == -1 && unix::errnov() == unix::EINTR);
            if (ret == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            _pid = -1;
            return ExitStatus(status);
            #elifdef _WIN32
            if (!_process) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            if (win32::WaitForSingleObject(static_cast<win32::Handle>(_process), win32::INFINITE) == win32::WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            win32::DWord code = 0;
            win32::GetExitCodeProcess(static_cast<win32::Handle>(_process), &code);
            return ExitStatus(static_cast<i32>(code));
            #else
            return Unexpected(Ops::error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Non-blocking poll: returns the exit status if the child has already
         * finished, or an empty Optional if it is still running.
         * @return Expected<Optional<ExitStatus>, ErrorCode> The exit status or an error.
         */
        [[nodiscard]]
        Expected<Optional<ExitStatus>, ErrorCode> try_wait() {
            #if defined(__unix__) || defined(__APPLE__)
            if (_pid == -1) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            i32 status = 0;
            unix::ProcessId ret = unix::sys::waitpid(_pid, &status, unix::sys::WNOHANG);
            if (ret == 0) {
                return nullopt;
            }
            if (ret == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            _pid = -1;
            return ExitStatus(status);
            #elifdef _WIN32
            if (!_process) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            win32::DWord res = win32::WaitForSingleObject(static_cast<win32::Handle>(_process), 0);
            if (res == win32::WAIT_TIMEOUT) {
                return nullopt;
            }
            if (res == win32::WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            win32::DWord code = 0;
            win32::GetExitCodeProcess(static_cast<win32::Handle>(_process), &code);
            return ExitStatus(static_cast<i32>(code));
            #else
            return nullopt;
            #endif
        }

        /**
         * @brief Wait up to @p timeout for the child to exit.
         *
         * Returns the exit status if the child finishes within @p timeout, or an
         * empty Optional if the timeout elapses first (the child is still running).
         * Unlike @ref wait, this will not block indefinitely; unlike @ref try_wait,
         * it gives the child a bounded window to finish. A non-positive @p timeout
         * behaves like a single @ref try_wait poll.
         *
         * @param timeout The maximum time to wait.
         * @return Expected<Optional<ExitStatus>, ErrorCode> The exit status, nullopt
         * on timeout, or an error.
         */
        [[nodiscard]]
        Expected<Optional<ExitStatus>, ErrorCode> wait_timeout(Milliseconds timeout) {
            #if defined(__unix__) || defined(__APPLE__)
            if (_pid == -1) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            // Poll with WNOHANG against a steady deadline, backing off between
            // polls. waitpid has no timeout variant that is portable across
            // Linux and the BSDs, so a bounded poll loop is the lowest-common-
            // denominator that stays correct everywhere.
            const Instant<SteadyClock> deadline = SteadyClock::now() + timeout;
            // Cap the sleep so we stay responsive without busy-spinning.
            constexpr u32 max_backoff_us = 10'000; // 10ms
            u32 backoff_us = 500;
            while (true) {
                i32 status = 0;
                unix::ProcessId ret;
                do {
                    ret = unix::sys::waitpid(_pid, &status, unix::sys::WNOHANG);
                } while (ret == -1 && unix::errnov() == unix::EINTR);
                if (ret == -1) {
                    return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
                }
                if (ret != 0) {
                    _pid = -1;
                    return ExitStatus(status);
                }
                if (SteadyClock::now() >= deadline) {
                    return nullopt;
                }
                unix::usleep(backoff_us);
                if (backoff_us < max_backoff_us) {
                    backoff_us *= 2;
                    if (backoff_us > max_backoff_us) {
                        backoff_us = max_backoff_us;
                    }
                }
            }
            #elifdef _WIN32
            if (!_process) {
                return Unexpected(Ops::error_code(Errc::NO_CHILD_PROCESS));
            }
            const i64 ms = timeout.count();
            const win32::DWord wait_ms = ms <= 0
                ? 0
                : static_cast<win32::DWord>(ms);
            const win32::DWord res = win32::WaitForSingleObject(static_cast<win32::Handle>(_process), wait_ms);
            if (res == win32::WAIT_TIMEOUT) {
                return nullopt;
            }
            if (res == win32::WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            win32::DWord code = 0;
            win32::GetExitCodeProcess(static_cast<win32::Handle>(_process), &code);
            return ExitStatus(static_cast<i32>(code));
            #else
            return Unexpected(Ops::error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Send SIGKILL / TerminateProcess to the child.
         * @return Expected<void, ErrorCode> An error if the operation failed.
         */
        [[nodiscard]]
        Expected<void, ErrorCode> kill() {
            #if defined(__unix__) || defined(__APPLE__)
            if (_pid == -1) {
                return {};
            }
            if (unix::kill(_pid, Signal::KILL) == -1) {
                return Unexpected(ErrorCode(unix::errnov(), Ops::system_category()));
            }
            return {};
            #elifdef _WIN32
            if (!_process) {
                return {};
            }
            if (!win32::TerminateProcess(static_cast<win32::Handle>(_process), 1)) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            return {};
            #else
            return Unexpected(Ops::error_code(Errc::FUNCTION_NOT_SUPPORTED));
            #endif
        }

        /**
         * @brief Consume the Process: drain all piped output concurrently (to prevent
         * pipe-buffer deadlocks), wait for exit, and return an Output.
         * @return Expected<Output, ErrorCode> The output or an error.
         */
        [[nodiscard]]
        Expected<Output, ErrorCode> wait_with_output() {
            Vector<u8> out_data;
            Vector<u8> err_data;
            Thread out_thread;
            Thread err_thread;

            #if defined(__unix__) || defined(__APPLE__)
            i32 out_fd = Ops::exchange(_stdout_rd, -1);
            i32 err_fd = Ops::exchange(_stderr_rd, -1);
            if (out_fd != -1) {
                out_thread = Thread(drain_to, out_fd, &out_data);
            }
            if (err_fd != -1) {
                err_thread = Thread(drain_to, err_fd, &err_data);
            }
            #elifdef _WIN32
            win32::Handle out_h = Ops::exchange(_stdout_rd, nullptr);
            win32::Handle err_h = Ops::exchange(_stderr_rd, nullptr);
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
}
