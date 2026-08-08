#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Standard directory lookup, the C face of NSSearchPathForDirectoriesInDomains
 * - <NSSystemDirectories.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<NSSystemDirectories.h>)
    using SearchPathEnumerationState = ::NSSearchPathEnumerationState;
    using SearchPathDirectory = ::NSSearchPathDirectory;
    using SearchPathDomainMask = ::NSSearchPathDomainMask;

    using ::NSStartSearchPathEnumeration;
    using ::NSGetNextSearchPathEnumeration;
    #endif
}
