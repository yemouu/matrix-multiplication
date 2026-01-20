#include <stdio.h>
#include <stdlib.h>

#define MATRIX_MIN_VALUE 1
#define MATRIX_MAX_VALUE 100

void initialize_matrix(int ***matrix, size_t rows, size_t columns) {
  *matrix = calloc(rows, sizeof(int *));
  if (*matrix == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory");
    exit(1);
  }

  for (size_t row = 0; row < rows; row++) {
    (*matrix)[row] = calloc(columns, sizeof(int));
    if ((*matrix)[row] == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory");
      exit(1);
    }

    for (size_t column = 0; column < columns; column++)
      (*matrix)[row][column] = 0;
  }
}

void randomize_matrix(int **matrix, size_t rows, size_t columns) {
  for (size_t row = 0; row < rows; row++)
    for (size_t column = 0; column < columns; column++)
      matrix[row][column] = rand() % (MATRIX_MAX_VALUE + 1) + MATRIX_MIN_VALUE;
}

void print_matrix(int **matrix, size_t rows, size_t columns) {
  for (size_t row = 0; row < rows; row++) {
    for (size_t column = 0; column < columns; column++) {
      printf("%d\t", matrix[row][column]);
    }

    printf("\n");
  }
}

void multiply_matrices(int **matrix_a, size_t a_rows, int **matrix_b,
                       size_t b_columns, size_t shared, int **result) {
  for (size_t s = 0; s < shared; s++)
    for (size_t row = 0; row < a_rows; row++)
      for (size_t column = 0; column < b_columns; column++)
        result[row][column] += matrix_a[row][s] * matrix_b[s][column];
}

void free_matrix(int **matrix, size_t rows) {
  if (matrix == NULL)
    return;

  for (size_t row = 0; row < rows; row++)
    free(matrix[row]);

  free(matrix);
}
