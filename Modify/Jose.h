#ifndef _JOSE_H_
#define _JOSE_H_
#include <GL/glx.h>
#include "fonts.h"

#define random(a) (rand()%a)
#define MAXBUTTONS 8

// Variables
extern float wid;
extern int white;
extern int lives;
extern int high_score;
extern int start;
extern int nbuttons;
extern int rbutton;
extern int lbutton;
extern int show_kangaroo;
extern int show_rhino;
extern int show_animal;
extern int show_ufo;
extern int done;
extern int ufocount;
extern int white_image;

// OpenGL variables
extern GLuint KangarooTexture;
extern GLuint RhinoTexture;
extern GLuint AnimalTexture;
extern GLuint MountainTexture;
extern GLuint LevelTexture;
extern GLuint StartTexture;
extern GLuint WhiteTexture;

// Prototypes
void restartGame(void);
void draw_kangaroo(void);
void draw_rhino(void);
void draw_animal(void);
void draw_white(void);
void draw_background(void);
void perspective(void);
void StartMenu(void);
void mouse_click(int action);
void check_mouse(XEvent *e);
void buttonRender (void);
void buttonsInit(void);
void ufoReset(void);
void rhinoReset(void);
void animalReset(void);
void kangarooReset(void);
void kangarooDeath(void);

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
