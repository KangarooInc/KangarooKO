#ifndef _MINIGAME_H_
#define _MINIGAME_H_
#include  "ppm.h"
extern int show_car;
extern int show_rhino;
extern int show_MiniGame;
extern int show_animal;
extern GLunit minitexture;
extern GLunit car1texture;
extern GLunit car2texture;
extern GLunit car3texture;

extern Ppmimage *car1Image;
extern Ppmimage *car2Image;
extern Ppmimage *car3Image;
extern Ppmimage *backroundImage;
extern unsigned char *Transparent;
extern void init_MiniGame();
extern void mini_render();
#endif
