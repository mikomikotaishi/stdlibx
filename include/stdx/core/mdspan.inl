#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #ifdef __cpp_lib_mdspan
    using ::core::prelude::Extents;
    using ::core::prelude::DynamicExtents;
    using ::core::prelude::Dims;
    using ::core::prelude::LayoutLeftPadded;
    using ::core::prelude::LayoutRightPadded;
    using ::core::prelude::DefaultAccessor;
    using ::core::prelude::LayoutLeft;
    using ::core::prelude::LayoutRight;
    using ::core::prelude::LayoutStride;
    using ::core::prelude::MultiDimensionalSpan;
    using ::core::prelude::StridedSlice;
    using ::core::prelude::SubMultiDimensionalSpanMappingResult;
    using ::core::prelude::FullExtentTag;
    using ::core::prelude::FullExtent;

    using ::core::prelude::submdspan_extents;
    using ::core::prelude::submdspan;
    #endif
}
