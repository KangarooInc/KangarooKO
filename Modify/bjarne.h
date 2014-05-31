/* Header file */
#ifndef _BJARNE_H_
#define _BJARNE_H_
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
