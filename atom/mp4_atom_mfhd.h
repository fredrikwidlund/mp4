#ifndef MP4_ATOM_MFHD_H_INCLUDED
#define MP4_ATOM_MFHD_H_INCLUDED

typedef struct mp4_atom_mfhd mp4_atom_mfhd;
struct mp4_atom_mfhd
{
  mp4_atom_base base;
  uint32_t      sequence_number;
};

mp4_atom *mp4_atom_mfhd_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_mfhd_construct(mp4_atom_mfhd *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_mfhd_destruct(mp4_atom_mfhd *);
void      mp4_atom_mfhd_add(mp4_atom_mfhd *, mp4_atom_type);

#endif /* MP4_ATOM_MFHD_H_INCLUDED */
