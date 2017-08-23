#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_mfhd.h"

static void mp4_atom_mfhd_delete(mp4_atom *atom)
{
  mp4_atom_mfhd_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_mfhd_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_mfhd *mfhd;

  mfhd = mp4_atom_mfhd_new(type, parent);
  bytes_pop(bytes, NULL, 4);
  mfhd->sequence_number = bytes_pop32(bytes);

  if (!bytes_valid(bytes) || bytes_size(bytes))
    {
      mp4_atom_mfhd_delete(mfhd);
      return NULL;
    }

  return mfhd;
}

static void mp4_atom_mfhd_debug(mp4_atom *atom)
{
  mp4_atom_mfhd *mfhd = atom;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- sequence number %u\n", mfhd->sequence_number);
}

mp4_atom_descriptor mp4_atom_mfhd_descriptor =
{
  .unpack = mp4_atom_mfhd_unpack,
  .delete = mp4_atom_mfhd_delete,
  .debug = mp4_atom_mfhd_debug
};

mp4_atom *mp4_atom_mfhd_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_mfhd *mfhd;

  mfhd = calloc(1, sizeof *mfhd);
  mp4_atom_mfhd_construct(mfhd, type, parent, &mp4_atom_mfhd_descriptor);
  return mfhd;
}

void mp4_atom_mfhd_construct(mp4_atom_mfhd *mfhd, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  *mfhd = (mp4_atom_mfhd) {0};
  mp4_atom_base_construct(&mfhd->base, type, parent, descriptor);
}

void mp4_atom_mfhd_destruct(mp4_atom_mfhd *mfhd)
{
}
