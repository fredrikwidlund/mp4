#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_sidx.h"

static void mp4_atom_sidx_delete(mp4_atom *atom)
{
  mp4_atom_sidx_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_sidx_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_sidx *sidx;
  mp4_atom_sidx_reference reference;
  uint32_t v32;
  size_t i;

  sidx = mp4_atom_sidx_new(type, parent);
  sidx->version = bytes_pop8(bytes);
  bytes_pop(bytes, NULL, 3);
  sidx->reference_id = bytes_pop32(bytes);
  sidx->timescale = bytes_pop32(bytes);
  sidx->earliest_presentation_time = sidx->version == 0 ? bytes_pop32(bytes) : bytes_pop64(bytes);
  sidx->first_offset = sidx->version == 0 ? bytes_pop32(bytes) : bytes_pop64(bytes);
  bytes_pop(bytes, NULL, 2);
  sidx->reference_count = bytes_pop16(bytes);

  for (i = 1; i <= sidx->reference_count; i ++)
    {
      v32 = bytes_pop32(bytes);
      reference.reference_type = v32 >> 31;
      reference.referenced_size = (v32 << 1) >> 1;
      reference.subsegment_duration = bytes_pop32(bytes);
      v32 = bytes_pop32(bytes);
      reference.starts_with_sap = v32 >> 31;
      reference.sap_type = (v32 << 1) >> 29;
      reference.sap_delta_time = (v32 << 4) >> 4;
      vector_push_back(&sidx->references, &reference);
    }

  if (!bytes_valid(bytes) || bytes_size(bytes))
    {
      mp4_atom_sidx_delete(sidx);
      return NULL;
    }

  return sidx;
}

static void mp4_atom_sidx_debug(mp4_atom *atom)
{
  mp4_atom_sidx *sidx = atom;
  mp4_atom_sidx_reference *reference;
  size_t i;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- version %u\n", sidx->version);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- reference_id %u\n", sidx->reference_id);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- timescale %u\n", sidx->timescale);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- earliest presentation time %lu\n", sidx->earliest_presentation_time);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- first offset %lu\n", sidx->first_offset);

  reference = vector_data(&sidx->references);
  for (i = 0; i < sidx->reference_count; i ++)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "  [reference %lu]\n", i + 1);
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "  - reference type %u\n", reference[i].reference_type);
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "  - referenced size %u\n", reference[i].referenced_size);
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "  - subsegment duration %f\n", (float) reference[i].subsegment_duration / sidx->timescale);
    }
}

mp4_atom_descriptor mp4_atom_sidx_descriptor =
{
  .unpack = mp4_atom_sidx_unpack,
  .delete = mp4_atom_sidx_delete,
  .debug = mp4_atom_sidx_debug
};

mp4_atom *mp4_atom_sidx_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_sidx *sidx;

  sidx = calloc(1, sizeof *sidx);
  mp4_atom_sidx_construct(sidx, type, parent, &mp4_atom_sidx_descriptor);
  return sidx;
}

void mp4_atom_sidx_construct(mp4_atom_sidx *sidx, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  *sidx = (mp4_atom_sidx) {0};
  mp4_atom_base_construct(&sidx->base, type, parent, descriptor);
  vector_construct(&sidx->references, sizeof (mp4_atom_sidx_reference));
}

void mp4_atom_sidx_destruct(mp4_atom_sidx *sidx)
{
  mp4_atom_base_destruct(&sidx->base);
}
