#pragma once

/**
 * @namespace stdx::simd
 * @brief Standard library data parallel operations.
 */
export namespace stdx::simd {
    #ifdef __cpp_lib_simd
    template <typename T, typename Abi = std::simd::basic_vec<T>::abi_type>
    using BasicSimd = std::simd::basic_vec<T, Abi>;

    template <typename T, usize N = std::simd::vec<T>::size>
    using Simd = std::simd::vec<T, N>;

    template <usize N, typename Abi>
    using BasicMask = std::simd::basic_mask<N, Abi>;

    template <typename T, usize N = std::simd::mask<T>::size>
    using Mask = std::simd::mask<T, N>;

    template <typename... Fs>
    struct Flags: public std::simd::flags<Fs...> {
        using Self = std::simd::flags<Fs...>;

        static constexpr auto DEFAULT = std::simd::flag_default;
        static constexpr auto CONVERT = std::simd::flag_convert;
        static constexpr auto ALIGNED = std::simd::flag_aligned;

        template <usize N>
            requires (std::simd::has_single_bit(N))
        static constexpr auto OVERALIGNED = std::simd::flag_overaligned<N>;
    };

    template <typename T, typename U = typename T::value_type>
    using Alignment = std::simd::alignment<T, U>;

    template <typename T, typename U = typename T::value_type>
    constexpr usize AlignmentValue = std::simd::alignment_v<T, U>;

    template <typename T, typename V>
    using Rebind = std::simd::rebind<T, U>;

    template <typename T, typename V>
    using RebindType = std::simd::rebind_t<T, U>;

    template <usize N, typename V>
    using Resize = std::simd::resize<N, V>;

    template <usize N, typename V>
    using ResizeType = std::simd::resize_t<N, V>;

    using std::simd::unchecked_load;
    using std::simd::partial_load;
    using std::simd::unchecked_store;
    using std::simd::partial_store;
    using std::simd::chunk;
    using std::simd::cat;
    using std::simd::all_of;
    using std::simd::any_of;
    using std::simd::none_of;
    using std::simd::reduce_count;
    using std::simd::reduce_min_index;
    using std::simd::reduce_max_index;
    using std::simd::reduce;
    using std::simd::reduce_min;
    using std::simd::reduce_max;
    using std::simd::min;
    using std::simd::max;
    using std::simd::minmax;
    using std::simd::clamp;
    using std::simd::select;
    #endif
}
