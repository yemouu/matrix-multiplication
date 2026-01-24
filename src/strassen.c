#include <stddef.h>

#include "matrix-common.h"

void base_strassen(Matrix *a, Matrix *b, Matrix *c) {
  const int a11 = a->values[(a->row_offset) * a->columns + (a->column_offset)];
  const int a12 = a->values[(a->row_offset) * a->columns + (a->column_offset + 1)];
  const int a21 = a->values[(a->row_offset + 1) * a->columns + (a->column_offset)];
  const int a22 = a->values[(a->row_offset + 1) * a->columns + (a->column_offset + 1)];

  const int b11 = b->values[(b->row_offset) * b->columns + (b->column_offset)];
  const int b12 = b->values[(b->row_offset) * b->columns + (b->column_offset + 1)];
  const int b21 = b->values[(b->row_offset + 1) * b->columns + (b->column_offset)];
  const int b22 = b->values[(b->row_offset + 1) * b->columns + (b->column_offset + 1)];

  const int m1 = (a11 + a22) * (b11 + b22);
  const int m2 = (a21 + a22) * b11;
  const int m3 = a11 * (b12 - b22);
  const int m4 = a22 * (b21 - b11);
  const int m5 = (a11 + a12) * b22;
  const int m6 = (a21 - a11) * (b11 + b12);
  const int m7 = (a12 - a22) * (b21 + b22);

  c->values[(c->row_offset) * c->columns + (c->column_offset)] = m1 + m4 - m5 + m7;
  c->values[(c->row_offset) * c->columns + (c->column_offset + 1)] = m3 + m5;
  c->values[(c->row_offset + 1) * c->columns + (c->column_offset)] = m2 + m4;
  c->values[(c->row_offset + 1) * c->columns + (c->column_offset + 1)] = m1 - m2 + m3 + m6;
}

void strassen(Matrix *a, Matrix *b, Matrix *c) {
  if (c->rows == 2 || c->row_offset_size == 2) {
    base_strassen(a, b, c);
    return;
  }

  size_t size;
  if (c->row_offset_size != 0)
    size = c->row_offset_size / 2;
  else
    size = c->rows / 2;

  Matrix a11 = create_matrix_offset(a, a->row_offset, a->column_offset, size);
  Matrix a12 = create_matrix_offset(a, a->row_offset, a->column_offset + size, size);
  Matrix a21 = create_matrix_offset(a, a->row_offset + size, a->column_offset, size);
  Matrix a22 = create_matrix_offset(a, a->row_offset + size, a->column_offset + size, size);

  Matrix b11 = create_matrix_offset(b, b->row_offset, b->column_offset, size);
  Matrix b12 = create_matrix_offset(b, b->row_offset, b->column_offset + size, size);
  Matrix b21 = create_matrix_offset(b, b->row_offset + size, b->column_offset, size);
  Matrix b22 = create_matrix_offset(b, b->row_offset + size, b->column_offset + size, size);

  Matrix c11 = create_matrix_offset(c, c->row_offset, c->column_offset, size);
  Matrix c12 = create_matrix_offset(c, c->row_offset, c->column_offset + size, size);
  Matrix c21 = create_matrix_offset(c, c->row_offset + size, c->column_offset, size);
  Matrix c22 = create_matrix_offset(c, c->row_offset + size, c->column_offset + size, size);

  Matrix temp = initialize_matrix(size, 9 * size);
  Matrix o1 = create_matrix_offset(&temp, 0, 0, size);
  Matrix o2 = create_matrix_offset(&temp, 0, size, size);
  Matrix m1 = create_matrix_offset(&temp, 0, 2 * size, size);
  Matrix m2 = create_matrix_offset(&temp, 0, 3 * size, size);
  Matrix m3 = create_matrix_offset(&temp, 0, 4 * size, size);
  Matrix m4 = create_matrix_offset(&temp, 0, 5 * size, size);
  Matrix m5 = create_matrix_offset(&temp, 0, 6 * size, size);
  Matrix m6 = create_matrix_offset(&temp, 0, 7 * size, size);
  Matrix m7 = create_matrix_offset(&temp, 0, 8 * size, size);

  // Calculate m1-m7
  strassen(matrix_add(&a11, &a22, &o1), matrix_add(&b11, &b22, &o2), &m1);
  strassen(matrix_add(&a21, &a22, &o1), &b11, &m2);
  strassen(&a11, matrix_subtract(&b12, &b22, &o2), &m3);
  strassen(&a22, matrix_subtract(&b21, &b11, &o2), &m4);
  strassen(matrix_add(&a11, &a12, &o1), &b22, &m5);
  strassen(matrix_subtract(&a21, &a11, &o1), matrix_add(&b11, &b12, &o2), &m6);
  strassen(matrix_subtract(&a12, &a22, &o1), matrix_add(&b21, &b22, &o2), &m7);

  // Calculate C11-C22
  matrix_add(matrix_subtract(matrix_add(&m1, &m4, &o1), &m5, &o2), &m7, &c11);
  matrix_add(&m3, &m5, &c12);
  matrix_add(&m2, &m4, &c21);
  matrix_add(matrix_add(matrix_subtract(&m1, &m2, &o1), &m3, &o2), &m6, &c22);

  free_matrix(&temp);
}
