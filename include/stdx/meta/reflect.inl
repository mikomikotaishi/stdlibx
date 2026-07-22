#pragma once

#ifdef __cpp_lib_reflection
using stdx::collections::EnumSet;
using stdx::collections::Vector;
using stdx::debug::SourceLocation;

namespace reflect = stdx::meta::reflect;
#endif

/**
 * @namespace stdx::meta::reflect
 * @brief Higher-level object wrappers around reflection facilities.
 */
export namespace stdx::meta::reflect {
    #ifdef __cpp_lib_reflection
    /**
     * @enum ReflectionOf
     * @brief The kind of entity reflected by an Info. Used for type dispatch in Mirror and its subclasses.
     */
    enum class ReflectionOf: u8 {
        NONE, ///< Null reflection
        SCALAR, ///< A value of scalar type
        STATIC_OBJECT, ///< An object of static storage duration
        VARIABLE, ///< A variable
        STRUCTURED_BINDING, ///< A structured binding
        FUNCTION, ///< A function
        FUNCTION_PARAMETER, ///< A function parameter
        ENUM, ///< An enumerator
        ANNOTATION, ///< An annotation
        TYPE_ALIAS, ///< A type alias
        TYPE, ///< A type
        MEMBER, ///< A class member
        UNNAMED_BIT_FIELD, ///< An unnamed bit-field
        CLASS_TEMPLATE, ///< A class template
        FUNCTION_TEMPLATE, ///< A function template
        VARIABLE_TEMPLATE, ///< A variable template
        ALIAS_TEMPLATE, ///< An alias template
        CONCEPT, ///< A concept
        NAMESPACE_ALIAS, ///< A namespace alias
        NAMESPACE, ///< A namespace
        BASE_CLASS, ///< A direct base class relationship
        DATA_MEMBER_DESCRIPTION, ///< A data member description
    };

    /**
     * @enum Modifier
     * @brief The kind of modifier applied to an entity.
     */
    enum class Modifier: u8 {
        ALIGNAS, ///< The entity is declared with an alignas specifier or has an alignment specified by the implementation.
        ALIGNOF, ///< The entity is declared with an alignof specifier or has an alignment specified by the implementation.
        CONST, ///< The entity is const-qualified.
        CONSTEVAL, ///< The entity is declared with the consteval specifier.
        CONSTEXPR, ///< The entity is declared with the constexpr specifier.
        CONSTINIT, ///< The entity is declared with the constinit specifier.
        EXPLICIT, ///< The entity is declared with the explicit specifier.
        EXTERN, ///< The entity is declared with the extern specifier or has external linkage.
        FINAL, ///< The entity is declared with the final specifier.
        FRIEND, ///< The entity is a friend of some class.
        INLINE, ///< The entity is declared with the inline specifier.
        MUTABLE, ///< The entity is declared with the mutable specifier.
        NOEXCEPT, ///< The entity is declared with the noexcept specifier.
        OVERRIDE, ///< The entity is declared with the override specifier.
        PRIVATE, ///< The entity is private.
        PROTECTED, ///< The entity is protected.
        PUBLIC, ///< The entity is public.
        RESTRICT, ///< The entity is declared with the restrict specifier.
        SIGNED, ///< The entity is declared with the signed specifier or is of a signed integer type.
        STATIC, ///< The entity is declared with the static specifier or has static storage duration.
        THREAD_LOCAL, ///< The entity is declared with the thread_local specifier or has thread storage duration.
        UNSIGNED, ///< The entity is declared with the unsigned specifier or is of an unsigned integer type.
        VIRTUAL, ///< The entity is declared with the virtual specifier or is a virtual member function.
        VOLATILE, ///< The entity is volatile-qualified.
    };

    /**
     * @enum AccessFlag
     * @brief Access level of a class member.
     */
    enum class AccessFlag: u8 {
        PUBLIC, ///< Public access (default for non-class-scope entities)
        PROTECTED, ///< Protected access (class members declared protected)
        PRIVATE, ///< Private access (class members declared private)
    };

    /**
     * @enum CvQualifier
     * @brief CV qualifiers, which apply to types, member functions, and data members.
     */
    enum class CvQualifier: u8 {
        CONST, ///< The entity is const-qualified.
        VOLATILE, ///< The entity is volatile-qualified.
    };

    /**
     * @enum RefQualifier
     * @brief Reference qualifier on a member function (&, &&, or unqualified).
     */
    enum class RefQualifier: u8 {
        NONE, ///< No reference qualifier
        LVALUE, ///< Lvalue reference qualifier (&)
        RVALUE, ///< Rvalue reference qualifier (&&)
    };

    /**
     * @enum FunctionSpecifier
     * @brief Specifiers that may appear on functions, methods, constructors, and destructors.
     *
     * Not every specifier is meaningful on every kind of entity; methods that return
     * an EnumSet<FunctionSpecifier> only populate the bits applicable to that entity.
     */
    enum class FunctionSpecifier: u8 {
        VIRTUAL, ///< The function is virtual.
        PURE_VIRTUAL, ///< The function is pure virtual.
        OVERRIDE, ///< The function is declared with the override specifier.
        FINAL, ///< The function is declared with the final specifier.
        EXPLICIT, ///< The function is declared with the explicit specifier (constructors only).
        NOEXCEPT, ///< The function is declared with the noexcept specifier.
        STATIC, ///< The function is declared with the static specifier (methods only) or is a static member function.
        DELETED, ///< The function is deleted.
        DEFAULTED, ///< The function is defaulted.
    };

    /**
     * @enum StorageClass
     * @brief Storage class of a variable or static data member.
     */
    enum class StorageClass: u8 {
        NONE, ///< No storage class (automatic storage duration for variables, non-static for data members)
        AUTOMATIC, ///< Automatic storage duration (local variables)
        STATIC, ///< Static storage duration (variables declared with static or data members declared with static)
        THREAD_LOCAL, ///< Thread storage duration (variables declared with thread_local or data members declared with thread_local)
    };

    /**
     * @enum Linkage
     * @brief Linkage of a named entity.
     */
    enum class Linkage: u8 {
        NONE, ///< No linkage (local variables, unnamed entities)
        INTERNAL, ///< Internal linkage (entities declared in an unnamed namespace or with the static specifier at namespace scope)
        MODULE, ///< Module linkage (entities declared in a named module interface unit or with the export specifier)
        EXTERNAL, ///< External linkage (entities declared at namespace scope that do not have internal or module linkage)
        EXTERN_C, ///< C language linkage (entities declared with extern "C")
    };

    class Mirror;
    class Type;
    class Field;
    class Variable;
    class Callback;
    class Method;
    class Constructor;
    class Destructor;
    class Parameter;
    class Enumerator;
    class Base;
    class Namespace;
    class NamespaceAlias;
    class TypeAlias;
    class Concept;
    class Template;
    class Annotation;
    class StructuredBinding;

    template <ReflectableAsClass T>
    class Class;

    template <ReflectableAsEnum E>
    class Enum;

    template <ReflectableAsUnion U>
    class Union;

    /**
     * @class Mirror
     * @brief Base wrapper that pairs every reflection entity with the
     * operations applicable to any reflection.
     *
     * Holds a single Info. Concrete subclasses (Field, Method, ...)
     * narrow the kind and add operations specific to that kind.
     */
    class Mirror {
    public:
        /**
         * The underlying reflection. public so the type satisfies the
         * "structural type" rule (required for define_static_array(),
         * NTTPs, etc.).
         */
        const Info _info = {};

        consteval Mirror() noexcept = default;

        consteval explicit Mirror(Info i) noexcept:
            _info{i} {}

        [[nodiscard]]
        consteval Info value() const noexcept {
            return _info;
        }

        [[nodiscard]]
        consteval operator Info() const noexcept {
            return _info;
        }

        /**
         * @brief The entity's identifier, or nullopt if it is unnamed
         * (anonymous structs, unnamed bit-fields, etc.).
         * @return The identifier, or nullopt if the entity is unnamed.
         */
        [[nodiscard]]
        consteval Optional<StringView> name() const {
            if (!reflect::has_identifier(_info)) {
                return nullopt;
            }
            return reflect::identifier_of(_info);
        }

        /**
         * @brief UTF-8 form of name(). Returns nullopt under the same
         * conditions as name().
         * @return The UTF-8 identifier, or nullopt if the entity is unnamed.
         */
        [[nodiscard]]
        consteval Optional<Utf8StringView> u8name() const {
            if (!reflect::has_identifier(_info)) {
                return nullopt;
            }
            return reflect::u8identifier_of(_info);
        }

        [[nodiscard]]
        consteval StringView display_name() const {
            return reflect::display_string_of(_info);
        }

        [[nodiscard]]
        consteval Utf8StringView u8display_name() const {
            return reflect::u8display_string_of(_info);
        }

        [[nodiscard]]
        consteval SourceLocation location() const {
            return reflect::source_location_of(_info);
        }

        [[nodiscard]]
        consteval bool is_type() const {
            return reflect::is_type(_info);
        }

        [[nodiscard]]
        consteval bool is_namespace() const {
            return reflect::is_namespace(_info);
        }

        [[nodiscard]]
        consteval bool is_function() const {
            return reflect::is_function(_info);
        }

        [[nodiscard]]
        consteval bool is_variable() const {
            return reflect::is_variable(_info);
        }

        [[nodiscard]]
        consteval bool is_template() const {
            return reflect::is_template(_info);
        }

        [[nodiscard]]
        consteval bool is_concept() const {
            return reflect::is_concept(_info);
        }

        [[nodiscard]]
        consteval bool is_value() const {
            return reflect::is_value(_info);
        }

        [[nodiscard]]
        consteval bool is_object() const {
            return reflect::is_object(_info);
        }

        [[nodiscard]]
        consteval bool is_type_alias() const {
            return reflect::is_type_alias(_info);
        }

        [[nodiscard]]
        consteval bool is_namespace_alias() const {
            return reflect::is_namespace_alias(_info);
        }

        [[nodiscard]]
        consteval bool is_annotation() const {
            return reflect::is_annotation(_info);
        }

        [[nodiscard]]
        consteval bool is_enumerator() const {
            return reflect::is_enumerator(_info);
        }

        [[nodiscard]]
        consteval bool is_structured_binding() const {
            return reflect::is_structured_binding(_info);
        }

        [[nodiscard]]
        consteval bool is_class_member() const {
            return reflect::is_class_member(_info);
        }

        [[nodiscard]]
        consteval bool is_namespace_member() const {
            return reflect::is_namespace_member(_info);
        }

        [[nodiscard]]
        consteval bool is_static_member() const {
            return reflect::is_static_member(_info);
        }

        [[nodiscard]]
        consteval bool is_nonstatic_data_member() const {
            return reflect::is_nonstatic_data_member(_info);
        }

        [[nodiscard]]
        consteval bool is_base() const {
            return reflect::is_base(_info);
        }

        /**
         * @brief The enclosing entity (the namespace, class, function, etc.
         * that this entity is declared inside), or nullopt for the global
         * namespace.
         */
        [[nodiscard]]
        consteval Optional<Mirror> parent() const {
            if (!reflect::has_parent(_info)) {
                return nullopt;
            }
            return Mirror(reflect::parent_of(_info));
        }

        /**
         * @brief True iff this reflection is the global namespace.
         */
        [[nodiscard]]
        consteval bool is_global_namespace() const {
            return reflect::is_namespace(_info) && !reflect::has_parent(_info);
        }

        /**
         * @brief True iff this entity is declared directly at global namespace
         * scope. False for the global namespace itself.
         */
        [[nodiscard]]
        consteval bool is_in_global_namespace() const {
            if (!reflect::has_parent(_info)) {
                return false;
            }
            const Info p = reflect::parent_of(_info);
            return reflect::is_namespace(p) && !reflect::has_parent(p);
        }

        /**
         * @brief Number of enclosing entities up to (but not including) the
         * global namespace. Counts class scopes, function scopes, etc.
         *
         * For stdx::time::Duration::operator+, returns 4 (operator+ -> Duration
         * -> time -> stdx -> global). For the global namespace itself, returns 0.
         */
        [[nodiscard]]
        consteval usize scope_depth() const {
            usize d = 0;
            Info cur = _info;
            while (reflect::has_parent(cur)) {
                cur = reflect::parent_of(cur);
                ++d;
            }
            return d;
        }

        /**
         * @brief Number of enclosing namespaces up to (but not including) the
         * global namespace. Skips class and other non-namespace scopes.
         *
         * For stdx::meta::reflect::Mirror, returns 3 (reflect, meta, stdx).
         */
        [[nodiscard]]
        consteval usize namespace_depth() const {
            usize d = 0;
            Info cur = _info;
            while (reflect::has_parent(cur)) {
                cur = reflect::parent_of(cur);
                if (reflect::is_namespace(cur)) {
                    ++d;
                }
            }
            return d;
        }

        /**
         * @brief All enclosing scopes from outermost to innermost (excludes
         * this entity itself). For ::stdx::time::Duration, returns
         * { ::, stdx, time }. Empty for the global namespace.
         */
        [[nodiscard]]
        consteval Vector<Mirror> scope_chain() const {
            Vector<Mirror> tmp;
            Info cur = _info;
            while (reflect::has_parent(cur)) {
                cur = reflect::parent_of(cur);
                tmp.emplace_back(cur);
            }
            Vector<Mirror> result;
            result.reserve(tmp.size());
            for (usize i = tmp.size(); i > 0; --i) {
                result.emplace_back(tmp[i - 1]._info);
            }
            return result;
        }

        /**
         * @brief Innermost enclosing class, or nullopt if this entity is not
         * declared inside any class (e.g. namespace-scoped entities).
         */
        [[nodiscard]]
        consteval Optional<Type> enclosing_class() const;

        /**
         * @brief Innermost enclosing namespace. For the global namespace
         * itself, returns the global namespace. Walks through class scopes
         * to find the first namespace ancestor.
         */
        [[nodiscard]]
        consteval Namespace enclosing_namespace() const;

        [[nodiscard]]
        consteval AccessFlag access() const {
            if (reflect::is_protected(_info)) {
                return AccessFlag::PROTECTED;
            }
            if (reflect::is_private(_info)) {
                return AccessFlag::PRIVATE;
            }
            return AccessFlag::PUBLIC;
        }

        [[nodiscard]]
        consteval EnumSet<ReflectionOf> kinds() const {
            if (_info == Info{}) {
                return EnumSet<ReflectionOf>::of(ReflectionOf::NONE);
            }
            EnumSet<ReflectionOf> result;
            if (reflect::is_value(_info)) {
                result.insert(ReflectionOf::SCALAR);
            }
            if (reflect::is_object(_info)) {
                result.insert(ReflectionOf::STATIC_OBJECT);
            }
            if (reflect::is_variable(_info)) {
                result.insert(ReflectionOf::VARIABLE);
            }
            if (reflect::is_structured_binding(_info)) {
                result.insert(ReflectionOf::STRUCTURED_BINDING);
            }
            if (reflect::is_function(_info)) {
                result.insert(ReflectionOf::FUNCTION);
            }
            if (reflect::is_function_parameter(_info)) {
                result.insert(ReflectionOf::FUNCTION_PARAMETER);
            }
            if (reflect::is_enumerator(_info)) {
                result.insert(ReflectionOf::ENUM);
            }
            if (reflect::is_annotation(_info)) {
                result.insert(ReflectionOf::ANNOTATION);
            }
            if (reflect::is_type_alias(_info)) {
                result.insert(ReflectionOf::TYPE_ALIAS);
            }
            if (reflect::is_type(_info)) {
                result.insert(ReflectionOf::TYPE);
            }
            if (reflect::is_class_member(_info)) {
                result.insert(ReflectionOf::MEMBER);
            }
            if (reflect::is_bit_field(_info) && !reflect::has_identifier(_info)) {
                result.insert(ReflectionOf::UNNAMED_BIT_FIELD);
            }
            if (reflect::is_class_template(_info)) {
                result.insert(ReflectionOf::CLASS_TEMPLATE);
            }
            if (reflect::is_function_template(_info)) {
                result.insert(ReflectionOf::FUNCTION_TEMPLATE);
            }
            if (reflect::is_variable_template(_info)) {
                result.insert(ReflectionOf::VARIABLE_TEMPLATE);
            }
            if (reflect::is_alias_template(_info)) {
                result.insert(ReflectionOf::ALIAS_TEMPLATE);
            }
            if (reflect::is_concept(_info)) {
                result.insert(ReflectionOf::CONCEPT);
            }
            if (reflect::is_namespace_alias(_info)) {
                result.insert(ReflectionOf::NAMESPACE_ALIAS);
            }
            if (reflect::is_namespace(_info)) {
                result.insert(ReflectionOf::NAMESPACE);
            }
            if (reflect::is_base(_info)) {
                result.insert(ReflectionOf::BASE_CLASS);
            }
            if (reflect::is_data_member_spec(_info)) {
                result.insert(ReflectionOf::DATA_MEMBER_DESCRIPTION);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Annotation> annotations() const;

        template <typename T>
        [[nodiscard]]
        consteval Vector<Annotation> annotations_with_type() const;

        [[nodiscard]]
        constexpr bool operator==(const Mirror& other) const noexcept = default;
    };

    /**
     * @class Type
     * @brief Wraps the reflection of a type.
     * @extends Mirror
     *
     * Exposes the full battery of type traits (size, alignment, category
     * tests, qualifier removal/addition, relations). Subclassed by
     * Class<T>, Enum<E>, and Union<U> for entities whose type is
     * known statically.
     */
    class Type: public Mirror {
    public:
        consteval Type() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Type(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_type(i)) {
                throw ReflectiveOperationException("Provided Info is not a type", ^^Type);
            }
        }

        [[nodiscard]]
        consteval usize size() const {
            return reflect::size_of(_info);
        }

        [[nodiscard]]
        consteval usize alignment() const {
            return reflect::alignment_of(_info);
        }

        [[nodiscard]]
        consteval usize bit_size() const {
            return reflect::bit_size_of(_info);
        }

        [[nodiscard]]
        consteval bool is_void() const {
            return reflect::is_void_type(_info);
        }

        [[nodiscard]]
        consteval bool is_null_pointer() const {
            return reflect::is_null_pointer_type(_info);
        }

        [[nodiscard]]
        consteval bool is_integral() const {
            return reflect::is_integral_type(_info);
        }

        [[nodiscard]]
        consteval bool is_floating_point() const {
            return reflect::is_floating_point_type(_info);
        }

        [[nodiscard]]
        consteval bool is_array() const {
            return reflect::is_array_type(_info);
        }

        [[nodiscard]]
        consteval bool is_pointer() const {
            return reflect::is_pointer_type(_info);
        }

        [[nodiscard]]
        consteval bool is_reference() const {
            return reflect::is_reference_type(_info);
        }

        [[nodiscard]]
        consteval bool is_lvalue_reference() const {
            return reflect::is_lvalue_reference_type(_info);
        }

        [[nodiscard]]
        consteval bool is_rvalue_reference() const {
            return reflect::is_rvalue_reference_type(_info);
        }

        [[nodiscard]]
        consteval bool is_enum() const {
            return reflect::is_enum_type(_info);
        }

        [[nodiscard]]
        consteval bool is_union() const {
            return reflect::is_union_type(_info);
        }

        [[nodiscard]]
        consteval bool is_class() const {
            return reflect::is_class_type(_info) && !reflect::is_union_type(_info);
        }

        [[nodiscard]]
        consteval bool is_record() const {
            return reflect::is_class_type(_info);
        }

        [[nodiscard]]
        consteval bool is_function_type() const {
            return reflect::is_function_type(_info);
        }

        [[nodiscard]]
        consteval bool is_member_pointer() const {
            return reflect::is_member_pointer_type(_info);
        }

        [[nodiscard]]
        consteval bool is_member_object_pointer() const {
            return reflect::is_member_object_pointer_type(_info);
        }

        [[nodiscard]]
        consteval bool is_member_function_pointer() const {
            return reflect::is_member_function_pointer_type(_info);
        }

        [[nodiscard]]
        consteval bool is_reflection() const {
            return reflect::is_reflection_type(_info);
        }

        [[nodiscard]]
        consteval bool is_arithmetic() const {
            return reflect::is_arithmetic_type(_info);
        }

        [[nodiscard]]
        consteval bool is_fundamental() const {
            return reflect::is_fundamental_type(_info);
        }

        [[nodiscard]]
        consteval bool is_scalar() const {
            return reflect::is_scalar_type(_info);
        }

        [[nodiscard]]
        consteval bool is_object_type() const {
            return reflect::is_object_type(_info);
        }

        [[nodiscard]]
        consteval bool is_compound() const {
            return reflect::is_compound_type(_info);
        }

        [[nodiscard]]
        consteval bool is_const() const {
            return reflect::is_const_type(_info);
        }

        [[nodiscard]]
        consteval bool is_volatile() const {
            return reflect::is_volatile_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_copyable() const {
            return reflect::is_trivially_copyable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_standard_layout() const {
            return reflect::is_standard_layout_type(_info);
        }

        [[nodiscard]]
        consteval bool is_empty() const {
            return reflect::is_empty_type(_info);
        }

        [[nodiscard]]
        consteval bool is_polymorphic() const {
            return reflect::is_polymorphic_type(_info);
        }

        [[nodiscard]]
        consteval bool is_abstract() const {
            return reflect::is_abstract_type(_info);
        }

        [[nodiscard]]
        consteval bool is_final() const {
            return reflect::is_final_type(_info);
        }

        [[nodiscard]]
        consteval bool is_aggregate() const {
            return reflect::is_aggregate_type(_info);
        }

        [[nodiscard]]
        consteval bool is_signed() const {
            return reflect::is_signed_type(_info);
        }

        [[nodiscard]]
        consteval bool is_unsigned() const {
            return reflect::is_unsigned_type(_info);
        }

        [[nodiscard]]
        consteval bool is_bounded_array() const {
            return reflect::is_bounded_array_type(_info);
        }

        [[nodiscard]]
        consteval bool is_unbounded_array() const {
            return reflect::is_unbounded_array_type(_info);
        }

        [[nodiscard]]
        consteval bool is_scoped_enum() const {
            return reflect::is_scoped_enum_type(_info);
        }

        [[nodiscard]]
        consteval bool is_complete() const {
            return reflect::is_complete_type(_info);
        }

        [[nodiscard]]
        consteval bool is_enumerable() const {
            return reflect::is_enumerable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_implicit_lifetime() const {
            return reflect::is_implicit_lifetime_type(_info);
        }

        [[nodiscard]]
        consteval bool has_virtual_destructor() const {
            return reflect::has_virtual_destructor(_info);
        }

        [[nodiscard]]
        consteval bool has_unique_object_representations() const {
            return reflect::has_unique_object_representations(_info);
        }

        [[nodiscard]]
        consteval bool is_default_constructible() const {
            return reflect::is_default_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_copy_constructible() const {
            return reflect::is_copy_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_move_constructible() const {
            return reflect::is_move_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_copy_assignable() const {
            return reflect::is_copy_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_move_assignable() const {
            return reflect::is_move_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_destructible() const {
            return reflect::is_destructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_swappable() const {
            return reflect::is_swappable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_default_constructible() const {
            return reflect::is_trivially_default_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_copy_constructible() const {
            return reflect::is_trivially_copy_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_move_constructible() const {
            return reflect::is_trivially_move_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_copy_assignable() const {
            return reflect::is_trivially_copy_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_move_assignable() const {
            return reflect::is_trivially_move_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_trivially_destructible() const {
            return reflect::is_trivially_destructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_default_constructible() const {
            return reflect::is_nothrow_default_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_copy_constructible() const {
            return reflect::is_nothrow_copy_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_move_constructible() const {
            return reflect::is_nothrow_move_constructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_copy_assignable() const {
            return reflect::is_nothrow_copy_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_move_assignable() const {
            return reflect::is_nothrow_move_assignable_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_destructible() const {
            return reflect::is_nothrow_destructible_type(_info);
        }

        [[nodiscard]]
        consteval bool is_nothrow_swappable() const {
            return reflect::is_nothrow_swappable_type(_info);
        }

        [[nodiscard]]
        consteval usize rank() const {
            return reflect::rank(_info);
        }

        [[nodiscard]]
        consteval usize extent(usize dim = 0uz) const {
            return reflect::extent(_info, dim);
        }

        [[nodiscard]]
        consteval Type remove_const() const {
            return Type(reflect::remove_const(_info));
        }

        [[nodiscard]]
        consteval Type remove_volatile() const {
            return Type(reflect::remove_volatile(_info));
        }

        [[nodiscard]]
        consteval Type remove_cv() const {
            return Type(reflect::remove_cv(_info));
        }

        [[nodiscard]]
        consteval Type add_const() const {
            return Type(reflect::add_const(_info));
        }

        [[nodiscard]]
        consteval Type add_volatile() const {
            return Type(reflect::add_volatile(_info));
        }

        [[nodiscard]]
        consteval Type add_cv() const {
            return Type(reflect::add_cv(_info));
        }

        [[nodiscard]]
        consteval Type remove_reference() const {
            return Type(reflect::remove_reference(_info));
        }

        [[nodiscard]]
        consteval Type add_lvalue_reference() const {
            return Type(reflect::add_lvalue_reference(_info));
        }

        [[nodiscard]]
        consteval Type add_rvalue_reference() const {
            return Type(reflect::add_rvalue_reference(_info));
        }

        [[nodiscard]]
        consteval Type remove_pointer() const {
            return Type(reflect::remove_pointer(_info));
        }

        [[nodiscard]]
        consteval Type add_pointer() const {
            return Type(reflect::add_pointer(_info));
        }

        [[nodiscard]]
        consteval Type remove_cvref() const {
            return Type(reflect::remove_cvref(_info));
        }

        [[nodiscard]]
        consteval Type decay() const {
            return Type(reflect::decay(_info));
        }

        [[nodiscard]]
        consteval Type remove_extent() const {
            return Type(reflect::remove_extent(_info));
        }

        [[nodiscard]]
        consteval Type remove_all_extents() const {
            return Type(reflect::remove_all_extents(_info));
        }

        [[nodiscard]]
        consteval Type make_signed() const {
            return Type(reflect::make_signed(_info));
        }

        [[nodiscard]]
        consteval Type make_unsigned() const {
            return Type(reflect::make_unsigned(_info));
        }

        [[nodiscard]]
        consteval Type underlying() const {
            return Type(reflect::underlying_type(_info));
        }

        [[nodiscard]]
        consteval Type dealias() const {
            return Type(reflect::dealias(_info));
        }

        [[nodiscard]]
        consteval bool same_as(Type other) const {
            return reflect::is_same_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool base_of(Type other) const {
            return reflect::is_base_of_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool virtual_base_of(Type other) const {
            return reflect::is_virtual_base_of_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool convertible_to(Type other) const {
            return reflect::is_convertible_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool nothrow_convertible_to(Type other) const {
            return reflect::is_nothrow_convertible_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool layout_compatible_with(Type other) const {
            return reflect::is_layout_compatible_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool pointer_interconvertible_base_of(Type other) const {
            return reflect::is_pointer_interconvertible_base_of_type(_info, other._info);
        }

        [[nodiscard]]
        consteval bool has_template_arguments() const {
            return reflect::has_template_arguments(_info);
        }

        [[nodiscard]]
        consteval Vector<Mirror> template_arguments() const {
            Vector<Mirror> result;
            for (Info a : reflect::template_arguments_of(_info)) {
                result.emplace_back(a);
            }
            return result;
        }

        [[nodiscard]]
        consteval EnumSet<CvQualifier> cv_qualifiers() const {
            EnumSet<CvQualifier> result;
            if (reflect::is_const_type(_info)) {
                result.insert(CvQualifier::CONST);
            }
            if (reflect::is_volatile_type(_info)) {
                result.insert(CvQualifier::VOLATILE);
            }
            return result;
        }

        [[nodiscard]]
        consteval Template template_of() const;
    };

    /**
     * @class Parameter
     * @brief A parameter of a function.
     * @extends Mirror
     */
    class Parameter: public Mirror {
    public:
        consteval Parameter() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Parameter(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_function_parameter(i)) {
                throw ReflectiveOperationException("Provided Info is not a function parameter", ^^Parameter);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        [[nodiscard]]
        consteval bool has_default() const {
            return reflect::has_default_argument(_info);
        }

        [[nodiscard]]
        consteval bool is_explicit_object() const {
            return reflect::is_explicit_object_parameter(_info);
        }

        [[nodiscard]]
        consteval bool is_function_parameter() const {
            return reflect::is_function_parameter(_info);
        }
    };

    /**
     * @class Callback
     * @brief A free or unspecified function. Member functions are
     * represented by Method, constructors/destructors by their
     * own wrappers.
     * @extends Mirror
     */
    class Callback: public Mirror {
    public:
        consteval Callback() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Callback(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_function(i)) {
                throw ReflectiveOperationException("Provided Info is not a function", ^^Callback);
            }
        }

        [[nodiscard]]
        consteval Type return_type() const {
            return Type(reflect::return_type_of(_info));
        }

        [[nodiscard]]
        consteval Vector<Parameter> parameters() const {
            Vector<Parameter> result;
            for (Info p : reflect::parameters_of(_info)) {
                result.emplace_back(p);
            }
            return result;
        }

        [[nodiscard]]
        consteval bool is_noexcept() const {
            return reflect::is_noexcept(_info);
        }

        [[nodiscard]]
        consteval bool is_deleted() const {
            return reflect::is_deleted(_info);
        }

        [[nodiscard]]
        consteval bool is_defaulted() const {
            return reflect::is_defaulted(_info);
        }

        [[nodiscard]]
        consteval bool is_explicit() const {
            return reflect::is_explicit(_info);
        }

        [[nodiscard]]
        consteval bool is_user_provided() const {
            return reflect::is_user_provided(_info);
        }

        [[nodiscard]]
        consteval bool is_user_declared() const {
            return reflect::is_user_declared(_info);
        }

        [[nodiscard]]
        consteval bool is_vararg() const {
            return reflect::is_vararg_function(_info);
        }

        [[nodiscard]]
        consteval bool is_conversion() const {
            return reflect::is_conversion_function(_info);
        }

        [[nodiscard]]
        consteval bool is_operator() const {
            return reflect::is_operator_function(_info);
        }

        [[nodiscard]]
        consteval bool is_literal_operator() const {
            return reflect::is_literal_operator(_info);
        }

        [[nodiscard]]
        consteval StringView operator_symbol() const {
            return reflect::symbol_of(reflect::operator_of(_info));
        }

        [[nodiscard]]
        consteval Utf8StringView u8operator_symbol() const {
            return reflect::u8symbol_of(reflect::operator_of(_info));
        }
    };

    /**
     * @class Method
     * @brief A method of a class.
     * @extends Callback
     */
    class Method: public Callback {
    public:
        consteval Method() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Method(Info i):
            Callback(i)
        {
            if (i != Info{} &&
                (!reflect::is_class_member(i) || reflect::is_constructor(i) || reflect::is_destructor(i)))
            {
                throw ReflectiveOperationException("Provided Info is not a (non-special) class member function", ^^Method);
            }
        }

        [[nodiscard]]
        consteval Type declaring_class() const {
            return Type(reflect::parent_of(_info));
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }

        [[nodiscard]]
        consteval bool is_virtual() const {
            return reflect::is_virtual(_info);
        }

        [[nodiscard]]
        consteval bool is_pure_virtual() const {
            return reflect::is_pure_virtual(_info);
        }

        [[nodiscard]]
        consteval bool is_override() const {
            return reflect::is_override(_info);
        }

        [[nodiscard]]
        consteval bool is_final() const {
            return reflect::is_final(_info);
        }

        [[nodiscard]]
        consteval bool is_static() const {
            return reflect::is_static_member(_info);
        }

        [[nodiscard]]
        consteval bool is_const() const {
            return reflect::is_const(_info);
        }

        [[nodiscard]]
        consteval bool is_volatile() const {
            return reflect::is_volatile(_info);
        }

        [[nodiscard]]
        consteval bool is_lvalue_ref_qualified() const {
            return reflect::is_lvalue_reference_qualified(_info);
        }

        [[nodiscard]]
        consteval bool is_rvalue_ref_qualified() const {
            return reflect::is_rvalue_reference_qualified(_info);
        }

        [[nodiscard]]
        consteval bool is_special_member() const {
            return reflect::is_special_member_function(_info);
        }

        [[nodiscard]]
        consteval bool is_assignment() const {
            return reflect::is_assignment(_info);
        }

        [[nodiscard]]
        consteval bool is_copy_assignment() const {
            return reflect::is_copy_assignment(_info);
        }

        [[nodiscard]]
        consteval bool is_move_assignment() const {
            return reflect::is_move_assignment(_info);
        }

        [[nodiscard]]
        consteval EnumSet<CvQualifier> cv_qualifiers() const {
            EnumSet<CvQualifier> result;
            if (reflect::is_const(_info)) {
                result.insert(CvQualifier::CONST);
            }
            if (reflect::is_volatile(_info)) {
                result.insert(CvQualifier::VOLATILE);
            }
            return result;
        }

        [[nodiscard]]
        consteval RefQualifier ref_qualifier() const {
            if (reflect::is_lvalue_reference_qualified(_info)) {
                return RefQualifier::LVALUE;
            }
            if (reflect::is_rvalue_reference_qualified(_info)) {
                return RefQualifier::RVALUE;
            }
            return RefQualifier::NONE;
        }

        [[nodiscard]]
        consteval EnumSet<FunctionSpecifier> specifiers() const {
            EnumSet<FunctionSpecifier> result;
            if (reflect::is_virtual(_info)) {
                result.insert(FunctionSpecifier::VIRTUAL);
            }
            if (reflect::is_pure_virtual(_info)) {
                result.insert(FunctionSpecifier::PURE_VIRTUAL);
            }
            if (reflect::is_override(_info)) {
                result.insert(FunctionSpecifier::OVERRIDE);
            }
            if (reflect::is_final(_info)) {
                result.insert(FunctionSpecifier::FINAL);
            }
            if (reflect::is_explicit(_info)) {
                result.insert(FunctionSpecifier::EXPLICIT);
            }
            if (reflect::is_noexcept(_info)) {
                result.insert(FunctionSpecifier::NOEXCEPT);
            }
            if (reflect::is_static_member(_info)) {
                result.insert(FunctionSpecifier::STATIC);
            }
            if (reflect::is_deleted(_info)) {
                result.insert(FunctionSpecifier::DELETED);
            }
            if (reflect::is_defaulted(_info)) {
                result.insert(FunctionSpecifier::DEFAULTED);
            }
            return result;
        }
    };

    /**
     * @class Field
     * @brief A field of a class.
     * @extends Mirror
     */
    class Field: public Mirror {
    public:
        consteval Field() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Field(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_nonstatic_data_member(i)) {
                throw ReflectiveOperationException("Provided Info is not a non-static data member", ^^Field);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        [[nodiscard]]
        consteval Type declaring_class() const {
            return Type(reflect::parent_of(_info));
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }

        [[nodiscard]]
        consteval bool is_mutable() const {
            return reflect::is_mutable_member(_info);
        }

        [[nodiscard]]
        consteval bool is_bit_field() const {
            return reflect::is_bit_field(_info);
        }

        [[nodiscard]]
        consteval bool has_default_initializer() const {
            return reflect::has_default_member_initializer(_info);
        }

        [[nodiscard]]
        consteval MemberOffset offset() const {
            return reflect::offset_of(_info);
        }

        [[nodiscard]]
        consteval usize bit_size() const {
            return reflect::bit_size_of(_info);
        }

        [[nodiscard]]
        consteval EnumSet<CvQualifier> cv_qualifiers() const {
            return Type(reflect::type_of(_info)).cv_qualifiers();
        }
    };

    /**
     * @class Variable
     * @brief A variable.
     * @extends Mirror
     */
    class Variable: public Mirror {
    public:
        consteval Variable() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Variable(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_variable(i)) {
                throw ReflectiveOperationException("Provided Info is not a variable", ^^Variable);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        [[nodiscard]]
        consteval bool is_const() const {
            return reflect::is_const(_info);
        }

        [[nodiscard]]
        consteval bool is_volatile() const {
            return reflect::is_volatile(_info);
        }

        [[nodiscard]]
        consteval bool has_static_storage() const {
            return reflect::has_static_storage_duration(_info);
        }

        [[nodiscard]]
        consteval bool has_thread_storage() const {
            return reflect::has_thread_storage_duration(_info);
        }

        [[nodiscard]]
        consteval bool has_automatic_storage() const {
            return reflect::has_automatic_storage_duration(_info);
        }

        [[nodiscard]]
        consteval bool has_internal_linkage() const {
            return reflect::has_internal_linkage(_info);
        }

        [[nodiscard]]
        consteval bool has_module_linkage() const {
            return reflect::has_module_linkage(_info);
        }

        [[nodiscard]]
        consteval bool has_external_linkage() const {
            return reflect::has_external_linkage(_info);
        }

        [[nodiscard]]
        consteval bool has_c_language_linkage() const {
            return reflect::has_c_language_linkage(_info);
        }

        [[nodiscard]]
        consteval bool has_linkage() const {
            return reflect::has_linkage(_info);
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }

        [[nodiscard]]
        consteval EnumSet<CvQualifier> cv_qualifiers() const {
            return Type(reflect::type_of(_info)).cv_qualifiers();
        }

        [[nodiscard]]
        consteval StorageClass storage_class() const {
            if (reflect::has_thread_storage_duration(_info)) {
                return StorageClass::THREAD_LOCAL;
            }
            if (reflect::has_static_storage_duration(_info)) {
                return StorageClass::STATIC;
            }
            if (reflect::has_automatic_storage_duration(_info)) {
                return StorageClass::AUTOMATIC;
            }
            return StorageClass::NONE;
        }

        [[nodiscard]]
        consteval Linkage linkage() const {
            if (reflect::has_c_language_linkage(_info)) {
                return Linkage::EXTERN_C;
            }
            if (reflect::has_external_linkage(_info)) {
                return Linkage::EXTERNAL;
            }
            if (reflect::has_module_linkage(_info)) {
                return Linkage::MODULE;
            }
            if (reflect::has_internal_linkage(_info)) {
                return Linkage::INTERNAL;
            }
            return Linkage::NONE;
        }
    };

    /**
     * @class Constructor
     * @brief Represents a constructor in the reflection system.
     * @extends Mirror
     */
    class Constructor: public Mirror {
    public:
        consteval Constructor() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Constructor(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_constructor(i)) {
                throw ReflectiveOperationException("Provided Info is not a constructor", ^^Constructor);
            }
        }

        [[nodiscard]]
        consteval Type declaring_class() const {
            return Type(reflect::parent_of(_info));
        }

        [[nodiscard]]
        consteval Vector<Parameter> parameters() const {
            Vector<Parameter> result;
            for (Info p : reflect::parameters_of(_info)) {
                result.emplace_back(p);
            }
            return result;
        }

        [[nodiscard]]
        consteval bool is_default() const {
            return reflect::is_default_constructor(_info);
        }

        [[nodiscard]]
        consteval bool is_copy() const {
            return reflect::is_copy_constructor(_info);
        }

        [[nodiscard]]
        consteval bool is_move() const {
            return reflect::is_move_constructor(_info);
        }

        [[nodiscard]]
        consteval bool is_explicit() const {
            return reflect::is_explicit(_info);
        }

        [[nodiscard]]
        consteval bool is_deleted() const {
            return reflect::is_deleted(_info);
        }

        [[nodiscard]]
        consteval bool is_defaulted() const {
            return reflect::is_defaulted(_info);
        }

        [[nodiscard]]
        consteval bool is_noexcept() const {
            return reflect::is_noexcept(_info);
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }

        [[nodiscard]]
        consteval EnumSet<FunctionSpecifier> specifiers() const {
            EnumSet<FunctionSpecifier> result;
            if (reflect::is_explicit(_info)) {
                result.insert(FunctionSpecifier::EXPLICIT);
            }
            if (reflect::is_noexcept(_info)) {
                result.insert(FunctionSpecifier::NOEXCEPT);
            }
            if (reflect::is_deleted(_info)) {
                result.insert(FunctionSpecifier::DELETED);
            }
            if (reflect::is_defaulted(_info)) {
                result.insert(FunctionSpecifier::DEFAULTED);
            }
            return result;
        }
    };

    /**
     * @class Destructor
     * @brief Represents a destructor in the reflection system.
     * @extends Mirror
     */
    class Destructor: public Mirror {
    public:
        consteval Destructor() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Destructor(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_destructor(i)) {
                throw ReflectiveOperationException("Provided Info is not a destructor", ^^Destructor);
            }
        }

        [[nodiscard]]
        consteval Type declaring_class() const {
            return Type(reflect::parent_of(_info));
        }

        [[nodiscard]]
        consteval bool is_virtual() const {
            return reflect::is_virtual(_info);
        }

        [[nodiscard]]
        consteval bool is_pure_virtual() const {
            return reflect::is_pure_virtual(_info);
        }

        [[nodiscard]]
        consteval bool is_deleted() const {
            return reflect::is_deleted(_info);
        }

        [[nodiscard]]
        consteval bool is_defaulted() const {
            return reflect::is_defaulted(_info);
        }

        [[nodiscard]]
        consteval bool is_noexcept() const {
            return reflect::is_noexcept(_info);
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }

        [[nodiscard]]
        consteval EnumSet<FunctionSpecifier> specifiers() const {
            EnumSet<FunctionSpecifier> result;
            if (reflect::is_virtual(_info)) {
                result.insert(FunctionSpecifier::VIRTUAL);
            }
            if (reflect::is_pure_virtual(_info)) {
                result.insert(FunctionSpecifier::PURE_VIRTUAL);
            }
            if (reflect::is_noexcept(_info)) {
                result.insert(FunctionSpecifier::NOEXCEPT);
            }
            if (reflect::is_deleted(_info)) {
                result.insert(FunctionSpecifier::DELETED);
            }
            if (reflect::is_defaulted(_info)) {
                result.insert(FunctionSpecifier::DEFAULTED);
            }
            return result;
        }
    };

    /**
     * @class Base
     * @brief Represents a base class in the reflection system.
     * @extends Mirror
     */
    class Base: public Mirror {
    public:
        consteval Base() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Base(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_base(i)) {
                throw ReflectiveOperationException("Provided Info is not a base class", ^^Base);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        [[nodiscard]]
        consteval bool is_virtual() const {
            return reflect::is_virtual(_info);
        }

        [[nodiscard]]
        consteval bool is_public() const {
            return reflect::is_public(_info);
        }

        [[nodiscard]]
        consteval bool is_protected() const {
            return reflect::is_protected(_info);
        }

        [[nodiscard]]
        consteval bool is_private() const {
            return reflect::is_private(_info);
        }
    };

    /**
     * @class Enumerator
     * @brief Represents an enumerator in the reflection system.
     * @extends Mirror
     */
    class Enumerator: public Mirror {
    public:
        consteval Enumerator() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Enumerator(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_enumerator(i)) {
                throw ReflectiveOperationException("Provided Info is not an enumerator", ^^Enumerator);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        template <typename T>
        [[nodiscard]]
        consteval T as() const {
            return reflect::extract<T>(_info);
        }
    };

    /**
     * @class Namespace
     * @brief Represents a namespace in the reflection system.
     * @extends Mirror
     */
    class Namespace: public Mirror {
    public:
        consteval Namespace() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Namespace(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_namespace(i)) {
                throw ReflectiveOperationException("Provided Info is not a namespace", ^^Namespace);
            }
        }

        /**
         * @brief Distance from the global namespace. The global namespace
         * itself has depth 0; top-level namespaces have depth 1; etc.
         *
         * Unambiguous on Namespace because a namespace's parent chain
         * consists entirely of namespaces.
         */
        [[nodiscard]]
        consteval usize depth() const {
            return scope_depth();
        }

        [[nodiscard]]
        consteval Vector<Mirror> members(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Mirror> result;
            for (Info m : reflect::members_of(_info, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }
    };

    /**
     * @class NamespaceAlias
     * @brief Represents a namespace alias in the reflection system.
     * @extends Mirror
     */
    class NamespaceAlias: public Mirror {
    public:
        consteval NamespaceAlias() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit NamespaceAlias(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_namespace_alias(i)) {
                throw ReflectiveOperationException("Provided Info is not a namespace alias", ^^NamespaceAlias);
            }
        }

        [[nodiscard]]
        consteval Namespace target() const {
            return Namespace(reflect::dealias(_info));
        }
    };

    /**
     * @class TypeAlias
     * @brief Represents a type alias in the reflection system.
     * @extends Mirror
     */
    class TypeAlias: public Mirror {
    public:
        consteval TypeAlias() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit TypeAlias(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_type_alias(i)) {
                throw ReflectiveOperationException("Provided Info is not a type alias", ^^TypeAlias);
            }
        }

        [[nodiscard]]
        consteval Type target() const {
            return Type(reflect::dealias(_info));
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::dealias(_info));
        }
    };

    /**
     * @class Concept
     * @brief Represents a concept in the reflection system.
     * @extends Mirror
     */
    class Concept: public Mirror {
    public:
        consteval Concept() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Concept(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_concept(i)) {
                throw ReflectiveOperationException("Provided Info is not a concept", ^^Concept);
            }
        }

        template <typename... Args>
        [[nodiscard]]
        consteval bool can_substitute() const {
            return reflect::can_substitute(_info, Vector<Info>{^^Args...});
        }

        template <typename... Args>
        [[nodiscard]]
        consteval Info substitute() const {
            return reflect::substitute(_info, Vector<Info>{^^Args...});
        }
    };

    /**
     * @class Template
     * @brief Represents a template in the reflection system.
     * @extends Mirror
     */
    class Template: public Mirror {
    public:
        consteval Template() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Template(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_template(i)) {
                throw ReflectiveOperationException("Provided Info is not a template", ^^Template);
            }
        }

        [[nodiscard]]
        consteval bool is_class_template() const {
            return reflect::is_class_template(_info);
        }

        [[nodiscard]]
        consteval bool is_function_template() const {
            return reflect::is_function_template(_info);
        }

        [[nodiscard]]
        consteval bool is_variable_template() const {
            return reflect::is_variable_template(_info);
        }

        [[nodiscard]]
        consteval bool is_alias_template() const {
            return reflect::is_alias_template(_info);
        }

        [[nodiscard]]
        consteval bool is_constructor_template() const {
            return reflect::is_constructor_template(_info);
        }

        [[nodiscard]]
        consteval bool is_conversion_function_template() const {
            return reflect::is_conversion_function_template(_info);
        }

        [[nodiscard]]
        consteval bool is_operator_function_template() const {
            return reflect::is_operator_function_template(_info);
        }

        [[nodiscard]]
        consteval bool is_literal_operator_template() const {
            return reflect::is_literal_operator_template(_info);
        }

        template <typename... Args>
        [[nodiscard]]
        consteval bool can_substitute() const {
            return reflect::can_substitute(_info, Vector<Info>{^^Args...});
        }

        template <typename... Args>
        [[nodiscard]]
        consteval Info substitute() const {
            return reflect::substitute(_info, Vector<Info>{^^Args...});
        }
    };

    /**
     * @class Annotation
     * @brief Represents an annotation in the reflection system.
     * @extends Mirror
     */
    class Annotation: public Mirror {
    public:
        consteval Annotation() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit Annotation(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_annotation(i)) {
                throw ReflectiveOperationException("Provided Info is not an annotation", ^^Annotation);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }

        template <typename T>
        [[nodiscard]]
        consteval T as() const {
            return reflect::extract<T>(_info);
        }
    };

    /**
     * @class StructuredBinding
     * @brief Represents a structured binding in the reflection system.
     * @extends Mirror
     */
    class StructuredBinding: public Mirror {
    public:
        consteval StructuredBinding() noexcept = default;

        [[=Throws<ReflectiveOperationException>()]]
        consteval explicit StructuredBinding(Info i):
            Mirror(i) {
            if (i != Info{} && !reflect::is_structured_binding(i)) {
                throw ReflectiveOperationException("Provided Info is not a structured binding", ^^StructuredBinding);
            }
        }

        [[nodiscard]]
        consteval Type type() const {
            return Type(reflect::type_of(_info));
        }
    };

    /**
     * @class Class
     * @brief Statically-typed wrapper for class (non-union) types.
     * @extends Type
     * @tparam T The class type being reflected.
     */
    template <ReflectableAsClass T>
    class Class: public Type {
    public:
        using Of = T;
        static constexpr Info VALUE = ^^T;

        consteval Class() noexcept:
            Type(^^T) {}

        [[nodiscard]]
        consteval Vector<Field> fields(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Field> result;
            for (Info m : reflect::nonstatic_data_members_of(^^T, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Variable> static_fields(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Variable> result;
            for (Info m : reflect::static_data_members_of(^^T, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Method> methods(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Method> result;
            for (Info m : reflect::members_of(^^T, ctx)) {
                if (reflect::is_function(m)
                    && reflect::is_class_member(m)
                    && !reflect::is_constructor(m)
                    && !reflect::is_destructor(m))
                {
                    result.emplace_back(m);
                }
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Constructor> constructors(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Constructor> result;
            for (Info m : reflect::members_of(^^T, ctx)) {
                if (reflect::is_constructor(m)) {
                    result.emplace_back(m);
                }
            }
            return result;
        }

        [[nodiscard]]
        consteval Destructor destructor(AccessContext ctx = AccessContext::unchecked()) const {
            for (Info m : reflect::members_of(^^T, ctx)) {
                if (reflect::is_destructor(m)) {
                    return Destructor(m);
                }
            }
            return Destructor{};
        }

        [[nodiscard]]
        consteval Vector<Base> bases(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Base> result;
            for (Info b : reflect::bases_of(^^T, ctx)) {
                result.emplace_back(b);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Mirror> subobjects(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Mirror> result;
            for (Info s : reflect::subobjects_of(^^T, ctx)) {
                result.emplace_back(s);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Mirror> members(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Mirror> result;
            for (Info m : reflect::members_of(^^T, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }

        [[nodiscard]]
        consteval bool has_inaccessible_bases(AccessContext ctx = AccessContext::unchecked()) const {
            return reflect::has_inaccessible_bases(^^T, ctx);
        }

        [[nodiscard]]
        consteval bool has_inaccessible_nonstatic_data_members(AccessContext ctx = AccessContext::unchecked()) const {
            return reflect::has_inaccessible_nonstatic_data_members(^^T, ctx);
        }

        [[nodiscard]]
        consteval bool has_inaccessible_subobjects(AccessContext ctx = AccessContext::unchecked()) const {
            return reflect::has_inaccessible_subobjects(^^T, ctx);
        }
    };

    /**
     * @class Enum
     * @brief Represents an enum in the reflection system.
     * @extends Type
     * @tparam E The enum type being reflected.
     */
    template <ReflectableAsEnum E>
    class Enum: public Type {
    public:
        using Of = E;
        static constexpr Info VALUE = ^^E;

        consteval Enum() noexcept:
            Type(^^E) {}

        [[nodiscard]]
        consteval Vector<Enumerator> enumerators() const {
            Vector<Enumerator> result;
            for (Info e : reflect::enumerators_of(^^E)) {
                result.emplace_back(e);
            }
            return result;
        }

        [[nodiscard]]
        consteval Type underlying() const {
            return Type(reflect::underlying_type(^^E));
        }

        [[nodiscard]]
        consteval bool is_scoped() const {
            return reflect::is_scoped_enum_type(^^E);
        }
    };

    /**
     * @class Union
     * @brief Represents a union in the reflection system.
     * @extends Type
     * @tparam U The union type being reflected.
     */
    template <ReflectableAsUnion U>
    class Union: public Type {
    public:
        using Of = U;
        static constexpr Info VALUE = ^^U;

        consteval Union() noexcept:
            Type(^^U) {}

        [[nodiscard]]
        consteval Vector<Field> fields(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Field> result;
            for (Info m : reflect::nonstatic_data_members_of(^^U, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }

        [[nodiscard]]
        consteval Vector<Mirror> members(AccessContext ctx = AccessContext::unchecked()) const {
            Vector<Mirror> result;
            for (Info m : reflect::members_of(^^U, ctx)) {
                result.emplace_back(m);
            }
            return result;
        }
    };

    consteval Vector<Annotation> Mirror::annotations() const {
        Vector<Annotation> result;
        for (Info a : reflect::annotations_of(_info)) {
            result.emplace_back(a);
        }
        return result;
    }

    template <typename T>
    consteval Vector<Annotation> Mirror::annotations_with_type() const {
        Vector<Annotation> result;
        for (Info a : reflect::annotations_of_with_type(_info, ^^T)) {
            result.emplace_back(a);
        }
        return result;
    }

    consteval Template Type::template_of() const {
        return Template(reflect::template_of(_info));
    }

    consteval Optional<Type> Mirror::enclosing_class() const {
        Info cur = _info;
        while (reflect::has_parent(cur)) {
            cur = reflect::parent_of(cur);
            if (reflect::is_class_type(cur) && !reflect::is_union_type(cur)) {
                return Type(cur);
            }
        }
        return nullopt;
    }

    consteval Namespace Mirror::enclosing_namespace() const {
        if (reflect::is_namespace(_info) && !reflect::has_parent(_info)) {
            return Namespace(_info);
        }
        Info cur = _info;
        while (reflect::has_parent(cur)) {
            cur = reflect::parent_of(cur);
            if (reflect::is_namespace(cur)) {
                return Namespace(cur);
            }
        }
        return Namespace(_info);
    }
    #endif
}
