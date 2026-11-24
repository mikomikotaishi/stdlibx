#pragma once

using stdx::core::SameAs;
using stdx::meta::FixedString;
using stdx::meta::RemoveConstVolatileReferenceType;

namespace stdx::data {
    /**
     * @internal
     * @enum DataFormat
     * @brief Discriminator distinguishing one data format's annotations from another's.
     */
    enum class DataFormat {
        JSON, ///< The JSON data format.
        XML, ///< The XML data format.
        TOML, ///< The TOML data format.
        YAML, ///< The YAML data format.
        CSV, ///< The CSV data format.
    };
}

/**
 * @namespace stdx::data
 * @brief Data format parsing and serialization/deserialization.
 */
export namespace stdx::data {
    /**
     * @struct PropertyNameAnnotation
     * @brief Field annotation overriding the data format key used for a member.
     * @tparam Format The data format this annotation applies to.
     * @tparam Name The key to read and write instead of the member's own name.
     *
     * Applied to a struct member, it makes a given format's serializer emit, and
     * its deserializer read, @p Name in place of the C++ identifier. The
     * @p Format tag means one format never honors another's rename.
     */
    template <DataFormat Format, FixedString Name>
    struct PropertyNameAnnotation {
        static constexpr DataFormat FORMAT = Format; ///< The owning format.
        static constexpr FixedString NAME = Name; ///< The overriding key.

        /**
         * @brief The overriding key.
         * @return The key as a view.
         */
        [[nodiscard]]
        static constexpr StringView str() noexcept {
            return NAME.view();
        }
    };

    /**
     * @struct IgnoreAnnotation
     * @brief Field annotation excluding a member from a format's serialization.
     * @tparam Format The data format this annotation applies to.
     *
     * The @p Format tag is the annotation's only state; it exists solely so that
     * one format's Ignore is a different type from another's, which is what lets
     * serde match its own and ignore the rest.
     */
    template <DataFormat Format>
    struct IgnoreAnnotation {
        static constexpr DataFormat FORMAT = Format; ///< The owning format.
    };

    /**
     * @struct TransientAnnotation
     * @brief Field annotation excluding a member from *every* format's serialization.
     *
     * The format-agnostic counterpart to @ref IgnoreAnnotation: where a tagged
     * Ignore excludes a member from one format only, Transient excludes it from
     * all of them.
     */
    struct TransientAnnotation {};

    /**
     * @brief The {@code [[=Transient]]} annotation.
     *
     * Marks a member as never serialized, in any format. For a format-specific
     * exclusion, use that format's Ignore (e.g. {@code [[=JsonIgnore]]}) instead.
     */
    inline constexpr TransientAnnotation Transient = {};

    /**
     * @struct FlattenAnnotation
     * @brief Field annotation inlining a nested struct's members into the
     * parent, in *every* format.
     *
     * The flattened member contributes no key of its own: its fields are
     * written into the enclosing object (table, row) under their own keys and
     * read back from it the same way, recursively, with their own renames,
     * exclusions and flattens applied. Only a struct or class member can be
     * flattened (not a string, container or Optional), and the flattened keys
     * must not collide with the parent's - both are compile errors.
     */
    struct FlattenAnnotation {};

    /**
     * @brief The {@code [[=Flatten]]} annotation.
     *
     * Flattens a member in every format. To flatten in one format only (say,
     * CSV, which cannot nest, while JSON keeps the nesting), use that format's
     * Flatten (e.g. {@code [[=CsvFlatten]]}) instead.
     */
    inline constexpr FlattenAnnotation Flatten = {};

    /**
     * @struct FormatFlattenAnnotation
     * @brief Field annotation flattening a member in one format only.
     * @tparam Format The data format this annotation applies to.
     *
     * The per-format counterpart to @ref FlattenAnnotation, as
     * @ref IgnoreAnnotation is to @ref TransientAnnotation.
     */
    template <DataFormat Format>
    struct FormatFlattenAnnotation {
        static constexpr DataFormat FORMAT = Format; ///< The owning format.
    };

    #ifdef __cpp_impl_reflection
    /**
     * @struct RenameAnnotation
     * @brief Field annotation overriding a member's key in *every* format.
     *
     * The format-agnostic counterpart to @ref PropertyNameAnnotation. Where both
     * apply, the format's own PropertyName wins, so a member can carry one name
     * everywhere and a different one in a single format. The string is copied
     * into static storage, so the annotation stays a structural value.
     */
    struct RenameAnnotation {
        const char* name; ///< The overriding key, in static storage.

        consteval explicit RenameAnnotation(StringView key):
            name{Ops::static_string(key)} {}

        /**
         * @brief The overriding key.
         * @return The key as a view.
         */
        [[nodiscard]]
        constexpr StringView str() const noexcept {
            return name;
        }
    };

    /**
     * @brief The {@code [[=Rename("...")]]} annotation.
     * @param key The key to read and write, in every format, instead of the
     * member's own name. Must not be empty (a compile error at the use site).
     */
    [[nodiscard]]
    consteval RenameAnnotation Rename(StringView key) {
        return RenameAnnotation{key};
    }
    #endif

    /**
     * @struct ReflectableAnnotation
     * @brief Class annotation letting serialization see a class's private and
     * protected data members.
     *
     * By default only public data members are serialized, and a class with
     * non-public data members is rejected at compile time rather than having
     * them silently skipped - skipping would lose data on a round trip, and
     * reading them unasked would leak or overwrite state the class keeps
     * behind its interface. Marking the class is the owner's opt-in: its
     * non-public members are then read and written like public ones, in every
     * format. Annotations, renames and exclusions on those members apply as
     * usual.
     */
    struct ReflectableAnnotation {};

    /**
     * @brief The {@code [[=Reflectable]]} class annotation:
     * {@code struct [[=Reflectable]] Account { ... };}
     *
     * For a type you cannot annotate, pass @ref AllowPrivate to the
     * serialize/deserialize call instead.
     */
    inline constexpr ReflectableAnnotation Reflectable = {};

    /**
     * @struct AllowPrivateTag
     * @brief Tag type for @ref AllowPrivate.
     *
     * Its default constructor is explicit so that a stray {} argument cannot
     * turn on private access by accident.
     */
    struct AllowPrivateTag {
        explicit constexpr AllowPrivateTag() noexcept = default;
    };

    /**
     * @brief Force serialization to read and write private and protected data
     * members, for types you do not own and so cannot mark
     * {@code [[=Reflectable]]}.
     *
     * Pass it as the extra argument, e.g. {@code Json::serialize(value,
     * AllowPrivate)} or {@code json.deserialize<T>(AllowPrivate)}. It applies
     * to the whole call - every nested and flattened type too - since a type
     * you do not own usually contains others you do not own either. It reaches
     * past the class's interface: values go in without its setters or
     * invariant checks, so use it on types whose layout you know.
     */
    inline constexpr AllowPrivateTag AllowPrivate{};
}

namespace stdx::data {
    /**
     * @internal
     * @brief Whether T is a @ref PropertyNameAnnotation for the given format.
     *
     * A dedicated trait rather than stdx::meta::SpecializationOf because the
     * annotation carries non-type (DataFormat, FixedString) arguments, which the
     * type-only SpecializationOf cannot match; and because matching must pin the
     * format, not merely the template.
     */
    template <typename T, DataFormat Format>
    constexpr bool IsPropertyNameAnnotationValue = false;

    template <DataFormat Format, FixedString Name>
    constexpr bool IsPropertyNameAnnotationValue<PropertyNameAnnotation<Format, Name>, Format> = true;

    /**
     * @concept IsPropertyNameAnnotation
     * @brief Satisfied only by a @ref PropertyNameAnnotation whose format is @p Format.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format that must match.
     */
    template <typename T, DataFormat Format>
    concept IsPropertyNameAnnotation = IsPropertyNameAnnotationValue<RemoveConstVolatileReferenceType<T>, Format>;

    /**
     * @internal
     * @brief Whether T is the @ref IgnoreAnnotation for the given format.
     */
    template <typename T, DataFormat Format>
    constexpr bool IsIgnoreAnnotationValue = false;

    template <DataFormat Format>
    constexpr bool IsIgnoreAnnotationValue<IgnoreAnnotation<Format>, Format> = true;

    /**
     * @concept IsIgnoreAnnotation
     * @brief Satisfied only by the @ref IgnoreAnnotation whose format is @p Format.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format that must match.
     */
    template <typename T, DataFormat Format>
    concept IsIgnoreAnnotation = IsIgnoreAnnotationValue<RemoveConstVolatileReferenceType<T>, Format>;

    /**
     * @concept IsTransientAnnotation
     * @brief Satisfied only by the format-agnostic @ref TransientAnnotation.
     * @tparam T The (cvref-stripped) annotation type to test.
     *
     * Unlike @ref IsIgnoreAnnotation this takes no format: a Transient member is
     * excluded everywhere, so every format matches it the same way.
     */
    template <typename T>
    concept IsTransientAnnotation = SameAs<RemoveConstVolatileReferenceType<T>, TransientAnnotation>;

    /**
     * @concept ExcludedFrom
     * @brief Satisfied when annotation type T excludes a member from @p Format.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format doing the asking.
     *
     * Composes the two orthogonal exclusion scopes so a format's serde has a
     * single rule to apply per annotation: the universal @ref TransientAnnotation
     * (which excludes a member from every format) or that format's own
     * @ref IgnoreAnnotation (which excludes it from that format alone). A
     * different format's Ignore satisfies neither, so it does not exclude here.
     * Each sub-library's field walk tests this against every annotation; the
     * policy lives here once rather than being re-spelled per format.
     */
    template <typename T, DataFormat Format>
    concept ExcludedFrom = IsTransientAnnotation<T> || IsIgnoreAnnotation<T, Format>;

    /**
     * @internal
     * @brief Whether T is the @ref FormatFlattenAnnotation for the given format.
     */
    template <typename T, DataFormat Format>
    constexpr bool IsFormatFlattenAnnotationValue = false;

    template <DataFormat Format>
    constexpr bool IsFormatFlattenAnnotationValue<FormatFlattenAnnotation<Format>, Format> = true;

    /**
     * @concept FlattenedIn
     * @brief Satisfied when annotation type T flattens a member in @p Format:
     * the universal @ref FlattenAnnotation, or that format's own
     * @ref FormatFlattenAnnotation. Another format's Flatten does not count.
     * @tparam T The (cvref-stripped) annotation type to test.
     * @tparam Format The format doing the asking.
     */
    template <typename T, DataFormat Format>
    concept FlattenedIn = SameAs<RemoveConstVolatileReferenceType<T>, FlattenAnnotation>
        || IsFormatFlattenAnnotationValue<RemoveConstVolatileReferenceType<T>, Format>;

    #ifdef __cpp_impl_reflection
    /**
     * @concept IsRenameAnnotation
     * @brief Satisfied only by the format-agnostic @ref RenameAnnotation.
     * @tparam T The (cvref-stripped) annotation type to test.
     */
    template <typename T>
    concept IsRenameAnnotation = SameAs<RemoveConstVolatileReferenceType<T>, RenameAnnotation>;

    /**
     * @internal
     * @brief Whether @p field is excluded from @p Format (see @ref ExcludedFrom).
     */
    template <DataFormat Format, stdx::meta::reflect::Field field>
    [[nodiscard]]
    consteval bool is_excluded() {
        bool excluded = false;
        template for (constexpr stdx::meta::reflect::Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (ExcludedFrom<Ann, Format>) {
                excluded = true;
            }
        }
        return excluded;
    }

    /**
     * @internal
     * @brief Whether @p field is flattened in @p Format (see @ref FlattenedIn).
     */
    template <DataFormat Format, stdx::meta::reflect::Field field>
    [[nodiscard]]
    consteval bool is_flattened() {
        bool flattened = false;
        template for (constexpr stdx::meta::reflect::Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (FlattenedIn<Ann, Format>) {
                flattened = true;
            }
        }
        return flattened;
    }

    /**
     * @internal
     * @brief Whether @p field renames itself in @p Format, by either the
     * format's PropertyName or the universal Rename.
     */
    template <DataFormat Format, stdx::meta::reflect::Field field>
    [[nodiscard]]
    consteval bool is_renamed() {
        bool renamed = false;
        template for (constexpr stdx::meta::reflect::Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsPropertyNameAnnotation<Ann, Format> || IsRenameAnnotation<Ann>) {
                renamed = true;
            }
        }
        return renamed;
    }

    /**
     * @internal
     * @brief The key @p field is read and written under in @p Format.
     *
     * The format's own PropertyName wins over the universal Rename, which wins
     * over the member's identifier; among several of the same kind, the last
     * one written wins.
     */
    template <DataFormat Format, stdx::meta::reflect::Field field>
    [[nodiscard]]
    consteval StringView key_of() {
        Optional<StringView> specific;
        Optional<StringView> universal;
        template for (constexpr stdx::meta::reflect::Annotation ann: Ops::static_array(field.annotations())) {
            using Ann = typename [:ann.type().value():];
            if constexpr (IsPropertyNameAnnotation<Ann, Format>) {
                constexpr Ann instance = ann.template as<Ann>();
                specific = instance.str();
            } else if constexpr (IsRenameAnnotation<Ann>) {
                constexpr Ann instance = ann.template as<Ann>();
                universal = instance.str();
            }
        }
        if (specific.has_value()) {
            return *specific;
        }
        if (universal.has_value()) {
            return *universal;
        }
        return field.name().value();
    }

    /**
     * @internal
     * @brief Whether class T carries {@code [[=Reflectable]]}.
     */
    template <typename T>
    [[nodiscard]]
    consteval bool is_reflectable_marked() {
        return !stdx::meta::reflect::Class<T>().template annotations_with_type<ReflectableAnnotation>().empty();
    }

    /**
     * @internal
     * @brief Why T's data members may not be walked under the given access -
     * it has non-public ones and neither opted in nor was forced - or an empty
     * view if they may.
     */
    template <typename T, bool Forced>
    [[nodiscard]]
    consteval StringView access_error() {
        using stdx::meta::reflect::AccessContext;
        if (Forced || is_reflectable_marked<T>()) {
            return {};
        }
        const usize all = stdx::meta::reflect::Class<T>().fields(AccessContext::unchecked()).size();
        const usize visible = stdx::meta::reflect::Class<T>().fields(AccessContext::unprivileged()).size();
        if (all == visible) {
            return {};
        }
        String message = "'";
        message += std::meta::display_string_of(^^T);
        message += "' has private or protected data members: mark it [[=Reflectable]], "
            "or pass AllowPrivate to read them anyway";
        return Ops::static_string(message);
    }

    /**
     * @internal
     * @brief The access context to walk T's data members with: every member
     * when forced (@ref AllowPrivate) or when T is {@code [[=Reflectable]]},
     * else public members only.
     *
     * Every serializer's member walk goes through this, so the rule - and the
     * compile-time rejection of a class whose non-public members would
     * otherwise be silently dropped - lives in one place.
     */
    template <typename T, bool Forced>
    [[nodiscard]]
    consteval stdx::meta::reflect::AccessContext fields_access() {
        constexpr StringView problem = access_error<T, Forced>();
        static_assert(problem.empty(), problem);
        if constexpr (Forced) {
            return stdx::meta::reflect::AccessContext::unchecked();
        } else if constexpr (is_reflectable_marked<T>()) {
            return stdx::meta::reflect::AccessContext::unchecked();
        } else {
            return stdx::meta::reflect::AccessContext::unprivileged();
        }
    }

    /**
     * @internal
     * @brief Whether a member of type T can be flattened: a class type that is
     * not a container, an Optional, or a string.
     *
     * A class is not required to be an aggregate: whether its members may be
     * walked at all is the access rule's call (fields_access), which rejects
     * a class with non-public members unless it is Reflectable or the call is
     * forced with AllowPrivate.
     */
    template <typename T>
    concept Flattenable = stdx::meta::reflect::ReflectableAsClass<T>
        && !stdx::meta::IsOptional<T>
        && !stdx::meta::SpecializationOf<T, std::vector>
        && !SameAs<T, String>;

    /**
     * @internal
     * @brief Append every key T contributes in @p Format, flattened members
     * expanded in place, to @p out - over the same members, under the same
     * access, as the serializer itself will walk.
     */
    template <DataFormat Format, typename T, bool Forced>
    consteval void collect_keys(Vector<StringView>& out) {
        constexpr stdx::meta::reflect::AccessContext ctx = fields_access<T, Forced>();
        template for (constexpr stdx::meta::reflect::Field field: Ops::static_array(stdx::meta::reflect::Class<T>().fields(ctx))) {
            if constexpr (!is_excluded<Format, field>()) {
                if constexpr (is_flattened<Format, field>()) {
                    using Mem = RemoveConstVolatileReferenceType<typename [:field.type().value():]>;
                    static_assert(Flattenable<Mem>, "only a struct or class member can be flattened");
                    static_assert(!is_renamed<Format, field>(), "a flattened member has no key of its own to rename");
                    collect_keys<Format, Mem, Forced>(out);
                } else {
                    out.push_back(key_of<Format, field>());
                }
            }
        }
    }

    /**
     * @internal
     * @brief Why T's keys are invalid in @p Format - an empty key or two equal
     * keys, flattened members included - or an empty view if they are valid.
     *
     * Returned as a static string so it can be a static_assert's message:
     * the mistake shows up at compile time, naming the key, rather than as one
     * value silently overwriting another.
     */
    template <DataFormat Format, typename T, bool Forced>
    [[nodiscard]]
    consteval StringView key_error() {
        Vector<StringView> keys;
        collect_keys<Format, T, Forced>(keys);
        for (usize i = 0; i < keys.size(); ++i) {
            if (keys[i].empty()) {
                return "a serialized key must not be empty";
            }
            for (usize j = i + 1; j < keys.size(); ++j) {
                if (keys[i] == keys[j]) {
                    String message = "duplicate serialized key '";
                    message += keys[i];
                    message += "'";
                    return Ops::static_string(message);
                }
            }
        }
        return {};
    }
    #endif
}
