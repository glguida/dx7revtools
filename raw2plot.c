/*
  raw2plot: raw audio to gnuplot.

  Copyright (C) 2020 Gianluca Guida <glguida@tlbflush.org>

  SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>

FILE *in, *out;

static void print(void)
{
  unsigned long i;
  ssize_t r;
  float f;

  i = 0;
  while ((r = fread(&f, sizeof(float), 1, in)) == 1)
    fprintf(out, "%ld %f\n", i++, f);
}

int main(int argc, char *argv[])
{
  in = stdin;
  out = stdout;

  print();

  return 0;
}
