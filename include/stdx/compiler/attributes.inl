#pragma once

/**
 * @namespace stdx::compiler
 * @brief The compiler interaction functionality in standard library.
 */
export namespace stdx::compiler {
    #ifdef __cpp_lib_reflection
    struct NoReturn {

    };

    struct Deprecated {
        StringView reason = ""sv;
    };

    struct FallThrough {

    };

    struct MaybeUnused {

    };

    struct NoDiscard {
        StringView reason = ""sv;
    };

    struct Likely {

    };

    struct Unlikely {

    };

    struct NoUniqueAddress {

    };

    struct Assume {
        bool expression;
    };

    struct Indeterminate {

    };
    #endif
}
