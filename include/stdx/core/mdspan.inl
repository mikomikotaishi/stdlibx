#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
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
    using MultiSpan = std::mdspan<E, Exts, LayoutPolicy, AccessorPolicy>;

    // template <typename OffsetT, typename LengthT, typename StrideT>
    // using StridedSlice = std::strided_slice<OffsetT, LengthT, StrideT>;

    template <typename LayoutMapping>
    using SubMultiSpanMappingResult = std::submdspan_mapping_result<LayoutMapping>;
    #endif

    #ifdef __cpp_lib_submdspan
    // using std::submdspan_extents;
    using std::submdspan;
    #endif

    #ifdef __cpp_lib_aligned_accessor
    template <typename E, u64 ByteAlign>
    using AlignedAccessor = std::aligned_accessor<E, ByteAlign>;
    #endif
}
