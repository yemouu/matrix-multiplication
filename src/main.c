#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "coppersmith-winograd.h"
#include "matrix-common.h"
#include "matrix.h"
#include "strassen.h"

typedef enum { GEMM, STRASSEN, COPPERSMITH_WINOGRAD } algorithm;

void usage_print(int exit_status) {
  const char *usage =
      "usage: matrix-multiplication [options]\n\n"
      "\t-a <algorithm> - select which algorithm to use (gemm [default], strassen, or coppersmith_winograd)\n"
      "\t-d             - enable debug prints\n"
      "\t-h             - display this message\n"
      "\t-s <integer>   - set a value for m n and k (creates a square matrix)\n"
      "\t-k <integer>   - set a value for k (determines the size of the matrix)\n"
      "\t-m <integer>   - set a value for m (determines the size of the matrix)\n"
      "\t-n <integer>   - set a value for n (determines the size of the matrix)\n";

  if (exit_status == 0) {
    printf("%s\n", usage);
    exit(exit_status);
  } else {
    fprintf(stderr, "%s\n", usage);
    exit(exit_status);
  }
}

size_t get_num_from_optarg(const char *opt_string) {
  char *junk;
  const long num = strtol(opt_string, &junk, 10);

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

  return (size_t)num;
}

int main(int argc, char **argv) {
  algorithm algo = GEMM;
  bool debug = false;
  size_t m = 512, n = 512, k = 512;

  int opt;
  while ((opt = getopt(argc, argv, ":a:dhk:m:n:s:")) != -1) {
    switch (opt) {
    case 'a':
      if (strcmp(optarg, "gemm") == 0)
        algo = GEMM;
      else if (strcmp(optarg, "strassen") == 0)
        algo = STRASSEN;
      else if (strcmp(optarg, "coppersmith_winograd") == 0)
        algo = COPPERSMITH_WINOGRAD;
      else {
        fprintf(stderr, "Option '-a' requires the operand be either of: gemm, strassen, or coppersmith_winograd\n");
        usage_print(1);
      }
      break;
    case 'd':
      debug = true;
      break;
    case 'h':
      usage_print(0);
      break;
    case 'k':
      k = get_num_from_optarg(optarg);
      break;
    case 'm':
      m = get_num_from_optarg(optarg);
      break;
    case 'n':
      n = get_num_from_optarg(optarg);
      break;
    case 's':
      k = get_num_from_optarg(optarg);
      m = k;
      n = k;
      break;
    case ':':
      fprintf(stderr, "Option -%c requires an operand\n", optopt);
      usage_print(1);
      break;
    case '?':
      fprintf(stderr, "Unknown option: '-%c'\n", optopt);
      usage_print(1);
      break;
    }
  }

  if (debug) {
    printf("Algorithm: ");
    switch (algo) {
    case GEMM:
      printf("gemm\n");
      break;
    case STRASSEN:
      printf("strassen\n");
      break;
    case COPPERSMITH_WINOGRAD:
      printf("coppersmith_winograd\n");
      break;
    };

    printf("debug: %d\n", debug);
    printf("M: %zu, N: %zu, K: %zu\n\n", m, n, k);
  }

  Matrix a, b, c, d;
  a = initialize_matrix(m, k);
  b = initialize_matrix(k, n);
  c = initialize_matrix(m, n);

  if (algo != GEMM)
    d = initialize_matrix(m, n);

  srand(time(NULL));
  randomize_matrix(&a);
  randomize_matrix(&b);

  if (debug) {
    printf("Matrix A:\n");
    print_matrix(&a);
    printf("\n");

    printf("Matrix B:\n");
    print_matrix(&b);
    printf("\n");
  }

  clock_t begin, end;
  switch (algo) {
  case GEMM:
    begin = clock();
    gemm(&a, &b, &c);
    end = clock();
    break;
  case STRASSEN:
    if ((m != n) || (m != k) || ((m & (m - 1)) != 0)) {
      fprintf(stderr, "For the Strassen algorithm, the matrices should be the"
                      "same size and m, n, and k should be a power of 2.\n");
      return 1;
    }

    begin = clock();
    strassen(&a, &b, &d);
    end = clock();
    break;
  case COPPERSMITH_WINOGRAD:
    if ((m != n) || (m != k) || ((m & (m - 1)) != 0)) {
      fprintf(stderr, "For the Coppersmith-Winograd algorithm, the matrices should be the"
                      "same size and m, n, and k should be a power of 2.\n");
      return 1;
    }

    begin = clock();
    coppersmith_winograd(&a, &b, &d);
    end = clock();
    break;
  }

  const double elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Elapsed Time: %fs\n", elapsed_time);

  if (debug) {
    if (algo != GEMM)
      gemm(&a, &b, &c);

    printf("Matrix C:\n");
    print_matrix(&c);
    printf("\n");

    if (algo != GEMM) {
      printf("Matrix D:\n");
      print_matrix(&d);
      printf("\n");

      if (!compare_matrices(&c, &d)) {
        fprintf(stderr, "Error: Multiplication from alternative algorithm returned an unexpected result\n");
        return 1;
      }
    }
  }

  free_matrix(&a);
  free_matrix(&b);
  free_matrix(&c);

  if (algo != GEMM)
    free_matrix(&d);

  return 0;
}
