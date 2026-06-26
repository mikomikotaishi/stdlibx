#pragma once

using stdx::sys::Signal;
using stdx::thread::Thread;

using namespace stdx::os;

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
    Vector<u8> drain_handle(win32::Handle h) noexcept {
        Vector<u8> buf;
        Array<u8, 4096> tmp;
        win32::WinDWord n = 0;
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
    void drain_handle_to(win32::Handle h, Vector<u8>* out) noexcept {
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
        win32::Handle process = nullptr; ///< The child process HANDLE.
        win32::Handle thread = nullptr; ///< The child thread HANDLE (for proper cleanup).
        win32::Handle stdin_wr = nullptr; ///< The write end of the child's stdin pipe (or nullptr if not piped).
        win32::Handle stdout_rd = nullptr; ///< The read end of the child's stdout pipe (or nullptr if not piped).
        win32::Handle stderr_rd = nullptr; ///< The read end of the child's stderr pipe (or nullptr if not piped).
        #endif

        Child() = default;

        #ifdef __unix__
        Child(i32 pid, i32 stdin_wr, i32 stdout_rd, i32 stderr_rd) noexcept:
            pid{pid}, stdin_wr{stdin_wr}, stdout_rd{stdout_rd}, stderr_rd{stderr_rd} {}
        #elifdef _WIN32
        Child(win32::Handle process, win32::Handle thread, win32::Handle stdin_wr, win32::Handle stdout_rd, win32::Handle stderr_rd) noexcept:
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
            auto close_h = [](win32::Handle& h) noexcept -> void {
                if (h) {
                    win32::CloseHandle(static_cast<win32::Handle>(h)); h = nullptr;
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
        Child(const Child&) = delete("Child is not copyable.");
        Child& operator=(const Child&) = delete("Child is not copyable.");

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
            return win32::GetProcessId(static_cast<win32::Handle>(process));
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
            unix::ProcessId ret;
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
            if (win32::WaitForSingleObject(static_cast<win32::Handle>(process), INFINITE) == win32::WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            win32::WinDWord code = 0;
            win32::GetExitCodeProcess(static_cast<win32::Handle>(process), &code);
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
            unix::ProcessId ret = unix::sys::waitpid(pid, &status, unix::sys::WNOHANG);
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
            win32::WinDWord res = win32::WaitForSingleObject(static_cast<win32::Handle>(process), 0);
            if (res == win32::WAIT_TIMEOUT) {
                return Optional<ExitStatus>{};
            }
            if (res == win32::WAIT_FAILED) {
                return Unexpected(ErrorCode(static_cast<i32>(win32::GetLastError()), Ops::system_category()));
            }
            win32::WinDWord code = 0;
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
            win32::Handle out_h = Ops::exchange(stdout_rd, nullptr);
            win32::Handle err_h = Ops::exchange(stderr_rd, nullptr);
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
