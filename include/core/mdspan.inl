#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #ifdef __cpp_lib_mdspan
    template <typename IdxT, usize... Exts>
    using Extents = std::extents<IdxT, Exts...>;

    template <typename IdxT, usize Rank>
    using DynamicExtents = std::dextents<IdxT, Rank>;

    template <usize Rank, typename IdxT = usize>
    using Dims = std::dims<Rank, IdxT>;

    template <usize PaddingValue = DYNAMIC_EXTENT>
    using LayoutLeftPadded = std::layout_left_padded<PaddingValue>;

    template <usize PaddingValue = DYNAMIC_EXTENT>
    using LayoutRightPadded = std::layout_right_padded<PaddingValue>;

    template <typename E>
    using DefaultAccessor = std::default_accessor<E>;

    using LayoutLeft = std::layout_left;
    using LayoutRight = std::layout_right;
    using LayoutStride = std::layout_stride;

    template <
        typename E,
        typename Exts,
        typename LayoutPolicy = LayoutRight,
        typename AccessorPolicy = DefaultAccessor<E>
    >
    using MultiDimensionalSpan = std::mdspan<E, Exts, LayoutPolicy, AccessorPolicy>;
    
    template <typename OffsetT, typename LengthT, typename StrideT>
    using StridedSlice = std::strided_slice<OffsetT, LengthT, StrideT>;

    template <typename LayoutMapping>
    using SubMultiDimensionalSpanMappingResult = std::submdspan_layout_mapping_result<LayoutMapping>;

    using FullExtentTag = std::full_extent_t;
    inline constexpr FullExtentTag FullExtent = std::full_extent;

    using std::submdspan_extents;
    using std::submdspan;
    #endif
}
