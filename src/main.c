#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

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

  const clock_t begin = clock();
  multiply_matrices(matrix_a, m, matrix_b, n, k, matrix_c);
  const clock_t end = clock();

  const double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Matrix C:\n");
  print_matrix(matrix_c, m, n);
  printf("\n");

  printf("Elapsed Time: %fs\n", elapsed_time);

  free_matrix(matrix_a, m);
  free_matrix(matrix_b, k);
  free_matrix(matrix_c, m);

  return 0;
}
