#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"

int mp4_atom_level(mp4_atom *atom)
{
  return mp4_atom_base_level(atom);
}

mp4_atom *mp4_atom_load(char *path)
{
  mp4_atom *atom;
  bytes bytes;
  int e;

  e = bytes_open(&bytes, path);
  if (e == -1)
    return NULL;

  atom = mp4_atom_unpack(&bytes, "root", NULL);
  if (!atom)
    return NULL;

  if (bytes_size(&bytes) || !bytes_valid(&bytes))
    {
      mp4_atom_release(atom);
      return NULL;
    }

  return atom;
}

mp4_atom *mp4_atom_unpack(bytes *bytes, mp4_atom_type type, mp4_atom *parent)
{
  mp4_atom_descriptor *descriptor;

  descriptor = mp4_atom_descriptor_lookup(type);
  return descriptor ? descriptor->unpack(bytes, type, parent) : NULL;
}

void mp4_atom_release(mp4_atom *atom)
{
  mp4_atom_base *base = atom;
  base->descriptor->release(atom);
}

void mp4_atom_debug(mp4_atom *atom)
{
  mp4_atom_base *base = atom;
  base->descriptor->debug(atom);
}
