#include <stddef.h>

#include "matrix-common.h"

void gemm(Matrix *a, Matrix *b, Matrix *c) {
  for (size_t row = 0; row < a->rows; row++)
    for (size_t lane = 0; lane < a->columns; lane++)
      for (size_t column = 0; column < b->columns; column++)
        c->values[row * b->columns + column] +=
            a->values[row * a->columns + lane] * b->values[lane * b->columns + column];
}

