#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_
//gameover.h
#include "fonts.h"
#include "ppm.h"
// Variables
extern int done;
extern int lbutton;
extern int rbutton;
extern int nbuttons;
extern int show_kangaroo;
extern int show_rhino;
Ppmimage *gameoverImage;
extern GLuint gameoverTexture;
#define GOMAXBUTTONS 8


// Prototypes
void GObuttonsInit(void);
void bRender(void);
void GOmouse_click(int action);
extern void GameOver(void);


//Struct
typedef struct GOt_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} GOButton;
GOButton GObutton[GOMAXBUTTONS];

#endif
