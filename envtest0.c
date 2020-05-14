/*
  envtest0.c: simple reproduction of a specific DX7 envelope.

  Copyright (C) 2020 Gianluca Guida <glguida@tlbflush.org>

  SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *in, *out;

float
up(unsigned long sample)
{
  const float r50c = 0.000868;
  return r50c * r50c * sample * sample;
}

float
down(unsigned long sample)
{
  const float r50c = 0.00104;
  return expf(-r50c * sample);
}

int main(int argc, char *argv[])
{
  unsigned long i;
  float b;
  float v;

  v = 0;

  /* Segment 1: 0.0 -> 1.0 (level 99)*/
  i = 0;
  while (v < 1.0)
    {
      v = up(i++);
      printf("%f\n", v);
    }

  /* Segment 2: 1.0 -> .015 (level 50) */
  i = 0;
  b = 1.0;
  while (v > .015)
    {
      v = b * down(i++);
      printf("%f\n", v);
    }

  /* Segment 3: 0.015 -> .021 (level 800) */
  i = 0;
  b = v;
  while (v < .21)
    {
      v = b + up(i++);
      printf("%f\n", v);
    }

  i = 0;
  /* Sustain. */
  while (i++ < 38020) /* Roughly for 38k samples */
    printf("%f\n", v);

  i = 0;
  b = v;
  while (v > 0)
    {
      v = b * down(i++);
      printf("%f\n", v);
    }

}
