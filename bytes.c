#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "bytes.h"

static int bytes_need(bytes *bytes, size_t size)
{
  if (bytes_valid(bytes) && bytes_size(bytes) >= size)
    return 1;
  bytes->valid = 0;
  return 0;
}

void bytes_init(bytes *bytes)
{
  *bytes = (struct bytes) {0};
}

int bytes_valid(bytes *bytes)
{
  return bytes->valid;
}

int bytes_open(bytes *bytes, char *path)
{
  struct stat st;
  int e, fd;
  ssize_t n;

  bytes_init(bytes);

  e = stat(path, &st);
  if (e == -1)
    return -1;

  bytes->base = malloc(st.st_size);
  if (!bytes->base)
    return -1;

  fd = open(path, O_RDONLY);
  if (fd == -1)
    return -1;

  n = read(fd, bytes->base, st.st_size);
  (void) close(fd);
  if (n != st.st_size)
    return -1;

  bytes->end = bytes->base + n;
  bytes->valid = 1;
  return 0;
}

void bytes_clear(bytes *bytes)
{
  free(bytes->base);
  bytes_init(bytes);
}

uint8_t *bytes_base(bytes *bytes)
{
  return bytes->base;
}

size_t bytes_size(bytes *bytes)
{
  return bytes->end - bytes->base;
}

uint8_t bytes_pop8(bytes *bytes)
{
  uint8_t v;

  if (!bytes_need(bytes, 1))
    return 0;

  v = bytes->base[0];
  bytes->base ++;
  return v;
}

uint16_t bytes_pop16(bytes *bytes)
{
  uint16_t v;

  if (!bytes_need(bytes, 2))
    return 0;

  v = bytes->base[0];
  v <<= 8;
  v += bytes->base[1];
  bytes->base += 2;
  return v;
}

uint32_t bytes_pop32(bytes *bytes)
{
  uint32_t v;

  if (!bytes_need(bytes, 4))
    return 0;

  v = bytes->base[0];
  v <<= 8;
  v += bytes->base[1];
  v <<= 8;
  v += bytes->base[2];
  v <<= 8;
  v += bytes->base[3];

  bytes->base += 4;
  return v;
}

uint64_t bytes_pop64(bytes *bytes)
{
  uint64_t v;

  if (!bytes_need(bytes, 8))
    return 0;

  v = bytes_pop32(bytes);
  v <<= 32;
  v += bytes_pop32(bytes);
  return v;
}

void bytes_truncate(bytes *bytes, size_t size)
{
  if (!bytes_need(bytes, size))
    return;

  bytes->end = bytes->base + size;
}

void bytes_pop(bytes *bytes, uint8_t *data, size_t size)
{
  if (!bytes_need(bytes, size))
    return;

  if (data)
    memcpy(data, bytes->base, size);
  bytes->base += size;
}
