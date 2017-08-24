#ifndef MP4_ATOM_TRUN_H_INCLUDED
#define MP4_ATOM_TRUN_H_INCLUDED

typedef struct mp4_atom_trun_sample mp4_atom_trun_sample;
struct mp4_atom_trun_sample
{
  uint32_t      sample_duration;
  uint32_t      sample_size;
  uint32_t      sample_flags;
  int64_t       sample_composition_time_offset;
};

typedef struct mp4_atom_trun mp4_atom_trun;
struct mp4_atom_trun
{
  mp4_atom_base base;
  uint8_t       version;
  uint32_t      tr_flags;
  uint32_t      sample_count;
  uint32_t      data_offset;
  uint32_t      first_sample_flags;
  vector        samples;
};

mp4_atom *mp4_atom_trun_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_trun_construct(mp4_atom_trun *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_trun_destruct(mp4_atom_trun *);
void      mp4_atom_trun_add(mp4_atom_trun *, mp4_atom_type);

#endif /* MP4_ATOM_TRUN_H_INCLUDED */
