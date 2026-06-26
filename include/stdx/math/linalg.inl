#pragma once

/**
 * @namespace stdx::math::linalg
 * @brief Standard library linear algebra operations.
 */
export namespace stdx::math::linalg {
    #ifdef __cpp_lib_linalg
    template <typename Triangle, typename StorageOrder>
    using LayoutBlasPacked = std::linalg::layout_blas_packed<Triangle, StorageOrder>;

    template <typename Triangle, typename StorageOrder, typename Extents>
    using LayoutBlasPackedMapping = typename std::linalg::layout_blas_packed<Triangle, StorageOrder>
        ::template mapping<Extents>;

    template <typename ScalingFactor, typename NestedAccessor>
    using ScaledAccessor = std::linalg::scaled_accessor<ScalingFactor, NestedAccessor>;

    template <typename NestedAccessor>
    using ConjugatedAccessor = std::linalg::conjugated_accessor<NestedAccessor>;

    template <typename Layout>
    using LayoutTranspose = std::linalg::layout_transpose<Layout>;

    using std::linalg::scaled;
    using std::linalg::conjugated;
    using std::linalg::transposed;
    using std::linalg::conjugate_transposed;

    // BLAS 1 functions
    using std::linalg::setup_givens_rotation;
    using std::linalg::apply_givens_rotation;
    using std::linalg::swap_elements;
    using std::linalg::scale;
    using std::linalg::copy;
    using std::linalg::add;
    using std::linalg::dot;
    using std::linalg::dotc;
    using std::linalg::vector_sum_of_squares;
    using std::linalg::vector_two_norm;
    using std::linalg::vector_abs_sum;
    using std::linalg::vector_idx_abs_max;
    using std::linalg::matrix_frob_norm;
    using std::linalg::matrix_one_norm;
    using std::linalg::matrix_inf_norm;

    // BLAS 2 functions
    using std::linalg::matrix_vector_product;
    using std::linalg::symmetric_matrix_vector_product;
    using std::linalg::hermitian_matrix_vector_product;
    using std::linalg::triangular_matrix_vector_product;
    using std::linalg::triangular_matrix_vector_solve;
    using std::linalg::matrix_rank_1_update;
    using std::linalg::matrix_rank_1_update_c;
    using std::linalg::symmetric_matrix_rank_1_update;
    using std::linalg::hermitian_matrix_rank_1_update;
    using std::linalg::symmetric_matrix_rank_2_update;
    using std::linalg::hermitian_matrix_rank_2_update;

    // BLAS 3 functions
    using std::linalg::matrix_product;
    using std::linalg::symmetric_matrix_product;
    using std::linalg::hermitian_matrix_product;
    using std::linalg::triangular_matrix_product;
    using std::linalg::triangular_matrix_left_product;
    using std::linalg::triangular_matrix_right_product;
    using std::linalg::symmetric_matrix_rank_k_update;
    using std::linalg::hermitian_matrix_rank_k_update;
    using std::linalg::symmetric_matrix_rank_2k_update;
    using std::linalg::hermitian_matrix_rank_2k_update;
    using std::linalg::triangular_matrix_matrix_left_solve;
    using std::linalg::triangular_matrix_matrix_right_solve;
    #endif
}
