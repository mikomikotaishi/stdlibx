#pragma once

/**
 * @namespace stdx::json
 * @brief JSON parsing, construction, and serialization (ECMA-404 / RFC 8259).
 */
export namespace stdx::json {
    class Json;
    class JsonPointer;

    /**
     * @class JsonObject
     * @brief An insertion-ordered collection of string-keyed @ref Json members.
     *
     * JSON object members have document order, not sort order, so this keeps
     * them in the order they were inserted and preserves it across a parse ->
     * serialize round-trip. Backed by contiguous storage: for the handful of
     * keys a typical object holds, a packed scan beats the pointer-chasing of
     * a node-based map, and building one during a parse is a sequence of
     * push-backs rather than n logarithmic insertions. Lookup is linear; if a
     * workload ever needs more, a side index can be added without disturbing
     * the order guarantee or this interface.
     *
     * Keys and values live in two parallel vectors rather than one vector of
     * pairs: `Json` is still incomplete where this type is declared (it holds a
     * JsonObject in turn), and `std::pair<String, Json>` would demand a complete
     * Json there, whereas a `Vector<Json>` tolerates the incompleteness. Member
     * i is (`keys()[i]`, `values()[i]`).
     *
     * On a duplicate key, @ref set overwrites the existing member in place
     * (last value wins) rather than appending a second entry, so the invariant
     * "keys are unique, order is first-seen" holds.
     */
    class JsonObject {
    private:
        Vector<String> _keys; ///< Member keys, in insertion order; kept unique by @ref set.
        Vector<Json> _values; ///< Member values, parallel to @ref _keys.
    public:
        class Builder;

        JsonObject() = default;

        /**
         * @brief The number of members.
         * @return The member count.
         */
        [[nodiscard]]
        usize size() const noexcept {
            return _keys.size();
        }

        /**
         * @brief Whether the object has no members.
         * @return true if empty, false otherwise.
         */
        [[nodiscard]]
        bool empty() const noexcept {
            return _keys.empty();
        }

        /**
         * @brief Removes every member.
         */
        void clear() noexcept;

        /**
         * @brief The keys, in insertion order.
         * @return A reference to the key vector.
         */
        [[nodiscard]]
        const Vector<String>& keys() const noexcept {
            return _keys;
        }

        /**
         * @brief The values, in insertion order (parallel to @ref keys).
         * @return A reference to the value vector.
         */
        [[nodiscard]]
        const Vector<Json>& values() const noexcept {
            return _values;
        }

        [[nodiscard]]
        Vector<Json>& values() noexcept {
            return _values;
        }

        /**
         * @brief The index of the member with the given key.
         * @param key The key to look for.
         * @return The 0-based index, or an empty Optional if absent.
         */
        [[nodiscard]]
        Optional<usize> index_of(StringView key) const noexcept {
            for (usize i = 0; i < _keys.size(); ++i) {
                if (_keys[i] == key) {
                    return i;
                }
            }
            return nullopt;
        }

        /**
         * @brief Whether a member with the given key is present.
         * @param key The key to look for.
         * @return true if present, false otherwise.
         */
        [[nodiscard]]
        bool contains(StringView key) const noexcept {
            return index_of(key).has_value();
        }

        /**
         * @brief A pointer to the value for a key, or nullptr if absent.
         * @param key The key to look for.
         * @return A pointer to the value, or nullptr.
         */
        [[nodiscard]]
        const Json* find(StringView key) const noexcept;

        [[nodiscard]]
        Json* find(StringView key) noexcept;

        /**
         * @brief Inserts a member, or overwrites the existing one with this key.
         * @param key The member key.
         * @param value The value to store.
         *
         * A key already present keeps its position and takes the new value
         * (last-wins); a new key is appended after every existing member.
         */
        void set(StringView key, Json value);

        /**
         * @brief Accesses the value for a key, inserting a null member if absent.
         * @param key The member key.
         * @return A reference to the value (newly a null one if the key was absent).
         *
         * Auto-vivifying, like the subscript on the owning @ref Json: reading a
         * missing key through this creates it. Use @ref find or @ref contains
         * to probe without inserting.
         */
        [[nodiscard]]
        Json& operator[](StringView key);

        /**
         * @brief Removes the member with the given key, if present.
         * @param key The key to remove.
         * @return true if a member was removed, false if the key was absent.
         *
         * Preserves the order of the remaining members.
         */
        bool erase(StringView key);

        [[nodiscard]]
        bool operator==(const JsonObject& other) const;
    };

    /**
     * @class Json
     * @brief A single JSON value: null, boolean, number, string, array, or object.
     *
     * One value type models every JSON kind, so a parsed document is just a `Json`
     * and the kind is queried (@ref type, @ref is_object, ...) rather than carried in the
     * static type.
     */
    class [[nodiscard]] Json {
    public:
        using Array = Vector<Json>; ///< The element container for a JSON array.
        using Object = JsonObject; ///< The member container for a JSON object.
        using Pointer = JsonPointer; ///< A parsed JSON pointer.

        /**
         * @enum Type
         * @brief The kind of value a @ref Json currently holds.
         *
         * JSON itself has a single number type; INTEGER and FLOAT are split here
         * because C++ callers want the distinction (and because an integer that
         * survives a round-trip without acquiring a ".0" is usually what a reader
         * expects). A freshly default-constructed Json is NULL_VALUE.
         */
        enum class Type: u8 {
            NULL_VALUE, ///< JSON null (also the state of a default-constructed Json).
            BOOLEAN, ///< true or false.
            INTEGER, ///< A number that was read, or stored, as an integer.
            FLOAT, ///< A number with a fractional part or exponent.
            STRING, ///< A string.
            ARRAY, ///< An ordered list of values.
            OBJECT, ///< An ordered collection of key/value members.
        };
    private:
        Variant<Monostate, bool, i64, f64, String, Array, Object> _value; ///< The held value; the index selects the kind (see @ref Type).
    public:
        /**
         * @brief Constructs a null value.
         */
        Json() noexcept:
            _value{Monostate{}} {}

        /**
         * @brief Constructs a null value from nullptr.
         */
        Json(NullPointer _) noexcept:
            _value{Monostate{}} {}

        /**
         * @brief Constructs a boolean value.
         * @param b The boolean.
         */
        Json(bool b) noexcept:
            _value{b} {}

        /**
         * @brief Constructs an integer value.
         * @param n The integer.
         *
         * Every signed and unsigned integral narrower than or equal to i64
         * routes here; this overload is constrained to integral types (and not
         * bool, which has its own overload) so a char or an i32 does not get
         * caught by the floating-point or string constructors instead.
         */
        template <Integral T>
            requires (NotSameAs<T, bool>)
        Json(T n) noexcept:
            _value{static_cast<i64>(n)} {}

        /**
         * @brief Constructs a floating-point value.
         * @param x The number.
         */
        template <FloatingPoint F>
        Json(F x) noexcept:
            _value{static_cast<f64>(x)} {}

        /**
         * @brief Constructs a string value.
         * @param s The string.
         */
        Json(String s) noexcept:
            _value{Ops::move(s)} {}

        /**
         * @brief Constructs a string value.
         * @param s The string.
         */
        Json(StringView s):
            _value{String(s)} {}

        /**
         * @brief Constructs a string value.
         * @param s The null-terminated string.
         *
         * Present so a bare string literal becomes a JSON string rather than
         * selecting the bool overload through the pointer-to-bool conversion.
         */
        Json(const char* s):
            _value{String(s)} {}

        /**
         * @brief Constructs an array value.
         * @param elements The elements.
         */
        Json(Array elements) noexcept:
            _value{Ops::move(elements)} {}

        /**
         * @brief Constructs an object value.
         * @param members The members.
         */
        Json(Object members) noexcept:
            _value{Ops::move(members)} {}

        /**
         * @brief Constructs a value from a braced initializer list.
         * @param init The elements, or key/value members.
         */
        Json(InitializerList<Json> init);

        /**
         * @brief Builds an empty array value.
         * @return A Json holding an empty array.
         */
        [[nodiscard]]
        static Json array() {
            return Json(Array{});
        }

        /**
         * @brief Builds an array value from a list of elements.
         * @param elements The elements.
         * @return A Json holding the given elements as an array.
         *
         * Unlike the braced-init constructor this never reinterprets its
         * argument as an object, so it is the way to spell an array of
         * string-keyed pairs.
         */
        [[nodiscard]]
        static Json array(InitializerList<Json> elements) {
            return Json(Array(elements));
        }

        /**
         * @brief Builds an empty object value.
         * @return A Json holding an empty object.
         */
        [[nodiscard]]
        static Json object() {
            return Json(Object{});
        }

        /**
         * @brief The kind of value held.
         * @return The @ref Type.
         */
        [[nodiscard]]
        Type type() const noexcept {
            switch (_value.index()) {
                case 0:
                    return Type::NULL_VALUE;
                case 1:
                    return Type::BOOLEAN;
                case 2:
                    return Type::INTEGER;
                case 3:
                    return Type::FLOAT;
                case 4:
                    return Type::STRING;
                case 5:
                    return Type::ARRAY;
                default:
                    return Type::OBJECT;
            }
            Ops::unreachable();
        }

        [[nodiscard]]
        bool is_null() const noexcept {
            return _value.index() == 0;
        }

        [[nodiscard]]
        bool is_boolean() const noexcept {
            return _value.index() == 1;
        }

        [[nodiscard]]
        bool is_integer() const noexcept {
            return _value.index() == 2;
        }

        [[nodiscard]]
        bool is_float() const noexcept {
            return _value.index() == 3;
        }

        /**
         * @brief Whether this holds a number (integer or float).
         * @return true for either numeric kind, false otherwise.
         */
        [[nodiscard]]
        bool is_number() const noexcept {
            return is_integer() || is_float();
        }

        [[nodiscard]]
        bool is_string() const noexcept {
            return _value.index() == 4;
        }

        [[nodiscard]]
        bool is_array() const noexcept {
            return _value.index() == 5;
        }

        [[nodiscard]]
        bool is_object() const noexcept {
            return _value.index() == 6;
        }

        /**
         * @brief The number of elements or members.
         * @return The array length, the object member count, 0 for null, or 1
         * for any scalar.
         */
        [[nodiscard]]
        usize size() const noexcept {
            if (is_array()) {
                return Ops::get<Array>(_value).size();
            }
            if (is_object()) {
                return Ops::get<Object>(_value).size();
            }
            if (is_null()) {
                return 0;
            }
            return 1;
        }

        /**
         * @brief Whether a container value is empty (or the value is null).
         * @return true if null, an empty array, or an empty object; false
         * otherwise (including for any scalar).
         */
        [[nodiscard]]
        bool empty() const noexcept {
            if (is_null()) {
                return true;
            }
            if (is_array()) {
                return Ops::get<Array>(_value).empty();
            }
            if (is_object()) {
                return Ops::get<Object>(_value).empty();
            }
            return false;
        }

        /**
         * @brief The value as T, if it holds exactly that kind.
         * @tparam T One of bool, an integral type, a floating-point type,
         * String, or StringView.
         * @return The value, or an empty Optional on a type mismatch.
         *
         * The safe companion to @ref get: never throws, reports a mismatch as
         * an empty Optional. A number can be read as either an integral or a
         * floating-point T regardless of which numeric kind is stored, so
         * `as<f64>()` succeeds on an integer and `as<i64>()` truncates a float;
         * every other kind must match exactly.
         */
        template <typename T>
        [[nodiscard]]
        Optional<T> as() const {
            if constexpr (SameAs<T, bool>) {
                if (is_boolean()) {
                    return Ops::get<bool>(_value);
                }
            } else if constexpr (Integral<T>) {
                if (is_integer()) {
                    return static_cast<T>(Ops::get<i64>(_value));
                }
                if (is_float()) {
                    return static_cast<T>(Ops::get<f64>(_value));
                }
            } else if constexpr (FloatingPoint<T>) {
                if (is_float()) {
                    return static_cast<T>(Ops::get<f64>(_value));
                }
                if (is_integer()) {
                    return static_cast<T>(Ops::get<i64>(_value));
                }
            } else if constexpr (IsAnyOf<T, String, StringView, const char*>) {
                if (is_string()) {
                    return T(Ops::get<String>(_value));
                }
            }
            return nullopt;
        }

        /**
         * @brief The value as T, or a supplied fallback on a type mismatch.
         * @tparam T The requested type (see @ref as for the rules).
         * @param fallback The value to return if this does not hold a T.
         * @return The stored value, or @p fallback.
         */
        template <typename T>
        [[nodiscard]]
        T as_or(T fallback) const {
            return as<T>().value_or(Ops::move(fallback));
        }

        /**
         * @brief The value as T, or a thrown exception on a type mismatch.
         * @tparam T The requested type (see @ref as for the conversion rules).
         * @return The stored value.
         * @throws JsonTypeException if this does not hold a compatible kind.
         */
        template <typename T>
        THROWS(JsonTypeException)
        [[nodiscard]]
        T get() const {
            Optional<T> v = as<T>();
            if (!v.has_value()) {
                throw JsonTypeException(Ops::fmt("JSON value is not the requested type (is {})", type_name()));
            }
            return *v;
        }

        /**
         * @brief Implicitly converts to any type @ref get accepts.
         * @tparam T The destination type.
         * @return The stored value.
         * @throws JsonTypeException on a type mismatch.
         */
        template <NotSameAs<Json> T>
            requires (Integral<T> || FloatingPoint<T> || IsAnyOf<T, bool, String, StringView>)
        THROWS(JsonTypeException)
        operator T() const {
            return get<T>();
        }

        /**
         * @brief The underlying array, for element-wise work and range-for.
         * @return A reference to the array.
         * @throws JsonTypeException if this does not hold an array.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Array& as_array() {
            if (!is_array()) {
                throw JsonTypeException(Ops::fmt("JSON value is not an array (is {})", type_name()));
            }
            return Ops::get<Array>(_value);
        }

        [[nodiscard]]
        THROWS(JsonTypeException)
        const Array& as_array() const {
            if (!is_array()) {
                throw JsonTypeException(Ops::fmt("JSON value is not an array (is {})", type_name()));
            }
            return Ops::get<Array>(_value);
        }

        /**
         * @brief The underlying object, for member-wise work and range-for.
         * @return A reference to the object.
         * @throws JsonTypeException if this does not hold an object.
         *
         * Iterate with @ref JsonObject::keys and @ref JsonObject::values, which
         * run parallel in insertion order.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Object& as_object() {
            if (!is_object()) {
                throw JsonTypeException(Ops::fmt("JSON value is not an object (is {})", type_name()));
            }
            return Ops::get<Object>(_value);
        }

        [[nodiscard]]
        THROWS(JsonTypeException)
        const Object& as_object() const {
            if (!is_object()) {
                throw JsonTypeException(Ops::fmt("JSON value is not an object (is {})", type_name()));
            }
            return Ops::get<Object>(_value);
        }

        /**
         * @brief Accesses a member by key, creating it if absent.
         * @param key The member key.
         * @return A reference to the member value.
         * @throws JsonTypeException if this holds a non-object, non-null value.
         *
         * Auto-vivifying: a null value becomes an object on first subscript, and
         * a missing key is inserted as null, so `j["a"]["b"] = 1;` builds the
         * path. Use @ref at for a checked, non-creating lookup, or @ref contains
         * to probe.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Json& operator[](StringView key) {
            if (is_null()) {
                _value = Object{};
            }
            if (!is_object()) {
                throw JsonTypeException(Ops::fmt("cannot index a {} with a string key", type_name()));
            }
            return Ops::get<Object>(_value)[key];
        }

        /**
         * @brief Accesses a member by key (string-literal overload).
         * @param key The member key.
         * @return A reference to the member value.
         * @throws JsonTypeException if this holds a non-object, non-null value.
         *
         * Present so a bare string literal subscript picks the string-key
         * accessor outright, rather than being ambiguous between the StringView
         * overload (a user conversion) and the usize overload (via array-to-
         * pointer decay). Delegates to the StringView form.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Json& operator[](const char* key) {
            return (*this)[StringView(key)];
        }

        /**
         * @brief Accesses a member by key, without creating it.
         * @param key The member key.
         * @return A reference to the member value.
         * @throws JsonTypeException if this is not an object, or has no such key.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Json& at(StringView key) {
            Object& obj = as_object();
            Json* member = obj.find(key);
            if (member == nullptr) {
                throw JsonTypeException(Ops::fmt("no such key: {}", key));
            }
            return *member;
        }

        [[nodiscard]]
        THROWS(JsonTypeException)
        const Json& at(StringView key) const {
            const Object& obj = as_object();
            const Json* member = obj.find(key);
            if (member == nullptr) {
                throw JsonTypeException(Ops::fmt("no such key: {}", key));
            }
            return *member;
        }

        /**
         * @brief Whether an object member with the given key exists.
         * @param key The member key.
         * @return true if this is an object holding @p key, false otherwise
         * (including for any non-object value).
         */
        [[nodiscard]]
        bool contains(StringView key) const noexcept {
            return is_object() && Ops::get<Object>(_value).contains(key);
        }

        /**
         * @brief An object member converted to T, or a fallback.
         * @tparam T The requested type (see @ref as).
         * @param key The member key.
         * @param fallback The value returned when the key is absent or the
         * member is not a T.
         * @return The converted member, or @p fallback.
         */
        template <typename T>
        [[nodiscard]]
        T value(StringView key, T fallback) const {
            if (!is_object()) {
                return fallback;
            }
            const Object& obj = Ops::get<Object>(_value);
            const Json* member = obj.find(key);
            if (member == nullptr) {
                return fallback;
            }
            return member->template as<T>().value_or(Ops::move(fallback));
        }

        /**
         * @brief Sets an object member, creating the object if this is null.
         * @param key The member key.
         * @param v The value to store.
         * @throws JsonTypeException if this holds a non-object, non-null value.
         */
        THROWS(JsonTypeException)
        void set(StringView key, Json v) {
            if (is_null()) {
                _value = Object{};
            }
            if (!is_object()) {
                throw JsonTypeException(Ops::fmt("cannot set a key on a {}", type_name()));
            }
            Ops::get<Object>(_value).set(key, Ops::move(v));
        }

        /**
         * @brief Removes an object member.
         * @param key The member key.
         * @return true if a member was removed, false otherwise.
         */
        bool erase(StringView key) {
            if (!is_object()) {
                return false;
            }
            return Ops::get<Object>(_value).erase(key);
        }

        /**
         * @brief Accesses an array element by index.
         * @param index The 0-based index.
         * @return A reference to the element.
         * @throws JsonTypeException if this is not an array, or @p index is out
         * of range.
         */
        [[nodiscard]]
        THROWS(JsonTypeException)
        Json& operator[](usize index) {
            Array& arr = as_array();
            if (index >= arr.size()) {
                throw JsonTypeException(Ops::fmt("array index {} out of range (size {})", index, arr.size()));
            }
            return arr[index];
        }

        [[nodiscard]]
        THROWS(JsonTypeException)
        const Json& operator[](usize index) const {
            const Array& arr = as_array();
            if (index >= arr.size()) {
                throw JsonTypeException(Ops::fmt("array index {} out of range (size {})", index, arr.size()));
            }
            return arr[index];
        }

        /**
         * @brief Appends an element, creating the array if this is null.
         * @param v The element to append.
         * @throws JsonTypeException if this holds a non-array, non-null value.
         */
        THROWS(JsonTypeException)
        void push_back(Json v) {
            if (is_null()) {
                _value = Array{};
            }
            if (!is_array()) {
                throw JsonTypeException(Ops::fmt("cannot push_back onto a {}", type_name()));
            }
            Ops::get<Array>(_value).push_back(Ops::move(v));
        }

        /**
         * @brief Appends an element (alias for @ref push_back).
         * @param v The element to append.
         * @return A reference to this value, for chaining.
         * @throws JsonTypeException if this holds a non-array, non-null value.
         */
        THROWS(JsonTypeException)
        Json& operator+=(Json v) {
            push_back(Ops::move(v));
            return *this;
        }

        /**
         * @brief The name of the held kind, for diagnostics.
         * @return A static string such as "null", "boolean", or "object".
         */
        [[nodiscard]]
        StringView type_name() const noexcept {
            switch (type()) {
                case Type::NULL_VALUE:
                    return "null";
                case Type::BOOLEAN:
                    return "boolean";
                case Type::INTEGER:
                    return "integer";
                case Type::FLOAT:
                    return "float";
                case Type::STRING:
                    return "string";
                case Type::ARRAY:
                    return "array";
                case Type::OBJECT:
                    return "object";
            }
            Ops::unreachable();
        }

        /**
         * @brief Parses JSON text into a value.
         * @param text The document.
         * @return The parsed value.
         * @throws JsonParseException if @p text is not well-formed JSON.
         *
         * The throwing entry point, for when malformed input is a genuine error.
         * Use @ref try_parse where it is an expected outcome.
         */
        [[nodiscard]]
        THROWS(JsonParseException)
        static Json parse(StringView text);

        /**
         * @brief Parses JSON text, reporting failure as an empty Optional.
         * @param text The document.
         * @return The parsed value, or an empty Optional if @p text is malformed.
         */
        [[nodiscard]]
        static Optional<Json> try_parse(StringView text) noexcept;

        /**
         * @brief Serializes to compact JSON text (no insignificant whitespace).
         * @return The serialized document.
         */
        [[nodiscard]]
        String to_string() const;

        /**
         * @brief Serializes to indented, human-readable JSON text.
         * @param indent The number of spaces per nesting level.
         * @return The serialized document.
         */
        [[nodiscard]]
        String dump(usize indent = 2) const;

        /**
         * @brief Resolves a JSON Pointer against this value (non-throwing).
         * @param pointer The parsed pointer (RFC 6901).
         * @return A pointer to the addressed value, or nullptr if it does not
         * resolve.
         *
         * A pure lookup: it never creates missing members, and a token that
         * names an absent key, a non-numeric array index, or an out-of-range
         * index yields nullptr rather than throwing. The empty pointer resolves
         * to this value itself.
         */
        [[nodiscard]]
        const Json* find(const JsonPointer& pointer) const noexcept;

        [[nodiscard]]
        Json* find(const JsonPointer& pointer) noexcept;

        /**
         * @brief Resolves a JSON Pointer given as text (non-throwing).
         * @param pointer The pointer text, e.g. "/foo/0/bar".
         * @return A pointer to the addressed value, or nullptr if the text is a
         * malformed pointer or does not resolve.
         *
         * Parses @p pointer inline; a syntax error is reported the same way as a
         * failed lookup, as nullptr. Use the @ref JsonPointer overload to parse
         * once and resolve many times.
         */
        [[nodiscard]]
        const Json* find(StringView pointer) const noexcept;

        [[nodiscard]]
        Json* find(StringView pointer) noexcept;

        /**
         * @brief Resolves a JSON Pointer against this value, or throws.
         * @param pointer The parsed pointer (RFC 6901).
         * @return A reference to the addressed value.
         * @throws JsonPointerException if the pointer does not resolve.
         */
        [[nodiscard]]
        THROWS(JsonPointerException)
        const Json& at(const JsonPointer& pointer) const;

        [[nodiscard]]
        THROWS(JsonPointerException)
        Json& at(const JsonPointer& pointer);

        /**
         * @brief Whether a JSON Pointer resolves against this value.
         * @param pointer The parsed pointer (RFC 6901).
         * @return true if the pointer addresses an existing value, false otherwise.
         */
        [[nodiscard]]
        bool contains(const JsonPointer& pointer) const noexcept;

        #ifdef __cpp_lib_reflection
        /**
         * @brief Builds a Json from an arbitrary value by reflection.
         * @tparam T The source type.
         * @param value The value to convert.
         * @return The equivalent JSON value.
         */
        template <typename T>
        [[nodiscard]]
        static Json serialize(const T& value);

        /**
         * @brief Reconstructs a value of type T from this Json by reflection.
         * @tparam T The target type.
         * @return The reconstructed value.
         * @throws JsonTypeException if this value does not match T's shape.
         */
        template <typename T>
        [[nodiscard]]
        [[=Throws<JsonTypeException>]]
        T deserialize() const;
        #endif

        /**
         * @brief Direct access to the underlying variant, for visitation.
         * @return A reference to the held alternative set.
         *
         * The alternatives, by index, are Monostate, bool, i64, f64, String,
         * Array, Object - matching @ref Type.
         */
        [[nodiscard]]
        const Variant<Monostate, bool, i64, f64, String, Array, Object>& raw() const noexcept {
            return _value;
        }

        [[nodiscard]]
        bool operator==(const Json& other) const = default;
    };

    using JsonType = Json::Type;

    inline void JsonObject::clear() noexcept {
        _keys.clear();
        _values.clear();
    }

    inline const Json* JsonObject::find(StringView key) const noexcept {
        Optional<usize> i = index_of(key);
        return i.has_value() ? &_values[*i] : nullptr;
    }

    inline Json* JsonObject::find(StringView key) noexcept {
        Optional<usize> i = index_of(key);
        return i.has_value() ? &_values[*i] : nullptr;
    }

    inline void JsonObject::set(StringView key, Json value) {
        if (Json* existing = find(key)) {
            *existing = Ops::move(value);
            return;
        }
        _keys.emplace_back(key);
        _values.push_back(Ops::move(value));
    }

    inline Json& JsonObject::operator[](StringView key) {
        if (Json* existing = find(key)) {
            return *existing;
        }
        _keys.emplace_back(key);
        _values.emplace_back();
        return _values.back();
    }

    inline bool JsonObject::erase(StringView key) {
        Optional<usize> i = index_of(key);
        if (!i.has_value()) {
            return false;
        }
        _keys.erase(_keys.begin() + static_cast<Vector<String>::difference_type>(*i));
        _values.erase(_values.begin() + static_cast<Vector<Json>::difference_type>(*i));
        return true;
    }

    inline bool JsonObject::operator==(const JsonObject& other) const {
        return _keys == other._keys && _values == other._values;
    }

    inline Json::Json(InitializerList<Json> init) {
        // Object iff every element is a two-element array whose first element
        // is a string. An empty list is an (empty) object.
        bool looks_like_object = true;
        for (const Json& element: init) {
            if (!element.is_array() || element.size() != 2) {
                looks_like_object = false;
                break;
            }
            if (!element[0uz].is_string()) {
                looks_like_object = false;
                break;
            }
        }

        if (looks_like_object) {
            Object obj;
            for (const Json& element: init) {
                const Array& pair = Ops::get<Array>(element._value);
                obj.set(Ops::get<String>(pair[0]._value), pair[1]);
            }
            _value = Ops::move(obj);
        } else {
            _value = Array(init);
        }
    }
}
