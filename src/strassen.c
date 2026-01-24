#include <stddef.h>

#include "matrix.h"

Matrix create_matrix_offset(Matrix *matrix, size_t row_offset, size_t column_offset, size_t size) {
  Matrix result = {.values = matrix->values,
                   .rows = matrix->rows,
                   .columns = matrix->columns,
                   .row_offset = row_offset,
                   .row_offset_size = size,
                   .column_offset = column_offset,
                   .column_offset_size = size};
  return result;
}

void matrix_add(Matrix *a, Matrix *b, Matrix *c) {
  size_t row_size = 0;
  size_t column_size = 0;

  if (a->row_offset_size != 0)
    row_size = a->row_offset_size;
  else if (b->row_offset_size != 0)
    row_size = b->row_offset_size;
  else if (c->row_offset_size != 0)
    row_size = c->row_offset_size;
  else
    row_size = c->rows;

  if (a->column_offset_size != 0)
    column_size = a->column_offset_size;
  else if (b->column_offset_size != 0)
    column_size = b->column_offset_size;
  else if (c->column_offset_size != 0)
    column_size = c->column_offset_size;
  else
    column_size = c->columns;

  for (size_t row = 0; row < row_size; row++)
    for (size_t column = 0; column < column_size; column++)
      c->values[(c->row_offset + row) * c->columns + (c->column_offset + column)] =
          a->values[(a->row_offset + row) * a->columns + (a->column_offset + column)] +
          b->values[(b->row_offset + row) * b->columns + (b->column_offset + column)];
}

void matrix_subtract(Matrix *a, Matrix *b, Matrix *c) {
  size_t row_size = 0;
  size_t column_size = 0;

  if (a->row_offset_size != 0)
    row_size = a->row_offset_size;
  else if (b->row_offset_size != 0)
    row_size = b->row_offset_size;
  else if (c->row_offset_size != 0)
    row_size = c->row_offset_size;
  else
    row_size = c->rows;

  if (a->column_offset_size != 0)
    column_size = a->column_offset_size;
  else if (b->column_offset_size != 0)
    column_size = b->column_offset_size;
  else if (c->column_offset_size != 0)
    column_size = c->column_offset_size;
  else
    column_size = c->columns;

  for (size_t row = 0; row < row_size; row++)
    for (size_t column = 0; column < column_size; column++)
      c->values[(c->row_offset + row) * c->columns + (c->column_offset + column)] =
          a->values[(a->row_offset + row) * a->columns + (a->column_offset + column)] -
          b->values[(b->row_offset + row) * b->columns + (b->column_offset + column)];
}

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

  // TODO: Try and figure out a way to do this with less memory allocations
  // const int m1 = (a11 + a22) * (b11 + b22);
  // const int m2 = (a21 + a22) * b11;
  // const int m3 = a11 * (b12 - b22);
  // const int m4 = a22 * (b21 - b11);
  // const int m5 = (a11 + a12) * b22;
  // const int m6 = (a21 - a11) * (b11 + b12);
  // const int m7 = (a12 - a22) * (b21 + b22);

  Matrix temp = initialize_matrix(size, 9 * size);
  Matrix left = create_matrix_offset(&temp, 0, 0, size);
  Matrix right = create_matrix_offset(&temp, 0, size, size);
  Matrix m1 = create_matrix_offset(&temp, 0, 2 * size, size);
  Matrix m2 = create_matrix_offset(&temp, 0, 3 * size, size);
  Matrix m3 = create_matrix_offset(&temp, 0, 4 * size, size);
  Matrix m4 = create_matrix_offset(&temp, 0, 5 * size, size);
  Matrix m5 = create_matrix_offset(&temp, 0, 6 * size, size);
  Matrix m6 = create_matrix_offset(&temp, 0, 7 * size, size);
  Matrix m7 = create_matrix_offset(&temp, 0, 8 * size, size);

  // TODO: Instead of doing the addition and subtraction outside of the, consider making the addition and subtraction
  //       functions return a pointer to the result
  matrix_add(&a11, &a22, &left);
  matrix_add(&b11, &b22, &right);
  strassen(&left, &right, &m1);

  matrix_add(&a21, &a22, &left);
  strassen(&left, &b11, &m2);

  matrix_subtract(&b12, &b22, &right);
  strassen(&a11, &right, &m3);

  matrix_subtract(&b21, &b11, &right);
  strassen(&a22, &right, &m4);

  matrix_add(&a11, &a12, &left);
  strassen(&left, &b22, &m5);

  matrix_subtract(&a21, &a11, &left);
  matrix_add(&b11, &b12, &right);
  strassen(&left, &right, &m6);

  matrix_subtract(&a12, &a22, &left);
  matrix_add(&b21, &b22, &right);
  strassen(&left, &right, &m7);

  // Put results into C
  matrix_add(&m1, &m4, &left);
  matrix_subtract(&left, &m5, &right);
  matrix_add(&right, &m7, &c11);

  matrix_add(&m3, &m5, &c12);

  matrix_add(&m2, &m4, &c21);

  matrix_subtract(&m1, &m2, &left);
  matrix_add(&left, &m3, &right);
  matrix_add(&right, &m6, &c22);

  free_matrix(&temp);
}
