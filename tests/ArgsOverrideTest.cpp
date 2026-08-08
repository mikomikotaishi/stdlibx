import stdx;

using namespace stdx::test;

namespace {
    // Real argv is mutable, so these are (mutable) arrays rather than string literals.
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
    Environment::set_args(1, IGNORED);

    const Span<const StringView> args = Environment::args();
    expect_eq(args.size(), 3uz, "the already-built table keeps its size");
    expect(args[0] == "fake-program", "and its contents");
}

void test_user_arguments_are_one_subspan_away() {
    const Span<const StringView> user_args = Environment::args().subspan(1);
    require(user_args.size() == 2, "everything but the program name");

    static constexpr StringView EXPECTED[] = {"--alpha", "beta gamma"};
    usize index = 0;
    for (const StringView argument : user_args) {
        expect(argument == EXPECTED[index], "iteration visits the user arguments in order");
        ++index;
    }
    expect_eq(index, 2uz, "and visits each exactly once");
}

int main(int argc, char* argv[]) {
    Environment::set_args(3, SUPPLIED);
    return run(argc, argv, {
        {"Environment.set_args_supplies_the_command_line", test_set_args_supplies_the_command_line},
        {"Environment.set_args_after_the_first_read_is_ignored", test_set_args_after_the_first_read_is_ignored},
        {"Environment.user_arguments_are_one_subspan_away", test_user_arguments_are_one_subspan_away},
    });
}
