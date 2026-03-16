#pragma once

using stdx::ranges::InputRange;
using stdx::ranges::RangeValue;

/**
 * @namespace stdx::meta::reflect
 * @brief Wrapper namespace for standard library reflection operations.
 */
export namespace stdx::meta::reflect {
    #ifdef __cpp_lib_reflection
    using std::is_string_literal;
    using std::define_static_string;
    using std::define_static_array;
    using std::define_static_object;

    using Info = std::meta::info;
    using ReflectionException = std::meta::exception;

    class [[nodiscard]] Operators final {
    public:
        using Self = std::meta::operators;

        static constexpr Self NEW = std::meta::operators::op_new;
        static constexpr Self DELETE = std::meta::operators::op_delete;
        static constexpr Self ARRAY_NEW = std::meta::operators::op_array_new;
        static constexpr Self ARRAY_DELETE = std::meta::operators::op_array_delete;
        static constexpr Self CO_AWAIT = std::meta::operators::op_co_await;
        static constexpr Self PARENTHESES = std::meta::operators::op_parenthesis;
        static constexpr Self ARROW = std::meta::operators::op_arrow;
        static constexpr Self ARROW_STAR = std::meta::operators::op_arrow_star;
        static constexpr Self TILDE = std::meta::operators::op_tilde;
        static constexpr Self EXCLAMATION = std::meta::operators::op_exclamation;
        static constexpr Self PLUS = std::meta::operators::op_plus;
        static constexpr Self MINUS = std::meta::operators::op_minus;
        static constexpr Self STAR = std::meta::operators::op_star;
        static constexpr Self SLASH = std::meta::operators::op_slash;
        static constexpr Self PERCENT = std::meta::operators::op_percent;
        static constexpr Self CARET = std::meta::operators::op_caret;
        static constexpr Self AMPERSAND = std::meta::operators::op_ampersand;
        static constexpr Self EQUALS = std::meta::operators::op_equals;
        static constexpr Self PIPE = std::meta::operators::op_pipe;
        static constexpr Self PLUS_EQUALS = std::meta::operators::op_plus_equals;
        static constexpr Self MINUS_EQUALS = std::meta::operators::op_minus_equals;
        static constexpr Self STAR_EQUALS = std::meta::operators::op_star_equals;
        static constexpr Self SLASH_EQUALS = std::meta::operators::op_slash_equals; 
        static constexpr Self PERCENT_EQUALS = std::meta::operators::op_percent_equals;
        static constexpr Self CARET_EQUALS = std::meta::operators::op_caret_equals;
        static constexpr Self AMPERSAND_EQUALS = std::meta::operators::op_ampersand
        static constexpr Self PIPE_EQUALS = std::meta::operators::op_pipe_equals;
        static constexpr Self EQUALS_EQUALS = std::meta::operators::op_equals_equals;
        static constexpr Self EXCLAMATION_EQUALS = std::meta::operators::op_exclamation_equals;
        static constexpr Self LESS = std::meta::operators::op_less;
        static constexpr Self GREATER = std::meta::operators::op_greater;
        static constexpr Self LESS_EQUALS = std::meta::operators::op_less_equals;
        static constexpr Self GREATER_EQUALS = std::meta::operators::op_greater_equals;
        static constexpr Self SPACESHIP = std::meta::operators::op_spaceship;
        static constexpr Self AMPERSAND_AMPERSAND = std::meta::operators::op_ampersand_ampersand;
        static constexpr Self PIPE_PIPE = std::meta::operators::op_pipe_pipe;
        static constexpr Self LESS_LESS = std::meta::operators::op_less_less;
        static constexpr Self GREATER_GREATER = std::meta::operators::op_greater_g
        static constexpr Self LESS_LESS_EQUALS = std::meta::operators::op_less_less_equals;
        static constexpr Self GREATER_GREATER_EQUALS = std::meta::operators::op_greater
        static constexpr Self PLUS_PLUS = std::meta::operators::op_plus_plus;
        static constexpr Self MINUS_MINUS = std::meta::operators::op_minus_minus;
        static constexpr Self COMMA = std::meta::operators::op_comma;
    private:
        Self value;
    public:
        constexpr Operators(Self value) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using std::meta::operator_of;
    using std::meta::symbol_of;
    using std::meta::u8symbol_of;
    using std::meta::has_identifier;
    using std::meta::identifier_of;
    using std::meta::u8identifier_of;
    using std::meta::display_string_of;
    using std::meta::u8display_string_of;
    using std::meta::source_location_of;

    using std::meta::type_of;
    using std::meta::object_of;
    using std::meta::constant_of;

    using std::meta::is_public;
    using std::meta::is_protected;
    using std::meta::is_private;

    using std::meta::is_virtual;
    using std::meta::is_pure_virtual;
    using std::meta::is_override;
    using std::meta::is_final;

    using std::meta::is_deleted;
    using std::meta::is_defaulted;
    using std::meta::is_user_provided;
    using std::meta::is_user_declared;
    using std::meta::is_explicit;
    using std::meta::is_noexcept;

    using std::meta::is_bit_field;
    using std::meta::is_enumerator;
    using std::meta::is_annotation;

    using std::meta::is_const;
    using std::meta::is_volatile;
    using std::meta::is_mutable_member;
    using std::meta::is_lvalue_reference_qualified;
    using std::meta::is_rvalue_reference_qualified;

    using std::meta::has_static_storage_duration;
    using std::meta::has_thread_storage_duration;
    using std::meta::has_automatic_storage_duration;

    using std::meta::has_internal_linkage;
    using std::meta::has_module_linkage;
    using std::meta::has_external_linkage;
    using std::meta::has_c_language_linkage;
    using std::meta::has_linkage;

    using std::meta::is_complete_type;
    using std::meta::is_enumerable_type;

    using std::meta::is_variable;
    using std::meta::is_type;
    using std::meta::is_namespace;
    using std::meta::is_type_alias;
    using std::meta::is_namespace_alias;

    using std::meta::is_function;
    using std::meta::is_conversion_function;
    using std::meta::is_operator_function;
    using std::meta::is_literal_operator;
    using std::meta::is_special_member_function;
    using std::meta::is_constructor;
    using std::meta::is_default_constructor;
    using std::meta::is_copy_constructor;
    using std::meta::is_move_constructor;
    using std::meta::is_assignment;
    using std::meta::is_copy_assignment;
    using std::meta::is_move_assignment;
    using std::meta::is_destructor;

    using std::meta::is_function_parameter;
    using std::meta::is_explicit_object_parameter;
    using std::meta::has_default_argument;
    using std::meta::has_ellipsis_parameter;

    using std::meta::is_template;
    using std::meta::is_function_template;
    using std::meta::is_variable_template;
    using std::meta::is_class_template;
    using std::meta::is_alias_template;
    using std::meta::is_conversion_function_template;
    using std::meta::is_operator_function_template;
    using std::meta::is_literal_operator_template;
    using std::meta::is_constructor_template;
    using std::meta::is_concept;

    using std::meta::is_value;
    using std::meta::is_object;

    using std::meta::is_structured_binding;

    using std::meta::is_class_member;
    using std::meta::is_namespace_member;
    using std::meta::is_nonstatic_data_member;
    using std::meta::is_static_member;
    using std::meta::is_base;

    using std::meta::has_default_member_initializer;
    
    using std::meta::has_parent;
    using std::meta::parent_of;

    using std::meta::dealias;
    
    using std::meta::has_template_arguments;
    using std::meta::template_of;
    using std::meta::template_arguments_of;
    using std::meta::parameters_of;
    using std::meta::variable_of;
    using std::meta::return_type_of;

    using AccessContext = std::meta::access_context;

    using std::meta::is_accessible;
    using std::meta::has_inaccessible_nonstatic_data_members;
    using std::meta::has_inaccessible_bases;
    using std::meta::has_inaccessible_subobjects;

    using std::meta::members_of;
    using std::meta::bases_of;
    using std::meta::static_data_members_of;
    using std::meta::nonstatic_data_members_of;
    using std::meta::subobjects_of;
    using std::meta::enumerators_of;

    using MemberOffset = std::meta::member_offset;

    using std::meta::offset_of;
    using std::meta::size_of;
    using std::meta::alignment_of;
    using std::meta::bit_size_of;
    
    using std::meta::extract;

    template <typename Range>
    concept ReflectionRange = std::meta::reflection_range<Range>;

    using std::meta::can_substitute;
    using std::meta::substitute;

    using std::meta::reflect_constant;
    using std::meta::reflect_object;
    using std::meta::reflect_function;
    
    using std::meta::reflect_constant_string;
    using std::meta::reflect_constant_array;

    using DataMemberOptions = std::meta::data_member_options;

    using std::meta::data_member_spec;
    using std::meta::is_data_member_spec;
    using std::meta::define_aggregate;

    using std::meta::is_void_type;
    using std::meta::is_null_pointer_type;
    using std::meta::is_integral_type;
    using std::meta::is_floating_point_type;
    using std::meta::is_array_type;
    using std::meta::is_pointer_type;
    using std::meta::is_lvalue_reference_type;
    using std::meta::is_rvalue_reference_type;
    using std::meta::is_member_object_pointer_type;
    using std::meta::is_member_function_pointer_type;
    using std::meta::is_enum_type;
    using std::meta::is_union_type;
    using std::meta::is_class_type;
    using std::meta::is_function_type;
    using std::meta::is_reflection_type;

    using std::meta::is_reference_type;
    using std::meta::is_arithmetic_type;
    using std::meta::is_fundamental_type;
    using std::meta::is_object_type;
    using std::meta::is_scalar_type;
    using std::meta::is_compound_type;
    using std::meta::is_member_pointer_type;

    using std::meta::is_const_type;
    using std::meta::is_volatile_type;
    using std::meta::is_trivially_copyable_type;
    using std::meta::is_standard_layout_type;
    using std::meta::is_empty_type;
    using std::meta::is_polymorphic_type;
    using std::meta::is_abstract_type;
    using std::meta::is_final_type;
    using std::meta::is_aggregate_type;
    using std::meta::is_consteval_only_type;
    using std::meta::is_signed_type;
    using std::meta::is_unsigned_type;
    using std::meta::is_bounded_array_type;
    using std::meta::is_unbounded_array_type;
    using std::meta::is_scoped_enum_type;

    using std::meta::is_constructible_type;
    using std::meta::is_default_constructible_type;
    using std::meta::is_copy_constructible_type;
    using std::meta::is_move_constructible_type;

    using std::meta::is_assignable_type;
    using std::meta::is_copy_assignable_type;
    using std::meta::is_move_assignable_type;

    using std::meta::is_swappable_with_type;
    using std::meta::is_swappable_type;

    using std::meta::is_destructible_type;

    using std::meta::is_trivially_constructible_type;
    using std::meta::is_trivially_default_constructible_type;
    using std::meta::is_trivially_copy_constructible_type;
    using std::meta::is_trivially_move_constructible_type;

    using std::meta::is_trivially_assignable_type;
    using std::meta::is_trivially_copy_assignable_type;
    using std::meta::is_trivially_move_assignable_type;
    using std::meta::is_trivially_destructible_type;

    using std::meta::is_nothrow_constructible_type;
    using std::meta::is_nothrow_default_constructible_type;
    using std::meta::is_nothrow_copy_constructible_type;
    using std::meta::is_nothrow_move_constructible_type;

    using std::meta::is_nothrow_assignable_type;
    using std::meta::is_nothrow_copy_assignable_type;
    using std::meta::is_nothrow_move_assignable_type;

    using std::meta::is_nothrow_swappable_with_type;
    using std::meta::is_nothrow_swappable_type;

    using std::meta::is_nothrow_destructible_type;

    using std::meta::is_implicit_lifetime_type;

    using std::meta::has_virtual_destructor;

    using std::meta::has_unique_object_representations;

    using std::meta::reference_constructs_from_temporary;
    using std::meta::reference_converts_from_temporary;

    using std::meta::rank;
    using std::meta::extent;

    using std::meta::is_same_type;
    using std::meta::is_base_of_type;
    using std::meta::is_virtual_base_of_type;
    using std::meta::is_convertible_type;
    using std::meta::is_nothrow_convertible_type;
    using std::meta::is_layout_compatible_type;
    using std::meta::is_pointer_interconvertible_base_of_type;

    using std::meta::is_invocable_type;
    using std::meta::is_invocable_r_type;
    
    using std::meta::is_nothrow_invocable_type;
    using std::meta::is_nothrow_invocable_r_type;

    using std::meta::remove_const;
    using std::meta::remove_volatile;
    using std::meta::remove_cv;
    using std::meta::add_const;
    using std::meta::add_volatile;
    using std::meta::add_cv;

    using std::meta::remove_reference;
    using std::meta::add_lvalue_reference;
    using std::meta::add_rvalue_reference;

    using std::meta::make_signed;
    using std::meta::make_unsigned;

    using std::meta::remove_extent;
    using std::meta::remove_all_extents;

    using std::meta::remove_pointer;
    using std::meta::add_pointer;

    using std::meta::remove_cvref;
    using std::meta::decay;
    using std::meta::common_type;
    using std::meta::common_reference;
    using std::meta::underlying_type;
    using std::meta::invoke_result;
    using std::meta::unwrap_reference;
    using std::meta::unwrap_ref_decay;

    using std::meta::tuple_size;
    using std::meta::tuple_element;

    using std::meta::variant_size;
    using std::meta::variant_alternative;

    using std::meta::type_order;

    using std::meta::annotations_of;
    using std::meta::annotations_of_with_type
    #endif
}
