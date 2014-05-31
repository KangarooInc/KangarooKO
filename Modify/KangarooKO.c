//Kangaroo KO
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "timing.h"
#include "xwin.h"
#include "gameover.h"
#include "startmenu.h"
#include "bjarne.h"
//#include "hop.h"
#include "minigame.h"

#define USE_SOUND

#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif //USE_SOUND

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//X Windows variables
Display *dpy;
Window w;
GLXContext glc;

//function prototypes
void initXWindows(void);
void init_opengl(void);
void setup_screen_res(const int, const int);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void GOcheck_mouse(XEvent *e);
void check_keys(XEvent *e);
void init();
void init_sounds(void);
void physics(void);
void render(void);
void restartGame();
void rhinoReset(void);
void animalReset(void);
void kangarooReset(void);
void kangarooDeath(void);
void draw_kangaroo(void);
void draw_rhino(void);
void draw_animal(void);
void draw_ufo(void);
void draw_background(void);
void draw_white(void);
void perspective(void);

//structures
typedef struct t_rhino {
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Rhino;
Rhino rhino;

typedef struct t_animal {
    Vec pos;
    Vec vel;
    float height;
    float height2;
} Animal;
Animal animal;

typedef struct t_ufo {
    Vec pos;
    Vec vel;
} Ufo;
Ufo ufo;

typedef struct t_kangaroo {
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float height;
    float height2;
} Kangaroo;
Kangaroo kangaroo;

Ppmimage *kangarooImage=NULL;
Ppmimage *punchleftImage = NULL;
Ppmimage *punchrightImage = NULL;
//Ppmimage *punch3Image = NULL;
Ppmimage *lowhopImage;
Ppmimage *highhopImage;
Ppmimage *levelImage=NULL;
Ppmimage *mountainsImage=NULL;
Ppmimage *startImage=NULL;
Ppmimage *whiteImage=NULL;
Ppmimage *rhinoImage=NULL;
Ppmimage *animalImage=NULL;
Ppmimage *ufoImage=NULL;
Ppmimage *gameoverImage=NULL;
Ppmimage *car1Image=NULL;
Ppmimage *car2Image=NULL;
Ppmimage *car3Image=NULL;
Ppmimage *backgroundImage=NULL;
GLuint KangarooTexture;
GLuint RhinoTexture;
GLuint AnimalTexture;
GLuint UFOTexture;
GLuint MountainTexture;
GLuint LevelTexture;
GLuint StartTexture;
GLuint WhiteTexture;
GLuint punchleftTexture;
GLuint punchrightTexture;
GLuint lowhopTexture;
GLuint highhopTexture;
//GLuint punch3Texture;
GLuint GameOverTexture; //------------------------
GLuint minitexture;
GLuint car1texture;
GLuint car2texture;
GLuint car3texture;

//variables
int done;
int lbutton = 0;
int rbutton = 0;
int nbuttons = 0;
int show_rhino = 0;
int show_animal = 0;
int show_kangaroo = 1;
int show_ufo = 0;
int show_car = 0;
int show_MiniGame = 0;
int punch_count = 0;
int background = 1;
int start = 1;
int white = 0;
int gameover = 1; //------------------
int lives = 3;
int high_score = 0;    // high score tracker, prints in render()
int punch = 0;
int punch_image = 0;
int hop = 0;
int hop_image = 0;
float wid = 120.0f;
static double setLevel = 0.0;
static double setMountain = 0.0;
#ifdef USE_SOUND
int play_sounds = 1;
#endif //USE_SOUND
//
//static time_t seconds;

int main(void)
{
    logOpen();
    initXWindows();
    init_opengl();
    init();
    //buttonsInit();------------------------------------------------------------------
    init_sounds();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    while(!done) {
        while(XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            check_resize(&e);
            check_mouse(&e);
            GOcheck_mouse(&e);
            check_keys(&e);
        }
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        timeCopy(&timeStart, &timeCurrent);
        physicsCountdown += timeSpan;
        while(physicsCountdown >= physicsRate) {
            physics();
            physicsCountdown -= physicsRate;
        }
        render();
        glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
#ifdef USE_SOUND
    fmod_cleanup();
#endif //USE_SOUND
    logClose();
    return 0;
}

unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //
        //get the alpha value
        //
        //original code
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //		(int)*(ptr+0) +
        //		(int)*(ptr+1) +
        //		(int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //
        //new code, suggested by Chris Smith, Fall 2013
        *(ptr+3) = (a|b|c);
        //
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, xres, yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, xres, 0, yres, -1, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    kangarooImage    = ppm6GetImage("./images/ricky.ppm");
    levelImage       = ppm6GetImage("./images/LevelBlack.ppm");
    mountainsImage   = ppm6GetImage("./images/BackBlack.ppm");
    startImage       = ppm6GetImage("./images/blank.ppm");
    whiteImage       = ppm6GetImage("./images/white.ppm");
    gameoverImage    = ppm6GetImage("./images/gameover.ppm"); //-------------------------------------------------------
    rhinoImage       = ppm6GetImage("./images/rhino.ppm");
    animalImage      = ppm6GetImage("./images/altrhino.ppm");
    ufoImage	     = ppm6GetImage("./images/ufo.ppm");
    //
    //create opengl texture elements
    glGenTextures(1, &RhinoTexture);
    glGenTextures(1, &AnimalTexture);
    glGenTextures(1, &UFOTexture);
    glGenTextures(1, &KangarooTexture);
    glGenTextures(1, &LevelTexture);
    glGenTextures(1, &MountainTexture);
    glGenTextures(1, &StartTexture);
    glGenTextures(1, &WhiteTexture);
    glGenTextures(1, &GameOverTexture); //-----------------------------------------------------------------

    //-------------------------------------------------------------------------
    //
    // Kangaroo
    //
    int w = kangarooImage->width;
    int h = kangarooImage->height;

    glBindTexture(GL_TEXTURE_2D, KangarooTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *Transparent = buildAlphaData(kangarooImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // Kangaroo - Punch
    //
    init_punch_texture(w,h);
    //-------------------------------------------------------------------------
    //
    // Kangaroo - Hop
    //
    init_hop_texture(w,h);
    //-------------------------------------------------------------------------
    // MiniGame BackGround
    init_MiniGame();
    //
    // Rhino
    //
    int w1 = rhinoImage->width;
    int h1 = rhinoImage->height;

    glBindTexture(GL_TEXTURE_2D, RhinoTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(rhinoImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w1, h1, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // Animal
    //
    int w2 = animalImage->width;
    int h2 = animalImage->height;

    glBindTexture(GL_TEXTURE_2D, AnimalTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(animalImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // UFO Spaceship
    //
    int w3 = ufoImage->width;
    int h3 = ufoImage->height;

    glBindTexture(GL_TEXTURE_2D, UFOTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(ufoImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w3, h3, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // Background - Platforms
    //
    glBindTexture(GL_TEXTURE_2D, LevelTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(levelImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, levelImage->width, levelImage->height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // Background - Mountains
    //
    glBindTexture(GL_TEXTURE_2D, MountainTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    Transparent = buildAlphaData(mountainsImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mountainsImage->width, mountainsImage->height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, Transparent);
    free(Transparent);
    //-------------------------------------------------------------------------
    //
    // Background - Mini Game
    //
    init_MiniGame();
    //-------------------------------------------------------------------------
    //
    // Start Page
    //
    glBindTexture(GL_TEXTURE_2D, StartTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            startImage->width, startImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, startImage->data);
    //-------------------------------------------------------------------------
    //
    // Death flash
    //
    glBindTexture(GL_TEXTURE_2D, WhiteTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            whiteImage->width, whiteImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, whiteImage->data);
    //-------------------------------------------------------------------------
    //
    // Gameover
    //
    glBindTexture(GL_TEXTURE_2D, GameOverTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            gameoverImage->width, gameoverImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, gameoverImage->data);
    //-------------------------------------------------------------------------
}

void init_sounds(void)
{
#ifdef USE_SOUND
    //FMOD_RESULT result;
    if (fmod_init()) {
        printf("ERROR - fmod_init()\n\n");
        return;
    }
    if (fmod_createsound("./sounds/Intro Quack.mp3", 0)) {
        printf("ERROR - fmod_createsound()\n\n");
        return;
    }
    if (fmod_createsound("./sounds/KillingJokeIntro.mp3", 1)) {
        printf("ERROR - fmod_createsound()\n\n");
        return;
    }
    if (fmod_createsound("./sounds/PunchSoundEffect.mp3", 2)) {
        printf("ERROR - fmod_createsound()\n\n");
        return;
    }
    fmod_setmode(0,FMOD_LOOP_NORMAL);
    fmod_playsound(0);
    //fmod_systemupdate();
#endif //USE_SOUND
}

void init() {
    kangaroo.pos[0] = 60.0;
    kangaroo.pos[1] = 60.0;
    VecCopy(kangaroo.pos, kangaroo.lastpos);
    kangaroo.width = 200.0;
    kangaroo.width2 = kangaroo.width * 0.5;
    kangaroo.height = 100.0;
    kangaroo.height2 = kangaroo.height * 0.5;
    kangaroo.shape = 1;

    MakeVector(150.0,180.0,0.0, rhino.pos);
    MakeVector(-6.0,0.0,0.0, rhino.vel);
    rhino.height = 200.0;
    rhino.height2 = rhino.height * 0.5;

    MakeVector(150.0,180.0,0.0, animal.pos);
    MakeVector(-6.0,0.0,0.0, animal.vel);
    animal.height = 200.0;
    animal.height2 = animal.height * 0.5;

    MakeVector(300.0,600.0,0.0, ufo.pos);
    MakeVector(0.0,-6.0,0.0, ufo.vel);
}

void restartGame()
{
    lives = 3;
    high_score = 0;
    rhinoReset();
    animalReset();
    kangarooReset();
}

void rhinoReset(void)
{
    show_rhino = 1;

    int i = random(4);

    rhino.pos[0] = (float)xres + 150;
    if (i == 1)
        rhino.pos[1] = 260.0;
    else if (i == 2)
        rhino.pos[1] = 190.0;
    else if (i == 3)
        rhino.pos[1] = 120.0;
    else
        rhino.pos[1] = 60.0;
}

void animalReset(void)
{
    show_animal = 1;

    int i = random(4);

    animal.pos[0] = (float)xres + 150;
    if (i == 1)
        animal.pos[1] = 260.0;
    else if (i == 2)
        animal.pos[1] = 190.0;
    else if (i == 3)
        animal.pos[1] = 120.0;
    else
        animal.pos[1] = 60.0;
}

void kangarooReset(void)
{
    show_kangaroo = 1;

    kangaroo.pos[1] = 60.0;
    kangaroo.pos[0] = 60.0;
}

void kangarooDeath(void)
{
    lives--;
    rhinoReset();
    animalReset();
    kangarooReset();

}

void check_keys(XEvent *e)
{
    //keyboard input?
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            return;
    }
    if (e->type == KeyPress) {
        if (key == XK_Shift_L || key == XK_Shift_R) {
            return;
        }
    } else {
        return;
    }

    Flt punch_dist, hit_dist;
    switch(key) {
        /*Dont need this because start button;
          case XK_Return:
          start = 0;
          break;
          */
        case XK_a:
            show_ufo ^= 1;
            break;
        case XK_j:
            kangarooDeath();
            break;
        case XK_k:
            lives += 1;
            break;
        case XK_l:
            lives -= lives;
            break;
        case XK_Left:
            if (!(kangaroo.pos[0] - kangaroo.width2 < (xres-xres))) {
                VecCopy(kangaroo.pos, kangaroo.lastpos);
                hop ^= 1;
                kangaroo.pos[0] -= 67.0;
            }
            break;
        case XK_Right:
            if (!(kangaroo.pos[0] + kangaroo.width2 >= xres)) {
                VecCopy(kangaroo.pos, kangaroo.lastpos);
                hop ^= 1;
                kangaroo.pos[0] += 67.0;
            }
            break;
        case XK_Up:
            if (!(kangaroo.pos[1] >= (float)yres/2)) {
                VecCopy(kangaroo.pos, kangaroo.lastpos);
                hop ^= 1;
                kangaroo.pos[1] += 67.0;
            }
            break;
        case XK_Down:
            if (!(kangaroo.pos[1] < 100.0)) {
                VecCopy(kangaroo.pos, kangaroo.lastpos);
                hop ^= 1;
                kangaroo.pos[1] -= 67.0;
            }
            break;
        case XK_space:
            /*punch_image+=1;*/
            punch ^= 1;
            fmod_playsound(2);

            punch_dist = kangaroo.pos[0] + kangaroo.height2;
            hit_dist = rhino.pos[0] - rhino.height2;
            if (rhino.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
                    && rhino.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
                if ((hit_dist - punch_dist) >= 25 && (hit_dist - punch_dist <= 90.0))
                {
                    if (show_rhino) {
                        rhinoReset();
                    }
                    high_score += 100;
                }
            }
            hit_dist = animal.pos[0] - animal.height2;
            if (animal.pos[1] >= (kangaroo.pos[1] - kangaroo.height2)
                    && animal.pos[1] <= (kangaroo.pos[1] + kangaroo.height2)) {
                if ((hit_dist - punch_dist) >= 25 && (hit_dist - punch_dist <= 90.0))
                {
                    if (show_animal) {
                        animalReset();
                    }
                    high_score += 500;
                }
            }

            if(show_MiniGame)
            {
                punch_count++;
            }
            break;
        case XK_Escape:
            done=1;
            break;
    }
}

void move_rhino()
{
    //auto move rhino...
    //int addgrav = 1;
    //Update position
    rhino.pos[0] += rhino.vel[0]*1.5;
    rhino.pos[1] += rhino.vel[1];
    //Check for collision with window edges
    if ((rhino.pos[0] < -140.0 && rhino.vel[0] < 0.0) ||
            (rhino.pos[0] >= (float)xres+140.0 && rhino.vel[0] > 0.0)) {
        rhino.vel[0] = -rhino.vel[0];
        rhinoReset();
    }
}

void move_animal()
{
    //auto move animal...
    //int addgrav = 1;
    //Update position
    animal.pos[0] += animal.vel[0]*2;
    animal.pos[1] += animal.vel[1];
    //Check for collision with window edges
    if ((animal.pos[0] < -140.0 && animal.vel[0] < 0.0) ||
            (animal.pos[0] >= (float)xres+140.0 && animal.vel[0] > 0.0)) {
        animal.vel[0] = -animal.vel[0];
        animalReset();
    }
}

void move_ufo()
{
    ufo.pos[1] += ufo.vel[1];
}

void physics(void)
{
    Flt d0,d1,dist;
    Flt hit_dist;

    if(punch)
    {
        punch_image += 1;
        if(punch_image == 3)
        {
            punch_image = 0;
            punch ^= 1;
        }
    }

    if(hop)
    {
        hop_image += 1;
        if(hop_image == 3)
        {
            hop_image = 0;
            hop ^= 1;
        }
    }

    if (show_rhino) {
        move_rhino();
        hit_dist = rhino.pos[0] - 100.0;
        d0 = kangaroo.pos[0] - hit_dist;
        d1 = kangaroo.pos[1] - rhino.pos[1];
        dist = d0*d0+d1*d1;
        if (dist < (50.0*50.0)) {
            kangarooDeath();
        }
    }

    if (show_animal) {
        move_animal();
        hit_dist = animal.pos[0] - 100.0;
        d0 = kangaroo.pos[0] - hit_dist;
        d1 = kangaroo.pos[1] - animal.pos[1];
        dist = d0*d0+d1*d1;
        if (dist < (50.0*50.0)) {
            kangarooDeath();
        }
    }

    if (show_ufo)
        move_ufo();

}

void draw_kangaroo(void)
{
    //Log("draw_kangaroo()...\n");
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, KangarooTexture);
    glBegin(GL_QUADS);
    float w = kangaroo.width2;
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

void draw_rhino(void)
{
    glPushMatrix();
    glTranslatef(rhino.pos[0], rhino.pos[1], rhino.pos[2]);

    glBindTexture(GL_TEXTURE_2D, RhinoTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    //This is for the rhino image going right to left
    glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    //#################################################
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}

void draw_animal(void)
{
    glPushMatrix();
    glTranslatef(animal.pos[0], animal.pos[1], animal.pos[2]);

    glBindTexture(GL_TEXTURE_2D, AnimalTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    //This is for the animal image going right to left
    glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    //#################################################
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}

void draw_ufo(void)
{
    glPushMatrix();
    glTranslatef(ufo.pos[0], ufo.pos[1], ufo.pos[2]);
    glBindTexture(GL_TEXTURE_2D, UFOTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    if (ufo.vel[0] > 0.0) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    } else {
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}

void draw_white(void)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, WhiteTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
}

void draw_background(void)
{
    double *yOFFsetLevel = &setLevel;
    double *yOFFsetMountain = &setMountain;

    if(!start)
    {
        *yOFFsetLevel -= 1;
        /*if(*yOFFsetLevel>100.0)*/
        /**yOFFsetLevel -=100.0;*/

        *yOFFsetMountain -= .25;
        /*if(*yOFFsetMountain>100.0)*/
        /**yOFFsetMountain -=100.0;*/
    }

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, MountainTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslatef(*yOFFsetMountain, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres*5, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres*5, 0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, LevelTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glTranslatef(*yOFFsetLevel, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres*5, yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres*5, 0);
    glEnd();
    glPopMatrix();
}

void perspective(void)
{

    /////////////////////////////////////////////////////////////
    //For the Perspective
    if (kangaroo.pos[1] < rhino.pos[1])
    {
        if (kangaroo.pos[1] < animal.pos[1])
        {
            if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
            else if (animal.pos[1] >= rhino.pos[1]){
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
        }

        else if (kangaroo.pos[1] >= animal.pos[1])
        {
            if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
            }
            else if (animal.pos[1] >= rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
        }
    }
    //////////////////////////////////////////

    else if (kangaroo.pos[1] >= rhino.pos[1])
    {
        if (kangaroo.pos[1] >= animal.pos[1])
        {
            if (animal.pos[1] >= rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
            else if (animal.pos[1] < rhino.pos[1]){
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
            }
        }

        else if (kangaroo.pos[1] < animal.pos[1])
        {
            if (animal.pos[1] >= rhino.pos[1]){
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
                if (show_rhino) {
                    draw_rhino();
                }
            }
            else if (animal.pos[1] < rhino.pos[1]){
                if (show_rhino) {
                    draw_rhino();
                }
                if (show_animal) {
                    draw_animal();
                }
                if (show_kangaroo) {
                    draw_kangaroo();
                    punch_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                    hop_render(kangaroo.pos[0],kangaroo.pos[1],kangaroo.pos[2]);
                }
            }
        }
    }
}

void render(void)
{
    Rect r;
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    //
    //draw a quad with texture
    glColor3f(1.0, 1.0, 1.0);

    //Loop the background
    double *yOFFsetLevel = &setLevel;
    double *yOFFsetMountain = &setMountain;

    if(*yOFFsetLevel <= -(levelImage->width))
        *yOFFsetLevel = 0.0;
    if(*yOFFsetMountain <= -(mountainsImage->width))
        *yOFFsetMountain = 0.0;
    /////////////////////////////////////////////

    if (background) {
        draw_background();
    }
    if (start) {
        StartMenu();
    }
    if(white)
    {
        draw_white();
    }
    if (lives <= 0) {
        GameOver();
    }

    perspective();

    if (high_score >= 1000) {
        draw_ufo();
        show_ufo = 1;
    }

    if (high_score >= 5000) {
        show_MiniGame = 1;
    }

    if (show_MiniGame) {
        mini_render();
    }

    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    unsigned int cref = 0x00ffffff;

    if(start)
    {
        r.bot = yres - 100;
        r.left = xres/2;
        r.center = 1;
        unsigned int cref = 0x00ffffff;

        //Help menu only show in start
        ggprint8b(&r, 16, cref, "Move Up: UP Arrow Key");
        ggprint8b(&r, 16, cref, "Move Down: Down Arrow Key");
        ggprint8b(&r, 16, cref, "Move Left: Left Arrow Key");
        ggprint8b(&r, 16, cref, "Move Right: Right Arrow Key");
        ggprint8b(&r, 16, cref, "Punch: Space Key");
    }
    if(show_kangaroo && show_rhino)
    {
        //In game menu, only show in game
        ggprint16(&r, 16, cref, "Lives: %i", lives);
        ggprint16(&r, 16, cref, "High Score: %i", high_score);
    }
}
