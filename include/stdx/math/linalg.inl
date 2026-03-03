#pragma once

/**
 * @namespace stdx::math::linalg
 * @brief Wrapper namespace for standard library linear algebra operations.
 */
export namespace stdx::math::linalg {
    #ifdef __cpp_lib_linalg
    using ::core::math::linalg::LayoutBlasPacked;
    using ::core::math::linalg::LayoutBlasPackedMapping;
    using ::core::math::linalg::ScaledAccessor;
    using ::core::math::linalg::ConjugatedAccessor;
    using ::core::math::linalg::LayoutTranspose;
    using ::core::math::linalg::ColumnMajorTag;
    using ::core::math::linalg::ColumnMajor;
    using ::core::math::linalg::RowMajorTag;
    using ::core::math::linalg::RowMajor;
    using ::core::math::linalg::UpperTriangleTag;
    using ::core::math::linalg::UpperTriangle;
    using ::core::math::linalg::LowerTriangleTag;
    using ::core::math::linalg::LowerTriangle;
    using ::core::math::linalg::ImplicitUnitDiagonalTag;
    using ::core::math::linalg::ImplicitUnitDiagonal;
    using ::core::math::linalg::ExplicitDiagonalTag;
    using ::core::math::linalg::ExplicitDiagonal;

    using ::core::math::linalg::scaled;
    using ::core::math::linalg::conjugated;
    using ::core::math::linalg::transposed;
    using ::core::math::linalg::conjugate_transposed;

    // BLAS 1 functions
    using ::core::math::linalg::setup_givens_rotation;
    using ::core::math::linalg::apply_givens_rotation;
    using ::core::math::linalg::swap_elements;
    using ::core::math::linalg::scale;
    using ::core::math::linalg::copy;
    using ::core::math::linalg::add;
    using ::core::math::linalg::dot;
    using ::core::math::linalg::dotc;
    using ::core::math::linalg::vector_sum_of_squares;
    using ::core::math::linalg::vector_two_norm;
    using ::core::math::linalg::vector_abs_sum;
    using ::core::math::linalg::vector_idx_abs_max;
    using ::core::math::linalg::matrix_frob_norm;
    using ::core::math::linalg::matrix_one_norm;
    using ::core::math::linalg::matrix_inf_norm;

    // BLAS 2 functions
    using ::core::math::linalg::matrix_vector_product;
    using ::core::math::linalg::symmetric_matrix_vector_product;
    using ::core::math::linalg::hermitian_matrix_vector_product;
    using ::core::math::linalg::triangular_matrix_vector_product;
    using ::core::math::linalg::triangular_matrix_vector_solve;
    using ::core::math::linalg::matrix_rank_1_update;
    using ::core::math::linalg::matrix_rank_1_update_c;
    using ::core::math::linalg::symmetric_matrix_rank_1_update;
    using ::core::math::linalg::hermitian_matrix_rank_1_update;
    using ::core::math::linalg::symmetric_matrix_rank_2_update;
    using ::core::math::linalg::hermitian_matrix_rank_2_update;

    // BLAS 3 functions
    using ::core::math::linalg::matrix_product;
    using ::core::math::linalg::symmetric_matrix_product;
    using ::core::math::linalg::hermitian_matrix_product;
    using ::core::math::linalg::triangular_matrix_product;
    using ::core::math::linalg::triangular_matrix_left_product;
    using ::core::math::linalg::triangular_matrix_right_product;
    using ::core::math::linalg::symmetric_matrix_rank_k_update;
    using ::core::math::linalg::hermitian_matrix_rank_k_update;
    using ::core::math::linalg::symmetric_matrix_rank_2k_update;
    using ::core::math::linalg::hermitian_matrix_rank_2k_update;
    using ::core::math::linalg::triangular_matrix_matrix_left_solve;
    using ::core::math::linalg::triangular_matrix_matrix_right_solve;
    #endif
}
