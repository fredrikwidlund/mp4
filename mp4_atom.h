#ifndef MP4_ATOM_H_INCLUDED
#define MP4_ATOM_H_INCLUDED

typedef char mp4_atom_type[4];
//typedef uint8_t mp4_atom_type[4];
typedef void    mp4_atom;

mp4_atom *mp4_atom_unpack(bytes *, mp4_atom_type, mp4_atom *);
mp4_atom *mp4_atom_load(char *);
void      mp4_atom_release(mp4_atom *);
void      mp4_atom_debug(mp4_atom *);
int       mp4_atom_level(mp4_atom *);

#endif /* MP4_ATOM_H_INCLUDED */
