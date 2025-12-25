/**
 * @file cctype.cppm
 * @module core:text.cctype
 * @brief Module file for standard library character type operations.
 *
 * This file contains the imports for the character type operations in the standard library.
 */

module;

#include <cctype>

export module core:text.cctype;

/**
 * @namespace core::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace core::text {
    using std::isalnum;
    using std::isalpha;
    using std::islower;
    using std::isupper;
    using std::isdigit;
    using std::isxdigit;
    using std::iscntrl;
    using std::isgraph;
    using std::isspace;
    using std::isblank;
    using std::isprint;
    using std::ispunct;
    using std::tolower;
    using std::toupper;
}
