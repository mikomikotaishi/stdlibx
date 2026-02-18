#pragma once

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    using JumpBuffer = std::jmp_buf;
    
    using std::longjmp;
}
