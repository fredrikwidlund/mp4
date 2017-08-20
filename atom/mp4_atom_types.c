#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_desc.h"

typedef struct mp4_atom_types mp4_atom_types;
struct mp4_atom_types
{
  mp4_atom      base;
  size_t        count;
  mp4_atom_type types[];
};

static mp4_atom *unpack(mp4_desc *desc, bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_types *atom;
  size_t count;

  assert(bytes_size(bytes) % 4 == 0);
  count = bytes_size(bytes) / 4;
  atom = calloc(1, sizeof *atom + (count * sizeof atom->types[0]));
  assert(atom);
  atom->base = (mp4_atom) {type, parent, desc};
  atom->count = count;
  memcpy(atom->types, bytes_base(bytes), bytes_size(bytes));
  return &atom->base;
}

static void debug(mp4_atom *base)
{
  mp4_atom_types *atom = (mp4_atom_types *) base;

  size_t i;

  printf("[%.*s]\n", (int) sizeof(base->type), base->type.name);
  printf("- types");
   for (i = 0; i < atom->count; i ++)
     printf(" '%.*s'", (int) sizeof (atom->types[i]), atom->types[i].name);
  printf("\n");
}

mp4_desc mp4_atom_types_desc =
{
  .unpack = unpack,
  .debug = debug
};
