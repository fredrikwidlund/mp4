#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <assert.h>
#include <sys/stat.h>

#include "bytes.h"
#include "mp4.h"

int main(int argc, char **argv)
{
  mp4 mp4;
  int e;

  e = mp4_open(&mp4, argv[1]);
  if (e == -1)
    err(1, "mp4_open");

  mp4_clear(&mp4);
}
