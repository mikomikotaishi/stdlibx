#pragma once

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
export namespace stdx::json {
    /**
     * @class JsonObject::Builder
     * @brief A fluent builder for an object-valued @ref Json.
     *
     * Borrows the readable, explicit construction of Jakarta JSON-P's
     * `Json.createObjectBuilder()` without adopting its immutability: this
     * accumulates members and hands back an ordinary mutable @ref Json from
     * @ref build. Each @ref add returns the builder, so members chain:
     * @code
     *   Json config = JsonObject::Builder()
     *       .add("host", "localhost")
     *       .add("port", 8080)
     *       .build();
     * @endcode
     * Members keep the order they were added, and a repeated key overwrites in
     * place, matching @ref JsonObject::set.
     */
    class JsonObject::Builder {
    private:
        JsonObject _object; ///< Members accumulated so far, in add order.
    public:
        Builder() = default;

        /**
         * @brief Adds (or overwrites) a member.
         * @param key The member key.
         * @param value The value.
         * @return A reference to this builder, for chaining.
         */
        Builder& add(StringView key, Json value) {
            _object.set(key, Ops::move(value));
            return *this;
        }

        /**
         * @brief Finishes building and returns the object value.
         * @return A Json holding the accumulated object.
         *
         * Moves the accumulated members out; the builder is left empty and may
         * be reused to build another object.
         */
        [[nodiscard]]
        Json build() {
            Json result(Ops::move(_object));
            _object = JsonObject();
            return result;
        }
    };
}
