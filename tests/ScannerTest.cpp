#include <version>

import stdx;

using stdx::collections::Vector;
using stdx::io::IOException;
using stdx::io::InputStream;
using stdx::io::InputStringStream;
using stdx::io::Scanner;
using stdx::io::StreamBuffer;

using namespace stdx::test;

namespace {
    /**
     * @class FailingBuffer
     * @brief A stream buffer that serves a fixed prefix and then fails the read.
     * @extends StreamBuffer
     *
     * istream turns an exception thrown out of underflow() into badbit, which is
     * what Scanner latches as Scanner::Error::IO_ERROR. Returning eof instead
     * would be a clean end of input - precisely the case a read failure has to
     * stay distinguishable from.
     */
    class FailingBuffer final: public StreamBuffer {
    private:
        String _data; ///< The prefix served before the failure.
        usize _position = 0; ///< How much of the prefix has been served.
        char _current = '\0'; ///< One-character get area.
    protected:
        int_type underflow() override {
            if (gptr() < egptr()) {
                return traits_type::to_int_type(*gptr());
            }
            if (_position >= _data.size()) {
                throw IOException("simulated read failure");
            }
            _current = _data[_position++];
            setg(&_current, &_current, &_current + 1);
            return traits_type::to_int_type(_current);
        }
    public:
        explicit FailingBuffer(String data):
            _data{Ops::move(data)} {}
    };
}

void test_string_source_splits_lines() {
    Scanner sc("alpha\nbeta\ngamma"s);

    Expected<String, Scanner::Error> first = sc.next_line();
    Expected<String, Scanner::Error> second = sc.next_line();
    Expected<String, Scanner::Error> third = sc.next_line();

    require(
        first.has_value() && second.has_value() && third.has_value(),
        "a String-constructed Scanner yields one line per newline"
    );
    expect_eq(*first, "alpha", "first line of a String source");
    expect_eq(*second, "beta", "second line of a String source");
    expect_eq(*third, "gamma", "last line of a String source (no trailing newline)");
    expect(!sc.next_line().has_value(), "the String source is exhausted after its last line");
}

void test_string_source_splits_tokens_across_lines() {
    Scanner sc("alpha beta\ngamma"s);

    Vector<String> tokens;
    while (Expected<String, Scanner::Error> token = sc.next()) {
        tokens.push_back(*token);
    }

    expect_eq(tokens.size(), 3uz, "a String source tokenizes across its line breaks");
    expect(tokens.size() >= 1 && tokens[0] == "alpha", "first token of a String source");
    expect(tokens.size() >= 3 && tokens[2] == "gamma", "token on the second line of a String source");
}

void test_crlf_input_strips_carriage_returns() {
    Scanner sc("one\r\ntwo\r\n"s);

    Expected<String, Scanner::Error> first = sc.next_line();
    Expected<String, Scanner::Error> second = sc.next_line();

    require(first.has_value() && second.has_value(), "CRLF input yields two lines");
    expect_eq(*first, "one", "the carriage return is not part of the line");
    expect_eq(*second, "two", "the carriage return is stripped on every line");
    expect(!sc.next_line().has_value(), "a trailing CRLF does not produce an extra line");
}

void test_failed_parse_leaves_the_token() {
    Scanner sc("abc 42"s);

    expect(!sc.next_int().has_value(), "a non-numeric token does not parse as an int");

    Expected<String, Scanner::Error> token = sc.next();
    require(token.has_value(), "the rejected token is still available");
    expect_eq(*token, "abc", "a failed next_int() does not consume the token");

    Expected<i32, Scanner::Error> value = sc.next_int();
    require(value.has_value(), "the following token still parses");
    expect_eq(*value, 42, "scanning resumes at the next token");
}

void test_has_next_int_does_not_consume() {
    Scanner sc("42 x"s);

    expect(sc.has_next_int(), "has_next_int() sees the leading integer");
    expect(sc.has_next_int(), "has_next_int() is repeatable");

    Expected<i32, Scanner::Error> value = sc.next_int();
    require(value.has_value(), "the integer is still there after two peeks");
    expect_eq(*value, 42, "has_next_int() left the token in place");

    expect(!sc.has_next_int(), "the next token is not an integer");
    Expected<String, Scanner::Error> rest = sc.next();
    require(rest.has_value(), "the non-integer token is still available");
    expect_eq(*rest, "x", "a false has_next_int() does not consume the token");
}

void test_blank_lines_do_not_hide_later_tokens() {
    InputStringStream in("alpha\n\n\nbeta");
    Scanner sc(in);

    Vector<String> tokens;
    while (Expected<String, Scanner::Error> token = sc.next()) {
        tokens.push_back(*token);
    }

    expect_eq(tokens.size(), 2uz, "a run of blank lines is skipped rather than ending the scan");
    expect(tokens.size() >= 1 && tokens[0] == "alpha", "token before the blank lines");
    expect(tokens.size() >= 2 && tokens[1] == "beta", "token after the blank lines");
}

void test_closed_scanner_throws() {
    Scanner sc("alpha beta"s);

    Expected<String, Scanner::Error> first = sc.next();
    require(first.has_value(), "the scanner reads before it is closed");

    sc.close();

    expect_throws<IllegalStateException>(
        [&] { static_cast<void>(sc.next()); },
        "next() on a closed scanner throws"
    );
    expect_throws<IllegalStateException>(
        [&] { static_cast<void>(sc.has_next()); },
        "has_next() on a closed scanner throws"
    );
    expect_throws<IllegalStateException>(
        [&] { static_cast<void>(sc.next_line()); },
        "next_line() on a closed scanner throws"
    );
    expect_throws<IllegalStateException>(
        [&] { static_cast<void>(sc.next_int()); },
        "next_int() on a closed scanner throws"
    );
    expect_no_throw(
        [&] { sc.close(); },
        "closing an already closed scanner is harmless"
    );
}

void test_delimiter_reports_the_current_set() {
    Scanner sc("x"s);

    expect_eq(sc.delimiter(), " \t\n\r\f\v", "the default delimiter set is whitespace");
    sc.use_delimiter(",;");
    expect_eq(sc.delimiter(), ",;", "use_delimiter() replaces the set");
}

void test_use_delimiter_splits_on_characters() {
    Scanner sc("alpha,beta,gamma"s);
    sc.use_delimiter(",");

    Vector<String> tokens;
    while (Expected<String, Scanner::Error> token = sc.next()) {
        tokens.push_back(*token);
    }

    expect_eq(tokens.size(), 3uz, "a comma delimiter splits the line into three tokens");
    expect(tokens.size() >= 1 && tokens[0] == "alpha", "first comma-separated token");
    expect(tokens.size() >= 3 && tokens[2] == "gamma", "last comma-separated token");
}

void test_use_delimiter_keeps_whitespace_inside_tokens() {
    Scanner sc("hello world,42"s);
    sc.use_delimiter(",");

    Expected<String, Scanner::Error> first = sc.next();
    require(first.has_value(), "the first comma-separated token is available");
    expect_eq(*first, "hello world", "whitespace is not a delimiter once the set is replaced");
}

void test_tokens_never_span_a_line_break() {
    Scanner sc("alpha,beta\ngamma"s);
    sc.use_delimiter(",");

    Vector<String> tokens;
    while (Expected<String, Scanner::Error> token = sc.next()) {
        tokens.push_back(*token);
    }

    expect_eq(tokens.size(), 3uz, "a line break ends a token whatever the delimiter set");
    expect(tokens.size() >= 2 && tokens[1] == "beta", "the token before the line break stops there");
    expect(tokens.size() >= 3 && tokens[2] == "gamma", "the next line starts a new token");
}

void test_delimiter_can_change_mid_scan() {
    Scanner sc("alpha beta,gamma"s);

    Expected<String, Scanner::Error> first = sc.next();
    require(first.has_value(), "the first whitespace-delimited token is available");
    expect_eq(*first, "alpha", "scanning starts with the default delimiter set");

    sc.use_delimiter(",");
    Expected<String, Scanner::Error> second = sc.next();
    require(second.has_value(), "a token is available under the new delimiter set");
    expect_eq(*second, " beta", "the new set applies from the next token, so the space is kept");
}

void test_lone_carriage_returns_split_lines() {
    Scanner sc("one\rtwo\rthree"s);

    Vector<String> lines;
    while (Expected<String, Scanner::Error> line = sc.next_line()) {
        lines.push_back(*line);
    }

    expect_eq(lines.size(), 3uz, "a lone carriage return terminates a line");
    expect(lines.size() >= 1 && lines[0] == "one", "first CR-terminated line");
    expect(lines.size() >= 3 && lines[2] == "three", "last line after CR terminators");
}

void test_mixed_line_terminators() {
    Scanner sc("one\r\ntwo\rthree\nfour"s);

    Vector<String> lines;
    while (Expected<String, Scanner::Error> line = sc.next_line()) {
        lines.push_back(*line);
    }

    expect_eq(lines.size(), 4uz, "CRLF, CR and LF terminators are all recognised");
    expect(lines.size() >= 2 && lines[1] == "two", "line between a CRLF and a CR");
    expect(lines.size() >= 4 && lines[3] == "four", "final unterminated line");
}

void test_has_next_line_is_false_after_the_last_token() {
    Scanner unterminated("alpha beta"s);
    static_cast<void>(unterminated.next());
    static_cast<void>(unterminated.next());
    expect(
        !unterminated.has_next_line(),
        "a spent final line with no terminator reports no further line"
    );
    expect(
        !unterminated.next_line().has_value(),
        "and next_line() agrees there is nothing left"
    );

    Scanner terminated("alpha beta\n"s);
    static_cast<void>(terminated.next());
    static_cast<void>(terminated.next());
    expect(
        terminated.has_next_line(),
        "a spent line that had a terminator still has an empty remainder"
    );
    Expected<String, Scanner::Error> remainder = terminated.next_line();
    require(remainder.has_value(), "the empty remainder is readable");
    expect_eq(*remainder, "", "the remainder after the last token is empty");
}

void test_error_distinguishes_mismatch_from_exhaustion() {
    Scanner sc("abc"s);

    Expected<i32, Scanner::Error> mismatch = sc.next_int();
    require(!mismatch.has_value(), "a non-numeric token does not parse");
    expect(mismatch.error() == Scanner::Error::MISMATCH, "a present but unparsable token is a mismatch");

    static_cast<void>(sc.next());

    Expected<i32, Scanner::Error> exhausted = sc.next_int();
    require(!exhausted.has_value(), "nothing remains after the token is taken");
    expect(exhausted.error() == Scanner::Error::END_OF_INPUT, "an empty source reports end of input");

    Expected<String, Scanner::Error> no_line = sc.next_line();
    require(!no_line.has_value(), "no line remains either");
    expect(no_line.error() == Scanner::Error::END_OF_INPUT, "next_line() reports end of input too");
}

void test_read_failure_is_not_end_of_input() {
    FailingBuffer buffer("alpha beta\n"s);
    InputStream in(&buffer);
    Scanner sc(in);

    Expected<String, Scanner::Error> first = sc.next();
    Expected<String, Scanner::Error> second = sc.next();
    require(first.has_value() && second.has_value(), "the readable prefix scans normally");
    expect_eq(*first, "alpha", "first token before the failure");
    expect_eq(*second, "beta", "second token before the failure");

    Expected<String, Scanner::Error> failed = sc.next();
    require(!failed.has_value(), "the scan stops once the source fails");
    expect_eq(
        failed.error(),
        Scanner::Error::IO_ERROR,
        "a read failure is reported apart from a clean end of input"
    );
}

void test_read_failure_is_latched() {
    FailingBuffer buffer(""s);
    InputStream in(&buffer);
    Scanner sc(in);

    Expected<String, Scanner::Error> first = sc.next();
    require(!first.has_value(), "an immediately failing source yields no token");
    expect_eq(first.error(), Scanner::Error::IO_ERROR, "the first read reports the failure");

    Expected<i32, Scanner::Error> again = sc.next_int();
    require(!again.has_value(), "the source is still failed");
    expect_eq(again.error(), Scanner::Error::IO_ERROR, "the failure is latched, not forgotten");

    Expected<String, Scanner::Error> line = sc.next_line();
    require(!line.has_value(), "no line is readable either");
    expect_eq(line.error(), Scanner::Error::IO_ERROR, "next_line() reports the failure too");
}

#ifdef __cpp_lib_generator
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

void test_generator_throws_on_read_failure() {
    FailingBuffer buffer("alpha\n"s);
    InputStream in(&buffer);
    Scanner sc(in);

    expect_throws<IOException>(
        [&] {
            for (const String& _: sc.tokens()) {
                // Drain the readable prefix until the source fails.
            }
        },
        "tokens() surfaces a read failure instead of ending quietly"
    );
}
#endif

int main(int argc, char* argv[]) {
    #ifndef __cpp_lib_generator
    System::out.println("[test] Generator-based Scanner tests disabled (standard library does not support generators).");
    #endif

    return run(argc, argv, {
        {"Scanner.string_source_splits_lines", test_string_source_splits_lines},
        {"Scanner.string_source_splits_tokens_across_lines", test_string_source_splits_tokens_across_lines},
        {"Scanner.crlf_input_strips_carriage_returns", test_crlf_input_strips_carriage_returns},
        {"Scanner.failed_parse_leaves_the_token", test_failed_parse_leaves_the_token},
        {"Scanner.has_next_int_does_not_consume", test_has_next_int_does_not_consume},
        {"Scanner.blank_lines_do_not_hide_later_tokens", test_blank_lines_do_not_hide_later_tokens},
        {"Scanner.closed_scanner_throws", test_closed_scanner_throws},
        {"Scanner.delimiter_reports_the_current_set", test_delimiter_reports_the_current_set},
        {"Scanner.use_delimiter_splits_on_characters", test_use_delimiter_splits_on_characters},
        {"Scanner.use_delimiter_keeps_whitespace_inside_tokens", test_use_delimiter_keeps_whitespace_inside_tokens},
        {"Scanner.tokens_never_span_a_line_break", test_tokens_never_span_a_line_break},
        {"Scanner.delimiter_can_change_mid_scan", test_delimiter_can_change_mid_scan},
        {"Scanner.lone_carriage_returns_split_lines", test_lone_carriage_returns_split_lines},
        {"Scanner.mixed_line_terminators", test_mixed_line_terminators},
        {"Scanner.has_next_line_false_after_last_token", test_has_next_line_is_false_after_the_last_token},
        {"Scanner.error_distinguishes_mismatch_from_exhaustion", test_error_distinguishes_mismatch_from_exhaustion},
        {"Scanner.read_failure_is_not_end_of_input", test_read_failure_is_not_end_of_input},
        {"Scanner.read_failure_is_latched", test_read_failure_is_latched},
        #ifdef __cpp_lib_generator
        {"Scanner.lines_splits_input", test_lines_splits_input},
        {"Scanner.tokens_splits_on_whitespace", test_tokens_splits_on_whitespace},
        {"Scanner.tokens_lazily_consumable", test_tokens_is_lazily_consumable},
        {"Scanner.empty_input_yields_nothing", test_empty_input_yields_nothing},
        {"Scanner.generator_throws_on_read_failure", test_generator_throws_on_read_failure},
        #endif
    });
}
