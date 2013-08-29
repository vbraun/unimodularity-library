/**
 *          Copyright Matthias Walter 2010.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 **/

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <boost/numeric/ublas/matrix.hpp>

#include "matrix_transposed.hpp"
#include <iomanip>

namespace unimod
{

  /**
   * Exception to indicate a pivot on a zero element.
   */

  class matrix_binary_pivot_exception: public std::exception
  {
  public:
    const char* what() const throw ()
    {
      return "Cannot pivot on a zero entry!";
    }
  };

  /**
   * Free function to set a matrix value of a permuted matrix.
   *
   * @param matrix The permuted matrix
   * @param row Row index
   * @param column Column index
   * @param value New value
   */

  template <typename MatrixType>
  inline void matrix_set_value(MatrixType& matrix, size_t row, size_t column, typename MatrixType::value_type value)
  {
    matrix(row, column) = value;
  }

  /**
   * Dummy function for the version with writable orignal matrix.
   */

  template <typename MatrixType>
  inline void matrix_set_value(const MatrixType& matrix, size_t row, size_t column, typename MatrixType::value_type value)
  {
    assert (false);
  }

  /**
   * Free function to permute two rows of a permuted matrix.
   *
   * @param matrix The permuted matrix
   * @param index1 First index
   * @param index2 Second index
   */

  template <typename MatrixType>
  inline void matrix_permute1(MatrixType& matrix, size_t index1, size_t index2)
  {
    for (size_t index = 0; index < matrix.size2(); ++index)
    {
      std::swap(matrix(index1, index), matrix(index2, index));
    }
  }

  /**
   * Free function to permute two columns of a permuted matrix.
   *
   * @param matrix The permuted matrix
   * @param index1 First index
   * @param index2 Second index
   */

  template <typename MatrixType>
  inline void matrix_permute2(MatrixType& matrix, size_t index1, size_t index2)
  {
    for (size_t index = 0; index < matrix.size1(); ++index)
    {
      std::swap(matrix(index, index1), matrix(index, index2));
    }
  }

  /**
   * Free function to perform a binary pivot on a matrix.
   *
   * @param matrix The matrix
   * @param i Row index
   * @param j Column index
   */

  template <typename MatrixType>
  void matrix_binary_pivot(MatrixType& matrix, size_t i, size_t j)
  {
    typedef typename MatrixType::value_type value_type;
    const value_type& base_value = matrix(i, j);

    if (base_value == 0)
    {
      throw matrix_binary_pivot_exception();
    }

    for (size_t row = 0; row < matrix.size1(); ++row)
    {
      if (row == i)
      {
        continue;
      }
      const value_type& first = matrix(row, j);
      if (first == 0)
      {
        continue;
      }

      for (size_t column = 0; column < matrix.size2(); ++column)
      {
        if (column == j)
        {
          continue;
        }
        const value_type& second = matrix(i, column);
        if (second == 0)
        {
          continue;
        }
        matrix(row, column) = 1 - matrix(row, column);
      }
    }
  }

  /**
   * Free function to perform a ternary pivot on a matrix.
   *
   * @param matrix The matrix
   * @param i Row index
   * @param j Column index
   */

  template <typename MatrixType>
  void matrix_ternary_pivot(MatrixType& matrix, size_t i, size_t j)
  {
    typedef typename MatrixType::value_type value_type;
    const value_type& base_value = matrix(i, j);

    if (base_value == 0)
    {
      throw matrix_binary_pivot_exception();
    }

    for (size_t row = 0; row < matrix.size1(); ++row)
    {
      if (row == i)
      {
        continue;
      }
      const value_type& first = matrix(row, j);
      if (first == 0)
      {
        continue;
      }

      for (size_t column = 0; column < matrix.size2(); ++column)
      {
        if (column == j)
        {
          continue;
        }
        const value_type& second = matrix(i, column);
        if (second == 0)
        {
          continue;
        }
        value_type value = matrix(row, column) - first * second / base_value;
        while (value > 1)
          value -= 3;
        while (value < -1)
          value += 3;
        matrix(row, column) = value;
      }
    }
  }

  /**
   * Counts how many of the rows in the specified range have a property.
   *
   * @param matrix The given matrix
   * @param row_first First row in range
   * @param row_beyond Beyond row in range
   * @param column_first First column for property check
   * @param column_beyond Beyond column for property check
   * @param check Property checking routine
   * @return Number of rows with the property
   */

  template <typename MatrixType, typename PropertyCheck>
  inline size_t matrix_count_property_row_series(const MatrixType& matrix, size_t row_first, size_t row_beyond, size_t column_first,
      size_t column_beyond, PropertyCheck check)
  {
    for (size_t row = row_first; row < row_beyond; ++row)
    {
      for (size_t column = column_first; column < column_beyond; ++column)
        check(matrix(row, column));
      if (!check())
        return row - row_first;
    }
    return row_beyond - row_first;
  }

  /**
   * Counts how many of the columns in the specified range have a property.
   *
   * @param matrix The given matrix
   * @param row_first First column for property check
   * @param row_beyond Beyond column for property check
   * @param column_first First row in range
   * @param column_beyond Beyond row in range
   * @param check Property checking routine
   * @return Number of rows with the property
   */

  template <typename MatrixType, typename PropertyCheck>
  inline size_t matrix_count_property_column_series(const MatrixType& matrix, size_t row_first, size_t row_beyond, size_t column_first,
      size_t column_beyond, PropertyCheck check)
  {
    matrix_transposed <const MatrixType> transposed(matrix);
    return matrix_count_property_row_series(transposed, column_first, column_beyond, row_first, row_beyond, check);
  }

  /**
   * Prints a matrix
   *
   * @param matrix The matrix to be printed
   */

  template <typename MatrixType>
  inline void matrix_print(const MatrixType& matrix)
  {
    for (size_t row = 0; row < matrix.size1(); ++row)
    {
      for (size_t column = 0; column < matrix.size2(); ++column)
      {
        std::cout << " " << std::setw(2) << matrix(row, column);
      }
      std::cout << "\n";
    }
    std::cout << std::flush;
  }

  /**
   * Tests two matrices for exact equality.
   *
   * @param matrix1 First matrix
   * @param matrix2 Second matrix
   * @return true if and only if all entries match
   */

  template <typename MatrixType1, typename MatrixType2>
  inline bool matrix_equals(const MatrixType1& matrix1, const MatrixType2& matrix2)
  {
    if (matrix1.size1() != matrix2.size1())
      return false;
    if (matrix1.size2() != matrix2.size2())
      return false;

    for (size_t row = 0; row < matrix1.size1(); ++row)
    {
      for (size_t column = 0; column < matrix1.size2(); ++column)
      {
        if (matrix1(row, column) != matrix2(row, column))
          return false;
      }
    }
    return true;
  }

  template <typename Matrix>
  bool matrix_row_zero(const Matrix& matrix, size_t row, size_t column_first, size_t column_beyond)
  {
    for (size_t c = column_first; c != column_beyond; ++c)
      if (matrix(row, c) != 0)
        return false;
    return true;
  }

  template <typename Matrix>
  bool matrix_column_zero(const Matrix& matrix, size_t column, size_t row_first, size_t row_beyond)
  {
    return matrix_row_zero(make_transposed_matrix(matrix), column, row_first, row_beyond);
  }

  template <typename Matrix>
  bool find_smallest_nonzero_matrix_entry(const Matrix& matrix, size_t row_first, size_t row_beyond, size_t column_first, size_t column_beyond,
      size_t& row, size_t& column)
  {
    bool result = false;
    int current_value = 0;
    for (size_t r = row_first; r != row_beyond; ++r)
    {
      for (size_t c = column_first; c != column_beyond; ++c)
      {
        int value = matrix(r, c);
        if (value == 0)
          continue;

        value = value >= 0 ? value : -value;

        if (!result || value < current_value)
        {
          result = true;
          row = r;
          column = c;
          current_value = value;
        }
      }
    }
    return result;
  }

  template <typename Matrix1, typename Matrix2>
  bool equals(const Matrix1& first, const Matrix2& second)
  {
    if (first.size1() != second.size1())
      return false;
    if (first.size2() != second.size2())
      return false;
    for (size_t r = 0; r < first.size1(); ++r)
    {
      for (size_t c = 0; c < first.size2(); ++c)
      {
        if (first(r, c) != second(r, c))
          return false;
      }
    }
    return true;
  }
}

#endif /* MATRIX_HPP_ */
