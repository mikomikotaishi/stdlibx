/**
 * @file stdx.foundation.cppm
 * @module stdx:foundation
 * @brief Module file for the most foundational operations.
 *
 * This file contains exports of the most foundational pieces of the
 * standard library.
 */

module;

#include <version>

export module stdx:foundation;

import std;

/**
 * @namespace stdx
 * @brief The standard library.
 */
export namespace stdx {
    using std::operator<<;
    using std::operator>>;
    using std::operator+;
    using std::operator-;
    using std::operator*;
    using std::operator/;
    using std::operator%;
    using std::operator&;
    using std::operator|;
    using std::operator^;
    using std::operator~;
    using std::operator<;
    using std::operator>;
    using std::operator<=;
    using std::operator>=;
    using std::operator==;
    using std::operator!=;
    using std::operator<=>;
    using std::operator&&;
    using std::operator||;
    using std::operator<<=;
    using std::operator>>=;

    /**
     * @namespace alloc
     * @brief Standard library allocation operations.
     */
    namespace alloc {
        template <typename Ptr>
        using Allocator = std::allocator<Ptr>;
    }

    /**
     * @namespace collections
     * @brief Collection objects of the standard library.
     */
    namespace collections {
        using std::erase;
        using std::erase_if;

        using std::begin;
        using std::cbegin;
        using std::end;
        using std::cend;
        using std::rbegin;
        using std::crbegin;
        using std::rend;
        using std::crend;
        using std::size;
        using std::ssize;
        using std::empty;
        using std::data;
    }

    /**
     * @namespace core
     * @brief Core objects of the standard library.
     */
    namespace core {
        using i8 = std::int8_t;
        using i16 = std::int16_t;
        using i32 = std::int32_t;
        using i64 = std::int64_t;
        using u8 = std::uint8_t;
        using u16 = std::uint16_t;
        using u32 = std::uint32_t;
        using u64 = std::uint64_t;
        using usize = std::size_t;
        using isize = std::intptr_t;
        using intmax = std::intmax_t;
        using uintmax = std::uintmax_t;
        using ptrdiff = std::ptrdiff_t;

        #ifdef __STDCPP_FLOAT16_T__
        using f16 = std::float16_t;
        #endif
        #ifdef __STDCPP_BFLOAT16_T__
        using f32 = std::float32_t;
        #else
        using f32 = float;
        #endif
        #ifdef __STDCPP_FLOAT64_T__
        using f64 = std::float64_t;
        #else
        using f64 = double;
        #endif
        #ifdef __STDCPP_FLOAT128_T__
        using f128 = std::float128_t;
        #else
        using f128 = long double;
        #endif
        #ifdef __STDCPP_BFLOAT16_T__
        using bf16 = std::bfloat16_t;
        #endif

        using byte = unsigned char;
        using char8 = char8_t;
        using char16 = char16_t;
        using char32 = char32_t;
        using wchar = wchar_t;

        using c32 = std::complex<float>;
        using c64 = std::complex<double>;
        using c128 = std::complex<long double>;

        using ByteUnit = std::byte;

        using NullPointer = std::nullptr_t;

        using Void = void;

        using divres = std::div_t;
        using ldivres = std::ldiv_t;
        using lldivres = std::lldiv_t;
        using imaxdiv = std::imaxdiv_t;
        using maxalign = std::max_align_t;

        template <typename T, usize N>
        using Array = std::array<T, N>;

        template <typename T, typename E>
        using Expected = std::expected<T, E>;

        template <typename E>
        using Unexpected = std::unexpected<E>;

        template <typename T1, typename T2>
        using Pair = std::pair<T1, T2>;

        inline constexpr usize DYNAMIC_EXTENT = std::dynamic_extent;

        template <typename T, usize Extent = DYNAMIC_EXTENT>
        using Span = std::span<T, Extent>;

        template <typename... Es>
        using Tuple = std::tuple<Es...>;

        template <typename T>
        using Optional = std::optional<T>;

        using std::nullopt;
        using NullOption = std::nullopt_t;

        template <typename... Ts>
        using Variant = std::variant<Ts...>;

        using Monostate = std::monostate;

        template <typename Sig>
        using Function = std::function<Sig>;

        template <typename T>
        struct Hash: std::hash<T> {
            
        };

        template <typename T = void>
        using EqualTo = std::equal_to<T>;

        template <typename T = void>
        using NotEqualTo = std::not_equal_to<T>;

        template <typename T = void>
        using Greater = std::greater<T>;

        template <typename T = void>
        using Less = std::less<T>;

        template <typename T = void>
        using GreaterEqual = std::greater_equal<T>;

        template <typename T = void>
        using LessEqual = std::less_equal<T>;

        using CompareThreeWay = std::compare_three_way;

        using std::hash;

        class [[nodiscard]] Errc final {
        public:
            using Self = std::errc;

            static constexpr Self SUCCESS = std::errc();
            static constexpr Self ADDRESS_FAMILY_NOT_SUPPORTED = std::errc::address_family_not_supported;
            static constexpr Self ADDRESS_IN_USE = std::errc::address_in_use;
            static constexpr Self ADDRESS_NOT_AVAILABLE = std::errc::address_not_available;
            static constexpr Self ALREADY_CONNECTED = std::errc::already_connected;
            static constexpr Self ARGUMENT_LIST_TOO_LONG = std::errc::argument_list_too_long;
            static constexpr Self ARGUMENT_OUT_OF_DOMAIN = std::errc::argument_out_of_domain;
            static constexpr Self BAD_ADDRESS = std::errc::bad_address;
            static constexpr Self BAD_FILE_DESCRIPTOR = std::errc::bad_file_descriptor;
            static constexpr Self BROKEN_PIPE = std::errc::broken_pipe;
            static constexpr Self CONNECTION_ABORTED = std::errc::connection_aborted;
            static constexpr Self CONNECTION_ALREADY_IN_PROGRESS = std::errc::connection_already_in_progress;
            static constexpr Self CONNECTION_REFUSED = std::errc::connection_refused;
            static constexpr Self CONNECTION_RESET = std::errc::connection_reset;
            static constexpr Self CROSS_DEVICE_LINK = std::errc::cross_device_link;
            static constexpr Self DESTINATION_ADDRESS_REQUIRED = std::errc::destination_address_required;
            static constexpr Self DEVICE_OR_RESOURCE_BUSY = std::errc::device_or_resource_busy;
            static constexpr Self DIRECTORY_NOT_EMPTY = std::errc::directory_not_empty;
            static constexpr Self EXECUTABLE_FORMAT_ERROR = std::errc::executable_format_error;
            static constexpr Self FILE_EXISTS = std::errc::file_exists;
            static constexpr Self FILE_TOO_LARGE = std::errc::file_too_large;
            static constexpr Self FILENAME_TOO_LONG = std::errc::filename_too_long;
            static constexpr Self FUNCTION_NOT_SUPPORTED = std::errc::function_not_supported;
            static constexpr Self HOST_UNREACHABLE = std::errc::host_unreachable;
            static constexpr Self ILLEGAL_BYTE_SEQUENCE = std::errc::illegal_byte_sequence;
            static constexpr Self INAPPROPRIATE_IO_CONTROL_OPERATION = std::errc::inappropriate_io_control_operation;
            static constexpr Self INTERRUPTED = std::errc::interrupted;
            static constexpr Self INVALID_ARGUMENT = std::errc::invalid_argument;
            static constexpr Self INVALID_SEEK = std::errc::invalid_seek;
            static constexpr Self IO_ERROR = std::errc::io_error;
            static constexpr Self IS_ADIRECTORY = std::errc::is_a_directory;
            static constexpr Self MESSAGE_SIZE = std::errc::message_size;
            static constexpr Self NETWORK_DOWN = std::errc::network_down;
            static constexpr Self NETWORK_RESET = std::errc::network_reset;
            static constexpr Self NETWORK_UNREACHABLE = std::errc::network_unreachable;
            static constexpr Self NO_BUFFER_SPACE = std::errc::no_buffer_space;
            static constexpr Self NO_CHILD_PROCESS = std::errc::no_child_process;
            static constexpr Self NO_LOCK_AVAILABLE = std::errc::no_lock_available;
            static constexpr Self NO_MESSAGE = std::errc::no_message;
            static constexpr Self NO_PROTOCOL_OPTION = std::errc::no_protocol_option;
            static constexpr Self NO_SPACE_ON_DEVICE = std::errc::no_space_on_device;
            static constexpr Self NO_SUCH_DEVICE_OR_ADDRESS = std::errc::no_such_device_or_address;
            static constexpr Self NO_SUCH_DEVICE = std::errc::no_such_device;
            static constexpr Self NO_SUCH_FILE_OR_DIRECTORY = std::errc::no_such_file_or_directory;
            static constexpr Self NO_SUCH_PROCESS = std::errc::no_such_process;
            static constexpr Self NOT_ADIRECTORY = std::errc::not_a_directory;
            static constexpr Self NOT_ASOCKET = std::errc::not_a_socket;
            static constexpr Self NOT_CONNECTED = std::errc::not_connected;
            static constexpr Self NOT_ENOUGH_MEMORY = std::errc::not_enough_memory;
            static constexpr Self NOT_SUPPORTED = std::errc::not_supported;
            static constexpr Self OPERATION_CANCELED = std::errc::operation_canceled;
            static constexpr Self OPERATION_IN_PROGRESS = std::errc::operation_in_progress;
            static constexpr Self OPERATION_NOT_PERMITTED = std::errc::operation_not_permitted;
            static constexpr Self OPERATION_NOT_SUPPORTED = std::errc::operation_not_supported;
            static constexpr Self OPERATION_WOULD_BLOCK = std::errc::operation_would_block;
            static constexpr Self PERMISSION_DENIED = std::errc::permission_denied;
            static constexpr Self PROTOCOL_NOT_SUPPORTED = std::errc::protocol_not_supported;
            static constexpr Self READ_ONLY_FILE_SYSTEM = std::errc::read_only_file_system;
            static constexpr Self RESOURCE_DEADLOCK_WOULD_OCCUR = std::errc::resource_deadlock_would_occur;
            static constexpr Self RESOURCE_UNAVAILABLE_TRY_AGAIN = std::errc::resource_unavailable_try_again;
            static constexpr Self RESULT_OUT_OF_RANGE = std::errc::result_out_of_range;
            static constexpr Self TIMED_OUT = std::errc::timed_out;
            static constexpr Self TOO_MANY_FILES_OPEN_IN_SYSTEM = std::errc::too_many_files_open_in_system;
            static constexpr Self TOO_MANY_FILES_OPEN = std::errc::too_many_files_open;
            static constexpr Self TOO_MANY_LINKS = std::errc::too_many_links;
            static constexpr Self TOO_MANY_SYMBOLIC_LINK_LEVELS = std::errc::too_many_symbolic_link_levels;
            static constexpr Self VALUE_TOO_LARGE = std::errc::value_too_large;
            static constexpr Self WRONG_PROTOCOL_TYPE = std::errc::wrong_protocol_type;
        private:
            const Self value = SUCCESS;
        public:
            constexpr Errc() noexcept = default;

            constexpr Errc(Self value) noexcept:
                value{value} {}

            constexpr operator Self() const noexcept {
                return value;
            }
        };
    }

    /**
     * @namespace io
     * @brief Standard library IO operations.
     */
    namespace io {
        class [[nodiscard]] IOErrc final {
        public:
            using Self = std::io_errc;

            static constexpr Self SUCCESS = std::io_errc();
            static constexpr Self STREAM = std::io_errc::stream;
        private:
            const Self value = SUCCESS;
        public:
            constexpr IOErrc() noexcept = default;

            constexpr IOErrc(Self value) noexcept:
                value{value} {}

            constexpr operator Self() const noexcept {
                return value;
            }
        };
    }

    /**
     * @namespace future
     * @brief Standard library concurrency operations.
     */
    namespace future {
        class [[nodiscard]] FutureErrc final {
        public:
            using Self = std::future_errc;

            static constexpr Self SUCCESS = std::future_errc();
            static constexpr Self FUTURE_ALREADY_RETRIEVED = std::future_errc::future_already_retrieved;
            static constexpr Self PROMISE_ALREADY_SATISFIED = std::future_errc::promise_already_satisfied;
            static constexpr Self NO_STATE = std::future_errc::no_state;
            static constexpr Self BROKEN_PROMISE = std::future_errc::broken_promise;
        private:
            const Self value = SUCCESS;
        public:
            constexpr FutureErrc() noexcept = default;

            constexpr FutureErrc(Self value) noexcept:
                value{value} {}

            constexpr operator Self() const noexcept {
                return value;
            }
        };
    }

    /**
     * @namespace literals
     * @brief Standard library literals.
     */
    namespace literals {
        /**
         * @namespace chrono_literals
         * @brief Chrono literals namespace
         */
        inline namespace chrono_literals  {
            using std::literals::chrono_literals::operator""d;
            using std::literals::chrono_literals::operator""h;
            using std::literals::chrono_literals::operator""min;
            using std::literals::chrono_literals::operator""ms;
            using std::literals::chrono_literals::operator""s;
            using std::literals::chrono_literals::operator""us;
            using std::literals::chrono_literals::operator""y;
        }

        /**
         * @namespace complex_literals
         * @brief Complex literals namespace
         */
        inline namespace complex_literals {
            using std::literals::complex_literals::operator""i;
            using std::literals::complex_literals::operator""if;
            using std::literals::complex_literals::operator""il;
        }

        /**
         * @namespace string_literals
         * @brief String literals namespace
         */
        inline namespace string_literals {
            using std::literals::string_literals::operator""s;
        }

        /**
         * @namespace string_view_literals
         * @brief String view literals namespace
         */
        inline namespace string_view_literals {
            using std::literals::string_view_literals::operator""sv;
        }
    }

    /**
     * @namespace meta::reflect
     * @brief Standard library reflection operations.
     */
    namespace meta::reflect {
        #ifdef __cpp_lib_reflection
        using Info = std::meta::info;
        using ReflectiveOperationException = std::meta::exception;
        #endif
    }

    /**
     * @namespace ranges
     * @brief Standard library ranges operations.
     */
    namespace ranges {
        template <typename T>
        concept Range = std::ranges::range<T>;

        template <typename T>
        concept View = std::ranges::view<T>;

        template <Range R>
        using RangeDifference = std::ranges::range_difference_t<R>;

        template <Range R>
        using RangeValue = std::ranges::range_value_t<R>;
    }

    /**
     * @namespace text
     * @brief Standard library text operations.
     */
    namespace text {
        template <typename Char>
        using CharTraits = std::char_traits<Char>;

        /**
         * @namespace stdx::text::string
         * @brief Standard library string operations.
         */
        namespace string {
            using String = std::string;

            template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = ::stdx::alloc::Allocator<Char>>
            using BasicString = std::basic_string<Char, Traits, Alloc>;

            using Utf8String = std::u8string;
            using Utf16String = std::u16string;
            using Utf32String = std::u32string;
            using WideString = std::wstring;

            using StringView = std::string_view;

            template <typename Char, typename Traits = CharTraits<Char>>
            using BasicStringView = std::basic_string_view<Char>;

            using Utf8StringView = std::u8string_view;
            using Utf16StringView = std::u16string_view;
            using Utf32StringView = std::u32string_view;

            using WideStringView = std::wstring_view;
        }
    }

    /**
     * @namespace core
     * @brief Core objects of the standard library.
     */
    namespace core {
        using ::stdx::io::IOErrc;

        using ::stdx::future::FutureErrc;

        using ::stdx::text::string::String;
        using ::stdx::text::string::BasicString;
        using ::stdx::text::string::Utf8String;
        using ::stdx::text::string::Utf16String;
        using ::stdx::text::string::Utf32String;
        using ::stdx::text::string::WideString;
        using ::stdx::text::string::StringView;
        using ::stdx::text::string::BasicStringView;
        using ::stdx::text::string::Utf8StringView;
        using ::stdx::text::string::Utf16StringView;
        using ::stdx::text::string::Utf32StringView;
        using ::stdx::text::string::WideStringView;
    }
}
