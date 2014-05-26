/* Header file */
#ifndef _PUNCH_H_
#define _PUNCH_H_
#include "ppm.h"

extern Ppmimage *punchleftImage;
extern Ppmimage *punchrightImage;
//extern Ppmimage *punch3Image;
extern GLuint punchleftTexture;
extern GLuint punchrightTexture;
//extern GLuint punch3Texture;
extern unsigned char *Transparent;
extern int punch;
extern int punch_image;
extern int show_kangaroo;

extern void punch_render(double x, double y, double z);
extern void init_punch_texture(int, int);
extern unsigned char *buildAlphaData(Ppmimage *img);

#endif
