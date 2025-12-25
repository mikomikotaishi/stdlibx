/**
 * @file xml.cppm
 * @module stdx:xml
 * @brief Import of XML modules.
 *
 * This file imports the modules for the XML handling library.
 * Adapted from TinyXML2 (https://github.com/leethomason/tinyxml2)
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
export module stdx:xml;

import std;
#else
export module stdlibx:xml;

import stdlib;
#endif

import :xml.StringPair;

/**
 * @namespace stdx::xml
 * @brief Wrapper namespace for standard library extension XML operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::xml {
#else
export namespace stdlibx::xml {
#endif

class Attr;
class Comment;
class ConstHandle;
class Declaration;
class Document;
class Element;
class Handle;
class Node;
class Printer;
class Text;
class Unknown;
class Visitor;

class Node {
private:
    friend class Document;
    friend class Element;
};



}

