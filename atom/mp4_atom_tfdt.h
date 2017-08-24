#ifndef MP4_ATOM_TFDT_H_INCLUDED
#define MP4_ATOM_TFDT_H_INCLUDED

typedef struct mp4_atom_tfdt mp4_atom_tfdt;
struct mp4_atom_tfdt
{
  mp4_atom_base base;
  uint8_t       version;
  uint64_t      base_media_decode_time;
};

mp4_atom *mp4_atom_tfdt_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_tfdt_construct(mp4_atom_tfdt *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_tfdt_destruct(mp4_atom_tfdt *);
void      mp4_atom_tfdt_add(mp4_atom_tfdt *, mp4_atom_type);

#endif /* MP4_ATOM_TFDT_H_INCLUDED */
