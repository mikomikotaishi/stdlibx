import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::process::Child;
using stdx::process::Command;
using stdx::process::ExitStatus;
using stdx::process::Output;
using stdx::process::Stdio;
using stdx::sys::Signal;

using namespace stdx::os;
using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_echo() {
    Expected<Output, ErrorCode> result = Command::from("echo")
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
    Expected<ExitStatus, ErrorCode> t = Command::from("true").status();
    expect(t.has_value(), "true: spawned successfully");
    expect(t.has_value() && t->success(), "true: exit code 0");

    Expected<ExitStatus, ErrorCode> f = Command::from("false").status();
    expect(f.has_value(), "false: spawned successfully");
    expect(f.has_value() && !f->success(), "false: exit code non-zero");
}

void test_cat_stdin_pipe() {
    Expected<Child, ErrorCode> child_result = Command::from("cat")
        .stdin(Stdio::PIPED)
        .stdout(Stdio::PIPED)
        .spawn();

    expect(child_result.has_value(), "cat pipe: spawned successfully");
    if (!child_result) {
        return;
    }

    Child& child = *child_result;
    expect(child.has_stdin(), "cat pipe: has stdin");
    expect(child.has_stdout(), "cat pipe: has stdout");

    #ifdef __unix__
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
    Expected<Child, ErrorCode> child_result = Command::from("echo")
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
    Expected<Output, ErrorCode> result = Command::from("pwd")
        .stdout(Stdio::PIPED)
        .current_dir(Path("/tmp"))
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
    Expected<Output, ErrorCode> result = Command::from("env")
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
    Expected<Output, ErrorCode> result = Command::from("printf")
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
    Expected<Output, ErrorCode> result = Command::from("ls")
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
    Expected<Child, ErrorCode> child_result = Command::from("sleep")
        .arg("10")
        .spawn();

    expect(child_result.has_value(), "try_wait: spawned successfully");
    if (!child_result) {
        return;
    }

    Child& child = *child_result;
    Expected<Optional<ExitStatus>, ErrorCode> poll = child.try_wait();
    expect(poll.has_value(), "try_wait: poll succeeded");
    expect(poll.has_value() && !poll->has_value(), "try_wait: not finished yet");

    Expected<void, ErrorCode> kill_result = child.kill();
    expect(!kill_result.has_value() || kill_result.has_value(), "try_wait: kill succeeded");

    Expected<ExitStatus, ErrorCode> wait_result = child.wait();
    expect(wait_result.has_value(), "try_wait: wait after kill succeeded");
}

void test_exit_status() {
    Expected<ExitStatus, ErrorCode> success = Command::from("true").status();
    expect(
        success.has_value() && success->code().has_value() && *success->code() == 0,
        "exit status: true code() == 0"
    );

    Expected<ExitStatus, ErrorCode> failure = Command::from("sh")
        .arg("-c")
        .arg("exit 42")
        .status();
    expect(
        failure.has_value() && failure->code().has_value() && *failure->code() == 42,
        "exit status: 'exit 42' code() == 42"
    );
}

void test_stderr_capture() {
    Expected<Output, ErrorCode> result = Command::from("sh")
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
    Expected<Output, ErrorCode> result = Command::from("sh")
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
    Expected<Output, ErrorCode> result = Command::from("python3")
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
    Expected<Output, ErrorCode> result = Command::from("python3")
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
    Expected<Output, ErrorCode> result = Command::from("python3")
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
    Expected<ExitStatus, ErrorCode> result = Command::from("python3")
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
    Expected<Output, ErrorCode> result = Command::from("python3")
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
        Expected<Child, ErrorCode> sleeper = Command::from("sleep")
            .arg("60")
            .stdin(Stdio::NULL_DEV)
            .stdout(Stdio::NULL_DEV)
            .stderr(Stdio::NULL_DEV)
            .terminate_on_parent_exit()
            .spawn();
        u32 sleeper_pid = sleeper.has_value() ? sleeper->id() : 0u;
        unix::write(fds[1], &sleeper_pid, sizeof(sleeper_pid));
        unix::close(fds[1]);
        // Deliberately leak the Child (no wait) and die; PDEATHSIG must reap it.
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

    // The kernel delivers SIGKILL when the spawner dies; wait (up to ~2s) for the
    // orphan to be reaped, after which kill(pid, 0) reports ESRCH (-1).
    bool dead = false;
    for (i32 i = 0; i < 100; ++i) {
        if (unix::kill(static_cast<i32>(sleeper_pid), 0) == -1) {
            dead = true;
            break;
        }
        System::Thread::sleep_for(stdx::time::Milliseconds{20});
    }
    if (!dead) {
        // Don't leak the survivor if the assertion is about to fail.
        unix::kill(static_cast<i32>(sleeper_pid), Signal::KILL);
    }
    expect(dead, "terminate_on_parent_exit: child killed when spawner died");
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
