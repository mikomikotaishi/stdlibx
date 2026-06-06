#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Win32 file I/O API (fileapi.h) wrappers.
 *
 * These are the kernel-level Win32 functions (CreateFile, ReadFile, ...).
 * For CRT POSIX-style I/O (_open / _read / _write) see io.inl.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<fileapi.h>)
    // Find / file info structures
    using FindData = ::WIN32_FIND_DATAA;
    using FindDataW = ::WIN32_FIND_DATAW;
    using FindDataPointer = ::LPWIN32_FIND_DATAA;
    using FindDataWPointer = ::LPWIN32_FIND_DATAW;
    using FileAttributeData = ::WIN32_FILE_ATTRIBUTE_DATA;
    using FileInformationByHandle = ::BY_HANDLE_FILE_INFORMATION;
    using FileInformationByHandlePointer = ::LPBY_HANDLE_FILE_INFORMATION;
    using FileBasicInfo = ::FILE_BASIC_INFO;
    using FileStandardInfo = ::FILE_STANDARD_INFO;
    using FileIdInfo = ::FILE_ID_INFO;
    using FileNameInfo = ::FILE_NAME_INFO;
    using FileRenameInfo = ::FILE_RENAME_INFO;
    using FileDispositionInfo = ::FILE_DISPOSITION_INFO;
    using FileAllocationInfo = ::FILE_ALLOCATION_INFO;
    using FileEndOfFileInfo = ::FILE_END_OF_FILE_INFO;
    using FileStreamInfo = ::FILE_STREAM_INFO;
    using FileCompressionInfo = ::FILE_COMPRESSION_INFO;
    using FileAttributeTagInfo = ::FILE_ATTRIBUTE_TAG_INFO;
    using FileFullDirInfo = ::FILE_FULL_DIR_INFO;
    using FileIdBothDirInfo = ::FILE_ID_BOTH_DIR_INFO;
    using FileInfoByHandleClass = ::FILE_INFO_BY_HANDLE_CLASS;

    // File creation / handles
    using ::CreateFileA;
    using ::CreateFileW;
    using ::CreateFile2;
    using ::CloseHandle;
    using ::DuplicateHandle;
    using ::GetHandleInformation;
    using ::SetHandleInformation;

    // Read / write
    using ::ReadFile;
    using ::ReadFileEx;
    using ::ReadFileScatter;
    using ::WriteFile;
    using ::WriteFileEx;
    using ::WriteFileGather;
    using ::FlushFileBuffers;

    // Position / size
    using ::SetFilePointer;
    using ::SetFilePointerEx;
    using ::GetFileSize;
    using ::GetFileSizeEx;
    using ::SetEndOfFile;
    using ::SetFileValidData;

    // Attributes / timestamps
    using ::GetFileAttributesA;
    using ::GetFileAttributesW;
    using ::GetFileAttributesExA;
    using ::GetFileAttributesExW;
    using ::SetFileAttributesA;
    using ::SetFileAttributesW;
    using ::GetFileTime;
    using ::SetFileTime;
    using ::GetFileType;
    using ::GetFileInformationByHandle;
    using ::GetFileInformationByHandleEx;
    using ::SetFileInformationByHandle;

    // Delete / rename / copy
    using ::DeleteFileA;
    using ::DeleteFileW;
    using ::MoveFileA;
    using ::MoveFileW;
    using ::MoveFileExA;
    using ::MoveFileExW;
    using ::CopyFileA;
    using ::CopyFileW;
    using ::CopyFileExA;
    using ::CopyFileExW;
    using ::ReplaceFileA;
    using ::ReplaceFileW;

    // Locking
    using ::LockFile;
    using ::LockFileEx;
    using ::UnlockFile;
    using ::UnlockFileEx;

    // Find
    using ::FindFirstFileA;
    using ::FindFirstFileW;
    using ::FindFirstFileExA;
    using ::FindFirstFileExW;
    using ::FindNextFileA;
    using ::FindNextFileW;
    using ::FindClose;

    // Directories
    using ::CreateDirectoryA;
    using ::CreateDirectoryW;
    using ::CreateDirectoryExA;
    using ::CreateDirectoryExW;
    using ::RemoveDirectoryA;
    using ::RemoveDirectoryW;
    using ::GetCurrentDirectoryA;
    using ::GetCurrentDirectoryW;
    using ::SetCurrentDirectoryA;
    using ::SetCurrentDirectoryW;
    using ::GetFullPathNameA;
    using ::GetFullPathNameW;
    using ::GetTempPathA;
    using ::GetTempPathW;
    using ::GetTempFileNameA;
    using ::GetTempFileNameW;
    using ::GetFinalPathNameByHandleA;
    using ::GetFinalPathNameByHandleW;

    // Drives / volumes
    using ::GetLogicalDrives;
    using ::GetLogicalDriveStringsA;
    using ::GetLogicalDriveStringsW;
    using ::GetDriveTypeA;
    using ::GetDriveTypeW;
    using ::GetDiskFreeSpaceA;
    using ::GetDiskFreeSpaceW;
    using ::GetDiskFreeSpaceExA;
    using ::GetDiskFreeSpaceExW;
    using ::GetVolumeInformationA;
    using ::GetVolumeInformationW;
    #endif
}
