/* Header file */
#ifndef _PUNCH_H_
#define _PUNCH_H_
#include "ppm.h"

Ppmimage *punch1Image;
Ppmimage *punch2Image;
Ppmimage *punch3Image;
extern GLuint punch1Texture;
extern GLuint punch2Texture;
extern GLuint punch3Texture;
extern GLuint silhouetteTexture2;
extern GLuint silhouetteTexture3;
extern GLuint silhouetteTexture4;
extern int silhouette2;
extern int punch;
extern int punch_image;
extern int show_kangaroo;

extern void punch_render(double x, double y, double z);
extern void init_punch_texture(void);
extern unsigned char *buildAlphaData(Ppmimage *img);

#endif
