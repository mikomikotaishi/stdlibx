#pragma once

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @concept ByteReader
     * @brief The reading half of a reliable, ordered conversation in bytes.
     * @tparam S The type to check.
     */
    template <typename S>
    concept ByteReader = S::IS_BYTE_STREAM &&
        requires (S& stream, Span<byte> incoming) {
        requires SameAs<decltype(stream.try_receive(incoming)), Optional<usize>>;
    };

    /**
     * @concept ByteWriter
     * @brief The writing half of a reliable, ordered conversation in bytes.
     * @tparam S The type to check.
     */
    template <typename S>
    concept ByteWriter = S::IS_BYTE_STREAM &&
        requires (S& stream, Span<const byte> outgoing) {
        requires SameAs<decltype(stream.try_send(outgoing)), Optional<usize>>;
    };

    /**
     * @concept ByteStream
     * @brief A connected, reliable, ordered conversation in bytes, in both directions.
     * @tparam S The type to check.
     */
    template <typename S>
    concept ByteStream = ByteReader<S> && ByteWriter<S>;

    /**
     * @concept Acceptor
     * @brief Something that yields connections one at a time without blocking.
     * @tparam L The type to check.
     */
    template <typename L>
    concept Acceptor = requires (L& listener) {
        { listener.try_accept() } -> SameAs<Optional<typename L::Stream>>;
        { listener.native_handle() } -> SameAs<Socket::NativeHandle>;
    };
}
