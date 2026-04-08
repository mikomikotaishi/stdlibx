#pragma once

/**
 * @namespace stdx::compiler
 * @brief Wrapper namespace for the compiler interaction functionality in standard library.
 */
export namespace stdx::compiler {
    #ifdef __cpp_lib_reflection
    struct Visibility {
        enum class Level {
            DEFAULT,
            HIDDEN,
            INTERNAL,
        };

        Level level = Level::DEFAULT;
    };
    #endif
}
