#ifndef _MINIGAME_H_
#define _MINIGAME_H_
#include  "ppm.h"
extern int show_car;
extern int show_rhino;
extern int show_MiniGame;
extern int show_animal;
extern int punch_count;
extern GLuint minitexture;
extern GLuint car1texture;
extern GLuint car2texture;
extern GLuint car3texture;

extern Ppmimage *car1Image;
extern Ppmimage *car2Image;
extern Ppmimage *car3Image;
extern Ppmimage *backgroundImage;
extern unsigned char *Transparent;
extern void init_MiniGame();
extern void mini_render();
extern unsigned char *buildAlphaData(Ppmimage *img);
#endif
