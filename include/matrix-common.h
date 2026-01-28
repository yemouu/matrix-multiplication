#ifndef matrix_common_h_INCLUDED
#define matrix_common_h_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *values;
  size_t rows;
  size_t columns;

  size_t row_offset;
  size_t row_offset_size;
  size_t column_offset;
  size_t column_offset_size;
} Matrix;

void randomize_matrix(Matrix *matrix);

bool compare_matrices(Matrix *a, Matrix *b);
void print_matrix(Matrix *matrix);

inline Matrix initialize_matrix(size_t rows, size_t columns) {
  Matrix matrix = {.values = (int *)malloc(rows * columns * sizeof(int)), .rows = rows, .columns = columns};

  if (matrix.values == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory");
    exit(1);
  }

  return matrix;
}

inline Matrix create_matrix_offset(Matrix *matrix, size_t row_offset, size_t column_offset, size_t size) {
  return (Matrix){.values = matrix->values,
                  .rows = matrix->rows,
                  .columns = matrix->columns,
                  .row_offset = row_offset,
                  .row_offset_size = size,
                  .column_offset = column_offset,
                  .column_offset_size = size};
}

inline void free_matrix(Matrix *matrix) {
  if (matrix->values == NULL)
    return;

  free(matrix->values);
}

inline void matrix_add(Matrix *a, Matrix *b, Matrix *c) {
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

inline void matrix_subtract(Matrix *a, Matrix *b, Matrix *c) {
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

#endif // matrix_common_h_INCLUDED
