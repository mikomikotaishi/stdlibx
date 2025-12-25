/**
 * @file ArgumentParser.cppm
 * @module stdx:util.ArgumentParser
 * @brief Implementation of the ArgumentParser class.
 *
 * This file contains the implementation of the ArgumentParser class, which is used to represent
 * the actual argument parser.
 * Adapted from p-ranav/argparse (https://github.com/p-ranav/argparse)
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:util.ArgumentParser;

import std;
#else
export module stdlibx:util.ArgumentParser;

import stdlib;
#endif

import core;

import :util.ArgumentParser.Details;

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
using std::io::OutputStream;
using std::fmt::FormatContext;
using std::fmt::FormatParseContext;
using std::fmt::Formatter;

namespace fmt = std::fmt;
namespace ranges = std::ranges;
#else
using stdlib::io::OutputStream;
using stdlib::fmt::FormatContext;
using stdlib::fmt::FormatParseContext;
using stdlib::fmt::Formatter;

namespace fmt = stdlib::fmt;
namespace ranges = stdlib::ranges;
#endif

#if 0

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library extension utility operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::util {
#else
export namespace stdlibx::util {
#endif

class ArgumentParser;

class Argument {
private:
    using ReturnableAction = Function<Any(const String&)>;
    using VoidAction = Function<void(const String&)>;

    friend class ArgumentParser;
    friend class Formatter;

    class NArgsRange {
    private:
        usize min;
        usize max;
    public:
        NArgsRange(usize min, usize max) throws (InvalidArgumentException):
            min{min}, max{max} {
            if (min > max) {
                throw InvalidArgumentException("Range of number of arguments is invalid!");
            }
        }

        [[nodiscard]]
        bool contains(usize value) const noexcept {
            return value >= min && value <= max;
        }

        [[nodiscard]]
        bool is_exact() const noexcept {
            return min == max;
        }

        [[nodiscard]]
        bool is_right_bounded() const noexcept {
            return max < NumericLimits<usize>::max();
        }

        [[nodiscard]]
        usize get_min() const noexcept {
            return min;
        }

        [[nodiscard]]
        usize get_max() const noexcept {
            return max;
        }

        [[nodiscard]]
        StrongOrdering operator<=>(const NArgsRange& other) const noexcept {
            return min != other.min
                ? min <=> other.min
                : max <=> other.max;
        }
    };

    Vector<String> names;
    Vector<Variant<ReturnableAction, VoidAction>> actions;
    Variant<ReturnableAction, VoidAction> default_action{ 
        InPlaceType<ReturnableAction>,
        [](const String& s) -> String { return s; }
    };
    String help;
    String meta_variable;
    String default_value_representation;
    Optional<String> default_value_string;
    Any default_value;
    NArgsRange num_args_range{1, 1};
    StringView used_name;
    StringView prefix_chars;
    usize group_index = 0;
    u32 usage_newline_counter = 0;
    bool accepts_optional_like_value : 1;
    bool optional : 1;
    bool required : 1;
    bool repeatable : 1;
    bool used : 1;
    bool hidden : 1;

    template <usize N, usize... I>
    explicit Argument(StringView prefix_chars, Array<StringView, N>&& a, [[maybe_unused]] IndexSequence<I...> ind_seq):
        prefix_chars{prefix_chars}, accepts_optional_like_value{false}, optional{(optional(a[I], prefix_chars) || ...)},
        required{false}, repeatable{false}, used{false}, hidden{false} {
        (names.emplace_back(a[I]), ...);
        ranges::sort(
            names,
            []<TotallyOrdered T>(const T& lhs, const T& rhs) -> bool 
                requires requires(const T& t) { { t.size() } -> ConvertibleTo<usize>; }
            {
                return lhs.size() == rhs.size()
                    ? lhs < rhs
                    : lhs.size() < rhs.size();
            }
        );
    }
public:
    template <usize N>
    explicit Argument(StringView prefix_chars, Array<StringView, N>&& a):
        Argument(prefix_chars, core::util::move(a), make_index_sequence<N>{}) {}
};

}

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
using stdx::util::Argument;
#else
using stdlibx::util::Argument;
#endif

template <>
struct Formatter<Argument::NArgsRange> {
    static constexpr const char* parse(FormatParseContext& ctx) noexcept {
        return ctx.begin();
    }

    static FormatContext::iterator format(const Argument::NArgsRange& range, FormatContext& ctx) const {
        if (range.is_exact()) {
            if (range.get_min() != 0 && range.get_min() != 1) {
                return fmt::format_to(ctx.out(), "[nargs: {}] ", range.get_min());
            }
        } else {
            if (!range.is_right_bounded()) {
                return fmt::format_to(ctx.out(), "[nargs: {} or more] ", range.get_min());
            } else {
                return fmt::format_to(ctx.out(), "[nargs={}..{}] ", range.get_min(), range.get_max());
            }
        }
        return ctx.out();
    }
};

#endif

