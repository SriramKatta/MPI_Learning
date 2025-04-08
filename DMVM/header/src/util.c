#include "util.h"

#include <stdio.h>
#include <stdlib.h>

inline size_t chunkstart(size_t N, int rank, int size) {
  return rank * (N / size) + MIN(N % size, rank);
}

inline size_t rowsinrank(size_t N, int rank, int size) {
  return N / size + ((N % size > rank) ? 1 : 0);
}

inline void matDataToRankFile(double *A, int rows, int cols, int rank) {
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

inline void vecDataToRankFile(double *V, int rows, int rank, char ch) {
  char fname[20];
  snprintf(fname, 20, "data_vec_%c_%d.txt", ch, rank);
  FILE *fptr = fopen(fname, "w");
  for (size_t i = 0; i < rows; i++) {
    fprintf(fptr, "%3.2lf\n", V[i]);
  }
}
