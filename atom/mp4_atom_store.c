#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_store.h"

static mp4_atom *mp4_atom_store_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_store *store;

  store = mp4_atom_store_new(type, parent);
  mp4_atom_store_set(store, bytes_base(bytes), bytes_size(bytes));
  bytes_pop(bytes, NULL, bytes_size(bytes));

  return store;
}

static void mp4_atom_store_delete(mp4_atom *atom)
{
  mp4_atom_store_destruct(atom);
  free(atom);
}

static void mp4_atom_store_debug(mp4_atom *atom)
{
  mp4_atom_store *store = atom;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- size %lu\n", store->size);
}

mp4_atom_descriptor mp4_atom_store_descriptor =
{
  .unpack = mp4_atom_store_unpack,
  .delete = mp4_atom_store_delete,
  .debug = mp4_atom_store_debug
};

mp4_atom *mp4_atom_store_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_store *store;

  store = calloc(1, sizeof *store);
  mp4_atom_store_construct(store, type, parent, &mp4_atom_store_descriptor);
  return store;
}

void mp4_atom_store_construct(mp4_atom_store *store, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  mp4_atom_base_construct(&store->base, type, parent, descriptor);
  store->data = NULL;
  store->size = 0;
}

void mp4_atom_store_destruct(mp4_atom_store *store)
{
  mp4_atom_base_destruct(&store->base);
  free(store->data);
  store->data = NULL;
  store->size = 0;
}

void mp4_atom_store_set(mp4_atom_store *store, void *data, size_t size)
{
  free(store->data);
  store->data = malloc(size);
  store->size = size;
  memcpy(store->data, data, size);
}
