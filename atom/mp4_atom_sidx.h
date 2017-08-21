#ifndef MP4_ATOM_SIDX_H_INCLUDED
#define MP4_ATOM_SIDX_H_INCLUDED

typedef struct mp4_atom_sidx_reference mp4_atom_sidx_reference;
struct mp4_atom_sidx_reference
{
  uint32_t      reference_type:1;
  uint32_t      referenced_size:31;
  uint32_t      subsegment_duration;
  uint32_t      starts_with_sap:1;
  uint32_t      sap_type:3;
  uint32_t      sap_delta_time:28;
};

typedef struct mp4_atom_sidx mp4_atom_sidx;
struct mp4_atom_sidx
{
  mp4_atom_base base;
  uint8_t       version;
  uint32_t      reference_id;
  uint32_t      timescale;
  uint64_t      earliest_presentation_time;
  uint64_t      first_offset;
  uint16_t      reference_count;
  vector        references;
};

mp4_atom *mp4_atom_sidx_new(mp4_atom_type, mp4_atom *);
void      mp4_atom_sidx_construct(mp4_atom_sidx *, mp4_atom_type, mp4_atom *, mp4_atom_descriptor *);
void      mp4_atom_sidx_destruct(mp4_atom_sidx *);
void      mp4_atom_sidx_add(mp4_atom_sidx *, mp4_atom_type);

#endif /* MP4_ATOM_SIDX_H_INCLUDED */
