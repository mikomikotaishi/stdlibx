#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    using LocaleConversion = std::lconv;

    using std::setlocale;
    using std::localeconv;
}
