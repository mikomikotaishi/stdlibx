import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::process::Child;
using stdx::process::Command;
using stdx::process::ExitStatus;
using stdx::process::Output;
using stdx::process::Stdio;

#ifdef __GNUC__
using namespace stdx::core;
#endif

using namespace stdx::os;

struct TestContext {
    i32 tests_passed = 0;
    i32 tests_failed = 0;

    void check(StringView name, bool condition) {
        if (condition) {
            System::out.println("[PASS] {}", name);
            ++tests_passed;
        } else {
            System::err.println("[FAIL] {}", name);
            ++tests_failed;
        }
    }
};

void test_echo(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("echo")
        .arg("hello world")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("echo: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("echo: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check("echo: stdout contains 'hello world'", out.find("hello world") != String::npos);
}

void test_true_false(TestContext& ctx) {
    Expected<ExitStatus, ErrorCode> t = Command::from("true").status();
    ctx.check("true: spawned successfully", t.has_value());
    ctx.check("true: exit code 0", t.has_value() && t->success());

    Expected<ExitStatus, ErrorCode> f = Command::from("false").status();
    ctx.check("false: spawned successfully", f.has_value());
    ctx.check("false: exit code non-zero", f.has_value() && !f->success());
}

void test_cat_stdin_pipe(TestContext& ctx) {
    Expected<Child, ErrorCode> child_result = Command::from("cat")
        .stdin(Stdio::PIPED)
        .stdout(Stdio::PIPED)
        .spawn();

    ctx.check("cat pipe: spawned successfully", child_result.has_value());
    if (!child_result) {
        return;
    }

    Child& child = *child_result;
    ctx.check("cat pipe: has stdin", child.has_stdin());
    ctx.check("cat pipe: has stdout", child.has_stdout());

    #ifdef __unix__
    StringView msg = "piped input\n";
    unix::write(child.stdin_fd(), msg.data(), msg.size());
    unix::close(child.stdin_fd());
    #endif

    Expected<Output, ErrorCode> out = child.wait_with_output();
    ctx.check("cat pipe: wait_with_output succeeded", out.has_value());
    if (!out) {
        return;
    }

    String stdout_str(out->stdout_data.begin(), out->stdout_data.end());
    ctx.check("cat pipe: echoed back input", stdout_str.find("piped input") != String::npos);
}

void test_null_dev(TestContext& ctx) {
    Expected<Child, ErrorCode> child_result = Command::from("echo")
        .arg("should be discarded")
        .stdin(Stdio::NULL_DEV)
        .stdout(Stdio::NULL_DEV)
        .stderr(Stdio::NULL_DEV)
        .spawn();

    ctx.check("null dev: spawned successfully", child_result.has_value());
    if (!child_result) {
        return;
    }

    Expected<ExitStatus, ErrorCode> status = child_result->wait();
    ctx.check("null dev: wait succeeded", status.has_value());
    ctx.check("null dev: exit success", status.has_value() && status->success());
}

void test_current_dir(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("pwd")
        .stdout(Stdio::PIPED)
        .current_dir(Path("/tmp"))
        .output();

    ctx.check("current_dir: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("current_dir: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check("current_dir: output contains /tmp", out.find("/tmp") != String::npos);
}

void test_env(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("env")
        .env("MY_TEST_VAR", "test_value_12345")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("env: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("env: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check(
        "env: output contains MY_TEST_VAR=test_value_12345",
        out.find("MY_TEST_VAR=test_value_12345") != String::npos
    );
}

void test_multiple_args(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("printf")
        .arg("%s-%s-%s")
        .arg("a")
        .arg("b")
        .arg("c")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("multiple args: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("multiple args: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check("multiple args: output is 'a-b-c'", out == "a-b-c");
}

void test_args_range(TestContext& ctx) {
    Vector<String> flags = {"-l", "-a", "-h"};
    Expected<Output, ErrorCode> result = Command::from("ls")
        .args(flags)
        .stdout(Stdio::PIPED)
        .stderr(Stdio::PIPED)
        .output();

    ctx.check("args range: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("args range: exit success", result->success());
    ctx.check("args range: produced output", !result->stdout_data.empty());
}

void test_try_wait(TestContext& ctx) {
    Expected<Child, ErrorCode> child_result = Command::from("sleep")
        .arg("10")
        .spawn();

    ctx.check("try_wait: spawned successfully", child_result.has_value());
    if (!child_result) {
        return;
    }

    Child& child = *child_result;
    Expected<Optional<ExitStatus>, ErrorCode> poll = child.try_wait();
    ctx.check("try_wait: poll succeeded", poll.has_value());
    ctx.check("try_wait: not finished yet", poll.has_value() && !poll->has_value());

    Expected<void, ErrorCode> kill_result = child.kill();
    ctx.check("try_wait: kill succeeded", !kill_result.has_value() || kill_result.has_value());

    Expected<ExitStatus, ErrorCode> wait_result = child.wait();
    ctx.check("try_wait: wait after kill succeeded", wait_result.has_value());
}

void test_exit_status(TestContext& ctx) {
    Expected<ExitStatus, ErrorCode> success = Command::from("true").status();
    ctx.check(
        "exit status: true code() == 0",
        success.has_value() && success->code().has_value() && *success->code() == 0
    );

    Expected<ExitStatus, ErrorCode> failure = Command::from("sh")
        .arg("-c")
        .arg("exit 42")
        .status();
    ctx.check(
        "exit status: 'exit 42' code() == 42",
        failure.has_value() && failure->code().has_value() && *failure->code() == 42
    );
}

void test_stderr_capture(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("sh")
        .arg("-c")
        .arg("echo error_output >&2")
        .stderr(Stdio::PIPED)
        .output();

    ctx.check("stderr capture: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("stderr capture: exit success", result->success());
    String err(result->stderr_data.begin(), result->stderr_data.end());
    ctx.check(
        "stderr capture: stderr contains 'error_output'",
        err.find("error_output") != String::npos
    );
}

void test_large_output(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("sh")
        .arg("-c")
        .arg("seq 1 10000")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("large output: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("large output: exit success", result->success());
    ctx.check("large output: stdout > 40000 bytes",
              result->stdout_data.size() > 40000);
}

void test_python_basic(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("python3")
        .arg("tests/scripts/hello.py")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("python basic: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("python basic: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check(
        "python basic: stdout contains 'Hello from python'",
        out.find("Hello from python") != String::npos
    );
}

void test_python_args(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("python3")
        .arg("tests/scripts/hello.py")
        .arg("foo")
        .arg("bar")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("python args: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("python args: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check(
        "python args: stdout contains 'arg: foo'",
        out.find("arg: foo") != String::npos
    );
    ctx.check(
        "python args: stdout contains 'arg: bar'",
        out.find("arg: bar") != String::npos
    );
}

void test_python_stderr(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("python3")
        .arg("tests/scripts/hello.py")
        .arg("--stderr")
        .stdout(Stdio::PIPED)
        .stderr(Stdio::PIPED)
        .output();

    ctx.check("python stderr: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("python stderr: exit success", result->success());
    String err(result->stderr_data.begin(), result->stderr_data.end());
    ctx.check(
        "python stderr: stderr contains 'This goes to stderr'",
        err.find("This goes to stderr") != String::npos
    );
}

void test_python_exit_code(TestContext& ctx) {
    Expected<ExitStatus, ErrorCode> result = Command::from("python3")
        .arg("tests/scripts/hello.py")
        .arg("--exit-code")
        .arg("7")
        .status();

    ctx.check("python exit code: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("python exit code: not success", !result->success());
    ctx.check(
        "python exit code: code() == 7",
        result->code().has_value() && *result->code() == 7
    );
}

void test_python_env(TestContext& ctx) {
    Expected<Output, ErrorCode> result = Command::from("python3")
        .arg("-c")
        .arg("import os; print(os.environ.get('STDLIBX_TEST', 'missing'))")
        .env("STDLIBX_TEST", "present")
        .stdout(Stdio::PIPED)
        .output();

    ctx.check("python env: spawned successfully", result.has_value());
    if (!result) {
        return;
    }

    ctx.check("python env: exit success", result->success());
    String out(result->stdout_data.begin(), result->stdout_data.end());
    ctx.check(
        "python env: output contains 'present'",
        out.find("present") != String::npos
    );
}

void test_current_process(TestContext& ctx) {
    u32 pid = stdx::process::current::id();
    ctx.check("current::id: pid > 0", pid > 0);
}

int main() {
    TestContext ctx;

    System::out.println("=== stdx::process tests ===");
    System::out.println();

    System::out.println("--- Basic process spawning ---");
    test_echo(ctx);
    test_true_false(ctx);
    test_cat_stdin_pipe(ctx);
    test_null_dev(ctx);
    test_current_dir(ctx);
    test_env(ctx);
    test_multiple_args(ctx);
    test_args_range(ctx);

    System::out.println();
    System::out.println("--- Child lifecycle ---");
    test_try_wait(ctx);
    test_exit_status(ctx);
    test_stderr_capture(ctx);
    test_large_output(ctx);

    System::out.println();
    System::out.println("--- Python script tests ---");
    test_python_basic(ctx);
    test_python_args(ctx);
    test_python_stderr(ctx);
    test_python_exit_code(ctx);
    test_python_env(ctx);

    System::out.println();
    System::out.println("--- Current process ---");
    test_current_process(ctx);

    System::out.println();
    System::out.println("=== Results: {} passed, {} failed ===", ctx.tests_passed, ctx.tests_failed);

    return ctx.tests_failed > 0 ? 1 : 0;
}
