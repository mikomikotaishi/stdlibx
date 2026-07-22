#pragma once

using stdx::mem::SharedPointer;
using stdx::meta::TypeIndex;

#ifdef __cpp_impl_reflection

namespace stdx::inject {
    template <typename T>
    constexpr bool IsSharedPointerValue = false;

    template <typename U>
    constexpr bool IsSharedPointerValue<SharedPointer<U>> = true;

    template <typename T>
    struct SharedPointerElement {};

    template <typename U>
    struct SharedPointerElement<SharedPointer<U>> {
        using Element = U;
    };

    /**
     * @concept Annotatable
     * @brief A type usable as a binding annotation: hashable via {@code std::hash}.
     */
    export template <typename T>
    concept Annotatable = requires (T t) { Hash<T>()(t); };

    /**
     * @class AnnotationKey
     * @brief Identifies a binding annotation, either by exact value or by type only.
     *
     * An EXACT key matches a specific annotation value (e.g. {@code Named("db")});
     * a TYPE_ONLY key matches any annotation of that type and is used as a
     * fallback so a single provider can serve all values of an annotation type.
     */
    export class AnnotationKey {
    public:
        enum class Kind: u8 {
            EXACT, ///< Matches one specific annotation value.
            TYPE_ONLY, ///< Matches any annotation of the type.
        };
    private:
        TypeIndex _type; ///< The annotation's C++ type.
        Kind _kind; ///< Whether this key matches by value or by type.
        Any _value; ///< The annotation value (EXACT only).
        usize _hash = 0; ///< Hash of the annotation value (EXACT only).
    public:
        /**
         * @brief Constructs an EXACT key for a specific annotation value.
         * @param value The annotation value.
         */
        template <Annotatable T>
        explicit AnnotationKey(T value):
            _type{typeid(T)}, _kind{Kind::EXACT},
            _value{value}, _hash{Hash<T>()(value)} {}

        /**
         * @brief Constructs a TYPE_ONLY key for an annotation type.
         * @param type The annotation type's index.
         */
        explicit AnnotationKey(TypeIndex type):
            _type{type}, _kind{Kind::TYPE_ONLY} {}

        [[nodiscard]]
        bool operator==(const AnnotationKey& other) const {
            if (_type != other._type || _kind != other._kind) {
                return false;
            }
            if (_kind == Kind::TYPE_ONLY) {
                return true;
            }
            return _hash == other._hash;
        }

        [[nodiscard]]
        usize hash() const {
            usize hash = Hash<TypeIndex>()(_type);
            hash ^= Hash<i32>()(static_cast<i32>(_kind)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            if (_kind == Kind::EXACT) {
                hash ^= _hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }

        /**
         * @brief The annotation value (empty for TYPE_ONLY keys).
         */
        [[nodiscard]]
        const Any& value() const noexcept {
            return _value;
        }

        /**
         * @brief The annotation's type index.
         */
        [[nodiscard]]
        TypeIndex type() const noexcept {
            return _type;
        }

        /**
         * @brief Creates a TYPE_ONLY key for the annotation type {@code T}.
         */
        template <Annotatable T>
        [[nodiscard]]
        static AnnotationKey type_only() {
            return AnnotationKey(TypeIndex(typeid(T)));
        }
    };

    /**
     * @struct BindingKey
     * @brief Map key for a binding: the bound type plus an optional annotation.
     */
    export struct BindingKey {
        TypeIndex type; ///< The bound type.
        Optional<AnnotationKey> annotation; ///< The binding's annotation, if any.

        bool operator==(const BindingKey&) const = default;
    };

    /**
     * @struct BindingKeyHash
     * @brief Hasher for {@code BindingKey}.
     */
    export struct BindingKeyHash {
        [[nodiscard]]
        usize operator()(const BindingKey& key) const {
            usize hash = Hash<TypeIndex>()(key.type);
            if (key.annotation.has_value()) {
                hash ^= key.annotation->hash();
            }
            return hash;
        }
    };
}

#endif
