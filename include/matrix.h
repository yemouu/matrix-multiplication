#ifndef matrix_h_INCLUDE
#define matrix_h_INCLUDE

#include <stddef.h>

void initialize_matrix(int ***matrix, size_t rows, size_t columns);

void randomize_matrix(int **matrix, size_t rows, size_t columns);

void multiply_matrices(int **matrix_a, size_t a_rows, int **matrix_b,
                       size_t b_columns, size_t shared, int **result);

void print_matrix(int **matrix, size_t rows, size_t columns);

void free_matrix(int **matrix, size_t rows);

#endif // matrix_h_INCLUDE
