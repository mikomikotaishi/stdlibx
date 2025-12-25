/**
 * @file algorithm.cppm
 * @module core:util.algorithm
 * @brief Module file for standard library algorithm operations.
 *
 * This file contains the implementation of the algorithm operations in the standard library.
 */

module;

#include <algorithm>

export module core:util.algorithm;

/**
 * @namespace core::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace core::util {
    using std::for_each;
    using std::for_each_n;
    using std::all_of;
    using std::any_of;
    using std::none_of;
    using std::find;
    using std::find_if;
    using std::find_if_not;
    using std::find_end;
    using std::find_first_of;
    using std::adjacent_find;
    using std::count;
    using std::count_if;
    using std::mismatch;
    using std::equal;
    using std::search;
    using std::search_n;

    using std::copy;
    using std::copy_if;
    using std::copy_n;
    using std::copy_backward;
    using std::move;
    using std::move_backward;

    using std::swap;
    using std::swap_ranges;
    using std::iter_swap;

    using std::transform;
    using std::replace;
    using std::replace_if;
    using std::replace_copy;
    using std::replace_copy_if;

    using std::fill;
    using std::fill_n;
    using std::generate;
    using std::generate_n;

    using std::remove;
    using std::remove_if;
    using std::remove_copy;
    using std::remove_copy_if;
    using std::unique;
    using std::unique_copy;

    using std::reverse;
    using std::reverse_copy;
    using std::rotate;
    using std::rotate_copy;
    using std::shift_left;
    using std::shift_right;
    using std::random_shuffle;
    using std::shuffle;

    using std::sample;

    using std::is_partitioned;
    using std::partition;
    using std::partition_copy;
    using std::stable_partition;
    using std::partition_point;

    using std::sort;
    using std::stable_sort;
    using std::partial_sort;
    using std::partial_sort_copy;
    using std::is_sorted;
    using std::is_sorted_until;
    using std::nth_element;

    using std::lower_bound;
    using std::upper_bound;
    using std::equal_range;
    using std::binary_search;

    using std::includes;
    using std::set_union;
    using std::set_intersection;
    using std::set_difference;
    using std::set_symmetric_difference;

    using std::merge;
    using std::inplace_merge;

    using std::push_heap;
    using std::pop_heap;
    using std::make_heap;
    using std::sort_heap;
    using std::is_heap;
    using std::is_heap_until;

    using std::max;
    using std::max_element;
    using std::min;
    using std::min_element;
    using std::minmax;
    using std::minmax_element;
    using std::clamp;

    using std::lexicographical_compare;
    using std::lexicographical_compare_three_way;

    using std::next_permutation;
    using std::prev_permutation;
    using std::is_permutation;
}
