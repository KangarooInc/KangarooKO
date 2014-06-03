#ifndef _ANGEL_H_
#define _ANGEL_H
#include <GL/glx.h>
#include "ppm.h"

#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define random(a) (rand()%a)
typedef double Flt;
typedef double Vec[3];
extern Ppmimage *appleImage;
extern GLuint appletexture;
//extern int apple;
void apple_render();
void applevector (void);
//extern Ppmimage *badappleImage;
//extern GLuint badappletexture;
//extern int apple;
void applecollision(void);
void badapple_render();
void move_apple();//############################################3
void appleReset(void); //##########################################
void applePhysics();
extern void init_apple_texture();
//extern void init_badapple_texture();
extern int high_score;
extern unsigned char *buildAlphaData(Ppmimage *img);


    Flt d0,d1,dist;
extern int show_apple;
Flt punch_dist, hit_dist, apple_hit_dist;


typedef struct t_apple {
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Apple;
Apple apple;
#endif
