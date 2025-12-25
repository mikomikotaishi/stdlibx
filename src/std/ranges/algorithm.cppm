/**
 * @file algorithm.cppm
 * @module std:ranges.algorithm
 * @brief Module file for standard library algorithm operations.
 *
 * This file contains the implementation of the algorithm operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:ranges.algorithm;
#else
export module stdlib:ranges.algorithm;
#endif

import core;

/**
 * @namespace std::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::ranges {
#else 
export namespace stdlib::ranges {
#endif
    using core::ranges::in_fun_result;
    using core::ranges::in_in_result;
    using core::ranges::in_out_result;
    using core::ranges::in_in_out_result;
    using core::ranges::in_out_out_result;
    using core::ranges::min_max_result;
    using core::ranges::in_found_result;
    using core::ranges::in_value_result;
    using core::ranges::out_value_result;

    using core::ranges::for_each;
    using core::ranges::for_each_n;
    using core::ranges::all_of;
    using core::ranges::any_of;
    using core::ranges::none_of;
    using core::ranges::find;
    using core::ranges::find_if;
    using core::ranges::find_if_not;
    using core::ranges::find_last;
    using core::ranges::find_last_if;
    using core::ranges::find_last_if_not;
    using core::ranges::find_end;
    using core::ranges::find_first_of;
    using core::ranges::adjacent_find;
    using core::ranges::count;
    using core::ranges::count_if;
    using core::ranges::mismatch;
    using core::ranges::equal;
    using core::ranges::search;
    using core::ranges::search_n;
    using core::ranges::contains;
    using core::ranges::contains_subrange;

    using core::ranges::fold_left;
    using core::ranges::fold_left_first;
    using core::ranges::fold_right;
    using core::ranges::fold_right_last;
    using core::ranges::fold_left_with_iter;
    using core::ranges::fold_left_first_with_iter;

    using core::ranges::copy;
    using core::ranges::copy_if;
    using core::ranges::copy_n;
    using core::ranges::copy_backward;
    using core::ranges::move;
    using core::ranges::move_backward;

    using core::ranges::swap;
    using core::ranges::swap_ranges;
    using core::ranges::iter_swap;

    using core::ranges::transform;
    using core::ranges::replace;
    using core::ranges::replace_if;
    using core::ranges::replace_copy;
    using core::ranges::replace_copy_if;

    using core::ranges::fill;
    using core::ranges::fill_n;
    using core::ranges::generate;
    using core::ranges::generate_n;

    using core::ranges::remove;
    using core::ranges::remove_if;
    using core::ranges::remove_copy;
    using core::ranges::remove_copy_if;
    using core::ranges::unique;
    using core::ranges::unique_copy;

    using core::ranges::reverse;
    using core::ranges::reverse_copy;
    using core::ranges::rotate;
    using core::ranges::rotate_copy;
    using core::ranges::shuffle;

    using core::ranges::sample;

    using core::ranges::is_partitioned;
    using core::ranges::partition;
    using core::ranges::partition_copy;
    using core::ranges::stable_partition;
    using core::ranges::partition_point;

    using core::ranges::sort;
    using core::ranges::stable_sort;
    using core::ranges::partial_sort;
    using core::ranges::partial_sort_copy;
    using core::ranges::is_sorted;
    using core::ranges::is_sorted_until;
    using core::ranges::nth_element;

    using core::ranges::lower_bound;
    using core::ranges::upper_bound;
    using core::ranges::equal_range;
    using core::ranges::binary_search;

    using core::ranges::includes;
    using core::ranges::set_union;
    using core::ranges::set_intersection;
    using core::ranges::set_difference;
    using core::ranges::set_symmetric_difference;

    using core::ranges::merge;
    using core::ranges::inplace_merge;

    using core::ranges::push_heap;
    using core::ranges::pop_heap;
    using core::ranges::make_heap;
    using core::ranges::sort_heap;
    using core::ranges::is_heap;
    using core::ranges::is_heap_until;

    using core::ranges::max;
    using core::ranges::max_element;
    using core::ranges::min;
    using core::ranges::min_element;
    using core::ranges::minmax;
    using core::ranges::minmax_element;
    using core::ranges::clamp;

    using core::ranges::lexicographical_compare;

    using core::ranges::next_permutation;
    using core::ranges::prev_permutation;
    using core::ranges::is_permutation;
}
