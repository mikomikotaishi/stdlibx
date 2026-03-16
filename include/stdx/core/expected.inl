#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_expected
    using ::core::prelude::Expected;
    using ::core::prelude::Unexpected;
    using ::core::prelude::UnexpectTag;
    using ::core::prelude::Unexpect;
    using ::core::prelude::BadExpectedAccessException;
    using ::core::prelude::ErrorDescription;

    using ::core::prelude::swap;

    using ::core::prelude::Ok;
    using ::core::prelude::Err;
    #endif
}
