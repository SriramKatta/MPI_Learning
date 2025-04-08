/*
 * Copyright (C) 2022 NHR@FAU, University Erlangen-Nuremberg.
 * All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file.
 */
#include "timing.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

double dmvm(double *restrict y, const double *restrict a,
            const double *restrict x, int N, int iter) {
  int rank = 0, size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int num = N / size;
  int rest = N % size;
  int uppernbr = (rank - 1) % size;
  if (uppernbr < 0) {
    uppernbr = size - 1;
  }

  int lowernbr = (rank + 1) % size;

  int cs = chunkstart(N, rank, size);
  int localN = rowsinrank(N, rank, size);
  int Ncurr = localN;

  double ts, te;
  ts = MPI_Wtime();
  for (int j = 0; j < iter; j++) {
    for (size_t rot = 0; rot < size; rot++) {
      // compute local section
      for (int r = 0; r < localN; r++) {
        for (int c = cs; c < cs + Ncurr; c++) {
          y[r] += a[r * N + c] * x[c - cs];
        }
      }
      cs += Ncurr;
      if (cs >= N)
        cs = 0;
      Ncurr = rowsinrank(N, uppernbr, size);
      // send the rhs vector and get the needed one
      MPI_Status state;
      if (0 == rank) {
        MPI_Send(x, num + (rest ? 1 : 0), MPI_DOUBLE, uppernbr, 0, MPI_COMM_WORLD);
        MPI_Recv(x, num + (rest ? 1 : 0), MPI_DOUBLE, lowernbr, 0, MPI_COMM_WORLD, &state);
      } else {
        MPI_Recv(x, num + (rest ? 1 : 0), MPI_DOUBLE, lowernbr, 0, MPI_COMM_WORLD, &state);
        MPI_Send(x, num + (rest ? 1 : 0), MPI_DOUBLE, uppernbr, 0, MPI_COMM_WORLD);
      }
      //if (rot != size -1 ) {
      //  MPI_Sendrecv_replace(x, num + (rest ? 1 : 0), MPI_DOUBLE, uppernbr, 0,
      //                       lowernbr, 0, MPI_COMM_WORLD, &state);
      //}
    }
  }
  te = MPI_Wtime();

  return te - ts;
}

#ifdef CHECK
{
  double sum = 0.0;

  for (int i = 0; i < N; i++) {
    sum += y[i];
    y[i] = 0.0;
  }
  fprintf(stderr, "Sum: %f\n", sum);
}
#endif