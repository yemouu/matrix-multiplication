#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// void initialize_flat_matrix(double *matrix, size_t rows, size_t columns) {}
// void randomize_flat_matrix(double *matrix, size_t rows, size_t columns) {}
// void print_flat_matrix(int **matrix) {}

int main(int argc, char **argv) {
  if (argc > 4) {
    fprintf(stderr, "Error: Program accepts up to 3 arguments\n");
    exit(1);
  }

  size_t mnk[3] = {512, 512, 512};

  for (int i = 1; i < argc; i++) {
    char *junk;
    const long num = strtol(argv[i], &junk, 10);

    if (num <= 0) {
      fprintf(stderr, "Error: Number should be greater than 0\n");
      exit(1);
    }

    if (errno == ERANGE) {
      fprintf(stderr, "Error: Number was too large\n");
      exit(1);
    }

    if (*junk != '\0') {
      fprintf(stderr, "Error: Number had invalid characters: %s\n", junk);
      exit(1);
    }

    mnk[i - 1] = (size_t)num;
  }

  const size_t m = mnk[0];
  const size_t n = mnk[1];
  const size_t k = mnk[2];

  printf("M: %zu, N: %zu, K: %zu\n", m, n, k);

  int **matrix_a, **matrix_b, **matrix_c = NULL;

  initialize_matrix(&matrix_a, m, k);
  initialize_matrix(&matrix_b, k, n);
  initialize_matrix(&matrix_c, m, n);

  srand(time(NULL));
  randomize_matrix(matrix_a, m, k);
  randomize_matrix(matrix_b, k, n);

  printf("Matrix A:\n");
  print_matrix(matrix_a, m, k);
  printf("\n");

  printf("Matrix B:\n");
  print_matrix(matrix_b, k, n);
  printf("\n");

  multiply_matrices(matrix_a, m, matrix_b, n, k, matrix_c);

  printf("Matrix C:\n");
  print_matrix(matrix_c, m, n);
  printf("\n");

  free_matrix(matrix_a, m);
  free_matrix(matrix_b, k);
  free_matrix(matrix_c, m);

  return 0;
}
