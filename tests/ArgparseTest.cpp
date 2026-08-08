import stdx;

using stdx::collections::Vector;
using stdx::util::ArgumentParser;
using stdx::util::CommandLineParserException;
using stdx::util::DefaultArguments;
using stdx::util::NArgsPattern;

using namespace stdx::test;

/**
 * @brief A parser with no built-in -h/--help or --version.
 *
 * The defaults call exit() through their actions, which would take the test
 * runner with them. Every test here builds its own argument set anyway.
 */
[[nodiscard]]
static ArgumentParser parser(StringView name = "prog") {
    return ArgumentParser(name, "1.0", DefaultArguments::NONE);
}

/**
 * @brief Tests that a default is used when the option is absent and dropped when it is not.
 */
void test_argparse_defaults() {
    ArgumentParser absent = parser();
    absent.add_argument("--name").default_value("world"s);
    absent.parse_args("prog");
    expect_eq(absent.get("--name"), "world", "an absent option falls back to its default");
    expect(!absent.is_used("--name"), "and is not reported as used");

    ArgumentParser given = parser();
    given.add_argument("--name").default_value("world"s);
    given.parse_args("prog", "--name", "stdlibx");
    expect_eq(given.get("--name"), "stdlibx", "a supplied value replaces the default");
    expect(given.is_used("--name"), "and is reported as used");
}

/**
 * @brief Tests flags: absent is false, present is true, no value is consumed.
 */
void test_argparse_flags() {
    ArgumentParser p = parser();
    p.add_argument("--verbose").flag();
    p.add_argument("--quiet").flag();
    p.add_argument("rest").remaining();

    p.parse_args("prog", "--verbose", "tail");
    expect(p.get<bool>("--verbose"), "a flag that was given reads as true");
    expect(!p.get<bool>("--quiet"), "a flag that was not given reads as false");

    const Vector<String> rest = p.get<Vector<String>>("rest");
    expect(rest.size() == 1 && rest.front() == "tail", "a flag consumes no following value");
}

/**
 * @brief Tests the typed scan shapes: decimal, hexadecimal and floating point.
 */
void test_argparse_scan() {
    ArgumentParser p = parser();
    p.add_argument("--count").scan<'d', i32>();
    p.add_argument("--mask").scan<'x', u32>();
    p.add_argument("--prefixed").scan<'x', u32>();
    p.add_argument("--ratio").scan<'g', f64>();
    p.add_argument("--auto").scan<'i', i32>();

    p.parse_args(
        "prog",
        "--count", "42",
        "--mask", "ff",
        "--prefixed", "0xff",
        "--ratio", "2.5",
        "--auto", "0x10"
    );

    expect_eq(p.get<i32>("--count"), 42, "a decimal scan yields an integer");
    expect_eq(p.get<u32>("--mask"), 0xffu, "a hexadecimal scan reads base 16 without a prefix");
    expect_eq(p.get<u32>("--prefixed"), 0xffu, "and with one");
    // f64 is _Float64, which std::from_chars has no overload for; the scan has
    // to route through double, so this asserts more than it looks like.
    expect_near(p.get<f64>("--ratio"), 2.5, 1e-12, "a general scan yields a double");
    expect_eq(p.get<i32>("--auto"), 16, "an 'i' scan infers the base from the prefix");
}

/**
 * @brief Tests that a scan rejects a value it cannot parse.
 */
void test_argparse_scan_rejects_garbage() {
    ArgumentParser p = parser();
    p.add_argument("--count").scan<'d', i32>();

    expect_throws<InvalidArgumentException>(
        [&] -> void { p.parse_args("prog", "--count", "twelve"); },
        "a non-numeric value for a numeric option is refused"
    );
}

/**
 * @brief Tests the --option=value spelling as well as --option value.
 */
void test_argparse_assign_syntax() {
    ArgumentParser p = parser();
    p.add_argument("--count").scan<'d', i32>();
    p.parse_args("prog", "--count=7");
    expect_eq(p.get<i32>("--count"), 7, "--option=value is accepted");

    ArgumentParser custom = parser();
    custom.set_assign_chars(":");
    custom.add_argument("--count").scan<'d', i32>();
    custom.parse_args("prog", "--count:9");
    expect_eq(custom.get<i32>("--count"), 9, "the assignment character is configurable");
}

/**
 * @brief Tests that a negative number is a value, not an option.
 *
 * The classic parser bug: '-5' begins with the prefix character, so a naive
 * scan treats it as an unknown short option and the command line becomes
 * unusable for anything that takes a negative number.
 */
void test_argparse_negative_numbers() {
    ArgumentParser p = parser();
    p.add_argument("--offset").scan<'d', i32>();
    p.parse_args("prog", "--offset", "-5");
    expect_eq(p.get<i32>("--offset"), -5, "a negative value is not mistaken for an option");

    ArgumentParser positional = parser();
    positional.add_argument("delta").scan<'g', f64>();
    positional.parse_args("prog", "-1.5");
    expect_near(positional.get<f64>("delta"), -1.5, 1e-12, "nor is a negative positional");
}

/**
 * @brief Tests positional arguments, which are filled in declaration order.
 */
void test_argparse_positionals() {
    ArgumentParser p = parser();
    p.add_argument("source");
    p.add_argument("destination");
    p.parse_args("prog", "in.txt", "out.txt");

    expect_eq(p.get("source"), "in.txt", "the first positional takes the first operand");
    expect_eq(p.get("destination"), "out.txt", "the second takes the second");
}

/**
 * @brief Tests that a missing required option is refused.
 */
void test_argparse_required() {
    ArgumentParser missing = parser();
    missing.add_argument("--config").required();
    expect_throws<CommandLineParserException>(
        [&] -> void { missing.parse_args("prog"); },
        "a required option that was not supplied is an error"
    );

    ArgumentParser supplied = parser();
    supplied.add_argument("--config").required();
    expect_no_throw(
        [&] -> void { supplied.parse_args("prog", "--config", "a.toml"); },
        "and is accepted when supplied"
    );
}

/**
 * @brief Tests that an option with no value is refused rather than silently empty.
 */
void test_argparse_missing_value() {
    ArgumentParser p = parser();
    p.add_argument("--name");
    expect_throws<CommandLineParserException>(
        [&] -> void { p.parse_args("prog", "--name"); },
        "an option at the end of the line with no value is an error"
    );
}

/**
 * @brief Tests nargs with an exact count and with a range.
 */
void test_argparse_nargs() {
    ArgumentParser exact = parser();
    exact.add_argument("--point").nargs(2uz).scan<'d', i32>();
    exact.parse_args("prog", "--point", "3", "4");
    const Vector<i32> point = exact.get<Vector<i32>>("--point");
    expect(point.size() == 2 && point[0] == 3 && point[1] == 4, "nargs(2) collects exactly two");

    ArgumentParser wrong = parser();
    wrong.add_argument("--point").nargs(2uz).scan<'d', i32>();
    expect_throws<CommandLineParserException>(
        [&] -> void { wrong.parse_args("prog", "--point", "3"); },
        "too few values for an exact nargs is an error"
    );

    ArgumentParser ranged = parser();
    ranged.add_argument("--tags").nargs(1uz, 3uz);
    ranged.parse_args("prog", "--tags", "a", "b");
    expect(ranged.get<Vector<String>>("--tags").size() == 2, "a range accepts a count inside it");
}

/**
 * @brief Tests the nargs patterns that stand for open-ended counts.
 */
void test_argparse_nargs_patterns() {
    ArgumentParser any = parser();
    any.add_argument("--files").nargs(NArgsPattern::ANY);
    any.parse_args("prog", "--files");
    expect(any.get<Vector<String>>("--files").empty(), "ANY accepts nothing at all");

    ArgumentParser one = parser();
    one.add_argument("--files").nargs(NArgsPattern::AT_LEAST_ONE);
    expect_throws<CommandLineParserException>(
        [&] -> void { one.parse_args("prog", "--files"); },
        "AT_LEAST_ONE refuses an empty list"
    );

    ArgumentParser filled = parser();
    filled.add_argument("--files").nargs(NArgsPattern::AT_LEAST_ONE);
    filled.parse_args("prog", "--files", "a", "b", "c");
    expect(filled.get<Vector<String>>("--files").size() == 3, "and takes everything that follows");
}

/**
 * @brief Tests that choices restrict the accepted values.
 */
void test_argparse_choices() {
    ArgumentParser good = parser();
    good.add_argument("--level").choices("debug", "info", "warn");
    good.parse_args("prog", "--level", "info");
    expect_eq(good.get("--level"), "info", "a listed choice is accepted");

    ArgumentParser bad = parser();
    bad.add_argument("--level").choices("debug", "info", "warn");
    expect_throws<CommandLineParserException>(
        [&] -> void { bad.parse_args("prog", "--level", "trace"); },
        "an unlisted choice is refused"
    );
}

/**
 * @brief Tests that a repeatable option accumulates instead of overwriting.
 */
void test_argparse_append() {
    ArgumentParser p = parser();
    p.add_argument("--include").append();
    p.parse_args("prog", "--include", "a", "--include", "b", "--include", "c");

    const Vector<String> includes = p.get<Vector<String>>("--include");
    expect(includes.size() == 3, "every occurrence is kept");
    expect(
        includes.size() == 3 && includes[0] == "a" && includes[2] == "c",
        "and they are kept in the order given"
    );
}

/**
 * @brief Tests that a non-repeatable option given twice is refused.
 */
void test_argparse_duplicate_option() {
    ArgumentParser p = parser();
    p.add_argument("--name");
    expect_throws<CommandLineParserException>(
        [&] -> void { p.parse_args("prog", "--name", "a", "--name", "b"); },
        "repeating an option that is not append() is an error"
    );
}

/**
 * @brief Tests that remaining() sweeps up the tail, options and all.
 */
void test_argparse_remaining() {
    ArgumentParser p = parser();
    p.add_argument("--verbose").flag();
    p.add_argument("command").remaining();

    p.parse_args("prog", "--verbose", "run", "--inner-flag", "value");

    expect(p.get<bool>("--verbose"), "the parser's own flag is still taken");
    const Vector<String> rest = p.get<Vector<String>>("command");
    expect(rest.size() == 3, "everything after it is handed on untouched");
    expect(
        rest.size() == 3 && rest[1] == "--inner-flag",
        "including things that look like options"
    );
}

/**
 * @brief Tests that an unknown option is an error, and that it can be tolerated on request.
 */
void test_argparse_unknown_options() {
    ArgumentParser strict = parser();
    strict.add_argument("--name");
    expect_throws<CommandLineParserException>(
        [&] -> void { strict.parse_args("prog", "--nmae", "typo"); },
        "an unknown option is refused by parse_args"
    );

    ArgumentParser lenient = parser();
    lenient.add_argument("--name");
    const Vector<String> unknown = lenient.parse_known_args("prog", "--name", "x", "--other", "y");
    expect_eq(lenient.get("--name"), "x", "parse_known_args still fills what it knows");
    expect(!unknown.empty(), "and hands back what it does not");
}

/**
 * @brief Tests that the nullary parse_args()/parse_known_args() read the process's
 * own command line rather than nothing at all.
 *
 * The command line is whatever this binary was invoked with - bare under ctest, but
 * carrying the runner's own options when someone passes --filter - so the assertions
 * are written against Environment::args() rather than fixed values.
 */
void test_argparse_no_explicit_arguments() {
    const Span<const StringView> real = Environment::args();
    require(!real.empty(), "the process command line is available");

    // Nothing declared, so every element after the program name comes back unknown:
    // the count alone proves element 0 was skipped, and that no more than it was.
    ArgumentParser known = parser();
    Vector<String> unknown;
    expect_no_throw([&] -> void { unknown = known.parse_known_args(); }, "parse_known_args() needs no arguments");
    require(unknown.size() == real.size() - 1, "it sees every element after the program name");

    bool in_order = true;
    for (usize i = 0; in_order && i < unknown.size(); ++i) {
        in_order = unknown[i] == real[i + 1];
    }
    expect(in_order, "and they are real[1..], in order, so it read args() and not something else");
    expect(unknown == parser().parse_known_args(real), "matching what passing Environment::args() by hand gives");

    // parse_args() is strict, so whether it throws depends on that same command line.
    // Either outcome is correct; what must hold is that both spellings reach the same
    // one, which is what distinguishes reading args() from parsing nothing at all.
    const auto outcome = [](ArgumentParser& p, auto&& parse) -> String {
        try {
            parse(p);
            return "";
        } catch (const CommandLineParserException& e) {
            return String(e.what());
        }
    };

    ArgumentParser strict = parser();
    ArgumentParser spelled_out = parser();
    expect_eq(
        outcome(strict, [](ArgumentParser& p) -> void { p.parse_args(); }),
        outcome(spelled_out, [&](ArgumentParser& p) -> void { p.parse_args(real); }),
        "parse_args() and parse_args(Environment::args()) reach the same outcome"
    );
}

/**
 * @brief Tests present(), which distinguishes "absent" from "empty".
 */
void test_argparse_present() {
    ArgumentParser p = parser();
    p.add_argument("--name");
    p.parse_args("prog");
    expect(!p.present("--name").has_value(), "an unset option with no default presents nothing");

    ArgumentParser given = parser();
    given.add_argument("--name");
    given.parse_args("prog", "--name", "x");
    const Optional<String> value = given.present("--name");
    expect(value.has_value() && *value == "x", "a set option presents its value");

    // An argument with a default can never be absent, so asking is a programmer
    // error rather than an empty answer.
    ArgumentParser defaulted = parser();
    defaulted.add_argument("--name").default_value("d"s);
    defaulted.parse_args("prog");
    expect_throws<LogicException>(
        [&] -> void { static_cast<void>(defaulted.present("--name")); },
        "present() on an argument with a default is a logic error"
    );
}

/**
 * @brief Tests store_into, which writes straight into a caller's variable.
 */
void test_argparse_store_into() {
    bool verbose = false;
    i32 count = 0;
    String name;

    ArgumentParser p = parser();
    p.add_argument("--verbose").store_into(verbose);
    p.add_argument("--count").store_into(count);
    p.add_argument("--name").store_into(name);

    p.parse_args("prog", "--verbose", "--count", "12", "--name", "stdlibx");

    expect(verbose, "a bound bool is set");
    expect_eq(count, 12, "a bound integer is parsed and set");
    expect_eq(name, "stdlibx", "a bound string is set");
}

/**
 * @brief Tests that mutually exclusive options cannot be combined.
 */
void test_argparse_mutually_exclusive() {
    ArgumentParser both = parser();
    auto& group = both.add_mutually_exclusive_group();
    group.add_argument("--json").flag();
    group.add_argument("--xml").flag();

    expect_no_throw(
        [&] -> void { both.parse_args("prog", "--json"); },
        "one of the pair is fine"
    );

    ArgumentParser clash = parser();
    auto& clash_group = clash.add_mutually_exclusive_group();
    clash_group.add_argument("--json").flag();
    clash_group.add_argument("--xml").flag();
    expect_throws<CommandLineParserException>(
        [&] -> void { clash.parse_args("prog", "--json", "--xml"); },
        "both at once is refused"
    );

    ArgumentParser required = parser();
    auto& required_group = required.add_mutually_exclusive_group(true);
    required_group.add_argument("--json").flag();
    required_group.add_argument("--xml").flag();
    expect_throws<CommandLineParserException>(
        [&] -> void { required.parse_args("prog"); },
        "a required group refuses neither"
    );
}

/**
 * @brief Tests subcommands.
 */
void test_argparse_subparsers() {
    ArgumentParser add = parser("add");
    add.add_argument("--force").flag();
    add.add_argument("path");

    ArgumentParser remove = parser("remove");
    remove.add_argument("path");

    ArgumentParser root = parser("tool");
    root.add_subparser(add);
    root.add_subparser(remove);

    root.parse_args("tool", "add", "--force", "notes.md");

    expect(root.is_subcommand_used("add"), "the named subcommand is marked used");
    expect(!root.is_subcommand_used("remove"), "the other one is not");
    expect(add.get<bool>("--force"), "the subparser owns its own options");
    expect_eq(add.get("path"), "notes.md", "and its own positionals");
}

/**
 * @brief Tests that the prefix character set is configurable.
 */
void test_argparse_prefix_chars() {
    ArgumentParser p = parser();
    p.set_prefix_chars("+");
    p.add_argument("+count").scan<'d', i32>();
    p.parse_args("prog", "+count", "3");
    expect_eq(p.get<i32>("+count"), 3, "a non-dash prefix works");
}

/**
 * @brief Tests that help and usage render without throwing, and mention what was declared.
 *
 * A smoke test rather than a formatting check: the point is that the help path
 * is exercised at all, since it is the part users see first and the part no
 * other test touches.
 */
void test_argparse_help_renders() {
    ArgumentParser p = parser("tool");
    p.add_description("Does a thing.");
    p.add_epilog("See the manual.");
    p.add_argument("--name").help("who to greet").metavar("NAME");
    p.add_argument("--hidden-one").flag().hidden();
    p.add_argument("input");

    String text;
    expect_no_throw([&] -> void { text = p.help().str(); }, "help() renders");

    expect(text.find("--name") != String::npos, "help lists a declared option");
    expect(text.find("who to greet") != String::npos, "and its help text");
    expect(text.find("Does a thing.") != String::npos, "and the description");
    expect(text.find("--hidden-one") == String::npos, "but not a hidden one");

    String usage;
    expect_no_throw([&] -> void { usage = p.usage(); }, "usage() renders");
    expect(usage.find("tool") != String::npos, "usage names the program");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Argparse.defaults", test_argparse_defaults},
        {"Argparse.flags", test_argparse_flags},
        {"Argparse.scan", test_argparse_scan},
        {"Argparse.scan_rejects_garbage", test_argparse_scan_rejects_garbage},
        {"Argparse.assign_syntax", test_argparse_assign_syntax},
        {"Argparse.negative_numbers", test_argparse_negative_numbers},
        {"Argparse.positionals", test_argparse_positionals},
        {"Argparse.required", test_argparse_required},
        {"Argparse.missing_value", test_argparse_missing_value},
        {"Argparse.nargs", test_argparse_nargs},
        {"Argparse.nargs_patterns", test_argparse_nargs_patterns},
        {"Argparse.choices", test_argparse_choices},
        {"Argparse.append", test_argparse_append},
        {"Argparse.duplicate_option", test_argparse_duplicate_option},
        {"Argparse.remaining", test_argparse_remaining},
        {"Argparse.unknown_options", test_argparse_unknown_options},
        {"Argparse.no_explicit_arguments", test_argparse_no_explicit_arguments},
        {"Argparse.present", test_argparse_present},
        {"Argparse.store_into", test_argparse_store_into},
        {"Argparse.mutually_exclusive", test_argparse_mutually_exclusive},
        {"Argparse.subparsers", test_argparse_subparsers},
        {"Argparse.prefix_chars", test_argparse_prefix_chars},
        {"Argparse.help_renders", test_argparse_help_renders},
    });
}
