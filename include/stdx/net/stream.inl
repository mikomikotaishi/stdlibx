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
     *
     * Bytes arrive in order, whole, and without duplication, and one logical
     * message may be spread across any number of reads. That last part is the
     * whole difficulty a codec above this has to deal with, and the reason it
     * wants to be written against a requirement rather than against a class: a
     * response parser needs no idea whether the bytes came off a TCP
     * connection, out of a TLS session, or out of a literal in a test.
     *
     * The required form is the non-throwing one. A stream is driven by a
     * reactor that has just been told the descriptor is ready, so "not ready
     * yet" is an ordinary answer rather than a failure, and an empty Optional
     * says it without unwinding. It also keeps two answers apart that must not
     * be run together: a returned 0 is end of stream, an empty Optional is come
     * back later. A model is free to offer throwing counterparts too, and
     * TcpStream does.
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
     *
     * The counterpart to @ref ByteReader, on the same terms: opt in through
     * `IS_BYTE_STREAM`, and non-throwing so that a socket which will take no
     * more right now can say so without an exception. A short write is normal
     * and the caller keeps the remainder.
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
     *
     * What a full-duplex protocol needs, and what TcpStream and TlsStream are.
     * Ask for it when the code really does both: naming it lets TlsStream be
     * substituted for TcpStream without the layer above noticing, and lets a
     * codec be exercised without a network.
     *
     * Prefer @ref ByteReader or @ref ByteWriter where only one direction is
     * used. A request-body reader reads, a chunked encoder writes, and a
     * connection whose writing half has been shut down can honestly only be
     * read - asking such code for the operation it never calls is a false
     * requirement, in the same way a function that does not mutate should take
     * a const reference.
     */
    template <typename S>
    concept ByteStream = ByteReader<S> && ByteWriter<S>;
}
