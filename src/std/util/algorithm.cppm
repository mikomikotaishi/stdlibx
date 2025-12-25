/**
 * @file algorithm.cppm
 * @module std:util.algorithm
 * @brief Module file for standard library algorithm operations.
 *
 * This file contains the implementation of the algorithm operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:util.algorithm;
#else
export module stdlib:util.algorithm;
#endif

import core;

/**
 * @namespace std::util
 * @brief Wrapper namespace for standard library utility operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::util {
#else 
export namespace stdlib::util {
#endif
    using core::util::for_each;
    using core::util::for_each_n;
    using core::util::all_of;
    using core::util::any_of;
    using core::util::none_of;
    using core::util::find;
    using core::util::find_if;
    using core::util::find_if_not;
    using core::util::find_end;
    using core::util::find_first_of;
    using core::util::adjacent_find;
    using core::util::count;
    using core::util::count_if;
    using core::util::mismatch;
    using core::util::equal;
    using core::util::search;
    using core::util::search_n;

    using core::util::copy;
    using core::util::copy_if;
    using core::util::copy_n;
    using core::util::copy_backward;
    using core::util::move;
    using core::util::move_backward;

    using core::util::swap;
    using core::util::swap_ranges;
    using core::util::iter_swap;

    using core::util::transform;
    using core::util::replace;
    using core::util::replace_if;
    using core::util::replace_copy;
    using core::util::replace_copy_if;

    using core::util::fill;
    using core::util::fill_n;
    using core::util::generate;
    using core::util::generate_n;

    using core::util::remove;
    using core::util::remove_if;
    using core::util::remove_copy;
    using core::util::remove_copy_if;
    using core::util::unique;
    using core::util::unique_copy;

    using core::util::reverse;
    using core::util::reverse_copy;
    using core::util::rotate;
    using core::util::rotate_copy;
    using core::util::shift_left;
    using core::util::shift_right;
    using core::util::random_shuffle;
    using core::util::shuffle;

    using core::util::sample;

    using core::util::is_partitioned;
    using core::util::partition;
    using core::util::partition_copy;
    using core::util::stable_partition;
    using core::util::partition_point;

    using core::util::sort;
    using core::util::stable_sort;
    using core::util::partial_sort;
    using core::util::partial_sort_copy;
    using core::util::is_sorted;
    using core::util::is_sorted_until;
    using core::util::nth_element;

    using core::util::lower_bound;
    using core::util::upper_bound;
    using core::util::equal_range;
    using core::util::binary_search;

    using core::util::includes;
    using core::util::set_union;
    using core::util::set_intersection;
    using core::util::set_difference;
    using core::util::set_symmetric_difference;

    using core::util::merge;
    using core::util::inplace_merge;

    using core::util::push_heap;
    using core::util::pop_heap;
    using core::util::make_heap;
    using core::util::sort_heap;
    using core::util::is_heap;
    using core::util::is_heap_until;

    using core::util::max;
    using core::util::max_element;
    using core::util::min;
    using core::util::min_element;
    using core::util::minmax;
    using core::util::minmax_element;
    using core::util::clamp;

    using core::util::lexicographical_compare;
    using core::util::lexicographical_compare_three_way;

    using core::util::next_permutation;
    using core::util::prev_permutation;
    using core::util::is_permutation;
}
