#pragma once

using stdx::time::Minutes;
using stdx::time::Nanoseconds;
using stdx::time::TimeOfDay;
using stdx::time::YearMonthDay;

/**
 * @namespace stdx::data::toml
 * @brief TOML parsing, construction, and serialization (TOML v1.0.0).
 */
export namespace stdx::data::toml {
    class Toml;

    /**
     * @typedef TomlLocalDate
     * @brief A TOML local date (`1979-05-27`): a calendar date with no time or offset.
     */
    using TomlLocalDate = YearMonthDay;

    /**
     * @typedef TomlLocalTime
     * @brief A TOML local time (`07:32:00`): a time of day with no date or offset.
     *
     * Nanosecond resolution, which covers TOML's "arbitrary" fractional seconds
     * to the precision the library's clocks carry.
     */
    using TomlLocalTime = TimeOfDay<Nanoseconds>;

    /**
     * @struct TomlLocalDateTime
     * @brief A TOML local date-time (`1979-05-27T07:32:00`): a date and time with no offset.
     *
     * Composed from the library's existing date and time-of-day types rather
     * than a single chrono time point, because TOML's local date-time is
     * deliberately zone-less - it denotes a wall-clock reading, not an instant,
     * so there is no clock to anchor it to.
     */
    struct TomlLocalDateTime {
        TomlLocalDate date; ///< The calendar date.
        TomlLocalTime time; ///< The time of day.

        [[nodiscard]]
        bool operator==(const TomlLocalDateTime& other) const noexcept {
            return date == other.date && time.to_duration() == other.time.to_duration();
        }
    };

    /**
     * @struct TomlOffsetDateTime
     * @brief A TOML offset date-time (`1979-05-27T07:32:00-08:00`): an instant with a UTC offset.
     *
     * A local date-time paired with its offset from UTC, in minutes (TOML offsets
     * have minute granularity). Storing the offset alongside the wall-clock
     * reading, rather than normalizing to UTC, preserves the exact text a
     * document carried so it round-trips without drift.
     */
    struct TomlOffsetDateTime {
        TomlLocalDateTime datetime; ///< The local date-time.
        Minutes offset; ///< The offset from UTC (may be negative; zero for 'Z').

        [[nodiscard]]
        bool operator==(const TomlOffsetDateTime& other) const noexcept {
            return datetime == other.datetime && offset == other.offset;
        }
    };

    /**
     * @class TomlTable
     * @brief An insertion-ordered collection of string-keyed @ref Toml members.
     *
     * TOML tables have document order, not sort order, so this keeps members in
     * insertion order and preserves it across a parse -> serialize round-trip.
     * The parallel-vector design (keys and values in separate vectors) is the
     * same one @ref stdx::data::json::JsonObject uses, and for the same reason:
     * `Toml` is incomplete where this type is declared, and a `Vector<Toml>`
     * tolerates that whereas a `std::pair<String, Toml>` would not. Member i is
     * (`keys()[i]`, `values()[i]`). A repeated key overwrites in place.
     */
    class TomlTable {
    private:
        Vector<String> _keys; ///< Member keys, in insertion order; kept unique by @ref set.
        Vector<Toml> _values; ///< Member values, parallel to @ref _keys.
    public:
        TomlTable() = default;

        [[nodiscard]]
        usize size() const noexcept {
            return _keys.size();
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return _keys.empty();
        }

        void clear() noexcept;

        [[nodiscard]]
        const Vector<String>& keys() const noexcept {
            return _keys;
        }

        [[nodiscard]]
        const Vector<Toml>& values() const noexcept {
            return _values;
        }

        [[nodiscard]]
        Vector<Toml>& values() noexcept {
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

        [[nodiscard]]
        bool contains(StringView key) const noexcept {
            return index_of(key).has_value();
        }

        [[nodiscard]]
        const Toml* find(StringView key) const noexcept;

        [[nodiscard]]
        Toml* find(StringView key) noexcept;

        /**
         * @brief Inserts a member, or overwrites the existing one with this key.
         * @param key The member key.
         * @param value The value to store.
         */
        void set(StringView key, Toml value);

        /**
         * @brief Accesses the value for a key, inserting an empty table if absent.
         * @param key The member key.
         * @return A reference to the value.
         */
        [[nodiscard]]
        Toml& operator[](StringView key);

        /**
         * @brief Removes the member with the given key, if present.
         * @param key The key to remove.
         * @return true if a member was removed, false otherwise.
         */
        bool erase(StringView key);

        [[nodiscard]]
        bool operator==(const TomlTable& other) const;
    };

    /**
     * @class Toml
     * @brief A single TOML value: boolean, integer, float, string, one of three
     * date-time kinds, array, or table.
     *
     * One value type models every TOML kind, queried via @ref type / @ref is_table
     * rather than carried in the static type - the same single-value-type design
     * as the JSON module. TOML has no null, so a default-constructed Toml is an
     * empty table (the natural document root), not a null. Integers are 64-bit
     * signed per the TOML spec.
     */
    class [[nodiscard]] Toml {
    public:
        using Array = Vector<Toml>; ///< The element container for a TOML array.
        using Table = TomlTable; ///< The member container for a TOML table.
        using LocalDate = TomlLocalDate; ///< A TOML local date.
        using LocalTime = TomlLocalTime; ///< A TOML local time.
        using LocalDateTime = TomlLocalDateTime; ///< A TOML local date-time.
        using OffsetDateTime = TomlOffsetDateTime; ///< A TOML offset date-time.

        /**
         * @enum Type
         * @brief The kind of value a @ref Toml currently holds.
         */
        enum class Type: u8 {
            BOOLEAN, ///< true or false.
            INTEGER, ///< A 64-bit signed integer.
            FLOAT, ///< An IEEE-754 double.
            STRING, ///< A string.
            LOCAL_DATE, ///< A date with no time or offset.
            LOCAL_TIME, ///< A time of day with no date or offset.
            LOCAL_DATE_TIME, ///< A date and time with no offset.
            OFFSET_DATE_TIME, ///< A date and time with a UTC offset.
            ARRAY, ///< An ordered list of values.
            TABLE, ///< An ordered collection of key/value members.
        };
    private:
        Variant<
            bool, i64, f64, String,
            LocalDate, LocalTime, LocalDateTime, OffsetDateTime,
            Array, Table
        > _value; ///< The held value; the index selects the kind (see @ref Type).
    public:
        /**
         * @brief Constructs an empty table (the default TOML value).
         */
        Toml() noexcept:
            _value{Table{}} {}

        /**
         * @brief Constructs a boolean value.
         */
        Toml(bool b) noexcept:
            _value{b} {}

        /**
         * @brief Constructs an integer value.
         *
         * Every signed and unsigned integral (other than bool) routes here and
         * is stored as i64, matching TOML's 64-bit signed integer type.
         */
        template <Integral T>
            requires (NotSameAs<T, bool>)
        Toml(T n) noexcept:
            _value{static_cast<i64>(n)} {}

        /**
         * @brief Constructs a floating-point value.
         */
        template <FloatingPoint F>
        Toml(F x) noexcept:
            _value{static_cast<f64>(x)} {}

        Toml(String s) noexcept:
            _value{Ops::move(s)} {}

        Toml(StringView s):
            _value{String(s)} {}

        /**
         * @brief Constructs a string value from a literal.
         *
         * Present so a bare string literal becomes a TOML string rather than
         * selecting the bool constructor through pointer-to-bool conversion.
         */
        Toml(const char* s):
            _value{String(s)} {}

        Toml(LocalDate d) noexcept:
            _value{d} {}

        Toml(LocalTime t) noexcept:
            _value{t} {}

        Toml(LocalDateTime dt) noexcept:
            _value{dt} {}

        Toml(OffsetDateTime dt) noexcept:
            _value{dt} {}

        Toml(Array elements) noexcept:
            _value{Ops::move(elements)} {}

        Toml(Table members) noexcept:
            _value{Ops::move(members)} {}

        /**
         * @brief Builds an empty array value.
         * @return A Toml holding an empty array.
         */
        [[nodiscard]]
        static Toml array() {
            return Toml(Array{});
        }

        /**
         * @brief Builds an array value from a list of elements.
         */
        [[nodiscard]]
        static Toml array(InitializerList<Toml> elements) {
            return Toml(Array(elements));
        }

        /**
         * @brief Builds an empty table value.
         * @return A Toml holding an empty table.
         */
        [[nodiscard]]
        static Toml table() {
            return Toml(Table{});
        }

        /**
         * @brief The kind of value held.
         * @return The @ref Type.
         */
        [[nodiscard]]
        Type type() const noexcept {
            switch (_value.index()) {
                case 0: return Type::BOOLEAN;
                case 1: return Type::INTEGER;
                case 2: return Type::FLOAT;
                case 3: return Type::STRING;
                case 4: return Type::LOCAL_DATE;
                case 5: return Type::LOCAL_TIME;
                case 6: return Type::LOCAL_DATE_TIME;
                case 7: return Type::OFFSET_DATE_TIME;
                case 8: return Type::ARRAY;
                default: return Type::TABLE;
            }
        }

        [[nodiscard]]
        bool is_boolean() const noexcept {
            return _value.index() == 0;
        }

        [[nodiscard]]
        bool is_integer() const noexcept {
            return _value.index() == 1;
        }

        [[nodiscard]]
        bool is_float() const noexcept {
            return _value.index() == 2;
        }

        [[nodiscard]]
        bool is_number() const noexcept {
            return is_integer() || is_float();
        }

        [[nodiscard]]
        bool is_string() const noexcept {
            return _value.index() == 3;
        }

        [[nodiscard]]
        bool is_local_date() const noexcept {
            return _value.index() == 4;
        }

        [[nodiscard]]
        bool is_local_time() const noexcept {
            return _value.index() == 5;
        }

        [[nodiscard]]
        bool is_local_date_time() const noexcept {
            return _value.index() == 6;
        }

        [[nodiscard]]
        bool is_offset_date_time() const noexcept {
            return _value.index() == 7;
        }

        /**
         * @brief Whether this holds any of the four date-time kinds.
         */
        [[nodiscard]]
        bool is_datetime() const noexcept {
            const usize i = _value.index();
            return i >= 4 && i <= 7;
        }

        [[nodiscard]]
        bool is_array() const noexcept {
            return _value.index() == 8;
        }

        [[nodiscard]]
        bool is_table() const noexcept {
            return _value.index() == 9;
        }

        /**
         * @brief The value as T, if it holds exactly that kind.
         * @tparam T bool, an integral/floating type, String/StringView, one of the
         * date-time kinds, Array, or Table.
         * @return The value, or an empty Optional on a type mismatch.
         *
         * Numbers cross-convert between integral and floating requests (like the
         * JSON value); every other kind must match exactly.
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
            } else if constexpr (IsAnyOf<T, String, StringView>) {
                if (is_string()) {
                    return T(Ops::get<String>(_value));
                }
            } else if constexpr (SameAs<T, LocalDate>) {
                if (is_local_date()) {
                    return Ops::get<LocalDate>(_value);
                }
            } else if constexpr (SameAs<T, LocalTime>) {
                if (is_local_time()) {
                    return Ops::get<LocalTime>(_value);
                }
            } else if constexpr (SameAs<T, LocalDateTime>) {
                if (is_local_date_time()) {
                    return Ops::get<LocalDateTime>(_value);
                }
            } else if constexpr (SameAs<T, OffsetDateTime>) {
                if (is_offset_date_time()) {
                    return Ops::get<OffsetDateTime>(_value);
                }
            }
            return nullopt;
        }

        /**
         * @brief The value as T, or a supplied fallback on a type mismatch.
         */
        template <typename T>
        [[nodiscard]]
        T as_or(T fallback) const {
            return as<T>().value_or(Ops::move(fallback));
        }

        /**
         * @brief The value as T, or a thrown exception on a type mismatch.
         * @throws TomlTypeException if this does not hold a compatible kind.
         */
        template <typename T>
        THROWS(TomlTypeException)
        [[nodiscard]]
        T get() const {
            Optional<T> v = as<T>();
            if (!v.has_value()) {
                throw TomlTypeException(Ops::fmt("TOML value is not the requested type (is {})", type_name()));
            }
            return *v;
        }

        /**
         * @brief The underlying array.
         * @throws TomlTypeException if this does not hold an array.
         */
        [[nodiscard]]
        THROWS(TomlTypeException)
        Array& as_array() {
            if (!is_array()) {
                throw TomlTypeException(Ops::fmt("TOML value is not an array (is {})", type_name()));
            }
            return Ops::get<Array>(_value);
        }

        [[nodiscard]]
        THROWS(TomlTypeException)
        const Array& as_array() const {
            if (!is_array()) {
                throw TomlTypeException(Ops::fmt("TOML value is not an array (is {})", type_name()));
            }
            return Ops::get<Array>(_value);
        }

        /**
         * @brief The underlying table.
         * @throws TomlTypeException if this does not hold a table.
         */
        [[nodiscard]]
        THROWS(TomlTypeException)
        Table& as_table() {
            if (!is_table()) {
                throw TomlTypeException(Ops::fmt("TOML value is not a table (is {})", type_name()));
            }
            return Ops::get<Table>(_value);
        }

        [[nodiscard]]
        THROWS(TomlTypeException)
        const Table& as_table() const {
            if (!is_table()) {
                throw TomlTypeException(Ops::fmt("TOML value is not a table (is {})", type_name()));
            }
            return Ops::get<Table>(_value);
        }

        /**
         * @brief Accesses a table member by key, creating it if absent.
         * @param key The member key.
         * @return A reference to the member value.
         * @throws TomlTypeException if this holds a non-table value.
         *
         * A missing key is inserted as an empty table, so `t["a"]["b"] = 1;`
         * builds the path. Use @ref at for a checked, non-creating lookup.
         */
        [[nodiscard]]
        THROWS(TomlTypeException)
        Toml& operator[](StringView key) {
            if (!is_table()) {
                throw TomlTypeException(Ops::fmt("cannot index a {} with a string key", type_name()));
            }
            return Ops::get<Table>(_value)[key];
        }

        [[nodiscard]]
        THROWS(TomlTypeException)
        Toml& operator[](const char* key) {
            return (*this)[StringView(key)];
        }

        /**
         * @brief Accesses a table member by key, without creating it.
         * @throws TomlTypeException if this is not a table, or has no such key.
         */
        [[nodiscard]]
        THROWS(TomlTypeException)
        Toml& at(StringView key) {
            Table& t = as_table();
            Toml* member = t.find(key);
            if (member == nullptr) {
                throw TomlTypeException(Ops::fmt("no such key: {}", key));
            }
            return *member;
        }

        [[nodiscard]]
        THROWS(TomlTypeException)
        const Toml& at(StringView key) const {
            const Table& t = as_table();
            const Toml* member = t.find(key);
            if (member == nullptr) {
                throw TomlTypeException(Ops::fmt("no such key: {}", key));
            }
            return *member;
        }

        /**
         * @brief Whether a table member with the given key exists.
         */
        [[nodiscard]]
        bool contains(StringView key) const noexcept {
            return is_table() && Ops::get<Table>(_value).contains(key);
        }

        /**
         * @brief Sets a table member, requiring this to be a table.
         * @throws TomlTypeException if this holds a non-table value.
         */
        THROWS(TomlTypeException)
        void set(StringView key, Toml v) {
            if (!is_table()) {
                throw TomlTypeException(Ops::fmt("cannot set a key on a {}", type_name()));
            }
            Ops::get<Table>(_value).set(key, Ops::move(v));
        }

        /**
         * @brief Accesses an array element by index.
         * @throws TomlTypeException if this is not an array, or @p index is out of range.
         */
        [[nodiscard]]
        THROWS(TomlTypeException)
        Toml& operator[](usize index) {
            Array& arr = as_array();
            if (index >= arr.size()) {
                throw TomlTypeException(Ops::fmt("array index {} out of range (size {})", index, arr.size()));
            }
            return arr[index];
        }

        [[nodiscard]]
        THROWS(TomlTypeException)
        const Toml& operator[](usize index) const {
            const Array& arr = as_array();
            if (index >= arr.size()) {
                throw TomlTypeException(Ops::fmt("array index {} out of range (size {})", index, arr.size()));
            }
            return arr[index];
        }

        /**
         * @brief Appends an element, creating the array if this is an empty table.
         * @throws TomlTypeException if this holds a non-array, non-empty-table value.
         *
         * The empty-table default stands in for "uninitialized" (TOML has no
         * null), so an appended element turns a fresh Toml into an array.
         */
        THROWS(TomlTypeException)
        void push_back(Toml v) {
            if (is_table() && Ops::get<Table>(_value).empty()) {
                _value = Array{};
            }
            if (!is_array()) {
                throw TomlTypeException(Ops::fmt("cannot push_back onto a {}", type_name()));
            }
            Ops::get<Array>(_value).push_back(Ops::move(v));
        }

        /**
         * @brief The number of elements or members.
         * @return The array length, the table member count, or 1 for a scalar.
         */
        [[nodiscard]]
        usize size() const noexcept {
            if (is_array()) {
                return Ops::get<Array>(_value).size();
            }
            if (is_table()) {
                return Ops::get<Table>(_value).size();
            }
            return 1;
        }

        /**
         * @brief The name of the held kind, for diagnostics.
         */
        [[nodiscard]]
        StringView type_name() const noexcept {
            switch (type()) {
                case Type::BOOLEAN: return "boolean";
                case Type::INTEGER: return "integer";
                case Type::FLOAT: return "float";
                case Type::STRING: return "string";
                case Type::LOCAL_DATE: return "local-date";
                case Type::LOCAL_TIME: return "local-time";
                case Type::LOCAL_DATE_TIME: return "local-date-time";
                case Type::OFFSET_DATE_TIME: return "offset-date-time";
                case Type::ARRAY: return "array";
                default: return "table";
            }
        }

        /**
         * @brief Parses TOML text into a value (always a table at the top level).
         * @throws TomlParseException if @p text is not well-formed TOML.
         */
        [[nodiscard]]
        THROWS(TomlParseException)
        static Toml parse(StringView text);

        /**
         * @brief Parses TOML text, reporting failure as an empty Optional.
         */
        [[nodiscard]]
        static Optional<Toml> try_parse(StringView text) noexcept;

        /**
         * @brief Serializes to TOML text.
         */
        [[nodiscard]]
        String to_string() const;

        #ifdef __cpp_lib_reflection
        /**
         * @brief Builds a Toml from an arbitrary value by reflection.
         *
         * Reads public data members only; a class with private or protected
         * ones must be marked {@code [[=Reflectable]]}, or serialized with the
         * AllowPrivate overload.
         */
        template <typename T>
        [[nodiscard]]
        static Toml serialize(const T& value);

        /**
         * @brief Like serialize(const T&), but also reads private and
         * protected data members, of T and every type nested in it.
         * @see stdx::data::AllowPrivate
         */
        template <typename T>
        [[nodiscard]]
        static Toml serialize(const T& value, AllowPrivateTag);

        /**
         * @brief Reconstructs a value of type T from this Toml by reflection.
         * @throws TomlTypeException if this value does not match T's shape.
         *
         * Writes public data members only; see serialize(const T&).
         */
        template <typename T>
        [[nodiscard]]
        THROWS(TomlTypeException)
        T deserialize() const;

        /**
         * @brief Like deserialize(), but also writes private and protected
         * data members, of T and every type nested in it.
         * @see stdx::data::AllowPrivate
         */
        template <typename T>
        [[nodiscard]]
        THROWS(TomlTypeException)
        T deserialize(AllowPrivateTag) const;
        #endif

        /**
         * @brief Value equality.
         *
         * Hand-written rather than defaulted because one alternative,
         * @ref TomlLocalTime (a chrono hh_mm_ss), has no `operator==`; a defaulted
         * comparison would be ill-formed. Two values are equal only if they hold
         * the same kind and compare equal within it; the time kinds compare by
         * their underlying duration.
         */
        [[nodiscard]]
        bool operator==(const Toml& other) const {
            if (type() != other.type()) {
                return false;
            }
            switch (type()) {
                case Type::BOOLEAN:
                    return Ops::get<bool>(_value) == Ops::get<bool>(other._value);
                case Type::INTEGER:
                    return Ops::get<i64>(_value) == Ops::get<i64>(other._value);
                case Type::FLOAT:
                    return Ops::get<f64>(_value) == Ops::get<f64>(other._value);
                case Type::STRING:
                    return Ops::get<String>(_value) == Ops::get<String>(other._value);
                case Type::LOCAL_DATE:
                    return Ops::get<LocalDate>(_value) == Ops::get<LocalDate>(other._value);
                case Type::LOCAL_TIME:
                    return Ops::get<LocalTime>(_value).to_duration()
                        == Ops::get<LocalTime>(other._value).to_duration();
                case Type::LOCAL_DATE_TIME:
                    return Ops::get<LocalDateTime>(_value) == Ops::get<LocalDateTime>(other._value);
                case Type::OFFSET_DATE_TIME:
                    return Ops::get<OffsetDateTime>(_value) == Ops::get<OffsetDateTime>(other._value);
                case Type::ARRAY:
                    return Ops::get<Array>(_value) == Ops::get<Array>(other._value);
                case Type::TABLE:
                    return Ops::get<Table>(_value) == Ops::get<Table>(other._value);
            }
            return false;
        }
    };

    using TomlType = Toml::Type;

    inline void TomlTable::clear() noexcept {
        _keys.clear();
        _values.clear();
    }

    inline const Toml* TomlTable::find(StringView key) const noexcept {
        Optional<usize> i = index_of(key);
        return i.has_value() ? &_values[*i] : nullptr;
    }

    inline Toml* TomlTable::find(StringView key) noexcept {
        Optional<usize> i = index_of(key);
        return i.has_value() ? &_values[*i] : nullptr;
    }

    inline void TomlTable::set(StringView key, Toml value) {
        if (Toml* existing = find(key)) {
            *existing = Ops::move(value);
            return;
        }
        _keys.emplace_back(key);
        _values.push_back(Ops::move(value));
    }

    inline Toml& TomlTable::operator[](StringView key) {
        if (Toml* existing = find(key)) {
            return *existing;
        }
        _keys.emplace_back(key);
        _values.emplace_back();
        return _values.back();
    }

    inline bool TomlTable::erase(StringView key) {
        Optional<usize> i = index_of(key);
        if (!i.has_value()) {
            return false;
        }
        _keys.erase(_keys.begin() + static_cast<Vector<String>::difference_type>(*i));
        _values.erase(_values.begin() + static_cast<Vector<Toml>::difference_type>(*i));
        return true;
    }

    inline bool TomlTable::operator==(const TomlTable& other) const {
        return _keys == other._keys && _values == other._values;
    }
}
