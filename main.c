#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <assert.h>
#include <sys/stat.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"

int main(int argc, char **argv)
{
  mp4_atom *atom;

  atom = mp4_atom_load(argv[1]);
  if (!atom)
    err(1, "mp4_atom_load");

  mp4_atom_debug(atom);
  mp4_atom_delete(atom);
}
