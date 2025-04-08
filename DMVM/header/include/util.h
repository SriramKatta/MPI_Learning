/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#ifndef __UTIL_H_
#define __UTIL_H_
#define HLINE                                                                  \
  "--------------------------------------------------------------------------" \
  "--\n"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef ABS
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#endif

inline size_t chunkstart(size_t N, int rank, int size) {
  return rank * N / size + MIN(N % size, rank);
}

inline size_t rowsinrank(size_t N, int rank, int size) {
  return N / size + ((N % size > rank) ? 1 : 0);
}

void matDataToRankFile(double *A, int rows, int cols, int rank) {
  char fname[20];
  snprintf(fname, 20, "data_mat_%d.txt", rank);
  FILE *fptr = fopen(fname, "w");
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      fprintf(fptr, " %3.2lf ", A[i * cols + j]);
    }
    fprintf(fptr, "\n");
  }
}

void vecDataToRankFile(double *V, int rows, int rank) {
  char fname[20];
  snprintf(fname, 20, "data_vec_%d.txt", rank);
  FILE *fptr = fopen(fname, "w");
  for (size_t i = 0; i < rows; i++) {
    fprintf(fptr, "%3.2lf\n", V[i]);
  }
}

#endif // __UTIL_H_
