#pragma once

/**
 * @namespace core::prelude
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core::prelude {
    using LocaleConversion = std::lconv;

    using std::setlocale;
    using std::localeconv;
}
