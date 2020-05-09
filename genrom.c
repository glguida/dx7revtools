/*
  genrom: simple voices generator for DX7/TX7.

  Copyright (C) 2020 Gianluca Guida <glguida@tlbflush.org>

  SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <string.h>


struct dx7voice_packed32
{
  struct {
    uint8_t eg_rate[4];
    uint8_t eg_level[4];
    uint8_t kbdlvl_breakpoint;
    uint8_t kbdlvl_ldepth;
    uint8_t kbdlvl_rdepth;
    uint8_t kbdlvl_lrcurve;
    uint8_t freq_det_ratescale;
    uint8_t keyvelsens_ampmodsens;
    uint8_t outputlevel;
    uint8_t freq_coarse_oscmode;
    uint8_t freq_fine;
  } ops[6];

  uint8_t pitch_eg_rate[4];
  uint8_t pitch_eg_level[4];
  uint8_t algorithm;
  uint8_t oscsync_feedback;
  uint8_t lfo_speed;
  uint8_t lfo_delay;
  uint8_t lfo_pmd;
  uint8_t lfo_amd;
  uint8_t lfo_modsens_wave_sync;
  uint8_t transpose;
  char    name[10];
} __attribute__((packed));
typedef struct dx7voice_packed32 voice_t;

voice_t voices[32];

FILE *out;

static void
write_rom(void)
{
  int r;
  uint8_t chksum;
  const char hdr[] = { 0xf0, 67, 0, 9, 0x20, 0, };

  chksum = 0;
  for (int i = 0; i < sizeof(voices); i++)
    chksum += *((uint8_t *)voices + i) & 0x7f;

  r = fwrite(hdr, sizeof(hdr), 1, out);
  if (r != 1)
    err(-1, "Writing header");

  r = fwrite(voices, sizeof(voices), 1, out);
  if (r != 1)
    err(-1, "Writing voices");

  r = fputc((int)-chksum & 0x7f, out);
  if (r == EOF)
    err(-1, "Writing chksum");

  r = fputc((int)0xf7, out);
  if (r == EOF)
    err(-1, "Writing EOX");
}

int main(int argc, char *argv[])
{
  voice_t v;

  out = stdout;

  /*
    Simple voice for initial testing.
  */
  memset(&v, 0, sizeof(v));
  const char name[10] = { 'E', 'N', 'V',' ', 'T', 'E', 'S', 'T' , '#', '0'};
  memcpy(v.name, name, sizeof(v.name));

  /* Set operator #1 EG rate. */
  v.ops[0].eg_rate[0] = 50; 
  v.ops[0].eg_rate[1] = 50;
  v.ops[0].eg_rate[2] = 50;
  v.ops[0].eg_rate[3] = 50;

  /* Set operator #1 EG levels. */
  v.ops[0].eg_level[0] = 99;
  v.ops[0].eg_level[1] = 50;
  v.ops[0].eg_level[2] = 80;
  v.ops[0].eg_level[3] = 0;


  v.ops[0].keyvelsens_ampmodsens = 3;
  v.ops[0].freq_coarse_oscmode = 31<<1;
  v.ops[0].outputlevel = 99;

  for (int i = 1; i < 6; i++)
    {
      for (int j = 0; j < 4; j++)
	{
	  v.ops[i].eg_rate[j] = 0;
	  v.ops[i].eg_level[j] = 0;
	}
      v.ops[i].outputlevel = 0;
    }

  for (int j = 0; j < 4; j++)
    {
      v.pitch_eg_rate[j] = 99;
      v.pitch_eg_rate[j] = 99;
    }

  v.algorithm = 31; /* Simplest algorithm (32), no operator is modulated. */

  voices[0] = v;

  write_rom();
}
