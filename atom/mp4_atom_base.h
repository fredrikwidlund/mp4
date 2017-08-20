#ifndef MP4_ATOM_BASE_H_INCLUDED
#define MP4_ATOM_BASE_H_INCLUDED

typedef struct mp4_atom_base mp4_atom_base;
struct mp4_atom_base
{
  mp4_atom_type        type;
  mp4_atom            *parent;
  mp4_atom_descriptor *descriptor;
};

void     mp4_atom_base_construct(mp4_atom_base *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
int      mp4_atom_base_level(mp4_atom_base *);
char    *mp4_atom_base_type(mp4_atom_base *);

#endif /* MP4_ATOM_BASE_H_INCLUDED */
