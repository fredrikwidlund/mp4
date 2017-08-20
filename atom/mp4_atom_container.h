#ifndef MP4_ATOM_CONTAINER_H_INCLUDED
#define MP4_ATOM_CONTAINER_H_INCLUDED

typedef struct mp4_atom_container mp4_atom_container;
struct mp4_atom_container
{
  mp4_atom_base base;
  vector        children;
};

mp4_atom *mp4_atom_container_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_container_construct(mp4_atom_container *, mp4_atom_type, mp4_atom *,
                                       mp4_atom_descriptor *);
void      mp4_atom_container_add(mp4_atom_container *, mp4_atom *);

mp4_atom *mp4_atom_container_unpack(bytes *, mp4_atom_type, mp4_atom *);
void      mp4_atom_container_debug(mp4_atom *);

#endif /* MP4_ATOM_CONTAINER_H_INCLUDED */
