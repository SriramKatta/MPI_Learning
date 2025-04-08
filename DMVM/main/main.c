/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "allocate.h"
#include "timing.h"
#include "util.h"

#include <mpi.h>

extern double dmvm(double *restrict y, const double *restrict a,
                   const double *restrict x, int N, int iter);

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank = 0, size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  size_t bytesPerWord = sizeof(double);
  size_t N = 0;
  size_t iter = 1;
  double *a, *x, *y;
  double t0, t1;
  double walltime;

  if (argc > 2) {
    N = atoi(argv[1]);
    iter = atoi(argv[2]);
  } else {
    printf("Usage: %s <N> <iter>\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  size_t localN = rowsinrank(N, rank, size);

  a = (double *)allocate(ARRAY_ALIGNMENT, localN * N * bytesPerWord);
  x = (double *)allocate(ARRAY_ALIGNMENT, localN * bytesPerWord);
  y = (double *)allocate(ARRAY_ALIGNMENT, localN * bytesPerWord);

  size_t globalchunkstart = chunkstart(N, rank, size);
  // initialize arrays
  for (int i = 0; i < localN; i++) {
    x[i] = (double)(i + globalchunkstart);
    y[i] = 0.0;

    for (int j = 0; j < N; j++) {
      a[i * N + j] = (double)j + (i + globalchunkstart);
    }
  }

  walltime = dmvm(y, a, x, N, iter);

  double flops = (double)2.0 * N * N * iter;
  // # iterations, problem size, flop rate, walltime
  if (0 == rank)
    printf("%zu %zu %.2f %.2f\n", iter, N, 1.0E-06 * flops / walltime,
           walltime);

#if 1
  matDataToRankFile(a, localN, N, rank);
  vecDataToRankFile(x, localN, rank, 'x');
  vecDataToRankFile(y, localN, rank, 'y');
#endif

  MPI_Finalize();
  return EXIT_SUCCESS;
}
