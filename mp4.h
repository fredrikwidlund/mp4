#ifndef MP4_H_INCLUDED
#define MP4_H_INCLUDED

typedef struct mp4 mp4;
struct mp4
{
  bytes  bytes;
  void  *root;
};

int mp4_open(mp4 *, char *);
void mp4_clear(mp4 *);

#endif /* MP4_H_INCLUDED */
