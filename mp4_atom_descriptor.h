#ifndef MP4_ATOM_DESCRIPTOR_H_INCLUDED
#define MP4_ATOM_DESCRIPTOR_H_INCLUDED

typedef struct mp4_atom_descriptor mp4_atom_descriptor;
typedef struct mp4_atom_descriptor_map mp4_atom_descriptor_map;

struct mp4_atom_descriptor
{
  mp4_atom     *(*unpack)(bytes *, mp4_atom_type, mp4_atom *);
  void          (*delete)(mp4_atom *);
  void          (*debug)(mp4_atom *);
};

mp4_atom_descriptor *mp4_atom_descriptor_lookup(mp4_atom_type);

#endif /* MP4_ATOM_DESCRIPTOR_H_INCLUDED */
