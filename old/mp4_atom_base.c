
/*
void mp4_atom_container(atom *a)
{
  bytes *b, child_bytes;
  atom_type type;
  atom *child;
  size_t size;

  b = &a->bytes;
  while (bytes_size(b))
    {
      assert(bytes_valid(b));

      size = bytes_pop32(b);
      bytes_pop(b, type.name, sizeof type.name);
      if (size == 0)
        size = bytes_size(b) - 8;
      else if (size == 1)
        size = bytes_pop64(b) - 16;
      else
        size -= 8;

      child_bytes = *b;
      bytes_truncate(&child_bytes, size);
      child = mp4_atom_new(child_bytes, type, a);
      vector_push_back(&a->children, child);
      bytes_pop(b, NULL, size);
    }
}

void mp4_atom_type(atom *a)
{
  bytes *b;
  atom_type type;

  b = &a->bytes;

  printf("%*s- types", mp4_atom_level(a) * 4, "");
  while (bytes_size(b))
    {
      bytes_pop(b, type.name, sizeof type.name);
      printf(" '%.*s'", 4, type.name);
    }
  printf("\n");
}

void mp4_atom_sidx(atom *a)
{
  bytes *b;
  int version, i;
  uint16_t reference_count;
  uint32_t reference_id, timescale;
  uint64_t earliest_presentation_time, first_offset;

  b = &a->bytes;

  version = bytes_pop8(b);
  bytes_pop(b, NULL, 3);
  reference_id = bytes_pop32(b);
  timescale = bytes_pop32(b);
  earliest_presentation_time = version == 0 ? bytes_pop32(b) : bytes_pop64(b);
  first_offset = version == 0 ? bytes_pop32(b) : bytes_pop64(b);;
  bytes_pop(b, NULL, 2);
  reference_count = bytes_pop16(b);

  printf("%*s- id %u, timescale %u\n", mp4_atom_level(a) * 4, "", reference_id, timescale);
  printf("%*s- ept %lu, offset %lu\n", mp4_atom_level(a) * 4, "", earliest_presentation_time, first_offset);
  printf("%*s- reference count %u\n", mp4_atom_level(a) * 4, "", reference_count);

  for (i = 0; i < reference_count; i ++)
    {
      uint32_t v;

      printf("%*s  - [%d]\n", mp4_atom_level(a) * 4, "", i);
      v = bytes_pop32(b);
      printf("%*s    - reference type %u\n", mp4_atom_level(a) * 4, "", v >> 31);
      printf("%*s    - referenced size %u\n", mp4_atom_level(a) * 4, "", (v << 1) >> 1);
      v = bytes_pop32(b);
      printf("%*s    - subsegment duration %u\n", mp4_atom_level(a) * 4, "", v);
      v = bytes_pop32(b);
      printf("%*s    - starts with sap %u\n", mp4_atom_level(a) * 4, "", v >> 31);
      printf("%*s    - sap type %u\n", mp4_atom_level(a) * 4, "", (v << 1) >> 29);
      printf("%*s    - sap delta time %u\n", mp4_atom_level(a) * 4, "", (v << 4) >> 4);
      (void) v;
    }

  assert(bytes_size(b) == 0 && bytes_valid(b));
}

void mp4_atom_parse(atom *a)
{
  static const mp4_atom_map maps[] = {
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
  const mp4_atom_map *map;

  for (map = maps; map->function && map->type.value != a->type.value; map ++)
    ;

  /*

  map = NULL;
  for (i = 0; i < sizeof maps / sizeof maps[0]; i ++)
    if (a->type.value == maps[i].type.value)
      {
        map = &(maps[i]);
        break;
      }
  */

  printf("%*s", mp4_atom_level(a) * 4, "");
  printf("[%.*s] size %lu%s", 4, a->type.name, bytes_size(&a->bytes), map->function ? "\n" : " (unknown)\n");

  if (map->function)
    map->function(a);
}

