#ifndef BYTES_H_INCLUDED
#define BYTES_H_INCLUDED

typedef struct bytes bytes;
struct bytes
{
  int      valid;
  uint8_t *base;
  uint8_t *end;
};

void      bytes_init(bytes *);
int       bytes_valid(bytes *);
int       bytes_open(bytes *, char *);
void      bytes_clear(bytes *);
uint8_t  *bytes_base(bytes *);
size_t    bytes_size(bytes *);
uint8_t   bytes_pop8(bytes *);
uint16_t  bytes_pop16(bytes *);
uint32_t  bytes_pop32(bytes *);
uint64_t  bytes_pop64(bytes *);
void      bytes_truncate(bytes *, size_t);
void      bytes_pop(bytes *, uint8_t *, size_t);

#endif /* BYTES_H_INCLUDED */
