#ifndef matrix_h_INCLUDE
#define matrix_h_INCLUDE

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  int *values;
  size_t rows;
  size_t columns;

  size_t row_offset;
  size_t row_offset_size;
  size_t column_offset;
  size_t column_offset_size;
} Matrix;

Matrix initialize_matrix(size_t rows, size_t columns);
void randomize_matrix(Matrix *matrix);
void free_matrix(Matrix *matrix);

void multiply_matrices(Matrix *a, Matrix *b, Matrix *c);

bool compare_matrices(Matrix *a, Matrix *b);

void print_matrix(Matrix *matrix);

#endif // matrix_h_INCLUDE
