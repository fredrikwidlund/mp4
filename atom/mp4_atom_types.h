#ifndef MP4_ATOM_TYPES_H_INCLUDED
#define MP4_ATOM_TYPES_H_INCLUDED

typedef struct mp4_atom_types mp4_atom_types;
struct mp4_atom_types
{
  mp4_atom_base  base;
  mp4_atom_type  brand;
  uint32_t       minor_version;
  vector         compatible;
};

mp4_atom *mp4_atom_types_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_types_construct(mp4_atom_types *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_types_destruct(mp4_atom_types *);
void      mp4_atom_types_add(mp4_atom_types *, mp4_atom_type);

#endif /* MP4_ATOM_TYPES_H_INCLUDED */
