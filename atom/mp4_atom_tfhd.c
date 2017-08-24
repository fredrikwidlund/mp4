#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_tfhd.h"

static void mp4_atom_tfhd_delete(mp4_atom *atom)
{
  mp4_atom_tfhd_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_tfhd_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_tfhd *tfhd;

  tfhd = mp4_atom_tfhd_new(type, parent);
  bytes_pop(bytes, NULL, 1);
  tfhd->tf_flags = bytes_pop16(bytes);
  tfhd->tf_flags <<= 8;
  tfhd->tf_flags += bytes_pop8(bytes);

  tfhd->track_id = bytes_pop32(bytes);

  if (tfhd->tf_flags & 0x000001)
    tfhd->base_data_offset = bytes_pop64(bytes);
  if (tfhd->tf_flags & 0x000002)
    tfhd->sample_description_index = bytes_pop32(bytes);
  if (tfhd->tf_flags & 0x000008)
    tfhd->default_sample_duration = bytes_pop32(bytes);
  if (tfhd->tf_flags & 0x000010)
    tfhd->default_sample_size = bytes_pop32(bytes);
  if (tfhd->tf_flags & 0x000020)
    tfhd->default_sample_size = bytes_pop32(bytes);

  if (!bytes_valid(bytes) || bytes_size(bytes))
    {
      mp4_atom_tfhd_delete(tfhd);
      return NULL;
    }

return tfhd;
}

static void mp4_atom_tfhd_debug(mp4_atom *atom)
{
  mp4_atom_tfhd *tfhd = atom;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- tf flags 0x%06x\n", tfhd->tf_flags);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- track id %u\n", tfhd->track_id);
  if (tfhd->tf_flags & 0x000002)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "- sample description index %u\n", tfhd->sample_description_index);
    }
  if (tfhd->tf_flags & 0x000008)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "- default sample duration %u\n", tfhd->default_sample_duration);
    }
}

mp4_atom_descriptor mp4_atom_tfhd_descriptor =
{
  .unpack = mp4_atom_tfhd_unpack,
  .delete = mp4_atom_tfhd_delete,
  .debug = mp4_atom_tfhd_debug
};

mp4_atom *mp4_atom_tfhd_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_tfhd *tfhd;

  tfhd = calloc(1, sizeof *tfhd);
  mp4_atom_tfhd_construct(tfhd, type, parent, &mp4_atom_tfhd_descriptor);
  return tfhd;
}

void mp4_atom_tfhd_construct(mp4_atom_tfhd *tfhd, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  *tfhd = (mp4_atom_tfhd) {0};
  mp4_atom_base_construct(&tfhd->base, type, parent, descriptor);
}

void mp4_atom_tfhd_destruct(mp4_atom_tfhd *tfhd)
{
  mp4_atom_base_destruct(&tfhd->base);
}
