/*
  raw2plot: raw audio to gnuplot.

  Copyright (C) 2020 Gianluca Guida <glguida@tlbflush.org>

  SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *in, *out;

static float env(float f)
{
  static float lmax = 0, pf = 0, diff = 0;
  float pdiff;

  pdiff = diff;
  diff = f - pf;
  if (pdiff >= 0 && diff < 0)
    {
      /* f > lmax or derivative changed sign into negative (zero case a *bit* edgy): local maximum. */
      lmax = pf;
    }
  pf = f;
  return lmax;
}

static void print(float n)
{
  unsigned long i;
  ssize_t r;
  float f;

  fprintf(out, "#sample out env sqrt(env)\n");
  i = 0;
  while ((r = fread(&f, sizeof(float), 1, in)) == 1)
    {
      float e = env(f)/n;
      fprintf(out, "%ld %f %f %f\n", i++, f, e, sqrtf(e));
    }
}

int main(int argc, char *argv[])
{
  float norm = 1.0;

  in = stdin;
  out = stdout;

  if (argc >= 2)
    norm = (float)atof(argv[1]);

  print(norm);

  return 0;
}
