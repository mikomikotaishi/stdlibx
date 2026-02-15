/**
 * @file system_error.cppm
 * @module core:system_error
 * @brief Module file for standard library system error classes.
 *
 * This file contains the implementation of error classes in the standard library.
 */

module;

#include <ios>
#include <system_error>

export module core:system_error;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    using ErrorCategory = std::error_category;
    using ErrorCondition = std::error_condition;
    using ErrorCode = std::error_code;

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
        static constexpr Self IOERROR = std::errc::io_error;
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
        Self value;
    public:
        constexpr Errc(Self value = SUCCESS) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using SystemException = std::system_error;

    template <typename T>
    using IsErrorCodeEnum = std::is_error_code_enum<T>;

    template <typename T>
    constexpr bool IsErrorCodeEnumValue = std::is_error_code_enum_v<T>;

    template <typename T>
    using IsErrorConditionEnum = std::is_error_condition_enum<T>;

    template <typename T>
    constexpr bool IsErrorConditionEnumValue = std::is_error_condition_enum_v<T>;

    using std::generic_category;
    using std::system_category;
    using std::make_error_code;
    using std::make_error_condition;

    using std::hash;
}
