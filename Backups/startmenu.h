#ifndef _STARTMENU_H_
#define _STARTMENU_H_

#include <GL/glx.h>
#include "fonts.h"

// Variables
extern int done;
extern int start;
extern int lbutton;
extern int rbutton;
extern int nbuttons;
extern int show_kangaroo;
extern int show_rhino;
extern int show_animal;
extern GLuint StartTexture;
#define MAXBUTTONS 8


// Prototypes
void buttonsInit(void);
void buttonRender (void);
void mouse_click(int action);
void rhinoReset(void);
void animalReset(void);
void kangarooReset(void);
void StartMenu(void);


// Struct
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;
Button button[MAXBUTTONS];
#endif
