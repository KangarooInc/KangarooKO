#ifndef _GABE_H_
#define _GABE_H_

#include <GL/glx.h>
#include <time.h>
#include "ppm.h"

#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)

extern int lives;
extern float wid;
extern int yellow;
extern int show_ufo;
extern int ufocount;
extern int ufochoice;
extern int yellow_image;
unsigned char *Transparent;

extern Ppmimage *ufoImage;
extern Ppmimage *yellowImage;
extern GLuint UFOTexture;
extern GLuint YellowTexture;

void ufoReset(void);
void ufoFlash(void);
void ufoFreeze(void);
void move_ufo(void);
void draw_ufo(void);
void ufoOpenGL(void);
void init_ufo(void);
void rhinoReset(void);
void draw_yellow(void);
void animalReset(void);
void kangarooReset(void);
unsigned char *buildAlphaData();
//void fileWrite(unsigned double userScore)
#endif
