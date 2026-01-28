#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix-common.h"

#define MATRIX_MIN_VALUE 1
#define MATRIX_MAX_VALUE 100

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

bool compare_matrices(Matrix *a, Matrix *b) {
  if ((a->rows != b->rows) || (a->columns != b->columns))
    return false;

  for (size_t index = 0; index < (a->rows * a->columns); index++)
    if (a->values[index] != b->values[index])
      return false;

  return true;
};
