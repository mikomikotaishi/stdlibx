#pragma once

using stdx::fmt::Formatter;

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class AddressSyntaxException
     * @brief Exception class for malformed address literals.
     * @extends InvalidArgumentException
     *
     * The AddressSyntaxException class is used to represent an exception that
     * occurs when text handed to an address constructor is not a valid literal.
     * It does not extend IOException (unlike UnknownHostException), as parsing a
     * literal is purely syntactic and never touches the network.
     */
    class AddressSyntaxException: public InvalidArgumentException {
    public:
        using InvalidArgumentException::InvalidArgumentException;
    };

    class IPv6Address;

    /**
     * @class IPv4Address
     * @brief A 32-bit IPv4 address (RFC 791).
     *
     * Stores the four octets in network (big-endian) order, so index 0 is the
     * leading octet of the dotted-quad form. Construction is purely syntactic
     * (no name resolution); use a resolver to turn a host name into an address.
     */
    class [[nodiscard]] IPv4Address {
    public:
        static const IPv4Address ANY; ///< The wildcard address 0.0.0.0, which binds every local interface.
        static const IPv4Address LOOPBACK; ///< The loopback address 127.0.0.1.
        static const IPv4Address BROADCAST; ///< The limited broadcast address 255.255.255.255.
    private:
        Array<u8, 4> _bytes = {}; ///< Big-endian octets of the IPv4 address.
    public:
        constexpr IPv4Address() noexcept = default;

        constexpr explicit IPv4Address(Array<u8, 4> bytes) noexcept:
            _bytes{bytes} {}

        constexpr IPv4Address(u8 a, u8 b, u8 c, u8 d) noexcept:
            _bytes{a, b, c, d} {}

        constexpr explicit IPv4Address(u32 host_order) noexcept:
            _bytes{
                static_cast<u8>((host_order >> 24) & 0xff),
                static_cast<u8>((host_order >> 16) & 0xff),
                static_cast<u8>((host_order >> 8) & 0xff),
                static_cast<u8>(host_order & 0xff)
            } {}

        /**
         * @brief Parses a dotted-quad literal.
         * @param text The text to parse, e.g. "192.168.0.1".
         * @throws AddressSyntaxException if @p text is not a valid literal.
         */
        THROWS(AddressSyntaxException)
        constexpr explicit IPv4Address(StringView text) {
            const Optional<IPv4Address> parsed = parse(text);
            if (!parsed.has_value()) {
                throw AddressSyntaxException("invalid IPv4 literal");
            }
            this->_bytes = parsed->_bytes;
        }

        /**
         * @brief Parses a dotted-quad literal.
         * @param text The text to parse.
         * @return The parsed address, or an empty Optional if @p text is malformed.
         *
         * Accepts exactly four decimal octets separated by '.', each in [0, 255].
         * Leading zeros are rejected, as are the short forms accepted by
         * inet_aton ("10.1", "0x7f.1"): both are read as octal or as a packed
         * integer by some resolvers and not by others, and that disagreement is
         * a classic way past an address allow-list.
         */
        [[nodiscard]]
        static constexpr Optional<IPv4Address> parse(StringView text) noexcept {
            Array<u8, 4> bytes = {};
            usize index = 0;
            usize position = 0;

            while (index < 4) {
                u32 value = 0;
                usize digits = 0;
                while (position < text.size() && text[position] >= '0' && text[position] <= '9') {
                    value = value * 10 + static_cast<u32>(text[position] - '0');
                    ++position;
                    ++digits;
                    if (digits > 3 || value > 255) {
                        return nullopt;
                    }
                }
                if (digits == 0 || (digits > 1 && text[position - digits] == '0')) {
                    return nullopt;
                }
                bytes[index++] = static_cast<u8>(value);

                if (index < 4) {
                    if (position >= text.size() || text[position] != '.') {
                        return nullopt;
                    }
                    ++position;
                }
            }
            if (position != text.size()) {
                return nullopt;
            }
            return IPv4Address(bytes);
        }

        /**
         * @brief The four octets in network order.
         * @return The octets, leading octet first.
         */
        [[nodiscard]]
        constexpr Array<u8, 4> octets() const noexcept {
            return _bytes;
        }

        /**
         * @brief The address as a host-order 32-bit integer.
         * @return The address, with the leading octet in the most significant byte.
         */
        [[nodiscard]]
        constexpr u32 host_order() const noexcept {
            return (static_cast<u32>(_bytes[0]) << 24)
                | (static_cast<u32>(_bytes[1]) << 16)
                | (static_cast<u32>(_bytes[2]) << 8)
                | static_cast<u32>(_bytes[3]);
        }

        /**
         * @brief Whether this is the unspecified address 0.0.0.0.
         * @return true if the address is unspecified, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_unspecified() const noexcept {
            return host_order() == 0;
        }

        /**
         * @brief Whether this address is in 127.0.0.0/8.
         * @return true if the address is loopback, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_loopback() const noexcept {
            return _bytes[0] == 127;
        }

        /**
         * @brief Whether this address is in one of the RFC 1918 private ranges.
         * @return true if the address is private, false otherwise.
         * @details The private ranges are 10/8, 172.16/12, and 192.168/16.
         */
        [[nodiscard]]
        constexpr bool is_private() const noexcept {
            return (_bytes[0] == 10)
                || (_bytes[0] == 172 && (_bytes[1] >= 16 && _bytes[1] <= 31))
                || (_bytes[0] == 192 && _bytes[1] == 168);
        }

        /**
         * @brief Whether this address is in the RFC 3927 link-local range 169.254.0.0/16.
         * @return true if the address is link-local, false otherwise.
         * @details The link-local range is used for automatic address configuration when no DHCP server is available.
         */
        [[nodiscard]]
        constexpr bool is_link_local() const noexcept {
            return _bytes[0] == 169 && _bytes[1] == 254;
        }

        /**
         * @brief Whether this address is in the multicast range 224.0.0.0/4.
         * @return true if the address is multicast, false otherwise.
         * @details The multicast range is used for one-to-many communication.
         */
        [[nodiscard]]
        constexpr bool is_multicast() const noexcept {
            return (_bytes[0] & 0xf0) == 0xe0;
        }

        /**
         * @brief Whether this is the limited broadcast address 255.255.255.255.
         * @return true if the address is broadcast, false otherwise.
         * @details The limited broadcast address is used to send a message to all hosts on the local network.
         */
        [[nodiscard]]
        constexpr bool is_broadcast() const noexcept {
            return host_order() == 0xFFFFFFFF;
        }

        /**
         * @brief The IPv4-mapped IPv6 form of this address, ::ffff:a.b.c.d.
         * @return The IPv6 address in ::ffff:0:0/96 with the embedded IPv4 address.
         * @details The IPv4-mapped form is used to represent IPv4 addresses in an IPv6 context, such as when using dual-stack sockets.
         *
         * The returned address is always in the ::ffff:0:0/96 range, and is not the deprecated IPv4-compatible form (::a.b.c.d).
         */
        [[nodiscard]]
        constexpr IPv6Address to_ipv6_mapped() const noexcept;

        /**
         * @brief The dotted-quad representation.
         * @return The address in the form "a.b.c.d".
         */
        [[nodiscard]]
        String to_string() const {
            return Ops::fmt("{}.{}.{}.{}", _bytes[0], _bytes[1], _bytes[2], _bytes[3]);
        }

        /**
         * @brief The octet at @p index, counting from the leading octet.
         * @param index The index of the octet to retrieve, in [0, 3].
         * @return The octet at @p index.
         */
        [[nodiscard]]
        constexpr u8 operator[](usize index) const noexcept {
            return _bytes[index];
        }

        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const IPv4Address& other) const noexcept {
            return _bytes <=> other._bytes;
        }

        [[nodiscard]]
        constexpr bool operator==(const IPv4Address& other) const noexcept = default;
    };

    inline constexpr IPv4Address IPv4Address::ANY{0, 0, 0, 0};
    inline constexpr IPv4Address IPv4Address::LOOPBACK{127, 0, 0, 1};
    inline constexpr IPv4Address IPv4Address::BROADCAST{255, 255, 255, 255};

    /**
     * @class IPv6Address
     * @brief A 128-bit IPv6 address (RFC 4291), with an optional zone index.
     *
     * Stores the sixteen bytes in network (big-endian) order. A non-zero scope
     * id is the RFC 4007 zone index that disambiguates a link-local address
     * between interfaces; it participates in equality, since the same
     * link-local bytes on two links are two different destinations.
     */
    class [[nodiscard]] IPv6Address {
    public:
        static const IPv6Address ANY; ///< The unspecified address ::, which binds every local interface.
        static const IPv6Address LOOPBACK; ///< The loopback address ::1.
    private:
        Array<u8, 16> _bytes = {}; ///< Big-endian bytes of the IPv6 address.
        u32 _scope_id = 0; ///< The RFC 4007 zone index, or 0 for none.

        /**
         * @brief The value of the hexadecimal digit @p c.
         * @param c The character to convert.
         * @return The digit's value, or 0xFF if @p c is not a hexadecimal digit.
         */
        [[nodiscard]]
        static constexpr u8 hex_value(char c) noexcept {
            if (c >= '0' && c <= '9') {
                return static_cast<u8>(c - '0');
            }
            if (c >= 'a' && c <= 'f') {
                return static_cast<u8>(c - 'a' + 10);
            }
            if (c >= 'A' && c <= 'F') {
                return static_cast<u8>(c - 'A' + 10);
            }
            return 0xFF;
        }

        /**
         * @brief Appends @p value to @p out as lowercase hex with no leading zeros.
         * @param out The string to append to.
         * @param value The group to append.
         */
        static void append_group(String& out, u16 value) {
            static constexpr StringView HEX_DIGITS = "0123456789abcdef";
            bool leading = true;
            for (isize shift = 12; shift >= 0; shift -= 4) {
                const u8 digit = static_cast<u8>((value >> shift) & 0xf);
                if (digit == 0 && leading && shift != 0) {
                    continue;
                }
                leading = false;
                out += HEX_DIGITS[digit];
            }
        }
    public:
        constexpr IPv6Address() noexcept = default;

        constexpr explicit IPv6Address(Array<u8, 16> bytes, u32 scope_id = 0) noexcept:
            _bytes{bytes}, _scope_id{scope_id} {}

        constexpr IPv6Address(
            u16 a,
            u16 b,
            u16 c,
            u16 d,
            u16 e,
            u16 f,
            u16 g,
            u16 h,
            u32 scope_id = 0
        ) noexcept:
            _bytes{
                static_cast<u8>(a >> 8), static_cast<u8>(a & 0xff),
                static_cast<u8>(b >> 8), static_cast<u8>(b & 0xff),
                static_cast<u8>(c >> 8), static_cast<u8>(c & 0xff),
                static_cast<u8>(d >> 8), static_cast<u8>(d & 0xff),
                static_cast<u8>(e >> 8), static_cast<u8>(e & 0xff),
                static_cast<u8>(f >> 8), static_cast<u8>(f & 0xff),
                static_cast<u8>(g >> 8), static_cast<u8>(g & 0xff),
                static_cast<u8>(h >> 8), static_cast<u8>(h & 0xff)
            },
            _scope_id{scope_id} {}

        /**
         * @brief Parses an IPv6 literal.
         * @param text The text to parse, e.g. "2001:db8::1".
         * @throws AddressSyntaxException if @p text is not a valid literal.
         */
        THROWS(AddressSyntaxException)
        constexpr explicit IPv6Address(StringView text) {
            const Optional<IPv6Address> parsed = parse(text);
            if (!parsed.has_value()) {
                throw AddressSyntaxException("invalid IPv6 literal");
            }
            this->_bytes = parsed->_bytes;
            this->_scope_id = parsed->_scope_id;
        }

        /**
         * @brief Parses an IPv6 literal.
         * @param text The text to parse.
         * @return The parsed address, or an empty Optional if @p text is malformed.
         *
         * Accepts the RFC 4291 forms: eight colon-separated hexadecimal groups,
         * at most one "::" standing for one or more elided all-zero groups, and
         * an optional trailing dotted-quad occupying the last two groups
         * (e.g. "::ffff:192.168.0.1"). A "%N" suffix sets the zone index; only
         * the numeric form is accepted here, since mapping an interface name to
         * an index needs the OS.
         */
        [[nodiscard]]
        static constexpr Optional<IPv6Address> parse(StringView text) noexcept {
            StringView s = text;
            u32 scope = 0;

            if (const auto percent = s.find('%'); percent != StringView::npos) {
                const StringView zone = s.substr(percent + 1);
                s = s.substr(0, percent);
                if (zone.empty()) {
                    return nullopt;
                }
                u64 value = 0;
                for (const char c: zone) {
                    if (c < '0' || c > '9') {
                        return nullopt;
                    }
                    value = value * 10 + static_cast<u64>(c - '0');
                    if (value > 0xFFFFFFFFull) {
                        return nullopt;
                    }
                }
                scope = static_cast<u32>(value);
            }

            Array<u8, 16> bytes = {};
            usize filled = 0;
            usize gap = 0;
            bool has_gap = false;
            usize position = 0;

            if (s.size() >= 2 && s[0] == ':' && s[1] == ':') {
                has_gap = true;
                position = 2;
            } else if (!s.empty() && s[0] == ':') {
                return nullopt;
            }

            while (position < s.size()) {
                const usize start = position;
                u32 value = 0;
                usize digits = 0;
                while (position < s.size() && hex_value(s[position]) != 0xFF) {
                    value = value * 16 + hex_value(s[position]);
                    ++position;
                    ++digits;
                    if (digits > 4) {
                        return nullopt;
                    }
                }

                // A '.' means those digits were really the head of a trailing
                // IPv4 literal, which occupies the final two groups.
                if (position < s.size() && s[position] == '.') {
                    const Optional<IPv4Address> embedded = IPv4Address::parse(s.substr(start));
                    if (!embedded.has_value() || filled + 4 > 16) {
                        return nullopt;
                    }
                    for (const u8 octet: embedded->octets()) {
                        bytes[filled++] = octet;
                    }
                    position = s.size();
                    break;
                }

                if (digits == 0 || filled + 2 > 16) {
                    return nullopt;
                }
                bytes[filled++] = static_cast<u8>(value >> 8);
                bytes[filled++] = static_cast<u8>(value & 0xff);

                if (position == s.size()) {
                    break;
                }
                if (s[position] != ':') {
                    return nullopt;
                }
                ++position;
                if (position < s.size() && s[position] == ':') {
                    if (has_gap) {
                        return nullopt;
                    }
                    has_gap = true;
                    gap = filled;
                    ++position;
                } else if (position == s.size()) {
                    return nullopt;
                }
            }

            if (has_gap) {
                // "::" has to stand for at least one elided group.
                if (filled == 16) {
                    return nullopt;
                }
                // Slide everything parsed after the "::" down to the tail. Each
                // source byte is cleared as it moves, and every index a later
                // step clears was already vacated, so the elided middle is left
                // holding the zeros it was initialized with.
                for (usize k = 0, tail = filled - gap; k < tail; ++k) {
                    bytes[15 - k] = bytes[filled - 1 - k];
                    bytes[filled - 1 - k] = 0;
                }
            } else if (filled != 16) {
                return nullopt;
            }

            return IPv6Address(bytes, scope);
        }

        /**
         * @brief The sixteen bytes in network order.
         * @return The bytes, leading byte first.
         */
        [[nodiscard]]
        constexpr Array<u8, 16> octets() const noexcept {
            return _bytes;
        }

        /**
         * @brief The eight 16-bit groups in host order, leading group first.
         * @return The groups, leading group first.
         */
        [[nodiscard]]
        constexpr Array<u16, 8> groups() const noexcept {
            Array<u16, 8> out = {};
            for (usize i = 0; i < 8; ++i) {
                out[i] = static_cast<u16>((static_cast<u16>(_bytes[i * 2]) << 8) | _bytes[i * 2 + 1]);
            }
            return out;
        }

        /**
         * @brief The RFC 4007 zone index.
         * @return The zone index, or 0 if this address is not scoped.
         * @details The zone index disambiguates a link-local address between interfaces.
         * It is only meaningful for link-local addresses, but is stored for all addresses and participates in
         * equality and hashing.
         */
        [[nodiscard]]
        constexpr u32 scope_id() const noexcept {
            return _scope_id;
        }

        /**
         * @brief A copy of this address with @p scope_id as its zone index.
         * @param scope_id The new zone index.
         * @return A copy of this address with the new zone index.
         */
        [[nodiscard]]
        constexpr IPv6Address with_scope_id(u32 scope_id) const noexcept {
            return IPv6Address(_bytes, scope_id);
        }

        /**
         * @brief Whether this is the unspecified address ::.
         * @return true if the address is unspecified, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_unspecified() const noexcept {
            for (const u8 byte: _bytes) {
                if (byte != 0) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Whether this is the loopback address ::1.
         * @return true if the address is loopback, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_loopback() const noexcept {
            for (usize i = 0; i < 15; ++i) {
                if (_bytes[i] != 0) {
                    return false;
                }
            }
            return _bytes[15] == 1;
        }

        /**
         * @brief Whether this address is in the multicast range ff00::/8.
         * @return true if the address is multicast, false otherwise.
         */
        [[nodiscard]]
        constexpr bool is_multicast() const noexcept {
            return _bytes[0] == 0xff;
        }

        /**
         * @brief Whether this address is in the link-local range fe80::/10.
         * @return true if the address is link-local, false otherwise.
         * @details The link-local range is used for automatic address configuration when no DHCP server is
         * available. Link-local addresses are only valid on the local link and are not routable.
         */
        [[nodiscard]]
        constexpr bool is_link_local() const noexcept {
            return _bytes[0] == 0xfe && (_bytes[1] & 0xc0) == 0x80;
        }

        /**
         * @brief Whether this address is in the RFC 4193 unique-local range fc00::/7.
         * @return true if the address is unique-local, false otherwise.
         * @details The unique-local range is used for private networks and is not routable on
         * the public Internet. Unique-local addresses are similar to IPv4 private addresses (RFC 1918).
         */
        [[nodiscard]]
        constexpr bool is_unique_local() const noexcept {
            return (_bytes[0] & 0xfe) == 0xfc;
        }

        /**
         * @brief Whether this address is IPv4-mapped, i.e. in ::ffff:0:0/96.
         * @return true if the address is IPv4-mapped, false otherwise.
         * @details The IPv4-mapped form is used to represent IPv4 addresses in an
         * IPv6 context, such as when using dual-stack sockets. The IPv4-mapped form is
         * always in the ::ffff:0:0/96 range, and is not the deprecated IPv4-compatible form (::a.b.c.d).
         */
        [[nodiscard]]
        constexpr bool is_v4_mapped() const noexcept {
            for (usize i = 0; i < 10; ++i) {
                if (_bytes[i] != 0) {
                    return false;
                }
            }
            return _bytes[10] == 0xff && _bytes[11] == 0xff;
        }

        /**
         * @brief Whether this address is IPv4-compatible, i.e. ::a.b.c.d.
         * @return true if the address is IPv4-compatible, false otherwise.
         * @details The IPv4-compatible form is used to represent IPv4 addresses in an IPv6 context, but it
         * is deprecated by RFC 4291; prefer @ref is_v4_mapped.
         */
        [[nodiscard]]
        constexpr bool is_v4_compatible() const noexcept {
            for (usize i = 0; i < 12; ++i) {
                if (_bytes[i] != 0) {
                    return false;
                }
            }
            return !is_unspecified() && !is_loopback();
        }

        /**
         * @brief The IPv4 address embedded in an IPv4-mapped or IPv4-compatible address.
         * @return The embedded address, or an empty Optional if there is none.
         * @details The embedded address is the last four bytes of the IPv6 address, interpreted as an IPv4 address.
         */
        [[nodiscard]]
        constexpr Optional<IPv4Address> to_ipv4() const noexcept {
            if (!is_v4_mapped() && !is_v4_compatible()) {
                return nullopt;
            }
            return IPv4Address(_bytes[12], _bytes[13], _bytes[14], _bytes[15]);
        }

        /**
         * @brief The canonical text representation (RFC 5952).
         * @return The address text.
         *
         * Groups are lowercase and stripped of leading zeros, the longest run of
         * two or more all-zero groups is elided as "::" (the leftmost such run
         * on a tie), IPv4-mapped addresses print as "::ffff:a.b.c.d", and a
         * non-zero zone index is appended as "%N".
         */
        [[nodiscard]]
        String to_string() const {
            String out;
            if (is_v4_mapped()) {
                out = "::ffff:";
                out += IPv4Address(_bytes[12], _bytes[13], _bytes[14], _bytes[15]).to_string();
            } else {
                const Array<u16, 8> g = groups();

                usize best_start = 0;
                usize best_length = 0;
                usize run_start = 0;
                usize run_length = 0;
                for (usize i = 0; i < 8; ++i) {
                    if (g[i] != 0) {
                        run_length = 0;
                        continue;
                    }
                    if (run_length == 0) {
                        run_start = i;
                    }
                    ++run_length;
                    if (run_length > best_length) {
                        best_length = run_length;
                        best_start = run_start;
                    }
                }
                if (best_length < 2) {
                    best_length = 0;
                }

                out.reserve(39);
                for (usize i = 0; i < 8; ) {
                    if (best_length != 0 && i == best_start) {
                        out += "::";
                        i += best_length;
                        continue;
                    }
                    if (!out.empty() && out.back() != ':') {
                        out += ':';
                    }
                    append_group(out, g[i]);
                    ++i;
                }
            }
            if (_scope_id != 0) {
                out += Ops::fmt("%{}", _scope_id);
            }
            return out;
        }

        /**
         * @brief The byte at @p index, counting from the leading byte.
         * @param index The index of the byte to retrieve, in [0, 15].
         * @return The byte at @p index.
         */
        [[nodiscard]]
        constexpr u8 operator[](usize index) const noexcept {
            return _bytes[index];
        }

        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const IPv6Address& other) const noexcept {
            if (const StrongOrdering bytes = _bytes <=> other._bytes; bytes != 0) {
                return bytes;
            }
            return _scope_id <=> other._scope_id;
        }

        [[nodiscard]]
        constexpr bool operator==(const IPv6Address& other) const noexcept = default;
    };

    inline constexpr IPv6Address IPv6Address::ANY{};
    inline constexpr IPv6Address IPv6Address::LOOPBACK(0, 0, 0, 0, 0, 0, 0, 1);

    constexpr IPv6Address IPv4Address::to_ipv6_mapped() const noexcept {
        return IPv6Address(Array<u8, 16>{
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0xff, 0xff,
            _bytes[0], _bytes[1], _bytes[2], _bytes[3]
        });
    }

    /**
     * @class IPAddress
     * @brief An IP address of either family.
     *
     * A single value type covers both families, so code that does not care which
     * one it holds can pass an IPAddress around and query the family only where
     * it matters. Ordering places every IPv4 address before every IPv6 address.
     */
    class [[nodiscard]] IPAddress {
    public:
        /**
         * @enum Family
         * @brief The address family of an IPAddress.
         */
        enum class Family: u8 {
            IPV4, ///< A 32-bit IPv4 address (RFC 791).
            IPV6, ///< A 128-bit IPv6 address (RFC 4291).
        };
    private:
        Variant<IPv4Address, IPv6Address> _address; ///< The held address; index selects the family.
    public:
        constexpr IPAddress() noexcept = default;

        constexpr IPAddress(Variant<IPv4Address, IPv6Address> address) noexcept:
            _address{address} {}

        constexpr IPAddress(IPv4Address address) noexcept:
            _address{address} {}

        constexpr IPAddress(IPv6Address address) noexcept:
            _address{address} {}

        /**
         * @brief Parses an IPv4 or IPv6 literal.
         * @param text The text to parse.
         * @throws AddressSyntaxException if @p text is not a valid literal.
         */
        THROWS(AddressSyntaxException)
        constexpr explicit IPAddress(StringView text) {
            if (const Optional<IPv4Address> v4 = IPv4Address::parse(text)) {
                this->_address = *v4;
                return;
            }
            if (const Optional<IPv6Address> v6 = IPv6Address::parse(text)) {
                this->_address = *v6;
                return;
            }
            throw AddressSyntaxException("invalid IP literal");
        }

        /**
         * @brief Parses an IPv4 or IPv6 literal.
         * @param text The text to parse.
         * @return The parsed address, or an empty Optional if @p text is malformed.
         *
         * Tries @ref IPv4Address::parse first, then @ref IPv6Address::parse; the
         * two grammars do not overlap, so the order only affects which one gets
         * to reject the text first.
         */
        [[nodiscard]]
        static constexpr Optional<IPAddress> parse(StringView text) noexcept {
            try {
                return IPAddress(text);
            } catch (const AddressSyntaxException& _) {
                return nullopt;
            }
        }

        /**
         * @brief The family of the held address.
         * @return Family::IPV4 if this holds an IPv4 address, Family::IPV6 if it holds an IPv6 address.
         */
        [[nodiscard]]
        constexpr Family family() const noexcept {
            return is_v4() ? Family::IPV4 : Family::IPV6;
        }

        /**
         * @brief Whether this holds an IPv4 address.
         * @return true if this holds an IPv4 address, false if it holds an IPv6 address.
         */
        [[nodiscard]]
        constexpr bool is_v4() const noexcept {
            return holds_alternative<IPv4Address>(_address);
        }

        /**
         * @brief Whether this holds an IPv6 address.
         * @return true if this holds an IPv6 address, false if it holds an IPv4 address.
         */
        [[nodiscard]]
        constexpr bool is_v6() const noexcept {
            return holds_alternative<IPv6Address>(_address);
        }

        /**
         * @brief The held address as an IPv4Address.
         * @return The address, or an empty Optional if this holds an IPv6 address.
         */
        [[nodiscard]]
        constexpr Optional<IPv4Address> to_v4() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return *address;
            }
            return nullopt;
        }

        /**
         * @brief The held address as an IPv6Address.
         * @return The address, or an empty Optional if this holds an IPv4 address.
         */
        [[nodiscard]]
        constexpr Optional<IPv6Address> to_v6() const noexcept {
            if (const IPv6Address* address = get_if<IPv6Address>(&_address)) {
                return *address;
            }
            return nullopt;
        }

        /**
         * @brief This address in IPv6 form, mapping an IPv4 address to ::ffff:a.b.c.d.
         * @return The address in IPv6 form, or an empty Optional if this holds an IPv6 address.
         *
         * Useful for a dual-stack listener, which sees IPv4 peers as IPv4-mapped
         * addresses on an AF_INET6 socket.
         */
        [[nodiscard]]
        constexpr IPv6Address to_v6_mapped() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->to_ipv6_mapped();
            }
            return *get_if<IPv6Address>(&_address);
        }

        /**
         * @brief The wildcard address of @p family.
         * @param family The family of the wildcard address to return.
         * @return The wildcard address of @p family (0.0.0.0 for IPv4, :: for IPv6).
         */
        [[nodiscard]]
        static constexpr IPAddress any(Family family) noexcept {
            if (family == Family::IPV4) {
                return IPAddress(IPv4Address::ANY);
            }
            return IPAddress(IPv6Address::ANY);
        }

        /**
         * @brief The loopback address of @p family.
         * @param family The family of the loopback address to return.
         * @return The loopback address of @p family (127.0.0.1 for IPv4, ::1 for IPv6).
         */
        [[nodiscard]]
        static constexpr IPAddress loopback(Family family) noexcept {
            if (family == Family::IPV4) {
                return IPAddress(IPv4Address::LOOPBACK);
            }
            return IPAddress(IPv6Address::LOOPBACK);
        }

        /**
         * @brief Whether this is the unspecified address of its family.
         * @return true if the address is unspecified, false otherwise.
         * @details The unspecified address is 0.0.0.0 for IPv4 and :: for IPv6.
         */
        [[nodiscard]]
        constexpr bool is_unspecified() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->is_unspecified();
            }
            return get_if<IPv6Address>(&_address)->is_unspecified();
        }

        /**
         * @brief Whether this is a loopback address of its family.
         * @return true if the address is loopback, false otherwise.
         * @details The loopback address is 127.0.0.1 for IPv4 and ::1 for IPv6.
         */
        [[nodiscard]]
        constexpr bool is_loopback() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->is_loopback();
            }
            return get_if<IPv6Address>(&_address)->is_loopback();
        }

        /**
         * @brief Whether this is a multicast address of its family.
         * @return true if the address is multicast, false otherwise.
         * @details The multicast address is 224.0.0.0/4 for IPv4 and ff00::/8 for IPv6.
         */
        [[nodiscard]]
        constexpr bool is_multicast() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->is_multicast();
            }
            return get_if<IPv6Address>(&_address)->is_multicast();
        }

        /**
         * @brief Whether this is a link-local address of its family.
         * @return true if the address is link-local, false otherwise.
         * @details The link-local address is 169.254.0.0/16 for IPv4 and fe80::/10 for IPv6.
         */
        [[nodiscard]]
        constexpr bool is_link_local() const noexcept {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->is_link_local();
            }
            return get_if<IPv6Address>(&_address)->is_link_local();
        }

        /**
         * @brief The text representation of the held address.
         * @return The dotted-quad or RFC 5952 form, with no brackets.
         * @details The dotted-quad form is used for IPv4 addresses, and the RFC 5952 form is used for IPv6 addresses.
         */
        [[nodiscard]]
        String to_string() const {
            if (const IPv4Address* address = get_if<IPv4Address>(&_address)) {
                return address->to_string();
            }
            return get_if<IPv6Address>(&_address)->to_string();
        }

        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const IPAddress& other) const noexcept {
            if (const StrongOrdering family = Ops::to_underlying(this->family()) <=> Ops::to_underlying(other.family());
                family != 0) {
                return family;
            }
            if (const IPv4Address* left = get_if<IPv4Address>(&_address)) {
                return *left <=> *get_if<IPv4Address>(&other._address);
            }
            return *get_if<IPv6Address>(&_address) <=> *get_if<IPv6Address>(&other._address);
        }

        [[nodiscard]]
        constexpr bool operator==(const IPAddress& other) const noexcept = default;
    };

    /**
     * @class Endpoint
     * @brief An IP address paired with a transport port: one end of a connection.
     */
    class [[nodiscard]] Endpoint {
    private:
        IPAddress _address; ///< The address of this endpoint.
        u16 _port = 0; ///< The transport port, in host order.

        /**
         * @brief Parses a decimal port number.
         * @param text The text to parse.
         * @return The port, or an empty Optional if @p text is not a port.
         */
        [[nodiscard]]
        static constexpr Optional<u16> parse_port(StringView text) noexcept {
            if (text.empty() || text.size() > 5) {
                return nullopt;
            }
            u32 value = 0;
            for (const char c: text) {
                if (c < '0' || c > '9') {
                    return nullopt;
                }
                value = value * 10 + static_cast<u32>(c - '0');
            }
            if (value > 65535u) {
                return nullopt;
            }
            return static_cast<u16>(value);
        }
    public:
        constexpr Endpoint() noexcept = default;

        constexpr Endpoint(IPAddress address, u16 port) noexcept:
            _address{address}, _port{port} {}

        constexpr Endpoint(IPv4Address address, u16 port) noexcept:
            _address{address}, _port{port} {}

        constexpr Endpoint(IPv6Address address, u16 port) noexcept:
            _address{address}, _port{port} {}

        /**
         * @brief Parses an "address:port" literal.
         * @param text The text to parse, e.g. "127.0.0.1:80" or "[::1]:80".
         * @throws AddressSyntaxException if @p text is not a valid endpoint.
         */
        THROWS(AddressSyntaxException)
        constexpr explicit Endpoint(StringView text) {
            if (text.empty()) {
                throw AddressSyntaxException("invalid endpoint literal");
            }

            if (text.front() == '[') {
                const auto close = text.find(']');
                if (close == StringView::npos
                    || close + 1 >= text.size()
                    || text[close + 1] != ':') {
                    throw AddressSyntaxException("invalid endpoint literal");
                }
                const Optional<IPv6Address> address = IPv6Address::parse(text.substr(1, close - 1));
                const Optional<u16> port = parse_port(text.substr(close + 2));
                if (!address.has_value() || !port.has_value()) {
                    throw AddressSyntaxException("invalid endpoint literal");
                }
                this->_address = *address;
                this->_port = *port;
                return;
            }

            const auto colon = text.rfind(':');
            if (colon == StringView::npos) {
                throw AddressSyntaxException("invalid endpoint literal");
            }
            const Optional<IPv4Address> address = IPv4Address::parse(text.substr(0, colon));
            const Optional<u16> port = parse_port(text.substr(colon + 1));
            if (!address.has_value() || !port.has_value()) {
                throw AddressSyntaxException("invalid endpoint literal");
            }
            this->_address = *address;
            this->_port = *port;
        }

        /**
         * @brief Parses an "address:port" literal.
         * @param text The text to parse.
         * @return The parsed endpoint, or an empty Optional if @p text is malformed.
         *
         * An IPv6 address has to be bracketed ("[::1]:80"), since an unbracketed
         * one is ambiguous with the port separator. The port is required, and the
         * address has to be a literal: an Endpoint is a resolved destination, so
         * host names are the resolver's business.
         */
        [[nodiscard]]
        static constexpr Optional<Endpoint> parse(StringView text) noexcept {
            try {
                return Endpoint(text);
            } catch (const AddressSyntaxException& _) {
                return nullopt;
            }
        }

        /**
         * @brief The address of this endpoint.
         * @return The address.
         */
        [[nodiscard]]
        constexpr IPAddress address() const noexcept {
            return _address;
        }

        /**
         * @brief The transport port, in host order.
         * @return The port.
         */
        [[nodiscard]]
        constexpr u16 port() const noexcept {
            return _port;
        }

        /**
         * @brief The family of the endpoint's address.
         * @return Family::IPV4 if the address is IPv4, Family::IPV6 if it is IPv6.
         */
        [[nodiscard]]
        constexpr IPAddress::Family family() const noexcept {
            return _address.family();
        }

        /**
         * @brief A copy of this endpoint with @p port as its port.
         * @param port The new port.
         * @return An endpoint with the same address and @p port.
         */
        [[nodiscard]]
        constexpr Endpoint with_port(u16 port) const noexcept {
            return Endpoint(_address, port);
        }

        /**
         * @brief The text representation, as accepted by @ref parse.
         * @return "a.b.c.d:port", or "[address]:port" for IPv6.
         */
        [[nodiscard]]
        String to_string() const {
            if (_address.is_v6()) {
                return Ops::fmt("[{}]:{}", _address.to_string(), _port);
            }
            return Ops::fmt("{}:{}", _address.to_string(), _port);
        }

        [[nodiscard]]
        constexpr StrongOrdering operator<=>(const Endpoint& other) const noexcept {
            if (const auto address = _address <=> other._address; address != 0) {
                return address;
            }
            return _port <=> other._port;
        }

        [[nodiscard]]
        constexpr bool operator==(const Endpoint& other) const noexcept = default;
    };
}

using stdx::net::IPv4Address;
using stdx::net::IPv6Address;
using stdx::net::IPAddress;
using stdx::net::Endpoint;

namespace stdx::core {
    template <>
    struct Hash<IPv4Address> {
        [[nodiscard]]
        usize operator()(const IPv4Address& address) const noexcept {
            return Hash<u32>()(address.host_order());
        }
    };

    template <>
    struct Hash<IPv6Address> {
        [[nodiscard]]
        usize operator()(const IPv6Address& address) const noexcept {
            usize h = Hash<u32>()(address.scope_id());
            for (const u8 byte: address.octets()) {
                h ^= Hash<u8>()(byte) + 0x9E3779B9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    template <>
    struct Hash<IPAddress> {
        [[nodiscard]]
        usize operator()(const IPAddress& address) const noexcept {
            const usize h = Hash<u8>()(Ops::to_underlying(address.family()));
            if (const Optional<IPv4Address> v4 = address.to_v4()) {
                return h ^ (Hash<IPv4Address>()(*v4) + 0x9E3779B9 + (h << 6) + (h >> 2));
            }
            const usize inner = Hash<IPv6Address>()(*address.to_v6());
            return h ^ (inner + 0x9E3779B9 + (h << 6) + (h >> 2));
        }
    };

    template <>
    struct Hash<Endpoint> {
        [[nodiscard]]
        usize operator()(const Endpoint& endpoint) const noexcept {
            const usize h = Hash<IPAddress>()(endpoint.address());
            return h ^ (Hash<u16>()(endpoint.port()) + 0x9E3779B9 + (h << 6) + (h >> 2));
        }
    };
}

template <>
struct stdx::core::hash<IPv4Address>: public Hash<IPv4Address> {};

template <>
struct stdx::core::hash<IPv6Address>: public Hash<IPv6Address> {};

template <>
struct stdx::core::hash<IPAddress>: public Hash<IPAddress> {};

template <>
struct stdx::core::hash<Endpoint>: public Hash<Endpoint> {};

namespace stdx::fmt {
    template <>
    struct Formatter<IPv4Address> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const IPv4Address& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };

    template <>
    struct Formatter<IPv6Address> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const IPv6Address& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };

    template <>
    struct Formatter<IPAddress> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const IPAddress& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };

    template <>
    struct Formatter<IPAddress::Family> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(IPAddress::Family value, FormatContext& ctx) const {
            StringView name;
            switch (value) {
                case IPAddress::Family::IPV4:
                    name = "IPv4";
                    break;
                case IPAddress::Family::IPV6:
                    name = "IPv6";
                    break;
            }
            return format_to(ctx.out(), "{}", name);
        }
    };

    template <>
    struct Formatter<Endpoint> {
        constexpr auto parse(FormatParseContext& ctx) noexcept {
            return ctx.begin();
        }

        auto format(const Endpoint& value, FormatContext& ctx) const {
            return format_to(ctx.out(), "{}", value.to_string());
        }
    };
}

template <>
struct stdx::fmt::formatter<IPv4Address>: public Formatter<IPv4Address> {};

template <>
struct stdx::fmt::formatter<IPv6Address>: public Formatter<IPv6Address> {};

template <>
struct stdx::fmt::formatter<IPAddress>: public Formatter<IPAddress> {};

template <>
struct stdx::fmt::formatter<IPAddress::Family>: public Formatter<IPAddress::Family> {};

template <>
struct stdx::fmt::formatter<Endpoint>: public Formatter<Endpoint> {};
