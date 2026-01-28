#include <stddef.h>

#include "matrix-common.h"

void base_coppersmith_winograd(Matrix *a, Matrix *b, Matrix *c) {
  const int a11 = a->values[(a->row_offset) * a->columns + (a->column_offset)];
  const int a12 = a->values[(a->row_offset) * a->columns + (a->column_offset + 1)];
  const int a21 = a->values[(a->row_offset + 1) * a->columns + (a->column_offset)];
  const int a22 = a->values[(a->row_offset + 1) * a->columns + (a->column_offset + 1)];

  const int b11 = b->values[(b->row_offset) * b->columns + (b->column_offset)];
  const int b12 = b->values[(b->row_offset) * b->columns + (b->column_offset + 1)];
  const int b21 = b->values[(b->row_offset + 1) * b->columns + (b->column_offset)];
  const int b22 = b->values[(b->row_offset + 1) * b->columns + (b->column_offset + 1)];

  const int s1 = a21 + a22;
  const int s2 = s1 - a11;
  const int s3 = a11 - a21;
  const int s4 = a12 - s2;

  const int t1 = b12 - b11;
  const int t2 = b22 - t1;
  const int t3 = b22 - b12;
  const int t4 = t2 - b21;

  const int m1 = a11 * b11;
  const int m2 = a12 * b21;
  const int m3 = s4 * b22;
  const int m4 = a22 * t4;
  const int m5 = s1 * t1;
  const int m6 = s2 * t2;
  const int m7 = s3 * t3;

  const int u1 = m1 + m2;
  const int u2 = m1 + m6;
  const int u3 = u2 + m7;
  const int u4 = u2 + m5;
  const int u5 = u4 + m3;
  const int u6 = u3 - m4;
  const int u7 = u3 + m5;

  c->values[(c->row_offset) * c->columns + (c->column_offset)] = u1;
  c->values[(c->row_offset) * c->columns + (c->column_offset + 1)] = u5;
  c->values[(c->row_offset + 1) * c->columns + (c->column_offset)] = u6;
  c->values[(c->row_offset + 1) * c->columns + (c->column_offset + 1)] = u7;
}

void coppersmith_winograd(Matrix *a, Matrix *b, Matrix *c) {
  if (c->rows == 2 || c->row_offset_size == 2) {
    base_coppersmith_winograd(a, b, c);
    return;
  }

  const size_t size = (c->row_offset_size != 0) ? (c->row_offset_size / 2) : (c->rows / 2);

  Matrix a11 = create_matrix_offset(a, a->row_offset, a->column_offset, size);
  Matrix a12 = create_matrix_offset(a, a->row_offset, a->column_offset + size, size);
  Matrix a21 = create_matrix_offset(a, a->row_offset + size, a->column_offset, size);
  Matrix a22 = create_matrix_offset(a, a->row_offset + size, a->column_offset + size, size);

  Matrix b11 = create_matrix_offset(b, b->row_offset, b->column_offset, size);
  Matrix b12 = create_matrix_offset(b, b->row_offset, b->column_offset + size, size);
  Matrix b21 = create_matrix_offset(b, b->row_offset + size, b->column_offset, size);
  Matrix b22 = create_matrix_offset(b, b->row_offset + size, b->column_offset + size, size);

  Matrix c11 = create_matrix_offset(c, c->row_offset, c->column_offset, size);
  Matrix c12 = create_matrix_offset(c, c->row_offset, c->column_offset + size, size);
  Matrix c21 = create_matrix_offset(c, c->row_offset + size, c->column_offset, size);
  Matrix c22 = create_matrix_offset(c, c->row_offset + size, c->column_offset + size, size);

  Matrix temp = initialize_matrix(size, 18 * size);
  Matrix s1 = create_matrix_offset(&temp, 0, 0, size);
  Matrix s2 = create_matrix_offset(&temp, 0, size, size);
  Matrix s3 = create_matrix_offset(&temp, 0, 2 * size, size);
  Matrix s4 = create_matrix_offset(&temp, 0, 3 * size, size);

  Matrix t1 = create_matrix_offset(&temp, 0, 4 * size, size);
  Matrix t2 = create_matrix_offset(&temp, 0, 5 * size, size);
  Matrix t3 = create_matrix_offset(&temp, 0, 6 * size, size);
  Matrix t4 = create_matrix_offset(&temp, 0, 7 * size, size);

  Matrix m1 = create_matrix_offset(&temp, 0, 8 * size, size);
  Matrix m2 = create_matrix_offset(&temp, 0, 9 * size, size);
  Matrix m3 = create_matrix_offset(&temp, 0, 10 * size, size);
  Matrix m4 = create_matrix_offset(&temp, 0, 11 * size, size);
  Matrix m5 = create_matrix_offset(&temp, 0, 12 * size, size);
  Matrix m6 = create_matrix_offset(&temp, 0, 13 * size, size);
  Matrix m7 = create_matrix_offset(&temp, 0, 14 * size, size);

  Matrix u2 = create_matrix_offset(&temp, 0, 15 * size, size);
  Matrix u3 = create_matrix_offset(&temp, 0, 16 * size, size);
  Matrix u4 = create_matrix_offset(&temp, 0, 17 * size, size);

  // Calculate s1-s4
  matrix_add(&a21, &a22, &s1);
  matrix_subtract(&s1, &a11, &s2);
  matrix_subtract(&a11, &a21, &s3);
  matrix_subtract(&a12, &s2, &s4);

  // Calculate t1-t4
  matrix_subtract(&b12, &b11, &t1);
  matrix_subtract(&b22, &t1, &t2);
  matrix_subtract(&b22, &b12, &t3);
  matrix_subtract(&t2, &b21, &t4);

  // Calculate m1-m7
  coppersmith_winograd(&a11, &b11, &m1);
  coppersmith_winograd(&a12, &b21, &m2);
  coppersmith_winograd(&s4, &b22, &m3);
  coppersmith_winograd(&a22, &t4, &m4);
  coppersmith_winograd(&s1, &t1, &m5);
  coppersmith_winograd(&s2, &t2, &m6);
  coppersmith_winograd(&s3, &t3, &m7);

  // Calculate u1-u7 and assign to c
  matrix_add(&m1, &m2, &c11);
  matrix_add(&m1, &m6, &u2);
  matrix_add(&u2, &m7, &u3);
  matrix_add(&u2, &m5, &u4);
  matrix_add(&u4, &m3, &c12);
  matrix_subtract(&u3, &m4, &c21);
  matrix_add(&u3, &m5, &c22);

  free_matrix(&temp);
}
