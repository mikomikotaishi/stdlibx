/**
 * @file StringPair.cppm
 * @module stdx:xml.StringPair
 * @brief Implementation of the StringPair class
 *
 * This file contains the implementation of the StringPair class.
 * Adapted from tinyxml2 (https://github.com/leethomason/tinyxml2)
 */

/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:xml.StringPair;

import std;
#else
export module stdlibx:xml.StringPair;

import stdlib;
#endif

/**
 * @namespace stdx::xml
 * @brief Wrapper namespace for standard library extension XML operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::xml {
#else
export namespace stdlibx::xml {
#endif

class StringPair {
private:
    i32 flags = 0;
    char* start = nullptr;
    char* end = nullptr;

    void collapse_whitespace();

    enum class Notices: u16 {
        NEEDS_FLUSH = 0x100,
        NEEDS_DELETE = 0x200
    };
public:
    enum class Mode {
        NEEDS_ENTITY_PROCESSING = 0x01,
        NEEDS_NEWLINE_NORMALIZATION = 0x02,
        NEEDS_WHITESPACE_COLLAPSING = 0x04,
        TEXT_ELEMENT = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
        TEXT_ELEMENT_LEAVE_ENTITIES = NEEDS_NEWLINE_NORMALIZATION,
        ATTRIBUTE_NAME = 0,
        ATTRIBUTE_VALUE = NEEDS_ENTITY_PROCESSING | NEEDS_NEWLINE_NORMALIZATION,
        ATTRIBUTE_VALUE_LEAVE_ENTITIES = NEEDS_NEWLINE_NORMALIZATION,
        COMMENT = NEEDS_NEWLINE_NORMALIZATION
    };

    StringPair() = default;

    ~StringPair() {
        reset();
    }

    void reset() {

    }
};

}
