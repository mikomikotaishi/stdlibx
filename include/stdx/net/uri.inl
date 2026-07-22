#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Experimental standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class UriSyntaxException
     * @brief Exception class for URI syntax errors.
     * @extends Exception
     *
     * The UriSyntaxException class is used to represent an exception that occurs when a URI has invalid syntax.
     */
    class UriSyntaxException: public Exception {
    public:
        explicit UriSyntaxException(const String& msg = ""):
            Exception(), msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return msg.c_str();
        }
    private:
        String msg; ///< The human-readable description of the syntax error.
    };

    /**
     * @class Uri
     * @brief A parsed Uniform Resource Identifier (RFC 3986).
     *
     * Represents the generic syntax
     * `scheme ":" ["//" authority] path ["?" query] ["#" fragment]`.
     * A single value type covers every URI - hierarchical URLs, URNs, and
     * relative references alike; "URL"/"URN" are roles queried via
     * @ref has_authority / @ref is_urn, not distinct types. Construction is
     * purely syntactic (no network access); use a Uri-consuming client to
     * actually resolve or fetch a resource.
     */
    class [[nodiscard]] Uri {
    private:
        Optional<String> uri_scheme; ///< The scheme, without the trailing ':' (empty if this is a relative reference).
        Optional<String> uinfo; ///< The userinfo, without the trailing '@'.
        String uri_host; ///< The host, with any IPv6 brackets stripped.
        String uri_path; ///< The path (always present, possibly empty).
        Optional<String> qur; ///< The query, without the leading '?'.
        Optional<String> frag; ///< The fragment, without the leading '#'.
        Optional<u16> uri_port; ///< The port, if an explicit non-empty one was given.
        bool has_auth = false; ///< Whether an "//authority" component is present (it may still be empty).

        /**
         * @brief Case-insensitive ASCII string comparison.
         * @param a The first string to compare.
         * @param b The second string to compare.
         * @return true if @p a and @p b are equal ignoring ASCII case, false otherwise.
         */
        [[nodiscard]]
        static bool ascii_ci_equal(StringView a, StringView b) noexcept {
            if (a.size() != b.size()) {
                return false;
            }
            for (usize i = 0; i < a.size(); ++i) {
                char ca = a[i];
                char cb = b[i];
                if (ca >= 'A' && ca <= 'Z') {
                    ca = static_cast<char>(ca + 32);
                }
                if (cb >= 'A' && cb <= 'Z') {
                    cb = static_cast<char>(cb + 32);
                }
                if (ca != cb) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Whether @p s is a syntactically valid scheme: ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ).
         * @param s The scheme to validate.
         * @returns true if @p s is a valid scheme, false otherwise.
         */
        [[nodiscard]]
        static bool valid_scheme(StringView s) noexcept {
            if (s.empty()) {
                return false;
            }
            const char first = s.front();
            if (!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z'))) {
                return false;
            }
            for (const char c: s) {
                const bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                    || (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.';
                if (!ok) {
                    return false;
                }
            }
            return true;
        }
    public:
        Uri() = default;

        THROWS(UriSyntaxException)
        explicit Uri(StringView uri) {
            StringView s = uri;

            // RFC 3986 Appendix B decomposition, peeled right-to-left so each
            // delimiter is unambiguous by the time we look for it.
            if (const auto hash = s.find('#'); hash != StringView::npos) {
                this->frag = String(s.substr(hash + 1));
                s = s.substr(0, hash);
            }
            if (const auto question = s.find('?'); question != StringView::npos) {
                this->qur = String(s.substr(question + 1));
                s = s.substr(0, question);
            }

            // scheme: a valid scheme followed by ':' occurring before any '/'.
            if (const auto colon = s.find(':'); colon != StringView::npos) {
                const auto slash = s.find('/');
                const StringView candidate = s.substr(0, colon);
                if ((slash == StringView::npos || colon < slash) && valid_scheme(candidate)) {
                    uri_scheme = String(candidate);
                    s = s.substr(colon + 1);
                }
            }

            // authority: introduced by "//", runs to the next '/' (or the end).
            if (s.size() >= 2 && s[0] == '/' && s[1] == '/') {
                this->has_auth = true;
                s = s.substr(2);
                const auto slash = s.find('/');
                StringView authority = (slash == StringView::npos) ? s : s.substr(0, slash);
                s = (slash == StringView::npos) ? ""sv : s.substr(slash);

                if (const auto at = authority.find('@'); at != StringView::npos) {
                    this->uinfo = String(authority.substr(0, at));
                    authority = authority.substr(at + 1);
                }

                StringView port_text;
                bool has_port = false;
                if (!authority.empty() && authority.front() == '[') {
                    // IP-literal (e.g. IPv6): host is bracketed and may itself contain ':'.
                    const auto close = authority.find(']');
                    if (close == StringView::npos) {
                        throw UriSyntaxException("unterminated IPv6 literal");
                    }
                    this->uri_host = String(authority.substr(1, close - 1));
                    const StringView after = authority.substr(close + 1);
                    if (!after.empty()) {
                        if (after.front() != ':') {
                            throw UriSyntaxException("unexpected characters after IPv6 literal");
                        }
                        port_text = after.substr(1);
                        has_port = true;
                    }
                } else if (const auto colon = authority.find(':'); colon != StringView::npos) {
                    this->uri_host = String(authority.substr(0, colon));
                    port_text = authority.substr(colon + 1);
                    has_port = true;
                } else {
                    this->uri_host = String(authority);
                }

                // An empty port ("host:") is permitted and means "no explicit port".
                if (has_port && !port_text.empty()) {
                    u32 value = 0;
                    for (const char c: port_text) {
                        if (c < '0' || c > '9') {
                            throw UriSyntaxException("port number contains non-digit");
                        }
                        value = value * 10 + static_cast<u32>(c - '0');
                        if (value > 65535u) {
                            throw UriSyntaxException("port number out of range");
                        }
                    }
                    this->uri_port = static_cast<u16>(value);
                }
            }

            this->uri_path = String(s);
        }

        /**
         * @brief Parse a string into a Uri.
         * @param uri The text to parse.
         * @returns The parsed Uri, or an empty Optional if @p uri is malformed.
         */
        [[nodiscard]]
        static Optional<Uri> parse(StringView uri) noexcept {
            try {
                return Uri(uri);
            } catch (const UriSyntaxException& _) {
                return nullopt;
            }
        }

        /**
         * @brief The scheme, without the trailing ':'.
         * @returns The scheme, or an empty view for a relative reference.
         */
        [[nodiscard]]
        StringView scheme() const noexcept {
            return uri_scheme ? StringView(*uri_scheme) : StringView{};
        }

        /**
         * @brief Whether an "//authority" component is present.
         * @return true if an authority is present (it may still be empty), false otherwise.
         */
        [[nodiscard]]
        bool has_authority() const noexcept {
            return has_auth;
        }

        /**
         * @brief The userinfo, without the trailing '@'.
         * @returns The userinfo, or an empty view if absent.
         */
        [[nodiscard]]
        StringView userinfo() const noexcept {
            return uinfo ? StringView(*uinfo) : StringView{};
        }

        /**
         * @brief The host, with any IPv6 brackets stripped.
         * @returns The host, or an empty view if there is no authority.
         */
        [[nodiscard]]
        StringView host() const noexcept {
            return StringView(uri_host);
        }

        /**
         * @brief The explicit port, if one was given.
         * @return An Optional containing the port number, or an empty Optional if no explicit port was given.
         */
        [[nodiscard]]
        Optional<u16> port() const noexcept {
            return uri_port;
        }

        /**
         * @brief The path (always present, possibly empty).
         * @returns The path.
         */
        [[nodiscard]]
        StringView path() const noexcept {
            return StringView(uri_path);
        }

        /**
         * @brief The query, without the leading '?'.
         * @returns The query, or an empty Optional if no '?' was present.
         */
        [[nodiscard]]
        Optional<StringView> query() const noexcept {
            if (!qur) {
                return nullopt;
            }
            return StringView(*qur);
        }

        /**
         * @brief The fragment, without the leading '#'.
         * @returns The fragment, or an empty Optional if no '#' was present.
         */
        [[nodiscard]]
        Optional<StringView> fragment() const noexcept {
            if (!frag) {
                return nullopt;
            }
            return StringView(*frag);
        }

        /**
         * @brief Whether this URI has a scheme.
         * @returns true if a scheme is present (RFC 3986 calls this a URI, as
         * opposed to a relative reference). Note this is "has a scheme", not
         * RFC 3986's stricter "absolute-URI" (which also forbids a fragment).
         */
        [[nodiscard]]
        bool is_absolute() const noexcept {
            return uri_scheme.has_value();
        }

        /**
         * @brief Whether this URI uses the "urn" scheme.
         * @returns true if the scheme is "urn" (case-insensitive), false otherwise.
         */
        [[nodiscard]]
        bool is_urn() const noexcept {
            return uri_scheme && ascii_ci_equal(*uri_scheme, "urn");
        }

        /**
         * @brief Serialize this URI back to its string form.
         * @returns The reassembled URI text.
         */
        [[nodiscard]]
        String to_string() const {
            String out;
            if (uri_scheme) {
                out += Ops::format("{}{}", *uri_scheme, ':');
            }
            if (has_auth) {
                out += "//";
                if (uinfo) {
                    out += Ops::format("{}{}", *uinfo, '@');
                }
                if (uri_host.find(':') != String::npos) {
                    out += Ops::format("[{}]", uri_host);
                } else {
                    out += uri_host;
                }
                if (uri_port) {
                    out += Ops::format("{}{}", ':', *uri_port);
                }
            }
            out += uri_path;
            if (qur) {
                out += Ops::format("{}{}", '?', *qur);
            }
            if (frag) {
                out += Ops::format("{}{}", '#', *frag);
            }
            return out;
        }

        /**
         * @brief Component-wise equality.
         *
         * Compares the parsed components exactly; this is syntactic equality,
         * not RFC 3986 normalized equivalence (e.g. scheme/host case and
         * percent-encoding are not normalized before comparison).
         */
        [[nodiscard]]
        bool operator==(const Uri& other) const = default;
    };
}

using stdx::net::Uri;

namespace stdx::fmt {
    template <>
    struct Formatter<Uri> {
        static constexpr const char* parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        static FormatContext::iterator format(const Uri& value, FormatContext& ctx) {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<Uri> : public stdx::fmt::Formatter<Uri> {};
