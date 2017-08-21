#ifndef MP4_ATOM_STORE_H_INCLUDED
#define MP4_ATOM_STORE_H_INCLUDED

typedef struct mp4_atom_store mp4_atom_store;
struct mp4_atom_store
{
  mp4_atom_base  base;
  void          *data;
  size_t         size;
};

mp4_atom *mp4_atom_store_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_store_construct(mp4_atom_store *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_store_destruct(mp4_atom_store *);
void      mp4_atom_store_set(mp4_atom_store *, void *, size_t);

#endif /* MP4_ATOM_STORE_H_INCLUDED */
