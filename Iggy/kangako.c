//cs335 Spring 2014
//
//program: lab2_rainforest.c
//author:  Gordon Griesel
//date:    2014
//
//Must add code to remove a raindrop node from the linked-list
//when it stops or leaves the screen.
//
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
Window win;
GLXContext glc;

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void init();
void init_sounds(void);
void physics(void);
void render(void);

/*
//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
	(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------*/

int done=0;
//int xres=800, yres=600;

typedef struct t_kangaroo {
    Vec pos;
    Vec vel;
} Kangaroo;
Kangaroo kangaroo;

typedef struct t_rhino {
    Vec pos;
    Vec vel;
} Rhino;
Rhino rhino;

typedef struct t_ufo {
    Vec pos;
    Vec vel;
} Ufo;
Ufo ufo;

Ppmimage *kangarooImage=NULL;
Ppmimage *backgroundImage=NULL;
Ppmimage *umbrellaImage=NULL;
Ppmimage *rhinoImage=NULL;
Ppmimage *ufoImage=NULL;
GLuint kangarooTexture;
GLuint rhinoTexture;
GLuint ufoTexture;
GLuint silhouetteTexture;
GLuint silhouetteTexture1;
GLuint silhouetteTexture2;
GLuint backgroundTexture;
GLuint umbrellaTexture;
int show_kangaroo=0;
int background=1;
int show_rhino=0;
int show_ufo=0;
int silhouette=1;
int silhouette1=1;
int silhouette2=2;
#ifdef USE_SOUND
int play_sounds = 0;
#endif //USE_SOUND
//
//
#define USE_UMBRELLA
#ifdef USE_UMBRELLA
#define UMBRELLA_FLAT  0
#define UMBRELLA_ROUND 1
typedef struct t_umbrella {
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float radius;
} Umbrella;
Umbrella umbrella;
int show_umbrella=0;
int deflection=0;
#endif //USE_UMBRELLA


int main(void)
{
    logOpen();
    initXWindows();
    init_opengl();
    init();
    init_sounds();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    while(!done) {
	while(XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_resize(&e);
	    check_mouse(&e);
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

/*void cleanupXWindows(void)
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Kangaroo K.O.");
}

void setup_screen_res(const int w, const int h)
{
    xres = w;
    yres = h;
}

void initXWindows(void)
{
    Window root;
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;

    setup_screen_res(640, 480);
    dpy = XOpenDisplay(NULL);
    if(dpy == NULL) {
	printf("\n\tcannot connect to X server\n\n");
	exit(EXIT_FAILURE);
    }
    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
	printf("\n\tno appropriate visual found\n\n");
	exit(EXIT_FAILURE);
    } 
    //else {
    //	// %p creates hexadecimal output like in glxinfo
    //	printf("\n\tvisual %p selected\n", (void *)vi->visualid);
    //}
    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
	    vi->depth, InputOutput, vi->visual,
	    CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}*/

void reshape_window(int width, int height)
{
    //window has been resized.
    setup_screen_res(width, height);
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, xres, 0, yres, -1, 1);
    set_title();
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
    kangarooImage    = ppm6GetImage("./images/kanga.ppm");
    backgroundImage  = ppm6GetImage("./images/background2.ppm");
    rhinoImage       = ppm6GetImage("./images/rhino.ppm");
    ufoImage	 = ppm6GetImage("./images/ufo.ppm");
    umbrellaImage    = ppm6GetImage("./images/kanga.ppm");
    //
    //create opengl texture elements
    glGenTextures(1, &kangarooTexture);
    glGenTextures(1, &silhouetteTexture);
    glGenTextures(1, &silhouetteTexture1);
    glGenTextures(1, &silhouetteTexture2);
    glGenTextures(1, &backgroundTexture);
    glGenTextures(1, &rhinoTexture);
    glGenTextures(1, &umbrellaTexture);
    //-------------------------------------------------------------------------
    //kangaroo
    //
    int w = kangarooImage->width;
    int h = kangarooImage->height;
    //
    glBindTexture(GL_TEXTURE_2D, kangarooTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, kangarooImage->data);
    //-------------------------------------------------------------------------
    //
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silhouetteData = buildAlphaData(kangarooImage);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    free(silhouetteData);
    //-------------------------------------------------------------------------
    //
    //rhino
    //
    int w1 = rhinoImage->width;
    int h1 = rhinoImage->height;

    glBindTexture(GL_TEXTURE_2D, rhinoTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w1, h1, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, rhinoImage->data);

    glBindTexture(GL_TEXTURE_2D, silhouetteTexture1);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silhouetteData1 = buildAlphaData(rhinoImage);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w1, h1, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData1);
    free(silhouetteData1);
    //
    //-------------------------------------------------------------------------
    //ufo spaceship
    int w2 = ufoImage->width;
    int h2 = ufoImage->height;

    glBindTexture(GL_TEXTURE_2D, ufoTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, ufoImage->data);

    glBindTexture(GL_TEXTURE_2D, silhouetteTexture2);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silhouetteData2 = buildAlphaData(ufoImage);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData2);
    free(silhouetteData2);
    //
    //-------------------------------------------------------------------------
    //umbrella
    //
    glBindTexture(GL_TEXTURE_2D, umbrellaTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    silhouetteData = buildAlphaData(umbrellaImage);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    free(silhouetteData);
    //-------------------------------------------------------------------------
    //
    //background
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
	    backgroundImage->width, backgroundImage->height,
	    0, GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);
    //-------------------------------------------------------------------------
}

void check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != xres || xce.height != yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}

void init_sounds(void)
{
#ifdef USE_SOUND
    //FMOD_RESULT result;
    if (fmod_init()) {
	printf("ERROR - fmod_init()\n\n");
	return;
    }
    if (fmod_createsound("./sounds/tick.wav", 0)) {
	printf("ERROR - fmod_createsound()\n\n");
	return;
    }
    if (fmod_createsound("./sounds/drip.wav", 1)) {
	printf("ERROR - fmod_createsound()\n\n");
	return;
    }
    fmod_setmode(0,FMOD_LOOP_OFF);
    //fmod_playsound(0);
    //fmod_systemupdate();
#endif //USE_SOUND
}

void init() {
#ifdef USE_UMBRELLA
    umbrella.pos[0] = 100.0;
    umbrella.pos[1] = (double)(yres-195);
    VecCopy(umbrella.pos, umbrella.lastpos);
    umbrella.width = 200.0;
    umbrella.width2 = umbrella.width * 0.5;
    umbrella.radius = (float)umbrella.width2;
    umbrella.shape = UMBRELLA_FLAT;
#endif //USE_UMBRELLA
    MakeVector(-150.0,180.0,0.0, kangaroo.pos);
    MakeVector(6.0,0.0,0.0, kangaroo.vel);

    MakeVector(150.0,180.0,0.0, rhino.pos);
    MakeVector(-6.0,0.0,0.0, rhino.vel);

    MakeVector(300.0,600.0,0.0, ufo.pos);
    MakeVector(0.0,-6.0,0.0, ufo.vel);
}

void check_mouse(XEvent *e)
{
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button is down
	}
	if (e->xbutton.button==3) {
	    //Right button is down
	}
    }
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
	//Mouse moved
	savex = e->xbutton.x;
	savey = e->xbutton.y;
    }
}

void check_keys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return;
    }
    if (e->type == KeyPress) {
	if (key == XK_Shift_L || key == XK_Shift_R) {
	    shift=1;
	    return;
	}
    } else {
	return;
    }
    //
    switch(key) {
	case XK_k:
	    show_kangaroo ^= 1;
	    if (show_kangaroo) {
		kangaroo.pos[0] = -250.0;
	    }
	    break;
	case XK_r:
	    show_rhino ^= 1;
	    break;
	case XK_a:
	    show_ufo ^= 1;
	    break;
	case XK_d:
	    deflection ^= 1;
	    break;
	case XK_b:
	    background ^= 1;
	    break;
	case XK_s:
	    silhouette ^= 1;
	    printf("silhouette: %i\n",silhouette);
	    break;
	case XK_u:
	    show_umbrella ^= 1;
	    break;
	case XK_p:
	    umbrella.shape ^= 1;
	    break;
	case XK_Left:
	    VecCopy(umbrella.pos, umbrella.lastpos);
	    umbrella.pos[0] -= 10.0;
	    break;
	case XK_Right:
	    VecCopy(umbrella.pos, umbrella.lastpos);
	    umbrella.pos[0] += 10.0;
	    break;
	case XK_Up:
	    VecCopy(umbrella.pos, umbrella.lastpos);
	    umbrella.pos[1] += 65.0;
	    break;
	case XK_Down:
	    VecCopy(umbrella.pos, umbrella.lastpos);
	    umbrella.pos[1] -= 65.0;
	    break;
	case XK_n:
	    play_sounds ^= 1;
	    break;
	case XK_w:
	    if (shift) {
		//shrink the umbrella
		umbrella.width *= (1.0 / 1.05);
	    } else {
		//enlarge the umbrella
		umbrella.width *= 1.05;
	    }
	    //half the width
	    umbrella.width2 = umbrella.width * 0.5;
	    umbrella.radius = (float)umbrella.width2;
	    break;
	case XK_Escape:
	    done=1;
	    break;
    }
}

void normalize(Vec vec)
{
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    Flt len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
	MakeVector(0.0,0.0,1.0,vec);
	return;
    }
    len = 1.0 / sqrt(len);
    vec[0] = xlen * len;
    vec[1] = ylen * len;
    vec[2] = zlen * len;
}

void move_kangaroo()
{
    //move kangaroo...
    //int addgrav = 1;
    //Update position
    kangaroo.pos[0] += kangaroo.vel[0];
    kangaroo.pos[1] += kangaroo.vel[1];
    //Check for collision with window edges
    if ((kangaroo.pos[0] < -140.0 && kangaroo.vel[0] < 0.0) ||
	    (kangaroo.pos[0] >= (float)xres+140.0 && kangaroo.vel[0] > 0.0)) {
	kangaroo.vel[0] = -kangaroo.vel[0];
	//addgrav = 0;
    }
    if ((kangaroo.pos[1] < 150.0 && kangaroo.vel[1] < 0.0) ||
	    (kangaroo.pos[1] >= (float)yres && kangaroo.vel[1] > 0.0)) {
	kangaroo.vel[1] = -kangaroo.vel[1];
	//addgrav = 0;
    }
    /*Gravity
      if (addgrav)
      kangaroo.vel[1] -= 0.75;*/
}

void move_rhino()
{
    rhino.pos[1] += rhino.vel[1];
}

void move_ufo()
{
    ufo.pos[1] += ufo.vel[1];
}

void physics(void)
{
    if (show_kangaroo)
	move_kangaroo();
    if (show_rhino)
	move_rhino();
    if (show_ufo)
	move_ufo();
}

#ifdef USE_UMBRELLA
void draw_umbrella(void)
{
    //Log("draw_umbrella()...\n");
    if (umbrella.shape == UMBRELLA_FLAT) {
	glColor4f(1.0f, 0.2f, 0.2f, 0.5f);
	glLineWidth(8);
	glBegin(GL_LINES);
	glVertex2f(umbrella.pos[0]-umbrella.width2, umbrella.pos[1]);
	glVertex2f(umbrella.pos[0]+umbrella.width2, umbrella.pos[1]);
	glEnd();
	glLineWidth(1);
    } else {
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glTranslatef(umbrella.pos[0],umbrella.pos[1],umbrella.pos[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, umbrellaTexture);
	glBegin(GL_QUADS);
	float w = umbrella.width2;
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
	glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
	glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
    }
}
#endif //USE_UMBRELLA

void render(void)
{
    Rect r;
    static double yOFFset = 0.0;
    yOFFset -= 0.01;
    if(yOFFset>100.0)
	yOFFset -=100.0;

    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    //
    //draw a quad with texture
    float wid = 120.0f;
    glColor3f(1.0, 1.0, 1.0);
    if (background) {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTranslatef(yOFFset, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres*20, yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres*20, 0);
	glEnd();
	glPopMatrix();
    }
    if (show_kangaroo) {
	glPushMatrix();
	glTranslatef(kangaroo.pos[0], kangaroo.pos[1], kangaroo.pos[2]);
	if (!silhouette) {
	    glBindTexture(GL_TEXTURE_2D, kangarooTexture);
	} else {
	    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	    glEnable(GL_ALPHA_TEST);
	    glAlphaFunc(GL_GREATER, 0.0f);
	    glColor4ub(255,255,255,255);
	}
	glBegin(GL_QUADS);
	if (kangaroo.vel[0] > 0.0) {
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
    if (show_rhino) {
	glPushMatrix();
	glTranslatef(rhino.pos[0], rhino.pos[1], rhino.pos[2]);
	if (!silhouette1) {
	    glBindTexture(GL_TEXTURE_2D, rhinoTexture);
	} else {
	    glBindTexture(GL_TEXTURE_2D, silhouetteTexture1);
	    glEnable(GL_ALPHA_TEST);
	    glAlphaFunc(GL_GREATER, 0.0f);
	    glColor4ub(255,255,255,255);
	}
	glBegin(GL_QUADS);
	if (rhino.vel[0] > 0.0) {
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
    if (show_ufo) {
	glPushMatrix();
	glTranslatef(ufo.pos[0], ufo.pos[1], ufo.pos[2]);
	if (!silhouette2) {
	    glBindTexture(GL_TEXTURE_2D, ufoTexture);
	} else {
	    glBindTexture(GL_TEXTURE_2D, silhouetteTexture2);
	    glEnable(GL_ALPHA_TEST);
	    glAlphaFunc(GL_GREATER, 0.0f);
	    glColor4ub(255,255,255,255);
	}
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

    glDisable(GL_TEXTURE_2D);
    //glColor3f(1.0f, 0.0f, 0.0f);
    //glBegin(GL_QUADS);
    //	glVertex2i(10,10);
    //	glVertex2i(10,60);
    //	glVertex2i(60,60);
    //	glVertex2i(60,10);
    //glEnd();
    //return;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    //
#ifdef USE_UMBRELLA
    if (show_umbrella)
	draw_umbrella();
#endif //USE_UMBRELLA
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    //
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    unsigned int cref = 0x00ffffff;
    ggprint8b(&r, 16, cref, "K - Kangaroo");
    ggprint8b(&r, 16, cref, "B - Background");
    ggprint8b(&r, 16, cref, "S - Silhouette");
    ggprint8b(&r, 16, cref, "U - Umbrella");
    ggprint8b(&r, 16, cref, "R - Rhino");
    ggprint8b(&r, 16, cref, "D - Deflection");
    ggprint8b(&r, 16, cref, "N - Sounds");
    ggprint8b(&r, 16, cref, "A - Alien Abduction");
}

