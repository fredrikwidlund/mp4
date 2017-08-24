#include <stdlib.h>
#include <stdint.h>

#include <dynamic.h>

#include "bytes.h"
#include "mp4_atom.h"
#include "mp4_atom_descriptor.h"

typedef struct mp4_atom_descriptor_map mp4_atom_descriptor_map;
struct mp4_atom_descriptor_map
{
  mp4_atom_type        type;
  mp4_atom_descriptor *descriptor;
};

extern mp4_atom_descriptor mp4_atom_container_descriptor;
extern mp4_atom_descriptor mp4_atom_store_descriptor;
extern mp4_atom_descriptor mp4_atom_types_descriptor;
extern mp4_atom_descriptor mp4_atom_sidx_descriptor;
extern mp4_atom_descriptor mp4_atom_mfhd_descriptor;
extern mp4_atom_descriptor mp4_atom_tfhd_descriptor;
extern mp4_atom_descriptor mp4_atom_tfdt_descriptor;
extern mp4_atom_descriptor mp4_atom_trun_descriptor;

static mp4_atom_descriptor_map maps[] =
  {
    {"root", &mp4_atom_container_descriptor},
    {"moov", &mp4_atom_container_descriptor},
    {"moof", &mp4_atom_container_descriptor},
    {"mdia", &mp4_atom_container_descriptor},
    {"minf", &mp4_atom_container_descriptor},
    {"stbl", &mp4_atom_container_descriptor},
    {"trak", &mp4_atom_container_descriptor},
    {"traf", &mp4_atom_container_descriptor},
    {"ftyp", &mp4_atom_types_descriptor},
    {"styp", &mp4_atom_types_descriptor},
    {"sidx", &mp4_atom_sidx_descriptor},
    {"mfhd", &mp4_atom_mfhd_descriptor},
    {"tfhd", &mp4_atom_tfhd_descriptor},
    {"tfdt", &mp4_atom_tfdt_descriptor},
    {"trun", &mp4_atom_trun_descriptor},
    {"", &mp4_atom_store_descriptor},
  };

/*
    {.type.value = 0, .function = mp4_atom_container},
    {.type.name = "moov", .function = mp4_atom_container}, // container for all the meta-data (iso)
    {.type.name = "moof", .function = mp4_atom_container}, // movie fragment (iso)
    {.type.name = "trak", .function = mp4_atom_container}, // container for an individual track or stream (iso)
    {.type.name = "traf", .function = mp4_atom_container}, // track fragment (iso)
    {.type.name = "mdia", .function = mp4_atom_container}, // container for the media information in a track (iso)
    {.type.name = "minf", .function = mp4_atom_container}, // media information container (iso)
    {.type.name = "stbl", .function = mp4_atom_container}, // sample table box, container for the time/space map (iso)
    {.type.name = "ftyp", .function = mp4_atom_type},      // file type and compatibility (jp2, iso)
    {.type.name = "styp", .function = mp4_atom_type},      // segment type box (3gpp)
    {.type.name = "sidx", .function = mp4_atom_sidx},      // segment index box (3gpp)
    {.type.value = 0, .function = NULL},
  };
  const m
*/

mp4_atom_descriptor *mp4_atom_descriptor_lookup(mp4_atom_type type)
{
  mp4_atom_descriptor_map *map;

  for (map = maps;; map ++)
    if (*(uint32_t *) (map->type) == *(uint32_t *) type || !map->type[0])
      return map->descriptor;

  return NULL;
}
