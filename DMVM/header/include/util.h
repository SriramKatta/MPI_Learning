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

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef ABS
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#endif

size_t chunkstart(size_t N, int rank, int size);

size_t rowsinrank(size_t N, int rank, int size);

void matDataToRankFile(double *A, int rows, int cols, int rank);

void vecDataToRankFile(double *V, int cols, int rank, char ch);

#endif // __UTIL_H_
