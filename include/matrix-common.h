#ifndef matrix_common_h_INCLUDED
#define matrix_common_h_INCLUDED

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
Matrix create_matrix_offset(Matrix *matrix, size_t row_offset, size_t column_offset, size_t size);
void free_matrix(Matrix *matrix);

Matrix *matrix_add(Matrix *a, Matrix *b, Matrix *c);
Matrix *matrix_subtract(Matrix *a, Matrix *b, Matrix *c);

bool compare_matrices(Matrix *a, Matrix *b);
void print_matrix(Matrix *matrix);

#endif // matrix_common_h_INCLUDED
