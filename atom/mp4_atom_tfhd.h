#ifndef MP4_ATOM_TFHD_H_INCLUDED
#define MP4_ATOM_TFHD_H_INCLUDED

typedef struct mp4_atom_tfhd mp4_atom_tfhd;
struct mp4_atom_tfhd
{
  mp4_atom_base base;
  uint32_t      tf_flags;
  uint32_t      track_id;
  uint64_t      base_data_offset;
  uint32_t      sample_description_index;
  uint32_t      default_sample_duration;
  uint32_t      default_sample_size;
  uint32_t      default_sample_flags;
};

mp4_atom *mp4_atom_tfhd_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_tfhd_construct(mp4_atom_tfhd *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_tfhd_destruct(mp4_atom_tfhd *);
void      mp4_atom_tfhd_add(mp4_atom_tfhd *, mp4_atom_type);

#endif /* MP4_ATOM_TFHD_H_INCLUDED */
