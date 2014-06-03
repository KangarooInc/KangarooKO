#ifndef _BJARNE_H_
#define _BJARNE_H_
#include <GL/glx.h>
#include "ppm.h"

// Definition
typedef double Flt;

// Variables
extern int hop;
extern int lives;
extern int hop_image;
extern int punch;
extern int punch_image;
extern int show_kangaroo;
extern int show_rhino;
extern int show_ufo;
extern int high_score;
extern int ufocount;
extern int start;
extern int ufochoice;
extern int show_animal;
extern int white;
extern int white_image;
extern int yellow;
extern int yellow_image;
extern unsigned char *Transparent;
extern unsigned char *buildAlphaData(Ppmimage *img);

// OpenGL Stuff
extern Ppmimage *lowhopImage;
extern Ppmimage *highhopImage;
extern Ppmimage *punchleftImage;
extern Ppmimage *punchrightImage;
extern GLuint lowhopTexture;
extern GLuint highhopTexture;
extern GLuint punchleftTexture;
extern GLuint punchrightTexture;


// Functions
void punchKey(void);
void move_rhino(void);
void move_animal(void);
void physics(void);
void move_ufo(void);
void rhinoReset(void);
void animalReset(void);
void kangarooDeath(void);
void hop_render(double x, double y, double z);
void init_hop_texture(int, int);
void punch_render(double x, double y, double z);
void init_punch_texture(int, int);
#endif
