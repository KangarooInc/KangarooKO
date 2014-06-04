#ifndef _ANGEL_H_
#define _ANGEL_H
#include <GL/glx.h>
#include "ppm.h"
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define random(a) (rand()%a)
typedef double Flt;
typedef double Vec[3];
extern Ppmimage *appleImage;
extern Ppmimage *bappleImage;
extern GLuint appletexture;
extern GLuint bappletexture;
void apple_render();
void bapple_render();
void applevector (void);
void bad_applevector (void);
void applecollision(void);
void bapplecollision(void);
void badapple_render();
void move_apple();
void move_bapple();
void appleReset(void);
void bappleReset(void);
void applePhysics();
void bapplePhysics();
extern void init_apple_texture();
extern void init_bapple_texture();
extern int high_score;
extern unsigned char *buildAlphaData(Ppmimage *img);
Flt d0,d1,dist;
extern int show_apple;
Flt punch_dist, hit_dist, apple_hit_dist, bapple_hit_dist;
extern int show_bapple;
typedef struct t_apple 
{
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Apple;
Apple apple;

typedef struct t_bapple 
{
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Bapple;
Bapple bapple;
#endif
