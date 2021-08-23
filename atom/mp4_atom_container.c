#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_container.h"

static mp4_atom *mp4_atom_container_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_container *container;
  mp4_atom_type child_type;
  mp4_atom *child;
  struct bytes child_bytes;
  size_t size;

  container = mp4_atom_container_new(type, parent);

  while (bytes_valid(bytes) && bytes_size(bytes))
    {
      size = bytes_pop32(bytes);
      bytes_pop(bytes, (uint8_t *) child_type, sizeof child_type);
      if (size == 0)
        size = bytes_size(bytes) - 8;
      else if (size == 1)
        size = bytes_pop64(bytes) - 16;
      else
        size -= 8;

      child_bytes = *bytes;
      bytes_truncate(&child_bytes, size);
      child = mp4_atom_unpack(&child_bytes, child_type, container);
      mp4_atom_container_add(container, child);
      bytes_pop(bytes, NULL, size);
    }

  return container;
}

static void mp4_atom_container_delete(mp4_atom *atom)
{
  mp4_atom_container *container = atom;
  mp4_atom **child;
  size_t i;

  child = vector_data(&container->children);
  for (i = 0; i < vector_size(&container->children); i ++)
    mp4_atom_delete(child[i]);

  mp4_atom_container_destruct(container);
  free(container);
}

static void mp4_atom_container_debug(mp4_atom *atom)
{
  mp4_atom_container *container = atom;
  mp4_atom **child;
  size_t i;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  child = vector_data(&container->children);
  for (i = 0; i < vector_size(&container->children); i ++)
    mp4_atom_debug(child[i]);
}

mp4_atom_descriptor mp4_atom_container_descriptor =
{
  .unpack = mp4_atom_container_unpack,
  .delete = mp4_atom_container_delete,
  .debug = mp4_atom_container_debug
};

mp4_atom *mp4_atom_container_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_container *container;

  container = calloc(1, sizeof *container);
  mp4_atom_container_construct(container, type, parent, &mp4_atom_container_descriptor);
  return container;
}

void mp4_atom_container_construct(mp4_atom_container *container, mp4_atom_type type, mp4_atom *parent,
                                  mp4_atom_descriptor *descriptor)
{
  mp4_atom_base_construct(&container->base, type, parent, descriptor);
  vector_construct(&container->children, sizeof (mp4_atom *));
}

void mp4_atom_container_destruct(mp4_atom_container *container)
{
  mp4_atom_base_destruct(&container->base);
  vector_destruct(&container->children, NULL);
}

void mp4_atom_container_add(mp4_atom_container *container, mp4_atom *atom)
{
  vector_push_back(&container->children, &atom);
}
