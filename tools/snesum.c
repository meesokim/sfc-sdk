/*
 * Copyright (c) 2010 Forest Belton (apples)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * File:           snesum.c
 * Description:    An application to compute and update a checksum for a SNES ROM
 *                 file.
 * Changelog:
 *  --Date:        Feb 24th, 2010
 *  --Description: Cleaned up a bit of the indentation for consistency. Added
 *                 comprehensive support for program parameters. This allows
 *                 for HiROM ROMs to be used. Also added a usage message which
 *                 displays information about the program.
 *  --Author:      Forest Belton (apples)
 *
 *  --Date:        Feb 24th, 2010
 *  --Description: Initial revision.
 *  --Author:      Forest Belton (apples)
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define die(msg) do {                \
  fprintf(stderr, "error: %s", msg); \
  exit(EXIT_FAILURE);                \
} while(0)

#define LOROM_HEADER          0x7fc0
#define HIROM_HEADER          0xffc0
#define OFFSET_CHECKSUM       0x001c

void usage(void)
{
  fprintf(stderr, "SNESum v0.02 by Forest Belton\n");
  fprintf(stderr, "usage: snesum [opts] [rom]\n");
  fprintf(stderr,
    "[opts] -l: Treat the ROM as a LoROM. Enabled by default.\n"
    "       -h: Treat the ROM as a HiROM.\n"
    "       -u: Print this message.\n");
  fprintf(stderr, "[rom]  The ROM file to update the checksum of.\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  FILE        *in;
  uint16_t     sum;
  unsigned int addr, i, is_lorom = 1;
  int          c;
  
  /* Parse program parameters. */
  while((c = getopt(argc, argv, "lhu")) != -1) {
    switch(c) {
      case 'h':
        is_lorom = 0;
        break;
      
      case 'l':
        is_lorom = 1;
        break;
      
      case 'u':
        usage();
        break;
    }
  }
  
  /* Display usage if insufficient parameters. */
  if((argc != 2) || (argv[optind] == NULL)) {
    usage();
  }
  
  if(!(in = fopen(argv[optind], "rb+"))) {
    die("failed to open input file");
  }
  
  /* Determine whether the ROM has a SMC header or not. */
  fseek(in, 0, SEEK_END);
  switch(ftell(in) % 1024) {
    /* This ROM is headerless. */
    case 0:
      fseek(in, 0, SEEK_SET);
      break;
    
    /* This ROM is headered. */
    case 512:
      fseek(in, 512, SEEK_SET);
      break;
    
    /* Incorrect size. */
    default:
      fclose(in);
      die("incorrect ROM size");
  }

  /* Compute address of checksum entry. */
  addr = is_lorom ? LOROM_HEADER + OFFSET_CHECKSUM
                  : HIROM_HEADER + OFFSET_CHECKSUM;
  
  /* Checksum everything up to the checksum entry. */
  sum = 0;
  for(i = 0; i < addr; ++i) {
    if((c = fgetc(in)) == EOF) {
      fclose(in);
      die("unexpected end of file");
    }
    
    sum += (uint8_t)c;
  }
  
  /* Seek past the checksum fields. */
  fseek(in, sizeof(uint16_t) * 2, SEEK_CUR);
  sum += 0x1fe;
  
  /* Finish computing the checksum. */
  while((c = fgetc(in)) != EOF) {
    sum += (uint8_t)c;
  }
  
  /* Seek back to the checksum entry. */
  fseek(in, addr, SEEK_SET);
  
  /* Rewrite the checksum entry. */
  fputc((sum >> 8) & 0xff, in);
  fputc(sum & 0xff,        in);
  
  /* Rewrite the checksum complement entry. */
  sum ^= 0xffff;
  fputc((sum >> 8) & 0xff, in);
  fputc(sum & 0xff,        in);
  
  /* Close input file. */
  fclose(in);
  
  return 0;
}
