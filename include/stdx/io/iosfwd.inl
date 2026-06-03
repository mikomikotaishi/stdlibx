#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    using StreamPosition = std::streampos;
    using WideStreamPosition = std::wstreampos;
    using StreamPosition8 = std::u8streampos;
    using StreamPosition16 = std::u16streampos;
    using StreamPosition32 = std::u32streampos;
}
