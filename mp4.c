#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <dynamic.h>

#include "bytes.h"
#include "mp4.h"

typedef struct atom_type atom_type;
struct atom_type
{
  union
  {
    uint8_t    name[4];
    uint32_t   value;
  };
};

const atom_type atom_type_none = {0};

typedef struct atom atom;
struct atom
{
  atom        *parent;
  atom_type    type;
  bytes        bytes;
  vector       children;
};

typedef struct mp4_atom_map mp4_atom_map;
struct mp4_atom_map
{
  atom_type    type;
  void       (*function)(atom *);
};


atom *mp4_atom_new(bytes, atom_type, atom *);

int mp4_atom_level(atom *a)
{
  int level = 0;

  while (a->parent)
    {
      a = a->parent;
      level ++;
    }

  return level;
}

void mp4_atom_container(atom *a)
{
  bytes *b, child_bytes;
  atom_type type;
  atom *child;
  size_t size;

  b = &a->bytes;
  while (bytes_size(b))
    {
      assert(bytes_valid(b));

      size = bytes_pop32(b);
      bytes_pop(b, type.name, sizeof type.name);
      if (size == 0)
        size = bytes_size(b) - 8;
      else if (size == 1)
        size = bytes_pop64(b) - 16;
      else
        size -= 8;

      child_bytes = *b;
      bytes_truncate(&child_bytes, size);
      child = mp4_atom_new(child_bytes, type, a);
      vector_push_back(&a->children, child);
      bytes_pop(b, NULL, size);
    }
}

void mp4_atom_type(atom *a)
{
  bytes *b;
  atom_type type;

  b = &a->bytes;

  printf("%*s- types", mp4_atom_level(a) * 4, "");
  while (bytes_size(b))
    {
      bytes_pop(b, type.name, sizeof type.name);
      printf(" '%.*s'", 4, type.name);
    }
  printf("\n");
}

/*
typedef struct atom_sidx atom_sidx;
struct __attribute__((__packed__)) atom_sidx
{
  uint8_t  v;
  uint8_t  x[3];
  uint32_t id;
  uint32_t timescale;
  uint64_t ept;
  uint64_t offset;
  uint16_t reserved;
  uint16_t ref;
  uint32_t type:1;
  uint32_t size:31;
  uint32_t duration;
  uint32_t sap_type:3;
  uint32_t sap_delta_time:28;
};
*/

void mp4_atom_sidx(atom *a)
{
  bytes *b;
  uint32_t v;

  b = &a->bytes;
  printf("%lu\n", bytes_size(b));
  printf("%*s- ", mp4_atom_level(a) * 4, "");
  printf("version %u", bytes_pop8(b));
  bytes_pop(b, NULL, 3);
  printf(", id %u", bytes_pop32(b));
  printf(", timescale %u", bytes_pop32(b));
  printf(", ept %lu", bytes_pop64(b));
  printf(", offset %lu", bytes_pop64(b));
  bytes_pop(b, NULL, 2);
  printf(", ref %u", bytes_pop16(b));
  v = bytes_pop32(b);
  printf(", type %u", v >> 31);
  printf(", size %u", v & ~(1 << 31));
  printf(", duration %u", bytes_pop32(b));
  v = bytes_pop32(b);
  printf(", sap type %u", v >> 29);
  printf(", sap delta time %u", v & ~(7 << 29));

  assert(bytes_size(b) == 0 && bytes_valid(b));
  printf("\n");
}

void mp4_atom_parse(atom *a)
{
  static const mp4_atom_map maps[] = {
    {.type.value = 0, .function = mp4_atom_container},
    {.type.name = "moov", .function = mp4_atom_container}, // container for all the meta-data (iso)
    {.type.name = "moof", .function = mp4_atom_container}, // movie fragment (iso)
    {.type.name = "trak", .function = mp4_atom_container}, // container for an individual track or stream (iso)
    {.type.name = "traf", .function = mp4_atom_container}, // track fragment (iso)
    {.type.name = "mdia", .function = mp4_atom_container}, // container for the media information in a track (iso)
    {.type.name = "minf", .function = mp4_atom_container}, // media information container (iso)
    {.type.name = "stbl", .function = mp4_atom_container}, // sample table box, container for the time/space map (iso)
    {.type.name = "ftyp", .function = mp4_atom_type},      // file type and compatibility (jp2, iso)
    {.type.name = "styp", .function = mp4_atom_type},      // segment type box (3gpp)
    {.type.name = "sidx", .function = mp4_atom_sidx},      // segment index box (3gpp)
    {.type.value = 0, .function = NULL},
  };
  const mp4_atom_map *map;

  for (map = maps; map->function && map->type.value != a->type.value; map ++)
    ;

  /*

  map = NULL;
  for (i = 0; i < sizeof maps / sizeof maps[0]; i ++)
    if (a->type.value == maps[i].type.value)
      {
        map = &(maps[i]);
        break;
      }
  */

  printf("%*s", mp4_atom_level(a) * 4, "");
  printf("[%.*s] size %lu%s", 4, a->type.name, bytes_size(&a->bytes), map->function ? "\n" : " (unknown)\n");

  if (map->function)
    map->function(a);
}

void mp4_atom_init(atom *a)
{
  *a = (atom) {0};
  vector_construct(&a->children, sizeof a);
}

atom *mp4_atom_new(bytes bytes, atom_type type, atom *parent)
{
  atom *a;

  a = malloc(sizeof *a);
  assert(a);
  mp4_atom_init(a);

  a->parent = parent;
  a->type = type;
  a->bytes = bytes;
  mp4_atom_parse(a);

  return a;
}

int mp4_open(mp4 *mp4, char *path)
{
  int e;

  e = bytes_open(&mp4->bytes, path);
  if (e == -1)
    return -1;

  mp4->root = mp4_atom_new(mp4->bytes, atom_type_none, NULL);
  return 0;
}

void mp4_clear(mp4 *mp4)
{
  bytes_clear(&mp4->bytes);
}
