/*
  raw2plot: raw audio to gnuplot.

  Copyright (C) 2020 Gianluca Guida <glguida@tlbflush.org>

  SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>

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

static void print(void)
{
  unsigned long i;
  ssize_t r;
  float f;

  fprintf(out, "sample out env\n");
  i = 0;
  while ((r = fread(&f, sizeof(float), 1, in)) == 1)
    fprintf(out, "%ld %f %f\n", i++, f, env(f));
}

int main(int argc, char *argv[])
{
  in = stdin;
  out = stdout;

  print();

  return 0;
}
