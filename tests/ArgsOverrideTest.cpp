import stdx;

using namespace stdx::test;

namespace {
    // Real argv is mutable, so these are (mutable) arrays rather than string literals -
    // a caller handing set_args pointers into read-only storage would be lying about
    // the shape of what it supplies.
    char PROGRAM[] = "fake-program";
    char FLAG[] = "--alpha";
    char VALUE[] = "beta gamma";
    char* SUPPLIED[] = {PROGRAM, FLAG, VALUE};

    char REPLACEMENT[] = "too-late";
    char* IGNORED[] = {REPLACEMENT};
}

void test_set_args_supplies_the_command_line() {
    const Span<const StringView> args = Environment::args();

    require(args.size() == 3, "args() reports exactly what set_args was given");
    expect(args[0] == "fake-program", "element 0 is the supplied program name");
    expect(args[1] == "--alpha", "element 1 is preserved");
    expect(args[2] == "beta gamma", "an argument containing a space stays one element");
    expect(Environment::program_name() == "fake-program", "program_name() follows the override");
}

void test_set_args_after_the_first_read_is_ignored() {
    // Documented: the table is built once, on first use. The test above has already
    // read it, so this call cannot take effect - and must not corrupt what is there.
    Environment::set_args(1, IGNORED);

    const Span<const StringView> args = Environment::args();
    expect_eq(args.size(), 3uz, "the already-built table keeps its size");
    expect(args[0] == "fake-program", "and its contents");
}

int main(int argc, char* argv[]) {
    // Before anything reads args(). The runner is given the real argc/argv so that
    // ctest's own options still reach it rather than the supplied ones.
    Environment::set_args(3, SUPPLIED);
    return run(argc, argv, {
        {"Environment.set_args_supplies_the_command_line", test_set_args_supplies_the_command_line},
        {"Environment.set_args_after_the_first_read_is_ignored", test_set_args_after_the_first_read_is_ignored},
    });
}
