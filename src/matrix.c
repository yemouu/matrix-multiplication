#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

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

void print_matrix(Matrix *matrix) {
  for (size_t row = 0; row < matrix->rows; row++) {
    for (size_t column = 0; column < matrix->columns; column++)
      printf("%d\t", matrix->values[row * matrix->columns + column]);

    printf("\n");
  }
}

void multiply_matrices(Matrix *a, Matrix *b, Matrix *c) {
  for (size_t lane = 0; lane < a->columns; lane++)
    for (size_t row = 0; row < a->rows; row++)
      for (size_t column = 0; column < b->columns; column++)
        c->values[row * b->columns + column] +=
            a->values[row * a->columns + lane] * b->values[lane * b->columns + column];
}

bool compare_matrices(Matrix *a, Matrix *b) {
  if ((a->rows != b->rows) || (a->columns != b->columns))
    return false;

  for (size_t index = 0; index < (a->rows * a->columns); index++)
    if (a->values[index] != b->values[index])
      return false;

  return true;
};

void free_matrix(Matrix *matrix) {
  if (matrix->values == NULL)
    return;

  free(matrix->values);
}
