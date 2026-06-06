#pragma once

/**
 * @namespace stdx::sys
 * @brief Standard library system operations.
 */
export namespace stdx::sys {
    using JumpBuffer = std::jmp_buf;
    
    using std::longjmp;
}
