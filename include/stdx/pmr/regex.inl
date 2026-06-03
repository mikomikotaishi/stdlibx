#pragma once

/**
 * @namespace stdx::pmr::text
 * @brief Namespace for operations on polymorphic memory resources.
 */
namespace stdx::pmr::text {
    using CMatch = std::pmr::cmatch;
    using WideCMatch = std::pmr::wcmatch;
    using StringMatch = std::pmr::smatch;
    using WideStringMatch = std::pmr::wsmatch;
}
