#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief The Blocks runtime - <Block.h>.
 *
 * The Block_copy and Block_release spellings are macros; these are the
 * functions they expand to.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && defined(__BLOCKS__) && __has_include(<Block.h>)
    using ::_Block_copy;
    using ::_Block_release;
    using ::_Block_object_assign;
    using ::_Block_object_dispose;
    #endif
}
