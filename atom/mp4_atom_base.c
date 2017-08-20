#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"
#include "mp4_atom_base.h"

void mp4_atom_base_construct(mp4_atom_base *base, mp4_atom_type type, mp4_atom *parent, mp4_atom_descriptor *descriptor)
{
  memcpy(base->type, type, sizeof base->type);
  base->parent = parent;
  base->descriptor = descriptor;
}

int mp4_atom_base_level(mp4_atom_base *atom)
{
  int level = 0;

  while (atom->parent)
    {
      level ++;
      atom = atom->parent;
    }
  return level;
}

char *mp4_atom_base_type(mp4_atom_base *base)
{
  return base->type;
}
