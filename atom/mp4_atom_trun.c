#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"
#include "mp4_atom_trun.h"

static void mp4_atom_trun_delete(mp4_atom *atom)
{
  mp4_atom_trun_destruct(atom);
  free(atom);
}

static mp4_atom *mp4_atom_trun_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_trun *trun;
  mp4_atom_trun_sample sample;
  size_t i;

  trun = mp4_atom_trun_new(type, parent);
  trun->version = bytes_pop8(bytes);
  trun->tr_flags = bytes_pop16(bytes);
  trun->tr_flags <<= 8;
  trun->tr_flags += bytes_pop8(bytes);
  trun->sample_count = bytes_pop32(bytes);
  if (trun->tr_flags & 0x000001)
    trun->data_offset = bytes_pop32(bytes);
  if (trun->tr_flags & 0x000004)
    trun->first_sample_flags = bytes_pop32(bytes);

  for (i = 1; i <= trun->sample_count; i ++)
    {
      sample = (mp4_atom_trun_sample) {0};
      if (trun->tr_flags & 0x0000100)
        sample.sample_duration = bytes_pop32(bytes);
      if (trun->tr_flags & 0x0000200)
        sample.sample_size = bytes_pop32(bytes);
      if (trun->tr_flags & 0x0000400)
        sample.sample_flags = bytes_pop32(bytes);
      if (trun->tr_flags & 0x0000800)
        sample.sample_composition_time_offset = trun->version == 0 ? bytes_pop32(bytes) : (int32_t) bytes_pop32(bytes);
      vector_push_back(&trun->samples, &sample);
    }

  if (!bytes_valid(bytes) || bytes_size(bytes))
    {
      mp4_atom_trun_delete(trun);
      return NULL;
    }

  return trun;
}

static void mp4_atom_trun_debug(mp4_atom *atom)
{
  mp4_atom_trun *trun = atom;
  mp4_atom_trun_sample *sample = vector_data(&trun->samples);
  size_t i, total_size;

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "[%.*s]\n", (int) sizeof (mp4_atom_type), mp4_atom_base_type(atom));
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- version %u\n", trun->version);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- tr flags 0x%06x\n", trun->tr_flags);
  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- sample count %u\n", trun->sample_count);
  if (trun->tr_flags & 0x0000001)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "- data offset %d\n", trun->data_offset);
    }
  if (trun->tr_flags & 0x0000004)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "- first sample flags %08x\n", trun->first_sample_flags);
    }

  total_size = 0;
  for (i = 0; i < trun->sample_count; i ++)
    {
      mp4_atom_base_indent(stderr, atom);
      (void) fprintf(stderr, "  [sample %lu]\n", i + 1);

      if (trun->tr_flags & 0x0000100)
        {
          mp4_atom_base_indent(stderr, atom);
          (void) fprintf(stderr, "  - sample duration %u\n", sample[i].sample_duration);
        }
      if (trun->tr_flags & 0x0000200)
        {
          mp4_atom_base_indent(stderr, atom);
          (void) fprintf(stderr, "  - sample size %u\n", sample[i].sample_size);
          total_size += sample[i].sample_size;
        }
      if (trun->tr_flags & 0x0000400)
        {
          mp4_atom_base_indent(stderr, atom);
          (void) fprintf(stderr, "  - sample flags %08x\n", sample[i].sample_flags);
        }
    }

  mp4_atom_base_indent(stderr, atom);
  (void) fprintf(stderr, "- total sample size %lu\n", total_size);
}

mp4_atom_descriptor mp4_atom_trun_descriptor =
{
  .unpack = mp4_atom_trun_unpack,
  .delete = mp4_atom_trun_delete,
  .debug = mp4_atom_trun_debug
};

mp4_atom *mp4_atom_trun_new(mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_trun *trun;

  trun = calloc(1, sizeof *trun);
  mp4_atom_trun_construct(trun, type, parent, &mp4_atom_trun_descriptor);
  return trun;
}

void mp4_atom_trun_construct(mp4_atom_trun *trun, mp4_atom_type type, mp4_atom *parent,
                              mp4_atom_descriptor *descriptor)
{
  *trun = (mp4_atom_trun) {0};
  mp4_atom_base_construct(&trun->base, type, parent, descriptor);
  vector_construct(&trun->samples, sizeof (mp4_atom_trun_sample));
}

void mp4_atom_trun_destruct(mp4_atom_trun *trun)
{
  mp4_atom_base_destruct(&trun->base);
  vector_destruct(&trun->samples);
}
