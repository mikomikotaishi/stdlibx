import stdx;

using stdx::collections::Vector;
using stdx::io::InputStringStream;
using stdx::io::Scanner;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_lines_splits_input() {
    InputStringStream in("line one\nline two\nline three");
    Scanner sc(in);

    Vector<String> lines;
    for (const String& line: sc.lines()) {
        lines.push_back(line);
    }

    expect_eq(lines.size(), 3uz, "lines() yields one element per line");
    expect(lines.size() >= 1 && lines[0] == "line one", "first line preserved");
    expect(lines.size() >= 3 && lines[2] == "line three", "last line preserved (no trailing newline)");
}

void test_tokens_splits_on_whitespace() {
    InputStringStream in("alpha beta\ngamma  delta");
    Scanner sc(in);

    Vector<String> tokens;
    for (const String& token: sc.tokens()) {
        tokens.push_back(token);
    }

    expect_eq(tokens.size(), 4uz, "tokens() splits across whitespace and lines");
    expect(tokens.size() >= 1 && tokens[0] == "alpha", "first token");
    expect(tokens.size() >= 4 && tokens[3] == "delta", "last token (after a line break and double space)");
}

void test_tokens_is_lazily_consumable() {
    InputStringStream in("one two three four");
    Scanner sc(in);

    usize seen = 0;
    String first;
    for (const String& token: sc.tokens()) {
        first = token;
        ++seen;
        break;
    }
    expect_eq(seen, 1uz, "the token generator can be consumed one element at a time");
    expect(first == "one", "early break yields the first token");
}

void test_empty_input_yields_nothing() {
    InputStringStream lines_in("");
    Scanner lines_sc(lines_in);
    usize line_count = 0;
    for (const String& _: lines_sc.lines()) {
        ++line_count;
    }
    expect_eq(line_count, 0uz, "lines() over empty input yields nothing");

    InputStringStream tokens_in("");
    Scanner tokens_sc(tokens_in);
    usize token_count = 0;
    for (const String& _: tokens_sc.tokens()) {
        ++token_count;
    }
    expect_eq(token_count, 0uz, "tokens() over empty input yields nothing");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Scanner.lines_splits_input", test_lines_splits_input},
        {"Scanner.tokens_splits_on_whitespace", test_tokens_splits_on_whitespace},
        {"Scanner.tokens_lazily_consumable", test_tokens_is_lazily_consumable},
        {"Scanner.empty_input_yields_nothing", test_empty_input_yields_nothing},
    });
}
