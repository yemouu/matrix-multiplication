#ifndef matrix_h_INCLUDE
#define matrix_h_INCLUDE

#include <stdbool.h>
#include <stddef.h>

void initialize_matrix(int ***matrix, size_t rows, size_t columns);
void randomize_matrix(int **matrix, size_t rows, size_t columns);
void free_matrix(int **matrix, size_t rows);

void multiply_matrices(int **matrix_a, size_t a_rows, int **matrix_b, size_t b_columns, size_t shared, int **result);

bool compare_matrices(int **matrix_a, int **matrix_b, size_t rows, size_t columns);

void print_matrix(int **matrix, size_t rows, size_t columns);

#endif // matrix_h_INCLUDE
