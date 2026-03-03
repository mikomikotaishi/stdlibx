#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_mdspan
    using ::core::Extents;
    using ::core::DynamicExtents;
    using ::core::Dims;
    using ::core::LayoutLeftPadded;
    using ::core::LayoutRightPadded;
    using ::core::DefaultAccessor;
    using ::core::LayoutLeft;
    using ::core::LayoutRight;
    using ::core::LayoutStride;
    using ::core::MultiDimensionalSpan;
    using ::core::StridedSlice;
    using ::core::SubMultiDimensionalSpanMappingResult;
    using ::core::FullExtentTag;
    using ::core::FullExtent;

    using ::core::submdspan_extents;
    using ::core::submdspan;
    #endif
}
