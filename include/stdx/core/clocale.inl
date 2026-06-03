#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using LocaleConversion = std::lconv;

    using std::setlocale;
    using std::localeconv;
}
