/**
 * @file PipeStream.cppm
 * @module stdx:process.PipeStream
 * @brief Implementation of the PipeStream class.
 *
 * This file contains the implementation of the PipeStream class, which is used to
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:process.PipeStream;

import std;
#else
export module stdlibx:process.PipeStream;

import stdlib;
#endif

import :os;

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
#ifdef _WIN32
using stdx::os::windows::Handle;
#endif

using namespace stdx::os;
#else
#ifdef _WIN32
using stdlibx::os::windows::Handle;
#endif

using namespace stdlibx::os;
#endif

/**
 * @namespace stdx::process
 * @brief Wrapper namespace for standard library extension process operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::process {
#else
export namespace stdlibx::process {
#endif

#ifdef _WIN32
#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
using Handle = std::os::windows::Handle;
#else
using Handle = stdlib::os::windows::Handle;
#endif
#else
using Handle = i32;
#endif

class PipeStream {
private:
    Handle handle;
    bool ownsHandle;
public:
    /**
     * @brief Construct a new Pipe Stream object
     * 
     * @param handle The handle of the PipeStream
     * @param owns Whether the PipeStream owns the handle
     */
    explicit PipeStream(Handle handle, bool owns = true):
        handle{handle}, ownsHandle{owns} {}

    /**
     * @brief Destroy the Pipe Stream object
     */
    ~PipeStream() {
        if (ownsHandle) {
            #ifdef _WIN32
            windows::CloseHandle(handle);
            #else
            unix::close(handle);
            #endif
        }
    }

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    PipeStream(const PipeStream&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent copying.
     */
    PipeStream& operator=(const PipeStream&) = delete;

    PipeStream(PipeStream&& other) noexcept: 
        handle{other.handle}, ownsHandle{other.ownsHandle} {
        other.ownsHandle = false;
    }

    PipeStream& operator=(PipeStream&& other) noexcept {
        if (this != &other) {
            if (ownsHandle) {
                #ifdef _WIN32
                windows::CloseHandle(handle);
                #else
                unix::close(handle);
                #endif
            }
            handle = other.handle;
            ownsHandle = other.ownsHandle;
            other.ownsHandle = false;
        }
        return *this;
    }

    [[nodiscard]]
    Handle raw_handle() const noexcept {
        return handle;
    }
};

}
