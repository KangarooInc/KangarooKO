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
void jump();

int done=0;

typedef struct t_bigfoot {
	Vec pos;
	Vec vel;
} Bigfoot;
Bigfoot bigfoot;

Ppmimage *bigfootImage=NULL;
Ppmimage *forestImage=NULL;
Ppmimage *forestTransImage=NULL;
Ppmimage *umbrellaImage=NULL;
GLuint bigfootTexture;
GLuint silhouetteTexture;
GLuint forestTexture;
GLuint forestTransTexture;
GLuint umbrellaTexture;
int show_bigfoot=0;
int forest=1;
int silhouette=1;
int trees=1;
int jump_switch=0;
//int show_rain=0;
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
	bigfootImage     = ppm6GetImage("./images/kanga.ppm");
	forestImage      = ppm6GetImage("./images/background.ppm");
	forestTransImage = ppm6GetImage("./images/forestTrans.ppm");
	umbrellaImage    = ppm6GetImage("./images/kanga.ppm");
	//
	//create opengl texture elements
	glGenTextures(1, &bigfootTexture);
	glGenTextures(1, &silhouetteTexture);
	glGenTextures(1, &forestTexture);
	glGenTextures(1, &umbrellaTexture);
	//-------------------------------------------------------------------------
	//bigfoot
	//
	int w = bigfootImage->width;
	int h = bigfootImage->height;
	//
	glBindTexture(GL_TEXTURE_2D, bigfootTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
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
	unsigned char *silhouetteData = buildAlphaData(bigfootImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------
	//
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
	//forest
	glBindTexture(GL_TEXTURE_2D, forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
							forestImage->width, forestImage->height,
							0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);
	//-------------------------------------------------------------------------
	//
	//forest transparent part
	//
	glBindTexture(GL_TEXTURE_2D, forestTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = forestTransImage->width;
	h = forestTransImage->height;
	unsigned char *ftData = buildAlphaData(forestTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
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
	umbrella.pos[0] = 220.0;
	umbrella.pos[1] = (double)(yres-200);
	VecCopy(umbrella.pos, umbrella.lastpos);
	umbrella.width = 200.0;
	umbrella.width2 = umbrella.width * 0.5;
	umbrella.radius = (float)umbrella.width2;
	umbrella.shape = UMBRELLA_FLAT;
	#endif //USE_UMBRELLA
	MakeVector(-150.0,180.0,0.0, bigfoot.pos);
	MakeVector(6.0,0.0,0.0, bigfoot.vel);
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
	int i;
	switch(key) {
		case XK_b:
		    	trees ^= 1;
			show_bigfoot ^= 1;
			if (show_bigfoot) {
			   bigfoot.pos[0] = 750.0;
			   i = random(3);
			   if (i == 2)
			       bigfoot.pos[1] = 220.0;
			   if (i == 1)
			       bigfoot.pos[1] = 140.0;
			   else
			       bigfoot.pos[1] = 180.0;
			}
			break;
		case XK_d:
			deflection ^= 1;
			break;
		case XK_s:
			silhouette ^= 1;
			printf("silhouette: %i\n",silhouette);
			break;
		//case XK_t:
			//trees ^= 1;
			//break;
		case XK_u:
			show_umbrella ^= 1;
			//break;
		//case XK_p:
			umbrella.shape ^= 1;
			break;
		case XK_Left:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] -= 20.0;
			break;
		case XK_Right:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] += 20.0;
			break;
		//case XK_Up:
			//if (show_umbrella) 
			    //jump_switch ^= 1;
			//break;
		case XK_Up:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] += 10.0;
			break;
		case XK_Down:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] -= 10.0;
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

void jump()
{
    	int addgrav = 1;

    	umbrella.pos[1] += 6.0;

	if (addgrav)
	    umbrella.pos[1] -= 6.0;
}

void move_bigfoot()
{
	//move bigfoot...
	//int addgrav = 1;
	//Update position
	bigfoot.pos[0] -= bigfoot.vel[0];

	//bigfoot.pos[1] += bigfoot.vel[1];
	//Check for collision with window edges
	/*if ((bigfoot.pos[0] < -140.0 && bigfoot.vel[0] < 0.0) ||
				(bigfoot.pos[0] >= (float)xres+140.0 && bigfoot.vel[0] > 0.0)) {
		bigfoot.vel[0] = -bigfoot.vel[0];
		//bigfoot.vel[1] = -bigfoot.vel[1];
		addgrav = 0;
	}*/
	
	if (bigfoot.pos[0] < -140.0) {
	    trees ^= 1;
	    show_bigfoot ^= 1;
	    //bigfoot.vel[0] = -bigfoot.vel[0];
	    //addgrav = 0;
	}

	//Gravity
	//if (addgrav)
		//bigfoot.vel[1] -= 0.75;
}


void physics(void)
{
	if (show_bigfoot)
		move_bigfoot();
	if (jump_switch)
		jump();
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

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//
	//
	//draw a quad with texture
	float wid = 120.0f;
	glColor3f(1.0, 1.0, 1.0);
	if (forest) {
		glBindTexture(GL_TEXTURE_2D, forestTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
		glEnd();
	}
	if (show_bigfoot) {
		glPushMatrix();
		glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
		if (!silhouette) {
			glBindTexture(GL_TEXTURE_2D, bigfootTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}
		glBegin(GL_QUADS);
			if (bigfoot.vel[0] > 0.0) {
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
		//
		if (trees && silhouette) {
			glBindTexture(GL_TEXTURE_2D, forestTransTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
			glEnd();
		}
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
	//if (show_rain)
		//draw_raindrops();
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
	ggprint8b(&r, 16, cref, "B - Bigfoot");
	ggprint8b(&r, 16, cref, "F - Forest");
	ggprint8b(&r, 16, cref, "S - Silhouette");
	//ggprint8b(&r, 16, cref, "T - Trees");
	ggprint8b(&r, 16, cref, "U - Umbrella");
	ggprint8b(&r, 16, cref, "R - Rain");
	ggprint8b(&r, 16, cref, "D - Deflection");
	ggprint8b(&r, 16, cref, "N - Sounds");
}

