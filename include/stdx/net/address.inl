#pragma once

/**
 * @namespace stdx::net
 * @brief Experimental standard library networking operations.
 */
export namespace stdx::net {
    class [[nodiscard]] IPv4Address {
    private:
        Array<u8, 4> bytes; ///< Big-endian octets of the IPv4 address.
    public:
        constexpr IPv4Address() noexcept = default;

        constexpr explicit IPv4Address(Array<u8, 4> bytes) noexcept:
            bytes{bytes} {}

        constexpr IPv4Address(u8 a, u8 b, u8 c, u8 d) noexcept:
            bytes{a, b, c, d} {}

        constexpr explicit IPv4Address(u32 host_order) noexcept:
            bytes{
                static_cast<u8>((host_order >> 24) & 0xff),
                static_cast<u8>((host_order >> 16) & 0xff),
                static_cast<u8>((host_order >> 8) & 0xff),
                static_cast<u8>(host_order & 0xff)
            } {}

        [[nodiscard]]
        constexpr Array<u8, 4> octets() const noexcept {
            return bytes;
        }

        [[nodiscard]]
        constexpr u32 host_order() const noexcept {
            return (static_cast<u32>(bytes[0]) << 24)
                | (static_cast<u32>(bytes[1]) << 16)
                | (static_cast<u32>(bytes[2]) << 8)
                | static_cast<u32>(bytes[3]);
        }

        [[nodiscard]]
        static constexpr IPv4Address any() noexcept {
            return IPv4Address(0, 0, 0, 0);
        }

        [[nodiscard]]
        static constexpr IPv4Address loopback() noexcept {
            return IPv4Address(127, 0, 0, 1);
        }

        [[nodiscard]]
        static constexpr IPv4Address broadcast() noexcept {
            return IPv4Address(255, 255, 255, 255);
        }

        [[nodiscard]]
        constexpr bool is_unspecified() const noexcept {
            return host_order() == 0;
        }

        [[nodiscard]]
        constexpr bool is_loopback() const noexcept {
            return bytes[0] == 127;
        }

        [[nodiscard]]
        constexpr bool is_private() const noexcept {
            return (bytes[0] == 10)
                || (bytes[0] == 172 && (bytes[1] >= 16 && bytes[1] <= 31))
                || (bytes[0] == 192 && bytes[1] == 168);
        }

        [[nodiscard]]
        constexpr u8 operator[](usize index) const noexcept {
            return bytes[index];
        }
    };

    class [[nodiscard]] IPv6Address {
    private:
        Array<u8, 16> bytes; ///< Big-endian bytes of the IPv6 address.
    public:
        constexpr IPv6Address() noexcept = default;
    };

    class [[nodiscard]] IPAddress {
    private:
        Variant<IPv4Address, IPv6Address> address;
    public:
        IPAddress(Variant<IPv4Address, IPv6Address> address):
            address{address} {}
    };

    class [[nodiscard]] Endpoint {
    private:
        IPAddress address;
        u16 port;
    public:
        Endpoint(IPAddress address, u16 port):
            address{address}, port{port} {}
    };
}
