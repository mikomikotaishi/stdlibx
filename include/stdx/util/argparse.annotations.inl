#pragma once

#ifdef __cpp_lib_reflection
using stdx::collections::Vector;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Annotation;
using stdx::meta::reflect::Field;
using stdx::meta::reflect::ReflectableClass;

namespace stdx::util {
    /**
     * @brief Compile-time string usable as a non-type template parameter, so a
     * string literal can ride along in an annotation (e.g. Description<"...">).
     * @tparam N Length of the source literal, including its terminating NUL.
     */
    template <usize N>
    struct FixedString {
        char data[N]{};

        consteval FixedString(const char (&str)[N]) noexcept {
            for (usize i = 0; i < N; ++i) {
                data[i] = str[i];
            }
        }

        [[nodiscard]]
        consteval StringView view() const noexcept {
            return StringView{data, N - 1};
        }
    };

    template <typename T>
    constexpr bool IsOptionalValue = false;

    template <typename U>
    constexpr bool IsOptionalValue<Optional<U>> = true;

    /**
     * @concept IsOptional
     * @brief Satisfied by stdx::Optional specialisations. Such fields are left
     * empty (rather than required) when the argument is absent.
     */
    template <typename T>
    concept IsOptional = IsOptionalValue<T>;

    template <typename T>
    struct OptionalValueType {};

    template <typename U>
    struct OptionalValueType<Optional<U>> {
        using Value = U;
    };

    /**
     * @brief argv[1..] as a list of views. argv[0] (the program name) is dropped.
     */
    [[nodiscard]]
    Vector<StringView> collect_args(int argc, char* argv[]) {
        Vector<StringView> result;
        for (usize i = 1; i < argc; ++i) {
            result.emplace_back(argv[i]);
        }
        return result;
    }

    /**
     * @brief Index of an exact long-flag token (e.g. "--input") in the argument
     * list, or nullopt if it was not passed.
     */
    [[nodiscard]]
    Optional<usize> find_arg(const Vector<StringView>& args, StringView arg) noexcept {
        for (usize i = 0; i < args.size(); ++i) {
            if (args[i] == arg) {
                return i;
            }
        }
        return nullopt;
    }

    /**
     * @brief Index of the short-flag token carrying @p arg's letter, or nullopt.
     *
     * Matches any single-dash token (so combined flags such as "-abc" each count
     * as carrying 'a', 'b' and 'c'); double-dash tokens are skipped.
     * @param arg A two-character flag of the form "-x".
     */
    [[nodiscard]]
    Optional<usize> find_short_arg(const Vector<StringView>& args, StringView arg) noexcept {
        if (arg.size() != 2) {
            return nullopt;
        }
        const char letter = arg[1];
        for (usize i = 0; i < args.size(); ++i) {
            const StringView token = args[i];
            if (
                token.size() >= 2 && 
                token[0] == '-' &&
                token[1] != '-' &&
                token.find(letter) != StringView::npos
            ) {
                return i;
            }
        }
        return nullopt;
    }

    /**
     * @brief Turn a member identifier into its long flag, lowering camelCase and
     * snake_case to kebab-case and prefixing "--" (logLevel -> --log-level).
     */
    [[nodiscard]]
    String format_member_as_arg(StringView member_name) {
        String formatted;
        for (const char c: member_name) {
            if (c == '_') {
                formatted += '-';
            } else if (c >= 'A' && c <= 'Z') {
                if (!formatted.empty()) {
                    formatted += '-';
                }
                formatted += static_cast<char>(c + ('a' - 'A'));
            } else {
                formatted += c;
            }
        }
        return stdx::fmt::format("--{}", formatted);
    }

    template <typename T>
        requires IsSameValue<T, String>
    String convert_value(StringView value) {
        return String{value};
    }

    template <typename T>
        requires (Integral<T> && !IsSameValue<T, bool>)
    [[=Throws<InvalidArgumentException, InvalidRangeException>()]]
    T convert_value(StringView value) {
        return perform_from_chars<T, 10uz>(value);
    }

    template <IsOptional T>
    [[=Throws<InvalidArgumentException, InvalidRangeException>()]]
    typename OptionalValueType<T>::Value convert_value(StringView value) {
        return convert_value<typename OptionalValueType<T>::Value>(value);
    }

    template <typename T>
    constexpr StringView friendly_type_name() noexcept {
        return "value";
    }

    template <typename T>
        requires IsSameValue<T, String>
    constexpr StringView friendly_type_name() noexcept {
        return "string";
    }

    template <typename T>
        requires (Integral<T> && !IsSameValue<T, bool>)
    constexpr StringView friendly_type_name() noexcept {
        return "number";
    }

    template <typename T>
        requires IsSameValue<T, bool>
    constexpr StringView friendly_type_name() noexcept {
        return "bool";
    }

    template <IsOptional T>
    constexpr StringView friendly_type_name() noexcept {
        return friendly_type_name<typename OptionalValueType<T>::Value>();
    }
}

export namespace stdx::util {
    /**
     * @struct ShortName
     * @brief An annotation giving a field a single-character short flag.
     * @tparam C The flag letter, e.g. ShortName<'v'> -> -v.
     */
    template <char C>
    struct ShortName {
        static constexpr char letter = C;
    };

    /**
     * @struct TextAnnotation
     * @brief A base annotation carrying a compile-time string for the text annotations.
     * @tparam S The annotation text as a compile-time string.
     */
    template <FixedString S>
    struct TextAnnotation {
        [[nodiscard]]
        static consteval decltype(S) descriptor() noexcept {
            return S;
        }

        [[nodiscard]]
        static consteval StringView str() noexcept {
            return S.view();
        }
    };

    /**
     * @struct Description
     * @brief An annotation indicating human-readable help text
     * for a field, shown by help<T>().
     * @tparam S The description text as a compile-time string.
     */
    template <FixedString S>
    struct Description: TextAnnotation<S> {};

    /**
     * @struct Env
     * @brief An annotation to name an environment variable to consult when the
     * argument is not passed on the command line.
     * @tparam S The environment variable name as a compile-time string.
     */
    template <FixedString S>
    struct Env: TextAnnotation<S> {};

    /**
     * @concept IsShortName
     * @brief Satisfied by any ShortName specialisation.
     * @tparam T The annotation type to check.
     */
    template <typename T>
    concept IsShortName =
        requires { T::letter; }
        && IsSameValue<RemoveConstVolatileReferenceType<T>, ShortName<T::letter>>;

    /**
     * @concept IsDescription
     * @brief Satisfied by any Description specialisation.
     * @tparam T The annotation type to check.
     */
    template <typename T>
    concept IsDescription =
        requires { T::descriptor(); }
        && IsSameValue<RemoveConstVolatileReferenceType<T>, Description<T::descriptor()>>;

    /**
     * @concept IsEnv
     * @brief Satisfied by any Env specialisation.
     * @tparam T The annotation type to check.
     */
    template <typename T>
    concept IsEnv =
        requires { T::descriptor(); }
        && IsSameValue<RemoveConstVolatileReferenceType<T>, Env<T::descriptor()>>;

    template <ReflectableClass T>
    [[=Throws<CommandLineParserException, InvalidArgumentException, InvalidRangeException>()]]
    T ArgumentParser::parse(int argc, char* argv[]) {
        static_assert(
            Ops::class_of<T>().is_default_constructible(),
            "ArgumentParser::parse<T> requires T to be default-constructible"
        );

        constexpr AccessContext ctx = AccessContext::unchecked();
        const Vector<StringView> arguments = collect_args(argc, argv);

        T result{};

        template for (constexpr Field field: Ops::define_static_array(Ops::class_of<T>().fields(ctx))) {
            using MemberType = typename [:field.type().value():];

            Optional<String> short_flag;
            Optional<String> env_name;

            template for (constexpr Annotation ann: Ops::define_static_array(field.annotations())) {
                using AnnotationType = typename [:ann.type().value():];
                if constexpr (IsShortName<AnnotationType>) {
                    if (short_flag.has_value()) {
                        throw CommandLineParserException("an argument may have at most one ShortName");
                    }
                    short_flag = stdx::fmt::format("-{}", AnnotationType::letter);
                } else if constexpr (IsEnv<AnnotationType>) {
                    env_name = String{AnnotationType::str()};
                }
            }

            const String long_flag = format_member_as_arg(field.name().value());
            const Optional<usize> long_index = find_arg(arguments, long_flag);
            const Optional<usize> short_index =
                short_flag.has_value() ? find_short_arg(arguments, *short_flag) : nullopt;

            if (long_index.has_value() && short_index.has_value()) {
                throw CommandLineParserException(
                    stdx::fmt::format("cannot pass both {} and {}", long_flag, *short_flag)
                );
            }

            if constexpr (IsSameValue<MemberType, bool>) {
                result.[:field.value():] = long_index.has_value() || short_index.has_value();
            } else {
                constexpr bool optional_field = IsOptional<MemberType>;
                constexpr bool defaulted_field = field.has_default_initializer();
                static_assert(
                    !(optional_field && defaulted_field),
                    "an argument cannot be both Optional<...> and carry a default initializer"
                );

                const Optional<usize> index = long_index.has_value() ? long_index : short_index;

                if (index.has_value()) {
                    if (*index + 1 >= arguments.size()) {
                        throw CommandLineParserException(
                            stdx::fmt::format("missing value for {}", long_flag)
                        );
                    }
                    result.[:field.value():] = convert_value<MemberType>(arguments[*index + 1]);
                } else if (env_name.has_value()) {
                    if (Optional<StringView> env_value = Environment::get(*env_name); env_value.has_value()) {
                        result.[:field.value():] = convert_value<MemberType>(*env_value);
                    } else if (!optional_field && !defaulted_field) {
                        throw CommandLineParserException(stdx::fmt::format(
                            "missing {} (environment variable {} is unset)", long_flag, *env_name
                        ));
                    }
                } else if (!optional_field && !defaulted_field) {
                    throw CommandLineParserException(
                        stdx::fmt::format("missing required argument {}", long_flag)
                    );
                }
                // Optional / defaulted and absent: keep the default-constructed value.
            }
        }

        return result;
    }

    template <ReflectableClass T>
    String ArgumentParser::help(int argc, char* argv[]) {
        static_assert(
            Ops::class_of<T>().is_default_constructible(),
            "ArgumentParser::help<T> requires T to be default-constructible"
        );

        constexpr AccessContext ctx = AccessContext::unchecked();
        String out = stdx::fmt::format(
            "{}\n\n", argc > 0 ? StringView{argv[0]} : StringView{"program"}
        );

        template for (constexpr Field field: Ops::define_static_array(Ops::class_of<T>().fields(ctx))) {
            using MemberType = typename [:field.type().value():];

            Optional<String> short_flag;
            Optional<StringView> description;

            template for (constexpr Annotation ann: Ops::define_static_array(field.annotations())) {
                using AnnotationType = typename [:ann.type().value():];
                if constexpr (IsShortName<AnnotationType>) {
                    short_flag = stdx::fmt::format("-{}", AnnotationType::letter);
                } else if constexpr (IsDescription<AnnotationType>) {
                    description = AnnotationType::str();
                }
            }

            const String long_flag = format_member_as_arg(field.name().value());
            if (short_flag.has_value()) {
                out += stdx::fmt::format("  {}, {}", *short_flag, long_flag);
            } else {
                out += stdx::fmt::format("  {}", long_flag);
            }
            out += stdx::fmt::format("\n      {}", description.value_or(""));
            out += stdx::fmt::format("\n      [type: {}]", friendly_type_name<MemberType>());

            if constexpr (IsOptional<MemberType>) {
                out += "\n      [optional]";
            }
            if constexpr (field.has_default_initializer()) {
                if constexpr (requires (MemberType m) { stdx::fmt::format("{}", m); }) {
                    out += stdx::fmt::format("\n      [default: {}]", T().[:field.value():]);
                } else {
                    out += "\n      [has default]";
                }
            }
            out += "\n\n";
        }

        if (out.size() >= 2) {
            out.resize(out.size() - 2);
        }
        return out;
    }
}
#endif
