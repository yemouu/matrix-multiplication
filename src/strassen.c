#include <stddef.h>

#include "matrix.h"

void matrix_add(int **matrix_a, size_t offset_a, size_t voffset_a, int **matrix_b, size_t offset_b, size_t voffset_b,
                int **matrix_c, size_t offset_c, size_t voffset_c, size_t size) {
  for (size_t row = 0; row < size; row++)
    for (size_t column = 0; column < size; column++)
      matrix_c[offset_c + row][voffset_c + column] =
          matrix_a[offset_a + row][voffset_a + column] + matrix_b[offset_b + row][voffset_b + column];
}

void matrix_subtract(int **matrix_a, size_t offset_a, size_t voffset_a, int **matrix_b, size_t offset_b,
                     size_t voffset_b, int **matrix_c, size_t offset_c, size_t voffset_c, size_t size) {
  for (size_t row = 0; row < size; row++)
    for (size_t column = 0; column < size; column++)
      matrix_c[offset_c + row][voffset_c + column] =
          matrix_a[offset_a + row][voffset_a + column] - matrix_b[offset_b + row][voffset_b + column];
}

void strassen_multiply_matrices(int **matrix_a, size_t offset_a, size_t voffset_a, int **matrix_b, size_t offset_b,
                                size_t voffset_b, int **matrix_c, size_t offset_c, size_t voffset_c) {

  const int a11 = matrix_a[offset_a][voffset_a];
  const int a12 = matrix_a[offset_a][voffset_a + 1];
  const int a21 = matrix_a[offset_a + 1][voffset_a];
  const int a22 = matrix_a[offset_a + 1][voffset_a + 1];

  const int b11 = matrix_b[offset_b][voffset_b];
  const int b12 = matrix_b[offset_b][voffset_b + 1];
  const int b21 = matrix_b[offset_b + 1][voffset_b];
  const int b22 = matrix_b[offset_b + 1][voffset_b + 1];

  const int m1 = (a11 + a22) * (b11 + b22);
  const int m2 = (a21 + a22) * b11;
  const int m3 = a11 * (b12 - b22);
  const int m4 = a22 * (b21 - b11);
  const int m5 = (a11 + a12) * b22;
  const int m6 = (a21 - a11) * (b11 + b12);
  const int m7 = (a12 - a22) * (b21 + b22);

  matrix_c[offset_c][voffset_c] = m1 + m4 - m5 + m7;
  matrix_c[offset_c][voffset_c + 1] = m3 + m5;
  matrix_c[offset_c + 1][voffset_c] = m2 + m4;
  matrix_c[offset_c + 1][voffset_c + 1] = m1 - m2 + m3 + m6;
}

void _strassen_algorithm(int **matrix_a, size_t offset_a, size_t voffset_a, int **matrix_b, size_t offset_b,
                         size_t voffset_b, int **matrix_c, size_t offset_c, size_t voffset_c, size_t size) {
  if (size == 2) {
    strassen_multiply_matrices(matrix_a, offset_a, voffset_a, matrix_b, offset_b, voffset_b, matrix_c, offset_c,
                               voffset_c);
    return;
  }

  const size_t new_size = size / 2;

  const size_t new_offset_a = offset_a + new_size;
  const size_t new_voffset_a = voffset_a + new_size;

  const size_t new_offset_b = offset_b + new_size;
  const size_t new_voffset_b = voffset_b + new_size;

  const size_t new_offset_c = offset_c + new_size;
  const size_t new_voffset_c = voffset_c + new_size;

  // a11 = matrix_a offset     voffset     size
  // a12 = matrix_a offset     new_voffset size
  // a21 = matrix_a new_offset voffset     size
  // a22 = matrix_a new_offset new_voffset size

  // b11 = matrix_b offset     voffset     size
  // b12 = matrix_b offset     new_voffset size
  // b21 = matrix_b new_offset voffset     size
  // b22 = matrix_b new_offset new_voffset size

  // c11 = matrix_c offset     voffset     size
  // c12 = matrix_c offset     new_voffset size
  // c21 = matrix_c new_offset voffset     size
  // c22 = matrix_c new_offset new_voffset size

  // TODO: Try and figure out a way to do this with less memory allocations
  // const int m1 = (a11 + a22) * (b11 + b22);
  // const int m2 = (a21 + a22) * b11;
  // const int m3 = a11 * (b12 - b22);
  // const int m4 = a22 * (b21 - b11);
  // const int m5 = (a11 + a12) * b22;
  // const int m6 = (a21 - a11) * (b11 + b12);
  // const int m7 = (a12 - a22) * (b21 + b22);

  // These are used as temporary matricies for the addition and subtraction we do
  int **left = NULL, **right = NULL;
  int **m1 = NULL, **m2 = NULL, **m3 = NULL, **m4 = NULL, **m5 = NULL, **m6 = NULL, **m7 = NULL;

  initialize_matrix(&left, new_size, new_size);
  initialize_matrix(&right, new_size, new_size);

  // TODO: make simplified functions for addition/subtraction/multiplication that assume the offset is zero
  initialize_matrix(&m1, new_size, new_size);
  matrix_add(matrix_a, offset_a, voffset_a, matrix_a, new_offset_a, new_voffset_a, left, 0, 0, new_size);
  matrix_add(matrix_b, offset_b, voffset_b, matrix_b, new_offset_b, new_voffset_b, right, 0, 0, new_size);
  _strassen_algorithm(left, 0, 0, right, 0, 0, m1, 0, 0, new_size);

  initialize_matrix(&m2, new_size, new_size);
  matrix_add(matrix_a, new_offset_a, offset_a, matrix_a, new_offset_a, new_voffset_a, left, 0, 0, new_size);
  _strassen_algorithm(left, 0, 0, matrix_b, offset_b, voffset_b, m2, 0, 0, new_size);

  initialize_matrix(&m3, new_size, new_size);
  matrix_subtract(matrix_b, offset_b, new_voffset_b, matrix_b, new_offset_b, new_voffset_b, right, 0, 0, new_size);
  _strassen_algorithm(matrix_a, offset_a, voffset_a, right, 0, 0, m3, 0, 0, new_size);

  initialize_matrix(&m4, new_size, new_size);
  matrix_subtract(matrix_b, new_offset_b, voffset_b, matrix_b, offset_b, voffset_b, right, 0, 0, new_size);
  _strassen_algorithm(matrix_a, new_offset_a, new_voffset_a, right, 0, 0, m4, 0, 0, new_size);

  initialize_matrix(&m5, new_size, new_size);
  matrix_add(matrix_a, offset_a, voffset_a, matrix_a, offset_a, new_voffset_a, left, 0, 0, new_size);
  _strassen_algorithm(left, 0, 0, matrix_b, new_offset_b, new_voffset_b, m5, 0, 0, new_size);

  initialize_matrix(&m6, new_size, new_size);
  matrix_subtract(matrix_a, new_offset_a, voffset_a, matrix_a, offset_a, voffset_a, left, 0, 0, new_size);
  matrix_add(matrix_b, offset_b, voffset_b, matrix_b, offset_b, new_voffset_b, right, 0, 0, new_size);
  _strassen_algorithm(left, 0, 0, right, 0, 0, m6, 0, 0, new_size);

  initialize_matrix(&m7, new_size, new_size);
  matrix_subtract(matrix_a, offset_a, new_voffset_a, matrix_a, new_offset_a, new_voffset_a, left, 0, 0, new_size);
  matrix_add(matrix_b, new_offset_b, voffset_b, matrix_b, new_offset_b, new_voffset_b, right, 0, 0, new_size);
  _strassen_algorithm(left, 0, 0, right, 0, 0, m7, 0, 0, new_size);

  matrix_add(m1, 0, 0, m4, 0, 0, left, 0, 0, new_size);
  matrix_subtract(left, 0, 0, m5, 0, 0, right, 0, 0, new_size);
  matrix_add(right, 0, 0, m7, 0, 0, matrix_c, offset_c, voffset_c, new_size);

  matrix_add(m3, 0, 0, m5, 0, 0, matrix_c, offset_c, new_voffset_c, new_size);

  matrix_add(m2, 0, 0, m4, 0, 0, matrix_c, new_offset_c, voffset_c, new_size);

  matrix_subtract(m1, 0, 0, m2, 0, 0, left, 0, 0, new_size);
  matrix_add(left, 0, 0, m3, 0, 0, right, 0, 0, new_size);
  matrix_add(right, 0, 0, m6, 0, 0, matrix_c, new_offset_c, new_voffset_c, new_size);

  free_matrix(left, new_size);
  free_matrix(right, new_size);
  free_matrix(m1, new_size);
  free_matrix(m2, new_size);
  free_matrix(m3, new_size);
  free_matrix(m4, new_size);
  free_matrix(m5, new_size);
  free_matrix(m6, new_size);
  free_matrix(m7, new_size);
}

void strassen_algorithm(int **matrix_a, int **matrix_b, int **matrix_c, size_t size) {
  _strassen_algorithm(matrix_a, 0, 0, matrix_b, 0, 0, matrix_c, 0, 0, size);
}
