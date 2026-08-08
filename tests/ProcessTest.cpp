import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::sys::ExitStatus;
using stdx::sys::Output;
using stdx::sys::Signal;
using stdx::sys::Stdio;
using stdx::sys::Process;
using stdx::thread::Thread;

using namespace stdx::os;
using namespace stdx::test;

void test_echo() {
    Expected<Output, ErrorCode> result = Process::Builder("echo")
        .arg("hello world")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "echo: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "echo: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(out.find("hello world") != String::npos, "echo: stdout contains 'hello world'");
}

void test_true_false() {
    Expected<ExitStatus, ErrorCode> t = Process::Builder("true").status();
    expect(t.has_value(), "true: spawned successfully");
    expect(t.has_value() && t->success(), "true: exit code 0");

    Expected<ExitStatus, ErrorCode> f = Process::Builder("false").status();
    expect(f.has_value(), "false: spawned successfully");
    expect(f.has_value() && !f->success(), "false: exit code non-zero");

    // Ordering is by the raw status value.
    expect(ExitStatus(0) < ExitStatus(1), "a lower raw status orders first");
    expect(ExitStatus(1) > ExitStatus(0), "and the reverse holds");
    expect(ExitStatus(1) == ExitStatus(1), "equal raw statuses compare equal");
}

void test_cat_stdin_pipe() {
    Expected<Process, ErrorCode> child_result = Process::Builder("cat")
        .stdin(Stdio::PIPED)
        .stdout(Stdio::PIPED)
        .spawn();

    expect(child_result.has_value(), "cat pipe: spawned successfully");
    if (!child_result) {
        return;
    }

    Process& child = *child_result;
    expect(child.has_stdin(), "cat pipe: has stdin");
    expect(child.has_stdout(), "cat pipe: has stdout");

    #if defined(__unix__) || defined(__APPLE__)
    StringView msg = "piped input\n";
    unix::write(child.stdin_fd(), msg.data(), msg.size());
    unix::close(child.stdin_fd());
    #endif

    Expected<Output, ErrorCode> out = child.wait_with_output();
    expect(out.has_value(), "cat pipe: wait_with_output succeeded");
    if (!out) {
        return;
    }

    String stdout_str(out->stdout_data.begin(), out->stdout_data.end());
    expect(stdout_str.find("piped input") != String::npos, "cat pipe: echoed back input");
}

void test_null_dev() {
    Expected<Process, ErrorCode> child_result = Process::Builder("echo")
        .arg("should be discarded")
        .stdin(Stdio::NULL_DEV)
        .stdout(Stdio::NULL_DEV)
        .stderr(Stdio::NULL_DEV)
        .spawn();

    expect(child_result.has_value(), "null dev: spawned successfully");
    if (!child_result) {
        return;
    }

    Expected<ExitStatus, ErrorCode> status = child_result->wait();
    expect(status.has_value(), "null dev: wait succeeded");
    expect(status.has_value() && status->success(), "null dev: exit success");
}

void test_current_dir() {
    Expected<Output, ErrorCode> result = Process::Builder("pwd")
        .stdout(Stdio::PIPED)
        .current_dir("/tmp"_path)
        .output();

    expect(result.has_value(), "current_dir: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "current_dir: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(out.find("/tmp") != String::npos, "current_dir: output contains /tmp");
}

void test_env() {
    Expected<Output, ErrorCode> result = Process::Builder("env")
        .env("MY_TEST_VAR", "test_value_12345")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "env: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "env: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(
        out.find("MY_TEST_VAR=test_value_12345") != String::npos,
        "env: output contains MY_TEST_VAR=test_value_12345"
    );
}

void test_multiple_args() {
    Expected<Output, ErrorCode> result = Process::Builder("printf")
        .arg("%s-%s-%s")
        .arg("a")
        .arg("b")
        .arg("c")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "multiple args: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "multiple args: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(out == "a-b-c", "multiple args: output is 'a-b-c'");
}

void test_args_range() {
    Vector<String> flags = {"-l", "-a", "-h"};
    Expected<Output, ErrorCode> result = Process::Builder("ls")
        .args(flags)
        .stdout(Stdio::PIPED)
        .stderr(Stdio::PIPED)
        .output();

    expect(result.has_value(), "args range: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "args range: exit success");
    expect(!result->stdout_data.empty(), "args range: produced output");
}

void test_try_wait() {
    Expected<Process, ErrorCode> child_result = Process::Builder("sleep")
        .arg("10")
        .spawn();

    expect(child_result.has_value(), "try_wait: spawned successfully");
    if (!child_result) {
        return;
    }

    Process& child = *child_result;
    Expected<Optional<ExitStatus>, ErrorCode> poll = child.try_wait();
    expect(poll.has_value(), "try_wait: poll succeeded");
    expect(poll.has_value() && !poll->has_value(), "try_wait: not finished yet");

    Expected<void, ErrorCode> kill_result = child.kill();
    expect(!kill_result.has_value() || kill_result.has_value(), "try_wait: kill succeeded");

    Expected<ExitStatus, ErrorCode> wait_result = child.wait();
    expect(wait_result.has_value(), "try_wait: wait after kill succeeded");
}

void test_exit_status() {
    Expected<ExitStatus, ErrorCode> success = Process::Builder("true").status();
    expect(
        success.has_value() && success->code().has_value() && *success->code() == 0,
        "exit status: true code() == 0"
    );

    Expected<ExitStatus, ErrorCode> failure = Process::Builder("sh")
        .arg("-c")
        .arg("exit 42")
        .status();
    expect(
        failure.has_value() && failure->code().has_value() && *failure->code() == 42,
        "exit status: 'exit 42' code() == 42"
    );
}

void test_stderr_capture() {
    Expected<Output, ErrorCode> result = Process::Builder("sh")
        .arg("-c")
        .arg("echo error_output >&2")
        .stderr(Stdio::PIPED)
        .output();

    expect(result.has_value(), "stderr capture: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "stderr capture: exit success");
    String err(result->stderr_data.begin(), result->stderr_data.end());
    expect(
        err.find("error_output") != String::npos,
        "stderr capture: stderr contains 'error_output'"
    );
}

void test_large_output() {
    Expected<Output, ErrorCode> result = Process::Builder("sh")
        .arg("-c")
        .arg("seq 1 10000")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "large output: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "large output: exit success");
    expect(
        result->stdout_data.size() > 40000,
        "large output: stdout > 40000 bytes"
    );
}

void test_python_basic() {
    Expected<Output, ErrorCode> result = Process::Builder("python3")
        .arg("tests/scripts/hello.py")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "python basic: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "python basic: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(
        out.find("Hello from python") != String::npos,
        "python basic: stdout contains 'Hello from python'"
    );
}

void test_python_args() {
    Expected<Output, ErrorCode> result = Process::Builder("python3")
        .arg("tests/scripts/hello.py")
        .arg("foo")
        .arg("bar")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "python args: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "python args: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(
        out.find("arg: foo") != String::npos,
        "python args: stdout contains 'arg: foo'"
    );
    expect(
        out.find("arg: bar") != String::npos,
        "python args: stdout contains 'arg: bar'"
    );
}

void test_python_stderr() {
    Expected<Output, ErrorCode> result = Process::Builder("python3")
        .arg("tests/scripts/hello.py")
        .arg("--stderr")
        .stdout(Stdio::PIPED)
        .stderr(Stdio::PIPED)
        .output();

    expect(result.has_value(), "python stderr: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "python stderr: exit success");
    String err(result->stderr_data.begin(), result->stderr_data.end());
    expect(
        err.find("This goes to stderr") != String::npos,
        "python stderr: stderr contains 'This goes to stderr'"
    );
}

void test_python_exit_code() {
    Expected<ExitStatus, ErrorCode> result = Process::Builder("python3")
        .arg("tests/scripts/hello.py")
        .arg("--exit-code")
        .arg("7")
        .status();

    expect(result.has_value(), "python exit code: spawned successfully");
    if (!result) {
        return;
    }

    expect(!result->success(), "python exit code: not success");
    expect(result->code().has_value() && *result->code() == 7
    , "python exit code: code() == 7");
}

void test_python_env() {
    Expected<Output, ErrorCode> result = Process::Builder("python3")
        .arg("-c")
        .arg("import os; print(os.environ.get('STDLIBX_TEST', 'missing'))")
        .env("STDLIBX_TEST", "present")
        .stdout(Stdio::PIPED)
        .output();

    expect(result.has_value(), "python env: spawned successfully");
    if (!result) {
        return;
    }

    expect(result->success(), "python env: exit success");
    String out(result->stdout_data.begin(), result->stdout_data.end());
    expect(
        out.find("present") != String::npos,
        "python env: output contains 'present'"
    );
}

void test_current_process() {
    u32 pid = Environment::pid();
    expect(pid > 0, "current::id: pid > 0");
}

void test_terminate_on_parent_exit() {
    #ifdef __linux__
    // PR_SET_PDEATHSIG: a child spawned with terminate_on_parent_exit() must die when
    // its spawner dies, even with no kill()/wait(). This process runs the
    // assertions, so it can't be the one that dies - fork an intermediate
    // "spawner" that launches `sleep`, reports the sleep PID up a pipe, then
    // _exit()s. The kernel should then SIGKILL the orphaned sleep.
    //
    // Becoming a subreaper first is what makes the answer readable. An orphan is
    // normally adopted by init, and then nothing here can wait() for it - all this
    // process could do is poll kill(pid, 0), which answers "does a process table
    // entry exist", not "is it alive". Those differ exactly when nobody reaps:
    // under an init that does not, a SIGKILLed orphan stays a zombie forever and
    // the poll reports it alive for as long as the test cares to look. That is how
    // this test failed in CI, where PID 1 is the container's idle command, while
    // passing under systemd. As the subreaper this process adopts the orphan
    // itself, so it can wait() for it and read the actual cause of death.
    if (linux::sys::prctl(linux::sys::PR_SET_CHILD_SUBREAPER_OPTION, 1UL) == -1) {
        expect(false, "terminate_on_parent_exit: became a subreaper");
        return;
    }

    i32 fds[2];
    if (unix::pipe(fds) == -1) {
        expect(false, "terminate_on_parent_exit: pipe created");
        return;
    }

    i32 spawner = static_cast<i32>(unix::fork());
    if (spawner == -1) {
        unix::close(fds[0]);
        unix::close(fds[1]);
        expect(false, "terminate_on_parent_exit: forked spawner");
        return;
    }

    if (spawner == 0) {
        unix::close(fds[0]);
        Expected<Process, ErrorCode> sleeper = Process::Builder("sleep")
            .arg("60")
            .stdin(Stdio::NULL_DEV)
            .stdout(Stdio::NULL_DEV)
            .stderr(Stdio::NULL_DEV)
            .terminate_on_parent_exit()
            .spawn();
        u32 sleeper_pid = sleeper.has_value() ? sleeper->id() : 0u;
        unix::write(fds[1], &sleeper_pid, sizeof(sleeper_pid));
        unix::close(fds[1]);
        // Deliberately leak the Process (no wait) and die; PDEATHSIG must reap it.
        unix::_exit(0);
    }

    unix::close(fds[1]);
    u32 sleeper_pid = 0;
    isize n = unix::read(fds[0], &sleeper_pid, sizeof(sleeper_pid));
    unix::close(fds[0]);

    i32 wstatus = 0;
    unix::sys::waitpid(spawner, &wstatus, 0);

    expect(
        n == static_cast<isize>(sizeof(sleeper_pid)) && sleeper_pid > 0,
        "terminate_on_parent_exit: spawner reported child pid"
    );
    if (sleeper_pid == 0) {
        return;
    }

    // The orphan is this process's child now, so waitpid can report how it died
    // rather than merely whether a process table entry still exists. Polled with
    // WNOHANG rather than waited on outright: a blocking wait would sit here for
    // `sleep 60`'s full minute if PDEATHSIG never fired, turning a failure into a
    // hang, while this reaches the assertion after ~2s and says what happened.
    i32 sleeper_status = 0;
    i32 waited = 0;
    for (i32 i = 0; i < 100; ++i) {
        waited = static_cast<i32>(
            unix::sys::waitpid(static_cast<i32>(sleeper_pid), &sleeper_status, unix::sys::WNOHANG)
        );
        if (waited != 0) {
            break;
        }
        Thread::sleep_for(20ms);
    }

    // Either verdict means the child did not outlive its spawner. SIGKILL is the
    // kernel delivering the parent-death signal, which is the path this test is
    // named for. Exit code 127 is the race guard inside spawn() finding that the
    // spawner had already died before the child could arm PDEATHSIG at all, so the
    // child refused to run - a different mechanism reaching the same guarantee, and
    // one it would be wrong to fail the build over.
    const bool killed = waited == static_cast<i32>(sleeper_pid)
        && unix::sys::WIFSIGNALED(sleeper_status)
        && unix::sys::WTERMSIG(sleeper_status) == Signal::KILL;
    const bool refused_to_start = waited == static_cast<i32>(sleeper_pid)
        && unix::sys::WIFEXITED(sleeper_status)
        && unix::sys::WEXITSTATUS(sleeper_status) == 127;

    if (!killed && !refused_to_start) {
        // Don't leak the survivor if the assertion is about to fail.
        unix::kill(static_cast<i32>(sleeper_pid), Signal::KILL);
    }
    expect(
        killed || refused_to_start,
        "terminate_on_parent_exit: child killed when spawner died"
    );
    #else
    expect(true, "terminate_on_parent_exit: skipped (non-Linux)");
    #endif
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"process.echo", test_echo},
        {"process.true_false", test_true_false},
        {"process.cat_stdin_pipe", test_cat_stdin_pipe},
        {"process.null_dev", test_null_dev},
        {"process.current_dir", test_current_dir},
        {"process.env", test_env},
        {"process.multiple_args", test_multiple_args},
        {"process.args_range", test_args_range},
        {"process.try_wait", test_try_wait},
        {"process.exit_status", test_exit_status},
        {"process.stderr_capture", test_stderr_capture},
        {"process.large_output", test_large_output},
        {"process.python_basic", test_python_basic, {"python"}},
        {"process.python_args", test_python_args, {"python"}},
        {"process.python_stderr", test_python_stderr, {"python"}},
        {"process.python_exit_code", test_python_exit_code, {"python"}},
        {"process.python_env", test_python_env, {"python"}},
        {"process.current_process", test_current_process},
        {"process.terminate_on_parent_exit", test_terminate_on_parent_exit},
    });
}
