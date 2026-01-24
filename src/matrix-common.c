#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix-common.h"

#define MATRIX_MIN_VALUE 1
#define MATRIX_MAX_VALUE 100


Matrix initialize_matrix(size_t rows, size_t columns) {
  Matrix matrix = {.values = (int *)malloc(rows * columns * sizeof(int)), .rows = rows, .columns = columns};

  if (matrix.values == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory");
    exit(1);
  }

  return matrix;
}

void randomize_matrix(Matrix *matrix) {
  for (size_t index = 0; index < matrix->rows * matrix->columns; index++)
    matrix->values[index] = rand() % (MATRIX_MAX_VALUE + 1) + MATRIX_MIN_VALUE;
}

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

void free_matrix(Matrix *matrix) {
  if (matrix->values == NULL)
    return;

  free(matrix->values);
}

Matrix *matrix_add(Matrix *a, Matrix *b, Matrix *c) {
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

  return c;
}

Matrix *matrix_subtract(Matrix *a, Matrix *b, Matrix *c) {
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
  return c;
}

void print_matrix(Matrix *matrix) {
  for (size_t row = 0; row < matrix->rows; row++) {
    for (size_t column = 0; column < matrix->columns; column++)
      printf("%d\t", matrix->values[row * matrix->columns + column]);

    printf("\n");
  }
}

bool compare_matrices(Matrix *a, Matrix *b) {
  if ((a->rows != b->rows) || (a->columns != b->columns))
    return false;

  for (size_t index = 0; index < (a->rows * a->columns); index++)
    if (a->values[index] != b->values[index])
      return false;

  return true;
};
