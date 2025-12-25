/**
 * @file Stdio.cppm
 * @module stdx:process.Stdio
 * @brief Implementation of the Stdio class.
 *
 * This file contains the implementation of the Stdio class, which is used to
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:process.Stdio;

import std;
#else
export module stdlibx:process.Stdio;

import stdlib;
#endif

import :os;

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
using namespace stdx::os;
#else
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

/**
 * @enum StdioType
 * @brief Enumeration for stdio types.
 * 
 * The StdioType enumeration defines the stdio types that can be spawned.
 */
enum class StdioType: u8 {
    INHERIT, ///< Inherit stdio type.
    PIPE, ///< Pipe stdio type.
    NULL_TYPE, ///< Null stdio type.
};

/**
 * @class Stdio
 * @brief Class for executing Stdioes.
 *
 * The Stdio class runs a Stdio and allows communication with it.
 */
class Stdio {
private:
    StdioType stdioType; ///< The type of the Stdio

    /**
     * @brief Constructs a new Stdio object
     * 
     * @param type The type of Stdio to construct
     */
    explicit Stdio(StdioType stdioType):
        stdioType{stdioType} {
    }
public:
    static Stdio inherit() noexcept {
        return Stdio(StdioType::INHERIT);
    }

    static Stdio pipe() noexcept {
        return Stdio(StdioType::PIPE);
    }

    static Stdio null() noexcept {
        return Stdio(StdioType::NULL_TYPE);
    }

    StdioType type() const noexcept {
        return stdioType;
    }
};

}
