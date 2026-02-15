/**
 * @file filesystem.cppm
 * @module stdx:fs.filesystem
 * @brief Module file for standard library file system operations.
 *
 * This file contains the implementation of the file system operations in the standard library.
 */

module;

#include <filesystem>

export module stdx:fs.filesystem;

export {
    using std::operator|;
    using std::operator&;
    using std::operator^;
    using std::operator~;
    using std::operator|=;
    using std::operator&=;
    using std::operator^=;
}

/**
 * @namespace stdx::fs
 * @brief Wrapper namespace for standard library file system operations.
 */
export namespace stdx::fs {
    using Path = std::filesystem::path;
    using DirectoryEntry = std::filesystem::directory_entry;
    using DirectoryIterator = std::filesystem::directory_iterator;
    using RecursiveDirectoryIterator = std::filesystem::recursive_directory_iterator;
    using SpaceInfo = std::filesystem::space_info;

    class [[nodiscard]] FileType final {
    public:
        using Self = std::filesystem::file_type;

        static constexpr Self NONE = std::filesystem::file_type::none;
        static constexpr Self NOT_FOUND = std::filesystem::file_type::not_found;
        static constexpr Self REGULAR = std::filesystem::file_type::regular;
        static constexpr Self DIRECTORY = std::filesystem::file_type::directory;
        static constexpr Self SYMLINK = std::filesystem::file_type::symlink;
        static constexpr Self BLOCK = std::filesystem::file_type::block;
        static constexpr Self CHARACTER = std::filesystem::file_type::character;
        static constexpr Self FIFO = std::filesystem::file_type::fifo;
        static constexpr Self SOCKET = std::filesystem::file_type::socket;
        static constexpr Self UNKNOWN = std::filesystem::file_type::unknown;
    private:
        Self value;
    public:
        constexpr FileType(Self value = NONE) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] CopyOptions final {
    public:
        using Self = std::filesystem::copy_options;

        static constexpr Self NONE = std::filesystem::copy_options::none;
        static constexpr Self SKIP_EXISTING = std::filesystem::copy_options::skip_existing;
        static constexpr Self OVERWRITE_EXISTING = std::filesystem::copy_options::overwrite_existing;
        static constexpr Self UPDATE_EXISTING = std::filesystem::copy_options::update_existing;
        static constexpr Self RECURSIVE = std::filesystem::copy_options::recursive;
        static constexpr Self COPY_SYMLINKS = std::filesystem::copy_options::copy_symlinks;
        static constexpr Self SKIP_SYMLINKS = std::filesystem::copy_options::skip_symlinks;
        static constexpr Self DIRECTORIES_ONLY = std::filesystem::copy_options::directories_only;
        static constexpr Self CREATE_SYMLINKS = std::filesystem::copy_options::create_symlinks;
        static constexpr Self CREATE_HARD_LINKS = std::filesystem::copy_options::create_hard_links;
    private:
        Self value;
    public:
        constexpr CopyOptions(Self value = NONE) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] Permissions final {
    public:
        using Self = std::filesystem::perms;

        static constexpr Self NONE = std::filesystem::perms::none;
        static constexpr Self OWNER_READ = std::filesystem::perms::owner_read;
        static constexpr Self OWNER_WRITE = std::filesystem::perms::owner_write;
        static constexpr Self OWNER_EXEC = std::filesystem::perms::owner_exec;
        static constexpr Self OWNER_ALL = std::filesystem::perms::owner_all;
        static constexpr Self GROUP_READ = std::filesystem::perms::group_read;
        static constexpr Self GROUP_WRITE = std::filesystem::perms::group_write;
        static constexpr Self GROUP_EXEC = std::filesystem::perms::group_exec;
        static constexpr Self GROUP_ALL = std::filesystem::perms::group_all;
        static constexpr Self OTHERS_READ = std::filesystem::perms::others_read;
        static constexpr Self OTHERS_WRITE = std::filesystem::perms::others_write;
        static constexpr Self OTHERS_EXEC = std::filesystem::perms::others_exec;
        static constexpr Self OTHERS_ALL = std::filesystem::perms::others_all;
        static constexpr Self ALL = std::filesystem::perms::all;
        static constexpr Self SET_UID = std::filesystem::perms::set_uid;
        static constexpr Self SET_GID = std::filesystem::perms::set_gid;
        static constexpr Self STICKY_BIT = std::filesystem::perms::sticky_bit;
        static constexpr Self MASK = std::filesystem::perms::mask;
        static constexpr Self UNKNOWN = std::filesystem::perms::unknown;
    private:
        Self value;
    public:
        constexpr Permissions(Self value = NONE) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] PermissionOptions final {
    public:
        using Self = std::filesystem::perm_options;

        static constexpr Self REPLACE = std::filesystem::perm_options::replace;
        static constexpr Self ADD = std::filesystem::perm_options::add;
        static constexpr Self REMOVE = std::filesystem::perm_options::remove;
        static constexpr Self NO_FOLLOW = std::filesystem::perm_options::nofollow;
    private:
        Self value;
    public:
        constexpr PermissionOptions(Self value = REPLACE) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using FileStatus = std::filesystem::file_status;

    class [[nodiscard]] DirectoryOptions final {
    public:
        using Self = std::filesystem::directory_options;

        static constexpr Self NONE = std::filesystem::directory_options::none;
        static constexpr Self FOLLOW_DIRECTORY_SYMLINK = std::filesystem::directory_options::follow_directory_symlink;
        static constexpr Self SKIP_PERMISSION_DENIED = std::filesystem::directory_options::skip_permission_denied;
    private:
        Self value;
    public:
        constexpr DirectoryOptions(Self value = NONE) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using FileTimeType = std::filesystem::file_time_type;

    using FileSystemException = std::filesystem::filesystem_error;

    using std::filesystem::absolute;
    using std::filesystem::canonical;
    using std::filesystem::weakly_canonical;
    using std::filesystem::relative;
    using std::filesystem::proximate;
    using std::filesystem::copy;
    using std::filesystem::copy_file;
    using std::filesystem::copy_symlink;
    using std::filesystem::create_directory;
    using std::filesystem::create_directories;
    using std::filesystem::create_hard_link;
    using std::filesystem::create_symlink;
    using std::filesystem::create_directory_symlink;
    using std::filesystem::current_path;
    using std::filesystem::exists;
    using std::filesystem::equivalent;
    using std::filesystem::file_size;
    using std::filesystem::hard_link_count;
    using std::filesystem::last_write_time;
    using std::filesystem::permissions;
    using std::filesystem::read_symlink;
    using std::filesystem::remove;
    using std::filesystem::remove_all;
    using std::filesystem::rename;
    using std::filesystem::resize_file;
    using std::filesystem::space;
    using std::filesystem::status;
    using std::filesystem::symlink_status;
    using std::filesystem::temp_directory_path;

    using std::filesystem::is_block_file;
    using std::filesystem::is_character_file;
    using std::filesystem::is_directory;
    using std::filesystem::is_empty;
    using std::filesystem::is_fifo;
    using std::filesystem::is_other;
    using std::filesystem::is_regular_file;
    using std::filesystem::is_socket;
    using std::filesystem::is_symlink;
    using std::filesystem::status_known;

    using std::hash;
    using std::swap;
}
