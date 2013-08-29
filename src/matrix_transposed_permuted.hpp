/**
 *          Copyright Matthias Walter 2010.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 **/

#ifndef MATRIX_TRANSPOSED_PERMUTED_HPP_
#define MATRIX_TRANSPOSED_PERMUTED_HPP_

// Inline functions that refer to transposition and permutation need
// to be declared after both transposition and permutation alone,
// hence the need for a separate header file

namespace unimod
{

  /**
   * Free function to permute two rows of a transposed matrix.
   *
   * @param matrix The transposed matrix
   * @param index1 First index
   * @param index2 Second index
   */

  template <typename MatrixType>
  inline void matrix_permute1(matrix_transposed <MatrixType>& matrix, size_t index1, size_t index2)
  {
    matrix_permute2(matrix.data(), index1, index2);
  }

  /**
   * Free function to permute two columns of a transposed matrix.
   *
   * @param matrix The transposed matrix
   * @param index1 First index
   * @param index2 Second index
   */

  template <typename MatrixType>
  inline void matrix_permute2(matrix_transposed <MatrixType>& matrix, size_t index1, size_t index2)
  {
    matrix_permute1(matrix.data(), index1, index2);
  }

}


#endif
