#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_types.h"

static void mp4_atom_types_delete(mp4_atom *atom)
{
  mp4_atom_types_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_types_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_types *types;
  mp4_atom_type brand;

  types = mp4_atom_types_new(type, parent);
  bytes_pop(bytes, (uint8_t *) types->brand, sizeof brand);
  types->minor_version = bytes_pop32(bytes);

  while (bytes_size(bytes))
    {
      bytes_pop(bytes, (uint8_t *) brand, sizeof brand);
      vector_push_back(&types->compatible, &brand);
    }

  if (!bytes_valid(bytes))
    {
      mp4_atom_types_delete(types);
      return NULL;
    }

  return types;
}

static void mp4_atom_types_debug(mp4_atom *atom)
{
  mp4_atom_types *types = atom;
  size_t i;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- brand %.*s, version %u, compatible", (int) sizeof types->brand, types->brand, types->minor_version);
  for (i = 0; i < vector_size(&types->compatible); i ++)
    (void) fprintf(stderr, " %.*s%s", (int) sizeof (mp4_atom_type), (char *) vector_at(&types->compatible, i),
                   i < vector_size(&types->compatible) - 1 ? "," : "");
  (void) fprintf(stderr, "\n");
}

mp4_atom_descriptor mp4_atom_types_descriptor =
{
  .unpack = mp4_atom_types_unpack,
  .delete = mp4_atom_types_delete,
  .debug = mp4_atom_types_debug
};

mp4_atom *mp4_atom_types_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_types *types;

  types = calloc(1, sizeof *types);
  mp4_atom_types_construct(types, type, parent, &mp4_atom_types_descriptor);
  return types;
}

void mp4_atom_types_construct(mp4_atom_types *types, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  mp4_atom_base_construct(&types->base, type, parent, descriptor);
  memset(types->brand, 0, sizeof types->brand);
  types->minor_version = 0;
  vector_construct(&types->compatible, sizeof (mp4_atom_type));
}

void mp4_atom_types_destruct(mp4_atom_types *types)
{
  mp4_atom_base_destruct(&types->base);
  vector_destruct(&types->compatible, NULL);
}
