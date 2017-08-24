#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_tfdt.h"

static void mp4_atom_tfdt_delete(mp4_atom *atom)
{
  mp4_atom_tfdt_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_tfdt_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_tfdt *tfdt;

  tfdt = mp4_atom_tfdt_new(type, parent);
  tfdt->version = bytes_pop8(bytes);
  bytes_pop(bytes, NULL, 3);
  tfdt->base_media_decode_time = tfdt->version == 0 ? bytes_pop32(bytes) : bytes_pop64(bytes);
  return tfdt;
}

static void mp4_atom_tfdt_debug(mp4_atom *atom)
{
  mp4_atom_tfdt *tfdt = atom;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- version %u\n", tfdt->version);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- base media decode time %lu\n", tfdt->base_media_decode_time);
}

mp4_atom_descriptor mp4_atom_tfdt_descriptor =
{
  .unpack = mp4_atom_tfdt_unpack,
  .delete = mp4_atom_tfdt_delete,
  .debug = mp4_atom_tfdt_debug
};

mp4_atom *mp4_atom_tfdt_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_tfdt *tfdt;

  tfdt = calloc(1, sizeof *tfdt);
  mp4_atom_tfdt_construct(tfdt, type, parent, &mp4_atom_tfdt_descriptor);
  return tfdt;
}

void mp4_atom_tfdt_construct(mp4_atom_tfdt *tfdt, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  *tfdt = (mp4_atom_tfdt) {0};
  mp4_atom_base_construct(&tfdt->base, type, parent, descriptor);
}

void mp4_atom_tfdt_destruct(mp4_atom_tfdt *tfdt)
{
  mp4_atom_base_destruct(&tfdt->base);
}
