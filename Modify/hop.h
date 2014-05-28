#ifndef _HOP_H__
#define _HOP_H__
#include "ppm.h"

extern Ppmimage *lowhopImage;
extern Ppmimage *highhopImage;
//extern Ppmimage *punch3Image;
extern GLuint lowhopTexture;
extern GLuint highhopTexture;
//extern GLuint punch3Texture;
extern unsigned char *Transparent;
extern int hop;
extern int hop_image;
extern int show_kangaroo;

extern void hop_render(double x, double y, double z);
extern void init_hop_texture(int, int);
extern unsigned char *buildAlphaData(Ppmimage *img);

#endif
