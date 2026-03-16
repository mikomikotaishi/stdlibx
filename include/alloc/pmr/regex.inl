#pragma once

/**
 * @namespace aloc::pmr::text
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::pmr::text {
    using CMatch = std::pmr::cmatch;

    using WideCMatch = std::pmr::wcmatch;
    using StringMatch = std::pmr::smatch;
    using WideStringMatch = std::pmr::wsmatch;
}
